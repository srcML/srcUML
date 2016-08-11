/**
 * @file test_datatype.cpp
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

    tester_t tester("datatype");

    for(const std::string & class_type : { "class", "struct" }) {

        // simple tests
        tester.src2srcml(class_type + " foo {};").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[«datatype»;foo]\n");

        // access region tests
        if(class_type == "class")
            tester.src2srcml(class_type + " foo { foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[foo]\n");
        else if(class_type == "struct")
            tester.src2srcml(class_type + " foo { foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[«datatype»;foo]\n");

        tester.src2srcml(class_type + " foo { private: foo(); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { protected: foo(); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { private: foo(const foo & bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { protected: foo(const foo & bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { private: foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { protected: foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { private: foo & operator =(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { protected: foo & operator =(foo bar); };").run().test("[foo]\n");

        tester.src2srcml(class_type + " foo { private: foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { protected: foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[foo]\n");

        tester.src2srcml(class_type + " foo { public: foo(); public: foo(const foo & bar); public: foo & operator=(foo bar); };").run().test("[«datatype»;foo]\n");

        tester.src2srcml(class_type + " foo { private: foo(); public: foo(const foo & bar); public: foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { protected: foo(); public: foo(const foo & bar); public: foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); private: foo(const foo & bar); public: foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); protected: foo(const foo & bar); public: foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); public: foo(const foo & bar); private: foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); public: foo(const foo & bar); protected: foo & operator=(foo bar); };").run().test("[foo]\n");

        // parameter tests
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo &); foo & operator=(foo bar); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo &); foo & operator=(foo); };").run().test("[«datatype»;foo]\n");

        // missing at least one of the constructor or assignment
        tester.src2srcml(class_type + " foo { public: foo(const foo & bar); foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { public: foo & operator=(foo bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(const foo & bar); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); };").run().test("[foo]\n");

        // equality instead of assignment
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); bool operator==(const foo & bar); };").run().test("[foo]\n");

        // extra functions/constructor/destructor tests
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); bool operator==(const foo & bar); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); public: bool operator==(const foo & bar); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); private: bool operator==(const foo & bar); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); protected: bool operator==(const foo & bar); };").run().test("[«datatype»;foo]\n");

        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); void foobar(); };").run().test("[«datatype»;foo||+ foobar();]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); public: void foobar(); };").run().test("[«datatype»;foo||+ foobar();]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); private: void foobar(); };").run().test("[«datatype»;foo||- foobar();]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); protected: void foobar(); };").run().test("[«datatype»;foo||# foobar();]\n");

        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); foo(int); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); public: foo(int); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); private: foo(int); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); protected: foo(int); };").run().test("[«datatype»;foo]\n");

        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); ~foo(); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); public: ~foo(); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); private: ~foo(); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { public: foo(); foo(const foo & bar); foo & operator=(foo bar); protected: ~foo(); };").run().test("[«datatype»;foo]\n");

        tester.src2srcml(class_type + " foo { public: bool operator==(const foo & bar); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { private: bool operator==(const foo & bar); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { protected: bool operator==(const foo & bar); };").run().test("[«datatype»;foo]\n");

        tester.src2srcml(class_type + " foo { public: void foobar(); };").run().test("[«datatype»;foo||+ foobar();]\n");
        tester.src2srcml(class_type + " foo { private: void foobar(); };").run().test("[«datatype»;foo||- foobar();]\n");
        tester.src2srcml(class_type + " foo { protected: void foobar(); };").run().test("[«datatype»;foo||# foobar();]\n");

        // only other constructor
        if(class_type == "class")
            tester.src2srcml(class_type + " foo { foo(int); };").run().test("[foo]\n");
        else if(class_type == "struct")
            tester.src2srcml(class_type + " foo { foo(int); };").run().test("[foo]\n");

        tester.src2srcml(class_type + " foo { public: foo(int); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { private: foo(int); };").run().test("[foo]\n");
        tester.src2srcml(class_type + " foo { protected: foo(int); };").run().test("[foo]\n");

        // only destructor
        tester.src2srcml(class_type + " foo { public: ~foo(); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { private: ~foo(); };").run().test("[«datatype»;foo]\n");
        tester.src2srcml(class_type + " foo { protected: ~foo(); };").run().test("[«datatype»;foo]\n");

        // attribute tests
        tester.src2srcml(class_type + " foo { public: int f; };").run().test("[«datatype»;foo|+ f: number;]\n");

        if(class_type == "class")
            tester.src2srcml(class_type + " foo { int f; };").run().test("[«datatype»;foo|- f: number;]\n");
        else if(class_type == "struct")
            tester.src2srcml(class_type + " foo { int f; };").run().test("[«datatype»;foo|+ f: number;]\n");

        tester.src2srcml(class_type + " foo { public: int f; };").run().test("[«datatype»;foo|+ f: number;]\n");
        tester.src2srcml(class_type + " foo { private: int f; };").run().test("[«datatype»;foo|- f: number;]\n");
        tester.src2srcml(class_type + " foo { protected: int f; };").run().test("[«datatype»;foo|# f: number;]\n");

        tester.src2srcml(class_type + " foo { public: int f; foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[«datatype»;foo|+ f: number;]\n");

        if(class_type == "class")
            tester.src2srcml(class_type + " foo { int f; public: foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[«datatype»;foo|- f: number;]\n");
        else if(class_type == "struct")
            tester.src2srcml(class_type + " foo { int f; foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[«datatype»;foo|+ f: number;]\n");

        tester.src2srcml(class_type + " foo { public: int f; public: foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[«datatype»;foo|+ f: number;]\n");
        tester.src2srcml(class_type + " foo { private: int f; public: foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[«datatype»;foo|- f: number;]\n");
        tester.src2srcml(class_type + " foo { protected: int f; public: foo(); foo(const foo & bar); foo & operator=(foo bar); };").run().test("[«datatype»;foo|# f: number;]\n");

    }

    return tester.results();

}
