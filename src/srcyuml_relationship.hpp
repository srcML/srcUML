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

class srcyuml_relationship {


};

class srcyuml_relationships {

private:

    std::vector<std::shared_ptr<srcyuml_class>> & classes;


public:
    srcyuml_relationships(std::vector<std::shared_ptr<srcyuml_class>> & classes)
        : classes(classes) {

            analyze_classes();

        }

    ~srcyuml_relationships() {}

    friend std::ostream & operator<<(std::ostream & out, const srcyuml_relationships & relationships) {

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
 
    }

};

 #endif