include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(gcrypt_PKGCONF gcrypt)

# Include dir
find_path(gcrypt_INCLUDE_DIR
  NAMES gcrypt.h
  PATHS ${gcrypt_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(gcrypt_LIBRARY
  NAMES gcrypt
  PATHS ${gcrypt_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(gcrypt_PROCESS_INCLUDES gcrypt_INCLUDE_DIR gcrypt_INCLUDE_DIRS)
set(gcrypt_PROCESS_LIBS gcrypt_LIBRARY gcrypt_LIBRARIES)
libfind_process(gcrypt)