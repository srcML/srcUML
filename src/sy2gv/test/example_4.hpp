class taco{
public:
	virtual void make() = 0;
	virtual void eat() = 0;
	virtual void addCheese() = 0;
	virtual void addMeat() = 0;
};

class myTaco: taco{
public:
	void make(){
		count++;
		myTaco theTaco;
	};
	void eat(){
		count--;
	};
	void addCheese(){
		cheese = "American";
	};
	void addMeat(){
		meat = "ham";
	};
private:
	std::string cheese;
	std::string meat;
	std::string count;
	bool eaten;
};