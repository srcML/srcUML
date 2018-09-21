#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/fileformats/GraphIO.h>
#include <iostream>
 
using namespace ogdf;
 
int main(int argc, char ** argv)
{
	Graph G;
	GraphAttributes GA(G,
	  GraphAttributes::nodeGraphics |
	  GraphAttributes::edgeGraphics |
	  GraphAttributes::nodeLabel |
	  GraphAttributes::edgeStyle |
	  GraphAttributes::nodeStyle |
	  GraphAttributes::nodeTemplate);
	if (!GraphIO::readDOT(GA, G, argv[1]) ) {
		std::cerr << "Could not load unix-history.gml" << std::endl;
		return 1;
	}
 
	SugiyamaLayout SL;
	SL.setRanking(new OptimalRanking);
	SL.setCrossMin(new MedianHeuristic);
	GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
 
	OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
	ohl->layerDistance(30.0);
	ohl->nodeDistance(25.0);
	ohl->weightBalancing(0.8);
	SL.setLayout(ohl);
 
	SL.call(GA);
	GraphIO::drawSVG(GA, "unix-history-layout.svg", *svg_settings);
 
	return 0;
}