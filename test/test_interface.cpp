/**
 * @file test_interface.cpp
 *
 * @copyright Copyright (C) 2016 srcML, LLC. (www.srcML.org)
 *
 * This file is part of srcYUML.
 *
 * srcYUML is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * srcYUML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with srcYUML.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <tester.hpp>

#include <string>
#include <vector>

int main(int argc, char * argv[]) {

    tester_t tester("interface");

    std::vector<std::string> class_types{ "class", "struct" };

    for(const std::string & class_type : class_types) {

        tester.src2srcml(class_type + " foo { public: void bar() = 0; };").run().test("[«interface»;foo||+ bar(  );]\n");

        tester.src2srcml(class_type + " foo { public: void bar(); };").run().test("[«datatype»;foo||+ bar(  );]\n");
        tester.src2srcml(class_type + " foo { public: void bar() {} };").run().test("[«datatype»;foo||+ bar(  );]\n");

        tester.src2srcml(class_type + " foo { private: void bar() = 0; };").run().test("[«datatype»;foo||- bar(  );]\n");
        tester.src2srcml(class_type + " foo { protected: void bar() = 0; };").run().test("[«datatype»;foo||# bar(  );]\n");

    }

    return tester.results();

}
