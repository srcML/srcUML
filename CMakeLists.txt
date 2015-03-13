##
#  CMakeLists.txt
#
#  Copyright (C) 2014 SDML (www.sdml.info)
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

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
include(srcyuml)

include_directories(${LIBXML2_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/srcSAX/src/srcsax ${CMAKE_SOURCE_DIR}/srcSAX/src/cpp src/headers src/cpp)

add_subdirectory(src)
