#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bellesip" for configuration "RelWithDebInfo"
set_property(TARGET bellesip APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(bellesip PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/bellesip.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/bellesip.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS bellesip )
list(APPEND _IMPORT_CHECK_FILES_FOR_bellesip "${_IMPORT_PREFIX}/lib/bellesip.lib" "${_IMPORT_PREFIX}/bin/bellesip.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
