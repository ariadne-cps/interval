# Try to find the standalone Ariadne Interval library.
#
# Once done this will define:
#
#  AriadneInterval_FOUND - system has Ariadne Interval
#  ARIADNE_INTERVAL_INCLUDE_DIRS - the Ariadne Interval include directories
#  ARIADNE_INTERVAL_LIBRARIES - libraries required to use Ariadne Interval
#
# and, when found:
#
#  AriadneInterval::ariadne-interval - imported library target

# This file is part of Ariadne.

# Ariadne is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Ariadne is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.

find_library(ARIADNE_INTERVAL_LIBRARY NAMES ariadne-interval)

find_path(ARIADNE_INTERVAL_INCLUDE_DIR ariadne-interval.hpp PATH_SUFFIXES ariadne-interval)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AriadneInterval DEFAULT_MSG
  ARIADNE_INTERVAL_LIBRARY
  ARIADNE_INTERVAL_INCLUDE_DIR
)

set(ARIADNE_INTERVAL_FOUND ${AriadneInterval_FOUND})

if(AriadneInterval_FOUND)
  get_filename_component(ARIADNE_INTERVAL_INCLUDE_PARENT_DIR ${ARIADNE_INTERVAL_INCLUDE_DIR} DIRECTORY)
  set(ARIADNE_INTERVAL_INCLUDE_DIRS
    ${ARIADNE_INTERVAL_INCLUDE_PARENT_DIR}
    ${ARIADNE_INTERVAL_INCLUDE_DIR}
  )
  set(ARIADNE_INTERVAL_LIBRARIES
    ${ARIADNE_INTERVAL_LIBRARY}
  )

  if(NOT TARGET AriadneInterval::ariadne-interval)
    add_library(AriadneInterval::ariadne-interval UNKNOWN IMPORTED)
    set_target_properties(AriadneInterval::ariadne-interval PROPERTIES
      IMPORTED_LOCATION "${ARIADNE_INTERVAL_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${ARIADNE_INTERVAL_INCLUDE_DIRS}"
    )
  endif()
endif()

mark_as_advanced(
  ARIADNE_INTERVAL_INCLUDE_DIR
  ARIADNE_INTERVAL_LIBRARY
)
