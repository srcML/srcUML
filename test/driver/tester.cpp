/**
 * @file tester.cpp
 *
 * @copyright Copyright (C) 2016 srcML, LLC. (www.srcML.org)
 *
 * This file is part of srcUML.
 *
 * srcUML is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * srcUML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with srcUML.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <tester.hpp>

#include <srcml.h>
#include <srcuml_handler.hpp>

#include <sstream>
#include <iostream>
#include <iomanip>

const size_t tester_t::COLUMN_SIZE = 80;

tester_t::tester_t(const std::string & name) : name(name), test_count(0), number_passed(0), source_code(), srcml(), yuml() {}


tester_t & tester_t::src2srcml(const std::string & src) {

    source_code = src;
    srcml = std::string();

    srcml_archive * archive = srcml_archive_create();

    char * srcml_buffer = nullptr;
    size_t size = 0;
    srcml_archive_write_open_memory(archive, &srcml_buffer, &size);

    srcml_unit * unit = srcml_unit_create(archive);
    srcml_unit_set_language(unit, "C++");
    srcml_unit_parse_memory(unit, source_code.c_str(), source_code.size());

    srcml_archive_write_unit(archive, unit);
    srcml_unit_free(unit);

    srcml_archive_close(archive);
    srcml_archive_free(archive);

    srcml.append(srcml_buffer, size);

    return *this;

}

tester_t & tester_t::run() {

    std::ostringstream output;

    try {

        srcuml_handler handler(srcml, output, true, true, "yuml");

    } catch(...) {}

    yuml = output.str();

    return *this;

}

tester_t & tester_t::test(const std::string & expected_yuml) {

    ++test_count;

    if(yuml == expected_yuml) {

        ++number_passed;
        test_results.push_back(std::make_tuple(test_count, true, ""));

    } else {

        std::string error = "### expected ###\n";
        error += expected_yuml;
        error += "### actual ###\n";
        error += yuml;
        error += "### end ###\n\n";
        test_results.push_back(std::make_tuple(test_count, false, error));

    }

    yuml = std::string();

    return *this;
}

static size_t number_characters(size_t number) {

    size_t count = 1;
    size_t current_number = number;
    while((current_number /= 10))
        ++count;

    return count;

}

size_t tester_t::results() const {

    std::cout << std::setw(16) << std::left << (name + ":");

    size_t column_count = 0;
    for(std::tuple<int, bool, std::string> result : test_results) {

        if(column_count >= COLUMN_SIZE) {

            std::cout << '\n';
            std::cout << std::setw(16) << std::left << "...";
            column_count = 0;

        }

        column_count += number_characters(std::get<0>(result)) + 1;

        if(std::get<1>(result))
            std::cout << "\x1b[0;33m";
        else
            std::cout << "\x1b[0;31m";

        std::cout << std::get<0>(result);

        std::cout << "\x1b[0;0m ";

    }

    std::cout << "\n\n";

    if(number_passed != test_count) {

        for(std::tuple<int, bool, std::string> result : test_results) {

            if(!std::get<1>(result))
                std::cout << std::get<2>(result);

        }

    }


    std::cout << "Passed " << number_passed << " out of " << test_count << '\n';

    return test_count - number_passed;

}
