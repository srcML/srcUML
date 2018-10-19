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

class ogdf_outputter : srcuml_outputter{

	ogdf_outputter(){};

	void output(ogdf::GraphAttributes& ga, ogdf::Graph& g, std::vector<std::shared_ptr<srcuml_class>> & classes){
		//transfer information from srcUML to OGDF
		srcuml_relationships relationships = analyze_relationships(classes);

		for(const std::shared_ptr<srcuml_class> & aclass : classes){
			ogdf::node& cur_node = g.newNode();
			//Set Label
			std::string& label = ga.label(cur_node);
			label = generate_label(aclass);
			//Set Width/Height
			double& w = ga.width(cur_node);
			double& h = ga.height(cur_node);
			//Set Color
			Color& color = ga.fillColor(cur_node);

		}
	}

	std::string generate_label(const std::shared_ptr<srcuml_class> & aclass){

	}


	
}

#endif