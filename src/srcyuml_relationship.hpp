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

enum relationship_type { DEPENDENCY, ASSOCIATION, AGGREGATION, COMPOSITION, GENERALIZATION, REALIZATION };
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

        switch(relationship.type) {

            case GENERALIZATION: {
                out << "^-";
                break;
            }
            case REALIZATION: {
                out << "^-.-";
                break;
            }

        }

        out << '[' << relationship.from << "]\n";

        return out;

    }

};

class srcyuml_relationships {

private:

    std::vector<std::shared_ptr<srcyuml_class>> & classes;
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

        resolve_inheritence();

    }

    void resolve_inheritence_inner(std::map<std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>> & class_map, std::pair<std::shared_ptr<srcyuml_class>, bool> & class_pair) {

        for(const ClassPolicy::ParentData & parent_data : class_pair.first->get_data().parents) {

            std::map<std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>>::iterator parent = class_map.find(parent_data.name);

            if(parent != class_map.end()) {

                if(!parent->second.second)
                    resolve_inheritence_inner(class_map, parent->second);

                /** @todo should this be abstract instead? */
                if(!parent->second.second) {
                    class_pair.first->set_type(srcyuml_class::NONE);
                    break;
                }

            } else {
                /** @todo not sure what default shold be */
            }
                
        }

        class_pair.second = true;

    }


    void resolve_inheritence() {
        std::map<std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>> class_map;

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {
            class_map[aclass->get_name()] = std::make_pair(aclass, false);
        }

        for(std::pair<const std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>> & map_pair : class_map) {
            resolve_inheritence_inner(class_map, map_pair.second);
        }

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {

            for(const ClassPolicy::ParentData & parent_data : aclass->get_data().parents) {

                std::map<std::string, std::pair<std::shared_ptr<srcyuml_class>, bool>>::iterator parent = class_map.find(parent_data.name);
                relationship_type type = GENERALIZATION;
                if(aclass->get_type() == srcyuml_class::INTERFACE
                    || (parent != class_map.end() && parent->second.first->get_type() == srcyuml_class::INTERFACE)) {

                    type = REALIZATION;

                }

                if(parent != class_map.end())
                    relationships.emplace_back(parent->second.first->get_srcyuml_name(), aclass->get_srcyuml_name(), type);
                else
                    relationships.emplace_back(parent_data.name, aclass->get_srcyuml_name(), type);

            }

        }
 
    }

};

 #endif