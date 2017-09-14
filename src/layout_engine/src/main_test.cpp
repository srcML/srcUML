/*
Author: P.J. Leyden
File: main_test.cpp
To Compile:
	clang++ main_test.cpp -D NDEBUG -std=c++11 -Xlinker ../../../../ogdf/libOGDF.a
*/
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>

#include <ogdf/fileformats/GraphIO.h>

#include <ogdf/tree/TreeLayout.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/module/UMLLayoutModule.h>

#include <ogdf/uml/UMLGraph.h>
#include <ogdf/uml/PlanarizationLayoutUML.h>

#include <string>
#include <iostream>

int main(int argc, char ** argv) {

	//create a pointer to a graph
	ogdf::Graph * my_graph = new ogdf::Graph();

	//create the attributes class and associate it with my_graph and turn on all attr lists
	ogdf::UMLGraph * uml_graph = new ogdf::UMLGraph(*my_graph, 0x1FFFF);

	//get dot file
	std::ifstream dot_file(argv[1]);

	//read in a dot formated file and store in appropriate lists and objects
	if(!ogdf::GraphIO::readDOT(*uml_graph, *my_graph, dot_file))
		std::cout << "Error Read" << std::endl;

	//create a circular layout objects
	ogdf::CircularLayout * my_circle = new ogdf::CircularLayout();

	//run on graph
	my_circle->call(*uml_graph);
	
	//output file
	std::ostream * out;
	if(argc > 2){
		out = new std::ofstream(argv[2]);
	}else{
		out = &std::cout;
	}

	if(!ogdf::GraphIO::drawSVG(*uml_graph, *out))
		std::cout << "Error Write" << std::endl;

	return 0;

}


