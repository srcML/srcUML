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

    bool is_container;
    bool is_ordered;
    bool is_smart_pointer;

    bool has_index;
    std::string index;

public:
    srcyuml_type(const TypePolicy::TypeData * data)
        : data(data),
        name(),
        is_numeric(false),
        is_pointer(false),
        is_container(false),
        is_ordered(false),
        is_smart_pointer(false),
        has_index(false),
        index() {

            resolve_type();
            check_is_numeric();

    }

    ~srcyuml_type() { delete data; }


    bool get_is_pointer() const {
        return is_pointer;       
    }

    bool get_is_container() const {
        return is_container;       
    }

    bool get_is_ordered() const {
        return is_ordered;       
    }

    bool get_is_smart_pointer() const {
        return is_smart_pointer;       
    }

    bool get_has_index() const {
        return has_index;       
    }

    const std::string get_index() const {
        return index;       
    }

    friend std::ostream & operator<<(std::ostream & out, const srcyuml_type & type) {

        if(type.is_numeric)
            out << "number";
        else
            out << type.name;

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
            || name == "unsigned"
          )
            is_numeric = true;

    }

    void check_template_base(const std::string & name) {

        if(  name == "vector"
          || name == "list"
          || name == "deque"
          || name == "forward_list"
          || name == "stack"
          || name == "queue"
          || name == "priority_queue"
          || name == "array") {
            is_container = true;
            is_ordered = true;
        } else if( name == "set"
                || name == "map"
                || name == "unordered_set"
                || name == "unordered_map") {
            is_container = true;
        } else if(name == "auto_ptr"
               || name == "shared_ptr"
               || name == "unique_ptr") {
                is_smart_pointer = true;
        }

    }

    void resolve_type() {

        for(std::vector<std::pair<void *, TypePolicy::TypeType>>::const_reverse_iterator citr = data->types.rbegin(); citr != data->types.rend(); ++citr) {

            if(citr->second == TypePolicy::POINTER)
                is_pointer = true;

            if(citr->second != TypePolicy::NAME)
                continue;

            /** @todo need to look and see if using only last is valid */
            /** @todo issue in srcML to simplify markup, should make this simpler possibly eliminate if condition */
            const NamePolicy::NameData * type_name = static_cast<const NamePolicy::NameData *>(citr->first);
            if(type_name->names.size() >= 2)
                type_name = type_name->names.back();

            std::string type_str;

            /** @todo what if template argument is pointer? */
            if(type_name->templateArguments.empty()) {
                type_str = type_name->SimpleName();
            } else {
                check_template_base(type_name->SimpleName());
                type_str = resolve_template_type(type_name);
            }

            has_index = !type_name->arrayIndices.empty();
            if(has_index)
                index = type_name->arrayIndices[0];

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