#include <iostream>
#include <fstream>

using namespace std;

const string basename = "stuff";

int main(int argc, char *argv[]) {
    /* Generate the header */
    ofstream header(basename + ".h");

    header << "#ifndef __STUFF_H__" << endl;
    header << "#define __STUFF_H__" << endl;
    header << "" << endl;
    header << "extern const int arguments_n_stuff_cnt;" << endl;
    header << "extern const char *arguments_n_stuff[];" << endl;
    header << "" << endl;
    header << "#endif  /* __STUFF_H__ */" << endl;

    header.close();

    ofstream source(basename + ".cpp");

    source << "#include \"stuff.h\"" << endl;
    source << "" << endl;

    source << "const int arguments_n_stuff_cnt = " << (argc - 1) << ";" << endl;
    source << "const char *arguments_n_stuff[] = {" << endl;
    for(int i = 1; i < argc; i++) {
        source << "\"" << argv[i] << "\"," << endl;
    }
    source << "};" << endl;

    source.close();
}