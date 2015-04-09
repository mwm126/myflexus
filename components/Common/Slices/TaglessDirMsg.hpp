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
#ifndef FLEXUS_SLICES__TAGLESS_DIR_MSG_HPP_INCLUDED
#define FLEXUS_SLICES__TAGLESS_DIR_MSG_HPP_INCLUDED

#include <iostream>
#include <core/boost_extensions/intrusive_ptr.hpp>
#include <core/types.hpp>
#include <core/component.hpp>

#include <components/Common/GlobalHasher.hpp>
#include <components/Common/Util.hpp>

#ifdef FLEXUS_TaglessDirMsg_TYPE_PROVIDED
#error "Only one component may provide the Flexus::SharedTypes::TaglessDirMsg data type"
#endif
#define FLEXUS_TaglessDirMsg_TYPE_PROVIDED

namespace Flexus {
namespace SharedTypes {

typedef Flexus::SharedTypes::PhysicalMemoryAddress DestinationAddress;

struct TaglessDirMsg : public boost::counted_base {

  TaglessDirMsg()
  { }

  TaglessDirMsg(const TaglessDirMsg & msg)
    : theConflictFreeBuckets(msg.theConflictFreeBuckets)
  {}

  TaglessDirMsg(const TaglessDirMsg * msg)
    : theConflictFreeBuckets(msg->theConflictFreeBuckets)
  {}

  TaglessDirMsg(boost::intrusive_ptr<TaglessDirMsg> msg)
    : theConflictFreeBuckets(msg->theConflictFreeBuckets)
  {}

  std::map<int, std::set<int> > theConflictFreeBuckets;

  void addConflictFreeSet(int32_t core, std::set<int> &buckets) {
    theConflictFreeBuckets.insert(std::make_pair(core, buckets));
  }

  const std::map<int, std::set<int> >& getConflictFreeBuckets() const {
    return theConflictFreeBuckets;
  }

  void merge(boost::intrusive_ptr<TaglessDirMsg> msg) {
    theConflictFreeBuckets.insert(msg->theConflictFreeBuckets.begin(), msg->theConflictFreeBuckets.end());
  }

  int32_t messageSize() {
    int32_t map_entries = theConflictFreeBuckets.size();

    // If there's zero or one set of buckets, assume we have room in the control packet.
    if (map_entries < 2) {
      return 0;
    }

    int32_t core_id_bits = nCommonUtil::log_base2(Flexus::Core::ComponentManager::getComponentManager().systemWidth());
    int32_t num_hashes = nGlobalHasher::GlobalHasher::theHasher().numHashes();

    // Total size = # of entries (max = num cores - 1 -> need core_id_bits)
    //    + num_entries * entry size
    // Entry size = core_id + bucket bitmap
    int32_t total_bits = core_id_bits + map_entries * (core_id_bits + num_hashes);

    // Convert to number of bytes (round up)
    return ((total_bits + 7) / 8);
  }

};

typedef boost::intrusive_ptr<TaglessDirMsg> TaglessDirMsg_p;

inline std::ostream & operator<< (std::ostream & aStream, const TaglessDirMsg & msg) {

  aStream << "TaglessDirMsg: Conflict Free Buckets = ";
  std::map<int, std::set<int> >::const_iterator map_iter = msg.theConflictFreeBuckets.begin();
  for (; map_iter != msg.theConflictFreeBuckets.end(); map_iter++) {
    aStream << "Core " << map_iter->first << " = {";
    std::set<int>::iterator set_iter = map_iter->second.begin();
    bool needs_comma = false;
    for (; set_iter != map_iter->second.end(); set_iter++) {
      if (needs_comma) {
        aStream << ", ";
      }
      aStream << (*set_iter);
    }
    aStream << "},";
  }

  return aStream;
}

} //End SharedTypes
} //End Flexus

#endif //FLEXUS_SLICES__TAGLESS_DIR_MSG_HPP_INCLUDED
