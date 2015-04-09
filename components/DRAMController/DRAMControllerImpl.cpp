// DO-NOT-REMOVE begin-copyright-block 
//
// Redistributions of any form whatsoever must retain and/or include the
// following acknowledgment, notices and disclaimer:
//
// This product includes software developed by Carnegie Mellon University.
//
// Copyright 2012 by Mohammad Alisafaee, Eric Chung, Michael Ferdman, Brian 
// Gold, Jangwoo Kim, Pejman Lotfi-Kamran, Onur Kocberber, Djordje Jevdjic, 
// Jared Smolens, Stephen Somogyi, Evangelos Vlachos, Stavros Volos, Jason 
// Zebchuk, Babak Falsafi, Nikos Hardavellas and Tom Wenisch for the SimFlex 
// Project, Computer Architecture Lab at Carnegie Mellon, Carnegie Mellon University.
//
// For more information, see the SimFlex project website at:
//   http://www.ece.cmu.edu/~simflex
//
// You may not use the name "Carnegie Mellon University" or derivations
// thereof to endorse or promote products derived from this software.
//
// If you modify the software you must place a notice on or within any
// modified version provided or made available to any third party stating
// that you have modified the software.  The notice shall include at least
// your name, address, phone number, email address and the date and purpose
// of the modification.
//
// THE SOFTWARE IS PROVIDED "AS-IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER
// EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO ANY WARRANTY
// THAT THE SOFTWARE WILL CONFORM TO SPECIFICATIONS OR BE ERROR-FREE AND ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
// TITLE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
// BE LIABLE FOR ANY DAMAGES, INCLUDING BUT NOT LIMITED TO DIRECT, INDIRECT,
// SPECIAL OR CONSEQUENTIAL DAMAGES, ARISING OUT OF, RESULTING FROM, OR IN
// ANY WAY CONNECTED WITH THIS SOFTWARE (WHETHER OR NOT BASED UPON WARRANTY,
// CONTRACT, TORT OR OTHERWISE).
//
// DO-NOT-REMOVE end-copyright-block   
#include <components/DRAMController/DRAMController.hpp>
#include <components/DRAMController/MemStats.hpp>
#include <components/DRAMController/MessageMap.hpp>

#include <components/Common/MessageQueues.hpp>

#define FLEXUS_BEGIN_COMPONENT DRAMController
#include FLEXUS_BEGIN_COMPONENT_IMPLEMENTATION()

#define DBG_DefineCategories Memory
#define DBG_SetDefaultOps AddCat(Memory)
#include DBG_Control()

#include <components/Common/MemoryMap.hpp>
#include <components/Common/Slices/MemoryMessage.hpp>
#include <components/Common/Slices/TransactionTracker.hpp>
#include <components/Common/Slices/ExecuteState.hpp>
#include <core/simics/mai_api.hpp>

#define DRAM_TEST

namespace DRAMSim{

using namespace Flexus;
using namespace Simics;
using namespace Core;
using namespace SharedTypes;


using Flexus::SharedTypes::MemoryMap;

using boost::intrusive_ptr;


void power_callback(double a, double b, double c, double d){}

void alignTransactionAddress(Transaction &trans)
{
        // zero out the low order bits which correspond to the size of a transaction
        unsigned chunk=BL*JEDEC_DATA_BUS_BITS/8;
        unsigned throwAwayBits = dramsim_log2(chunk);
        uint64_t mask=(1L<<throwAwayBits) - 1;
        uint64_t new_address=trans.address & (~mask); 
        trans.address >>= throwAwayBits;
        trans.address <<= throwAwayBits;
        DBG_Assert(trans.address==new_address);
}

class FLEXUS_COMPONENT(DRAMController) {
  FLEXUS_COMPONENT_IMPL(DRAMController);

  boost::intrusive_ptr<MemoryMap> theMemoryMap;

  MemoryMessage::MemoryMessageType theFetchReplyType;
 
  MultiChannelMemorySystem* mem; 
 
  MemStats* theStats;
   
  unsigned size; //size in MB; 

