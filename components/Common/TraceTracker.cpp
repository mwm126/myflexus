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
#include "TraceTracker.hpp"
#include <core/boost_extensions/padded_string_cast.hpp>

#define DBG_DefineCategories TraceTrack
#define DBG_SetDefaultOps AddCat(TraceTrack)
#include DBG_Control()

using namespace Flexus;
using namespace Core;

#define MyLevel Iface

#define LOG2(x)         \
  ((x)==1 ? 0 :         \
  ((x)==2 ? 1 :         \
  ((x)==4 ? 2 :         \
  ((x)==8 ? 3 :         \
  ((x)==16 ? 4 :        \
  ((x)==32 ? 5 :        \
  ((x)==64 ? 6 :        \
  ((x)==128 ? 7 :       \
  ((x)==256 ? 8 :       \
  ((x)==512 ? 9 :       \
  ((x)==1024 ? 10 :     \
  ((x)==2048 ? 11 :     \
  ((x)==4096 ? 12 :     \
  ((x)==8192 ? 13 :     \
  ((x)==16384 ? 14 :    \
  ((x)==32768 ? 15 :    \
  ((x)==65536 ? 16 :    \
  ((x)==131072 ? 17 :   \
  ((x)==262144 ? 18 :   \
  ((x)==524288 ? 19 :   \
  ((x)==1048576 ? 20 :  \
  ((x)==2097152 ? 21 :  \
  ((x)==4194304 ? 22 :  \
  ((x)==8388608 ? 23 :  \
  ((x)==16777216 ? 24 : \
  ((x)==33554432 ? 25 : \
  ((x)==67108864 ? 26 : -0xffff)))))))))))))))))))))))))))

#define MIN(a,b) ((a) < (b) ? (a) : (b))

namespace nTraceTracker {


void TraceTracker::access(int32_t aNode, SharedTypes::tFillLevel cache, address_t addr, address_t pc,
                          bool prefetched, bool write, bool miss, bool priv, uint64_t ltime) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] access 0x" << std::hex << addr));
  //DBG_(Dev, (<< "[" << aNode << ":" << cache << "] access 0x" << std::hex << addr << " (ts:" << ltime <<")"));
}

void TraceTracker::commit(int32_t aNode, SharedTypes::tFillLevel cache, address_t addr, address_t pc, uint64_t aLogicalTime) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] commit 0x" << std::hex << addr));
}

void TraceTracker::store(int32_t aNode, SharedTypes::tFillLevel cache, address_t addr, address_t pc,
                         bool miss, bool priv, uint64_t aLogicalTime) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] store 0x" << std::hex << addr));
}

void TraceTracker::prefetch(int32_t aNode, SharedTypes::tFillLevel cache, address_t block) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] prefetch 0x" << std::hex << block));
}


void TraceTracker::fill(int32_t aNode, SharedTypes::tFillLevel cache, address_t block, SharedTypes::tFillLevel fillLevel, bool isFetch, bool isWrite) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] fill 0x" << std::hex << block));
  DBG_Assert(fillLevel != SharedTypes::eUnknown);
}

void TraceTracker::prefetchFill(int32_t aNode, SharedTypes::tFillLevel cache, address_t block, SharedTypes::tFillLevel fillLevel) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] prefetch fill 0x" << std::hex << block));
  DBG_Assert(fillLevel != SharedTypes::eUnknown);
}

void TraceTracker::prefetchHit(int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block, bool isWrite) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] prefetch hit 0x" << std::hex << block));
}

void TraceTracker::prefetchRedundant(int32_t aNode, Flexus::SharedTypes::tFillLevel cache, address_t block) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] prefetch redundant 0x" << std::hex << block));
}


void TraceTracker::insert(int32_t aNode, SharedTypes::tFillLevel cache, address_t block) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] insert 0x" << std::hex << block));
}

void TraceTracker::eviction(int32_t aNode, SharedTypes::tFillLevel cache, address_t block, bool drop) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] evict 0x" << std::hex << block));
}

void TraceTracker::invalidation(int32_t aNode, SharedTypes::tFillLevel cache, address_t block) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] invalidate 0x" << std::hex << block));
}

void TraceTracker::invalidAck(int32_t aNode, SharedTypes::tFillLevel cache, address_t block) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] invAck 0x" << std::hex << block));
}

void TraceTracker::invalidTagCreate(int32_t aNode, SharedTypes::tFillLevel cache, address_t block) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] invTagCreate 0x" << std::hex << block));
}

void TraceTracker::invalidTagRefill(int32_t aNode, SharedTypes::tFillLevel cache, address_t block) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] invTagRefill 0x" << std::hex << block));
}

void TraceTracker::invalidTagReplace(int32_t aNode, SharedTypes::tFillLevel cache, address_t block) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] invTagReplace 0x" << std::hex << block));
}

void TraceTracker::accessLoad(int32_t aNode, SharedTypes::tFillLevel cache, address_t block, uint32_t offset, int32_t size) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] accessLoad 0x" << std::hex << block << "," << offset));
}

void TraceTracker::accessStore(int32_t aNode, SharedTypes::tFillLevel cache, address_t block, uint32_t offset, int32_t size) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] accessStore 0x" << std::hex << block << "," << offset));
}

void TraceTracker::accessFetch(int32_t aNode, SharedTypes::tFillLevel cache, address_t block, uint32_t offset, int32_t size) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] accessLoad 0x" << std::hex << block << "," << offset));
}

void TraceTracker::accessAtomic(int32_t aNode, SharedTypes::tFillLevel cache, address_t block, uint32_t offset, int32_t size) {
  DBG_(MyLevel, ( << "[" << aNode << ":" << cache << "] accessAtomic 0x" << std::hex << block << "," << offset));
}

TraceTracker::TraceTracker()
{}

void TraceTracker::initialize() {
  DBG_(Iface, ( << "initializing TraceTracker"));
  Flexus::Stat::getStatManager()->addFinalizer( ll::bind(&TraceTracker::finalize, this) );
}

void TraceTracker::finalize() {
  DBG_(Iface, ( << "finalizing TraceTracker"));
}


} // namespace nTraceTracker

#ifndef _TRACETRACKER_OBJECT_DEFINED_
#define _TRACETRACKER_OBJECT_DEFINED_
nTraceTracker::TraceTracker theTraceTracker;
#endif
