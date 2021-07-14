#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bcmatroska2" for configuration "RelWithDebInfo"
set_property(TARGET bcmatroska2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(bcmatroska2 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/bcmatroska2.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS bcmatroska2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_bcmatroska2 "${_IMPORT_PREFIX}/lib/bcmatroska2.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
