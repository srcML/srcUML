/**
 * @file srcyuml_type.hpp
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

#ifndef INCLUDED_SRCYUML_TYPE_HPP
#define INCLUDED_SRCYUML_TYPE_HPP

#include <TypePolicySingleEvent.hpp>

class srcyuml_type {

private:

    const TypePolicy::TypeData * data;

    std::string name;
    bool is_numeric;
    bool is_pointer;

public:
    srcyuml_type(const TypePolicy::TypeData * data)
        : data(data),
        name(),
        is_numeric(false),
        is_pointer(false) {

            resolve_type();
            check_is_numeric();

        }

    ~srcyuml_type() { delete data; }


    friend std::ostream & operator<<(std::ostream & out, const srcyuml_type & type) {

        if(type.is_numeric)
            out << "number";
        else
            out << type.name;

        if(type.is_pointer)
            out << "［*］";

        return out;

    }

private:

    void check_is_numeric() {
        if(    name == "int"
            || name == "double"
            || name == "long"
            || name == "size_t"
            || name == "short"
            || name == "float"
            || name == "signed"
            || name == "unsigned")
            is_numeric = true;

    }

    void resolve_type() {

        for(std::vector<std::pair<void *, TypePolicy::TypeType>>::const_reverse_iterator citr = data->types.rbegin(); citr != data->types.rend(); ++citr) {

            if(citr->second == TypePolicy::POINTER)
                is_pointer = true;

            if(citr->second != TypePolicy::NAME)
                continue;

            /** @todo need to look and see if using only last is valid */
            const NamePolicy::NameData * type_name = static_cast<const NamePolicy::NameData *>(citr->first);
            if(!type_name->names.empty())
                type_name = type_name->names.back();
            std::string type_str;

            /** @todo what if template argument is pointer? */
            if(type_name->templateArguments.empty()) {
                type_str = type_name->SimpleName();
            } else {
                type_str = resolve_template_type(type_name);
            }


            name = type_str;
            break;

        }

    }

    std::string resolve_template_type(const NamePolicy::NameData * type_name) {

        if(type_name->templateArguments.empty())
            return type_name->SimpleName();

        /** @todo need to look and see if using only last is valid */
        for(std::vector<std::pair<void *, TemplateArgumentPolicy::TemplateArgumentType>>::const_reverse_iterator citr = type_name->templateArguments.back()->data.rbegin();
            citr != type_name->templateArguments.back()->data.rend();
            ++citr) {

            if(citr->second == TemplateArgumentPolicy::POINTER)
                is_pointer = true;

            if(citr->second != TemplateArgumentPolicy::NAME)
                continue;

            return resolve_template_type(static_cast<NamePolicy::NameData *>(citr->first));

        }

        return std::string();

    }

};

 #endif