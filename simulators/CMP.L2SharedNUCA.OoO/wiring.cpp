


#define FLEXUS_WIRING_FILE
#include <core/simulator_layout.hpp>


//This section contains the name of the simulator
#include <core/simulator_name.hpp>
namespace Flexus {
//Simulator Name
std::string theSimulatorName = "CMP.L2SharedNUCA.OoO v1.0";
}


#include FLEXUS_BEGIN_DECLARATION_SECTION()

#include <components/MemoryLoopback/MemoryLoopback.hpp>
#include <components/MemoryMap/MemoryMap.hpp>
#include <components/MagicBreak/MagicBreak.hpp>
#include <components/Cache/Cache.hpp>
#include <components/NetShim/MemoryNetwork.hpp>
#include <components/MultiNic/MultiNic2.hpp>
#include <components/CMPCache/CMPCache.hpp>
#include<components/FetchAddressGenerate/FetchAddressGenerate.hpp>
#include<components/uFetch/uFetch.hpp>
#include<components/uFetch/PortCombiner.hpp>
#include<components/v9Decoder/v9Decoder.hpp>
#include<components/uArch/uArch.hpp>
#include<components/SplitDestinationMapper/SplitDestinationMapper.hpp>

#include <components/WhiteBox/WhiteBox.hpp>

#include FLEXUS_END_DECLARATION_SECTION()


#include FLEXUS_BEGIN_COMPONENT_CONFIGURATION_SECTION()

CREATE_CONFIGURATION( WhiteBox, "white-box", theWhiteBoxCfg );

CREATE_CONFIGURATION( FetchAddressGenerate, "fag", theFAGCfg );
CREATE_CONFIGURATION( uFetch, "ufetch", theuFetchCfg );
CREATE_CONFIGURATION( PortCombiner, "combiner", theCombinerCfg );
CREATE_CONFIGURATION( v9Decoder, "decoder", theDecoderCfg );
CREATE_CONFIGURATION( uArch, "uarch", theuArchCfg );

CREATE_CONFIGURATION( Cache, "L1d", theL1dCfg );
CREATE_CONFIGURATION( CMPCache, "L2", theL2Cfg );
CREATE_CONFIGURATION( MultiNic2, "nic", theNicCfg );
CREATE_CONFIGURATION( MemoryNetwork, "network", theNetworkCfg );

CREATE_CONFIGURATION( SplitDestinationMapper, "net-mapper", theNetMapperCfg );

CREATE_CONFIGURATION( MemoryLoopback, "memory", theMemoryCfg );
CREATE_CONFIGURATION( MemoryMap, "memory-map", theMemoryMapCfg );
CREATE_CONFIGURATION( MagicBreak, "magic-break", theMagicBreakCfg );

