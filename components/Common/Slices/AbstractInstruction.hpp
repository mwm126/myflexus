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
#ifndef FLEXUS_SLICES_ABSTRACTINSTRUCTION_HPP_INCLUDED
#define FLEXUS_SLICES_ABSTRACTINSTRUCTION_HPP_INCLUDED

#include <ostream>
#include <core/boost_extensions/intrusive_ptr.hpp>
#include <components/Common/Slices/TransactionTracker.hpp>

#include <components/Common/Slices/FillLevel.hpp>

namespace Flexus {
namespace SharedTypes {

struct AbstractInstruction  : public boost::counted_base {
  boost::intrusive_ptr<TransactionTracker> theFetchTransaction;
protected:
  tFillLevel theInsnSourceLevel;

public:

  virtual void describe(std::ostream & anOstream) const;
  virtual bool haltDispatch() const;
  virtual void setFetchTransactionTracker(boost::intrusive_ptr<TransactionTracker> aTransaction) {
    theFetchTransaction = aTransaction;
  }
  virtual boost::intrusive_ptr<TransactionTracker> getFetchTransactionTracker() const {
    return theFetchTransaction;
  }
  AbstractInstruction() : theInsnSourceLevel(eL1I) {}
  virtual ~AbstractInstruction() {}

  virtual void setSourceLevel(tFillLevel aLevel) {
    theInsnSourceLevel = aLevel;
  }
  virtual tFillLevel sourceLevel() const {
    return theInsnSourceLevel;
  }

};

enum eSquashCause {
  kResynchronize = 1
  , kBranchMispredict = 2
  , kException = 3
  , kInterrupt = 4
  , kFailedSpec = 5
};

std::ostream & operator << ( std::ostream & anOstream, eSquashCause aCause );

std::ostream & operator << (std::ostream & anOstream, AbstractInstruction const & anInstruction);

} //Flexus
} //SharedTypes

#endif //FLEXUS_SLICES_ABSTRACTINSTRUCTION_HPP_INCLUDED
