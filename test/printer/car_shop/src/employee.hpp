#include "customer.hpp"
#include "shop.hpp"

class Employee : public Customer{
private:
	int employee_id;
	Shop *shop_of_work;

public:
	Employee();

};