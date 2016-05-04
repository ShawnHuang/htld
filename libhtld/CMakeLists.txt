#file( GLOB  files  *.cpp mftracker/*.cpp)
#add_library(libtld ${files})
add_subdirectory(libtld)
add_subdirectory(heterogenous_tld_lib)
