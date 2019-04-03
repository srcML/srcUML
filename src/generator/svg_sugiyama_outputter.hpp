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
		GraphAttributes::nodeStyle |
		GraphAttributes::edgeStyle |
		GraphAttributes::nodeTemplate);
	}

	bool output(std::ostream& out, std::vector<std::shared_ptr<srcuml_class>> & classes){
		//transfer information from srcUML to ogdf
		srcuml_relationships relationships = analyze_relationships(classes);
		std::map<std::string, ogdf::node> class_node_map;

		//Classes/Nodes
		for(const std::shared_ptr<srcuml_class> & aclass : classes){
			node cur_node = g.newNode();
			//Insert into map the node class pairing
			class_node_map.insert(std::pair<std::string, ogdf::node>(aclass->get_srcuml_name(), cur_node));

			std::string& label = ga.label(cur_node);
			int num_lines = 0;
			int longest_line = 0;
			label = generate_label(aclass, num_lines, longest_line);

			double& h = ga.height(cur_node);
			h = num_lines * 1.3 * 10;//num_lines * 50;

			double& w = ga.width(cur_node);
			w = longest_line * .75 * 10;//longest_line * 10;
		/*
			for(std::string stereo : aclass->get_stereotypes()){
				std::cerr << "Stereotype: " << stereo << '\n';
			}
		*/
			Color& color = ga.fillColor(cur_node);
			color = Color(Color::Name::Antiquewhite);
		}

		std::multimap<std::string, std::string> edge_map;
		//come up with rules for relationship 

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

			bool edge_exists = false;
			for(auto it = edge_map.begin(); it != edge_map.end(); ++it){
				if(it->first == src_it->first && it->second == dest_it->first){
					edge_exists = true;
				}
				if(it->first == dest_it->first && it->second == src_it->first){
					edge_exists = true;
				}
			}

			if(!edge_exists){
				edge_map.insert(std::pair<std::string, std::string>(src_it->first, dest_it->first));
				edge_map.insert(std::pair<std::string, std::string>(dest_it->first, src_it->first));
				//Add relationship checking. Choose more prominent of relationships. Create Hierarchy. 


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
		ohl->layerDistance(50.0);
		ohl->nodeDistance(50.0);
		ohl->weightBalancing(1);
		sl.setLayout(ohl);

		sl.call(ga);

		GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
		if(!drawSVG(ga, out, *svg_settings)){
			std::cout << "Error Write" << std::endl;
		}

		return true;
	}

private:

	Graph g;

	GraphAttributes ga;

	ClusterGraph cg;

	ClusterGraphAttributes cga;

};

#endif