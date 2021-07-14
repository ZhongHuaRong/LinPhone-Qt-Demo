#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bctoolbox" for configuration "RelWithDebInfo"
set_property(TARGET bctoolbox APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(bctoolbox PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/bctoolbox.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/bctoolbox.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS bctoolbox )
list(APPEND _IMPORT_CHECK_FILES_FOR_bctoolbox "${_IMPORT_PREFIX}/lib/bctoolbox.lib" "${_IMPORT_PREFIX}/bin/bctoolbox.dll" )

# Import target "bctoolbox-tester" for configuration "RelWithDebInfo"
set_property(TARGET bctoolbox-tester APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(bctoolbox-tester PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/bctoolbox-tester.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELWITHDEBINFO "bcunit"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/bctoolbox-tester.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS bctoolbox-tester )
list(APPEND _IMPORT_CHECK_FILES_FOR_bctoolbox-tester "${_IMPORT_PREFIX}/lib/bctoolbox-tester.lib" "${_IMPORT_PREFIX}/bin/bctoolbox-tester.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