  unsigned clockCounter;
  unsigned clockFudge;
  unsigned clockThreshold;
  unsigned interleaving; //address interleaving (in bytes) among memory controllers

public:
  FLEXUS_COMPONENT_CONSTRUCTOR(DRAMController)
    : base( FLEXUS_PASS_CONSTRUCTOR_ARGS )
  {}

  bool isQuiesced() const {
    if (! outQueue) {
      return true; //Quiesced before initialization
    }
    return (outQueue->empty() && pendingList->empty() && evictedList->empty());
  }

uint64_t translateAddress(uint64_t address){
  uint64_t rest = address % interleaving;
  uint64_t base = address - rest;
  return rest+ ((base-flexusIndex()*interleaving)/flexusWidth());
 // return address;
}

 void read_complete(unsigned id, uint64_t address, uint64_t clock_cycle){
   DBG_Assert(!outQueue->full());   
   int64_t delay=0;
   MemoryMessage::MemoryMessageType aType;
   if(pendingList->isPending(address, aType)){
       TimestampedTransport trans(pendingList->remove(address));
       outQueue->enqueue(trans);
       delay=theFlexus->cycleCount()-trans.cycle;
   }
   else if(evictedList->isPending(address, aType)){ 
       TimestampedTransport trans(evictedList->remove(address));
       outQueue->enqueue(trans);
       DBG_Assert((aType!=MemoryMessage::EvictDirty) && (aType!=MemoryMessage::Flush) && (aType!=MemoryMessage::FlushReq)); 
       delay=theFlexus->cycleCount()-trans.cycle;
  }
   else DBG_Assert(false, (<<"No address "<<address<<" found among the pending transactions"));
  DBG_Assert(delay>0);
  theStats->Latency_Histogram << std::make_pair(delay, 1);
  theStats->updateMinLatency(delay);
  theStats->updateMaxLatency(delay);
}

 void write_complete(unsigned id, uint64_t address, uint64_t clock_cycle){
   DBG_Assert(!outQueue->full());
   int64_t delay=0;
   MemoryMessage::MemoryMessageType aType;
   if(pendingList->isPending(address, aType)){
       TimestampedTransport trans(pendingList->remove(address));
       if((aType!=MemoryMessage::EvictDirty) && (aType!=MemoryMessage::FlushReq) && (aType!=MemoryMessage::Flush))  outQueue->enqueue(trans);
       delay=theFlexus->cycleCount()-trans.cycle;
   }
   else if(evictedList->isPending(address, aType)){
       TimestampedTransport trans(evictedList->remove(address));
       DBG_Assert((aType!=MemoryMessage::EvictDirty) && (aType!=MemoryMessage::FlushReq) && (aType!=MemoryMessage::Flush));
       outQueue->enqueue(trans);
       delay=theFlexus->cycleCount()-trans.cycle;
   }
   else DBG_Assert(false, (<<"No address "<<address<<" found among the pending transactions"));
   DBG_Assert(delay>0);
   theStats->Latency_Histogram << std::make_pair(delay, 1);
   theStats->updateMinLatency(delay);
   theStats->updateMaxLatency(delay);
}

