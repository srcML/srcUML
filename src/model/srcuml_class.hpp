/**
 * @file srcuml_class.hpp
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

#ifndef INCLUDED_SRCUML_CLASS_HPP
#define INCLUDED_SRCUML_CLASS_HPP

#include <ClassPolicySingleEvent.hpp>

#include <srcuml_attribute.hpp>
#include <srcuml_operation.hpp>
#include <static_outputter.hpp>

#include <map>
#include <set>

class srcuml_class {

private:
    const ClassPolicy::ClassData * data;

    std::string name;

    bool has_field;
    bool has_constructor;
    bool has_default_constructor;
    bool has_public_default_constructor;
    bool has_copy_constructor;
    bool has_public_copy_constructor;
    // handle if pure virtual destructor
    bool has_destructor;
    bool has_public_assignment;
    const FunctionPolicy::FunctionData * assignment;

    bool has_operator;
    bool has_method;

    bool is_interface;
    bool is_abstract;
    bool is_datatype;

    bool is_finalized;

    std::map<std::string, const FunctionPolicy::FunctionData *> implemented_functions_map;
    std::map<std::string, const FunctionPolicy::FunctionData *> pure_virtual_functions_map;

    std::vector<srcuml_attribute> attributes;

    std::set<std::string> stereotypes;

public:
    srcuml_class(const ClassPolicy::ClassData * data)
        : data(data),
          has_field(false),
          has_constructor(false),
          has_default_constructor(false),
          has_public_default_constructor(false),
          has_public_copy_constructor(false),
          has_copy_constructor(false),
          has_destructor(false),
          has_public_assignment(false),
          assignment(nullptr),
          has_operator(false),
          has_method(false),
          is_interface(false),
          is_abstract(data->hasPureVirtual),
          is_datatype(false),
          is_finalized(false) {

            analyze_data();

    }

    ~srcuml_class() { if(data) delete data; }

    const ClassPolicy::ClassData & get_data() const {

        return *data;

    }


    const std::string & get_name() const {

        return name;

    }

    std::string get_srcuml_name() const { //shouldn't do any formatting

        if(is_interface)
            return "«interface»;" + name;
        // not sure if should be gulliments or {}
        if(is_abstract)
            return "｛abstract｝;" + name;

        if(is_datatype)
            return "«datatype»;" + name;

        return name;

    }

    bool get_is_interface() const {
        return is_interface;
    }

    void set_is_interface(bool is_interface){
        this->is_interface = is_interface;
    }

    bool get_is_abstract() const {
        return is_abstract;
    }

    void set_is_abstract(bool is_abstract){
        this->is_abstract = is_abstract;
    }

    bool get_is_finalized() const {
        return is_finalized;
    }

    void set_is_finalized(bool is_finalized) {
        this->is_finalized = is_finalized;
    }

    bool get_has_method() const {
    	return has_method;
    }

    bool get_has_field() const {
    	return has_field;
    }

    const std::map<std::string, const FunctionPolicy::FunctionData *> & get_implemented_functions_map() const {
        return implemented_functions_map;
    }

    std::map<std::string, const FunctionPolicy::FunctionData *> & get_pure_virtual_functions_map() {
        return pure_virtual_functions_map;
    }

    const std::vector<srcuml_attribute> & get_attributes() const {
        return attributes;
    } 

    const std::set<std::string> & get_stereotypes() const {
        return stereotypes;
    }

private:

    void analyze_data() {

        name = data->name->SimpleName();
        // if(data->isGeneric) name += "<>";

        has_field = data->fields[ClassPolicy::PUBLIC].size() || data->fields[ClassPolicy::PRIVATE].size() || data->fields[ClassPolicy::PROTECTED].size();
        has_constructor = data->constructors[ClassPolicy::PUBLIC].size() || data->constructors[ClassPolicy::PRIVATE].size() || data->constructors[ClassPolicy::PROTECTED].size();
        has_destructor = data->hasDestructor;
        has_method = data->operators[ClassPolicy::PUBLIC].size() || data->operators[ClassPolicy::PRIVATE].size() || data->operators[ClassPolicy::PROTECTED].size();
        has_method = data->methods[ClassPolicy::PUBLIC].size() || data->methods[ClassPolicy::PRIVATE].size() || data->methods[ClassPolicy::PROTECTED].size();

        bool no_private_or_protected_methods
            = data->operators[ClassPolicy::PRIVATE].empty() && data->operators[ClassPolicy::PROTECTED].empty()
             && data->methods[ClassPolicy::PRIVATE].empty() && data->methods[ClassPolicy::PROTECTED].empty();

        for(std::size_t access = 0; access < ClassPolicy::PROTECTED; ++access) {

            for(const FunctionPolicy::FunctionData * constructor : data->constructors[access]) {

                if(constructor->isDelete)
                    continue;

                if(constructor->parameters.empty()) {

                    has_default_constructor = true;
                    if(access == ClassPolicy::PUBLIC)
                        has_public_default_constructor = true;

                } else if(constructor->parameters.size() == 1) {

                    for(const std::pair<void *, TypePolicy::TypeType> & p_type : constructor->parameters.back()->type->types) {

                        if(p_type.second == TypePolicy::NAME && name == static_cast<NamePolicy::NameData *>(p_type.first)->SimpleName()) {

                            has_copy_constructor = true;
                            if(access == ClassPolicy::PUBLIC)
                                has_public_copy_constructor = true;

                        }

                    }

                }

            }
        }

        for(std::size_t access = 0; access <= ClassPolicy::PROTECTED; ++access) {

            for(const FunctionPolicy::FunctionData * op : data->operators[access]) {

                if(!op->isDelete && !op->name->names.empty() && op->name->names.back()->ToString() == "=") {
                    
                    assignment = op;
                    if(access == ClassPolicy::PUBLIC)
                        has_public_assignment = true;
                    break;

                }

            }

        }

        if((!has_constructor && !assignment)
            || (has_public_default_constructor && has_public_copy_constructor && has_public_assignment)) {
            is_datatype = true;
        }

        if(    !has_constructor
            && !has_field
            && !has_destructor
            && no_private_or_protected_methods
            && (!assignment || assignment->isPureVirtual)) {

            is_interface = true;
            for(FunctionPolicy::FunctionData * op : data->operators[ClassPolicy::PUBLIC]) {
                if(!op->isPureVirtual) {
                    is_interface = false;
                    break;
                }
            }
            if(is_interface ) {
                for(FunctionPolicy::FunctionData * function : data->methods[ClassPolicy::PUBLIC]) {
                    if(!function->isPureVirtual) {
                        is_interface = false;
                        break;
                    }

                }
            }

        }

        for(std::size_t access = 0; access <= ClassPolicy::PROTECTED; ++access) {

            for(const DeclTypePolicy::DeclTypeData * field : data->fields[access]) {
                attributes.emplace_back(field, (ClassPolicy::AccessSpecifier)access);
            }

        }

        for(std::size_t access = 0; access <= ClassPolicy::PROTECTED; ++access) {

            for(const FunctionPolicy::FunctionData * method : data->methods[access]) {
                if(method->isPureVirtual)
                    pure_virtual_functions_map[method->ToString()] = method;
                else
                    implemented_functions_map[method->ToString()] = method;
            }

            for(const FunctionPolicy::FunctionData * op : data->operators[access]) {
               if(op->isPureVirtual)
                    pure_virtual_functions_map[op->ToString()] = op;
                else
                    implemented_functions_map[op->ToString()] = op;
            }

        }

        stereotypes = data->stereotypes;

    }

};

 #endif
