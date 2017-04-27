class foo{
public:
	int x;
	int y;
	
};

class bar: foo{
public:
	void foo1(foo &myFoo){
		foo anotherFoo;
	};
	void foo2(const foo thisFoo){
		foo moreFoo;
	}
private:
	foo mainFoo;
	foo finalFoo;
	
};
