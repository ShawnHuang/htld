#include<iostream>
#include<fstream>
#include<map>
#include "utilities.hpp"
#include "helper_cuda.h"

typedef std::map<int, cudaDeviceProp*> dev_props_map;
typedef std::pair<int, cudaDeviceProp*> dev_props_pair;
static dev_props_map* _dev_props = NULL;

std::string format(const char* format, ...) {

  int size     = 512;
    char* buffer = 0;
    buffer       = new char[size];
    va_list vl;
    va_start(vl, format);
    int nsize = vsnprintf(buffer, size, format, vl);
    if(size<=nsize){ //fail delete buffer and try again
        delete[] buffer;
        buffer = 0;
        buffer = new char[nsize+1]; //+1 for /0
        nsize  = vsnprintf(buffer, size, format, vl);
    }
    std::string ret(buffer);
    va_end(vl);
    delete[] buffer;
    return ret;
}

void getCPUProperties(CPU_PROPS* cpu_props) {
  //Set CPU Properties...
  cpu_props->numa_node_count          = 1;
  cpu_props->physical_proc_pack_count = 0;
  cpu_props->physical_proc_cores      = 1;
  cpu_props->logical_proc_cores       = 8;
  cpu_props->num_of_l1_cache          = 2;
  cpu_props->num_of_l2_cache          = 1;
  cpu_props->num_of_l3_cache          = 1;
}
int getNumOfDevices() {

  int device_count = 0;
  cudaGetDeviceCount(&device_count);
  return device_count;
}

cudaDeviceProp* getDeviceProps(int dev_num) {

  cudaDeviceProp* dev_prop;
  cudaError_t status;

  if(_dev_props == NULL)
    _dev_props = new dev_props_map();
  //Device Has Not Been Queried Yet...
  if(_dev_props->find(dev_num) == _dev_props->end()) {
    dev_prop = (cudaDeviceProp*)malloc(sizeof(cudaDeviceProp));
    status   = cudaGetDeviceProperties(dev_prop, dev_num);
    if(status != cudaSuccess) {
      fprintf(stderr, 
            "Cuda Device Query for Device %d Has Failed!",
          dev_num);
      return NULL;
    } else
      _dev_props->insert(dev_props_pair(dev_num, dev_prop));
  } else 
    dev_prop = _dev_props->find(dev_num)->second;
  return dev_prop;
}

int getNumOfCUDACoresPerSM(int dev_num) {

  int num_of_cores;
  cudaDeviceProp* dev_prop = _dev_props != NULL ? 
                           _dev_props->find(dev_num)->second : 
                             getDeviceProps(dev_num);

  if(dev_prop != NULL) {
    num_of_cores = _ConvertSMVer2Cores(dev_prop->major, dev_prop->minor);
  } else { 
    num_of_cores = -1;
    fprintf(stderr, 
        "getNumOfCUDACoresPerSM for Device %d Has Failed!\n No Such Device!",
        dev_num);
  }

  return num_of_cores;
}

int findOptimalNumOfTPB(int dev_num) {

  int n;
  cudaDeviceProp* props = getDeviceProps(dev_num);

  n = props->maxThreadsPerBlock;
  while(n >= 64) {
    if(props->maxThreadsPerMultiProcessor % n == 0) 
      break;
    else 
      n -= 32;
  }//End of while-Loop...

  return n;
}
