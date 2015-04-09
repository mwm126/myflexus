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
#include <core/boost_extensions/intrusive_ptr.hpp>

#include <list>

namespace nWhiteBox {

struct CPUState {
  uint64_t theThread;
  uint64_t thePC;
  uint32_t theTrap;
  std::list<uint64_t> theBackTrace;
  std::string theExecName;
};

std::ostream & operator<< (std::ostream & anOstream, CPUState & entry);

struct WhiteBox : public boost::counted_base {
  virtual uint64_t get_thread_t( int32_t aCPU ) = 0;
  virtual uint64_t get_idle_thread_t( int32_t aCPU ) = 0;
  virtual int64_t getPID( int32_t aCPU) = 0;
  virtual int32_t getPendingTrap( int32_t aCPU ) = 0;
  virtual void printPIDS( ) = 0;
  virtual uint64_t pc(int32_t aCPU) = 0;
  virtual void fillBackTrace(int32_t aCPU, std::list<uint64_t> & aTrace) = 0;
  virtual bool getState(int32_t aCPU, CPUState & aState) = 0;

  virtual ~WhiteBox() {};
  static boost::intrusive_ptr<WhiteBox> getWhiteBox();
};

} //namespace nMagicBreak

