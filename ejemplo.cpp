#include <iostream>
#include <cstdlib>
using namespace std;


int s() {
}

int main () {
    int x[10], w;
    x[20] = 5;
    w = x[20]*s();

    cout << x[20] << endl;
    cout << w << endl;
    return EXIT_SUCCESS;
}
