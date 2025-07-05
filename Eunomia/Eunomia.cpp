#include "Controller.h"
#include "EunomiaStream.h"
#include "Utility.h"
#include <exception>
#include <iostream>

using std::exception; using std::invalid_argument;
using std::cin; using std::cout; using std::endl;


int main(int argc, char* argv[]) {
    try {
        if (argc == 1) {
            Controller cont{};
            cont.run();
        }
        else if (argc == 2 && strcmp(argv[1], "-f") == 0) {
            eout << "Please provide a file name:  ";
            Controller cont{ readString(cin).c_str() };
            cont.run();
        }
        else if (argc == 3 && strcmp(argv[1], "-f") == 0) {
            Controller cont{ argv[2] };
            cont.run();
        }
        else {
            throw invalid_argument{ "Invalid command line arguments" };
        }
        return 0;
    }
    catch (exception& e) {
        eout << e.what() << endl;
        return 1;
    }
}