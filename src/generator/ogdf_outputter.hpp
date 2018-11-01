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
//Basic_Graph_Include================================================	
#include <ogdf/basic/Graph.h>
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
			ogdf::node cur_node = g.newNode();
			//Insert into map the node class pairing
			class_node_map.insert(std::pair<std::string, ogdf::node>(aclass->get_name(), cur_node));
			//Set Label
			std::string& label = ga.label(cur_node);
			label = generate_label(aclass);
			//Set Width/Height
			double& w = ga.width(cur_node);
			double& h = ga.height(cur_node);
			//Set Color
			Color& color = ga.fillColor(cur_node);
		}
		std::cerr << "First Loop\n"; 


		//Relationships/Edges
		for(const srcuml_relationship relationship : relationships.get_relationships()){
			//get the nodes from graph g, create edge and add apropriate info.
			//std::cout << "Taco in ogdf\n";
			//std::cerr <<"In Loop\n"; 
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
		}

		std::cerr << "Second Loop\n";


		for(EdgeElement * e = g.firstEdge(); e; e = e->succ()){
			float& w = ga.strokeWidth(e);
			w = 1;
		}

		std::cerr << "Third Loop\n";

		for(NodeElement * n = g.firstNode(); n; n = n->succ()){
			double& h = ga.height(n);
			double& w = ga.width(n);
			Color& color = ga.fillColor(n);

			h = 120;
			w = 120;
			color = Color(Color::Name::Antiquewhite);
		}

		std::cerr << "Forth Loop\n";


		SugiyamaLayout sl;
		sl.setRanking(new OptimalRanking);
		sl.setCrossMin(new MedianHeuristic);
 
		OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
		ohl->layerDistance(30.0);
		ohl->nodeDistance(25.0);
		ohl->weightBalancing(0.8);
		sl.setLayout(ohl);

		sl.call(ga);

		std::cerr << "Sugiyama \n";

		GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
		if(!ogdf::GraphIO::drawSVG(ga, out, *svg_settings)){
			std::cout << "Error Write" << std::endl;
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