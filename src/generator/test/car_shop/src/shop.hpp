#include "manufacturer.hpp"
#include "employee.hpp"
#include <vector>

class Shop{
private:
	Manufacturer *supplier;
	int* employee_ids;

public:
	Shop();
	void add_employee(const Employee&);
	void add_manu(const Manufacturer&);
};