  // Initialization
  void initialize() {
    if (cfg.MaxRequests<cfg.MaxReplies) {
      std::cout << "Error: more requests than replies." << std::endl;
      throw FlexusException();
    }
   std::string str1=name();
   str1.append(boost::lexical_cast<string>(flexusIndex()));
   theStats=new MemStats(str1);
 
    theMemoryMap = MemoryMap::getMemoryMap(flexusIndex());

    outQueue.reset(new nMessageQueues::MessageQueue< TimestampedTransport >(cfg.MaxReplies));
    pendingList.reset(new DRAMSim::MessageMap(cfg.MaxRequests)); 
    evictedList.reset(new DRAMSim::MessageMap(cfg.MaxRequests)); 

    if (cfg.UseFetchReply) {
      theFetchReplyType = MemoryMessage::FetchReply;
    } else {
      theFetchReplyType = MemoryMessage::MissReplyWritable;
    }
 
    interleaving = cfg.Interleaving;

    if(flexusIndex()==0) DBG_(Crit, (<<"System memory" << Processor::getProcessor(0)->getMemorySizeInMB()));

    size=cfg.Size;
    if(cfg.DynamicSize) size=Processor::getProcessor(0)->getMemorySizeInMB();
    DBG_Assert(size>0,(<<"Incorrectly read the memory size from the simics file"));
    size=size/flexusWidth();     

    mem = new MultiChannelMemorySystem(cfg.DeviceFile, cfg.MemorySystemFile, ".", "dram_results", size);

    Callback_t *read_cb = new Callback<  BOOST_PP_CAT(DRAMController,Component), void, unsigned, uint64_t, uint64_t>(this, &BOOST_PP_CAT(DRAMController,Component)::read_complete);
    Callback_t *write_cb = new Callback< BOOST_PP_CAT(DRAMController,Component), void, unsigned, uint64_t, uint64_t>(this, &BOOST_PP_CAT(DRAMController,Component)::write_complete);
    mem->RegisterCallbacks(read_cb, write_cb, power_callback );

    double clockCycle=1000.0/cfg.Frequency;
    double ratio = tCK/clockCycle;
    DBG_Assert(ratio>=1,(<<"Memory frequency higher than the CPU frequency!"));
    int ratio_int=(int)ratio;
    if(ratio-(double)ratio_int ==0.0){
      //frequencies are nicely aligned: i.e., CPU frequency is a multiple of th bus frequency
      //we will clock memory once every {ratio} cycles
      clockFudge=ratio_int;
      clockThreshold=0;
    }
    else{
      //approximation: we will clock memory 20 times for every  20*{ratio} cycles
      clockFudge = (unsigned) (ratio*20.0);
      clockThreshold=19;
    }

    DBG_Assert(ratio>=1,(<<"Memory frequency higher than the CPU frequency!"));
    clockCounter = 0;

  }

  void fillTracker(MemoryTransport & aMessageTransport) {
    if (aMessageTransport[TransactionTrackerTag]) {
      aMessageTransport[TransactionTrackerTag]->setFillLevel(eLocalMem);
      if (!aMessageTransport[TransactionTrackerTag]->fillType() ) {
        aMessageTransport[TransactionTrackerTag]->setFillType(eReplacement);
      }
      aMessageTransport[TransactionTrackerTag]->setResponder(flexusIndex());
      aMessageTransport[TransactionTrackerTag]->setNetworkTrafficRequired(false);
    }
  }



  //LoopBackIn PushInput Port
  //=========================
  bool available(interface::LoopbackIn const &) {
   bool isAvailable = mem->willAcceptTransaction();
   if(isAvailable) DBG_Assert(!pendingList->full(), Component(*this) ( << "No more space in the pending list!") ); 
   return isAvailable; 
 }

#ifdef DRAM_TEST
 void printMessage(MemoryTransport & aMessage){
            std::string str="/home/jevdjic/dramvalid/oracle_NAS";
            str.append(boost::lexical_cast<string>(flexusIndex()));
            str.append(".txt");
            char* cstr = new char [str.size()+1];
            strcpy (cstr, str.c_str());
            ofstream output;
            output.open(cstr, ios::out | ios::app);

            output<< "cycle: "<< theFlexus->cycleCount();
            output<< " | Address: "<<hex<< (aMessage[MemoryMessageTag]->address());
            output<< " | type : "<< aMessage[MemoryMessageTag]->type()<<std::endl;
            output.close();
      }

