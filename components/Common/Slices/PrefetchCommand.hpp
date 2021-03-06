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
#ifndef FLEXUS_SLICES__PREFETCHCOMMAND_HPP_INCLUDED
#define FLEXUS_SLICES__PREFETCHCOMMAND_HPP_INCLUDED

#include <core/boost_extensions/intrusive_ptr.hpp>

#include <core/types.hpp>

#include <list>

namespace Flexus {
namespace SharedTypes {

class PrefetchCommand : public boost::counted_base {
  typedef PhysicalMemoryAddress MemoryAddress;
public:
  enum PrefetchCommandType {
    // This command tells the PrefetchListener to prefetch the specified
    // list of addresses.
    ePrefetchAddressList,

    ePrefetchRequestMoreAddresses

  };

private:
  PrefetchCommandType theType;
  std::list<MemoryAddress> theAddressList;
  unsigned theSource;
  int32_t theQueue;
  int64_t theLocation;
  int64_t theTag;

public:
  const PrefetchCommandType type() const {
    return theType;
  }
  PrefetchCommandType & type() {
    return theType;
  }

  std::list<MemoryAddress> & addressList() {
    return theAddressList;
  }

  std::list<MemoryAddress> const & addressList() const {
    return theAddressList;
  }

  unsigned & source() {
    return theSource;
  }

  int32_t queue() const {
    return theQueue;
  }

  int32_t & queue() {
    return theQueue;
  }

  unsigned source() const {
    return theSource;
  }

  int64_t & location() {
    return theLocation;
  }

  int64_t location() const {
    return theLocation;
  }

  int64_t & tag() {
    return theTag;
  }

  int64_t tag() const {
    return theTag;
  }

  explicit PrefetchCommand(PrefetchCommandType aType)
    : theType(aType)
    , theSource(0)
    , theQueue(0)
    , theLocation(-1)
    , theTag(-1)
  {}

  friend std::ostream & operator << (std::ostream & s, PrefetchCommand const & aMsg);
};

} //SharedTypes
} //Scaffold

#endif  // FLEXUS_SLICES__PREFETCHCOMMAND_HPP_INCLUDED
