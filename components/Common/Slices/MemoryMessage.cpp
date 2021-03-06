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
#include <components/Common/Slices/MemoryMessage.hpp>

namespace Flexus {
namespace SharedTypes {

static uint32_t theMemoryMessageSerial = 0;

uint32_t memoryMessageSerial ( void ) {
  return ++theMemoryMessageSerial;
}

std::ostream & operator << (std::ostream & s, MemoryMessage::MemoryMessageType const & aMemMsgType) {
  static char const * message_types[] = {
    "Load Request",
    "Store Request",
    "Store Prefetch Request",
    "Fetch Request",
    "Non-allocating Store Request",
    "RMW Request",
    "Cmp-Swap Request",
    "Atomic Preload Request",
    "Flush Request",
    "Read Request",

    "Write Request",
    "Write Allocate",
    "Upgrade Request",
    "Upgrade Allocate",
    "Flush",
    "Eviction (dirty)",
    "Eviction (writable)",
    "Eviction (clean)",
    "SVB Clean Eviction",
    "Load Reply",
    "Store Reply",
    "Store Prefetch Reply",
    "Fetch Reply",
    "RMW Reply",
    "Cmp-Swap Reply",
    "Atomic Preload Reply",
    "Miss Reply",
    "Miss Writable Reply",
    "Miss Dirty Reply",

    "Upgrade Reply",
    "Non-Allocating Store Reply",
    "Invalidate",
    "Downgrade",
    "Probe",
    "DownProbe",
    "ReturnReq",
    "Invalidate Ack",
    "Invalidate Update Ack",
    "Downgrade Ack",

    "Downgrade Update Ack",
    "Probed - Not Present",
    "Probed - Clean",
    "Probed - Writable",
    "Probed - Dirty",
    "DownProbe - Present",
    "DownProbe - Not Present",
    "ReturnReply",
    "Stream Fetch",

    "Prefetch Read No-Allocate Request",
    "Prefetch Read Allocate",
    "Prefetch Insert Request",
    "Prefetch Insert Writable Request",
    "Prefetch Read Reply",
    "Prefetch Read Reply Writable",
    "Prefetch Read Reply Dirty",
    "Prefetch Read Redundant",
    "Stream Fetch Writable Reply"
    , "Stream Fetch Rejected"
    , "ReturnNAck"
    , "ReturnReplyDirty"
    , "FetchFwd"
    , "ReadFwd"
    , "WriteFwd"
    , "FwdNAck"
    , "FwdReply"
    , "FwdReplyOwned"
    , "FwdReplyWritable"
    , "FwdReplyDirty"
    , "ReadAck"
    , "ReadAckDirty"
    , "FetchAck"
    , "FetchAckDirty"
    , "WriteAck"
    , "UpgradeAck"
    , "Non-Allocating Store Ack"
    , "ReadNAck"
    , "FetchNAck"
    , "WriteNAck"
    , "UpgradeNAck"
    , "Non-Allocating Store NAck"
    , "MissNotify"
    , "MissNotifyData"
    , "BackInvalidate"
    , "InvalidateNAck"
    , "Protocol Message"
    , "Evict Ack"
    , "Write Retry"
    , "NumMemoryMessageTypes"
  };
  assert(aMemMsgType <= MemoryMessage::NumMemoryMessageTypes );
  return s << message_types[aMemMsgType];
}
std::ostream & operator << (std::ostream & s, MemoryMessage const & aMemMsg) {
  return s << "MemoryMessage[" << aMemMsg.type()
         << "]: Addr:0x" << std::hex << aMemMsg.address()
         << " Size:" << std::dec << aMemMsg.reqSize() << " Serial: " << aMemMsg.serial()
         << " Core: " << aMemMsg.coreIdx()
         << " DStream: " << std::boolalpha << aMemMsg.isDstream()
         << " Outstanding Msgs: " << aMemMsg.outstandingMsgs()
         << (aMemMsg.ackRequired() ? (aMemMsg.ackRequiresData() ? " Requires Ack+Data" : " Requires Ack") : "");
}

} //namespace SharedTypes
} //namespace Flexus

