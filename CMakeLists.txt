cmake_minimum_required(VERSION 2.6)
project ( readesm )
include (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
CHECK_INCLUDE_FILE( "unistd.h" HAVE_UNISTD_H)
ADD_EXECUTABLE( readesm readesm.cpp )
TARGET_LINK_LIBRARIES( readesm boost_program_options gmp gcrypt )

