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
#ifndef FLEXUS_DRAMCONTROLLER_MESSAGE_MAP_HPP_INCLUDED
#define FLEXUS_DRAMCONTROLLER_MESSAGE_MAP_HPP_INCLUDED

#include <list>
#include <map>

#include <core/stats.hpp>
#include <components/Common/Transports/MemoryTransport.hpp>

#define DBG_DeclareCategories CommonQueues
#define DBG_SetDefaultOps AddCat(CommonQueues)
#include DBG_Control()

namespace DRAMSim {

using namespace Flexus;
using namespace Core;
using namespace SharedTypes;

#define DRAM_TEST

class TimestampedTransport: public MemoryTransport{
 public:
  uint64_t cycle;
  TimestampedTransport(){
  }
  TimestampedTransport(const MemoryTransport &trans, uint64_t aCycle): MemoryTransport(trans), cycle(aCycle)
  {};
  TimestampedTransport(const TimestampedTransport& trans):MemoryTransport(trans) {
    cycle=trans.cycle;
  }
  TimestampedTransport & operator = (const TimestampedTransport& trans) {
    if (this != &trans) {
        this->MemoryTransport::operator=(trans);
        cycle=trans.cycle;
    }
    return *this;  
  }
};

class MessageMap {

  std::map<uint64_t, TimestampedTransport > theQueue;
  uint32_t theSize;
  uint32_t theCurrentUsage;

public:
  MessageMap(uint32_t aSize)
    : theSize(aSize)
    , theCurrentUsage(0)
  {}

#ifdef DRAM_TEST
void print(TimestampedTransport const & aMessage){
            std::string str="/home/jevdjic/dramvalid/oracle";
            str.append(".txt");
            char* cstr = new char [str.size()+1];
            strcpy (cstr, str.c_str());
            ofstream output;
            output.open(cstr, ios::out | ios::app);

            output<< "cycle: "<< theFlexus->cycleCount();
            output<< " | Address: "<<hex<< aMessage[MemoryMessageTag]->address();
            output<< " | type : "<< aMessage[MemoryMessageTag]->type()<<std::endl;
            output.close();
  }
#endif

  void insert(const TimestampedTransport & aMessage, uint64_t address) {
    DBG_Assert(theCurrentUsage<theSize,(<<"Not enough space in the list"));
    DBG_Assert(theQueue.count(address)==0,(<<"A transaction with the same address is already in the list!"));
    theQueue[address]=aMessage;
    ++theCurrentUsage;
  }

  TimestampedTransport remove(uint64_t address) {
    DBG_Assert(theCurrentUsage>0,(<<"Empty list"));
    DBG_Assert(theQueue.count(address)!=0,(<<"Address is NOT in the list!"));
    TimestampedTransport trans(theQueue[address]);
    theQueue.erase(address);    
    --theCurrentUsage;
    return trans;
  }
  
 bool empty() {
  return theCurrentUsage==0;
 }

 bool isPending(uint64_t address, MemoryMessage::MemoryMessageType &type){
   if(theQueue.count(address)>0) {
     type=theQueue[address][MemoryMessageTag]->type();
     return true;
   }
   return false;
 } 

 bool full(){
  return theSize==theCurrentUsage;
 }
};
} //namespace

#define DBG_Reset
#include DBG_Control()

#endif
