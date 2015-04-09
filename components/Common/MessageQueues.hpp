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
#ifndef FLEXUS_COMMON_MESSAGE_QUEUES_HPP_INCLUDED
#define FLEXUS_COMMON_MESSAGE_QUEUES_HPP_INCLUDED

#include <list>

#include <core/stats.hpp>
#include <components/Common/Transports/MemoryTransport.hpp>

#define DBG_DeclareCategories CommonQueues
#define DBG_SetDefaultOps AddCat(CommonQueues)
#include DBG_Control()

namespace nMessageQueues {

using namespace Flexus;
using namespace Core;
using namespace SharedTypes;

template <class Transport>
class MessageQueue {
  std::list< std::pair<Transport, int64_t> > theQueue;
  uint32_t theSize;
  uint32_t theCurrentUsage;
  uint32_t theCurrentReserve;

public:
  MessageQueue( )
    : theSize(1)
    , theCurrentUsage(0)
    , theCurrentReserve(0)
  {}
  MessageQueue( uint32_t aSize )
    : theSize(aSize)
    , theCurrentUsage(0)
    , theCurrentReserve(0)
  {}

  void setSize( uint32_t aSize ) {
    theSize = aSize;
  }

  void enqueue(Transport const & aMessage) {
    theQueue.push_back( std::make_pair(aMessage, Flexus::Core::theFlexus->cycleCount()) );
    ++theCurrentUsage;
    ++theCurrentReserve;
    DBG_Assert( theCurrentReserve >= theCurrentUsage, ( << theCurrentReserve << "/" << theCurrentUsage << "/" << theSize << " when enqueuing " << *(aMessage[MemoryMessageTag]) ) );
    DBG_Assert( theCurrentReserve <= theSize, ( << theCurrentReserve << "/" << theCurrentUsage << "/" << theSize << " when enqueuing " << *(aMessage[MemoryMessageTag]) ) );
  }

  Transport dequeue() {
    DBG_Assert( ! theQueue.empty() );
    Transport  ret_val(theQueue.front().first);
    theQueue.pop_front();
    --theCurrentUsage;
    --theCurrentReserve;
    DBG_Assert( theCurrentReserve >= theCurrentUsage );
    DBG_Assert( theCurrentUsage >= 0 );
    return ret_val;
  }

  const Transport & peek() {
    return theQueue.front().first;
  }

  bool hasSpace(uint32_t msgs) const {
    return (theCurrentReserve + msgs) <= theSize;
  }

  int64_t headTimestamp() const {
    DBG_Assert( ! theQueue.empty() );
    return theQueue.front().second;
  }

  bool empty() const {
    return theQueue.empty();
  }
  bool full(uint32_t anAdditionalReserve = 0) const {
    return theCurrentReserve + anAdditionalReserve >= theSize;
  }

  void reserve() {
    ++theCurrentReserve;
    DBG_Assert( theCurrentReserve >= theCurrentUsage );
    DBG_Assert( theCurrentReserve <= theSize );
  }

  void reserve(int32_t n) {
    theCurrentReserve += n;
    DBG_Assert( theCurrentReserve >= theCurrentUsage );
    DBG_Assert( theCurrentReserve <= theSize );
  }

  void unreserve() {
    --theCurrentReserve;
    DBG_Assert( theCurrentReserve >= theCurrentUsage );
    DBG_Assert( theCurrentReserve >= 0);
  }
  void unreserve(int32_t n) {
    theCurrentReserve -= n;
    DBG_Assert( theCurrentReserve >= theCurrentUsage );
    DBG_Assert( theCurrentReserve >= 0);
  }

  int32_t getReserve() {
    return theCurrentReserve - theCurrentUsage;
  }
  int32_t getSize() {
    return theCurrentUsage;
  }
  int32_t getFreeSpace() {
    return (theSize - theCurrentReserve);
  }

};  // class MessageQueue

typedef uint64_t CycleTime;

template< class Item >
class DelayFifo {
  typedef std::pair<Item, CycleTime> DelayElement;
  std::list<DelayElement> theQueue;
  uint32_t theSize;
  uint32_t theCurrentSize;

public:
  DelayFifo( )
    : theSize(1)
    , theCurrentSize(0)
  {}
  DelayFifo( uint32_t aSize )
    : theSize(aSize)
    , theCurrentSize(0)
  {}