 void printTransaction(Transaction & t){
            std::string str="/home/jevdjic/dramvalid/oracle_NAS";
             
            str.append(boost::lexical_cast<string>(flexusIndex()));
            str.append(".txt");
            char* cstr = new char [str.size()+1];
            strcpy (cstr, str.c_str());
            ofstream output;
            output.open(cstr, ios::out | ios::app);

            output<< "cycle: "<< t.timeAdded;
            output<< " | Address: "<<hex<<t.address;
            output<< " | type : "<< t.transactionType<<std::endl;
            output.close();
          }

#endif

void push(interface::LoopbackIn const &, MemoryTransport & aMessageTransport) {
	DBG_(Trace, Comp(*this) ( << "request received: " << *aMessageTransport[MemoryMessageTag]) Addr(aMessageTransport[MemoryMessageTag]->address()) );
        bool skip=false;
	intrusive_ptr<MemoryMessage> reply;
	TransactionType operation=DATA_READ;
	switch (aMessageTransport[MemoryMessageTag]->type()) {
		case MemoryMessage::LoadReq:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Read);
			reply = new MemoryMessage(MemoryMessage::LoadReply, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 64;
			fillTracker(aMessageTransport);
			operation=DATA_READ;
                        theStats->MemReads++;
			break;
		case MemoryMessage::FetchReq:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Read);
			reply = new MemoryMessage(theFetchReplyType, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 64;
			fillTracker(aMessageTransport);
			operation=DATA_READ;
			theStats->MemReads++;
			break;
		case MemoryMessage::StoreReq:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Write);
			reply = new MemoryMessage(MemoryMessage::StoreReply, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 0;
			fillTracker(aMessageTransport);
			operation=DATA_WRITE;
			theStats->MemWrites++;
			break;
		case MemoryMessage::StorePrefetchReq:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Write);
			reply = new MemoryMessage(MemoryMessage::StorePrefetchReply, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 0;
			fillTracker(aMessageTransport);
			operation=DATA_WRITE;
                        theStats->MemWrites++;
			break;
		case MemoryMessage::CmpxReq:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Write);
			reply = new MemoryMessage(MemoryMessage::CmpxReply, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 64;
			fillTracker(aMessageTransport);
			operation=DATA_WRITE;
                        theStats->MemWrites++;
			break;

		case MemoryMessage::ReadReq:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Read);
			reply = new MemoryMessage(MemoryMessage::MissReplyWritable, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 64;
			fillTracker(aMessageTransport);
			operation=DATA_READ;
                        theStats->MemReads++;
			break;
		case MemoryMessage::WriteReq:
		case MemoryMessage::WriteAllocate:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Write);
			reply = new MemoryMessage(MemoryMessage::MissReplyWritable, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 64;
			fillTracker(aMessageTransport);
			operation=DATA_WRITE;
                        theStats->MemWrites++;
			break;
		case MemoryMessage::NonAllocatingStoreReq:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Write);
			// reply = aMessageTransport[MemoryMessageTag];
			// reply->type() = MemoryMessage::NonAllocatingStoreReply;
			// make a new msg just loks ALL the other msg types
			reply = new MemoryMessage(MemoryMessage::NonAllocatingStoreReply, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = aMessageTransport[MemoryMessageTag]->reqSize();
			fillTracker(aMessageTransport);
			operation=DATA_WRITE;
                        theStats->MemNAS++;
			break;

		case MemoryMessage::UpgradeReq:
		case MemoryMessage::UpgradeAllocate:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Write);
			reply = new MemoryMessage(MemoryMessage::UpgradeReply, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 0;
			fillTracker(aMessageTransport);
			operation=DATA_WRITE;
                        theStats->MemWrites++;
			break;
		case MemoryMessage::FlushReq:
		case MemoryMessage::Flush:
		case MemoryMessage::EvictDirty:
                        // no reply required
                        if (aMessageTransport[TransactionTrackerTag]) {
                                aMessageTransport[TransactionTrackerTag]->setFillLevel(eLocalMem);
                                aMessageTransport[TransactionTrackerTag]->setFillType(eReplacement);
                                aMessageTransport[TransactionTrackerTag]->complete();
                        }
                        operation=DATA_WRITE;
                        theStats->MemEvictions++;
                        if (aMessageTransport[MemoryMessageTag]->ackRequired()) {
                           reply = new MemoryMessage(MemoryMessage::EvictAck, aMessageTransport[MemoryMessageTag]->address());
                           reply->reqSize() = 0;
                        } else {
                           skip=true; 
                        } 
                        break;
		case MemoryMessage::EvictWritable:
		case MemoryMessage::EvictClean:
			// no reply required
			if (aMessageTransport[TransactionTrackerTag]) {
				aMessageTransport[TransactionTrackerTag]->setFillLevel(eLocalMem);
				aMessageTransport[TransactionTrackerTag]->setFillType(eReplacement);
				aMessageTransport[TransactionTrackerTag]->complete();
			}
			return;
		case MemoryMessage::PrefetchReadAllocReq:
		case MemoryMessage::PrefetchReadNoAllocReq:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Read);
			reply = new MemoryMessage(MemoryMessage::PrefetchWritableReply, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 64;
			fillTracker(aMessageTransport);
			operation=DATA_READ;
                        theStats->MemReads++;
			break;
		case MemoryMessage::StreamFetch:
			theMemoryMap->recordAccess( aMessageTransport[MemoryMessageTag]->address(), MemoryMap::Read);
			reply = new MemoryMessage(MemoryMessage::StreamFetchWritableReply, aMessageTransport[MemoryMessageTag]->address());
			reply->reqSize() = 64;
			fillTracker(aMessageTransport);
			operation=DATA_READ;
                        theStats->MemReads++;
			break;
		case MemoryMessage::PrefetchInsert:
			// should never happen
			DBG_Assert(false, Component(*this) ( << "DRAMController received PrefetchInsert request") );
                        break;
                case MemoryMessage::PrefetchInsertWritable:
               // should never happen
                        DBG_Assert(false, Component(*this) ( << "DRAMController received PrefetchInsertWritable request") );
                        break;
                default:
                        DBG_Assert(false, Component(*this) ( << "Don't know how to handle message: " << aMessageTransport[MemoryMessageTag]->type() << "  No reply sent." ) );
                        return;
          }
  
#ifdef DRAM_TEST  
  if(aMessageTransport[MemoryMessageTag]->type()==MemoryMessage::NonAllocatingStoreReq)   printMessage(aMessageTransport);
#endif
   
 if(!skip) {
      DBG_(VVerb, Comp(*this) ( << "Queing reply: " << *reply) Addr(aMessageTransport[MemoryMessageTag]->address()) );
      aMessageTransport.set(MemoryMessageTag, reply);
    }
  
    uint64_t address= aMessageTransport[MemoryMessageTag]->address(); 
    address=translateAddress(address);
    Transaction t = Transaction(operation, address, NULL);
    alignTransactionAddress(t);
    mem->addTransaction(t);
    MemoryMessage::MemoryMessageType aType=MemoryMessage::EvictDirty;
    bool alreadyExists = pendingList->isPending(t.address, aType);
    TimestampedTransport trans(aMessageTransport,theFlexus->cycleCount());
    if(alreadyExists) {
      DBG_Assert((aType==MemoryMessage::EvictDirty) ||(aType==MemoryMessage::FlushReq) || (aType==MemoryMessage::Flush) );
      evictedList->insert(trans, t.address);    
    } 
    else pendingList->insert(trans, t.address);

    unsigned long size_in_bytes=size*1024L*1024L;
   if (address>=size_in_bytes) DBG_(Crit, (<<"In memory controller "<<flexusIndex()<<" address "<<address<<" is bigger than size "<<size_in_bytes));

  }

