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

#ifndef INCLUDED_SRCYUML_OUTPUT_HPP
#define INCLUDED_SRCYUML_OUTPUT_HPP

#include <srcyuml_class.hpp>
#include <srcyuml_relationship.hpp>

enum output_type {DOT, YUML, NONE};

class srcyuml_output{
private:

	output_type type = NONE;
	std::ostream * out;
	std::vector<std::shared_ptr<srcyuml_class>> classes;

public:

	srcyuml_output()                                  {}
	srcyuml_output(std::ostream & os)                 { out = &os; }
	srcyuml_output(std::ostream & os, std::string ot) { 
		if(ot == "dot" || ot == "DOT")
			type = DOT;
		else if(ot == "yuml" || ot == "YUML")
			type = YUML;
		out = &os; 
	}

	bool output_dot(){

		*out << "digraph hierarchy {\nsize=\"5, 5\"\n";
        *out << "node[shape=record,style=filled,fillcolor=gray95]\n";
        *out << "edge[dir=\"both\", arrowtail=\"empty\", arrowhead=\"empty\", labeldistance=\"2.0\"]\n";

        std::map<std::string, int> class_number_map;

        srcyuml_relationships relationships(classes);

        int class_num = 0;

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) {
            output_dot_aclass(*out, *aclass, class_number_map, class_num);
            class_num++;
        }

        output_dot_relations(*out, relationships, class_number_map);

        *out << '}';

        return true;

	}


	bool output_yuml(){

		srcyuml_relationships relationships(classes);

        for(const std::shared_ptr<srcyuml_class> & aclass : classes) 
            *out << *aclass;

		*out << relationships;

		return true;

	}


	bool output(std::vector<std::shared_ptr<srcyuml_class>> & clss){

		classes = clss; // inefficent, I know

		switch(type){
			case DOT:
				output_dot();
				break;

			case YUML:
				output_yuml();
				break;

			default:
				output_dot();
				break;

		}

		return true;

	}
};

#endif