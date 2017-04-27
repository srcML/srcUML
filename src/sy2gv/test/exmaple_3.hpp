/* Composite Design Pattern example.
 *
 *    * Compose objects into tree structures to represent
 *      whole-part hierarchies in a manner that lets clients
 *      treat objects and compositions uniformly.
 */

#include <iostream>
#include <vector>
using std::cout;
using std::vector;

//===========================================================================
// Class definitions

//---------------------------------------------------------------------------
class Component
{
  public:
      virtual ~Component() {}
      virtual void operation() = 0; //       {cout << "Override in all.\n"; }
      virtual void Add(Component*)    {cout << "Implement in Composite.\n"; }
      virtual void Remove(Component*) {cout << "Implement in Composite.\n"; }
      virtual void GetChild(int)      {cout << "Implement in Composite.\n"; }
};

//---------------------------------------------------------------------------
// Leaf class.
class Leaf: public Component
{
  public:
      virtual void operation()
          { cout << "Leaf operation.\n"; }
};

//---------------------------------------------------------------------------
// Composite class.
class Composite: public Component
{
  public:
      virtual void operation()       
      {
          for (int idx = 0; idx < _components.size(); ++idx)
              _components[idx]->operation();
          cout << "Composite operation.\n";
      }
      virtual void Add(Component* c)  { _components.push_back(c); }
      virtual void Remove(Component*)  { } // Implement
      virtual void GetChild(int)      { } // Implement

  private:
     vector <Component*> _components;
};

//---------------------------------------------------------------------------
// Client class.
class Client
{
  public:
     Client()  { _component =new Composite(); }
     ~Client() { delete _component;                }

     void do_something()
     {
         // Create some leaf nodes
         Component *leaf1      = new Leaf();
         Component *leaf2      = new Leaf();
         Component *leaf3      = new Leaf();

         // Interior node
         Component *composite1 = new Composite();
         composite1->Add(leaf1);
         composite1->Add(leaf2);
         composite1->Add(leaf3);

         // Root
         Component *leaf4      = new Leaf();
         _component->Add(composite1);
         _component->Add(leaf4);
         _component->operation();
     }

  private:
     Component* _component;
};

//===========================================================================
// Using the classes

int main()
{
    Client client;

    client.do_something();
}