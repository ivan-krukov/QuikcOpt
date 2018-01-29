#ifndef QUICKOPT_HPP
#define QUICKOPT_HPP

#include <getopt.h>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <exception>
#include <iostream>
#include <iomanip>
#include <eigen3/Eigen/Core>

namespace QuickOpt {

    using namespace std;


    class Argument {

        void splitString(const string &s, char delim, back_insert_iterator<deque<string>> result) {
            stringstream ss(s);
            string item;
            while (getline(ss, item, delim)) {
                *(result++) = item;
            }
        }

        deque<string> split(const string &s, char delim = ',') {
            deque<string> elems;
            splitString(s, delim, back_inserter(elems));
            return elems;
        }

        public:
            char shortName;
            string longName;
            int hasArg;
            bool required;
            bool isSet;

            string value;
            string defaultValue;

            string type;
            string help;

            // TODO: how can I template this properly?
            long as_long() { return std::stol(value); }

            double as_double() { return std::stod(value); }

            Eigen::VectorXi as_VectorXi() {
                deque<string> tokens = split(value);
                Eigen::VectorXi a(tokens.size());
                for(long i = 0; i < tokens.size(); i++) a(i) = std::stol(tokens[i]);
                return a;
            }

            Eigen::VectorXd as_VectorXd() {
                deque<string> tokens = split(value);
                Eigen::VectorXd a(tokens.size());
                for(long i = 0; i < tokens.size(); i++) a(i) = std::stod(tokens[i]);
                return a;
            }

            Eigen::MatrixXd as_MatrixXd() {
                deque<string> rows = split(value, ';');
                long nRow = rows.size();
                deque<deque<string>> m(rows.size());
                for(long i = 0; i < nRow; i++) m[i] = split(rows[i]);
                long nCol = m[0].size();

                Eigen::MatrixXd a(nRow, nCol);
                for(long i = 0; i < nRow; i++) {
                    for(long j = 0; j < nRow; j++) {
                        a(i,j) = stod(m[i][j]);
                    }
                }
                return a;
            }

    };

    class Parser {
        string progName;

        vector<Argument> args;
        map<string, long> argNames;
        map<char, long> argChars;

        string makeOptionSpec() {
            string spec = "";
            for(Argument& a : args) {
                spec += a.shortName;
                if(a.hasArg == required_argument) spec += ":";
            }
            return spec;
        }

        struct option* makeLongOptions() {

            long size = args.size();
            struct option* opts = (struct option*)malloc(sizeof(struct option) * (size + 1));

            for(long i = 0; i < args.size(); i++) {
                opts[i].name = args[i].longName.c_str();
                opts[i].has_arg = args[i].hasArg;
                opts[i].flag = nullptr;
                opts[i].val = args[i].shortName;
            }

            opts[size].name = nullptr;
            opts[size].has_arg = 0;
            opts[size].flag = nullptr;
            opts[size].val = 0;

            return opts;
        }

        public:

        Parser(string name): progName(name) {}

        void add(char shortName, string longName, int hasArg, bool required, string defaultValue, string type, string help) {
            Argument a;

            a.shortName = shortName;
            a.longName = longName;
            a.hasArg = hasArg;
            a.required = required;
            a.defaultValue = defaultValue;
            a.type = type;
            a.help = help;
            a.isSet = false;

            args.push_back(a);

            argNames[longName] = args.size() - 1;
            argChars[shortName] = args.size() - 1;
        }

        Argument& get(string name) {
            try {
                int index = argNames.at(name);
                return args[argNames.at(name)];
            } catch (out_of_range& e) {
                throw runtime_error("QuickOpt::Parser::get(): Unknown option '" + name + "'");
            }
        }

        Argument& get(char name) {
            try {
                int index = argChars.at(name);
                return args[argChars.at(name)];
            } catch (out_of_range& e) {
                throw runtime_error("QuickOpt::Parser::get(): Unknown short option '" + string(1, name));
            }
        }

        void printUsage(long width = 20) {
            cout << "USAGE: " << progName << " [options]" << endl << endl;
            
            cout << setiosflags(ios::left) << setw(width) << "NAME" << setw(width) << "TYPE" 
                << setw(width) << "DEFAULT" << "\t" << "DESCRIPTION" << endl;

            for(Argument& a : args) {

                if(a.required) cout << setw(width) << "-" + string(1, a.shortName) + " --" + a.longName;
                else cout << setw(width) << "[ -" + string(1, a.shortName) + " --" + a.longName + "]";

                cout << setw(width) << a.type << setw(width) << a.defaultValue << "\t" << a.help << endl;
            }
        }

        void parse(int argc, char* argv[]) {
            string optionSpec = makeOptionSpec();

            struct option* longOptions = makeLongOptions();

            char choice;
            do {
                choice = getopt_long(argc, argv, optionSpec.c_str(), longOptions, NULL);
                if(choice == -1) break;
                if(choice == '?') {
                    printUsage();
                    exit(EXIT_FAILURE);
                }

                Argument& a = get(choice);

                if(a.longName == "help") {
                    printUsage();
                    exit(EXIT_FAILURE);
                } else {
                    a.isSet = true;
                    if(a.hasArg != no_argument) a.value = optarg;
                }

            } while(choice != -1);

            free(longOptions);

            for(Argument& a : args) {
                if(a.required && (!a.isSet)) {
                    throw runtime_error("QuickOpt::Parser::parse(): required argument '" + a.longName + "' was not set");
                    printUsage();
                    exit(EXIT_FAILURE);
                }
            }
        }

    };
};

#endif /* QUICKOPT_HPP */
