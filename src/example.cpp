#include "QuickOpt.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    
    QuickOpt::Parser parser("test");

    parser.add('k', "number",   required_argument, true,  "", "int",    "Number of things");
    parser.add('n', "vector",   required_argument, true,  "", "int[k]", "Count for each bin");
    parser.add('H', "help",     no_argument,       false, "", "bool",   "Print this message and exit");

    parser.parse(argc, argv);

    long k = parser.get("number").as_long();
    Eigen::VectorXi n = parser.get("vector").as_VectorXi();

    cout << "k: " << k << endl;
    cout << "n: " << endl;
    cout << n << endl;
    return 0;
}
