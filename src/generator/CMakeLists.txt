##
# CMakeLists.txt
#
# Copyright (C) 2016 srcML, LLC. (www.srcML.org)
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

file(GLOB GENERATOR_SOURCE *.cpp)
file(GLOB GENERATOR_HEADER *.hpp)

add_library(generator OBJECT ${GENERATOR_HEADERS} ${GENERATOR_SOURCE})
# link_directories(/usr/local/lib/x86_64-linux-gnu)
# target_link_libraries(generator OGDF COIN)
