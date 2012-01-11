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
    std::cout << std::distance(v.begin(), v.end()) << std::endl;

    // EXTRA
    for(Vector<bool>::const_iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    v[3] = true;

    for(Vector<bool>::const_iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    // EXTRA END


    Vector<bool> w; // tom vektor
    std::copy(v.begin(), v.end(), std::back_inserter(w));
    std::cout << "v: " << std::distance(v.begin(), v.end()) << std::endl;
    std::cout << "w: " << std::distance(w.begin(), w.end()) << std::endl;
    // konstant iterator och konvertering
    Vector<bool>::const_iterator it = v.begin();
    std::advance(it, 2);

    // EXTRA
    std::advance(it, 1);
    std::cout << *it << std::endl;

    for(Vector<bool>::const_iterator it = w.begin(); it != w.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    // EXTRA END


    Vector<bool> test_a(10);
    Vector<bool> test_b(10);
    test_a[3] = 1;
    test_b[3] = 0;
    std::cout << test_a[3] << test_b[3] << std::endl;
    Vector<bool> res = test_a & test_b;
    std::cout << res[3] << std::endl;

    std::cout << "Bitvector, 100 length, bit 3,4,95,99,32 set to one" << std::endl;
    Vector<bool> weight_a(100);
    weight_a[3] = 1;
    weight_a[4] = 1;
    weight_a[95] = 1;
    weight_a[99] = 1;
    weight_a[32] = 1;
    std::cout << weight_a.weight_math() << std::endl;
    std::cout << weight_a.weight_lookup() << std::endl;
    std::cout << weight_a.weight_loop() << std::endl;

    std::cout << "Bitvector, 33 length, bit 21 set to zero" << std::endl;
    Vector<bool> weight_b(33, true);
    weight_b[21] = 0;
    std::cout << weight_b.weight_math() << std::endl;
    std::cout << weight_b.weight_lookup() << std::endl;
    std::cout << weight_b.weight_loop() << std::endl;

    return 0;
}
