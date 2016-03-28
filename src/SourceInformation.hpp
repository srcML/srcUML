#ifndef SOURCE_INFORMATION_HPP
#define SOURCE_INFORMATION_HPP

#include <map>
#include <list>
#include <string>
#include <iostream>
#include "srcYUMLClass.hpp"

class SourceInformation {
public:
	SourceInformation() {}
	void addClass(std::string class_name, srcYUMLClass& class_to_add) {
		if(class_name == "") {
			return;
		}
		class_to_add.setClassName(class_name);
		classes_in_source_.insert(std::pair<std::string, srcYUMLClass>(class_name, class_to_add));
	}

	std::string compileYUML() {
		std::string output_string;
		for(auto& c : classes_in_source_) {
			output_string += c.second.convertToYuml(c.first);
		}

		output_string += resolveRelationships();

		return output_string;
	}

	std::string resolveRelationships() {

		/*

			A data member could signify a aggregation - container
										  composition - control of lifecycle

			An argument in a function can signify dependency - using that class
				So, argument thats type d.n.e in the class's data members
			Generalization is shown in the class's inheritance list
         
            ## NEED ##
            Function in/outs
         

		*/
        // Classes <name, class> -> class : { attributes : inheritance }
        // name is important, if attribute type == Classes key name we have a relationship
        for(auto& class_itr : classes_in_source_) {
            for(auto& visibility_itr : class_itr.second.class_data_members) {
                for(auto& attribute_itr : visibility_itr.second) {
                    // We have found a has-a relationship
                    std::cout << attribute_itr.type << std::endl;
                    const auto& found_class = classes_in_source_.find(attribute_itr.type);
                    if(found_class != classes_in_source_.end()) {
                        // Composite aggregation
                        if(!attribute_itr.is_composite) {
                            //if(attribute_itr.multiplicity != "") {
                            if(found_class->second.is_data_type) {
                                // No relationships with datatypes
                                //Relationship temp(class_itr.first, found_class->second.interface_data_type_name, Aggregation);
                                //addRelationship(temp);
                            } else {
                                Relationship temp(class_itr.first, found_class->second.class_name_, Aggregation);
                                addRelationship(temp);
                            }
                            
                                //yuml_relationships += "[" + class_itr.first + "]" + "<>1->*[" + attribute_itr.type + "]\n";
                                //attribute_itr.is_relationship = true;
                            //}
                            //else {
                                //yuml_relationships += "[" + class_itr.first + "]" + "<>->[" + attribute_itr.type + "]\n";
                                //attribute_itr.is_relationship = true;
                            //}
                        }
                        else {
                            if(found_class->second.is_data_type) {
                                //Relationship temp(class_itr.first, found_class->second.interface_data_type_name, Composition);
                                //addRelationship(temp);
                            } else {
                                Relationship temp(class_itr.first, found_class->second.class_name_, Composition);
                                addRelationship(temp);
                            }
                            //if(attribute_itr.multiplicity != "") {
                                //yuml_relationships += "[" + class_itr.first + "]" + "++*->1" + "[" + attribute_itr.type + "]\n";
                                //attribute_itr.is_relationship = true;
                            //}
                            //else {
                                //yuml_relationships += "[" + class_itr.first + "]" + "++->" + "[" + attribute_itr.type + "]\n";
                                //attribute_itr.is_relationship = true;
                            //}
                        }
                    }
                }
                for(auto& class_in_class_itr : class_itr.second.classes_in_class) {
                    for(auto& class_in_class_visibility_itr : class_in_class_itr.second.class_data_members) {
                        for(auto& class_in_class_attr_itr : class_in_class_visibility_itr.second) {
                            // We have a relationship
                            const auto& found_class = classes_in_source_.find(class_in_class_attr_itr.type);
                            if(found_class != classes_in_source_.end()) {
                                if(!class_in_class_attr_itr.is_composite) {
                                    //if(class_in_class_attr_itr.multiplicity != "") {
                                        Relationship temp(class_itr.first, found_class->second.class_name_, Aggregation);
                                        addRelationship(temp);
                                        //yuml_relationships += "[" + class_in_class_attr_itr.type + "]" + "<>*->1" + "[" + class_itr.first + "]\n";
                                        //class_in_class_attr_itr.is_relationship = true;
                                    //}
                                    //else {
                                        //yuml_relationships += "[" + class_in_class_attr_itr.type + "]" + "<>->" + "[" + class_itr.first + "]\n";
                                        //class_in_class_attr_itr.is_relationship = true;
                                    //}
                                }
                                else {
                                    //if(class_in_class_attr_itr.multiplicity != "") {
                                        Relationship temp(class_itr.first, found_class->second.class_name_, Composition);
                                        addRelationship(temp);
                                        //yuml_relationships += "[" + class_in_class_attr_itr.type + "]" + "++*->1" + "[" + class_itr.first + "]\n";
                                        //class_in_class_attr_itr.is_relationship = true;
                                    //}
                                    //else {
                                       //yuml_relationships += "[" + class_in_class_attr_itr.type + "]" + "++->" + "[" + class_itr.first + "]\n";
                                        //class_in_class_attr_itr.is_relationship = true;
                                    //}
                                }
                            }
                        }
                    }
                }
           }
		}
        for(const auto& current_class_itr : classes_in_source_) {
            for(const auto& inheritance_list_itr : current_class_itr.second.inheritance_list_) {
                const auto& found_class = classes_in_source_.find(inheritance_list_itr);
                if(found_class != classes_in_source_.end()) {
                    if(found_class->second.is_interface) {
                        Relationship temp(current_class_itr.first, found_class->second.interface_data_type_name, Realization);
                        addRelationship(temp);
                    } else {
                        Relationship temp(current_class_itr.first, found_class->first, Generalization);
                        addRelationship(temp);
                    }
                }
            }
        }
        std::string relationships_output;
        for(const auto& from_relationships : relationships_in_source_) {
            for(const auto& to_relationships : from_relationships.second){
                relationships_output += to_relationships.second.buildRelationshipYuml() + "\n";
            }
        }
        return relationships_output;
    }

	void addRelationship(Relationship relationship_to_add) {
		const auto& inserted = relationships_in_source_[relationship_to_add.getFrom()].insert(std::pair<std::string, Relationship>(relationship_to_add.getTo(), relationship_to_add));
		if(!inserted.second) {
			// compare the relationship types to see which is greater
			if(inserted.first->second.getTypeOfRelationship() < relationship_to_add.getTypeOfRelationship()) {
                relationships_in_source_[relationship_to_add.getFrom()][relationship_to_add.getTo()] = relationship_to_add;
			}
		}
    }

private:
	// [ClassName, Class Information]
	std::map<std::string, srcYUMLClass> classes_in_source_;
	// [ClassName : yuml Representation]
	std::map<std::string, std::string> converted_classes_;
	// [FromClassName : [ToClassName, Relationship]]
	// This way we can create unique relationships for each from->to relationship
	// We will not have multiple relationships from one class to another
	std::map<std::string, std::map<std::string, Relationship>> relationships_in_source_;
};

#endif