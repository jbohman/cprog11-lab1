#include <iostream>
#include <stdexcept>
#include "bitvector.h"


int main()
{
    // Några saker som ska fungera:
    Vector<bool> a(7);           // initiering med 7 element
    Vector<bool> b(a);           // kopieringskonstruktor
    Vector<bool> c = a;          // kopieringskonstruktor

    a = b;                 // tilldelning genom kopiering
    a[5] = true;              // tilldelning till element

    const Vector<bool> e(10);    // konstant objekt med 10 element
    bool i = e[5];          // const int oper[](int) const körs
    i = a[0];              // vektorn är nollindexerad
    i = a[5];              // int oper[](int) körs

    //a[5] &= false;

    try {
        i = e[10];             // försöker hämta element som ligger utanför e
    } catch (std::out_of_range e) {
        std::cout << e.what() << std::endl;
    }

#if 0
    // Diverse saker att testa
    e[5] = 3;              // fel: (kompilerar ej) tilldelning till const
    b = b;                 // hmm: se till att inte minnet som skall behållas frigörs
#endif


    Vector<bool> v(31); // Skapa en 31 stor vektor
    v[3] = true;
    Vector<bool> w; // tom vektor
    std::copy(v.begin(), v.end(), std::back_inserter(w));
    std::cout << std::distance(v.begin(), v.end());
    // konstant iterator och konvertering
    Vector<bool>::const_iterator it = v.begin();
    std::advance(it, 2);

    return 0;
}
