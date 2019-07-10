#ifndef INCLUDED_SVG_SUGIYAMA_OUTPUTTER_HPP
#define INCLUDED_SVG_SUGIYAMA_OUTPUTTER_HPP


#include <svg_outputter.hpp>

class svg_sugiyama_outputter : public svg_outputter {

public:

	svg_sugiyama_outputter(){}

	bool output(std::ostream& out, std::vector<std::shared_ptr<srcuml_class>> & classes){
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
		
		if(!drawSVG(ga, out, *svg_settings, ne_arrow)){
			std::cout << "Error Write" << std::endl;
		}

		//===============================================================================================================

		return true;
	}
};

#endif