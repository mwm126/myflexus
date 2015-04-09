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
#ifndef FLEXUS_SLICES__DESTINATION_MESSAGE_HPP_INCLUDED
#define FLEXUS_SLICES__DESTINATION_MESSAGE_HPP_INCLUDED

#include <list>
#include <iostream>
#include <core/boost_extensions/intrusive_ptr.hpp>
#include <core/types.hpp>

#ifdef FLEXUS_DestinationMessage_TYPE_PROVIDED
#error "Only one component may provide the Flexus::SharedTypes::DestinationMessage data type"
#endif
#define FLEXUS_DestinationMessage_TYPE_PROVIDED

namespace Flexus {
namespace SharedTypes {

typedef Flexus::SharedTypes::PhysicalMemoryAddress DestinationAddress;

struct DestinationMessage : public boost::counted_base {

  enum DestinationType {
    Requester,
    Directory,
    RemoteDirectory,
    Source,
    Memory,
    Multicast,
    Other
  };

  DestinationMessage(DestinationType aType)
    : type(aType)
    , requester(-1)
    , directory(-1)
    , remote_directory(-1)
    , source(-1)
    , memory(-1)
    , other(-1)
    , to_i_cache(false)
    , from_i_cache(false)
  { }

  DestinationMessage(const DestinationMessage & msg)
    : type(msg.type)
    , requester(msg.requester)
    , directory(msg.directory)
    , remote_directory(msg.remote_directory)
    , source(msg.source)
    , memory(msg.memory)
    , multicast_list(msg.multicast_list)
    , other(msg.other)
    , to_i_cache(msg.to_i_cache)
    , from_i_cache(msg.from_i_cache)
  {}

  DestinationMessage(const DestinationMessage * msg)
    : type(msg->type)
    , requester(msg->requester)
    , directory(msg->directory)
    , remote_directory(msg->remote_directory)
    , source(msg->source)
    , memory(msg->memory)
    , multicast_list(msg->multicast_list)
    , other(msg->other)
    , to_i_cache(msg->to_i_cache)
    , from_i_cache(msg->from_i_cache)
  {}

  DestinationMessage(boost::intrusive_ptr<DestinationMessage> msg)
    : type(msg->type)
    , requester(msg->requester)
    , directory(msg->directory)
    , remote_directory(msg->remote_directory)
    , source(msg->source)
    , memory(msg->memory)
    , multicast_list(msg->multicast_list)
    , other(msg->other)
    , to_i_cache(msg->to_i_cache)
    , from_i_cache(msg->from_i_cache)
  {}

  DestinationType type;
  int32_t requester;
  int32_t directory;
  int32_t remote_directory;
  int32_t source;
  int32_t memory;
  std::list<int> multicast_list;
  int32_t other;
  bool to_i_cache;
  bool from_i_cache;

  inline bool isMultipleMsgs() const {
    return ((type == Multicast) && (multicast_list.size() > 1));
  }

  typedef boost::intrusive_ptr<DestinationMessage> DestinationMessage_p;

  DestinationMessage_p removeFirstMulticastDest() {
    DestinationMessage_p ret(new DestinationMessage(this));
    ret->multicast_list.clear();
    ret->other = multicast_list.front();
    ret->type = Other;
    multicast_list.pop_front();
    return ret;
  }

  void convertMulticast() {
    other = multicast_list.front();
    type = Other;
    multicast_list.pop_front();
  }

};

typedef boost::intrusive_ptr<DestinationMessage> DestinationMessage_p;

inline std::ostream & operator<< (std::ostream & aStream, const DestinationMessage & msg) {
  aStream << "DestMsg: type=";
  switch (msg.type) {
    case DestinationMessage::Requester:
      aStream << "Requester, ";
      break;
    case DestinationMessage::Directory:
      aStream << "Directory, ";
      break;
    case DestinationMessage::RemoteDirectory:
      aStream << "RemoteDirectory, ";
      break;
    case DestinationMessage::Source:
      aStream << "Source, ";
      break;
    case DestinationMessage::Memory:
      aStream << "Memory, ";
      break;
    case DestinationMessage::Multicast:
      aStream << "Multicast, ";
      break;
    case DestinationMessage::Other:
      aStream << "Other, ";
      break;
  }
  aStream << "Requester = " << msg.requester << ", ";
  aStream << "Directory = " << msg.directory << ", ";
  aStream << "RemoteDirectory = " << msg.remote_directory << ", ";
  aStream << "Source = " << msg.source << ", ";
  aStream << "Memory = " << msg.memory << ", ";
  aStream << "Multicast = (";
  for (std::list<int>::const_iterator iter = msg.multicast_list.begin(); iter != msg.multicast_list.end(); iter++) {
    aStream  << (*iter) << ",";
  }
  aStream << "), ";
  aStream << "Other = " << msg.other << ", ";
  aStream << "To ICache = " << std::boolalpha << msg.to_i_cache << ", ";
  aStream << "From ICache = " << std::boolalpha << msg.to_i_cache;

  return aStream;
}

} //End SharedTypes
} //End Flexus

#endif //FLEXUS_SLICES__DESTINATION_MESSAGE_HPP_INCLUDED
