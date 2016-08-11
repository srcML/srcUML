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

    for(const std::string & class_type : { "class", "struct" }) {

        // simple tests
        tester.src2srcml(class_type + " foo { public: void bar() = 0; };").run().test("[«interface»;foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo { public: virtual void bar() = 0; };").run().test("[«interface»;foo||+ bar();]\n");

        tester.src2srcml(class_type + " foo { public: virtual void bar(); };").run().test("[«datatype»;foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo { public: void bar(); };").run().test("[«datatype»;foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo { public: void bar() {} };").run().test("[«datatype»;foo||+ bar();]\n");

        if(class_type == "class")
            tester.src2srcml(class_type + " foo { virtual void bar() = 0; };").run().test("[{abstract};foo||- bar();]\n");
        else if(class_type == "struct")
            tester.src2srcml(class_type + " foo { virtual void bar() = 0; };").run().test("[«interface»;foo||+ bar();]\n");

        // non-public tests
        tester.src2srcml(class_type + " foo { private: virtual void bar() = 0; };").run().test("[{abstract};foo||- bar();]\n");
        tester.src2srcml(class_type + " foo { protected: virtual void bar() = 0; };").run().test("[{abstract};foo||# bar();]\n");

        // multiple tests one public one not
        tester.src2srcml(class_type + " foo { public: virtual void bar() = 0; void foobar() = 0; };").run().test("[«interface»;foo||+ bar();+ foobar();]\n");
        tester.src2srcml(class_type + " foo { public: virtual void bar() = 0; public: void foobar() = 0; };").run().test("[«interface»;foo||+ bar();+ foobar();]\n");
        tester.src2srcml(class_type + " foo { public: virtual void bar() = 0; private: void foobar() = 0; };").run().test("[{abstract};foo||+ bar();- foobar();]\n");
        tester.src2srcml(class_type + " foo { public: virtual void bar() = 0; protected: void foobar() = 0; };").run().test("[{abstract};foo||+ bar();# foobar();]\n");

        // tests with one pure virtual one not
        tester.src2srcml(class_type + " foo { public: virtual void bar() = 0; void foobar(); };").run().test("[{abstract};foo||+ bar();+ foobar();]\n");
        tester.src2srcml(class_type + " foo { public: void bar(); void foobar() = 0; };").run().test("[{abstract};foo||+ bar();+ foobar();]\n");

        /// constructor/destructor/assignment tests
        tester.src2srcml(class_type + " foo { public: foo(); virtual void bar() = 0; };").run().test("[{abstract};foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo { public: foo(foo & f); virtual void bar() = 0; };").run().test("[{abstract};foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo { public: foo(int); virtual void bar() = 0; };").run().test("[{abstract};foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo { public: ~foo(); virtual void bar() = 0; };").run().test("[{abstract};foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo { public: virtual void bar() = 0; foo & operator=(foo f); };").run().test("[{abstract};foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo { public: virtual void bar() = 0; foo & operator=(foo f) = 0; };").run().test("[«interface»;foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo { public: virtual void bar() = 0; private: foo & operator=(foo f) = 0; };").run().test("[{abstract};foo||+ bar();]\n");

        // attribute tests
        tester.src2srcml(class_type + " foo { public: int f; virtual void bar() = 0; };").run().test("[{abstract};foo|+ f: number;|+ bar();]\n");

        if(class_type == "class")
            tester.src2srcml(class_type + " foo { int f;  public: virtual void bar() = 0; };").run().test("[{abstract};foo|- f: number;|+ bar();]\n");
        else if(class_type == "struct")
            tester.src2srcml(class_type + " foo { int f;  public: virtual void bar() = 0; };").run().test("[{abstract};foo|+ f: number;|+ bar();]\n");

        tester.src2srcml(class_type + " foo { public: int f;  public: virtual void bar() = 0; };").run().test("[{abstract};foo|+ f: number;|+ bar();]\n");
        tester.src2srcml(class_type + " foo { private: int f;  public: virtual void bar() = 0; };").run().test("[{abstract};foo|- f: number;|+ bar();]\n");
        tester.src2srcml(class_type + " foo { protected: int f;  public: virtual void bar() = 0; };").run().test("[{abstract};foo|# f: number;|+ bar();]\n");

        // inheritence tests
        tester.src2srcml(class_type + " foo { public: void bar() = 0; };").run().test("[«interface»;foo||+ bar();]\n");

        // not found inheritence
        tester.src2srcml(class_type + " foo : public object { public: void bar() = 0; };").run().test("[«interface»;foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo : virtual public object { public: void bar() = 0; };").run().test("[«interface»;foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo : private object { public: void bar() = 0; };").run().test("[«interface»;foo||+ bar();]\n");
        tester.src2srcml(class_type + " foo : protected object { public: void bar() = 0; };").run().test("[«interface»;foo||+ bar();]\n");

        tester.src2srcml(class_type + " foo : public object_one, object_two  { public: void bar() = 0; };").run().test("[«interface»;foo||+ bar();]\n");

        // found interface inheritence
        tester.src2srcml(class_type + " object { public: void hash() = 0; };\n" + class_type + " foo : public object { public: void bar() = 0; };").run().test("[«interface»;object||+ hash();]\n[«interface»;foo||+ bar();]\n[«interface»;object]^-[«interface»;foo]\n");
        tester.src2srcml(class_type + " object { public: void hash() = 0; };\n" + class_type + " foo : virtual public object { public: void bar() = 0; };").run().test("[«interface»;object||+ hash();]\n[«interface»;foo||+ bar();]\n[«interface»;object]^-[«interface»;foo]\n");
        tester.src2srcml(class_type + " object { public: void hash() = 0; };\n" + class_type + " foo : private object { public: void bar() = 0; };").run().test("[«interface»;object||+ hash();]\n[«interface»;foo||+ bar();]\n[«interface»;object]^-[«interface»;foo]\n");
        tester.src2srcml(class_type + " object { public: void hash() = 0; };\n" + class_type + " foo : protected object { public: void bar() = 0; };").run().test("[«interface»;object||+ hash();]\n[«interface»;foo||+ bar();]\n[«interface»;object]^-[«interface»;foo]\n");

        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two { public: void clone() = 0; };\n" + class_type + " foo : public object_one, object_two  { public: void bar() = 0; };").run().test("[«interface»;object_one||+ hash();]\n[«interface»;object_two||+ clone();]\n[«interface»;foo||+ bar();]\n[«interface»;object_one]^-[«interface»;foo]\n[«interface»;object_two]^-[«interface»;foo]\n");

        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two : object_one { public: void clone() = 0; };\n" + class_type + " foo : public object_two  { public: void bar() = 0; };").run().test("[«interface»;object_one||+ hash();]\n[«interface»;object_two||+ clone();]\n[«interface»;foo||+ bar();]\n[«interface»;object_one]^-[«interface»;object_two]\n[«interface»;object_two]^-[«interface»;foo]\n");
        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two : object_one { public: void clone() = 0; };\n" + class_type + " object_three : object_one { public: void to_string() = 0; };\n" + class_type + " foo : public object_two, object_three { public: void bar() = 0; };").run().test("[«interface»;object_one||+ hash();]\n[«interface»;object_two||+ clone();]\n[«interface»;object_three||+ to_string();]\n[«interface»;foo||+ bar();]\n[«interface»;object_one]^-[«interface»;object_two]\n[«interface»;object_one]^-[«interface»;object_three]\n[«interface»;object_two]^-[«interface»;foo]\n[«interface»;object_three]^-[«interface»;foo]\n");

        // found concreate inheritence
        tester.src2srcml(class_type + " object { public: void hash(); };\n" + class_type + " foo : public object { public: void bar() = 0; };").run().test("[«datatype»;object||+ hash();]\n[{abstract};foo||+ bar();]\n[«datatype»;object]^-[{abstract};foo]\n");
        tester.src2srcml(class_type + " object { public: void hash(); };\n" + class_type + " foo : virtual public object { public: void bar() = 0; };").run().test("[«datatype»;object||+ hash();]\n[{abstract};foo||+ bar();]\n[«datatype»;object]^-[{abstract};foo]\n");
        tester.src2srcml(class_type + " object { public: void hash(); };\n" + class_type + " foo : private object { public: void bar() = 0; };").run().test("[«datatype»;object||+ hash();]\n[{abstract};foo||+ bar();]\n[«datatype»;object]^-[{abstract};foo]\n");
        tester.src2srcml(class_type + " object { public: void hash(); };\n" + class_type + " foo : protected object { public: void bar() = 0; };").run().test("[«datatype»;object||+ hash();]\n[{abstract};foo||+ bar();]\n[«datatype»;object]^-[{abstract};foo]\n");

        tester.src2srcml(class_type + " object_one { public: void hash(); };\n" + class_type + " object_two { public: void clone() = 0; };\n" + class_type + " foo : public object_one, object_two  { public: void bar() = 0; };").run().test("[«datatype»;object_one||+ hash();]\n[«interface»;object_two||+ clone();]\n[{abstract};foo||+ bar();]\n[«datatype»;object_one]^-[{abstract};foo]\n[«interface»;object_two]^-[{abstract};foo]\n");
        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two { public: void clone(); };\n" + class_type + " foo : public object_one, object_two  { public: void bar() = 0; };").run().test("[«interface»;object_one||+ hash();]\n[«datatype»;object_two||+ clone();]\n[{abstract};foo||+ bar();]\n[«interface»;object_one]^-[{abstract};foo]\n[«datatype»;object_two]^-[{abstract};foo]\n");
        tester.src2srcml(class_type + " object_one { public: void hash(); };\n" + class_type + " object_two { public: void clone(); };\n" + class_type + " foo : public object_one, object_two  { public: void bar() = 0; };").run().test("[«datatype»;object_one||+ hash();]\n[«datatype»;object_two||+ clone();]\n[{abstract};foo||+ bar();]\n[«datatype»;object_one]^-[{abstract};foo]\n[«datatype»;object_two]^-[{abstract};foo]\n");


        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two : object_one { public: void clone() = 0; };\n" + class_type + " foo : public object_two  { public: void bar(); };").run().test("[«interface»;object_one||+ hash();]\n[«interface»;object_two||+ clone();]\n[«datatype»;foo||+ bar();]\n[«interface»;object_one]^-[«interface»;object_two]\n[«interface»;object_two]^-[«datatype»;foo]\n");
        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two : object_one { public: void clone(); };\n" + class_type + " foo : public object_two  { public: void bar() = 0; };").run().test("[«interface»;object_one||+ hash();]\n[«datatype»;object_two||+ clone();]\n[{abstract};foo||+ bar();]\n[«interface»;object_one]^-[«datatype»;object_two]\n[«datatype»;object_two]^-[{abstract};foo]\n");
        tester.src2srcml(class_type + " object_one { public: void hash(); };\n" + class_type + " object_two : object_one { public: void clone() = 0; };\n" + class_type + " foo : public object_two  { public: void bar() = 0; };").run().test("[«datatype»;object_one||+ hash();]\n[{abstract};object_two||+ clone();]\n[{abstract};foo||+ bar();]\n[«datatype»;object_one]^-[{abstract};object_two]\n[{abstract};object_two]^-[{abstract};foo]\n");

        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two : object_one { public: void clone() = 0; };\n" + class_type + " object_three : object_one { public: void to_string() = 0; };\n" + class_type + " foo : public object_two, object_three { public: void bar() = 0; };").run().test("[«interface»;object_one||+ hash();]\n[«interface»;object_two||+ clone();]\n[«interface»;object_three||+ to_string();]\n[«interface»;foo||+ bar();]\n[«interface»;object_one]^-[«interface»;object_two]\n[«interface»;object_one]^-[«interface»;object_three]\n[«interface»;object_two]^-[«interface»;foo]\n[«interface»;object_three]^-[«interface»;foo]\n");
        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two : object_one { public: void clone() = 0; };\n" + class_type + " object_three : object_one { public: void to_string() = 0; };\n" + class_type + " foo : public object_two, object_three { public: void bar(); };").run().test("[«interface»;object_one||+ hash();]\n[«interface»;object_two||+ clone();]\n[«interface»;object_three||+ to_string();]\n[«datatype»;foo||+ bar();]\n[«interface»;object_one]^-[«interface»;object_two]\n[«interface»;object_one]^-[«interface»;object_three]\n[«interface»;object_two]^-[«datatype»;foo]\n[«interface»;object_three]^-[«datatype»;foo]\n");
        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two : object_one { public: void clone() = 0; };\n" + class_type + " object_three : object_one { public: void to_string(); };\n" + class_type + " foo : public object_two, object_three { public: void bar() = 0; };").run().test("[«interface»;object_one||+ hash();]\n[«interface»;object_two||+ clone();]\n[«datatype»;object_three||+ to_string();]\n[{abstract};foo||+ bar();]\n[«interface»;object_one]^-[«interface»;object_two]\n[«interface»;object_one]^-[«datatype»;object_three]\n[«interface»;object_two]^-[{abstract};foo]\n[«datatype»;object_three]^-[{abstract};foo]\n");
        tester.src2srcml(class_type + " object_one { public: void hash() = 0; };\n" + class_type + " object_two : object_one { public: void clone(); };\n" + class_type + " object_three : object_one { public: void to_string() = 0; };\n" + class_type + " foo : public object_two, object_three { public: void bar() = 0; };").run().test("[«interface»;object_one||+ hash();]\n[«datatype»;object_two||+ clone();]\n[«interface»;object_three||+ to_string();]\n[{abstract};foo||+ bar();]\n[«interface»;object_one]^-[«datatype»;object_two]\n[«interface»;object_one]^-[«interface»;object_three]\n[«datatype»;object_two]^-[{abstract};foo]\n[«interface»;object_three]^-[{abstract};foo]\n");

        tester.src2srcml(class_type + " object_one { public: void hash(); };\n" + class_type + " object_two : object_one { public: void clone() = 0; };\n" + class_type + " object_three : object_one { public: void to_string() = 0; };\n" + class_type + " foo : public object_two, object_three { public: void bar() = 0; };").run().test("[«datatype»;object_one||+ hash();]\n[{abstract};object_two||+ clone();]\n[{abstract};object_three||+ to_string();]\n[{abstract};foo||+ bar();]\n[«datatype»;object_one]^-[{abstract};object_two]\n[«datatype»;object_one]^-[{abstract};object_three]\n[{abstract};object_two]^-[{abstract};foo]\n[{abstract};object_three]^-[{abstract};foo]\n");

    }

    return tester.results();

}