  //Drive Interfaces
  void drive(interface::DRAMDrive const &) {
    if (clockCounter <=clockThreshold) mem->update();
    clockCounter=(clockCounter+1)%clockFudge;
    if (!outQueue->empty() && !FLEXUS_CHANNEL(LoopbackOut).available()) {
      DBG_(Trace, Comp(*this) ( << "Faile to send reply, channel not available." ));
    }
    while (FLEXUS_CHANNEL(LoopbackOut).available() && !outQueue->empty()) {
      TimestampedTransport trans(outQueue->dequeue());
      DBG_(Trace, Comp(*this) ( << "Sending reply: " << *(trans[MemoryMessageTag]) ) Addr(trans[MemoryMessageTag]->address()) );
      FLEXUS_CHANNEL(LoopbackOut) << trans;
    }
  }

void finalize() {}

private:
 boost::scoped_ptr< nMessageQueues::MessageQueue< TimestampedTransport > > outQueue;
 boost::scoped_ptr< DRAMSim::MessageMap> pendingList;
 boost::scoped_ptr< DRAMSim::MessageMap> evictedList; 
};

} //End Namespace DRAMSim

FLEXUS_COMPONENT_INSTANTIATOR( DRAMController, DRAMSim );

#include FLEXUS_END_COMPONENT_IMPLEMENTATION()
#define FLEXUS_END_COMPONENT DRAMController

#define DBG_Reset
#include DBG_Control()

