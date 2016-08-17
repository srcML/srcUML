/**
 * @file srcyuml_parameter.hpp
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

#ifndef INCLUDED_SRCYUML_PARAMETER_HPP
#define INCLUDED_SRCYUML_PARAMETER_HPP

#include <ParamTypePolicySingleEvent.hpp>

#include <srcyuml_type.hpp>
#include <string>

class srcyuml_parameter {

private:

    const ParamTypePolicy::ParamTypeData * data;

    srcyuml_type type;
    std::string name;

    bool is_pointer;

    bool has_index;
    std::string index;

public:
    srcyuml_parameter(const ParamTypePolicy::ParamTypeData * data)
        : data(data),
          type(data->type),
          name(data->name ? data->name->ToString() : ""),
          is_pointer(type.get_is_pointer()),
          has_index(false),
          index() {

            analyze_parameter();

    }

    const std::string & get_name() const {
        return name;
    }

    const srcyuml_type & get_type() const {
        return type;
    }

    const std::string get_multiplicity() const {

        if(!index.empty()) {

            std::string multiplicity = "［";
            if(is_pointer)
                multiplicity += "0..";
            multiplicity += index;
            multiplicity += "］";

            return multiplicity;

        } 

        if(is_pointer
            || has_index
            || type.get_is_container()) {
            return "［*］";
        }

        if(type.get_is_smart_pointer()) {
            return "［0..1］";
        }

        return "";

    }

    // ~srcyuml_parameter() { if(data) delete data; }

    friend std::ostream & operator<<(std::ostream & out, const srcyuml_parameter & parameter) {

        if(!parameter.type.get_is_const() && (parameter.type.get_is_pointer() || parameter.type.get_is_reference()))
            out << "inout";

        out << " " << parameter.name << ": " << parameter.type;

        // out << parameter.get_multiplicity();

        return out;

    }

private:
    void analyze_parameter() {

        if(!data->name) return;

        if(!data->name->arrayIndices.empty()) {
            has_index = true;
            index = data->name->arrayIndices[0];
        } else if(type.get_has_index()) {
            has_index = true;
            index = type.get_index();
        }

    }

};

 #endif