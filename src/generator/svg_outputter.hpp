/**
 * @file ogdf_outputter.hpp
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
#ifndef INCLUDED_SVG_OUTPUTTER_HPP
#define INCLUDED_SVG_OUTPUTTER_HPP

//srcUML_Requirements
#include <srcuml_class.hpp>
#include <srcuml_relationship.hpp>
#include <srcSAXEventDispatchUtilities.hpp>
#include <srcSAXController.hpp>
#include <srcuml_dispatcher.hpp>
#include <ClassPolicySingleEvent.hpp>
#include <srcuml_outputter.hpp>

//OGDF_Requirements
//Basic_Graph_Include================================================
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graphics.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
//===================================================================

//Cluster_Graph_Include==============================================
#include <ogdf/cluster/ClusterGraph.h>
#include <ogdf/cluster/ClusterGraphAttributes.h>
#include <ogdf/cluster/ClusterPlanarizationLayout.h>
//===================================================================

//Input_Output_Include===============================================
#include <ogdf/fileformats/GraphIO.h>
#include <svg_printer.hpp>
//===================================================================

//Layout_Include=====================================================
#include <ogdf/tree/TreeLayout.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/module/UMLLayoutModule.h>
//===================================================================

//Layered============================================================
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalHierarchyClusterLayout.h>
#include <ogdf/module/HierarchyClusterLayoutModule.h>
//===================================================================


using namespace ogdf;
using namespace ogdf::internal;

class svg_outputter : public srcuml_outputter{

public:

	virtual bool output(std::ostream& out, std::vector<std::shared_ptr<srcuml_class>> & classes) = 0;

	std::string generate_label(const std::shared_ptr<srcuml_class> & aclass, int &num_lines, int &longest_line){
		std::string label;
		if(aclass->get_srcuml_name() != aclass->get_name()){
			label += aclass->get_srcuml_name().substr(0, aclass->get_srcuml_name().find(aclass->get_name())) + "<svg_new_line>";
			++num_lines;
			if(aclass->get_srcuml_name().length() > longest_line){longest_line = aclass->get_srcuml_name().length();}
		}
		label += aclass->get_name();
		++num_lines;
		if(aclass->get_name().length() > longest_line){longest_line = aclass->get_name().length();}
		label += "<svg_box_divide>";
		++num_lines;
		for(const srcuml_attribute & attribute : aclass->get_attributes()){
			label += attribute.get_string_attribute() + "<svg_new_line>";
			++num_lines;
			if(attribute.get_string_attribute().length() > longest_line){longest_line = attribute.get_string_attribute().length();}
		}
		label += "<svg_box_divide>";
		++num_lines;


		for(std::size_t access = 0; access <= ClassPolicy::PROTECTED; ++access) {
			for(const FunctionPolicy::FunctionData * function : aclass->get_data().methods[access]) { //private members
				srcuml_operation op(function, (ClassPolicy::AccessSpecifier)access);

				std::string func = op.get_string_function();

				if(op.get_stereotypes().count("set") > 0){continue;}
				if(op.get_stereotypes().count("get") > 0){continue;}
				

				label += func;


				label += "<svg_new_line>";
				++num_lines;


				if(func.length() > longest_line){longest_line = func.length();}
			}
		}
		//create proper string such that SvgPrinter can parse.
		//\n will be <svg_new_line> box divider will be <svg_box_divide>
		return label;
	}

	bool drawSVG(const GraphAttributes &A, const std::string &filename, const GraphIO::SVGSettings &settings){
		std::ofstream os(filename);
		return drawSVG(A, os, settings);
	}

	bool drawSVG(const ClusterGraphAttributes &A, const std::string &filename, const GraphIO::SVGSettings &settings){
		std::ofstream os(filename);
		return drawSVG(A, os, settings);
	}

	bool drawSVG(const GraphAttributes &attr, std::ostream &os, const GraphIO::SVGSettings &settings){
		SvgPrinter printer(attr, settings);
		return printer.draw(os);
	}

	bool drawSVG(const ClusterGraphAttributes &attr, std::ostream &os, const GraphIO::SVGSettings &settings){
		SvgPrinter printer(attr, settings);
		return printer.draw(os);
	}
	
};

#endif