//You may optionally initialize configuration parameters from within this
//function.  This initialization occur before the command line is processed,
//so they will be overridden from the command line.
//
//Return value indicates whether simulation should abort if any parameters
//are left at their default values;
bool initializeParameters() {
  DBG_( Dev, ( << " initializing Parameters..." ) );

  theFAGCfg.Threads.initialize(1);
  theFAGCfg.MaxFetchAddress.initialize(10);
  theFAGCfg.MaxBPred.initialize(2);

  theuFetchCfg.Threads.initialize(1);
  theuFetchCfg.FAQSize.initialize(32);
  theuFetchCfg.MaxFetchLines.initialize(2);
  theuFetchCfg.MaxFetchInstructions.initialize(10);
  theuFetchCfg.ICacheLineSize.initialize(64);
  theuFetchCfg.PerfectICache.initialize(false);
  theuFetchCfg.PrefetchEnabled.initialize(true);
  theuFetchCfg.Size.initialize(65536);
  theuFetchCfg.Associativity.initialize(2);
  theuFetchCfg.MissQueueSize.initialize(4);
  theuFetchCfg.CleanEvict.initialize(false);
  theuFetchCfg.SendAcks.initialize(true);
  theuFetchCfg.UseReplyChannel.initialize(true);

  theDecoderCfg.Multithread.initialize(false);
  theDecoderCfg.FIQSize.initialize(16);
  theDecoderCfg.DispatchWidth.initialize(8);

  theuArchCfg.Multithread.initialize(false);
  theuArchCfg.ROBSize.initialize(256);
  theuArchCfg.SBSize.initialize(64);
  theuArchCfg.NAWBypassSB.initialize(true);
  theuArchCfg.NAWWaitAtSync.initialize(false);
  theuArchCfg.RetireWidth.initialize(8);
  theuArchCfg.SnoopPorts.initialize(1);
  theuArchCfg.MemoryPorts.initialize(4);
  theuArchCfg.StorePrefetches.initialize(30);
  theuArchCfg.PrefetchEarly.initialize(false);
  theuArchCfg.ConsistencyModel.initialize(1); //TSO
  theuArchCfg.CoherenceUnit.initialize(64);
  theuArchCfg.BreakOnResynchronize.initialize(false);
  theuArchCfg.SpinControl.initialize(true);
  theuArchCfg.SpeculativeOrder.initialize(false);
  theuArchCfg.SpeculateOnAtomicValue.initialize(false);
  theuArchCfg.SpeculateOnAtomicValuePerfect.initialize(false);
  theuArchCfg.SpeculativeCheckpoints.initialize(0);
  theuArchCfg.CheckpointThreshold.initialize(0);
  theuArchCfg.InOrderExecute.initialize(false);
  theuArchCfg.InOrderMemory.initialize(false);
  theuArchCfg.OffChipLatency.initialize(320);
  theuArchCfg.OnChipLatency.initialize(3);

//  theuArchCfg.EarlySGP.initialize(false); // CMU-ONLY
//  theuArchCfg.TrackParallelAccesses.initialize(false); // CMU-ONLY
  theuArchCfg.ValidateMMU.initialize(false);

  static const int K = 1024;

  theL1dCfg.Cores.initialize( 1 );
  theL1dCfg.BlockSize.initialize(64);
  theL1dCfg.Ports.initialize(4);
  theL1dCfg.PreQueueSizes.initialize(4);
  theL1dCfg.TagLatency.initialize(0);
  theL1dCfg.TagIssueLatency.initialize(1);
  theL1dCfg.DataLatency.initialize(2);
  theL1dCfg.DataIssueLatency.initialize(1);
  theL1dCfg.CacheLevel.initialize(eL1);
  theL1dCfg.QueueSizes.initialize(8);
  theL1dCfg.MAFSize.initialize(32);
  theL1dCfg.EvictBufferSize.initialize(8);
  theL1dCfg.SnoopBufferSize.initialize(8);
  theL1dCfg.ProbeFetchMiss.initialize(false);
  theL1dCfg.EvictClean.initialize(true);
  theL1dCfg.BusTime_NoData.initialize(1);
  theL1dCfg.BusTime_Data.initialize(2);
  theL1dCfg.MAFTargetsPerRequest.initialize(0);
  theL1dCfg.FastEvictClean.initialize(false);
  theL1dCfg.NoBus.initialize(false);
  theL1dCfg.Banks.initialize(1);
  theL1dCfg.TraceAddress.initialize(0);
  theL1dCfg.CacheType.initialize("InclMESI2PhaseWB");
  theL1dCfg.ArrayConfiguration.initialize("STD:size=65536:assoc=4:repl=LRU");
  theL1dCfg.EvictOnSnoop.initialize(true);
  theL1dCfg.UseReplyChannel.initialize(true);

  theL2Cfg.Cores.initialize(64);
  theL2Cfg.BlockSize.initialize(64);
  theL2Cfg.Banks.initialize(64);
  theL2Cfg.BankInterleaving.initialize(64);
  theL2Cfg.Groups.initialize(1);
  theL2Cfg.GroupInterleaving.initialize(4096);
  theL2Cfg.DirLatency.initialize(3);
  theL2Cfg.DirIssueLatency.initialize(1);
  theL2Cfg.TagLatency.initialize(3);
  theL2Cfg.TagIssueLatency.initialize(1);
  theL2Cfg.DataLatency.initialize(7);
  theL2Cfg.DataIssueLatency.initialize(1);
  theL2Cfg.QueueSize.initialize(8);
  theL2Cfg.MAFSize.initialize(32);
  theL2Cfg.DirEvictBufferSize.initialize(16);
  theL2Cfg.CacheEvictBufferSize.initialize(16);
  theL2Cfg.Policy.initialize("NonInclusiveMESI");
  theL2Cfg.DirectoryType.initialize("infinite");
  theL2Cfg.DirectoryConfig.initialize("");
  theL2Cfg.CacheLevel.initialize(eL2);
  theL2Cfg.EvictClean.initialize(false);
  theL2Cfg.ArrayConfiguration.initialize("STD:size=1048576:assoc=16:repl=LRU");

  theNicCfg.VChannels.initialize(3);
  theNicCfg.RecvCapacity.initialize(4);
  theNicCfg.SendCapacity.initialize(1);

  //theNetworkCfg.NetworkTopologyFile.initialize("16node-torus.topology");
  theNetworkCfg.NetworkTopologyFile.initialize("64x3-mesh.topology");
  theNetworkCfg.NumNodes.initialize( 3 * getSystemWidth() );
  theNetworkCfg.VChannels.initialize( 3 );

  theNetMapperCfg.Cores.initialize(16);
  theNetMapperCfg.Directories.initialize(16);
  theNetMapperCfg.MemControllers.initialize(4);
  theNetMapperCfg.DirInterleaving.initialize(64);
  theNetMapperCfg.MemInterleaving.initialize(4096);
  theNetMapperCfg.DirLocation.initialize("Distributed");
  theNetMapperCfg.MemLocation.initialize("4,7,8,11");
  theNetMapperCfg.MemReplyToDir.initialize(true);
  theNetMapperCfg.MemAcksNeedData.initialize(true);

  theMemoryCfg.Delay.initialize(240);
  theMemoryCfg.MaxRequests.initialize(128);
  theMemoryCfg.UseFetchReply.initialize(true);

  theMemoryMapCfg.PageSize.initialize(8 * K);
  theMemoryMapCfg.NumNodes.initialize( getSystemWidth() );
  theMemoryMapCfg.RoundRobin.initialize(true);
  theMemoryMapCfg.CreatePageMap.initialize(true);
  theMemoryMapCfg.ReadPageMap.initialize(true);

  theMagicBreakCfg.CkptCycleInterval.initialize(0);
  theMagicBreakCfg.CkptCycleName.initialize(0);
  theMagicBreakCfg.CheckpointOnIteration.initialize(false);
  theMagicBreakCfg.CheckpointEveryXTransactions.initialize(false);
  theMagicBreakCfg.TerminateOnTransaction.initialize(-1);
  theMagicBreakCfg.FirstTransactionIs.initialize(0);
  theMagicBreakCfg.CycleMinimum.initialize(0);
  theMagicBreakCfg.TransactionStatsInterval.initialize(10000);
  theMagicBreakCfg.StopCycle.initialize(0);
  theMagicBreakCfg.EnableTransactionCounts.initialize(false);
  theMagicBreakCfg.TransactionType.initialize(0);
  theMagicBreakCfg.TerminateOnIteration.initialize(-1);
  theMagicBreakCfg.TerminateOnMagicBreak.initialize(-1);
  theMagicBreakCfg.EnableIterationCounts.initialize(false);

  return true; //Abort simulation if parameters are not initialized
}

