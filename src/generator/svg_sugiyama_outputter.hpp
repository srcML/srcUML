/**
 * \file svg_sugiyama_outputter.hpp
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

#ifndef INCLUDED_SVG_SUGIYAMA_OUTPUTTER_HPP
#define INCLUDED_SVG_SUGIYAMA_OUTPUTTER_HPP

#include <svg_outputter.hpp>

/**
 * \class svg_sugiyama_outputter svg_sugiyama_outputter.hpp
 * \brief A Sugiyama style layout for UML diagrams.
 *
 * \ingroup Outputters
 *
 * This class implemenets a standard Sugiyama layout for UML diagrams.
 * More info on Sugiyama-style graph drawing can be found [HERE](https://en.wikipedia.org/wiki/Layered_graph_drawing)
 */

class svg_sugiyama_outputter : public svg_outputter {

public:
	svg_sugiyama_outputter(bool method, bool attr){show_methods = method; show_attributes = attr;}

	bool output(std::ostream& out, std::vector<std::shared_ptr<srcuml_class>> &classes){
		//transfer information from srcUML to ogdf
		
		init_standard(classes);

		//Layout
		//===============================================================================================================
		SugiyamaLayout sl;
		sl.setRanking(new OptimalRanking);
		sl.setCrossMin(new MedianHeuristic);
 
		OptimalHierarchyLayout *ohl = new OptimalHierarchyLayout;
		ohl->layerDistance(50.0);
		ohl->nodeDistance(50.0);
		ohl->weightBalancing(1);
		sl.setLayout(ohl);

		sl.call(ga);

		GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
		if(!drawSVG(ga, out, *svg_settings, node_edge_arrow)){
			std::cout << "Error Write" << std::endl;
		}
		//===============================================================================================================

		return true;
	}
};

#endif