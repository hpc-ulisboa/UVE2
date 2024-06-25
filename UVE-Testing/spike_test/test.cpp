#include <boost/regex.hpp>
#include <iostream>
#include <string>

int main() {
    std::string s = "Boost Libraries";
    boost::regex expr{"(\\w+)\\s(\\w+)"};
    boost::smatch what;

    if (boost::regex_search(s, what, expr)) {
        for (size_t i = 0; i < what.size(); ++i) {
            std::cout << what[i] << std::endl;
        }
    }

    return 0;
}