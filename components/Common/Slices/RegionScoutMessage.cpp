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
#include <iostream>
#include <list>

#include <components/Common/Slices/RegionScoutMessage.hpp>

namespace Flexus {
namespace SharedTypes {

std::ostream & operator << (std::ostream & s, RegionScoutMessage::RegionScoutMessageType const & aType) {
  char const * message_types[] = {
    "Region Probe",
    "Region State Probe",
    "Region Miss Reply",
    "Region Hit Reply",
    "Region Is Shared",
    "Region Not Shared",
    "Region Global Miss",
    "Region Partial Miss",
    "Region Probe Owner",
    "Region Owner Reply",
    "Region Set Owner",
    "Region Evict",
    "Block Probe",
    "BlockScout Probe",
    "Block Miss Reply",
    "Block Hit Reply",
    "Set Tag Probe",
    "RVA Set Tag Probe",
    "Set Tag Reply",
    "RVA Set Tag Reply"
  };
  s << message_types[aType];
  return s;
}

std::ostream & operator << (std::ostream & s, RegionScoutMessage const & aMsg) {
  s << aMsg.type() << " for Region " << std::hex << aMsg.region() << std::dec;
  return s;
}

} //End SharedTypes
} //End Flexus
