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
#include <map>
#include <string>
#include <list>
#include <iostream>

struct attributeDeclaration {
    std::string type;
    std::string name;
    attributeDeclaration(std::string t, std::string n) : type(t), name(n) {};
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

    
    // Keys can be public, private, or protected
    std::map<std::string, std::list<struct attributeDeclaration> > class_data_members_;
    std::map<std::string, std::list<std::string> > class_functions_;
    
    srcYUMLClass()
    {}
    
    void printData()
    {
        for(const auto& itr : class_data_members_)
        {
            std::cout << itr.first << "\n";
        }
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
    std::map<std::string, srcYUMLClass> classes_in_source_;
    
    // bool variables to determine program states
    bool consuming_class_,
         consuming_data_member_,
         consuming_function_,
         in_public_,
         in_private_,
         in_protected_,
         in_inheritance_list_,
         class_name_consumed_;
    
    
    
    // This could be a function or an attribute
    std::string current_recorded_data_in_class_;
    std::string current_data_member_type_;
    /*
     This is to be used to store class name until we hit the end tag
     so that we know what class key to map the data to.
     */
    std::string current_class_;
    
    /* 
       This holds what visibility layer we are in for the class
       so that we can properly map where we got data from in our class
     */
    std::string current_class_visibility_;
protected:

public:

    /**
     * srcYUMLHandler
     *
     * Default constructor default values to everything
     */
    srcYUMLHandler() : consuming_class_(false), consuming_data_member_(false), consuming_function_(false), in_public_(false), in_private_(false), in_protected_(false), in_inheritance_list_(false), class_name_consumed_(false) {;};


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
#if 0
    /**
     * startFunction
     * @param name the function's name
     * @param return_type the function return type
     * @param parameter_list a list of the function parameters in struct containing (declaration.type/declaration.name)
     * @param is_decl indicates if the call is a function declaration (true) or definition (false)
     *
     * SAX handler function for start of function with prototype.
     * Accessing references after callback termination is undefined.
     *
     * Overide for desired behaviour.
     */
    virtual void startFunction(const std::string & name, const std::string & return_type, const std::vector<declaration> & parameter_list, bool is_decl)
    {
        
    }
#endif
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
                                const struct srcsax_attribute * attributes)
    {
        std::string lname = localname;
        // We have started reading a class
        if(lname == "class")
        {
            consuming_class_ = true;
        }
        // We are now in the public part of the class
        else if(lname == "public" && consuming_class_)
        {
            in_public_ = true;
            current_class_visibility_ = "public";
        }
        // We are now in the private part of the class
        else if(lname == "private" && consuming_class_)
        {
            in_private_ = true;
            current_class_visibility_ = "private";
        }
        // We are now in the protected part of the class
        else if(lname == "protected" && consuming_class_)
        {
            in_protected_ = true;
            current_class_visibility_ = "protected";
        }
        // We are at a class attribute
        else if(lname == "decl_stmt" && consuming_class_)
        {
            consuming_data_member_ = true;
            current_recorded_data_in_class_ = "";
        }
        // We are at one of the classes functions
        else if(lname == "function" && consuming_class_)
        {
            consuming_function_ = true;
            current_recorded_data_in_class_ = "";
        }
        // if we hit the block of a function and its parent is a function we have consumed all needed data to record
        else if(lname == "block" && consuming_function_ && srcml_element_stack[srcml_element_stack.size() - 2] == "function")
        {
            classes_in_source_[current_class_].class_functions_[current_class_visibility_].push_back(current_recorded_data_in_class_);
            current_recorded_data_in_class_ = "";
            consuming_function_ = false;
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
#if 0
    /**
     * endFunction
     *
     * SAX handler function for end of a function.
     * Overide for desired behaviour.
     */
    virtual void endFunction() {}
#endif
    /**
     * endElement
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * SAX handler function for end of an element.
     * Overide for desired behaviour.
     */
    virtual void endElement(const char * localname, const char * prefix, const char * URI)
    {
        std::string lname = localname;
        // If we hit the </name> tag and the parent is the Class tag we have consumed the class' name
        if(consuming_class_ && lname == "name" && srcml_element_stack[srcml_element_stack.size() - 2] == "class" && !class_name_consumed_)
        {
            classes_in_source_[current_class_];
            class_name_consumed_ = true;
        }
        // If we hit this we have consumed the WHOLE type, even nested types
        else if(consuming_class_ && consuming_data_member_ && lname == "type" && srcml_element_stack[srcml_element_stack.size() - 2] == "decl")
        {
            current_data_member_type_ = current_recorded_data_in_class_;
            current_recorded_data_in_class_ = "";
        }
        // We hit an </decl_stmt> tag in the class so we now have all of the declaration information
        else if(consuming_class_ && lname == "decl_stmt" && consuming_data_member_)
        {
            struct attributeDeclaration temp(current_data_member_type_, current_recorded_data_in_class_);
            
            classes_in_source_[current_class_].class_data_members_[current_class_visibility_].push_back(temp);
            // attribute has been fully consumed
            consuming_data_member_ = false;
        }
        // We are no longer in public visibility
        else if(consuming_class_ && lname == "public")
        {
            in_public_ = false;
        }
        // We are no longer in private visibility
        else if(consuming_class_ && lname == "private")
        {
            in_private_ = false;
        }
        // We are no longer in protected visibility
        else if(consuming_class_ && lname == "protected")
        {
            in_protected_ = false;
        }
        else
        {
            // Do nothing
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
    virtual void charactersUnit(const char * ch, int len)
    {
        if(consuming_class_)
        {
            current_recorded_data_in_class_.append(ch, len);
        }
        
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

#pragma GCC diagnostic pop

};

#endif
