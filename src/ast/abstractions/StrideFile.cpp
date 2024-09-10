//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "StrideFile.h"
#include "../../tokens/tokenizer.h"
#include "ASTNodeAbstractions.h"
#include <fstream>
#include <iostream>

using namespace stride;

void StrideFile::compile()
{
    std::ifstream file_in(this->path);
    std::string content(( std::istreambuf_iterator<char>(file_in)),
                        ( std::istreambuf_iterator<char>()));

    std::string output_file_path = this->path
            .substr(0, this->path.find_last_of('.'))
            .append(".asm");

    std::cout << "Compiling file to " << output_file_path << std::endl;

    auto tokens = stride::tokenize(content);
    auto root = stride::ast::parse(*tokens);
    auto irCode = root->codegen();
}

void StrideFile::interpret()
{
 // TODO: Implement this function
}

void StrideFile::setCompilerFlag(std::string &flag, std::variant<std::string, long int> value)
{
    this->compilerFlags[flag] = std::move(value);
}

std::variant<std::string, long int> StrideFile::getCompilerFlag(std::string flag)
{
    return this->compilerFlags[flag];
}