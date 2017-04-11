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

    tester_t tester("relationships");

	//tester.src2srcml("class Parent{}; class Child: public Parent{};").run().test("[][Parent]^-[Child]");
	//tester.src2srcml("class Parent{}; class Child: public Parent{}; class Grandchild: public Child{};lass Parent{}; class Child: public Parent{}; class Grandchild: public Child{};").run().test("")

    return tester.results();

}