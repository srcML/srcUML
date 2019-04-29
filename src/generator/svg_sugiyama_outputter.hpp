#ifndef INCLUDED_SVG_SUGIYAMA_OUTPUTTER_HPP
#define INCLUDED_SVG_SUGIYAMA_OUTPUTTER_HPP


#include <svg_outputter.hpp>

class svg_sugiyama_outputter : public svg_outputter {

public:

	svg_sugiyama_outputter(){
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
	}

	bool output(std::ostream& out, std::vector<std::shared_ptr<srcuml_class>> & classes){
		//transfer information from srcUML to ogdf
		srcuml_relationships relationships = analyze_relationships(classes);
		std::map<std::string, node> class_node_map;

		//Classes/Nodes
		//===============================================================================================================
		for(const std::shared_ptr<srcuml_class> & aclass : classes){
			node cur_node = g.newNode();
			//Insert into map the node class pairing
			class_node_map.insert(std::pair<std::string, node>(aclass->get_srcuml_name(), cur_node));

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


		 

		//Relationships/Edges
		//===============================================================================================================
		std::multimap<std::pair<node, node>, relationship_type> edge_type_map;
		std::map<std::pair<node, edge>, std::string> ne_arrow;

		for(const srcuml_relationship relationship : relationships.get_relationships()){
			//get the nodes from graph g, create edge and add appropriate info.
			node lhs, rhs;

			std::map<std::string, node>::iterator src_it = class_node_map.find(relationship.get_source());
			if(src_it != class_node_map.end()){
				lhs = src_it->second;
			}

			std::map<std::string, node>::iterator dest_it = class_node_map.find(relationship.get_destination());
			if(dest_it != class_node_map.end()){
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

			ga.strokeWidth(cur_edge) = 2;

			StrokeType &st = ga.strokeType(cur_edge);
			EdgeArrow &ea = ga.arrowType(cur_edge);
			Graph::EdgeType &et = ga.type(cur_edge);

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
		//===============================================================================================================

		//Layout
		//===============================================================================================================

		SugiyamaLayout sl;
		sl.setRanking(new OptimalRanking);
		sl.setCrossMin(new MedianHeuristic);
 
		OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
		ohl->layerDistance(50.0);
		ohl->nodeDistance(50.0);
		ohl->weightBalancing(1);
		sl.setLayout(ohl);

		sl.call(ga);

		GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
		if(!drawSVG(ga, out, *svg_settings, ne_arrow)){
			std::cout << "Error Write" << std::endl;
		}

		//===============================================================================================================


		return true;
	}

private:

	Graph g;

	GraphAttributes ga;

	ClusterGraph cg;

	ClusterGraphAttributes cga;

};

#endif