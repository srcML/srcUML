class String{
	bool operator+(const String&);
	bool operator+(const int&);
	bool operator+(const std::string&);
	bool operator+(const float&);
};

class Person{
public:
	Person();
	String getFirst();
	String getMiddle();
	String getLast();
	bool   setFirst(const String&);
	bool   setMiddle(const String&);
	bool   setLast(const String&);
private:
	String first;
	String middle;
	String &last;
};
