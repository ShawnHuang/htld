############################################################################
# <summary> CMakeLists.txt for OpenCV and CUDA. </summary>
# <date>    2012-02-07          </date>
# <author>  Quan Tran Minh. edit by Johannes Kast, Michael Sarahan </author>
# <email>   quantm@unist.ac.kr  kast.jo@googlemail.com msarahan@gmail.com</email>
############################################################################

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(cs344)

find_package(OpenCV REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenMP REQUIRED)

link_libraries(${OpenCV_LIBS})

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")

if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if(CUDA_FOUND)
  # compared to class settings, we let NVidia's FindCUDA CMake detect 
  # whether to build x64.  We tell it to support most devices, though, 
  # to make sure more people can easily run class code without knowing 
  # about this compiler argument
  #-ccbin /usr/bin/clang; 
  set(CUDA_NVCC_FLAGS "
  -gencode;arch=compute_30,code=sm_30;  
  -gencode;arch=compute_35,code=sm_35;
  -gencode;arch=compute_35,code=compute_35;
  -gencode;arch=compute_20,code=sm_20; 
  -gencode;arch=compute_11,code=sm_11; 
  -gencode;arch=compute_12,code=sm_12;
  -gencode;arch=compute_13,code=sm_13;")

  # add -Wextra compiler flag for gcc compilations
  if (UNIX)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} "-Xcompiler -Wextra")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_definitions (-std=c++11 -g3)
  endif (UNIX)

  # add debugging to CUDA NVCC flags.  For NVidia's NSight tools.
  set(CUDA_NVCC_FLAGS_DEBUG ${CUDA_NVCC_FLAGS_DEBUG} "-G")
  set(USE_HTLD)

  #file( GLOB  hdr *.hpp *.h )
  #file( GLOB  cu  *.cu)
  #SET (HW1_files main.cpp reference_calc.cpp compare.cpp)

  include_directories (
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/main
    ${PROJECT_SOURCE_DIR}/libtld
    ${PROJECT_SOURCE_DIR}/imacq
    ${PROJECT_SOURCE_DIR}/libtld/mftracker
    ${PROJECT_SOURCE_DIR}/heterogenous_tld_lib
    ${PROJECT_SOURCE_DIR}/3rdParties/libconfig/lib
    ${PROJECT_SOURCE_DIR}/3rdParties/src/cub-1.3.1
    /home/shawn/Dev/NVIDIA_CUDA-5.5_Samples/NVIDIA_CUDA-5.5_Samples/common/inc
    /usr/local/cuda/include
    /usr/local/include/opencv
    /usr/local/include
  )
  add_subdirectory(${PROJECT_SOURCE_DIR}/main)
  add_subdirectory(${PROJECT_SOURCE_DIR}/libtld)
  add_subdirectory(${PROJECT_SOURCE_DIR}/imacq)
  add_subdirectory(${PROJECT_SOURCE_DIR}/heterogenous_tld_lib)
  add_subdirectory(${PROJECT_SOURCE_DIR}/3rdParties/lib)
  add_executable(run_main OpenTLD.cpp PPatchGenerator.cpp)
  #CUDA_ADD_EXECUTABLE(run_main OpenTLD.cpp) # ${hdr} ${cu})
  target_link_libraries(run_main main imacq libtld hetld hetld_cu ${OpenCV_LIBS} ${PROJECT_SOURCE_DIR}/3rdParties/lib/libconfig++.so)
else(CUDA_FOUND)
  message("CUDA is not installed on this system.")
endif()
