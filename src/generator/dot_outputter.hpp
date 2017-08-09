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

#ifndef INCLUDED_DOT_OUTPUTTER_HPP
#define INCLUDED_DOT_OUTPUTTER_HPP

#include <abstract_srcuml_outputter.hpp>

class dot_outputter : abstract_srcuml_outputter {

public:

	dot_outputter(){};

	bool output(std::ostream & out, std::vector<std::shared_ptr<srcyuml_class>> & classes){

		srcyuml_relationships relations = analyze_relationships(classes);

		out << "digraph hierarchy {\nsize=\"5, 5\"\n";
        out << "node[shape=record,style=filled,fillcolor=gray95]\n";
        out << "edge[dir=\"both\", arrowtail=\"empty\", arrowhead=\"empty\", labeldistance=\"2.0\"]\n";

        std::map<std::string, int> class_number_map;

        int class_num = 0;

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {
        	class_number_map.insert(std::pair<std::string, int>(aclass->get_srcyuml_name(), class_num));

    		out << class_num << "[label = \"{ ";
    		out << aclass->get_srcyuml_name();
    		if(aclass->has_field || aclass->has_method)//private members
            	out << '|';

        	for(const srcyuml_attribute & attribute : aclass->attributes) {//private members
            	if(attribute.get_is_static()) {
                	static_outputter::output(out, attribute);
            	} else {
                	out << attribute;
            	}
            	out << "\\n";
        	}

        	if(aclass->has_method)//private members
            	out << '|';

        	for(std::size_t access = 0; access <= ClassPolicy::PROTECTED; ++access) {
            	for(const FunctionPolicy::FunctionData * function : aclass->data->methods[access]) { //private members
                	srcyuml_operation op(function, (ClassPolicy::AccessSpecifier)access);
                	if(op.get_stereotypes().count("set") > 0){continue;}
                	if(op.get_stereotypes().count("get") > 0){continue;}
                	if(function->isStatic) {
                    	static_outputter::output(out, op);
                	} else {
                    	out << op;
                	}
                	out << "\\n";
            	}
        	}

        	out << "}\"]\n";
            	class_num++;
        }

	}

};

#endif