#include FLEXUS_END_COMPONENT_CONFIGURATION_SECTION()


#include FLEXUS_BEGIN_COMPONENT_INSTANTIATION_SECTION()
//All component Instances are created here.  This section
//also creates handles for each component
FLEXUS_INSTANTIATE_COMPONENT( WhiteBox, theWhiteBoxCfg, theWhiteBox );

FLEXUS_INSTANTIATE_COMPONENT_ARRAY( FetchAddressGenerate, theFAGCfg, theFAG, SCALE_WITH_SYSTEM_WIDTH, MULTIPLY, 1);
FLEXUS_INSTANTIATE_COMPONENT_ARRAY( uFetch, theuFetchCfg, theuFetch, SCALE_WITH_SYSTEM_WIDTH, MULTIPLY, 1);
FLEXUS_INSTANTIATE_COMPONENT_ARRAY( PortCombiner, theCombinerCfg, theuFetchCombiner, SCALE_WITH_SYSTEM_WIDTH, MULTIPLY, 1);
FLEXUS_INSTANTIATE_COMPONENT_ARRAY( v9Decoder, theDecoderCfg, theDecoder, SCALE_WITH_SYSTEM_WIDTH, MULTIPLY, 1);
FLEXUS_INSTANTIATE_COMPONENT_ARRAY( uArch, theuArchCfg, theuArch, SCALE_WITH_SYSTEM_WIDTH, MULTIPLY, 1);
FLEXUS_INSTANTIATE_COMPONENT_ARRAY( Cache, theL1dCfg, theL1d, SCALE_WITH_SYSTEM_WIDTH, MULTIPLY, 1);
FLEXUS_INSTANTIATE_COMPONENT_ARRAY( CMPCache, theL2Cfg, theL2, SCALE_WITH_SYSTEM_WIDTH, DIVIDE, 1 );
FLEXUS_INSTANTIATE_COMPONENT_ARRAY( MemoryLoopback, theMemoryCfg, theMemory, SCALE_WITH_SYSTEM_WIDTH, DIVIDE, 4 );
FLEXUS_INSTANTIATE_COMPONENT_ARRAY( MultiNic2, theNicCfg, theNic, SCALE_WITH_SYSTEM_WIDTH, MULTIPLY, 3 );
FLEXUS_INSTANTIATE_COMPONENT( MemoryNetwork, theNetworkCfg, theNetwork );
FLEXUS_INSTANTIATE_COMPONENT( MemoryMap, theMemoryMapCfg, theMemoryMap );
FLEXUS_INSTANTIATE_COMPONENT( MagicBreak, theMagicBreakCfg, theMagicBreak );
FLEXUS_INSTANTIATE_COMPONENT( SplitDestinationMapper, theNetMapperCfg, theNetMapper );

#include FLEXUS_END_COMPONENT_INSTANTIATION_SECTION()



