/**
 * @file dot_outputter.hpp
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

#ifndef INCLUDED_DOT_OUTPUTTER_HPP
#define INCLUDED_DOT_OUTPUTTER_HPP

#include <srcuml_outputter.hpp>

class dot_outputter : srcuml_outputter {

public:

	bool output(std::ostream & out, std::vector<std::shared_ptr<srcuml_class>> & classes){

		srcuml_relationships relationships = analyze_relationships(classes);

		out << "digraph hierarchy {\n";//size=\"5, 5\"\n";
        out << "node[shape=record,style=filled,fillcolor=gray95]\n";
        out << "edge[dir=\"both\", arrowtail=\"empty\", arrowhead=\"empty\", labeldistance=\"2.0\"]\n";

        std::map<std::string, std::string> class_number_map;

        int class_num = 0;
        std::string class_word = "class";

        //Classes

        for(const std::shared_ptr<srcuml_class> & aclass : classes) {

            std::string class_wn = class_word + std::to_string(class_num);

        	class_number_map.insert(std::pair<std::string, std::string>(aclass->get_srcuml_name(), class_wn));

    		out << class_wn << "[label = \"{ ";
    		out << aclass->get_srcuml_name();
    		if(aclass->get_has_field() || aclass->get_has_method())//private members
            	out << '|';

        	for(const srcuml_attribute & attribute : aclass->get_attributes()) {//private members
            	if(attribute.get_is_static()) {
                	static_outputter::output(out, attribute);
            	} else {
                	out << attribute;
            	}
            	out << "\\n";
        	}

        	if(aclass->get_has_method())//private members
            	out << '|';

        	for(std::size_t access = 0; access <= ClassPolicy::PROTECTED; ++access) {
            	for(const FunctionPolicy::FunctionData * function : aclass->get_data().methods[access]) { //private members
                	srcuml_operation op(function, (ClassPolicy::AccessSpecifier)access);
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

        //Relations

        for(const srcuml_relationship relationship : relationships.get_relationships()) {
            
        	const std::map<std::string, std::string>::const_iterator current_class = class_number_map.find(relationship.get_source());
        	out << current_class->second << "->";
        	const std::map<std::string, std::string>::const_iterator second_class = class_number_map.find(relationship.get_destination());
        	out << second_class->second;

        	switch(relationship.type) {

            	case DEPENDENCY: {
                	out << "[arrowhead=\"vee\", arrowtail=\"none\", style=\"dashed\"]\n";
                	break;
            	}
            	case ASSOCIATION:
            	case BIDIRECTIONAL: {
                	out << "[arrowhead=\"none\"]\n"; //currently same as generalization
                	break;
	            }
    	        case AGGREGATION: {
        	        out << "[arrowhead=\"none\", arrowtail=\"odiamond\"]\n";
            	    break;
            	}
            	case COMPOSITION: {
                	out << "[arrowhead=\"vee\", arrowtail=\"diamond\"]\n";
                	break;
            	}
            	case GENERALIZATION: {
                	out << "[arrowhead=\"none\"]\n";
                	break;
            	}
            	case REALIZATION: {
                	out << "[arrowhead=\"none\", style=\"dashed\"]\n";
                	break;
            	}

        	}

        }

        out << '}' << '\n';

	}

};

#endif