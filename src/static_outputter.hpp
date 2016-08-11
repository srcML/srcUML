/**
 * @file static_outputter.hpp
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

#ifndef INCLUDED_STATIC_OUTPUTTER_HPP
#define INCLUDED_STATIC_OUTPUTTER_HPP

#include <string>
#include <ostream>
#include <sstream>

class static_outputter {
#define COMBINING_LOW_LINE "\u0332"

public:

    template <typename T>
    static std::ostream & output(std::ostream & out, const T & t) {

        std::ostringstream str_out;
        str_out << t;

        for(char character : str_out.str()) {
            out << character << COMBINING_LOW_LINE;
        }

        return out;

    }

#undef COMBINING_LOW_LINE
};

#endif

