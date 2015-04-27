/**
 * @file srcYUMLHandler.hpp
 *
 * @copyright Copyright (C) 2015 srcML, LLC. (www.srcML.org)
 *
 * srcYUML is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * srcYUML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_SRCYUML_HANDLER_HPP
#define INCLUDED_SRCYUML_HANDLER_HPP

#include <srcSAXHandler.hpp>
#include <string>
#include <map>
#include <list>
#include <iostream>
#include <vector>
#include <fstream>

std::string removePortionOfString(std::string, std::string);
std::string replaceCommas(std::string);
std::string removeSemiColons(std::string);
void resolveMultiplicity(struct AttributeDeclaration&);

struct AttributeDeclaration {
    
    std::string type;
    std::string name;
    std::string multiplicity;
    
    AttributeDeclaration(std::string t, std::string n) : type(t), name(n) {};
};

struct FunctionDeclaration {
    std::string returnType;
    std::string function_name;
    std::list<struct AttributeDeclaration> function_parameters;
    
    FunctionDeclaration(std::string rType, std::string fName, std::list<struct AttributeDeclaration> params) : returnType(rType), function_name(fName), function_parameters(params) {};
};


class srcYUMLClass {
public:
    // attributes
    // functions
    // inheritance list
    // Inside a class
    // Inside public
    // inside private
    // inside protected
    // inside inheritance list
    // public, private, protected

    bool is_abstract;
    
    // Keys can be public, private, or protected
    std::map<std::string, std::list<struct AttributeDeclaration> > class_data_members;
    //std::map<std::string, std::list<std::string> > class_functions;
    std::list<std::string> inheritance_list;
    std::map<std::string, std::list<struct FunctionDeclaration>> class_functions;
    std::map<std::string, srcYUMLClass> classes_in_class;
    
    srcYUMLClass() : is_abstract(false) {} ;
    
    std::string convertToYuml(std::string class_name) const{
        
        std::string yuml_format;
        bool public_data, public_functions, private_data, private_functions, protected_data, protected_functions;
        
        public_data = public_functions = private_data = private_functions = protected_data = protected_functions = false;
        
        // Check to see if we have any data in each visibility for class_data_members
        if(class_data_members.find("public") != class_data_members.end()) public_data = true;
        if(class_data_members.find("private") != class_data_members.end()) private_data = true;
        if(class_data_members.find("protected") != class_data_members.end()) protected_data = true;
        
        // Check to see if we have any functions in each visibility for class_functions
        if(class_functions.find("public") != class_functions.end()) public_functions = true;
        if(class_functions.find("private") != class_functions.end()) private_functions = true;
        if(class_functions.find("protected") != class_functions.end()) protected_functions = true;
        
        
        yuml_format = "[" + class_name;
        
        if(public_data || private_data || protected_data) {
            yuml_format += "|";
        }
        
        if(public_data) {
            
            for(const auto& itr : class_data_members.at("public")) {
                yuml_format += "+" + itr.name + ":" + itr.type + itr.multiplicity + ";";
            }
        }
        if(private_data) {
            for(const auto& itr : class_data_members.at("private")) {
                yuml_format += "+" + itr.name + ":" + itr.type + itr.multiplicity + ";";
            }
        }
        if(protected_data) {
            for(const auto& itr : class_data_members.at("protected")) {
                yuml_format += "+" + itr.name + ":" + itr.type + itr.multiplicity + ";";
            }
        }

        
        
        if(public_functions || private_functions || protected_functions)
        {
            yuml_format += "|";
        }
        if(public_functions) {
            for(const auto& itr : class_functions.at("public")) {
                yuml_format += "+" + itr.function_name + "( ";
                size_t numberOfFunctionParameters = 0;
                for(const auto& funcParamItr : itr.function_parameters) {
                    yuml_format += funcParamItr.name + ":" + funcParamItr.type + funcParamItr.multiplicity;
                    ++numberOfFunctionParameters;
                    if(numberOfFunctionParameters < itr.function_parameters.size()) {
                        yuml_format += "، ";
                    }
                }
                yuml_format += " ):" + itr.returnType + ";";
                
            }
        }
        if(private_functions) {
            for(const auto& itr : class_functions.at("private")) {
                yuml_format += "+" + itr.function_name + "( ";
                size_t numberOfFunctionParameters = 0;
                for(const auto& funcParamItr : itr.function_parameters) {
                    yuml_format += funcParamItr.name + ":" + funcParamItr.type + funcParamItr.multiplicity;
                    ++numberOfFunctionParameters;
                    if(numberOfFunctionParameters < itr.function_parameters.size()) {
                        yuml_format += "، ";
                    }
                }
                yuml_format += " ):" + itr.returnType + ";";
                
            }
        }
        if(protected_functions) {
            for(const auto& itr : class_functions.at("protected")) {
                yuml_format += "+" + itr.function_name + "( ";
                size_t numberOfFunctionParameters = 0;
                for(const auto& funcParamItr : itr.function_parameters) {
                    yuml_format += funcParamItr.name + ":" + funcParamItr.type + funcParamItr.multiplicity;
                    ++numberOfFunctionParameters;
                    if(numberOfFunctionParameters < itr.function_parameters.size()) {
                        yuml_format += "، ";
                    }
                }
                yuml_format += " ): " + itr.returnType + ";";
                
            }
        }
        
        // End class
        yuml_format += "]\n";
        
        return yuml_format;
    }
    
private:

    

    
};


/**
 * srcYUMLHandler
 *
 * Base class that provides hooks for SAX processing.
 */
