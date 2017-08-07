/*
Author: P.J. Leyden
File: main_test.cpp
To Compile:
	clang++ main_test.cpp -std=c++11 -I/home/wolfdragoon31/Documents/Programming/ogdf/include/ -Xlinker ../../../../ogdf/libOGDF.a
*/
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
#include <string>

int main(){

	ogdf::Graph my_graph;

	int index = 0;

	//returns a node element
	
	
	my_graph.newNode(index);


	std::cout << my_graph.empty() << std::endl;

	return 0;
}


