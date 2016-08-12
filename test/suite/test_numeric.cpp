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

    tester_t tester("numeric");

    // simple numeric
    tester.src2srcml("class foo { public: int bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: short bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: size_t bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: double bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: float bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    // signed numeric
    tester.src2srcml("class foo { public: unsigned int bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: signed int bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: unsigned short bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: signed short bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    // long long variations with/without signed
    tester.src2srcml("class foo { public: long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: long long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: unsigned long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: unsigned long long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: signed long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: signed long long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");


    // const
    tester.src2srcml("class foo { public: const int bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: const size_t bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: const short bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: const long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: const long long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");


    tester.src2srcml("class foo { public: const unsigned int bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: const signed int bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: const unsigned short bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: const signed short bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: const long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: const long long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: const unsigned long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: const unsigned long long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: const signed long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: const signed long long long bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");


    tester.src2srcml("class foo { public: int const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: size_t const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: short const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: long long const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: long long long const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");


    tester.src2srcml("class foo { public: unsigned int const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: signed int const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: unsigned short const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: signed short const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: long long const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: long long long const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: unsigned long long const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: unsigned long long long const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: signed long long const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: signed long long long const bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    // pointer
    tester.src2srcml("class foo { public: int * bar; };").run().test("[«datatype»;foo|+ bar: number［*］ ｛ordered｝;]\n");
    tester.src2srcml("class foo { public: size_t * bar; };").run().test("[«datatype»;foo|+ bar: number［*］ ｛ordered｝;]\n");

    tester.src2srcml("class foo { public: short * bar; };").run().test("[«datatype»;foo|+ bar: number［*］ ｛ordered｝;]\n");

    tester.src2srcml("class foo { public: long long * bar; };").run().test("[«datatype»;foo|+ bar: number［*］ ｛ordered｝;]\n");
    tester.src2srcml("class foo { public: long long long * bar; };").run().test("[«datatype»;foo|+ bar: number［*］ ｛ordered｝;]\n");

    // reference
    tester.src2srcml("class foo { public: int & bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: size_t & bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: short & bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    tester.src2srcml("class foo { public: long long & bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");
    tester.src2srcml("class foo { public: long long long & bar; };").run().test("[«datatype»;foo|+ bar: number;]\n");

    // template
    tester.src2srcml("class foo { public: std::shared_ptr<int> bar; };").run().test("[«datatype»;foo|+ bar: number［0..1］;]\n");
    tester.src2srcml("class foo { public: std::shared_ptr<size_t> bar; };").run().test("[«datatype»;foo|+ bar: number［0..1］;]\n");

    tester.src2srcml("class foo { public: std::shared_ptr<short> bar; };").run().test("[«datatype»;foo|+ bar: number［0..1］;]\n");

    tester.src2srcml("class foo { public: std::shared_ptr<long long> bar; };").run().test("[«datatype»;foo|+ bar: number［0..1］;]\n");
    tester.src2srcml("class foo { public: std::shared_ptr<long long long> bar; };").run().test("[«datatype»;foo|+ bar: number［0..1］;]\n");

    // complex
    tester.src2srcml("class foo { public: std::shared_ptr<const unsigned int const * const> bar; };").run().test("[«datatype»;foo|+ bar: number［*］ ｛ordered｝;]\n");

    return tester.results();

}
