/**
 * @file static_outputter.hpp
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

#ifndef INCLUDED_STATIC_OUTPUTTER_HPP
#define INCLUDED_STATIC_OUTPUTTER_HPP

#include <string>
#include <ostream>
#include <sstream>

#define COMBINING_LOW_LINE "\u0332"
class static_outputter {

public:

    static std::size_t num_utf_bytes(const unsigned char & character) {

        if(character > 0xC0 && character <= 0xdf) return 2;
        if(character > 0xE0 && character <= 0xef) return 3;
        if(character > 0xF0 && character <= 0xf4) return 4;

        return 1;
    }

    template <typename T>
    static std::ostream & output(std::ostream & out, const T & t) {

        std::ostringstream str_out;
        str_out << t;

        const std::string & str = str_out.str();
        std::size_t size = str.size();
        for(std::size_t pos = 0; pos < size;) {

            std::size_t num_bytes = num_utf_bytes(str[pos]);

            while(num_bytes--)
               out << str[pos++];

            out << COMBINING_LOW_LINE;

        }

        return out;

    }

};


template <>
std::ostream & static_outputter::output<std::string>(std::ostream & out, const std::string & t) {

    const std::string & str = t;
    std::size_t size = str.size();
    for(std::size_t pos = 0; pos < size;) {

        std::size_t num_bytes = static_outputter::num_utf_bytes(str[pos]);

        while(num_bytes--)
           out << str[pos++];

        out << COMBINING_LOW_LINE;

    }

    return out;

}
#undef COMBINING_LOW_LINE
#endif

