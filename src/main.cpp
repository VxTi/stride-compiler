#include <iostream>
#include <fstream>
#include "analysis/lexer.h"
#include "ast/ast.h"
#include "ast/syntax/validation/syntax_validation.h"

using namespace stride;

int main(const int argc, const char **argv)
{

    if ( argc < 3 ) {
        std::cerr << "No input file provided" << std::endl;
        std::cerr << "Run the program as followed:" << std::endl;
        std::cerr << "./cstride <options?> <input file>" << std::endl;
        exit(1);
    }
    std::cout << "Compiling file " << argv[ 1 ] << " to path " << argv[ 2 ] << std::endl;

    std::ifstream file_in(argv[ 1 ]);
    std::string content(( std::istreambuf_iterator<char>(file_in)),
                        ( std::istreambuf_iterator<char>()));

    ast::ast_token_set_t token_set;
    ast::current_file_name = argv[ 1 ];
    ast::current_file_content = content;

    lexer::tokenize(content.c_str(), content.size(), token_set);
    auto root = ast::parse(token_set);
    ast::validation::validate(*root);

    return 0;
}


