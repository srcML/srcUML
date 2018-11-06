/**
 * @file srcuml_type.hpp
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

#ifndef INCLUDED_SRCUML_TYPE_HPP
#define INCLUDED_SRCUML_TYPE_HPP

#include <TypePolicySingleEvent.hpp>

class srcuml_type {

private:

    std::shared_ptr<TypePolicy::TypeData> data;

    std::string name;
    bool is_numeric;

    bool is_pointer;
    bool is_reference;
    bool is_rvalue;

    bool is_const;

    bool is_vector;
    bool is_list;
    bool is_deque;
    bool is_forward_list;
    bool is_stack;
    bool is_queue;
    bool is_priority_queue;
    bool is_array;

    bool is_set;
    bool is_map;
    bool is_unordered_set;
    bool is_unordered_map;

    bool is_auto_ptr;
    bool is_shared_ptr;
    bool is_unique_ptr;
    bool is_scoped_ptr;

    bool has_index;
    std::string index;

public:

    srcuml_type(TypePolicy::TypeData * data) : srcuml_type(std::shared_ptr<TypePolicy::TypeData>(data)) {}

    srcuml_type(std::shared_ptr<TypePolicy::TypeData> data)
        : data(data),
        name(),
        is_numeric(false),

        is_pointer(false),
        is_reference(false),
        is_rvalue(false),

        is_const(false),

        is_vector(false),
        is_list(false),
        is_deque(false),
        is_forward_list(false),
        is_stack(false),
        is_queue(false),
        is_priority_queue(false),
        is_array(false),

        is_set(false),
        is_map(false),
        is_unordered_set(false),
        is_unordered_map(false),

        is_auto_ptr(false),
        is_shared_ptr(false),
        is_unique_ptr(false),
        is_scoped_ptr(false),

        has_index(false),
        index() {

            resolve_type();
            check_is_numeric();

    }

    // ~srcuml_type() { delete data; }

    const std::string & get_type_name() const {
        return name;        
    }

    bool get_is_pointer() const {
        return is_pointer;       
    }

    bool get_is_reference() const {
        return is_reference;       
    }

    bool get_is_rvalue() const {
        return is_rvalue;       
    }

    bool get_is_const() const {
        return is_const;       
    }

    bool get_is_container() const {
        return is_vector || is_list || is_deque || is_forward_list
            || is_stack || is_queue || is_priority_queue || is_array
            || is_set || is_map || is_unordered_set || is_unordered_map;
    }

    bool get_is_ordered() const {
        return is_vector || is_list || is_deque || is_forward_list
            || is_stack || is_queue || is_priority_queue || is_array;
    }

    bool get_is_smart_pointer() const {
        return is_auto_ptr || is_shared_ptr || is_unique_ptr || is_scoped_ptr;       
    }

    bool get_is_composite() const {
        return (!is_pointer && !is_reference && !is_rvalue)
            || is_shared_ptr;
            /** @todo unique_ptr? */
    }

    bool get_is_aggregate() const {
        return is_pointer || is_reference || is_rvalue
            || is_auto_ptr
            || is_scoped_ptr;
    }

    bool get_has_index() const {
        return has_index;       
    }

    const std::string get_index() const {
        return index;       
    }

    std::string get_string_type() const {
        
        std::string t = "";

        if(is_numeric)
            t += "number";
        else
            t += name;

        return t;
    }

    friend std::ostream & operator<<(std::ostream & out, const srcuml_type & type) {

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

        if(name == "vector")
            is_vector = true;
        else if(name == "list")
            is_list = true;
        else if(name == "deque")
            is_deque = true;
        else if(name == "forward_list")
            is_forward_list = true;
        else if(name == "stack")
            is_stack = true;
        else if(name == "queue")
            is_queue = true;
        else if(name == "priority_queue")
            is_priority_queue = true;
        else if(name == "array")
            is_array = true;

        else if(name == "set")
            is_set = true;
        else if(name == "map")
            is_map = true;
        else if(name == "unordered_set")
            is_unordered_set = true;
        else if(name == "unordered_map")
            is_unordered_map = true;

        else if(name == "auto_ptr")
            is_auto_ptr = true;
        else if(name == "shared_ptr")
            is_shared_ptr = true;
        else if(name == "unique_ptr")
            is_unique_ptr = true;
        else if(name == "scoped_ptr")
            is_scoped_ptr = true;

    }

    void resolve_type() {

        std::vector<std::pair<void *, TypePolicy::TypeType>>::const_reverse_iterator citr;
        for(citr = data->types.rbegin(); citr != data->types.rend(); ++citr) {

            if(citr->second == TypePolicy::POINTER)
                is_pointer = true;

            if(citr->second == TypePolicy::REFERENCE)
                is_reference = true;

            if(citr->second == TypePolicy::RVALUE)
                is_rvalue = true;

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

        for(; citr != data->types.rend(); ++citr) {

            if(citr->second != TypePolicy::SPECIFIER)
                continue;

            const std::string & specifier = *static_cast<const std::string *>(citr->first);

            if(specifier == "const") {

                is_const = true;
                break;

            }


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

            if(citr->second == TemplateArgumentPolicy::OPERATOR && (*static_cast<std::string *>(citr->first)) == "*")
                is_pointer = true;

            if(citr->second == TemplateArgumentPolicy::REFERENCE)
                is_reference = true;

            if(citr->second == TemplateArgumentPolicy::RVALUE)
                is_rvalue = true;

            if(citr->second != TemplateArgumentPolicy::NAME)
                continue;

            return resolve_template_type(static_cast<NamePolicy::NameData *>(citr->first));

        }

        return std::string();

    }

};

 #endif
