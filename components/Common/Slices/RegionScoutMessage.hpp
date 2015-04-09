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
#ifndef FLEXUS_SLICES__REGIONSCOUTMESSAGE_HPP_INCLUDED
#define FLEXUS_SLICES__REGIONSCOUTMESSAGE_HPP_INCLUDED

#include <core/boost_extensions/intrusive_ptr.hpp>

#include <core/types.hpp>

#include <list>

namespace Flexus {
namespace SharedTypes {

class RegionScoutMessage : public boost::counted_base {
  typedef PhysicalMemoryAddress MemoryAddress;
public:
  enum RegionScoutMessageType {
    eRegionProbe,
    eRegionStateProbe,

    eRegionMissReply,
    eRegionHitReply,

    eRegionIsShared,
    eRegionNonShared,

    eRegionGlobalMiss,
    eRegionPartialMiss,

    eRegionProbeOwner,
    eRegionOwnerReply,
    eRegionSetOwner,

    eRegionEvict,

    eBlockProbe,
    eBlockScoutProbe,
    eBlockMissReply,
    eBlockHitReply,

    eSetTagProbe,
    eRVASetTagProbe,
    eSetTagReply,
    eRVASetTagReply

  };

private:
  RegionScoutMessageType theType;
  MemoryAddress theRegion;
  int32_t theOwner;
  int32_t theCount;
  uint32_t theBlocks;
  bool theShared;

  std::list<MemoryAddress> theTagList;

public:
  const RegionScoutMessageType type() const {
    return theType;
  }
  void  setType(const RegionScoutMessageType & type) {
    theType = type;
  }

  const MemoryAddress region() const {
    return theRegion;
  }
  void  setRegion(const MemoryAddress & region) {
    theRegion = region;
  }

  const int32_t owner() const {
    return theOwner;
  }
  void  setOwner(const int32_t & owner) {
    theOwner = owner;
  }

  const bool shared() const {
    return theShared;
  }
  void  setShared(const bool & shared) {
    theShared = shared;
  }

  const int32_t count() const {
    return theCount;
  }
  void  setCount(const int32_t & count) {
    theCount = count;
  }

  const uint32_t blocks() const {
    return theBlocks;
  }
  void  setBlocks(const uint32_t & blocks) {
    theBlocks = blocks;
  }

  void addTag(const MemoryAddress & addr) {
    theTagList.push_back(addr);
  }
  std::list<MemoryAddress>& getTags() {
    return theTagList;
  }

  explicit RegionScoutMessage(RegionScoutMessageType aType)
    : theType(aType)
    , theRegion(0)
    , theOwner(-1)
    , theBlocks(0)
    , theShared(true)
  {}

  explicit RegionScoutMessage(RegionScoutMessageType aType, const MemoryAddress & aRegion)
    : theType(aType)
    , theRegion(aRegion)
    , theOwner(-1)
    , theBlocks(0)
    , theShared(true)
  {}

  explicit RegionScoutMessage(RegionScoutMessageType aType, const MemoryAddress & aRegion, int32_t anOwner)
    : theType(aType)
    , theRegion(aRegion)
    , theOwner(anOwner)
    , theBlocks(0)
  {}

  friend std::ostream & operator << (std::ostream & s, RegionScoutMessage const & aMsg);
};

std::ostream & operator << (std::ostream & s, RegionScoutMessage::RegionScoutMessageType const & aType);

} //SharedTypes
} //Scaffold

#endif  // FLEXUS_SLICES__REGIONSCOUTMESSAGE_HPP_INCLUDED
