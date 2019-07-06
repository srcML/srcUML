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

	void init_standard(std::vector<std::shared_ptr<srcuml_class>> &classes){

		ga.init(g,
		GraphAttributes::nodeGraphics |
		GraphAttributes::edgeGraphics |
		GraphAttributes::nodeLabel |
		GraphAttributes::edgeLabel |
		GraphAttributes::nodeType  |
		GraphAttributes::edgeType  |
		GraphAttributes::edgeArrow |
		GraphAttributes::nodeStyle |
		GraphAttributes::edgeStyle |
		GraphAttributes::nodeTemplate);

		srcuml_relationships relationships = analyze_relationships(classes);

		//Classes/Nodes
		//===============================================================================================================

		for(const std::shared_ptr<srcuml_class> & aclass : classes){
			node cur_node = g.newNode();
			//Insert into map the node class pairing
			class_name_node_map.insert(std::pair<std::string, node>(aclass->get_srcuml_name(), cur_node));
			class_node_map.insert(std::pair<std::shared_ptr<srcuml_class>, node>(aclass, cur_node));

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

		//===============================================================================================================

		std::cerr << "Here1\n";

		//Relationships/Edges
		//===============================================================================================================

		for(const srcuml_relationship relationship : relationships.get_relationships()){
			//get the nodes from graph g, create edge and add appropriate info.
			node lhs, rhs;

			std::map<std::string, node>::iterator src_it = class_name_node_map.find(relationship.get_source());
			if(src_it != class_name_node_map.end()){
				lhs = src_it->second;
			}

			std::map<std::string, node>::iterator dest_it = class_name_node_map.find(relationship.get_destination());
			if(dest_it != class_name_node_map.end()){
				rhs = dest_it->second;
			}

			//find possible occurnce of edge already
			auto itr = edge_type_map.find( std::pair<node, node>(lhs, rhs) );

			//If edge doesn't exist, add it
			if(itr == edge_type_map.end()){
				edge_type_map.insert(std::pair<std::pair<node,node>, 
					relationship_type> (std::pair<node,node>(lhs,rhs), relationship.get_type()) );
				edge_type_map.insert(std::pair<std::pair<node,node>, 
					relationship_type> (std::pair<node,node>(rhs,lhs), NONE_TYPE ) );

			//if edge does exist, check that the new relation isn't stronger
			}else{
				if(itr->second == ASSOCIATION && (relationship.get_type() == BIDIRECTIONAL 
											   || relationship.get_type() == AGGREGATION
											   || relationship.get_type() == COMPOSITION)){
					itr->second = relationship.get_type();
				}else if(itr->second == BIDIRECTIONAL && (relationship.get_type() == AGGREGATION
											   || relationship.get_type() == COMPOSITION)){
					itr->second = relationship.get_type();
				}else if(itr->second == AGGREGATION && (relationship.get_type() == COMPOSITION)){
					itr->second = relationship.get_type();
				}
			}	
		}

		std::cerr << "Here2\n";

		//after do a loop to add to graph
		for(auto edge : edge_type_map){
			std::cerr << "Here2.1\n";

			if(edge.second == NONE_TYPE)continue;
			//edge.first.first  = source 
			//edge.first.second = destination
			//edge.second       = relationship type

			ogdf::edge cur_edge = g.newEdge(edge.first.first, edge.first.second);

			ga.strokeWidth(cur_edge) = 2;
			std::cerr << "Here2.2\n";
			StrokeType &st = ga.strokeType(cur_edge);
			EdgeArrow &ea = ga.arrowType(cur_edge);
			Graph::EdgeType &et = ga.type(cur_edge);

			std::cerr << "Here2.3\n";

			const relationship_type r_type = edge.second;
			std::cerr << "Here2.4\n";
			switch(r_type){
			case DEPENDENCY:
				std::cerr << "DEPENDENCY\n";
				st = StrokeType::Dash;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::dependency;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "none"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "filled_arrow"));
				break;
			case ASSOCIATION:
				std::cerr << "ASSOCIATION\n";
				st = StrokeType::Solid;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::association;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "none"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "filled_arrow"));
				break;
			case BIDIRECTIONAL:
				std::cerr << "BIDIRECTIONAL\n";
				st = StrokeType::Solid;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::association;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "filled_arrow"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "filled_arrow"));
				break;
			case AGGREGATION:
				std::cerr << "AGGREGATION\n";
				st = StrokeType::Solid;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::association;
				std::cerr << "TACO\n";
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "hollow_diamond"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "none"));
				break;
			case COMPOSITION:
				std::cerr << "COMPOSITION\n";
				st = StrokeType::Solid;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::association;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "filled_diamond"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "none"));
				break;
			case GENERALIZATION:
				std::cerr << "GENERALIZATION\n";
				st = StrokeType::Dash;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::generalization;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "hollow_arrow"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "none"));
				break;
			case REALIZATION:
				std::cerr << "REALIZATION\n";
				st = StrokeType::Dash;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::generalization;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "hollow_arrow"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "none"));
				break;
			}
		}

		std::cerr << "Here3\n";
	}

	void init_clustered(std::vector<std::shared_ptr<srcuml_class>> &classes){

		cg.init(g);

		cga.init(cg,
		GraphAttributes::nodeGraphics |
		GraphAttributes::edgeGraphics |
		GraphAttributes::nodeLabel |
		GraphAttributes::edgeLabel |
		GraphAttributes::nodeType  |
		GraphAttributes::edgeType  |
		GraphAttributes::edgeArrow |
		GraphAttributes::nodeStyle |
		GraphAttributes::edgeStyle |
		GraphAttributes::nodeTemplate);

		srcuml_relationships relationships = analyze_relationships(classes);

		//Classes/Nodes
		//===============================================================================================================

		for(const std::shared_ptr<srcuml_class> & aclass : classes){
			node cur_node = g.newNode();
			//Insert into map the node class pairing
			class_name_node_map.insert(std::pair<std::string, node>(aclass->get_srcuml_name(), cur_node));
			class_node_map.insert(std::pair<std::shared_ptr<srcuml_class>, node>(aclass, cur_node));

			std::string& label = cga.label(cur_node);
			int num_lines = 0;
			int longest_line = 0;
			label = generate_label(aclass, num_lines, longest_line);

			double& h = cga.height(cur_node);
			h = num_lines * 1.3 * 10;//num_lines * 50;

			double& w = cga.width(cur_node);
			w = longest_line * .75 * 10;//longest_line * 10;
		
			Color& color = cga.fillColor(cur_node);
			color = Color(Color::Name::Antiquewhite);
		}

		//===============================================================================================================

		//Relationships/Edges
		//===============================================================================================================

		for(const srcuml_relationship relationship : relationships.get_relationships()){
			//get the nodes from graph g, create edge and add appropriate info.
			node lhs, rhs;

			std::map<std::string, node>::iterator src_it = class_name_node_map.find(relationship.get_source());
			if(src_it != class_name_node_map.end()){
				lhs = src_it->second;
			}

			std::map<std::string, node>::iterator dest_it = class_name_node_map.find(relationship.get_destination());
			if(dest_it != class_name_node_map.end()){
				rhs = dest_it->second;
			}

			//find possible occurnce of edge already
			auto itr = edge_type_map.find( std::pair<node, node>(lhs, rhs) );

			//If edge doesn't exist, add it
			if(itr == edge_type_map.end()){
				edge_type_map.insert(std::pair<std::pair<node,node>, 
					relationship_type> (std::pair<node,node>(lhs,rhs), relationship.get_type()) );
				edge_type_map.insert(std::pair<std::pair<node,node>, 
					relationship_type> (std::pair<node,node>(rhs,lhs), NONE_TYPE ) );

			//if edge does exist, check that the new relation isn't stronger
			}else{
				if(itr->second == ASSOCIATION && (relationship.get_type() == BIDIRECTIONAL 
											   || relationship.get_type() == AGGREGATION
											   || relationship.get_type() == COMPOSITION)){
					itr->second = relationship.get_type();
				}else if(itr->second == BIDIRECTIONAL && (relationship.get_type() == AGGREGATION
											   || relationship.get_type() == COMPOSITION)){
					itr->second = relationship.get_type();
				}else if(itr->second == AGGREGATION && (relationship.get_type() == COMPOSITION)){
					itr->second = relationship.get_type();
				}
			}	
		}

		//after do a loop to add to graph
		for(auto edge : edge_type_map){

			if(edge.second == NONE_TYPE)continue;
			//edge.first.first  = source 
			//edge.first.second = destination
			//edge.second       = relationship type

			ogdf::edge cur_edge = g.newEdge(edge.first.first, edge.first.second);

			cga.strokeWidth(cur_edge) = 2;

			StrokeType &st = cga.strokeType(cur_edge);
			EdgeArrow &ea = cga.arrowType(cur_edge);
			Graph::EdgeType &et = cga.type(cur_edge);

			const relationship_type r_type = edge.second;
			switch(r_type){
			case DEPENDENCY:
				st = StrokeType::Dash;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::dependency;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "none"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "filled_arrow"));
				break;
			case ASSOCIATION:
				st = StrokeType::Solid;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::association;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "none"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "filled_arrow"));
				break;
			case BIDIRECTIONAL:
				st = StrokeType::Solid;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::association;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "filled_arrow"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "filled_arrow"));
				break;
			case AGGREGATION:
				st = StrokeType::Solid;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::association;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "hollow_diamond"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "none"));
				break;
			case COMPOSITION:
				st = StrokeType::Solid;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::association;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "filled_diamond"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "none"));
				break;
			case GENERALIZATION:
				st = StrokeType::Dash;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::generalization;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "hollow_arrow"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "none"));
				break;
			case REALIZATION:
				st = StrokeType::Dash;
				ea = EdgeArrow::Both;
				et = Graph::EdgeType::generalization;
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.first, cur_edge), "hollow_arrow"));
				ne_arrow.insert(std::make_pair(std::make_pair(edge.first.second, cur_edge), "none"));
				break;
			}
		}
	}

	bool drawSVG(const GraphAttributes &A, const std::string &filename, const GraphIO::SVGSettings &settings, const std::map<std::pair<node, edge>, std::string> &ne_arrow){
		std::ofstream os(filename);
		return drawSVG(A, os, settings, ne_arrow);
	}

	bool drawSVG(const ClusterGraphAttributes &A, const std::string &filename, const GraphIO::SVGSettings &settings, const std::map<std::pair<node, edge>, std::string> &ne_arrow){
		std::ofstream os(filename);
		return drawSVG(A, os, settings, ne_arrow);
	}

	bool drawSVG(const GraphAttributes &attr, std::ostream &os, const GraphIO::SVGSettings &settings, const std::map<std::pair<node, edge>, std::string> &ne_arrow){
		SvgPrinter printer(attr, settings, ne_arrow);
		return printer.draw(os);
	}

	bool drawSVG(const ClusterGraphAttributes &attr, std::ostream &os, const GraphIO::SVGSettings &settings, const std::map<std::pair<node, edge>, std::string> &ne_arrow){
		SvgPrinter printer(attr, settings, ne_arrow);
		return printer.draw(os);
	}

protected:
	Graph g;

	GraphAttributes ga;

	ClusterGraph cg;

	ClusterGraphAttributes cga;

	std::map<std::shared_ptr<srcuml_class>, node>           class_node_map;
	std::map<std::string, node>                             class_name_node_map;
	std::multimap<std::pair<node, node>, relationship_type> edge_type_map;
	std::map<std::pair<node, edge>, std::string>            ne_arrow;
	
};

#endif
