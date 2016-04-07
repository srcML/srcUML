#ifndef SRCYUML_CLASS_HPP
#define SRCYUML_CLASS_HPP

#include <iostream>
#include <map>
#include <string>
#include <list>

enum Multiplicity {
    OneToOne,
    OneToMany,
    ManyToMany
};

enum RelationshipType {
    Dependency,
    Association,
    Aggregation,
    Composition,
    Generalization,
    Realization
};

class Relationship {
public:
    Relationship() {}
    Relationship(std::string from, std::string to, RelationshipType type) : 
                                                                            from_(from),
                                                                            to_(to),
                                                                            type_(type) {}
    
    /**
     * setMultiplicity
     *
     * This function is here because some relationships may not have a multiplicity
     * so it is not always necessary to have a multiplicity
     */
    void setMultiplicity(Multiplicity multiplicity) { multiplicity_ = multiplicity; } 
    /**
     * buildRelationshipYuml
     * 
     * This function does exactly as it says, it will build the corresponding yuml
     * for the relationship that it represents
     */
    std::string buildRelationshipYuml() const {
        // first we need to enclose our from and to in brackets "[]"
        std::string enclosed_from = "[" + from_ + "]",
                    enclosed_to = "[" + to_ + "]";

        // Create the multiplicity strings
        std::string from_mult, to_mult;
        switch(multiplicity_) {
            case OneToOne : from_mult = to_mult = "1";
                            break;

            case OneToMany: from_mult = "1";
                            to_mult = "*";
                            break;

            case ManyToMany:from_mult = to_mult = "*";
                            break;

        }
        std::string relationship_with_mult;
        // Check the relationship type and build yuml string
        switch(type_) {
            case Dependency  :  relationship_with_mult = from_mult + "-.-" + to_mult + ">"; 
                                break;

            case Association :  relationship_with_mult = from_mult + "-" + to_mult + ">";
                                break;

            case Aggregation :  relationship_with_mult = from_mult + "+-" + to_mult + ">";
                                break;

            case Composition :  relationship_with_mult = from_mult + "++-" + to_mult + ">";
                                break;

            case Generalization:relationship_with_mult = "^-";
                                break;

            case Realization :  relationship_with_mult = "^-.-";
                                break;
        }

        if(relationship_with_mult != "") {
            if(type_ == Generalization || type_ == Realization) {
                return enclosed_to + relationship_with_mult + enclosed_from;
            } else {
                return enclosed_from + relationship_with_mult + enclosed_to;
            }
        } else {
            return "";
        }   
    }

    std::string getFrom(){return from_;}
    std::string getTo(){return to_;}
    RelationshipType getTypeOfRelationship(){return type_;}

private:
    std::string from_,
                to_;
    RelationshipType type_;
    Multiplicity multiplicity_;
};


/**
 * AttributDeclaration
 *
 * Structure used for storing class attribute declarations
 */
struct AttributeDeclaration {
    
    std::string type;
    std::string name;
    // Don't think I really need these here especially with the new relationship class
    std::string multiplicity;
    std::string parameter_direction;
    bool is_composite;
    //bool is_relationship;
    
    AttributeDeclaration(std::string t, std::string n) : type(t), name(n) {}//, is_composite(false), is_relationship(false) {};
};

/**
 * Constructor Declaration
 *
 */
struct ConstructorDeclaration {
    std::list<struct AttributeDeclaration> parameters;
    ConstructorDeclaration(std::list<struct AttributeDeclaration> params) : parameters(params) {};
};

/**
 * FunctionDeclaration
 *
 * Structure used for storing class function declarations
 */
struct FunctionDeclaration {
    std::string returnType;
    std::string function_name;
    bool pure_virtual, overloaded_assignment, overloaded_equality;
    std::list<struct AttributeDeclaration> function_parameters;
    
    FunctionDeclaration(std::string rType, std::string fName, std::list<struct AttributeDeclaration> params) : returnType(rType), function_name(fName), function_parameters(params), pure_virtual(false), overloaded_assignment(false), overloaded_equality(false) {};
};

/**
 * srcYUMLClass
 *
 * Class used for storing all information for a class located within parsed source code
 */
class srcYUMLClass {
public:
    bool is_interface,
         is_data_type,
         default_constructor,
         copy_constructor,
         overloaded_assignment;
    
    std::string class_name_;
    std::string interface_data_type_name;
    
    // Keys can be public, private, or protected
    std::map<std::string, std::list<struct AttributeDeclaration> > class_data_members;
    std::list<std::string> inheritance_list_;
    std::map<std::string, std::list<struct FunctionDeclaration>> class_functions;
    std::map<std::string, srcYUMLClass> classes_in_class;
    std::list<ConstructorDeclaration> constructors;
    
    
    
    // To class as our key, so we have unique relationships for each class
    std::map<std::string, Relationship> relationships_;
    
    srcYUMLClass() {}

    srcYUMLClass(std::string class_name) : class_name_(class_name) {}

    void setClassName(std::string class_name) {
        class_name_ = class_name;
    }

    void addInternalClassDeclaration(std::string class_name) {
        srcYUMLClass temp(class_name);
        classes_in_class.insert(std::pair<std::string, srcYUMLClass>(class_name, temp));
    }

    void addInternalClassDataMember(std::string class_name, struct AttributeDeclaration data_member, std::string visibility = "public"){
        classes_in_class[class_name].addClassDataMember(data_member, visibility);
    }

    void addClassDataMember(struct AttributeDeclaration data_member, std::string visibility = "public") {
        class_data_members[visibility].push_back(data_member);
    }

