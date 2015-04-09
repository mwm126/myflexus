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
#include <iomanip>

#include <core/boost_extensions/intrusive_ptr.hpp>
#include <boost/throw_exception.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
namespace ll = boost::lambda;

#include <boost/none.hpp>

#include <boost/dynamic_bitset.hpp>

#include <core/target.hpp>
#include <core/debug/debug.hpp>
#include <core/types.hpp>

#include <components/uArch/uArchInterfaces.hpp>

#include "../SemanticInstruction.hpp"
#include "../Effects.hpp"
#include "../SemanticActions.hpp"

#define DBG_DeclareCategories v9Decoder
#define DBG_SetDefaultOps AddCat(v9Decoder)
#include DBG_Control()

namespace nv9Decoder {

using namespace nuArch;
using nuArch::Instruction;

struct UpdateAddressAction : public BaseSemanticAction {

  eOperandCode theAddressCode;

  UpdateAddressAction ( SemanticInstruction * anInstruction, eOperandCode anAddressCode )
    : BaseSemanticAction ( anInstruction, 2 )
    , theAddressCode (anAddressCode)
  { }

  void squash(int32_t anOperand) {
    if (! cancelled() ) {
      DBG_( Verb, ( << *this << " Squashing vaddr." ) );
      core()->resolveVAddr( boost::intrusive_ptr<Instruction>(theInstruction), kUnresolved, 0x80);
    }
    BaseSemanticAction::squash(anOperand);
  }

  void satisfy(int32_t anOperand) {
    //updateAddress as soon as dependence is satisfied
    BaseSemanticAction::satisfy(anOperand);
    updateAddress();
  }

  void doEvaluate() {
    //Address is now updated when satisfied.
  }

  void updateAddress() {
    if (ready()) {
      uint64_t addr = theInstruction->operand< uint64_t > (theAddressCode);
      if (theInstruction->hasOperand( kUopAddressOffset ) ) {
        uint64_t offset = theInstruction->operand< uint64_t > (kUopAddressOffset);
        addr += offset;
      }
      int32_t asi = theInstruction->operand< uint64_t > (kOperand3);
      VirtualMemoryAddress vaddr(addr);
      DBG_( Verb, ( << *this << " updating vaddr=" << vaddr << " asi=" << std::hex << asi << std::dec) );
      core()->resolveVAddr( boost::intrusive_ptr<Instruction>(theInstruction), vaddr, asi);
      theInstruction->setASI(asi);

      satisfyDependants();
    }
  }

  void describe( std::ostream & anOstream) const {
    anOstream << theInstruction->identify() << " UpdateAddressAction";
  }
};

multiply_dependant_action updateAddressAction
( SemanticInstruction * anInstruction ) {
  UpdateAddressAction * act(new(anInstruction->icb()) UpdateAddressAction( anInstruction, kAddress ) );
  std::vector<InternalDependance> dependances;
  dependances.push_back( act->dependance(0) );
  dependances.push_back( act->dependance(1) );

  return multiply_dependant_action( act, dependances );
}

multiply_dependant_action updateCASAddressAction
( SemanticInstruction * anInstruction ) {
  UpdateAddressAction * act(new(anInstruction->icb()) UpdateAddressAction( anInstruction, kOperand1 ) );
  std::vector<InternalDependance> dependances;
  dependances.push_back( act->dependance(0) );
  dependances.push_back( act->dependance(1) );

  return multiply_dependant_action( act, dependances );
}

} //nv9Decoder
