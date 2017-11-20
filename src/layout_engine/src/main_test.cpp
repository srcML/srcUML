/*
Author: P.J. Leyden
File: main_test.cpp
To Compile:
	clang++ main_test.cpp -D NDEBUG -std=c++11 -oogdf_test -Xlinker ../../../../ogdf/libOGDF.a
*/
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

//Basic_Include======================================================
#include <string>
#include <iostream>
//===================================================================


int main(int argc, char ** argv) {


	//Create Objects=================================================
	//Create graph pointer
	ogdf::Graph * base_graph = new ogdf::Graph();

	//created cluster graph and asscoiate base_graph with it. 
	ogdf::ClusterGraph * cluster_graph = new ogdf::ClusterGraph(*base_graph);

	//create graph and attributes holder. Main Object
	ogdf::ClusterGraphAttributes * cluster_graph_attr = new ogdf::ClusterGraphAttributes(*cluster_graph, 0x1FFFF);

	//create svg settings object
	ogdf::GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();

	//get dot file
	std::ifstream dot_file(argv[1]);

	//read in a dot formated file and store in appropriate lists and objects
	if(!ogdf::GraphIO::readDOT(*cluster_graph_attr, *cluster_graph, *base_graph, dot_file)){
		std::cout << "Error Read" << std::endl;
	}
	//===============================================================


	//Layout_Call====================================================
	//create a circular layout objects
	ogdf::CircularLayout * circle_layout = new ogdf::CircularLayout();

	//run on graph
	circle_layout->call(*cluster_graph_attr);
	//===============================================================


	//Output_Call====================================================
	std::ostream * out;
	if(argc > 2){
		out = new std::ofstream(argv[2]);
	}else{
		out = &std::cout;
	}

	if(!ogdf::GraphIO::drawSVG(*cluster_graph_attr, *out, *svg_settings)){
		std::cout << "Error Write" << std::endl;
	}

	if(out != &std::cout)
		{delete out;}
	//===============================================================


	return 0;
}
