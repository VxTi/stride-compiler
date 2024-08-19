#include <iostream>
#include <fstream>
#include "analysis/lexer.h"
#include "parsing/ast_parser.h"


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

    size_t size;
    token_t *tokens;
    ast_node_t *nodes;

    lex_tokenize(content.c_str(), content.size(), &tokens, &size);
    std::cout << "Lexical analysis generated " << size << " tokens. \n" << std::endl;
    ast_parse(tokens, size, &nodes, &size);


    free(tokens);
    free(nodes);

    return 0;
}


