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
#ifndef _GLOBAL_HASHER_HPP_
#define _GLOBAL_HASHER_HPP_

#include <set>
#include <list>

#include <boost/function.hpp>

namespace nGlobalHasher {

typedef Flexus::SharedTypes::PhysicalMemoryAddress Address;

class GlobalHasher {
private:
  typedef boost::function<int(const Address &)> hash_fn_t;
  typedef std::list<hash_fn_t>::iterator hash_iterator_t;

  GlobalHasher();

  int32_t theHashShift;
  int32_t theHashMask;

  std::list<hash_fn_t> theHashList;

  bool has_been_initialized;

  int32_t simple_hash(int32_t offset, const Address & addr) const;
  int32_t xor_hash(int32_t offset, int32_t xor_shift, const Address & addr) const;
  int32_t shift_hash(int32_t offset, int32_t shift, const Address & addr) const;
  int32_t full_prime_hash(int32_t offset, int32_t prime, const Address & addr) const;

  boost::function<int(int)> createMatrixHash(std::string args, int32_t num_buckets, int32_t shift, int32_t mask, int32_t offset) const;

public:
  std::set<int> hashAddr(const Address & addr);

  void initialize(std::list<std::string> &hash_configs, int32_t initial_shift, int32_t buckets_per_hash, bool partitioned);

  int32_t numHashes() {
    return theHashList.size();
  }

  static GlobalHasher & theHasher();
};
};

#endif // ! _GLOBAL_HASHER_HPP_
