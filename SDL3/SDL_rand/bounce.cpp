#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    int v = strtol(argv[1], nullptr, 10);
    int y = 0;
    bool done = false;
    while (!done) {
        for (int i=0;i<y;i++) {
            cout << " ";
        }
        cout << "O" << endl;
        v--;
        y += v;
        if (y <= 0) {
            v = -v;
            if (v == 0) {
                done = true;
            }
        }
    }
    return EXIT_SUCCESS;
}