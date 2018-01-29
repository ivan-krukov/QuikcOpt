#include "QuickOpt.hpp"
#include <fstream>

using namespace std;

// Invocation:
// All arguments:
//
// ./example -c 100 --vector 1,2 --matrix '1,0;0,1' --echo --file="out.txt"
// 
// Help:
// ./example -help
int main(int argc, char *argv[])
{
    
    QuickOpt::Parser parser("test");

    // Table specification of arguments

    //         NAME                HAS_ARG (getopt)   REQ    DEFAULT    TYPE        HELP
    parser.add('c', "count",       required_argument, true,  "",        "int",      "Number of multiplications");
    parser.add('x', "vector",      required_argument, true,  "",        "int[K]",   "Vector data");
    parser.add('A', "matrix",      required_argument, true,  "",        "int[KxK]", "Matrix data");
    parser.add('e', "echo",        no_argument,       false, "false",   "bool",     "Echo input arguments");
    parser.add('f', "file",        required_argument, false, "stdout",  "string",   "Output file");
    parser.add('H', "help",        no_argument,       false, "false",   "bool",     "Print this message and exit");

    parser.parse(argc, argv);

    // long argument
    long count = parser.get("count").as_long();

    // Double vector
    Eigen::VectorXd x = parser.get("vector").as_VectorXd();

    // Double matrix
    Eigen::MatrixXd A = parser.get("matrix").as_MatrixXd();

    // Boolean argument
    bool echo = parser.get("echo").isSet;

    // Any other argument (string in this case)
    QuickOpt::Argument outputFile = parser.get("file");

    // Unspecified arguments throw runtime_error on access
    try {
        parser.get("unknown");
    } catch( runtime_error& e) {
        cerr << e.what() << endl;
    }

    // Example continued...

    if (echo) {
        cout << "Vector: " << endl;
        cout << x << endl;
        cout << "Matrix: " << endl;
        cout << A << endl;
    }

    Eigen::VectorXd r = x.transpose() * A;
    for(long i = 1; i < count; i++) r = r.transpose() * A;

    if(outputFile.isSet) {
        ofstream file(outputFile.value);
        if (file.is_open()) {
            file << r << endl;
            file.close();
        }
    }
    else {
        cout << r << endl;
    }

    return 0;
}
