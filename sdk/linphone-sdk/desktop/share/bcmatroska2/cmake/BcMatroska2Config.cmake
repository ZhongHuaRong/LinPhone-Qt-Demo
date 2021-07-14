############################################################################
# BcMatroska2Config.cmake
# Copyright (C) 2015  Belledonne Communications, Grenoble France
#
############################################################################
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
############################################################################
#
# Config file for the Matroska2 package.
# It defines the following variables:
#
#  BCMATROSKA2_FOUND - system has BcMatroska2
#  BCMATROSKA2_INCLUDE_DIRS - the BcMatroska2 include directory
#  BCMATROSKA2_LIBRARIES - The libraries needed to use BcMatroska2


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was BcMatroska2Config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include("${CMAKE_CURRENT_LIST_DIR}/BcMatroska2Targets.cmake")

set(BCMATROSKA2_TARGETNAME "bcmatroska2")

if(NO)
	set(BCMATROSKA2_LIBRARIES "${BCMATROSKA2_TARGETNAME}")
else()
	get_target_property(BCMATROSKA2_LIBRARIES ${BCMATROSKA2_TARGETNAME} LOCATION)
endif()

set(BCMATROSKA2_INCLUDE_DIRS "C:/GitLab-Runner/builds/651dcee5/2/BC/public/linphone-sdk/build-desktop/linphone-sdk/desktop/include")

set(BCMATROSKA2_FOUND 1)
