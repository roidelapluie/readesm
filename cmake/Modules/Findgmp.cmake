# - Try to find gmp
# Once done this will define
#
#  GMP_FOUND - system has gmp
#  GMP_INCLUDE_DIRS - the gmp include directory
#  GMP_LIBRARIES - Link these to use gmp
#  GMP_DEFINITIONS - Compiler switches required for using gmp
#
#  Copyright (c) 2011 Andreas Goelzer <andreas@goelzer.de>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (GMP_LIBRARIES AND GMP_INCLUDE_DIRS)
  # in cache already
  set(GMP_FOUND TRUE)
else (GMP_LIBRARIES AND GMP_INCLUDE_DIRS)

  find_path(GMP_INCLUDE_DIR
    NAMES
      gmp.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
  )

  find_library(GMP_LIBRARY
    NAMES
      gmp
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  set(GMP_INCLUDE_DIRS
    ${GMP_INCLUDE_DIR}
  )

  if (GMP_LIBRARY)
    set(GMP_LIBRARIES
        ${GMP_LIBRARIES}
        ${GMP_LIBRARY}
    )
  endif (GMP_LIBRARY)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(gmp DEFAULT_MSG GMP_LIBRARIES GMP_INCLUDE_DIRS)

  # show the GMP_INCLUDE_DIRS and GMP_LIBRARIES variables only in the advanced view
  mark_as_advanced(GMP_INCLUDE_DIRS GMP_LIBRARIES)

endif (GMP_LIBRARIES AND GMP_INCLUDE_DIRS)

