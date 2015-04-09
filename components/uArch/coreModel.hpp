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
#ifndef FLEXUS_uARCH_COREMODEL_HPP_INCLUDED
#define FLEXUS_uARCH_COREMODEL_HPP_INCLUDED

#include "uArchInterfaces.hpp"

#include <components/uFetch/uFetchTypes.hpp>

namespace Flexus {
namespace Simics {
struct Translation;
} //Simics
} //Flexus

namespace nuArch {

enum eLoseWritePermission {
  eLosePerm_Invalidate
  , eLosePerm_Downgrade
  , eLosePerm_Replacement
};

struct v9State {
  uint64_t theGlobalRegs[32];
  uint64_t theWindowRegs[128];
  uint64_t theFPRegs[64];
  uint64_t thePC;
  uint64_t theNPC;
  uint64_t theTBA;
  uint64_t theCWP;
  uint64_t theCCR;
  uint64_t theFPRS;
  uint64_t theFSR;
  uint64_t thePSTATE;
  uint64_t theASI;
  uint64_t theGSR;
  uint64_t theTL;
  uint64_t thePIL;
  uint64_t theCANSAVE;
  uint64_t theCANRESTORE;
  uint64_t theCLEANWIN;
  uint64_t theOTHERWIN;
  uint64_t theSOFTINT;
  uint64_t theWSTATE;
  uint64_t theY;
  uint64_t theGLOBALS;
  uint64_t theTICK;
  uint64_t theTICK_CMPR;
  uint64_t theSTICK;
  uint64_t theSTICK_CMPR;
  struct tte {
    uint64_t theTPC;
    uint64_t theTNPC;
    uint64_t theTSTATE;
    uint64_t theTT;
  } theTTE[5];
};

struct CoreModel : public uArch {
  static CoreModel * construct(uArchOptions_t options
                               , boost::function< void (Flexus::Simics::Translation &, bool) > translate
                               , boost::function< int(bool) > advance
                               , boost::function< void(eSquashCause) > squash
                               , boost::function< void(VirtualMemoryAddress, VirtualMemoryAddress) > redirect
                               , boost::function< void(int, int) > change_mode
                               , boost::function< void( boost::intrusive_ptr<BranchFeedback> )> feedback
                               , boost::function< void( bool )> signalStoreForwardingHit
                              );

  //Interface to mircoArch
  virtual void initializeRegister(mapped_reg aRegister, register_value aValue) = 0;
  virtual register_value readArchitecturalRegister( unmapped_reg aRegister, bool aRotate ) = 0;
  virtual int32_t selectedRegisterSet() const = 0;
  virtual void setRoundingMode(uint32_t aRoundingMode) = 0;

  virtual void setTPC( uint64_t aTPC, uint32_t aTL) = 0;
  virtual void setTNPC( uint64_t aTNPC, uint32_t aTL) = 0;
  virtual void setTSTATE( uint64_t aTSTATE, uint32_t aTL) = 0;
  virtual void setTT( uint32_t aTT, uint32_t aTL) = 0;
  virtual void setTBA( uint64_t aTBA) = 0;
  virtual void setPSTATE( uint64_t aPSTATE) = 0;
  virtual uint64_t getPSTATE( ) = 0;
  virtual void setTL( uint32_t aTL) = 0;
  virtual uint32_t getTL( ) = 0;
  virtual void setPIL( uint32_t aPIL) = 0;
  virtual void setCWP( uint32_t aCWP ) = 0;
  virtual void setCLEANWIN( uint32_t aCLEANWIN ) = 0;
  virtual void setCANSAVE( uint32_t aCANSAVE ) = 0;
  virtual void setCANRESTORE( uint32_t aCANRESTORE) = 0;
  virtual void setOTHERWIN( uint32_t aOTHERWIN) = 0;
  virtual void setWSTATE( uint32_t aWSTATE ) = 0;
  virtual void setVER( uint64_t aVER ) = 0;
  virtual void setTICK( uint64_t aTick ) = 0;
  virtual void setSTICK( uint64_t aStick ) = 0;
  virtual void setSTICKInterval( uint64_t aStickInterval ) = 0;
  virtual void setAG( bool ) = 0;
  virtual void setIG( bool ) = 0;
  virtual void setMG( bool ) = 0;
  virtual void setFSR( uint64_t anFSR) = 0;

  virtual void getv9State( v9State & aState) = 0;
  virtual void restorev9State( v9State & aState) = 0;
  virtual void setPC( uint64_t aPC) = 0;
  virtual void setNPC( uint64_t anNPC) = 0;
  virtual uint64_t pc() const = 0;
  virtual uint64_t npc() const = 0;

  virtual void dumpActions() = 0;
  virtual void reset() = 0;
  virtual void resetv9() = 0;

  virtual int32_t availableROB() const = 0;
  virtual bool isSynchronized() const = 0;
  virtual bool isStalled() const = 0;
  virtual int32_t iCount() const = 0;
  virtual bool isQuiesced() const = 0;
  virtual void dispatch(boost::intrusive_ptr<Instruction>) = 0;

  virtual void skipCycle() = 0;
  virtual void cycle(int32_t aPendingInterrupt) = 0;

  virtual void pushMemOp(boost::intrusive_ptr< MemOp >) = 0;
  virtual bool canPushMemOp() = 0;
  virtual boost::intrusive_ptr<MemOp> popMemOp() = 0;
  virtual boost::intrusive_ptr<MemOp> popSnoopOp() = 0;
  virtual void printROB() = 0;
  virtual void printSRB() = 0;
  virtual void printMemQueue() = 0;
  virtual void printMSHR() = 0;

  virtual void printRegMappings(std::string) = 0;
  virtual void printRegFreeList(std::string) = 0;
  virtual void printRegReverseMappings(std::string) = 0;
  virtual void printAssignments(std::string) = 0;

  virtual void loseWritePermission( eLoseWritePermission aReason, PhysicalMemoryAddress anAddress) = 0;

};

struct ResynchronizeWithSimicsException {
  bool expected;
  ResynchronizeWithSimicsException(bool was_expected = false )  : expected(was_expected) {}
};

} //nuArch

#endif //FLEXUS_uARCH_COREMODEL_HPP_INCLUDED
