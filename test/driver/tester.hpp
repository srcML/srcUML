/**
 * @file tester.hpp
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

#ifndef INCLUDED_TESTER_HPP
#define INCLUDED_TESTER_HPP

#include <string>
#include <vector>
#include <tuple>

class tester_t {

private:

    static const size_t COLUMN_SIZE;

    const std::string & name;
    size_t test_count;
    size_t number_passed;

    std::vector<std::tuple<size_t, bool, std::string>> test_results;


    // intermediate test variables
    std::string source_code;
    std::string srcml;
    std::string yuml;

public:

    tester_t(const std::string & name);

    tester_t & src2srcml(const std::string & src);
    tester_t & run();
    tester_t & test(const std::string & expected_yuml);

    size_t results() const;

};


#endif
