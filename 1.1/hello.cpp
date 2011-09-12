#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
    cout << "Hello";
    if (argc >= 2) {
        int count = atoi(argv[1]);
        if (count > 0) {
            for (int i = 0; i < count; ++i) {
                if (argc == 3) {
                    cout << " " << argv[2];
                } else {
                    cout << " world";
                }
            }
        } else {
            cout << " " << argv[1];
        }
    } else {
        cout << " world";
    }
    cout << "!" << endl;
}
