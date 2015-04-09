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
#include <core/boost_extensions/intrusive_ptr.hpp>

namespace nMagicBreak {

class IterationTracker;
class RegressionTracker;
class CycleTracker;
class ConsoleStringTracker;

struct BreakpointTracker : public boost::counted_base {
public:
  static boost::intrusive_ptr<IterationTracker> newIterationTracker();
  static boost::intrusive_ptr<RegressionTracker> newRegressionTracker();
  static boost::intrusive_ptr<BreakpointTracker> newTransactionTracker( int32_t aTransactionType = 1, int32_t aStopTransactionCount = -1, int32_t aStatInterval = -1, int32_t aCkptInterval = -1, int32_t aFirstTransactionIs = 0, uint64_t aMinCycles = 0 );
  static boost::intrusive_ptr<BreakpointTracker> newTerminateOnMagicBreak(int32_t aMagicBreakpoint);
  static boost::intrusive_ptr<BreakpointTracker> newSimPrintHandler();
  static boost::intrusive_ptr<CycleTracker> newCycleTracker( uint64_t aStopCycle, uint64_t aCkptInterval, uint32_t aCkptNameStart );
  static boost::intrusive_ptr<BreakpointTracker> newPacketTracker( int32_t aSrcPortNumber, char aServerMACCode, char aClientMACCode );
  static boost::intrusive_ptr<ConsoleStringTracker> newConsoleStringTracker();
  virtual ~BreakpointTracker() {};
};

struct IterationTracker : public BreakpointTracker {
  virtual void printIterationCounts(std::ostream & aStream) = 0;
  virtual void setIterationCount(uint32_t aCPU, int32_t aCount) = 0;
  virtual void enable() = 0;
  virtual void endOnIteration(int32_t aCount) = 0;
  virtual void enableCheckpoints() = 0;
  virtual void saveState(std::ostream &) = 0;
  virtual void loadState(std::istream &) = 0;
  virtual ~IterationTracker() {};
};

struct RegressionTracker : public BreakpointTracker {
  virtual void enable() = 0;
  virtual ~RegressionTracker () {};
};

struct ConsoleStringTracker : public BreakpointTracker {
  virtual void addString(std::string const &) = 0;
  virtual ~ConsoleStringTracker () {};
};

struct SimPrintHandler : public BreakpointTracker {
  virtual ~SimPrintHandler() {};
};

struct CycleTracker : public BreakpointTracker {
  virtual void tick() = 0;
  virtual ~CycleTracker() {};
};

} //namespace nMagicBreak

