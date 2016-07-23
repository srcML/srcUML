/**
 * @file test_attribute.cpp
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

    tester_t tester("empty");

    for(const std::string & class_type : { "class", "struct" }) {

        if(class_type == "class")
            tester.src2srcml(class_type + " object { foo bar; };").run().test("[«datatype»;object|- bar: foo;]\n");
        else if(class_type == "struct")
            tester.src2srcml(class_type + " object { foo bar; };").run().test("[«datatype»;object|+ bar: foo;]\n");

        // pointers
        tester.src2srcml(class_type + " object { public: foo * bar; };").run().test("[«datatype»;object|+ bar: foo［*］;]\n");
        tester.src2srcml(class_type + " object { public: foo bar[]; };").run().test("[«datatype»;object|+ bar: foo［*］;]\n");
        tester.src2srcml(class_type + " object { public: foo * bar[]; };").run().test("[«datatype»;object|+ bar: foo［*］;]\n");
        tester.src2srcml(class_type + " object { public: foo bar[10]; };").run().test("[«datatype»;object|+ bar: foo［10］;]\n");
        tester.src2srcml(class_type + " object { public: foo bar[SIZE]; };").run().test("[«datatype»;object|+ bar: foo［?］;]\n");
        tester.src2srcml(class_type + " object { public: foo * bar[10]; };").run().test("[«datatype»;object|+ bar: foo［0..10］;]\n");
        tester.src2srcml(class_type + " object { public: foo * bar[SIZE]; };").run().test("[«datatype»;object|+ bar: foo［0..?］;]\n");
        tester.src2srcml(class_type + " object { public: foo ** bar; };").run().test("[«datatype»;object|+ bar: foo［*］;]\n");

        // reference
        tester.src2srcml(class_type + " object { public: foo & bar; };").run().test("[«datatype»;object|+ bar: foo;]\n");

        // const
        tester.src2srcml(class_type + " object { public: const foo * bar; };").run().test("[«datatype»;object|+ bar: foo［*］;]\n");
        tester.src2srcml(class_type + " object { public: const foo & bar; };").run().test("[«datatype»;object|+ bar: foo;]\n");

        tester.src2srcml(class_type + " object { public: foo const & bar; };").run().test("[«datatype»;object|+ bar: foo;]\n");
        tester.src2srcml(class_type + " object { public: foo const * bar; };").run().test("[«datatype»;object|+ bar: foo［*］;]\n");

        tester.src2srcml(class_type + " object { public: foo * const bar; };").run().test("[«datatype»;object|+ bar: foo［*］;]\n");
        tester.src2srcml(class_type + " object { public: foo & const bar; };").run().test("[«datatype»;object|+ bar: foo;]\n");

        // array
        tester.src2srcml(class_type + " object { public: array<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::array<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // list
        tester.src2srcml(class_type + " object { public: list<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::list<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // forward_list
        tester.src2srcml(class_type + " object { public: forward_list<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::forward_list<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // vector
        tester.src2srcml(class_type + " object { public: vector<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::vector<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // set
        tester.src2srcml(class_type + " object { public: set<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::set<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // unordered_set
        tester.src2srcml(class_type + " object { public: unordered_set<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::unordered_set<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // deque
        tester.src2srcml(class_type + " object { public: deque<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::deque<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // stack
        tester.src2srcml(class_type + " object { public: stack<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::stack<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // queue
        tester.src2srcml(class_type + " object { public: queue<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::queue<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // priority_queue
        tester.src2srcml(class_type + " object { public: priority_queue<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");
        tester.src2srcml(class_type + " object { public: std::priority_queue<foo> bar; };").run().test("[«datatype»;object|+ bar: foo［*］;\n");

        // auto_ptr
        tester.src2srcml(class_type + " object { public: auto_ptr<foo> bar; };").run().test("[«datatype»;object|+ bar: foo\n");
        tester.src2srcml(class_type + " object { public: std::auto_ptr<foo> bar; };").run().test("[«datatype»;object|+ bar: foo;\n");

        /** @todo tests for map, multi*?, ordering on items */

    }

    return tester.results();

}
