/**
 * @file srcyuml_attribute.hpp
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

#ifndef INCLUDED_SRCYUML_ATTRIBUTE_HPP
#define INCLUDED_SRCYUML_ATTRIBUTE_HPP

#include <DeclTypePolicySingleEvent.hpp>

#include <srcyuml_type.hpp>
#include <string>

class srcyuml_attribute {

private:

    const DeclTypePolicy::DeclTypeData * data;
    const ClassPolicy::AccessSpecifier visibility;

    srcyuml_type type;
    std::string name;

    bool is_pointer;

    bool is_static;

    bool has_index;
    std::string index;

public:
    srcyuml_attribute(const DeclTypePolicy::DeclTypeData * data, ClassPolicy::AccessSpecifier visibility)
        : data(data),
          visibility(visibility),
          type(data->type),
          name(data->name ? data->name->ToString() : ""),
          is_pointer(type.get_is_pointer()),
          is_static(data->isStatic),
          has_index(false),
          index() {

            analyze_attribute();

    }

    const std::string & get_name() const {
        return name;
    }

    const srcyuml_type & get_type() const {
        return type;
    }

    bool get_is_static() const {
        return is_static;
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

    // ~srcyuml_attribute() { if(data) delete data; }

    friend std::ostream & operator<<(std::ostream & out, const srcyuml_attribute & attribute) {

        if(attribute.visibility == ClassPolicy::PUBLIC)
            out << '+';
        else if(attribute.visibility == ClassPolicy::PRIVATE)
            out << '-';
        else if(attribute.visibility == ClassPolicy::PROTECTED)
            out << '#';

        out << ' ';

        out << attribute.name << ": " << attribute.type;

        out << attribute.get_multiplicity();

        return out;

    }

private:
    void analyze_attribute() {

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