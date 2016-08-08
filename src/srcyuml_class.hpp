/**
 * @file srcyuml_class.hpp
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

#ifndef INCLUDED_SRCYUML_CLASS_HPP
#define INCLUDED_SRCYUML_CLASS_HPP

#include <ClassPolicySingleEvent.hpp>

class srcyuml_class {

private:
    const ClassPolicy::ClassData * data;

public:
    srcyuml_class(const ClassPolicy::ClassData * data) : data(data) {}
    ~srcyuml_class() { delete data; }

    void analyze_data() {}


    friend std::ostream & operator<<(std::ostream & out, const srcyuml_class & aclass) {

        out << '[';
        out << *aclass.data->name;

        bool has_fields = aclass.data->fields[0].size() || aclass.data->fields[1].size() || aclass.data->fields[2].size();
        bool has_methods = aclass.data->methods[0].size() || aclass.data->methods[1].size() || aclass.data->methods[2].size();

        if(has_fields || has_methods)
            out << '|';

        for(DeclTypePolicy::DeclTypeData * d_data : aclass.data->fields[0]) {
            out << "+ " << *d_data << ';';
        }

        for(DeclTypePolicy::DeclTypeData * d_data : aclass.data->fields[1]) {
            out << "- " << *d_data << ';';
        }

        for(DeclTypePolicy::DeclTypeData * d_data : aclass.data->fields[2]) {
            out << "# " << *d_data << ';';
        }

        if(has_methods)
            out << '|';

        for(FunctionSignaturePolicy::FunctionSignatureData * f_data : aclass.data->methods[0]) {
            out << "+ " << *f_data << ';';
        }

        for(FunctionSignaturePolicy::FunctionSignatureData * f_data : aclass.data->methods[1]) {
            out << "- " << *f_data << ';';
        }

        for(FunctionSignaturePolicy::FunctionSignatureData * f_data : aclass.data->methods[2]) {
            out << "# " << *f_data << ';';
        }

        out << "]\n";

        return out;

    }

};

 #endif