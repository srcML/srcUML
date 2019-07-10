#ifndef INCLUDED_SVG_THREE_OUTPUTTER_HPP
#define INCLUDED_SVG_THREE_OUTPUTTER_HPP

#include <ogdf/cluster/ClusterOrthoLayout.h>
#include <ogdf/cluster/ClusterPlanRep.h>
#include <svg_outputter.hpp>

class svg_three_outputter : public svg_outputter {

public:

	svg_three_outputter(){}

	bool output(std::ostream& out, std::vector<std::shared_ptr<srcuml_class>> & classes){
		//transfer information from srcUML to ogdf

		init_clustered(classes);
		
		//Layout
		//===============================================================================================================

		ClusterPlanarizationLayout cpl;
		cpl.call(g, cga, cg);

		GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
		if(!drawSVG(cga, out, *svg_settings, ne_arrow)){
			std::cout << "Error Write" << std::endl;
		}

		//===============================================================================================================

		return true;
	}
};

#endif