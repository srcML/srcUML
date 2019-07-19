/**
 * \file svg_multi_outputter.hpp
 *
 * \copyright Copyright (C) 2016 srcML, LLC. (www.srcML.org)
 * 
 * \par License:
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

#ifndef INCLUDED_SVG_MULTI_OUTPUTTER_HPP
#define INCLUDED_SVG_MULTI_OUTPUTTER_HPP

#include <svg_outputter.hpp>
#include <ogdf/cluster/ClusterPlanRep.h>

/**
 * \class svg_multi_outputter svg_multi_outputter.hpp
 * \brief Module for generating a multi clustered layout for UML diagrams.
 *
 * \ingroup Outputters
 *
 * This class is a layout type.
 * \todo Implement SVG multiple clustered layout.
 */

class svg_multi_outputter : public svg_outputter {

public:

	svg_multi_outputter(bool method, bool attr){show_methods = method; show_attributes = attr;}

	bool output(std::ostream& out, std::vector<std::shared_ptr<srcuml_class>> &classes){
		//transfer information from srcUML to ogdf

		init_clustered(classes);

		//Determine Clusters
		//===============================================================================================================
		/*
		Plan:
			Determine the clusters, storing pointers to the nodes in an SList<node> object, this will become the cluster
			How to determine the clusters will be the difficult part. I will want to come up with some heuristic, maybe based
			Dragan's Paper, and go fomr there. 
		*/
		//===============================================================================================================


		//Cluster Creation
		//===============================================================================================================
		//===============================================================================================================


		//Layout
		//===============================================================================================================
		ClusterPlanarizationLayout cpl;
		cpl.call(g, cga, cg);

		GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
		if(!drawSVG(cga, out, *svg_settings, node_edge_arrow)){
			std::cout << "Error Write" << std::endl;
		}
		//===============================================================================================================

		return true;
	}
};

#endif