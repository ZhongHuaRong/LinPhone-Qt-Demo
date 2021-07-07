#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bzrtp" for configuration "Release"
set_property(TARGET bzrtp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(bzrtp PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/bzrtp.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/bzrtp.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS bzrtp )
list(APPEND _IMPORT_CHECK_FILES_FOR_bzrtp "${_IMPORT_PREFIX}/lib/bzrtp.lib" "${_IMPORT_PREFIX}/bin/bzrtp.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
