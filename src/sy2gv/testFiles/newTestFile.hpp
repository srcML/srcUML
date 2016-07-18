class String{
public:
    String(){}
    String(const String&){}
    ~String(){}
    
    String& operator=(const String&){}
};
class Animals{
private:
	int numLegs;
	bool flight;
public:
	Animals(){}
	Animals(int){}
	void setType(){}
};
//=============================
class Pet : public Animals{
public:
	virtual void setName() = 0;
	virtual void setType() = 0;
};
//=============================
class MyPet : public Pet{
public:
	play(){}
	feed(){}
private:
	String petName;
	String type;
};
//=============================
class feedPet{
public:
	feed(){}
};