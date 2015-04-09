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
#ifndef _COMMON_SERIALIZERS_HPP_
#define _COMMON_SERIALIZERS_HPP_

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/tracking.hpp>
#include <bitset>
#define MAX_NUM_SHARERS 512

namespace boost { 
namespace serialization { 

template <class Archive, std::size_t size>
inline void save(
  Archive & ar,
  std::bitset<size> const & t,
  const unsigned int /* version */
){
  const std::string bits = t.template to_string<
    std::string::value_type,
    std::string::traits_type,
    std::string::allocator_type
  >();
  ar << BOOST_SERIALIZATION_NVP( bits );
}

template <class Archive, std::size_t size>
inline void load(
  Archive & ar,
  std::bitset<size> & t,
  const unsigned int /* version */
){
  std::string bits;
  ar >> BOOST_SERIALIZATION_NVP( bits );
  t = std::bitset<size>(bits);
}

template <class Archive, std::size_t size>
inline void serialize(
  Archive & ar,
  std::bitset<size> & t,
  const unsigned int version
){
  boost::serialization::split_free( ar, t, version );
}

// don't track bitsets since that would trigger tracking
// all over the program - which probably would be a surprise.
// also, tracking would be hard to implement since, we're
// serialization a representation of the data rather than
// the data itself.

template <std::size_t size>
struct tracking_level<std::bitset<size> >
    : mpl::int_<track_never> {} ;
}; 
};

namespace nCommonSerializers {

struct StdDirEntryExtendedSerializer {
  StdDirEntryExtendedSerializer(uint64_t t = 0, std::bitset<MAX_NUM_SHARERS> s = 0): tag(t), state(s) {} 
  uint64_t tag;
  std::bitset<MAX_NUM_SHARERS> state;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const uint32_t version){
    ar & tag;
    ar & state;
  }
};

inline std::ostream & operator<<(std::ostream & os, const StdDirEntryExtendedSerializer & serializer) {
  os << "Addr: " << std::hex << serializer.tag;
  os << ", State: ";
  uint64_t partialState =0;
  uint32_t counter = 0;
  for (int32_t i = MAX_NUM_SHARERS -1; i>=0;i--){
    partialState = partialState << 1;
    partialState |= (serializer.state[i] ? 1 : 0);
    if (i % 64 == 0){ 
      if (counter !=0) os << std::hex << partialState;
      partialState = 0;
      counter++;
    }
  }
  return os;
}

struct StdDirEntrySerializer {
  StdDirEntrySerializer(uint64_t t = 0, uint64_t s = 0) : tag(t), state(s) {}

  uint64_t tag;
  uint64_t state;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const uint32_t version) {
    ar & tag;
    ar & state;
  }
};

inline std::ostream & operator<<(std::ostream & os, const StdDirEntrySerializer & serializer) {
  os << "Addr: " << std::hex << serializer.tag;
  os << ", State: " << std::hex << serializer.state;
  return os;
}

struct RegionDirEntrySerializer {
  RegionDirEntrySerializer(uint64_t t = 0, uint8_t n = 0, int8_t o = -1)
    : tag(t), num_blocks(n), owner(o) {}

  uint64_t tag;
  std::vector<uint64_t> state;
  uint8_t  num_blocks;
  int8_t  owner;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const uint32_t version) {
    ar & tag;
    ar & num_blocks;
    ar & owner;
    ar & state;
  }
};

inline std::ostream & operator<<(std::ostream & os, const RegionDirEntrySerializer & serializer) {
  os << std::hex << serializer.tag;
  os << std::dec << ": owner = " << (int)serializer.owner << ", blocks = ";
  for (uint32_t i = 0; i < serializer.num_blocks; i++) {
    os << std::hex << "<" << serializer.state[i] << ">";
  }
  return os;
}

class SerializableRTDirEntry {
private:
  int64_t   theAddress;
  std::vector<int8_t>     theWays;

  SerializableRTDirEntry() { }
public:
  int8_t owner;
  bool shared;

  SerializableRTDirEntry(int32_t num_blocks) : theAddress(0), theWays(num_blocks, -1), owner(-1), shared(true) {}
  SerializableRTDirEntry(const SerializableRTDirEntry & entry)
    : theAddress(entry.theAddress), theWays(entry.theWays), owner(entry.owner), shared(entry.shared) {}

  const int64_t & tag() {
    return theAddress;
  }

  void reset(Flexus::SharedTypes::PhysicalMemoryAddress addr) {
    theAddress = addr;
    std::for_each(theWays.begin(), theWays.end(), boost::lambda::_1 = -1);
    owner = -1;
    shared = true;
  }

  int8_t & operator[](int32_t offset) {
    return theWays[offset];
  }

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const uint32_t version) {
    ar & theAddress;
    ar & theWays;
    ar & owner;
    ar & shared;
    DBG_(Trace, ( << "serialize Region: " << std::hex << theAddress << " owner = " << std::dec << (int)owner << (shared ? " Shared" : " Non-Shared") ));
  }
};

struct BlockSerializer {
  uint64_t    tag;
  uint8_t     way;
  uint8_t     state;

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const uint32_t version) {
    ar & tag;
    ar & state;
  }
};

}; // namespace nCommonSerializers

BOOST_CLASS_TRACKING(nCommonSerializers::StdDirEntrySerializer, boost::serialization::track_never);
BOOST_CLASS_TRACKING(nCommonSerializers::RegionDirEntrySerializer, boost::serialization::track_never);
BOOST_CLASS_TRACKING(nCommonSerializers::SerializableRTDirEntry, boost::serialization::track_never);
BOOST_CLASS_TRACKING(nCommonSerializers::BlockSerializer, boost::serialization::track_never);

#endif // ! _COMMON_SERIALIZERS_HPP_
