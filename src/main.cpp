#include <iostream>
#include "ast/abstractions/AST.h"
#include "StrideFile.h"

using namespace stride;

int main(const int argc, const char **argv)
{

    if ( argc < 2 )
    {
        std::cerr << "No input file provided" << std::endl;
        std::cerr << "Run the program as followed:" << std::endl;
        std::cerr << "./cstride <options?> <relative input file path>" << std::endl;
        exit(1);
    }

    auto *file = new stride::StrideFile((std::string &) argv[ 1 ]);
    file->compile();

    return 0;
}


