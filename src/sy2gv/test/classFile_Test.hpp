class String {
public:
    String(){}
    String(const String&){}
    ~String(){}
    
    String& operator=(const String&){}
};
//====================================================
class Window{
public:
    virtual void draw() = 0;
    virtual void minimize() = 0;
    virtual void maximize() = 0;
};
//====================================================
class MyOperatingSystemWindow : public Window{
public:
    MyOperatingSystemWindow(){}
private:
    String name;
    WindowID ID;
};
//====================================================
class MyApplicationWindow : public MyOperatingSystemWindow {
public:
    MyApplicationWindow();
private:
    List<Content> myApplicationContent;
};
//====================================================
class WindowID {
private:
    String ID, name, cool;
};
//====================================================
class Content {
public:
    virtual void setContent(String &newContent) {}
    void displayContent();
private:
    String currentContent;
};