class srcYUMLHandler : public srcSAXHandler {

private:
    
    // Map representing <className, dataInsideClass>
    std::map<std::string, srcYUMLClass> classes_in_source;
    std::list<struct AttributeDeclaration> function_parameters;
    // Map for classes AFTER they have been converted to yuml
    // The key is class name, and the string contained is the yuml string for that class
    // This will be used to build relationships in the yuml
    std::map<std::string, std::string> converted_classes;
    
    
    // bool variables to determine program states
    bool consuming_class,
         consuming_data_declaration,
         consuming_function,
         in_function_param_list,
         function_return_type_consumed,
         end_of_decl,
         data_member_type_consumed,
         class_in_class,
         class_in_class_name_consumed,
         in_public,
         in_private,
         in_protected,
         in_inheritance_list,
         class_name_consumed;
    
    
    // This could be a function or an attribute
    std::string current_recorded_data_in_class;
    std::string consuming_data_declaration_type;
    std::string current_data_declaration_names;
    std::string current_function_return_type;
    std::string current_function_name;
    
     // This is to be used to store class name until we hit the end tag
     // so that we know what class key to map the data to.
    std::string current_class;
    
    // Vector for holding the names of any classes contained in the current class
    std::vector<std::string> class_names_in_class_stack;
    
     // This holds what visibility layer we are in for the class
     // so that we can properly map where we got data from in our class
    std::string current_class_visibility;
    
    std::string yuml_relationships;
    
    std::ostream & output;
    

    
protected:

public:
    
    void processClassesInSource() {
        
        for(const auto& itr : classes_in_source) {
            converted_classes[itr.first] = itr.second.convertToYuml(itr.first);
            for(const auto& class_in_class_itr : itr.second.classes_in_class) {
                converted_classes[class_in_class_itr.first] = class_in_class_itr.second.convertToYuml(class_in_class_itr.first);
            }
        }

        for(const auto& itr : converted_classes) {
            output << itr.second;
        }
        
        resolveRelationships();
        
        output << "\n" + yuml_relationships;
        
    }
    /**
     * srcYUMLHandler
     *
     * Default constructor default values to everything
     */
    srcYUMLHandler(std::ostream & output) :  consuming_class(false),
                        consuming_data_declaration(false),
                        consuming_function(false),
                        in_function_param_list(false),
                        function_return_type_consumed(false),
                        end_of_decl(false),
                        data_member_type_consumed(false),
                        class_in_class(false),
                        class_in_class_name_consumed(false),
                        in_public(false),
                        in_private(false),
                        in_protected(false),
                        in_inheritance_list(false),
                        class_name_consumed(false),
                        output(output) {};


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    
    /**
     * startDocument
     *
     * SAX handler function for start of document.
     * Overide for desired behaviour.
     */
    virtual void startDocument() {}

    /**
     * endDocument
     *
     * SAX handler function for end of document.
     * Overide for desired behaviour.
     */
    virtual void endDocument() {}

