/*
Author: P.J. Leyden
File: main_test.cpp
To Compile:
	clang++ main_test.cpp -D NDEBUG -std=c++11 -oogdf_test -Xlinker ../../../../builds/ogdf_build/libOGDF.a -Xlinker ../../../../builds/ogdf_build/libCOIN.a -pthread
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

//Layered============================================================
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
//===================================================================

//Basic_Include======================================================
#include <string>
#include <iostream>
//===================================================================

using namespace ogdf;

int main(int argc, char ** argv) {


	//Create Objects=================================================
	//Create graph pointer
	Graph * base_graph = new ogdf::Graph();
	GraphAttributes GA(*base_graph,
		GraphAttributes::nodeGraphics |
		GraphAttributes::edgeGraphics |
		GraphAttributes::nodeLabel |
		GraphAttributes::edgeStyle |
		GraphAttributes::nodeStyle |
		GraphAttributes::nodeTemplate);


	//created cluster graph and asscoiate base_graph with it. 
	ClusterGraph * cluster_graph = new ogdf::ClusterGraph(*base_graph);

	//create graph and attributes holder. Main Object
	ClusterGraphAttributes * cluster_graph_attr = new ogdf::ClusterGraphAttributes(*cluster_graph, 0x1FFFF);

	//create svg settings object
	GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();

	//get dot file
	std::ifstream dot_file(argv[1]);

	//read in a dot formated file and store in appropriate lists and objects
	if(!ogdf::GraphIO::readDOT(*cluster_graph_attr, *cluster_graph, *base_graph, dot_file)){
		std::cout << "Error Read" << std::endl;
	}
	//===============================================================


	//Layout_Call====================================================
	//create a circular layout objects
	SugiyamaLayout * SL;
	SL->setRanking(new OptimalRanking);
	SL->setCrossMin(new MedianHeuristic);

	OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
	ohl->layerDistance(30.0);
	ohl->nodeDistance(25.0);
	ohl->weightBalancing(0.8);
	SL->setLayout(ohl);

	//run on graph
	SL->call(*cluster_graph_attr);
	//===============================================================


	//Output_Call====================================================
	//Output file
	std::ostream * out;
	if(argc > 2){
		out = new std::ofstream(argv[2]);
		std::cout << "Printing to: " << argv[2] << std::endl;
	}else{
		out = &std::cout;
	}

	//call draw function
	if(!ogdf::GraphIO::drawSVG(*cluster_graph_attr, *out, *svg_settings)){
		std::cout << "Error Write" << std::endl;
	}
	//===============================================================


	return 0;
}
