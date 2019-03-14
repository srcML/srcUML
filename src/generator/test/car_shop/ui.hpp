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
    std::string name;
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
    std::string cool;
};
//====================================================
class Content {
public:
    virtual void setContent(std::string &newContent) {}
    void displayContent();
private:
    std::string currentContent;
};