    /**
     * startRoot
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * SAX handler function for start of the root element.
     * Overide for desired behaviour.
     */
    virtual void startRoot(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {}

    /**
     * startUnit
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * SAX handler function for start of an unit.
     * Overide for desired behaviour.
     */
    virtual void startUnit(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {}

    /**
     * startElement
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * SAX handler function for start of an element.
     * Overide for desired behaviour.
     */
    virtual void startElement(const char * localname, const char * prefix, const char * URI,
                                int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                                const struct srcsax_attribute * attributes) {
        std::string lname = localname;
        // We have started reading a class
        if(lname == "class" || lname == "struct") {
            consuming_class = true;
        }
        // We are about to read the class name
        else if((consuming_class && lname == "name" && srcml_element_stack[srcml_element_stack.size() - 2] == "class") || (consuming_class && lname == "name" && srcml_element_stack[srcml_element_stack.size() - 2] == "struct")) {
            current_recorded_data_in_class = "";
        }
        // We are about to start reading the inheritance list
        else if(consuming_class && lname == "super" && srcml_element_stack[srcml_element_stack.size() - 2] == "class"){
            in_inheritance_list = true;
        }
        // We have found the names in the inheritance list
        else if(consuming_class && in_inheritance_list && lname == "name"){
            current_recorded_data_in_class = "";
        }
        else if((consuming_class && lname == "class") || (consuming_class && lname == "struct"))
        {
            current_recorded_data_in_class = "";
            class_in_class = true;
            class_in_class_name_consumed = false;
        }
        // We are now in the public part of the class
        else if(lname == "public" && consuming_class) {
            in_public = true;
            current_class_visibility = "public";
        }
        // We are now in the private part of the class
        else if(lname == "private" && consuming_class) {
            in_private = true;
            current_class_visibility = "private";
        }
        // We are now in the protected part of the class
        else if(lname == "protected" && consuming_class) {
            in_protected = true;
            current_class_visibility = "protected";
        }
        // We are at a class attribute
        else if(lname == "decl_stmt" && consuming_class && !consuming_function && !(srcml_element_stack[srcml_element_stack.size() - 2] == "block")) {
            consuming_data_declaration = true;
            current_recorded_data_in_class = "";
        }
        else if(lname == "decl" && consuming_class && consuming_function && in_function_param_list) {
            consuming_data_declaration = true;
            current_recorded_data_in_class = "";
        }
        else if(lname == "decl" && consuming_class && !consuming_function && consuming_data_declaration) {
            current_recorded_data_in_class = "";
        }
        // We are at one of the classes functions
        else if(lname == "function" && consuming_class) {
            consuming_function = true;
            current_recorded_data_in_class = "";
        }
        else if(lname == "type" && consuming_class && consuming_function && !in_function_param_list) {
            current_recorded_data_in_class = "";
        }
        else if(lname == "parameter_list" && consuming_class && consuming_function && !in_function_param_list) {
            in_function_param_list = true;
            current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "std::");
            current_function_name = current_recorded_data_in_class;
        }

    }

    /**
     * endRoot
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of the root element.
     * Overide for desired behaviour.
     */
    virtual void endRoot(const char * localname, const char * prefix, const char * URI) {}

    /**
     * endUnit
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an unit.
     * Overide for desired behaviour.
     */
    virtual void endUnit(const char * localname, const char * prefix, const char * URI) {}

    /**
     * endElement
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an element.
     * Overide for desired behaviour.
     */
    virtual void endElement(const char * localname, const char * prefix, const char * URI) {
        std::string lname = localname;
        // If we hit the </name> tag and the parent is the Class tag we have consumed the class' name
        if((consuming_class && lname == "name" && srcml_element_stack[srcml_element_stack.size() - 1] == "class" && !class_name_consumed && !consuming_function) || (consuming_class && lname == "name" && srcml_element_stack[srcml_element_stack.size() - 1] == "struct" && !class_name_consumed && !consuming_function)) {
            recordClassName();
        }
        // If we hit this we have consumed the WHOLE type
        else if(consuming_class && consuming_data_declaration && lname == "type" && !data_member_type_consumed && !consuming_function && !consuming_function) {
            recordDataDeclarationType();
        }
        else if(consuming_class && lname == "decl" && consuming_data_declaration && data_member_type_consumed && !consuming_function) {
            buildDataDeclarationNamesString();
        }
        // We hit an </decl_stmt> tag in the class so we now have all of the declaration information
        else if(consuming_class && lname == "decl_stmt" && consuming_data_declaration && data_member_type_consumed && !consuming_function) {
            end_of_decl = true;
            recordFullDataDeclaration();
        }
        // This will keep track of classes in the class
        else if(consuming_class && class_in_class && !class_in_class_name_consumed && lname == "name")
        {
            class_names_in_class_stack.push_back(current_recorded_data_in_class);
            class_in_class_name_consumed = true;
        }
        // We have consumed the whole inheritance list
        else if(consuming_class && in_inheritance_list && lname == "name") {
            recordClassInheritanceList();
        }
        // We are no longer in public visibility
        // and we must reset the visibility string
        else if(consuming_class && lname == "public") {
            in_public = false;
            current_class_visibility = "";
        }
        // We are no longer in private visibility
        else if(consuming_class && lname == "private") {
            in_private = false;
            current_class_visibility = "";
        }
        // We are no longer in protected visibility
        else if(consuming_class && lname == "protected") {
            in_protected = false;
            current_class_visibility = "";
        }
        // We have ended the class
        else if((consuming_class && lname == "class" && !class_in_class) || (consuming_class && lname == "struct" && !class_in_class)) {
            consuming_class = false;
            class_name_consumed = false;
        }
        // not sure here yet
        else if((consuming_class && lname == "class" && class_in_class) || (consuming_class && lname == "struct" && class_in_class)) {
            // we have consumed the top of the stack class completely
            class_names_in_class_stack.pop_back();
            if(class_names_in_class_stack.empty()) {
                class_in_class = false;
            }
        }
        else if(consuming_class && lname == "super") {
            in_inheritance_list = false;
        }
        else if(consuming_class && consuming_function && lname == "function") {
            consuming_function = false;
        }
        else if(lname == "type" && consuming_class && consuming_function && !in_function_param_list && !function_return_type_consumed) {
            recordFunctionReturnType();
        }
        else if(lname == "type" && consuming_class && consuming_function && in_function_param_list && consuming_data_declaration && function_return_type_consumed) {
            recordDataDeclarationType();
        }
        else if(lname == "name" && consuming_class && consuming_function && in_function_param_list && consuming_data_declaration && data_member_type_consumed) {
            end_of_decl = true;
            buildDataDeclarationNamesString();
            recordFullDataDeclaration();
        }
        else if(lname == "parameter_list" && consuming_class && consuming_function && in_function_param_list) {
            // build a FunctionDeclaration
            recordFunctionDeclaration();
        }
    }
    
    /**
     * charactersRoot
     * @param ch the characers
     * @param len number of characters
     *
     * SAX handler function for character handling at the root level.
     * Overide for desired behaviour.
     */
    virtual void charactersRoot(const char * ch, int len) {}

    /**
     * charactersUnit
     * @param ch the characers
     * @param len number of characters
     *
     * SAX handler function for character handling within a unit.
     * Overide for desired behaviour.
     */
    virtual void charactersUnit(const char * ch, int len) {
        std::string text_parsed(ch, len);
        current_recorded_data_in_class.append(text_parsed);
    }

    /**
     * metaTag
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes\
     *
     * SAX handler function for a meta tags.
     * Overide for desired behaviour.
     */
    virtual void metaTag(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {}

    /**
     * comment
     * @param value the comment content
     *
     * A comment has been parsed.
     * Overide for desired behaviour.
     */
    virtual void comment(const char * value) {}

    /**
     * cdataBlock
     * @param value the pcdata content
     * @param len the block length
     *
     * Called when a pcdata block has been parsed.
     * Overide for desired behaviour.
     */
    virtual void cdataBlock(const char * value, int len) {}

    /**
     * processingInstruction
     * @param target the processing instruction target.
     * @param data the processing instruction data.
     *
     * Called when a processing instruction has been parsed.
     * Overide for desired behaviour.
     */
    virtual void processingInstruction(const char * target, const char * data) {}

    void recordClassName() {
        
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "std::");
        current_recorded_data_in_class = replaceCommas(current_recorded_data_in_class);
        
        if(!class_in_class) {
            current_class = current_recorded_data_in_class;
            classes_in_source[current_class];
            class_name_consumed = true;
        }
        else if(class_in_class)
        {
            // add the name to the stack so we can modify it, then add the key to the map
            class_names_in_class_stack.push_back(current_recorded_data_in_class);
            classes_in_source[current_class].classes_in_class[current_recorded_data_in_class];
            class_in_class_name_consumed = true;
        }

    }
    
    void recordDataDeclarationType() {
        
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "std::");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "*");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, " ");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "const");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "struct");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "virtual");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "&");
        current_recorded_data_in_class = replaceCommas(current_recorded_data_in_class);
        
        
        consuming_data_declaration_type = current_recorded_data_in_class;
        current_recorded_data_in_class = "";
        
        data_member_type_consumed = true;

    }
    
    void buildDataDeclarationNamesString() {
        current_data_declaration_names = current_recorded_data_in_class;
        
    }

    void recordFullDataDeclaration() {
        if(end_of_decl) {
            end_of_decl = false;
            consuming_data_declaration = false;
            data_member_type_consumed = false;
        }

        current_data_declaration_names = current_data_declaration_names.substr(0, current_data_declaration_names.length());
        
        
        current_data_declaration_names = removePortionOfString(current_data_declaration_names, "std::");
        current_data_declaration_names = replaceCommas(current_data_declaration_names);
        current_data_declaration_names = removePortionOfString(current_data_declaration_names, ";");
        current_data_declaration_names = removePortionOfString(current_data_declaration_names, "*");
        current_data_declaration_names = removePortionOfString(current_data_declaration_names, " ");
        current_data_declaration_names = removePortionOfString(current_data_declaration_names, "struct");
        current_data_declaration_names = removePortionOfString(current_data_declaration_names, "const");
        current_data_declaration_names = removePortionOfString(current_data_declaration_names, "virtual");
        current_data_declaration_names = removePortionOfString(current_data_declaration_names, "&");
        
        struct AttributeDeclaration temp(consuming_data_declaration_type, current_data_declaration_names);
        resolveMultiplicity(temp);
        if(consuming_function) {
            function_parameters.push_back(temp);

            current_data_declaration_names = "";
            return;
        }
        if(!class_in_class) {
            if(in_public || in_private || in_protected) {
                classes_in_source[current_class].class_data_members[current_class_visibility].push_back(temp);
                // attribute has been fully consumed
            }
            // Default the visibility to public
            else {
                classes_in_source[current_class].class_data_members["public"].push_back(temp);
                // attribute has been fully consumed
            }
        }
        else if(class_in_class) {
            // Class in class data member has been fully consumed
            classes_in_source[current_class].classes_in_class[*class_names_in_class_stack.end()].class_data_members[current_class_visibility].push_back(temp);
        }
        current_data_declaration_names = "";

    }
    
    void recordClassInheritanceList() {
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "std::");
        current_recorded_data_in_class = replaceCommas(current_recorded_data_in_class);
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "virtual");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "public");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "private");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "protected");
        
        if(!class_in_class) {
            classes_in_source[current_class].inheritance_list.push_back(current_recorded_data_in_class);
        }
        else if(class_in_class) {
            classes_in_source[current_class].classes_in_class[*class_names_in_class_stack.end()].inheritance_list.push_back(current_recorded_data_in_class);
        }
    }
    
    void recordFunctionReturnType() {
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "std::");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "*");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, " ");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "const");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "struct");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "virtual");
        current_recorded_data_in_class = removePortionOfString(current_recorded_data_in_class, "&");
        current_function_return_type = current_recorded_data_in_class;
        current_recorded_data_in_class = "";
        function_return_type_consumed = true;
    }
    
    void recordFunctionDeclaration() {
        struct FunctionDeclaration temp(current_function_return_type, current_function_name, function_parameters);
        
        if(!class_in_class) {
            if(in_public || in_private || in_protected) {
                classes_in_source[current_class].class_functions[current_class_visibility].push_back(temp);
            }
            // in-case we are in a struct default the visibility option to public
            else {
                classes_in_source[current_class].class_functions["public"].push_back(temp);
            }
        }
        else if(class_in_class) {
            if( in_public || in_private || in_protected) {
                classes_in_source[current_class].classes_in_class[*class_names_in_class_stack.end()].class_functions[current_class_visibility].push_back(temp);
            }
            // in-case we are in a struct, once again default the visibility option to public
            else {
                
                classes_in_source[current_class].classes_in_class[*class_names_in_class_stack.end()].class_functions["public"].push_back(temp);
            }
        }
        current_recorded_data_in_class = "";
        current_function_return_type = "";
        consuming_function = false;
        in_function_param_list = false;
        function_return_type_consumed = false;
        function_parameters.clear();
    }
    
    void resolveRelationships() {
        // Classes <name, class> -> class : { attributes : inheritance }
        // name is important, if attribute type == Classes key name we have a relationship
        for(const auto& class_itr : classes_in_source) {
            for(const auto& visibility_itr : class_itr.second.class_data_members) {
                for(const auto& attribute_itr : visibility_itr.second) {
                    // We have found a has-a relationship
                    if(classes_in_source.find(attribute_itr.type) != classes_in_source.end()) {
                        yuml_relationships += "[" + class_itr.first + "]" + "->" + "[" + attribute_itr.type + "]\n";
                    }
                }
                for(const auto& class_in_class_itr : class_itr.second.classes_in_class) {
                    for(const auto& class_in_class_visibility_itr : class_in_class_itr.second.class_data_members) {
                        for(const auto& class_in_class_attr_itr : class_in_class_visibility_itr.second)
                            // We have a relationship
                            if(classes_in_source.find(class_in_class_attr_itr.type) != classes_in_source.end()) {
                                yuml_relationships += "[" + class_itr.first + "]" + "->" + "[" + class_in_class_attr_itr.type + "]\n";
                            }
                    }
                }
            }
        }
    }

    
