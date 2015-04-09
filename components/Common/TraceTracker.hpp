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
#ifndef _TRACE_TRACKER_HPP_
#define _TRACE_TRACKER_HPP_

#include <map>

#include <core/boost_extensions/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <core/target.hpp>
#include <core/types.hpp>
#include <core/stats.hpp>

#include <components/Common/Slices/TransactionTracker.hpp>

#include DBG_Control()

namespace nTraceTracker {


typedef uint32_t address_t;


class TraceTracker {


public:
  void access      (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t addr, address_t pc, bool prefetched, bool write, bool miss, bool priv, uint64_t ltime);
  void commit      (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t addr, address_t pc, uint64_t ltime);
  void store       (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t addr, address_t pc, bool miss, bool priv, uint64_t ltime);
  void prefetch    (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block);
  void fill        (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block, Flexus::SharedTypes::tFillLevel fillLevel, bool isFetch, bool isWrite);
  void prefetchFill(int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block, Flexus::SharedTypes::tFillLevel fillLevel);
  void prefetchHit (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block, bool isWrite);
  void prefetchRedundant(int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block);
  void insert      (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block);
  void eviction    (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block, bool drop);
  void invalidation(int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block);
  void invalidAck  (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block);
  void invalidTagCreate (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block);
  void invalidTagRefill (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block);
  void invalidTagReplace(int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block);

  void accessLoad  (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block, uint32_t offset, int32_t size);
  void accessStore (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block, uint32_t offset, int32_t size);
  void accessFetch (int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block, uint32_t offset, int32_t size);
  void accessAtomic(int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block, uint32_t offset, int32_t size);

  TraceTracker();
  void initialize();
  void finalize();


};

} // namespace nTraceTracker

extern nTraceTracker::TraceTracker theTraceTracker;

#endif
