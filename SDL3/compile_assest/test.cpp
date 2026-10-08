#include <iostream>

#include <assets.h>

using namespace std;

int main(int argc, char *argv[]) {
    cout << img_bmp_size << endl;
    for (int i=0;i<10;i++) {
        cout << img_bmp_data[i] << " ";
    }
}