  void setSize( uint32_t aSize ) {
    theSize = aSize;
  }

  void enqueue(Item anItem, uint32_t delay) {
    CycleTime ready = Flexus::Core::theFlexus->cycleCount() + delay;
    theQueue.push_back( std::make_pair(anItem, ready) );
    ++theCurrentSize;
  }

  bool ready() {
    if (theCurrentSize == 0) {
      return false;
    }
    return( Flexus::Core::theFlexus->cycleCount() >= theQueue.front().second );
  }

  Item dequeue() {
    // remove and remember the head of the queue
    Item ret_val(theQueue.front().first);
    theQueue.pop_front();
    --theCurrentSize;
    return ret_val;
  }

  Item & peek() {
    // remove and remember the head of the queue
    return theQueue.front().first;
  }

  bool empty() const {
    return theCurrentSize == 0;
  }
  bool full() const {
    return theCurrentSize >= theSize;
  }
  uint32_t size() const {
    return theCurrentSize;
  }

};  // class DelayFifo

template< class Item >
class PipelineFifo {
  typedef std::pair<Item, uint64_t> PipelineElement;
  std::list<PipelineElement> theQueue;
  std::list<uint64_t> theServerReadyTimes;
  uint32_t theCurrentSize;
  int64_t theIssueLatency;
  int64_t theLatency;

  uint64_t theLastArrival;
  boost::intrusive_ptr<Stat::StatLog2Histogram> theInterArrival;

public:
  PipelineFifo( std::string aName,
                uint32_t aNumPipelines,
                int64_t anIssueLatency,
                int64_t aLatency,
                boost::intrusive_ptr<Stat::StatLog2Histogram> anInterArrival = NULL )
    : theCurrentSize(0)
    , theIssueLatency(anIssueLatency)
    , theLatency(aLatency)
    , theLastArrival(0)
    , theInterArrival(anInterArrival) {
    DBG_Assert( aNumPipelines > 0);
    DBG_Assert( theIssueLatency >= 1);
    for (uint32_t i = 0; i < aNumPipelines; ++i) {
      theServerReadyTimes.push_back( 0 );
    }

    if ( anInterArrival == NULL ) {
      theInterArrival = new Stat::StatLog2Histogram(aName + "-InterArrivalTimes");
    }
  }

  // Enqueue an item for one or more non-overlapping accesses to the same resource
  void enqueue(Item anItem, const uint32_t aRepeatCount = 1) {
    uint64_t curr = Flexus::Core::theFlexus->cycleCount();
    DBG_Assert( theServerReadyTimes.front() <= curr, ( << " front=" << theServerReadyTimes.front() << " curr=" << curr) );
    for ( uint32_t i = 0; i < aRepeatCount; i++ ) {
      theServerReadyTimes.pop_front();
      theServerReadyTimes.push_back(curr + theIssueLatency * (i + 1) );
    }
    uint64_t complete = curr + theLatency * aRepeatCount;
    theQueue.push_back( std::make_pair(anItem, complete) );
    ++theCurrentSize;
    if ( theInterArrival )
      *theInterArrival << (curr - theLastArrival);
    theLastArrival = curr;
  }

  bool ready() {
    if (theCurrentSize == 0) {
      return false;
    }
    return( Flexus::Core::theFlexus->cycleCount() >= theQueue.front().second );
  }

  Item dequeue() {
    --theCurrentSize;
    DBG_Assert( theCurrentSize >= 0 );
    // remove and remember the head of the queue
    Item ret_val(theQueue.front().first);
    theQueue.pop_front();
    return ret_val;
  }

  Item peek() {
    return theQueue.front().first;
  }

  void stall() {
    std::list<uint64_t>::iterator iter = theServerReadyTimes.begin();
    std::list<uint64_t>::iterator end = theServerReadyTimes.end();
    while (iter != end) {
      ++(*iter);
      ++iter;
    }
  }
  bool serverAvail() const {
    return theServerReadyTimes.front() <= Flexus::Core::theFlexus->cycleCount();
  }
  bool empty() const {
    return theCurrentSize == 0;
  }
  uint32_t size() const {
    return theCurrentSize;
  }

};  // class PipelineFifo

} //End Namespace nMessageQueues

#define DBG_Reset
#include DBG_Control()

#endif //FLEXUS_COMMON_MESSAGE_QUEUES_HPP_INCLUDED

