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
#ifndef __REGION_TRACKER_COORDINATOR_HPP__
#define __REGION_TRACKER_COORDINATOR_HPP__

#include <boost/function.hpp>
#include <boost/dynamic_bitset.hpp>

#include <vector>

#include <core/types.hpp>

namespace nRTCoordinator {

typedef Flexus::SharedTypes::PhysicalMemoryAddress MemoryAddress;

struct RTFunctionList {
  boost::function<int32_t (MemoryAddress)> probeOwner;
  boost::function<bool (MemoryAddress, int)> setOwner;
  boost::function<boost::dynamic_bitset<> (MemoryAddress)> probePresence;
};

struct NotifierFunctionList {
  boost::function<void (MemoryAddress)> regionEvictNotify;
  boost::function<void (MemoryAddress)> regionAllocNotify;
};

class RTCoordinator {
private:
  RTCoordinator() {}

  std::vector<RTFunctionList> theRTFunctions;
  std::vector<RTFunctionList> thePassThroughFunctions;
  std::vector<NotifierFunctionList> theNotifiers;

public:
  static RTCoordinator & getCoordinator() {
    static RTCoordinator * theCoordinator = NULL;
    if (theCoordinator == NULL) {
      theCoordinator = new RTCoordinator();
    }
    return *theCoordinator;
  }

  void registerRTFunctions(int32_t index, RTFunctionList functions) {
    if (index >= (int)theRTFunctions.size()) {
      theRTFunctions.resize(index + 1);
    }
    theRTFunctions[index] = functions;
  }

  void registerPassThroughFunctions(int32_t index, RTFunctionList functions) {
    if (index >= (int)thePassThroughFunctions.size()) {
      thePassThroughFunctions.resize(index + 1);
    }
    thePassThroughFunctions[index] = functions;
  }

  RTFunctionList & getRTFunctions(int32_t index) {
    return theRTFunctions[index];
  }

  RTFunctionList & getPassThroughFunctions(int32_t index) {
    return thePassThroughFunctions[index];
  }

  int32_t probeOwner(int32_t index, MemoryAddress addr) {
    return theRTFunctions[index].probeOwner(addr);
  }

  bool setOwner(int32_t index, MemoryAddress addr, int32_t owner) {
    return theRTFunctions[index].setOwner(addr, owner);
  }

  int32_t probePTOwner(int32_t index, MemoryAddress addr) {
    return thePassThroughFunctions[index].probeOwner(addr);
  }

  bool setPTOwner(int32_t index, MemoryAddress addr, int32_t owner) {
    return thePassThroughFunctions[index].setOwner(addr, owner);
  }

  boost::dynamic_bitset<> probePresence(int32_t index, MemoryAddress addr) {
    return theRTFunctions[index].probePresence(addr);
  }

  void registerNotifierFunctions(int32_t index, NotifierFunctionList functions) {
    if (index >= (int)theNotifiers.size()) {
      theNotifiers.resize(index + 1);
    }
    theNotifiers[index] = functions;
  }

  void regionAllocNotify(int32_t index, MemoryAddress addr) {
    if (index < (int)theNotifiers.size() && !theNotifiers[index].regionAllocNotify.empty()) {
      theNotifiers[index].regionAllocNotify(addr);
    }
  }

  void regionEvictNotify(int32_t index, MemoryAddress addr) {
    if (index < (int)theNotifiers.size() && !theNotifiers[index].regionEvictNotify.empty()) {
      theNotifiers[index].regionEvictNotify(addr);
    }
  }

};

}; // namespace nRTCoordinator

#endif // ! __REGION_TRACKER_COORDINATOR_HPP__
