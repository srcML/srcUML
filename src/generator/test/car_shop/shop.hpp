#include "employee.hpp"
#include <vector>

class Shop{
private:
	std::vector<int> employee_ids;

public:
	Shop();
	void add_employee(const Employee&);
};