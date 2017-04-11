/**
 * @file test_numeric.cpp
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

int main(int argc, char * argv[]) {

    tester_t tester("dependencies");

    for(const std::string& class_type : {"class", "struct"}){

		tester.src2srcml(class_type + " foo{private: void f(bar a){}; };" + class_type + " bar{};").run().test("[«datatype»;foo||- f(a: bar);]\n[«datatype»;bar]\n[«datatype»;foo]-.->[«datatype»;bar]\n");
		tester.src2srcml(class_type + " foo{private: void f(){bar a;}; };" + class_type + " bar{};").run().test("[«datatype»;foo||- f();]\n[«datatype»;bar]\n[«datatype»;foo]-.->[«datatype»;bar]\n");
		tester.src2srcml(class_type + " foo{private: void f(bar a){bar b;}; };" + class_type + " bar{};").run().test("[«datatype»;foo||- f(a: bar);]\n[«datatype»;bar]\n[«datatype»;foo]-.->[«datatype»;bar]\n");
		tester.src2srcml(class_type + " foo{private: void f(bar a, pan b){};};" + class_type + " bar{};" + class_type + " pan{};").run().test("[«datatype»;foo||- f(a: bar, b: pan);]\n[«datatype»;bar]\n[«datatype»;pan]\n[«datatype»;foo]-.->[«datatype»;bar]\n[«datatype»;foo]-.->[«datatype»;pan]\n");
		tester.src2srcml(class_type + " foo{private: void f(bar a){pan b;};};" + class_type + " bar{};" + class_type + " pan{};").run().test("[«datatype»;foo||- f(a: bar);]\n[«datatype»;bar]\n[«datatype»;pan]\n[«datatype»;foo]-.->[«datatype»;bar]\n[«datatype»;foo]-.->[«datatype»;pan]\n");
		tester.src2srcml(class_type + " foo{private: void f(){bar a; pan b;};};" + class_type + " bar{};" + class_type + " pan{};").run().test("[«datatype»;foo||- f();]\n[«datatype»;bar]\n[«datatype»;pan]\n[«datatype»;foo]-.->[«datatype»;bar]\n[«datatype»;foo]-.->[«datatype»;pan]\n");
		tester.src2srcml(class_type + " foo{private: void f(bar a){}; void g(pan a){}; };" + class_type + " bar{};" + class_type + " pan{};").run().test("[«datatype»;foo||- f(a: bar);- g(a: pan);]\n[«datatype»;bar]\n[«datatype»;pan]\n[«datatype»;foo]-.->[«datatype»;bar]\n[«datatype»;foo]-.->[«datatype»;pan]\n");

	}

    return tester.results();

}