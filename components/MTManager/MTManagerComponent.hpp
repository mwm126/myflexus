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
#include <core/simulator_layout.hpp>

#define FLEXUS_BEGIN_COMPONENT MTManagerComponent
#include FLEXUS_BEGIN_COMPONENT_DECLARATION()

#include "components/MTManager/MTManager.hpp"

static const int32_t kFE_StrictRoundRobin = 0;
static const int32_t kFE_ICount = 1;

static const int32_t kBE_StrictRoundRobin = 0;
static const int32_t kBE_SmartRoundRobin = 1;

COMPONENT_PARAMETERS(
  PARAMETER(Cores, uint32_t, "Number of cores", "cores", 1 )
  PARAMETER(Threads, uint32_t, "Number of threads per core", "threads", 1 )
  PARAMETER(FrontEndPolicy, int, "Scheduling policy for front end", "front_policy", kFE_StrictRoundRobin )
  PARAMETER(BackEndPolicy, int, "Scheduling policy for back end", "back_policy", kBE_StrictRoundRobin )
);

COMPONENT_INTERFACE(
  DYNAMIC_PORT_ARRAY( PullInput, bool, EXStalled )
  DYNAMIC_PORT_ARRAY( PullInput, bool, FAGStalled )
  DYNAMIC_PORT_ARRAY( PullInput, bool, FStalled )
  DYNAMIC_PORT_ARRAY( PullInput, bool, DStalled )
  DYNAMIC_PORT_ARRAY( PullInput, int, FAQ_ICount)
  DYNAMIC_PORT_ARRAY( PullInput, int, FIQ_ICount)
  DYNAMIC_PORT_ARRAY( PullInput, int, ROB_ICount)
);

#include FLEXUS_END_COMPONENT_DECLARATION()
#define FLEXUS_END_COMPONENT MTManagerComponent
