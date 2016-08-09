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

#include <srcyuml_attribute.hpp>
#include <srcyuml_type.hpp>

class srcyuml_class {

private:

    enum class_type { NONE, INTERFACE, ABSTRACT, DATATYPE };

    const ClassPolicy::ClassData * data;

    std::string name;

    bool has_field;
    bool has_constructor;
    bool has_default_constructor;
    bool has_public_default_constructor;
    bool has_copy_constructor;
    bool has_public_copy_constructor;
    bool has_destructor;
    bool has_public_assignment;
    const FunctionSignaturePolicy::FunctionSignatureData * assignment;

    bool has_operator;
    bool has_method;

    class_type type;
    std::vector<srcyuml_attribute> attributes;

public:
    srcyuml_class(const ClassPolicy::ClassData * data)
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
          type(NONE) {

            analyze_data();

    }

    ~srcyuml_class() { if(data) delete data; }

    friend std::ostream & operator<<(std::ostream & out, const srcyuml_class & aclass) {

        out << '[';

        if(aclass.type == DATATYPE)
            out << "«datatype»;";
        else if(aclass.type == INTERFACE)
            out << "«interface»;";

        // not sure if should be gulliments or {}
        else if(aclass.type == ABSTRACT)
            out << "«abstract»;";

        out << aclass.name;

        if(aclass.has_field || aclass.has_method)
            out << '|';

        for(const srcyuml_attribute & attribute : aclass.attributes)
            out << attribute;

        if(aclass.has_method)
            out << '|';

        for(FunctionSignaturePolicy::FunctionSignatureData * function : aclass.data->methods[ClassPolicy::PUBLIC]) {
            out << "+ "; output_method(out, *function) << ';';
        }

        for(FunctionSignaturePolicy::FunctionSignatureData * function : aclass.data->methods[ClassPolicy::PRIVATE]) {
            out << "- "; output_method(out, *function) << ';';
        }

        for(FunctionSignaturePolicy::FunctionSignatureData * function : aclass.data->methods[ClassPolicy::PROTECTED]) {
            out << "# "; output_method(out, *function) << ';';
        }

        out << "]\n";

        return out;

    }

private:

    void analyze_data() {

        name = data->name->SimpleName();

        has_field = data->fields[ClassPolicy::PUBLIC].size() || data->fields[ClassPolicy::PRIVATE].size() || data->fields[ClassPolicy::PROTECTED].size();
        has_constructor = data->constructors[ClassPolicy::PUBLIC].size() || data->constructors[ClassPolicy::PRIVATE].size() || data->constructors[ClassPolicy::PROTECTED].size();
        has_destructor = data->hasDestructor;
        has_method = data->operators[ClassPolicy::PUBLIC].size() || data->operators[ClassPolicy::PRIVATE].size() || data->operators[ClassPolicy::PROTECTED].size();
        has_method = data->methods[ClassPolicy::PUBLIC].size() || data->methods[ClassPolicy::PRIVATE].size() || data->methods[ClassPolicy::PROTECTED].size();

        bool only_public_methods = 
            data->operators[ClassPolicy::PUBLIC].size() && data->operators[ClassPolicy::PRIVATE].empty() && data->operators[ClassPolicy::PROTECTED].empty()
            && data->methods[ClassPolicy::PUBLIC].size() && data->methods[ClassPolicy::PRIVATE].empty() && data->methods[ClassPolicy::PROTECTED].empty();

        for(std::size_t access = 0; access < ClassPolicy::PROTECTED; ++access) {

            for(const FunctionSignaturePolicy::FunctionSignatureData * constructor : data->constructors[access]) {

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

            for(const FunctionSignaturePolicy::FunctionSignatureData * op : data->operators[access]) {

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

            type = DATATYPE;

        } else if( !has_constructor
                && !has_field
                && !has_destructor
                && only_public_methods
                && (!assignment || assignment->isPureVirtual)) {

            bool is_interface = true;
            for(FunctionSignaturePolicy::FunctionSignatureData * op : data->operators[ClassPolicy::PUBLIC]) {

                if(!op->isPureVirtual) {
                    is_interface = false;
                }

            }
            for(FunctionSignaturePolicy::FunctionSignatureData * function : data->methods[ClassPolicy::PUBLIC]) {

                if(!function->isPureVirtual) {
                    is_interface = false;
                }

            }

            if(is_interface)
                type = INTERFACE;

        }

        for(std::size_t access = 0; access <= ClassPolicy::PROTECTED; ++access) {

            for(const DeclTypePolicy::DeclTypeData * field : data->fields[access]) {

                attributes.emplace_back(field, (ClassPolicy::AccessSpecifier)access);

            }

        }


    }

    static std::ostream & output_decl(std::ostream & out, const DeclTypePolicy::DeclTypeData & field) {

        if(field.name)
            out << *field.name;
        out << ": "; srcyuml_type the_type(field.type);
        out << the_type;

        return out;

    }

    static std::ostream & output_parameter(std::ostream & out, const ParamTypePolicy::ParamTypeData & parameter) {

        if(parameter.name)
            out << *parameter.name;
        out << ": ";

        srcyuml_type the_type(parameter.type);
        out << the_type;

        return out;

    }

    static std::ostream & output_return_type(std::ostream & out, const TypePolicy::TypeData & type) {

        if(type.types.size() == 1
         && type.types.back().second == TypePolicy::NAME
         && static_cast<NamePolicy::NameData *>(type.types.back().first)->ToString() == "void")
            return out;

        out << ": ";
        srcyuml_type the_type(&type);
        out << the_type;

        return out;

    }

    static std::ostream & output_method(std::ostream & out, const FunctionSignaturePolicy::FunctionSignatureData & function) {

        out << function.name->SimpleName();
        out << '(';
        for(std::size_t pos = 0; pos < function.parameters.size(); ++pos) {

            if(pos != 0)
                out << ", ";

            output_parameter(out, *function.parameters[pos]);

        }
        out << ')';

        if(function.returnType)
            output_return_type(out, *function.returnType);

        return out;

    }

};

 #endif