#ifndef INCLUDED_SVG_MULTI_OUTPUTTER_HPP
#define INCLUDED_SVG_MULTI_OUTPUTTER_HPP


#include <svg_outputter.hpp>
#include <ogdf/cluster/ClusterPlanRep.h>

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