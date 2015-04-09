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
#include <fstream>
#include <sstream>
#include <cmath>

#include <boost/regex.hpp>
#include <boost/throw_exception.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

#include <core/target.hpp>
#include <core/types.hpp>
#include <core/simics/api_wrappers.hpp>

// added by PLotfi
#include <core/configuration.hpp>
// end PLotfi

#include "mai_api.hpp"

namespace Flexus {
namespace Simics {

void onInterrupt (void * aPtr, Simics::API::conf_object_t * anObj, long long aVector);

struct InterruptManager {
  int theHandle;
  typedef std::map< Simics::API::conf_object_t *, boost::function< void( long long) > > dispatch_map;
  dispatch_map theInterruptDispatch;

  void registerHAP() {
    theHandle = API::SIM_hap_add_callback( "Core_Asynchronous_Trap", reinterpret_cast< API::obj_hap_func_t>(&onInterrupt), 0);
  }

  void registerCPU(API::conf_object_t * aCPU, boost::function< void( long long) > aDispatchFn ) {
    if (theInterruptDispatch.empty()) {
      registerHAP();
    }
    theInterruptDispatch.insert( std::make_pair( aCPU, aDispatchFn) );
  }

  void interrupt(API::conf_object_t * anObj, long long aVector ) {
    dispatch_map::iterator iter = theInterruptDispatch.find(anObj);
    if (iter != theInterruptDispatch.end()) {
      iter->second( aVector );
    }
  }

} theInterruptManager;

void onInterrupt (void * aPtr, API::conf_object_t * anObj, long long aVector) {
  theInterruptManager.interrupt( anObj, aVector );
}

#if FLEXUS_TARGET_IS(v9)

void v9ProcessorImpl::initialize() {
  DBG_( Dev, ( << "CPU[" << Simics::APIFwd::SIM_get_processor_number(*this) << "] Registering for interrupts "));
  theInterruptManager.registerCPU( *this, ll::bind( &v9ProcessorImpl::handleInterrupt, this, ll::_1 ) );
}

void v9ProcessorImpl::handleInterrupt( long long aVector) {
  DBG_( Iface, ( << "CPU[" << Simics::APIFwd::SIM_get_processor_number(*this) << "] Interrupt " << std::hex << aVector << std::dec ));
  thePendingInterrupt = aVector;
}

static const int kTL = 46;
static const int kTT1 = 78;

int v9ProcessorImpl::getPendingException() const {
  int tl = readRegister( kTL );
  if (tl > 0) {
    return readRegister( kTT1 + tl - 1);
  } else {
    return 0;
  }
}

int v9ProcessorImpl::getPendingInterrupt() const {
  if (thePendingInterrupt == API::Sim_PE_No_Exception) {
    return 0;
  }
  return thePendingInterrupt;
}

int v9ProcessorImpl::advance(bool anAcceptInterrupt) {
  int exception = 0;

  if (! theInterruptsConnected) {
    theInterruptsConnected = true;
    initialize();
  }

  if (thePendingInterrupt != API::Sim_PE_No_Exception && anAcceptInterrupt) {
    //Try to take the interrupt now.
    int interrupt = thePendingInterrupt;
    switch (API::SIM_instruction_handle_interrupt(*this, thePendingInterrupt)) {
      case API::Sim_IE_No_Exception:
        thePendingInterrupt = API::Sim_PE_No_Exception;
        DBG_( Crit, ( << "Got Sim_IE_No_Exception trying to take an interrupt" ) );
        break;
      case API::Sim_IE_OK:
        thePendingInterrupt = API::Sim_PE_No_Exception;
        return interrupt;
      case API::Sim_IE_Interrupts_Disabled:
        // OK try later */
        DBG_( Crit, ( << "Got Sim_IE_Interruts_Disabled trying to take an interrupt on CPU[" << Simics::APIFwd::SIM_get_processor_number(*this) << "]"  ) );
        return -1;
      case API::Sim_IE_Illegal_Interrupt_Point:
        // OK try later */
        DBG_( Crit, ( << "Got Sim_IE_Illegal_Interrupt_Point trying to take an interrupt" ) );
        return -1;
      case API::Sim_IE_Step_Breakpoint:
        //Return to prompt next cycle
        API::SIM_break_cycle(*this, 0);
        break;
      default:
        DBG_( Crit, ( << "Bad return value from SIM_instruction_handle_interrupt" ) );
    }
  }

  int except = API::SIM_get_pending_exception();
  if (except != API::SimExc_No_Exception) {
    DBG_( Crit, ( << "Pending Exception: " << except ) );
  }

  //Normal instruction processing
  API::instruction_id_t inst = API::SIM_instruction_begin(*this);
  API::SIM_instruction_insert(0, inst);

  bool retry = false;
  do {
    retry = false;
    API::instruction_error_t err = API::SIM_instruction_commit(inst);
    if (err == API::Sim_IE_Exception ) {
      err = API::SIM_instruction_handle_exception(inst);
      exception = getPendingException();
      DBG_( Verb, ( << "Exception raised by CPU[" << Simics::APIFwd::SIM_get_processor_number(*this) << "]: " << err << " Exception #:" << exception) );
      API::SIM_instruction_end(inst);
      if (err != API::Sim_IE_OK) {
        DBG_( Crit, ( << "Got an error trying to handle exception on CPU[" << Simics::APIFwd::SIM_get_processor_number(*this) << ": " << err << " exception was: " << exception) );
      }
      return exception; //Instruction is already ended
    } else if (err == API::Sim_IE_Code_Breakpoint || err == API::Sim_IE_Step_Breakpoint || err == API::Sim_IE_Hap_Breakpoint ) {
      DBG_( Verb, ( << "Triggerred Breakpoint on CPU[" << Simics::APIFwd::SIM_get_processor_number(*this) << "]" ) );
      API::SIM_break_cycle(*this, 0);
      retry = true;
    } else if (err != API::Sim_IE_OK) {
      DBG_( Crit, ( << "Got an error trying to advance CPU[" << Simics::APIFwd::SIM_get_processor_number(*this) << "]: " << err) );
      //Some error other than an exception
    }
  } while ( retry );
  API::SIM_instruction_end(inst);

  return exception;
}

unsigned long long endianFlip(unsigned long long val, int aSize);

unsigned long long v9ProcessorImpl::interruptRead(VirtualMemoryAddress anAddress, int anASI) const {
  DBG_( Verb, ( << "Reading " << anAddress << " in ASI: " << std::hex << anASI << std::dec ) );
  API::logical_address_t addr(anAddress);
  unsigned long long aValue = 0;
  API::v9_memory_transaction_t xact;
  memset( &xact, 0, sizeof(API::v9_memory_transaction_t ) );
  xact.priv = 1;
#if SIM_VERSION < 1200
  xact.align_kind = API::Align_Natural;
#else
  //align_kind was replaced by access_type in Simics 2.2.x
  xact.access_type = API::V9_Access_Normal;
#endif
  xact.address_space = anASI;
  xact.s.logical_address = addr;
  xact.s.physical_address = addr;
  xact.s.size = 8;
  xact.s.type = API::Sim_Trans_Load;
  xact.s.inquiry = 1;
  xact.s.ini_type = API::Sim_Initiator_CPU_UIII;
  xact.s.ini_ptr = theProcessor;
  xact.s.real_address = reinterpret_cast<char *>(&aValue);
  xact.s.exception = API::Sim_PE_No_Exception;

  API::exception_type_t except = sparc()->access_asi_handler(theProcessor, &xact);
  if (except != API::Sim_PE_No_Exception) {
    DBG_( Dev, ( << "except: " << except  ) );
  }
  return endianFlip(aValue, 8);
}

#endif //FLEXUS_TARGET_IS(v9)

ProcessorMapper * ProcessorMapper::theMapper = NULL;

struct cpu_desc_t {
  int vm;
  int cpu;
  int simics_id;
  bool is_client;
  cpu_desc_t(int v, int c, int s, bool client) : vm(v), cpu(c), simics_id(s), is_client(client) {}
};

struct cpu_desc_sorter_t {
  bool operator()(const cpu_desc_t & a, const cpu_desc_t & b) const {
    if (a.vm == b.vm) {
      if (a.cpu == b.cpu) {
        return (b.is_client);
      } else {
        return (a.cpu < b.cpu);
      }
    } else {
      return (a.vm < b.vm);
    }
  }
};

ProcessorMapper::ProcessorMapper() {
  // Find the Processors, Find All of them

  // Clear all exceptions so that get_all_processors works properly.

  API::SIM_clear_exception();

  API::attr_value_t proc_list = API::SIM_get_all_processors();
  DBG_Assert( proc_list.kind == API::Sim_Val_List, ( << "SIM_get_all_processors() did NOT return a LIST" ));
  int proc_count = proc_list.u.list.size;
  std::vector<cpu_desc_t> cpu_list;
  int num_flexus_cpus = 0;
  int num_client_cpus = 0;
  // added by PLotfi to support SPECweb2009 workloads
  int num_besim_cpus = 0;
  // end PLotfi
  int max_vm = -1;

  // changed by PLotfi
  boost::regex vm_expression("machine([0-9]*)_((?:)|(?:server_)|(?:client_)|(?:besim_))cpu([0-9]*)");
  //boost::regex vm_expression("machine([0-9]*)_((?:)|(?:server_)|(?:client_))cpu([0-9]*)");
  boost::regex non_vm_expression("((?:)|(?:server_)|(?:client_)|(?:besim_))cpu([0-9]*)");
  //boost::regex non_vm_expression("((?:)|(?:server_)|(?:client_))cpu([0-9]*)");
  // end PLotfi
  DBG_(Crit, ( << "Searching " << proc_count << " cpus." ));
  for (int i = 0; i < proc_count; i++) {
    API::conf_object_t * cpu = proc_list.u.list.vector[i].u.object;
    int simics_id = APIFwd::SIM_get_processor_number(cpu);
    DBG_(Crit, ( << "Processor " << i << ": " << cpu->name << " - CPU " << simics_id ));

    boost::cmatch what;
    if (boost::regex_match(cpu->name, what, vm_expression)) {
      int vm = boost::lexical_cast<int>(what[1]);
      int cpu = boost::lexical_cast<int>(what[3]);
      bool is_client = false;
      DBG_Assert(max_vm >= 0 || (num_flexus_cpus == 0 && num_client_cpus == 0), ( << "Combined VM/Non-VM workload detected: max_vm = " << max_vm << ", flexus = " << num_flexus_cpus << ", clients = " << num_client_cpus << ", vm = " << vm ));
      if (what[2] == "client_") {
        is_client = true;
        num_client_cpus++;
      } else if (what[2] == "besim_") {
        is_client = true;
        num_besim_cpus++;
      } else {
        num_flexus_cpus++;
      }
      if (vm > max_vm) {
        max_vm = vm;
      }
      cpu_list.push_back(cpu_desc_t(vm, cpu, simics_id, is_client));
    } else if (boost::regex_match(cpu->name, what, non_vm_expression)) {
      DBG_(Crit, ( << "Found CPU: '" << what[1] << "' - " << what[2] ));
      int vm = 0;
      int cpu = boost::lexical_cast<int>(what[2]);
      bool is_client = false;
      if (what[1] == "client_") {
        is_client = true;
        num_client_cpus++;
      } else if (what[1] == "besim_") {
        is_client = true;
        num_besim_cpus++;
      } else {
        num_flexus_cpus++;
      }
      DBG_Assert(max_vm == -1);
      cpu_list.push_back(cpu_desc_t(vm, cpu, simics_id, is_client));
    }
  }
  // Don't forget to free the processor list
  SIM_free_attribute(proc_list);

  DBG_(Crit, ( << "Found " << num_flexus_cpus << " Flexus CPUs and " << num_client_cpus << " Client CPUs and " << num_besim_cpus << " Besim CPUs in " << (max_vm + 1) << " VMs"));
  theProcMap.resize(num_flexus_cpus, std::make_pair(0, 0));
  theClientMap.resize(num_client_cpus + num_besim_cpus, 0);
  theReverseMap.resize(proc_count, std::make_pair(0, false));

  int client_index = 0;
  int flexus_index = 0;

  std::sort(cpu_list.begin(), cpu_list.end(), cpu_desc_sorter_t());

  std::vector<cpu_desc_t>::iterator iter = cpu_list.begin();
#if 0
  for (; iter != cpu_list.end(); iter++) {
    theReverseMap[iter->simics_id] = std::make_pair(flexus_index, iter->is_client);
    if (iter->is_client) {
      theClientMap[client_index] = iter->simics_id;
      DBG_(Verb, ( << "Client[" << client_index << "] = " << iter->simics_id ));
      client_index++;
    } else {
      theProcMap[flexus_index].first = iter->simics_id;
      theProcMap[flexus_index].second = iter->vm;
      DBG_(Verb, ( << "theProcMap[" << flexus_index << "] = (" << iter->simics_id << ", " << iter->vm << ")" ));
      flexus_index++;
    }
  }
#endif
  theNumVMs = max_vm + 1;
  if (theNumVMs == 0) {
    theNumVMs = 1;
  }

  Flexus::Core::ConfigurationManager::getParameterValue(":threads", false);
  Flexus::Core::ConfigurationManager::getParameterValue("-mapper:manual_organization");
  Flexus::Core::ConfigurationManager::getParameterValue("-mapper:organization");
  // commented by PLotfi
/*
  std::string line;
  std::string str ("\"");
  std::string str2 (":threads\"");
  std::string str3 ("-mapper:manual_organization\"");
  std::string str4 ("-mapper:organization\"");
  std::ifstream myfile ("user-postload.simics");
  std::stringstream ss (std::stringstream::in | std::stringstream::out);
*/
  char *charThreads,*charManual;
  int found,start,end,numThreads,manual;

/*
  // We read the user-postload.simics to read the number of threads and the way of manual organization of the VMs
  if (myfile.is_open()){
    DBG_(Dev, ( << "Reading user-postload.simics"));
    while ( myfile.good() )
    {
      getline (myfile,line);
      ss << line << std::endl;
    }
    myfile.close();
  }
  else
    DBG_Assert(false, ( << "Cannot read the user-postload.simics"));
*/

  // Search for the number of threads and read the value of this parameter
  std::string strThreads = Flexus::Core::ConfigurationManager::getParameterValue(":threads", false);
  if (strThreads == "not_found")
    numThreads = 1;
  else
    numThreads = atoi(strThreads.c_str()); 
/*
  found=(ss.str()).find(str2)+str2.length();
  start=(ss.str()).find(str,found);
  end=(ss.str()).find(str,start+1);
  std::string strThreads = (ss.str()).substr(start+1,end-start-1);
  charThreads = new char[strThreads.length() + 1];
  strcpy(charThreads, strThreads.c_str());
  numThreads = atoi(charThreads);
  delete charThreads; 
*/
  DBG_Assert(numThreads > 0, ( << "Number of threads per core: " << numThreads));

  // Search whether we have manual organization or not
  std::string strManual = Flexus::Core::ConfigurationManager::getParameterValue("-mapper:manual_organization");
  if (strManual == "not_found")
    manual = 0;
  else
    manual = atoi(strManual.c_str());

/*
  found=(ss.str()).find(str3)+str3.length();
  start=(ss.str()).find(str,found);
  end=(ss.str()).find(str,start+1);
  std::string strManual = (ss.str()).substr(start+1,end-start-1);
  charManual = new char[strManual.length() + 1];
  strcpy(charManual, strManual.c_str());
  manual = atoi(charManual);
  delete charManual;
*/
    
  int num_rows=0,cores_per_vm=0, sqrt_cores_per_vm=0, sqrt_numVMs=0, vms_per_row=0,vms_per_col=0,num_vm_rows=0,num_vm_cols=0;
  std::vector<int> *machines = new std::vector<int>[theNumVMs];
  std::vector<int> organization(num_flexus_cpus/numThreads);

  if (manual == 0){
    DBG_(Dev, ( << "Automatic organization of equal-size VMs"));
    cores_per_vm = (num_flexus_cpus/numThreads) / theNumVMs;
    sqrt_cores_per_vm = (int)std::ceil(std::sqrt(cores_per_vm));
    sqrt_numVMs = (int)std::ceil(std::sqrt(theNumVMs));
    if (sqrt_numVMs*sqrt_numVMs == theNumVMs){ // Square number of VMs
      vms_per_row = (int)std::ceil(std::sqrt(theNumVMs));
      vms_per_col = vms_per_row;
      DBG_(Crit, ( << "Square topology of VMs: " << vms_per_col << "x" << vms_per_row ));
      if (sqrt_cores_per_vm*sqrt_cores_per_vm == cores_per_vm){ // Square number of cores per VM
        num_vm_rows = (int)std::ceil(std::sqrt(cores_per_vm));
        num_vm_cols = num_vm_rows;
        DBG_(Crit, ( << "Square VM: " << num_vm_rows << "x" << num_vm_cols ));
      } 
      else { // Non square number of cores per VM. Dividing by 2 it becomes square number. 
             // e.g. 32 --> 32/2 = 16 and hence 8 rows of 4 cores each.  
  	     // so the VMs are mapped correctly according to the chip topology. 	
        num_vm_cols = (int) std::ceil(std::sqrt(cores_per_vm/2));
        num_vm_rows = cores_per_vm/num_vm_cols;
        DBG_(Crit, ( << "Rectangular VM: " << num_vm_rows << "x" << num_vm_cols ));
      }
    }  
    else { // Non square number of VMs. Dividing by it becomes square number.
  	   // e.g. 8 --> 8/2 = 4 and hence 2 rows of 4 VMs each. 
      vms_per_row = (int)std::ceil(std::sqrt(theNumVMs/2));
      vms_per_col = theNumVMs/vms_per_row;
      DBG_(Crit, ( << "Rectangular topology of VMs (rowsXcols): " << vms_per_col << "x" << vms_per_row ));
      if (sqrt_cores_per_vm*sqrt_cores_per_vm == cores_per_vm){ // Square number of cores per VM
        num_vm_rows = (int)std::ceil(std::sqrt(cores_per_vm));
        num_vm_cols = num_vm_rows;
        DBG_(Crit, ( << "Square VM: " << num_vm_rows << "x" << num_vm_cols ));
      }
      else { // Non square number of cores per VM. Dividing by 2 it becomes square number. 
  	     // e.g. 32 --> 32/2 = 16 and hence 4 rows of 8 cores each.  
        num_vm_rows = (int) std::ceil(std::sqrt(cores_per_vm/2));
        num_vm_cols = cores_per_vm/num_vm_rows;
        DBG_(Crit, ( << "Rectangular VM: " << num_vm_rows << "x" << num_vm_cols ));
      }
    }
    num_rows = num_vm_rows*vms_per_col;
    DBG_(Crit, ( << "VMS per row = " << vms_per_row << ", CPVM = " << cores_per_vm << ", GridOfVM = " << num_vm_cols << "*" << num_vm_rows << ", NumRows = " << num_rows ));
  }
  else {
    DBG_(Dev, ( << "Manual organization of VMs according to the configuration file"));
    int i=0,j=0;
    // Search for the organization parameter
    std::string strOrg = Flexus::Core::ConfigurationManager::getParameterValue("-mapper:organization"); 
    DBG_Assert(strOrg != "not_found", ( << "-mapper:organization parameter does not exist in the configuration file" )); 
/*
    found=(ss.str()).find(str4)+str4.length();
    start=(ss.str()).find(str,found);
    end=(ss.str()).find(str,start+1);
    std::string strOrg = (ss.str()).substr(start+1,end-start-1);
*/
    char *charOrg = new char[strOrg.length()+1];
    strcpy(charOrg, strOrg.c_str());
    std::string *tokens = new std::string[theNumVMs];
    char *temp=strtok(charOrg,";");
    while (temp!=NULL){
      tokens[i++]=temp;
      temp = strtok (NULL, ";");
    }
    delete charOrg;
    // the organization vector will have the new indices and the machine vector will have them in muliple rows; one for each VM
    for (i=0;i<theNumVMs;i++){
      char *charMachine = new char[tokens[i].length()+1];
      strcpy(charMachine,tokens[i].c_str());
      temp=strtok(charMachine,",");
      while (temp!=NULL){
        organization[j++]=atoi(temp);
        machines[i].push_back(atoi(temp));
        temp = strtok (NULL,",");
      }
      delete charMachine;
    }
  }

  for(; iter != cpu_list.end(); iter++) {
    theReverseMap[iter->simics_id] = std::make_pair(flexus_index, iter->is_client);
    if (iter->is_client) {
      theReverseMap[iter->simics_id] = std::make_pair(-1, iter->is_client);
      theClientMap[client_index] = iter->simics_id;
      DBG_(Crit, ( << "Client[" << client_index << "] = " << iter->simics_id ));
      client_index++;
    } 
    else {
      if (manual==0){ // re-map flexus index, so VMs are organized in a nice grid
        int core_index = flexus_index / numThreads;
        int vm = core_index / cores_per_vm;
        int core = core_index % cores_per_vm;
        int row_in_vm = core / (num_vm_cols);
        int col_in_vm = core % (num_vm_cols);
        int row_of_vm = vm / vms_per_row;
        int col_of_vm = vm % vms_per_row;
        int abs_row = row_in_vm + row_of_vm*num_vm_rows;
        int abs_col = col_in_vm + col_of_vm*num_vm_cols;
        int new_index = abs_row*(num_flexus_cpus/num_rows) + abs_col*numThreads+flexus_index%numThreads;
        theReverseMap[iter->simics_id] = std::make_pair(new_index, iter->is_client);
        theProcMap[new_index].first = iter->simics_id;
        theProcMap[new_index].second = iter->vm;
        DBG_(Crit, ( << "theProcMap[" << new_index << "] = (" << iter->simics_id << ", " << iter->vm << ") (abs_index = " << flexus_index << ")" ));
        flexus_index++;
      }
      else { // re-map flexus index, so VMs are organized according to user needs
        int new_index = organization[flexus_index/numThreads]*numThreads+flexus_index%numThreads;
        theReverseMap[iter->simics_id] = std::make_pair(new_index, iter->is_client);
        theProcMap[new_index].first = iter->simics_id;
        theProcMap[new_index].second = iter->vm;
        DBG_(Crit, ( << "theProcMap[" << new_index << "] = (" << iter->simics_id << ", " << iter->vm << ") (abs_index = " << flexus_index << ")" ));
        flexus_index++;
      }
    }
  }
  delete [] machines;
  DBG_(Dev, ( << "Finished creating Processor Mapper."));
  API::SIM_clear_exception();
}

int ProcessorMapper::mapFlexusIndex2ProcNum(int index) {
  if (theMapper == NULL) {
    theMapper = new ProcessorMapper();
  }
  return theMapper->theProcMap[index].first;
}

int ProcessorMapper::mapClientNum2ProcNum(int index) {
  if (theMapper == NULL) {
    theMapper = new ProcessorMapper();
  }
  return theMapper->theClientMap[index];
}

int ProcessorMapper::mapProcNum2FlexusIndex(int index) {
  if (theMapper == NULL) {
    theMapper = new ProcessorMapper();
  }
  return theMapper->theReverseMap[index].first;
}

int ProcessorMapper::mapFlexusIndex2VM(int index) {
  if (theMapper == NULL) {
    theMapper = new ProcessorMapper();
  }
  if (theMapper->theNumVMs == 0) {
    return 0;
  }
  return theMapper->theProcMap[index].second;
}

int ProcessorMapper::numVMs() {
  if (theMapper == NULL) {
    theMapper = new ProcessorMapper();
    DBG_Assert(theMapper != NULL);
  }
  return theMapper->theNumVMs;
}

int ProcessorMapper::numClients() {
  if (theMapper == NULL) {
    theMapper = new ProcessorMapper();
  }
  return (int)(theMapper->theClientMap.size());
}

int ProcessorMapper::numProcessors() {
  if (theMapper == NULL) {
    theMapper = new ProcessorMapper();
  }
  return (int)(theMapper->theProcMap.size());
}

} //end Namespace Simics
} //end namespace Flexus

