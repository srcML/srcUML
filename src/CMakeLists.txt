##
# CMakeLists.txt
#
# Copyright (C) 2015-2016 srcML, LLC. (www.srcML.org)
#
# This file is part of srcYUML.
#
# srcYUML is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# srcYUML is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with srcYUML.  If not, see <http://www.gnu.org/licenses/>.

file(GLOB SRCYUML_SOURCE *.cpp)
file(GLOB SRCYUML_HEADER *.hpp)

add_executable(srcyuml ${SRCYUML_SOURCE} ${SRCYUML_HEADER})
target_link_libraries(srcyuml srcsaxeventdispatch srcsax_static ${LIBXML2_LIBRARIES})
