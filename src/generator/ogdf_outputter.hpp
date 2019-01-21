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
#ifndef INCLUDED_OGDF_OUTPUTTER_HPP
#define INCLUDED_OGDF_OUTPUTTER_HPP

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
//===================================================================

//Input_Output_Include===============================================
#include <ogdf/fileformats/GraphIO.h>
#include "svg_printer.hpp"
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

class ogdf_outputter : srcuml_outputter{

public:

	ogdf_outputter(){
		ga.init(g,
		GraphAttributes::nodeGraphics |
		GraphAttributes::edgeGraphics |
		GraphAttributes::nodeLabel |
		GraphAttributes::edgeLabel |
		GraphAttributes::nodeStyle |
		GraphAttributes::edgeStyle |
		GraphAttributes::nodeTemplate);
	};

	bool output(std::ostream& out, std::vector<std::shared_ptr<srcuml_class>> & classes){
		//transfer information from srcUML to OGDF
		srcuml_relationships relationships = analyze_relationships(classes);
		std::map<std::string, ogdf::node> class_node_map;

		//Classes/Nodes
		for(const std::shared_ptr<srcuml_class> & aclass : classes){
			node cur_node = g.newNode();
			//Insert into map the node class pairing
			class_node_map.insert(std::pair<std::string, ogdf::node>(aclass->get_name(), cur_node));

			std::string& label = ga.label(cur_node);
			int num_lines = 0;
			int longest_line = 0;
			label = generate_label(aclass, num_lines, longest_line);

			double& h = ga.height(cur_node);
			h = num_lines * 1.3 * 10;//num_lines * 50;

			double& w = ga.width(cur_node);
			w = longest_line * .75 * 10;//longest_line * 10;

			Color& color = ga.fillColor(cur_node);
			color = Color(Color::Name::Antiquewhite);
		}

		//Relationships/Edges
		for(const srcuml_relationship relationship : relationships.get_relationships()){
			//get the nodes from graph g, create edge and add apropriate info.
			ogdf::node lhs, rhs;

			std::map<std::string, ogdf::node>::iterator src_it = class_node_map.find(relationship.get_source());
			if(src_it != class_node_map.end()){
				lhs = src_it->second;
			}

			std::map<std::string, ogdf::node>::iterator dest_it = class_node_map.find(relationship.get_destination());
			if(dest_it != class_node_map.end()){
				rhs = dest_it->second;
			}

			ogdf::edge cur_edge = g.newEdge(lhs, rhs);//need to pass to ogdf::node types


			float &w = ga.strokeWidth(cur_edge);
			w = 2;

			StrokeType &st = ga.strokeType(cur_edge);

			const relationship_type r_type = relationship.get_type();
			switch(r_type){
				case DEPENDENCY:
					st = StrokeType::Dash;
					break;
				case ASSOCIATION:
					st = StrokeType::Dash;
					break;
				case BIDIRECTIONAL:
					st = StrokeType::Dash;
					break;
				case AGGREGATION:
					st = StrokeType::Dash;
					break;
				case COMPOSITION:
					st = StrokeType::Dash;
					break;
				case GENERALIZATION:
					st = StrokeType::Dash;
					break;
				case REALIZATION:
					st = StrokeType::Dash;
					break;
			}
		}

		/*
			for(ogdf::edge * e = g.firstEdge(); e; e = e->succ()){
			
			}


			for(ogdf::node * n = g.firstNode(); n; n = n->succ()){

			}
		*/

		SugiyamaLayout sl;
		sl.setRanking(new OptimalRanking);
		sl.setCrossMin(new MedianHeuristic);
 
		OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
		ohl->layerDistance(120.0);
		ohl->nodeDistance(200.0);
		ohl->weightBalancing(1);
		sl.setLayout(ohl);

		sl.call(ga);

		GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
		if(!drawSVG(ga, out, *svg_settings)){
			std::cout << "Error Write" << std::endl;
		}


		return true;
	}

	std::string generate_label(const std::shared_ptr<srcuml_class> & aclass, int &num_lines, int &longest_line){
		std::string label;
		if(aclass->get_srcuml_name() != aclass->get_name()){
			label += aclass->get_srcuml_name() + "<svg_new_line>";
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
				if(op.get_stereotypes().count("set") > 0){continue;}
				if(op.get_stereotypes().count("get") > 0){continue;}
				
				label += op.get_string_function();
				label += "<svg_new_line>";
				++num_lines;
				if(op.get_string_function().length() > longest_line){longest_line = op.get_string_function().length();}
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

private:

	Graph g;

	GraphAttributes ga;
	
};

#endif
