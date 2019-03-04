//clang++ test.cpp -D NDEBUG -std=c++11 -oplay -Xlinker builds/ogdf_build/libOGDF.a -Xlinker builds/ogdf_build/libCOIN.a -pthread

//OGDF_Requirements
//Basic_Graph_Include================================================
#include <iostream>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graphics.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
//===================================================================

//Cluster_Graph_Include==============================================
#include <ogdf/cluster/ClusterGraph.h>
#include <ogdf/cluster/ClusterGraphAttributes.h>
#include <ogdf/cluster/ClusterPlanarizationLayout.h>
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

int main(){

	Graph g;

	ClusterGraph cg(g);

	ClusterGraphAttributes cga;

	cga.init(cg,
		GraphAttributes::nodeGraphics |
		GraphAttributes::edgeGraphics |
		GraphAttributes::nodeLabel |
		GraphAttributes::edgeLabel |
		GraphAttributes::nodeStyle |
		GraphAttributes::edgeStyle |
		GraphAttributes::nodeTemplate);

	SList<node> nlist;

	for(int i = 0; i < 10; ++i){
		node cur_node = g.newNode();
		nlist.pushBack(cur_node);
	}

	cluster c1 = cg.createCluster(nlist);
	nlist.clear();

	for(int i = 0; i < 10; ++i){
		node cur_node = g.newNode();
		nlist.pushBack(cur_node);
	}

	cluster c2 = cg.createCluster(nlist);
	nlist.clear();

	for(int i = 0; i < 10; ++i){
		node cur_node = g.newNode();
		nlist.pushBack(cur_node);
	}

	cluster c3 = cg.createCluster(nlist);
	nlist.clear();


	ClusterPlanarizationLayout cpl;
	cpl.call(g, cga, cg);

	GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
	std::ofstream out("test.svg");

	if(!GraphIO::drawSVG(cga, out, *svg_settings)){
		std::cout << "Error Write" << std::endl;
	}

	return true;
}