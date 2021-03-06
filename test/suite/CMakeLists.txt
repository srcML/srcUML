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

#
# add_srcuml_test
# Creates a srcuml test from a given file with a given name.
# - FILE_NAME the name of the unit test file.
# All arguments after the file name are considered to be linker arguments.
#
#
macro(add_srcuml_test TEST_FILE)

    get_filename_component(TEST_NAME_WITH_EXTENSION ${TEST_FILE} NAME)
    string(FIND ${TEST_NAME_WITH_EXTENSION} "." EXTENSION_BEGIN)
    string(SUBSTRING ${TEST_NAME_WITH_EXTENSION} 0 ${EXTENSION_BEGIN} TEST_NAME)

    add_executable(${TEST_NAME} ${TEST_FILE} $<TARGET_OBJECTS:generator> $<TARGET_OBJECTS:tester>)
    target_link_libraries(${TEST_NAME} srcsaxeventdispatch srcsax_static srcml ${LIBXML2_LIBRARIES} ${ARGN} OGDF COIN)
    add_test(NAME ${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)
    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

endmacro()

add_srcuml_test(test_empty.cpp)
add_srcuml_test(test_datatype.cpp)
add_srcuml_test(test_interface.cpp)
add_srcuml_test(test_numeric.cpp)
add_srcuml_test(test_attribute.cpp)
add_srcuml_test(test_relationships.cpp)
add_srcuml_test(test_dependencies.cpp)
