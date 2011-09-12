#include <iostream>

class A {
public:
    A() 
        {std::cout << "The default contructor" << std::endl; }
    A(const A & ref) 
        {std::cout << "The copy contructor" << std::endl; }
    ~A() 
        {std::cout << "The destructor" << std::endl; }
    A(char * s) 
        {std::cout << "Some other constructor " << s << std::endl;}
    A & operator=(const A & s) 
        {std::cout << "The assignment operator" << std::endl; 
         return *this;}
};

#define DEBUG(s) std::cout << "--- " << s << std::endl

void no_ref(A a) { DEBUG("inside no_ref"); }
void with_ref(const A & a) { DEBUG("inside with_ref"); }

int main()
{
    DEBUG("1. constructor with arg");
    A a("my name is a");
    
    DEBUG("2. assignment to new variable");
    A b = a;         // vad är skillnaden
    
    DEBUG("3. copy construction");
    A c(a);          // mellan dessa
    
    DEBUG("4. uninitialized variable");
    A d;             // tre tekniker?
    
    DEBUG("5. assignment to variable");
    d = a;

    DEBUG("--------");
    
    DEBUG("6. call by value");
    no_ref(a);       // Bildas temporära objekt?
    
    DEBUG("7. call by reference");
    with_ref(a);     // Bildas temporära objekt?

    DEBUG("---------");
    
    DEBUG("8. Allocate vector, put in A*");
    A *aa = new A[5];
    
    DEBUG("9. Free A* that points to A[]");
    delete aa;       // Vad kommer att hända?
    
    DEBUG("done!");
    return 0;
}
