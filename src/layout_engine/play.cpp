/*
Author: P.J. Leyden
File: play.cpp
To Compile:
	clang++ play.cpp -D NDEBUG -std=c++11 -oplay -Xlinker ../../../../builds/ogdf_build/libOGDF.a -Xlinker ../../../../builds/ogdf_build/libCOIN.a -pthread
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
#include <ogdf/layered/OptimalHierarchyClusterLayout.h>
#include <ogdf/module/HierarchyClusterLayoutModule.h>
//===================================================================

//Basic_Include======================================================
#include <string>
#include <iostream>
//===================================================================

 
using namespace ogdf;
using namespace ogdf::internal;
 
int main(int argc, char ** argv)
{
//Create Graph Objects
//===================================================================
	Graph g;

	GraphAttributes ga(g,
	  GraphAttributes::nodeGraphics |
	  GraphAttributes::edgeGraphics |
	  GraphAttributes::nodeLabel |
	  GraphAttributes::edgeLabel |
	  GraphAttributes::nodeStyle |
	  GraphAttributes::edgeStyle |
	  GraphAttributes::nodeTemplate);
//===================================================================

//Open Dot File and Read
//===================================================================
 	std::ifstream dot_file(argv[1]);
	if (!GraphIO::readDOT(ga, g, dot_file)) {
		std::cerr << "Could not load " << argv[1] << std::endl;
		return 1;
	}
//===================================================================

//Edit the Graph
//===================================================================

	for(EdgeElement * e = g.firstEdge(); e; e = e->succ()){
		float& w = ga.strokeWidth(e);
		w = 1;
	}

	for(NodeElement * n = g.firstNode(); n; n = n->succ()){
		double& h = ga.height(n);
		double& w = ga.width(n);
		Color& color = ga.fillColor(n);

		h = 120;
		w = 120;
		color = Color(Color::Name::Antiquewhite);
	}


	SugiyamaLayout sl;
	sl.setRanking(new OptimalRanking);
	sl.setCrossMin(new MedianHeuristic);
 
	OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
	ohl->layerDistance(30.0);
	ohl->nodeDistance(25.0);
	ohl->weightBalancing(0.8);
	sl.setLayout(ohl);

	sl.call(ga);

//===================================================================
	
//Set Output Destination
//===================================================================
	std::ostream * out;
	if(argc > 2){
		out = new std::ofstream(argv[2]);
		std::cout << "Printing to: " << argv[2] << std::endl;
	}else{
		out = &std::cout;
	}
//===================================================================

//Create SVG Settings
//===================================================================
	GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
	//For Collapse
//===================================================================

//Call Draw Function
//===================================================================
	if(!ogdf::GraphIO::drawSVG(ga, *out, *svg_settings)){
		std::cout << "Error Write" << std::endl;
	}
//===================================================================
	
 
	return 0;
}