//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "StrideFile.h"
#include "tokens/tokenizer.h"
#include "syntax_tree/ASTNodes.h"
#include <fstream>
#include <iostream>

using namespace stride;

StrideFile::StrideFile(const char *path)
{
    this->filePath = new std::string(path);
    std::ifstream file_in(*this->filePath);
    this->content = new std::string(( std::istreambuf_iterator<char>(file_in)),
                                    ( std::istreambuf_iterator<char>()));
}

void StrideFile::compile()
{
    std::string output_file_path = this->filePath->substr(0, this->filePath->find_last_of('.')).append(".asm");

    std::cout << "Compiling file \"" << this->filePath->c_str() << "\" to " << output_file_path << std::endl;

    auto tokens = stride::tokenize(this);
    auto root = stride::ast::parser::parse(*tokens);
    auto irCode = root->codegen();

    // Write to output file
    /*std::ofstream file_out(output_file_path);
    file_out << (char *) irCode->value;
    file_out.close();*/
}

void StrideFile::interpret()
{
    // TODO: Implement this function
    throw std::runtime_error("Interpretation is not yet implemented.");
}

void StrideFile::setCompilerFlag(std::string &flag, std::variant<std::string, long int> value)
{
    this->compilerFlags[ flag ] = std::move(value);
}

std::variant<std::string, long int> StrideFile::getCompilerFlag(std::string flag)
{
    return this->compilerFlags[ flag ];
}

std::string &StrideFile::path()
{
    return *this->filePath;
}

std::string &StrideFile::getContent()
{
    return *this->content;
}

StrideFile::~StrideFile()
{
    delete this->content;
    delete this->filePath;
}
