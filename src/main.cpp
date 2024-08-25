#include <iostream>
#include <fstream>
#include "analysis/lexer.h"
#include "ast/ast.h"

int main(const int argc, const char **argv)
{

    if ( argc < 3 ) {
        std::cerr << "No input file provided" << std::endl;
        std::cerr << "Run the program as followed:" << std::endl;
        std::cerr << "./cstride <input file> <output file>" << std::endl;
        exit(1);
    }
    std::cout << "Compiling file " << argv[ 1 ] << " to path " << argv[ 2 ] << std::endl;

    std::ifstream file_in(argv[ 1 ]);
    std::string content(( std::istreambuf_iterator<char>(file_in)),
                        ( std::istreambuf_iterator<char>()));

    stride::ast::ast_token_set_t token_set;
    stride::lexer::tokenize(content.c_str(), content.size(), token_set);
    std::cout << "Lexical analysis generated " << token_set.token_count << " tokens. \n" << std::endl;
    for (int i = 0; i < token_set.token_count; i++) {
        std::cout << "Token: " << token_set.tokens[i].value << " Type: " << token_set.tokens[i].type << std::endl;
    }
    stride::ast::parse(token_set);

    return 0;
}


