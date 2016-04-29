#ifndef UTILTIES_H_
#define UTILTIES_H_

//#include<Windows.h>
#include<string>
#include<cstdarg>
#include "cuda.h"
#include "cuda_runtime.h"
#include "global_declarations.hpp"
#include <math.h>

//#define round(fp) (int)((fp) >= 0 ? (fp) + 0.5 : (fp) - 0.5)
#define round(fp) (fp<0?ceil((fp)-0.5):floor((fp)+0.5))

//********************************************************************************
//**************************Identifying Processor...******************************
//********************************************************************************
struct cpu_props {

  int numa_node_count;
  int physical_proc_pack_count;
  int physical_proc_cores;
  int logical_proc_cores;
  int num_of_l1_cache;
  int num_of_l2_cache;
  int num_of_l3_cache;
};

typedef struct cpu_props CPU_PROPS;

HETLD_API std::string format(const char* format, ...);
HETLD_API void getCPUProperties(CPU_PROPS* cpu_props);

HETLD_API int getNumOfDevices();
HETLD_API cudaDeviceProp* getDeviceProps(int dev_num);
HETLD_API int getNumOfCUDACoresPerSM(int dev_num);
//Num of Threads Per Block With Respect to SM's Max # of Threads...
HETLD_API int findOptimalNumOfTPB(int dev_num);

#endif
