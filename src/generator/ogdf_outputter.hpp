#ifndef LOAD_GRAPH
#define LOAD_GRAPH

//srcUML_Requirements
#include <srcuml_class.hpp>
#include <srcuml_relationship.hpp>
#include <srcSAXEventDispatchUtilities.hpp>
#include <srcSAXController.hpp>
#include <srcuml_dispatcher.hpp>
#include <ClassPolicySingleEvent.hpp>
#include <srcuml_outputter.hpp>

//OGDF_Requirements
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/cluster/ClusterGraph.h>
#include <ogdf/cluster/ClusterGraphAttributes.h>

using namespace ogdf;

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
		/*std::map<std::string, ogdf::node&> class_node_map;

		//Classes/Nodes
		for(const std::shared_ptr<srcuml_class> & aclass : classes){
			ogdf::node cur_node = g.newNode();
			//Insert into map the node class pairing
			class_node_map.insert(std::pair<std::string, ogdf::node&>(aclass.get_name(), cur_node));
			//Set Label
			std::string& label = ga.label(cur_node);
			label = generate_label(aclass);
			//Set Width/Height
			double& w = ga.width(cur_node);
			double& h = ga.height(cur_node);
			//Set Color
			Color& color = ga.fillColor(cur_node);*/


		//Relationships/Edges
		for(const srcuml_relationship relationship : relationships.get_relationships()){
			//get the nodes from graph g, create edge and add apropriate info.
			//std::cout << "Taco in ogdf\n";
			//ogdf::edge& cur_edge = g.newEdge(class_node_map.find(relationship.get_source())->second, class_node_map.find(relationship.get_destination())->second);//need to pass to ogdf::node types
		}

		return true;
	}

	std::string generate_label(const std::shared_ptr<srcuml_class> & aclass){
		//create proper string such that SvgPrinter can parse.
		//\n will be <svg_new_line> box divider will be <svg_box_divide>

	}

private:

	Graph g;

	GraphAttributes ga;

	
};

#endif