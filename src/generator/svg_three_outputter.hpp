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
		std::cerr << "HERE1\n";
		init_clustered(classes);
		std::cerr << "HERE2\n";
		//Determine Clusters
		//===============================================================================================================

		SList<node> ctrl, bndr, enty;

		for(auto pair : class_node_map){
			std::string stereo = "";
			std::cerr << "HERE2.1\n";
			if(pair.first->get_stereotypes().begin() != pair.first->get_stereotypes().end()){
				stereo = *(pair.first->get_stereotypes().begin());
			}

			Color& color = cga.fillColor(pair.second);

			std::cerr << "HERE2.2\n";
			if(stereo == "control"){
				std::cerr << "HERE2.21\n";
				color = Color(224, 0, 0, 100);
				ctrl.pushBack(pair.second);

			}else if(stereo == "boundary"){
				std::cerr << "HERE2.22\n";
				color = Color(0, 224, 0, 100);
				bndr.pushBack(pair.second);

			}else if(stereo == "entity"){
				std::cerr << "HERE2.23\n";
				color = Color(0, 0, 224, 100);
				std::cerr << "HERE2.233\n";
				enty.pushBack(pair.second);

			}else if(stereo == ""){
				std::cerr << "HERE2.24\n";
				color = Color(130, 130, 130, 200);
			}
		}
		//===============================================================================================================

		std::cerr << "HERE3\n";

		//Cluster Creation
		//===============================================================================================================

		cluster entity = cg.createCluster(enty);
		cluster control = cg.createCluster(ctrl);
		cluster boundary = cg.createCluster(bndr);

		cga.label(control) = "Control";
		cga.label(boundary) = "Boundary";
		cga.label(entity) = "Entity";

		cga.strokeColor(entity) = Color(0, 0, 0, 255);
		cga.strokeColor(control) = Color(0, 0, 0, 255);
		cga.strokeColor(boundary) = Color(0, 0, 0, 255);

		cga.strokeWidth(entity) = 1.5;
		cga.strokeWidth(control) = 1.5;
		cga.strokeWidth(boundary) = 1.5;

		cga.fillColor(entity) = Color(0, 0, 224, 50);
		cga.fillColor(control) = Color(224, 0, 0, 50);
		cga.fillColor(boundary) = Color(0, 224, 0, 50);

		cga.setFillPattern(entity, FillPattern::Solid);
		cga.setFillPattern(control, FillPattern::Solid);
		cga.setFillPattern(boundary, FillPattern::Solid);
	
		//===============================================================================================================
	
		ClusterPlanarizationLayout cpl;
		cpl.call(g, cga, cg);

		GraphIO::SVGSettings * svg_settings = new ogdf::GraphIO::SVGSettings();
		
		if(!drawSVG(cga, out, *svg_settings, ne_arrow)){
			std::cout << "Error Write" << std::endl;
		}
	

	/*
		ClusterOrthoLayout col;
		ClusterPlanRep cpr(cga, cg);
		col.call();
	*/


		return true;
	}

private:

	Graph g;

	ClusterGraph cg;

	ClusterGraphAttributes cga;

};

#endif