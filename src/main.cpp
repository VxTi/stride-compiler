#include <iostream>
#include <fstream>
#include "analysis/lexer.h"
#include "ast/ast.h"
#include "ast/syntax/validation/syntax_validation.h"

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

    std::ifstream file_in(argv[ 1 ]);
    std::string content(( std::istreambuf_iterator<char>(file_in)),
                        ( std::istreambuf_iterator<char>()));

    std::string output_file_path = argv[ 1 ];
    output_file_path = output_file_path
            .substr(0, output_file_path.find_last_of('.'))
            .append(".asm");

    std::cout << "Compiling file to " << output_file_path << std::endl;

    ast::ast_token_set_t token_set;
    ast::current_file_name = argv[ 1 ];
    ast::current_file_content = content;

    lexer::tokenize(content.c_str(), content.size(), token_set);
    auto root = ast::parse(token_set);
    ast::validation::validate(*root);

    return 0;
}


