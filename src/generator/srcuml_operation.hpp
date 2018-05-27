/**
 * @file srcuml_operation.hpp
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

#ifndef INCLUDED_SRCUML_OPERATION_HPP
#define INCLUDED_SRCUML_OPERATION_HPP

#include <FunctionPolicySingleEvent.hpp>

#include <srcuml_type.hpp>
#include <srcuml_parameter.hpp>

#include <srcuml_utilities.hpp>

#include <sstream>
#include <iterator>
#include <string>
#include <set>

class srcuml_operation {

private:
    const FunctionPolicy::FunctionData * data;
    const ClassPolicy::AccessSpecifier visibility;

    std::set<std::string> stereotypes;

public:
    srcuml_operation(const FunctionPolicy::FunctionData * data, ClassPolicy::AccessSpecifier visibility)
        : data(data),
          visibility(visibility) {

            std::istringstream stream(data->stereotype);
            stereotypes = std::set<std::string>(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());

            analyze_operation();
    }

    // ~srcuml_operation() { if(data) delete data; }

    const FunctionPolicy::FunctionData & get_data() const {

        return *data;

    }

    const std::set<std::string> & get_stereotypes() const {

        return stereotypes;

    }

    friend std::ostream & operator<<(std::ostream & out, const srcuml_operation & operation) {

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

            srcuml_parameter the_parameter(operation.data->parameters[pos]);
            out << the_parameter;

        }
        out << ')';

        if(operation.data->returnType) {

            srcuml_type the_type(operation.data->returnType);
            if(the_type.get_type_name() != "void") {
                out << ": ";
                out << the_type;
            }            

        }

        if(!operation.data->stereotype.empty()) {

            out << " ｛";
            out << operation.data->stereotype;
            out << "｝";

        }

        return out;

    }

private:

    void analyze_operation() {}

};

 #endif