#include FLEXUS_BEGIN_COMPONENT_WIRING_SECTION()

//FROM                                  TO
//====                                  ==
//FAG to Fetch
WIRE( theFAG, FetchAddrOut,             theuFetch, FetchAddressIn         )
WIRE( theFAG, AvailableFAQ,             theuFetch, AvailableFAQOut        )

//Fetch to Decoder
WIRE( theuFetch, AvailableFIQ,          theDecoder, AvailableFIQOut       )
WIRE( theuFetch, FetchBundleOut,        theDecoder, FetchBundleIn         )
WIRE( theDecoder, SquashOut,            theuFetch, SquashIn               )
WIRE( theuArch, ChangeCPUState,         theuFetch, ChangeCPUState         )

//Decoder to uArch
WIRE( theDecoder, AvailableDispatchIn,  theuArch, AvailableDispatchOut    )
WIRE( theDecoder, DispatchOut,          theuArch, DispatchIn              )
WIRE( theuArch, SquashOut,              theDecoder, SquashIn              )

//uArch to FAG
WIRE( theuArch, BranchFeedbackOut,      theFAG, BranchFeedbackIn          )
WIRE( theuArch, RedirectOut,            theFAG, RedirectIn                )

//uFetch to IDMux
WIRE( theuFetch, FetchMissOut,          theNetMapper, ICacheRequestIn     )
WIRE( theuFetch, FetchSnoopOut,         theNetMapper, ICacheSnoopIn       )
WIRE( theuFetch, FetchReplyOut,         theNetMapper, ICacheReplyIn       )

//uArch to L1 D cache
WIRE( theuArch, MemoryOut_Request,      theL1d, FrontSideIn_Request       )
WIRE( theuArch, MemoryOut_Snoop,        theL1d, FrontSideIn_Snoop         )
WIRE( theL1d, FrontSideOut_D,           theuArch, MemoryIn               )

WIRE( theNetMapper, ICacheSnoopOut,     theuFetchCombiner, SnoopIn         )
WIRE( theNetMapper, ICacheReplyOut,     theuFetchCombiner, ReplyIn         )

WIRE( theuFetchCombiner, FetchMissOut,  theuFetch, FetchMissIn             )

//L1d to NetMapper
WIRE( theL1d, BackSideOut_Request,       theNetMapper, CacheRequestIn      )
WIRE( theL1d, BackSideOut_Snoop,         theNetMapper, CacheSnoopIn        )
WIRE( theL1d, BackSideOut_Reply,         theNetMapper, CacheReplyIn        )
WIRE( theNetMapper, CacheSnoopOut,      theL1d, BackSideIn_Request         )
WIRE( theNetMapper, CacheReplyOut,      theL1d, BackSideIn_Reply           )

//Memory to NetMapper
WIRE( theNetMapper, MemoryOut,          theMemory, LoopbackIn             )
WIRE( theMemory, LoopbackOut,           theNetMapper, MemoryIn            )

//Directory to NetMapper
WIRE( theNetMapper, DirReplyOut,        theL2, Reply_In            )
WIRE( theNetMapper, DirSnoopOut,        theL2, Snoop_In            )
WIRE( theNetMapper, DirRequestOut,      theL2, Request_In          )
WIRE( theL2, Reply_Out,          theNetMapper, DirReplyIn          )
WIRE( theL2, Snoop_Out,          theNetMapper, DirSnoopIn          )
WIRE( theL2, Request_Out,        theNetMapper, DirRequestIn        )

//NetMapper to Nic
WIRE( theNetMapper, ToNIC0,             theNic, FromNode0          )
WIRE( theNic, ToNode0,                  theNetMapper, FromNIC0     )
WIRE( theNetMapper, ToNIC1,             theNic, FromNode1          )
WIRE( theNic, ToNode1,                  theNetMapper, FromNIC1     )

//Nodes to Network
WIRE( theNic, ToNetwork,                theNetwork, FromNode              )
WIRE( theNetwork, ToNode,               theNic, FromNetwork               )

#include FLEXUS_END_COMPONENT_WIRING_SECTION()


#include FLEXUS_BEGIN_DRIVE_ORDER_SECTION()

DRIVE( theuFetch, uFetchDrive )
, DRIVE( theFAG, FAGDrive )
, DRIVE( theuArch, uArchDrive )
, DRIVE( theDecoder, DecoderDrive )
, DRIVE( theNic, MultiNicDrive )
, DRIVE( theNetwork, NetworkDrive )
, DRIVE( theMemory, LoopbackDrive )
, DRIVE( theL2, CMPCacheDrive )
, DRIVE( theL1d, CacheDrive )
, DRIVE( theMagicBreak, TickDrive )

#include FLEXUS_END_DRIVE_ORDER_SECTION()

