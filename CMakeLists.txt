##
#  CMakeLists.txt
#
#  Copyright (C) 2015 SDML (www.sdml.info)
#
#  This file is part of the srcYUML.
#
#  The srcYUML is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  The srcYUML is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the srcYUML; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

cmake_minimum_required(VERSION 2.8)
project(srcYUML)

enable_testing()

# find needed libraries
find_package(LibXml2 REQUIRED)

# include needed includes
include_directories(${LIBXML2_INCLUDE_DIR})
add_definitions("-std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(src ${CMAKE_SOURCE_DIR}/srcSAX/src/srcsax ${CMAKE_SOURCE_DIR}/srcSAX/src/cpp ${CMAKE_SOURCE_DIR}/srcSAX/src/windows)

add_subdirectory(srcSAX/src)
add_subdirectory(src)
