#ifndef INCLUDED_SVG_THREE_OUTPUTTER_HPP
#define INCLUDED_SVG_THREE_OUTPUTTER_HPP


#include <svg_outputter.hpp>

class svg_three_outputter : public svg_outputter {

public:

	svg_three_outputter(){
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

private:

	Graph g;

	GraphAttributes ga;

	ClusterGraph cg;

	ClusterGraphAttributes cga;

};

#endif