#pragma GCC diagnostic pop

};

// Used to remove a substring from a string
std::string removePortionOfString(std::string stringToFix, std::string sequenceToRemove) {
    if(stringToFix == "" || sequenceToRemove == "")
    {
        return "";
    }
    size_t pos;
    size_t lengthOfSequence = sequenceToRemove.length();
    pos = stringToFix.find(sequenceToRemove);
    while(pos != std::string::npos) {
        stringToFix = stringToFix.substr(0, pos) + stringToFix.substr(pos + lengthOfSequence, stringToFix.length());
        pos = stringToFix.find(sequenceToRemove);
    }
    
    
    return stringToFix;
    
}
// All commas contained in a string get replaced with a different type of comma
// So that they can be used in yuml
std::string replaceCommas(std::string stringToFix) {
    if(stringToFix == "")
        return "";
    
    size_t pos;
    pos = stringToFix.find(",");
    while(pos != std::string::npos) {
        stringToFix = stringToFix.substr(0, pos) + "،" + stringToFix.substr(pos + 1, stringToFix.length());
        pos = stringToFix.find(",");
    }
    return stringToFix;
}

void resolveMultiplicity(struct AttributeDeclaration& rhs) {
    // Make sure the type is a container of some sort
    if(rhs.type.find("<") != std::string::npos) {
        size_t begin_pos = rhs.type.find_last_of("<");
        std::string new_type = rhs.type.substr(begin_pos, rhs.type.length());
        size_t end_pos = new_type.find_first_of("،");
        if(end_pos == std::string::npos) {
            end_pos = new_type.find_first_of(">");
        }
        else {
            size_t find_last_comma = new_type.find_last_of("،");
            new_type = new_type.substr(find_last_comma, new_type.length());
            
            
            new_type = new_type.substr(0, new_type.length());
            end_pos = new_type.find_first_of(">");
        }
        new_type = new_type.substr(0, new_type.length());
        rhs.type = new_type.substr(1, end_pos -1);
        rhs.multiplicity = "［*］";
    }
    else if(rhs.type.find("[") != std::string::npos) {
        size_t begin_pos = rhs.type.find_last_of("[");
        rhs.type = rhs.type.substr(0, begin_pos - 1);
        rhs.multiplicity = "［*］";
        
    }
    else if(rhs.type.find("*") != std::string::npos) {
        size_t begin_pos = rhs.type.find_last_of("*");
        rhs.type = rhs.type.substr(0, begin_pos - 1);
        rhs.multiplicity = "［*］";
    }
}

#endif
