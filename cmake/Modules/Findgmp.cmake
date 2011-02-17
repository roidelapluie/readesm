include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(gmp_PKGCONF gmp)

# Include dir
find_path(gmp_INCLUDE_DIR
  NAMES gmp.h
  PATHS ${gmp_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(gmp_LIBRARY
  NAMES gmp
  PATHS ${gmp_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(gmp_PROCESS_INCLUDES gmp_INCLUDE_DIR gmp_INCLUDE_DIRS)
set(gmp_PROCESS_LIBS gmp_LIBRARY gmp_LIBRARIES)
libfind_process(gmp)