    void addClassToInheritanceList(std::string class_name){
        inheritance_list_.push_back(class_name);
    }

    void addClassToInternalClassInheritanceList(std::string class_name, std::string inherited_class) {
        classes_in_class[class_name].addClassToInheritanceList(inherited_class);
    }

    void addFunctionDeclaration(struct FunctionDeclaration function, std::string visibility = "public"){
        class_functions[visibility].push_back(function);
    }

    void addInternalClassFunctionDeclaration(std::string class_name,
                                               struct FunctionDeclaration function,
                                               std::string visibility = "public") {

        classes_in_class[class_name].addFunctionDeclaration(function, visibility);
    }
    
    void hasOverloadedAssignment() {
        overloaded_assignment = true;
    }
    
    void identifyInterface() {
        if(class_data_members.empty()) {
            for(const auto& visibility : class_functions) {
                for(const auto& function : visibility.second) {
                    if(!function.pure_virtual) return;
                }
            }
            is_interface = true;
        }
    }
    
    std::string convertToYuml(std::string class_name){
       
        std::string yuml_format;
        bool public_data, public_functions, private_data, private_functions, protected_data, protected_functions;
        
        public_data = public_functions = private_data = private_functions = protected_data = protected_functions = false;
        
        // Check to see if we have any data in each visibility for class_data_members
        if(class_data_members.find("public") != class_data_members.end()) public_data = true;
        if(class_data_members.find("private") != class_data_members.end()) private_data = true;
        if(class_data_members.find("protected") != class_data_members.end()) protected_data = true;
        
        // Check to see if we have any functions in each visibility for class_functions
        if(class_functions.find("public") != class_functions.end()) public_functions = true;
        if(class_functions.find("private") != class_functions.end()) private_functions = true;
        if(class_functions.find("protected") != class_functions.end()) protected_functions = true;
        
        if(is_interface) {
           yuml_format = "[＜＜interface＞＞;" + class_name ;
           interface_data_type_name = "＜＜interface＞＞;" + class_name;
        } else if(is_data_type) {
           yuml_format = "[＜＜datatype＞＞;" + class_name;
           interface_data_type_name = "＜＜datatype＞＞;" + class_name;
        } else {
            yuml_format = "[" + class_name;
        }
        
        
        if(public_data || private_data || protected_data) {
            yuml_format += "|";
        }
        
        if(public_data) {
            
            for(const auto& itr : class_data_members.at("public")) {
                yuml_format += "+" + itr.name + ":" + itr.type + itr.multiplicity + ";";
            }
        }
        if(private_data) {
            for(const auto& itr : class_data_members.at("private")) {
                yuml_format += "-" + itr.name + ":" + itr.type + itr.multiplicity + ";";
            }
        }
        if(protected_data) {
            for(const auto& itr : class_data_members.at("protected")) {
                yuml_format += "#" + itr.name + ":" + itr.type + itr.multiplicity + ";";
            }
        }

        if(public_functions || private_functions || protected_functions)
        {
            yuml_format += "|";
        }
        if(public_functions) {
            for(const auto& itr : class_functions.at("public")) {
                if(itr.overloaded_assignment) {
                    continue;
                } else if( itr.overloaded_equality) {
                    continue;
                }
                yuml_format += "+" + itr.function_name + "( ";
                size_t numberOfFunctionParameters = 0;
                for(const auto& funcParamItr : itr.function_parameters) {
                    yuml_format += funcParamItr.parameter_direction + " " + funcParamItr.name + ":" + funcParamItr.type + funcParamItr.multiplicity;
                    ++numberOfFunctionParameters;
                    if(numberOfFunctionParameters < itr.function_parameters.size()) {
                        yuml_format += "، ";
                    }
                }
                if(itr.returnType == "void") {
                    yuml_format += " );";
                } else {
                    yuml_format += " ): " + itr.returnType + ";";
                }
            }
        }
        if(private_functions) {
            for(const auto& itr : class_functions.at("private")) {
                if(itr.overloaded_assignment || itr.overloaded_equality) continue;
                yuml_format += "-" + itr.function_name + "( ";
                size_t numberOfFunctionParameters = 0;
                for(const auto& funcParamItr : itr.function_parameters) {
                    yuml_format += funcParamItr.parameter_direction + " " + funcParamItr.name + ":" + funcParamItr.type + funcParamItr.multiplicity;
                    ++numberOfFunctionParameters;
                    if(numberOfFunctionParameters < itr.function_parameters.size()) {
                        yuml_format += "، ";
                    }
                }
                if(itr.returnType == "void") {
                    yuml_format += " );";
                } else {
                    yuml_format += " ): " + itr.returnType + ";";
                }
            }
        }
        if(protected_functions) {
            for(const auto& itr : class_functions.at("protected")) {
                if(itr.overloaded_assignment || itr.overloaded_equality) continue;
                yuml_format += "#" + itr.function_name + "( ";
                size_t numberOfFunctionParameters = 0;
                for(const auto& funcParamItr : itr.function_parameters) {
                    yuml_format += funcParamItr.parameter_direction + " " + funcParamItr.name + ":" + funcParamItr.type + funcParamItr.multiplicity;
                    ++numberOfFunctionParameters;
                    if(numberOfFunctionParameters < itr.function_parameters.size()) {
                        yuml_format += "، ";
                    }
                }
                if(itr.returnType == "void") {
                    yuml_format += " );";
                } else {
                    yuml_format += " ): " + itr.returnType + ";";
                }
            }
        }
        
        // End class
        yuml_format += "]\n";
        
        return yuml_format;
    }
    
private:
};

#endif