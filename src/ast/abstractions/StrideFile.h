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

        std::string &path;
        std::map<std::string, std::variant<std::string, long int>> compilerFlags;

    public:

        explicit StrideFile(std::string &path) : path(path)
        {}

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

        void interpret();

    };
}
#endif //STRIDE_LANGUAGE_STRIDEFILE_H
