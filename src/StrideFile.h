//
// Created by Luca Warmenhoven on 11/09/2024.
//

#ifndef STRIDE_LANGUAGE_STRIDEFILE_H
#define STRIDE_LANGUAGE_STRIDEFILE_H

#include <string>
#include <map>

namespace stride
{

    class StrideFile
    {

    private:

        std::string *content;
        std::string *filePath;
        std::map<std::string, std::variant<std::string, long int>> compilerFlags;

    public:

        explicit StrideFile(const char *path);

        ~StrideFile();

        /**
         * Returns the path to the file.
         */
        std::string &path();

        /**
         * Returns the content of the file.
         */
        std::string &getContent();

        /**
         * Sets a compiler flag.
         * This will set a compiler flag that can be used to compile the file.
         * @param flag The flag to set.
         * @param value The value to set the flag to.
         */
        void setCompilerFlag(std::string &flag, std::variant<std::string, long int> value);

        /**
         * Gets a compiler flag.
         * This will get the value of a compiler flag that is set.
         * @param flag The flag to get.
         * @return The value of the flag.
         */
        std::variant<std::string, long int> getCompilerFlag(std::string flag);

        /**
         * Compiles the file.
         * This will read the file, compile it and write the output to a new
         * executable file.x
         */
        void compile();

        /**
         * Interprets the file.
         * This will read the file, compile it and interpret the output.
         * This is not yet implemented.
         */
        void interpret();

    };
}
#endif //STRIDE_LANGUAGE_STRIDEFILE_H
