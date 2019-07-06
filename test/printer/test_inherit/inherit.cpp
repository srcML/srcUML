class Doc{
public:
	void draw() = 0;
	void size(){return size_;}
private:
	int size_;
};

class MyDoc: public Doc{
public:
	void draw()override{}
	void size()override{}
};
