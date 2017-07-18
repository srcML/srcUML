/**
 * @file srcyuml_relationship.hpp
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

#ifndef INCLUDED_SRCYUML_RELATIONSHIP_HPP
#define INCLUDED_SRCYUML_RELATIONSHIP_HPP

#include <srcyuml_class.hpp>

#include <map>
#include <set>

enum relationship_type { DEPENDENCY, ASSOCIATION, BIDIRECTIONAL, AGGREGATION, COMPOSITION, GENERALIZATION, REALIZATION };
struct srcyuml_relationship {

    srcyuml_relationship(const std::string & source,
                         const std::string & source_label,
                         const std::string & destination,
                         const std::string & destination_label,
                         relationship_type type)
        : source(source),
          source_label(source_label),
          destination(destination),
          destination_label(destination_label),
          type(type) {}

    srcyuml_relationship(const std::string & source,
                         const std::string & destination,
                         relationship_type type)
        : srcyuml_relationship(source, "", destination, "", type) {}

    std::string source;
    std::string source_label;
    std::string destination;
    std::string destination_label;

    relationship_type type;

    friend std::ostream & operator<<(std::ostream & out, const srcyuml_relationship & relationship) {

        out << '[' << relationship.source << ']';

        if(relationship.type == BIDIRECTIONAL)
            out << '<';

        out << relationship.source_label;

        switch(relationship.type) {

            case DEPENDENCY: {
                out << "-.-";
                break;
            }
            case ASSOCIATION:
            case BIDIRECTIONAL: {
                out << '-';
                break;
            }
            case AGGREGATION: {
                out << "<>-";
                break;
            }
            case COMPOSITION: {
                out << "++-";
                break;
            }
            case GENERALIZATION: {
                out << "^-";
                break;
            }
            case REALIZATION: {
                out << "^-.-";
                break;
            }

        }

        out << relationship.destination_label;

        if(relationship.type != GENERALIZATION && relationship.type != REALIZATION)
            out << '>';

        out << '[' << relationship.destination << "]\n";

        return out;

    }

};

class srcyuml_relationships {

private:

    std::vector<std::shared_ptr<srcyuml_class>> & classes;

    std::map<std::string, std::shared_ptr<srcyuml_class>> class_map;

    std::vector<srcyuml_relationship> relationships;

public:
    srcyuml_relationships(std::vector<std::shared_ptr<srcyuml_class>> & classes)
        : classes(classes) {
            analyze_classes();
    }

    ~srcyuml_relationships() {}

    friend std::ostream & operator<<(std::ostream & out, const srcyuml_relationships & relationships) {

        for(const srcyuml_relationship relationship : relationships.relationships) {
            out << relationship;
        }

        return out;

    }

private:

    void analyze_classes() {

        generate_class_map();
        resolve_inheritence();
        generate_attribute_relationships();
        generate_dependency_relationships();

    }

    void add_relationship(const srcyuml_relationship & relationship) {
        relationships.emplace_back(relationship);
    }

    void generate_class_map() {

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {
            class_map[aclass->get_name()] = aclass;
        } 

    }

    void resolve_inheritence_inner(std::shared_ptr<srcyuml_class> & aclass) {

        bool has_found_parents = false;
        for(const ClassPolicy::ParentData & parent_data : aclass->get_data().parents) {

            std::map<std::string, std::shared_ptr<srcyuml_class>>::iterator parent = class_map.find(parent_data.name);

            if(parent != class_map.end()) {

                has_found_parents = true;

                if(!parent->second->get_is_finalized())
                    resolve_inheritence_inner(parent->second);

                if(!parent->second->get_is_interface()) {
                    aclass->set_is_interface(false);
                }

                // add pure virtual from parents
                for(const std::pair<std::string, const FunctionPolicy::FunctionData *> & function_pair
                        : parent->second->get_pure_virtual_functions_map()) {

                    std::map<std::string, const FunctionPolicy::FunctionData *>::const_iterator implemented_function
                        = aclass->get_implemented_functions_map().find(function_pair.first);
                    if(implemented_function == aclass->get_implemented_functions_map().end())
                        aclass->get_pure_virtual_functions_map()[function_pair.first] = function_pair.second;

                }

            } else {
                /** @todo not sure what default shold be */
            }
                
        }

        aclass->set_is_abstract(!aclass->get_pure_virtual_functions_map().empty());
        if(!has_found_parents
            && aclass->get_implemented_functions_map().empty()
            && aclass->get_pure_virtual_functions_map().empty())
            aclass->set_is_interface(false);

        // check if pure virtual are overriden
        aclass->set_is_finalized(true);

    }

    void resolve_inheritence() {

        for(std::pair<const std::string, std::shared_ptr<srcyuml_class>> & map_pair : class_map) {
            resolve_inheritence_inner(map_pair.second);
        }

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {

            for(const ClassPolicy::ParentData & parent_data : aclass->get_data().parents) {

                std::map<std::string, std::shared_ptr<srcyuml_class>>::iterator parent = class_map.find(parent_data.name);

                /** @todo should I show these? */
                if(parent == class_map.end()) continue;

                relationship_type type = GENERALIZATION;
                if(!aclass->get_is_abstract() && parent->second->get_is_abstract()) {
                    type = REALIZATION;
                }

                srcyuml_relationship relationship(parent->second->get_srcyuml_name(), aclass->get_srcyuml_name(), type);
                add_relationship(relationship);

            }

        }
 
    }

    void generate_attribute_relationships() {

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {

            /** @todo may want set so same type not added twice */

            for(const srcyuml_attribute & attribute : aclass->get_attributes()) {

                std::map<std::string, std::shared_ptr<srcyuml_class>>::iterator parent = class_map.find(attribute.get_type().get_type_name());
                if(parent == class_map.end()) continue;

                relationship_type type = ASSOCIATION;
                if(attribute.get_type().get_is_composite())
                    type = COMPOSITION;
                else if(attribute.get_type().get_is_aggregate())
                    type = AGGREGATION;

                std::string relationship_label = attribute.get_name() + attribute.get_multiplicity();
                srcyuml_relationship relationship(aclass->get_srcyuml_name(), "", parent->second->get_srcyuml_name(), relationship_label, type);
                add_relationship(relationship);

            }

        }

    }

    void generate_dependency_relationships(){//dependency is local variables or parameters
        for(const std::shared_ptr<srcyuml_class>& aclass : classes){
            //create set of already add dependecies so no repeats
            std::set<std::string> catalogued_dependencies;
            //obtain current class type
            std::string current_class_type = aclass->get_srcyuml_name();
            catalogued_dependencies.insert(current_class_type);

            for(const std::pair<std::string, const FunctionPolicy::FunctionData *> func : aclass->get_implemented_functions_map()){
                //Parameter dependencies
                for(const ParamTypePolicy::ParamTypeData* aparam : func.second->parameters){
                    //iterate over parameters
                    //obtain param_type which is a nice string form of the type
                    //==================================================
                    srcyuml_type* temp = new srcyuml_type(aparam->type);
                    std::string param_type = temp->get_type_name();
                    //==================================================

                    std::map<std::string, std::shared_ptr<srcyuml_class>>::iterator related_class = class_map.find(param_type);
                    if(related_class == class_map.end()) continue;
                    std::string working_dep = related_class->second->get_srcyuml_name();
                    std::set<std::string>::iterator catalogued_class = catalogued_dependencies.find(working_dep);
                    
                    //remove last condition to re-add multi dependencies
                    if(related_class == class_map.end() || current_class_type == working_dep )// || catalogued_class != catalogued_dependencies.end())
                        continue;

                    srcyuml_relationship relationship(current_class_type, working_dep, DEPENDENCY);
                    catalogued_dependencies.insert(working_dep); //add param_type is std::string 
                    add_relationship(relationship);                   
                }
                //decleration dependencies   
                for(const DeclTypePolicy::DeclTypeData* arelation : func.second->relations){
                    srcyuml_type* temp = new srcyuml_type(arelation->type);
                    std::string relate_type = temp->get_type_name();

                    std::map<std::string, std::shared_ptr<srcyuml_class>>::iterator related_class = class_map.find(relate_type);
                    if(related_class == class_map.end()) continue;
                    std::string working_dep = related_class->second->get_srcyuml_name();//get heuristic version of dependency name
                    std::set<std::string>::iterator catalogued_class = catalogued_dependencies.find(working_dep);

                    //remove last condition to re-add multi dependencies
                    if(related_class == class_map.end() || current_class_type == working_dep )// || catalogued_class != catalogued_dependencies.end())
                        continue;

                    srcyuml_relationship relationship(current_class_type, working_dep, DEPENDENCY);
                    catalogued_dependencies.insert(working_dep);
                    add_relationship(relationship);
                }
                //Return type dependency
                srcyuml_type* temp = new srcyuml_type(func.second->returnType);
                std::string return_type = temp->get_type_name();

                std::map<std::string, std::shared_ptr<srcyuml_class>>::iterator related_class = class_map.find(return_type);
                if(related_class == class_map.end()) continue;
                std::string working_dep = related_class->second->get_srcyuml_name();
                std::set<std::string>::iterator catalogued_class = catalogued_dependencies.find(working_dep);

                //remove last condition to re-add multi dependencies
                if(related_class == class_map.end() || current_class_type == working_dep )// || catalogued_class != catalogued_dependencies.end())
                    continue;

                srcyuml_relationship relationship(current_class_type, working_dep, DEPENDENCY);
                catalogued_dependencies.insert(working_dep);
                add_relationship(relationship);
            } 
        }
    }

};

#endif
