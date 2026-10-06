#include <iostream>

#include "stuff.h"

using namespace std;

int main(int argc, char *argv[]) {
    for(int i = 0; i < arguments_n_stuff_cnt; i++) {
        cout << arguments_n_stuff[i] << endl;
    }

    return 0;
}