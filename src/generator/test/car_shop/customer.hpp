#include <string>

class Customer{

private:
	std::string name;
	std::string address;
	std::string credit_card;

public:
	Customer();
	Customer(std::string name);

	void set_credit(std::string);
	std::string get_credit();

};