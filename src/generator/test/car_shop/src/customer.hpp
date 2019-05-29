#include "string.hpp"

class Customer{
private:
	String name;
	String address;
	String credit_card;

public:
	Customer();
	Customer(String name);

	void set_credit(String);
	String get_credit();

};