#include "string.hpp"

class Part{
private:
	int id_number;
	String name;
	String type;

public:
	//virtual Part() = 0;
	virtual void register_dimensions() = 0;

};