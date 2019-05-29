#ifndef INCLUDED_SVG_THREE_OUTPUTTER_HPP
#define INCLUDED_SVG_THREE_OUTPUTTER_HPP

#include <ogdf/cluster/ClusterOrthoLayout.h>
#include <ogdf/cluster/ClusterPlanRep.h>
#include <svg_outputter.hpp>

class svg_three_outputter : public svg_outputter {

public:

	svg_three_outputter(){
		cg.init(g);

		cga.init(cg,
		GraphAttributes::nodeGraphics |
		GraphAttributes::edgeGraphics |
		GraphAttributes::nodeLabel |
		GraphAttributes::edgeLabel |
		GraphAttributes::nodeStyle |
		GraphAttributes::edgeStyle |
		GraphAttributes::nodeTemplate);
	}

	bool output(std::ostream& out, std::vector<std::shared_ptr<srcuml_class>> & classes){
		//transfer information from srcUML to ogdf

		srcuml_relationships relationships = analyze_relationships(classes);
		std::map<std::string, ogdf::node> class_node_map;

		SList<node> ctrl, bndr, enty;

		//Classes/Nodes
		//===============================================================================================================
		for(const std::shared_ptr<srcuml_class> & aclass : classes){
			node cur_node = g.newNode();
			//Insert into map the node class pairing
			class_node_map.insert(std::pair<std::string, ogdf::node>(aclass->get_srcuml_name(), cur_node));


			int num_lines = 0;
			int longest_line = 0;
			cga.label(cur_node) = generate_label(aclass, num_lines, longest_line);
			cga.height(cur_node) = num_lines * 1.3 * 10;//num_lines * 50;

			cga.width(cur_node) = longest_line * .75 * 10;
			//longest_line * 10;

			std::string stereo = "";
			if(aclass->get_stereotypes().begin() != aclass->get_stereotypes().end()){
				stereo = *(aclass->get_stereotypes().begin());
			}

			Color& color = cga.fillColor(cur_node);

			if(stereo == "control"){
				
				color = Color(224, 0, 0, 100);
				ctrl.pushBack(cur_node);

			}else if(stereo == "boundary"){

				color = Color(0, 224, 0, 100);
				bndr.pushBack(cur_node);

			}else if(stereo == "entity"){

				color = Color(0, 0, 224, 100);
				enty.pushBack(cur_node);

			}else if(stereo == ""){
				color = Color(130, 130, 130, 200);
			}
		}
		//===============================================================================================================



		//Relationships/Edges
		//===============================================================================================================
		//std::multimap<std::string, std::string> edge_map;
		//std::map<edge, relationship_type> edge_type_map;
		std::multimap<std::pair<node, node>, relationship_type> edge_type_map;
		std::map<std::pair<node, edge>, std::string> ne_arrow;

		for(const srcuml_relationship relationship : relationships.get_relationships()){
			//get the nodes from graph g, create edge and add appropriate info.
			ogdf::node lhs, rhs;

			std::map<std::string, ogdf::node>::iterator src_it = class_node_map.find(relationship.get_source());
			if(src_it != class_node_map.end()){
				lhs = src_it->second;
			}

			std::map<std::string, ogdf::node>::iterator dest_it = class_node_map.find(relationship.get_destination());
			if(dest_it != class_node_map.end()){
				rhs = dest_it->second;
			}


			//find possible occurnce of edge already
			auto itr = edge_type_map.find( std::pair<node, node>(lhs, rhs) );
			if(itr == edge_type_map.end()){
				edge_type_map.insert( std::pair<std::pair<node,node>, relationship_type> (std::pair<node,node>(lhs,rhs) , relationship.get_type()) );
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
				//check the relationship type and chose based on the heirarchy
			}
			
		}

		//after do a loop to add to graph
		for(auto edge : edge_type_map){

			/*
				Run through the relationships and make a map of them first, determing there which is best
				std::map<ogdf::edge, relationship_type> edge_type_map;

				bool edge_exists = false;
				for(auto it = edge_map.begin(); it != edge_map.end(); ++it){
					if(it->first == src_it->first && it->second == dest_it->first){
						edge_exists = true;
					}
					if(it->first == dest_it->first && it->second == src_it->first){
						edge_exists = true;
					}
				}
			*/

			ogdf::edge cur_edge = g.newEdge(edge.first.first, edge.first.second);
			//edge_type_map.insert(std::pair<ogdf::edge, relationship_type>(cur_edge, edge.second));

			//ogdf::edge cur_edge = g.newEdge(lhs, rhs);//need to pass to ogdf::node types
			cga.strokeWidth(cur_edge) = 2;

			StrokeType &st = cga.strokeType(cur_edge);

			const relationship_type r_type = edge.second;
			switch(r_type){
			case DEPENDENCY:
				st = StrokeType::Dash;
				break;
			case ASSOCIATION:
				st = StrokeType::Solid;
				break;
			case BIDIRECTIONAL:
				st = StrokeType::Solid;
				break;
			case AGGREGATION:
				st = StrokeType::Solid;
				break;
			case COMPOSITION:
				st = StrokeType::Solid;
				break;
			case GENERALIZATION:
				st = StrokeType::Dash;
				break;
			case REALIZATION:
				st = StrokeType::Dash;
				break;
			}
		}
		//===============================================================================================================


		//Cluster Creation
		//===============================================================================================================

		cluster entity = cg.createCluster(enty);
		cluster control = cg.createCluster(ctrl);
		cluster boundary = cg.createCluster(bndr);

		cga.label(control) = "Control";
		cga.label(boundary) = "Boundary";
		cga.label(entity) = "Entity";

		cga.strokeColor(entity) = Color(0, 0, 0, 255);
		cga.strokeColor(control) = Color(0, 0, 0, 255);
		cga.strokeColor(boundary) = Color(0, 0, 0, 255);

		cga.strokeWidth(entity) = 1.5;
		cga.strokeWidth(control) = 1.5;
		cga.strokeWidth(boundary) = 1.5;

		cga.fillColor(entity) = Color(0, 0, 224, 50);
		cga.fillColor(control) = Color(224, 0, 0, 50);
		cga.fillColor(boundary) = Color(0, 224, 0, 50);

		cga.setFillPattern(entity, FillPattern::Solid);
		cga.setFillPattern(control, FillPattern::Solid);
		cga.setFillPattern(boundary, FillPattern::Solid);
	
		//===============================================================================================================
	
		ClusterPlanarizationLayout cpl;
		cpl.call(g, cga, cg);

		GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
		if(!drawSVG(cga, out, *svg_settings, ne_arrow)){
			std::cout << "Error Write" << std::endl;
		}
	

	/*
		ClusterOrthoLayout col;
		ClusterPlanRep cpr(cga, cg);
		col.call();
	*/


		return true;
	}

private:

	Graph g;

	ClusterGraph cg;

	ClusterGraphAttributes cga;

};

#endif