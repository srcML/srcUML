#include "ui.hpp"
#include "manufacturer.hpp"
#include "part.hpp"
#include "shop.hpp"

class Register{
private:
	MyApplicationWindow main;
	Shop shop;
	Manufacturer manu;
	Employee emp;
public:
	Register();
	void register_store_info(Shop);
	void order_parts(Part, Manufacturer);
	void register_employee(Employee);


};