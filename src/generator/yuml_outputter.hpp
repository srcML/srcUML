/**
 * @file srcuml_relationship.hpp
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

#ifndef INCLUDED_YUML_OUTPUTTER_HPP
#define INCLUDED_YUML_OUTPUTTER_HPP

#include <srcuml_outputter.hpp>

class yuml_outputter : srcuml_outputter {

public:

	bool output(std::ostream & out, std::vector<std::shared_ptr<srcuml_class>> & classes){

        srcuml_relationships relationships = analyze_relationships(classes);

        //Classes

        for(const std::shared_ptr<srcuml_class> & aclass : classes){

            out << '[';

            out << aclass->get_srcuml_name();

            if(aclass->get_has_field() || aclass->get_has_method())
                out << '|';

            for(const srcuml_attribute & attribute : aclass->get_attributes()) {
                if(attribute.get_is_static()) {
                    static_outputter::output(out, attribute);
                } else {
                    out << attribute;
                }
                out << ';';
            }

            if(aclass->get_has_method())
                out << '|';

            for(std::size_t access = 0; access <= ClassPolicy::PROTECTED; ++access) {
                for(const FunctionPolicy::FunctionData * function : aclass->get_data().methods[access]) {
                    srcuml_operation op(function, (ClassPolicy::AccessSpecifier)access);
                    if(op.get_stereotypes().count("set") > 0){continue;}
                    if(op.get_stereotypes().count("get") > 0){continue;}
                    if(function->isStatic) {
                        static_outputter::output(out, op);
                    } else {
                        out << op;
                    }
                    out << ';';
                }
            }

            out << "]\n";
        }

        //Relations

        for(const srcuml_relationship relationship : relationships.get_relationships()) {
            out << '[' << relationship.get_source() << ']';

            if(relationship.type == BIDIRECTIONAL)
                out << '<';

            out << relationship.get_source_label();

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

            out << relationship.get_destination_label();

            if(relationship.type != GENERALIZATION && relationship.type != REALIZATION)
                out << '>';

            out << '[' << relationship.get_destination() << "]\n";
        }

	}

};

#endif