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

enum relationship_type { DEPENDENCY, ASSOCIATION, BIDIRECTIONAL, AGGREGATION, COMPOSITION, GENERALIZATION, REALIZATION };
struct srcyuml_relationship {

    srcyuml_relationship(const std::string & to, const std::string & from, relationship_type type)
        : to(to),
          from(from),
          type(type) {}

    std::string to;
    std::string from;

    relationship_type type;

    friend std::ostream & operator<<(std::ostream & out, const srcyuml_relationship & relationship) {

        out << '[' << relationship.to << ']';

        if(relationship.type == BIDIRECTIONAL)
            out << '<';

        switch(relationship.type) {

            case DEPENDENCY: {
                out << "-.=";
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

        if(relationship.type != GENERALIZATION && relationship.type != REALIZATION)
            out << '>';

        out << '[' << relationship.from << "]\n";

        return out;

    }

};

class srcyuml_relationships {

private:

    std::vector<std::shared_ptr<srcyuml_class>> & classes;

    std::map<std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>> class_map;

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
        generate_relationships();

    }

    void generate_class_map() {

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {
            class_map[aclass->get_name()] = std::make_pair(aclass, false);
        } 

    }

    void resolve_inheritence_inner(std::pair<std::shared_ptr<srcyuml_class>, bool> & class_pair) {

        if(!class_pair.first->get_is_interface()) {
            class_pair.second = true;
            return;
        }

        for(const ClassPolicy::ParentData & parent_data : class_pair.first->get_data().parents) {

            std::map<std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>>::iterator parent = class_map.find(parent_data.name);

            if(parent != class_map.end()) {

                if(!parent->second.second)
                    resolve_inheritence_inner(parent->second);

                /** @todo should this be abstract instead? */
                if(!parent->second.first->get_is_interface()) {
                    class_pair.first->set_is_interface(false);
                    break;
                }

            } else {
                /** @todo not sure what default shold be */
            }
                
        }

        class_pair.second = true;

    }

    void resolve_inheritence() {

        for(std::pair<const std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>> & map_pair : class_map) {
            resolve_inheritence_inner(map_pair.second);
        }

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {

            for(const ClassPolicy::ParentData & parent_data : aclass->get_data().parents) {

                std::map<std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>>::iterator parent = class_map.find(parent_data.name);

                /** @todo should I show these? */
                if(parent == class_map.end()) continue;

                relationship_type type = GENERALIZATION;
                // if(!aclass->get_is_abstract() && parent->second.first->get_is_abstract()) {
                //     type = REALIZATION;
                // }

                relationships.emplace_back(parent->second.first->get_srcyuml_name(), aclass->get_srcyuml_name(), type);

            }

        }
 
    }

    void generate_relationships() {

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {

            /** @todo may want set so same type not added twice */

            for(const srcyuml_attribute & attribute : aclass->get_attributes()) {

                std::map<std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>>::iterator parent = class_map.find(attribute.get_type().get_type_name());
                if(parent == class_map.end()) continue;

                relationship_type type = ASSOCIATION;
                if(attribute.get_type().get_is_composite())
                    type = COMPOSITION;
                else if(attribute.get_type().get_is_aggregate())
                    type = AGGREGATION;
                relationships.emplace_back(aclass->get_srcyuml_name(), parent->second.first->get_srcyuml_name(), type);

            }

        }

    }

};

 #endif