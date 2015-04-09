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
#ifndef FLEXUS_COMMON__DOUBLE_WORD_HPP_INCLUDED
#define FLEXUS_COMMON__DOUBLE_WORD_HPP_INCLUDED

#include <core/debug/debug.hpp>

namespace Flexus {
namespace SharedTypes {

struct DoubleWord {
  uint64_t theDoubleWord;
  uint8_t theValidBits;

  DoubleWord()
    : theDoubleWord(0xEAEAEAEA)
    , theValidBits(0)
  {}

  DoubleWord(uint64_t aULL)
    : theDoubleWord(aULL)
    , theValidBits(0xFF)
  {}

  DoubleWord(uint64_t aBase, DoubleWord const & anApplyVal)
    : theDoubleWord(aBase)
    , theValidBits(0xFF) {
    apply(anApplyVal);
  }

  void set(uint64_t aValue, int32_t aSize, uint32_t anOffset) {
    switch (aSize) {
      case 1:
        getByte( anOffset ) = aValue;
        theValidBits |= (0x1 << (7 - anOffset));
        break;
      case 2:
        getHalfWord( anOffset ) = aValue;
        theValidBits |= (0x3 << (6 - anOffset));
        break;
      case 4:
        getWord( anOffset ) = aValue;
        theValidBits |= (0xF << (4 - anOffset));
        break;
      case 8:
        getDoubleWord( anOffset ) = aValue;
        theValidBits = 0xFF;
        break;
      default:
        DBG_Assert(false, ( << "Unsupported size for DoubleWord::set") );
    }
  }

  bool isValid(int32_t aSize, uint32_t anOffset) const {
    uint8_t bits = 0;
    switch (aSize) {
      case 1:
        bits = 0x1 << (7 - anOffset);
        break;
      case 2:
        bits = 0x3 << (6 - anOffset);
        break;
      case 4:
        bits = 0xF << (4 - anOffset);
        break;
      case 8:
        bits = 0xFF;
        break;
      default:
        return false;
    }
    bool ret_val = (theValidBits & bits) == bits;
    return ret_val;
  }

  bool isEqual(uint64_t aValue, int32_t aSize, uint32_t anOffset) {
    if (! isValid(aSize, anOffset)) {
      return false;
    }
    switch (aSize) {
      case 1:
        return getByte( anOffset ) == static_cast<uint8_t>(aValue);
      case 2:
        return getHalfWord( anOffset ) == static_cast<uint16_t>(aValue);
      case 4:
        return getWord( anOffset ) == static_cast<uint32_t>(aValue);
      case 8:
        return getDoubleWord( anOffset ) == aValue;
      default:
        DBG_Assert(false, ( << "Unsupported size for DoubleWord::set") );
        return false;
    }
  }

  uint8_t & getByte( uint32_t anOffset) {
    return *( reinterpret_cast< uint8_t *>(&theDoubleWord) + (7 - anOffset) );
  }
  uint16_t & getHalfWord( uint32_t anOffset) {
    return *( reinterpret_cast< uint16_t *>(&theDoubleWord) + (3 - (anOffset >> 1)) );
  }
  uint32_t & getWord( uint32_t anOffset) {
    return *( reinterpret_cast< uint32_t *>(&theDoubleWord) + (1 - (anOffset >> 2) ) );
  }
  uint64_t & getDoubleWord( uint32_t anOffset /* ignored */) {
    DBG_Assert(anOffset == 0);
    return theDoubleWord;
  }

  void apply( DoubleWord const & aMask) {
    if (aMask.theValidBits != 0) {
      uint64_t masked_val = 0;
      for ( int32_t i = 7; i >= 0; --i) {
        masked_val <<= 8;
        if (aMask.theValidBits & (1 << i)) {
          theValidBits |= (1 << i);
          masked_val |= ((aMask.theDoubleWord >> (8 * i )) & 0xFF);
        } else {
          masked_val |= ((theDoubleWord >> (8 * i )) & 0xFF);
        }
      }
      theDoubleWord = masked_val;
    }
  }

};

bool operator== (uint64_t compare, DoubleWord const & aDoubleWord);
bool operator== (DoubleWord const & entry, uint64_t compare);
std::ostream & operator <<(std::ostream & anOstream, DoubleWord const & aDoubleWord);

} //SharedTypes
} //Flexus

#endif //FLEXUS_COMMON__DOUBLE_WORD_HPP_INCLUDED
