/**
 * @file srcyuml_operation.hpp
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

#ifndef INCLUDED_SRCYUML_OPERATION_HPP
#define INCLUDED_SRCYUML_OPERATION_HPP

#include <FunctionPolicySingleEvent.hpp>

#include <srcyuml_type.hpp>
#include <srcyuml_parameter.hpp>

#include <srcyuml_utilities.hpp>

class srcyuml_operation {

private:
    const FunctionPolicy::FunctionData * data;
    const ClassPolicy::AccessSpecifier visibility;

    std::string stereotype;

public:
    srcyuml_operation(const FunctionPolicy::FunctionData * data, ClassPolicy::AccessSpecifier visibility)
        : data(data),
          visibility(visibility),
          stereotype(data->stereotype) {

            srcyuml::trim(stereotype);
            analyze_operation();
    }

    // ~srcyuml_operation() { if(data) delete data; }

    const FunctionPolicy::FunctionData & get_data() const {

        return *data;

    }

    friend std::ostream & operator<<(std::ostream & out, const srcyuml_operation & operation) {

        if(operation.visibility == ClassPolicy::PUBLIC)
            out << '+';
        else if(operation.visibility == ClassPolicy::PRIVATE)
            out << '-';
        else if(operation.visibility == ClassPolicy::PROTECTED)
            out << '#';

        out << ' ';

        out << operation.data->name->SimpleName();

        out << '(';
        for(std::size_t pos = 0; pos < operation.data->parameters.size(); ++pos) {

            if(pos != 0)
                out << ", ";

            srcyuml_parameter the_parameter(operation.data->parameters[pos]);
            out << the_parameter;

        }
        out << ')';

        if(operation.data->returnType) {

            srcyuml_type the_type(operation.data->returnType);
            if(the_type.get_type_name() != "void") {
                out << ": ";
                out << the_type;
            }            

        }

        if(!operation.data->stereotype.empty()) {

            out << " ｛";
            out << operation.stereotype;
            out << "｝";

        }

        return out;

    }

private:

    void analyze_operation() {}

};

 #endif
