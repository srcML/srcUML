/**
 * @file ogdf_outputter.hpp
 *
 * @copyright Copyright (C) 2016 srcML, LLC. (www.srcML.org)
 *
 * This file is part of srcUML.
 *
 * srcUML is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * srcUML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with srcUML.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef INCLUDED_SVG_HELPER_HPP
#define INCLUDED_SVG_HELPER_HPP

#include <iostream>
#include <map>
#include <vector>

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
#include <ogdf/basic/graphics.h>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>
//===================================================================

//Cluster_Graph_Include==============================================
#include <ogdf/cluster/ClusterGraph.h>
#include <ogdf/cluster/ClusterGraphAttributes.h>
//===================================================================

using namespace ogdf;
using namespace ogdf::internal;

enum EndType { FilledTriangle, HollowTriangle, FilledDiamond, HollowDiamond };

class svg_helper{
private:

	std::map<ogdf::node, std::pair<double, double>> node_xy;
	std::map<ogdf::node, std::pair<double, double>> node_rc;
	//in pair its source then target end
	std::map<ogdf::edge, std::pair<EndType, EndType>> edge_end_st;

public:

	//Constructor
	svg_helper();
};

#endif