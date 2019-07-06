##
# CMakeLists.txt
#
# Copyright (C) 2015-2016 srcML, LLC. (www.srcML.org)
#
# This file is part of srcUML.
#
# srcUML is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# srcUML is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with srcUML.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
project(srcUML)

enable_testing()

#find needed libraries
find_package(LibXml2 REQUIRED)

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)

# include needed includes
include_directories(${LIBXML2_INCLUDE_DIR})
#include_directories(${OGDF})
add_definitions("-std=c++1y")
# add_definitions("-pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(srcSAXEventDispatch/CMake)
#add_subdirectory(ogdf)

include_directories(${DISPATCH_INCLUDE_DIR} src/generator test/driver ogdf/include ${CMAKE_BINARY_DIR}/ogdf/include ${Boost_INCLUDE_DIR})

add_subdirectory(src)
#add_subdirectory(test)

