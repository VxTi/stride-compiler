//
// Created by Luca Warmenhoven on 10/09/2024.
//

#ifndef STRIDE_LANGUAGE_TOKENPATTERNS_H
#define STRIDE_LANGUAGE_TOKENPATTERNS_H

#include "abstractions/TokenSet.h"
#include "../tokens/token.h"

namespace stride::ast::pattern
{

    /**
     * Creates a new PatternFragment that accepts a specific token type.
     * A PatternFragment is a part of a pattern that can be used to match a specific token type.
     * This can be used to match a specific token type in a pattern.
     */
    class PatternFragment
    {
    public:

        const int patternLength;

        /**
         * Creates a new PatternFragment.
         */
        PatternFragment() : patternLength(1)
        {}

        /**
         * Creates a new PatternFragment with a specific length.
         * @param length The length of the pattern.
         */
        explicit PatternFragment(int length) : patternLength(length)
        {}

        /**
         * Checks if the provided token is acceptable.
         * This returns true by default, if not implemented by the subclass.
         * @param token The token to check.
         * @return True if the token is acceptable, false otherwise.
         */
        virtual bool accepts(TokenSet &stream)
        { return true; }
    };

    /**
    * A pattern is a set of rules that can be used to match a specific set of tokens.
    * This can be used to match a specific set of tokens in a pattern.
    */
    class Pattern
    {
    private:
        std::vector<PatternFragment *> fragments;

    public:

        /**
         * Creates a new Pattern with a set of fragments.
         */
        template<typename... Args>
        explicit Pattern(Args... args) : fragments{ args... }
        {}

        virtual bool accepts(TokenSet &tokens)
        { return false; }

        /**
         * Returns the length of the pattern.
         */
        int patternLength();
    };

    /**
     * A pattern that references another pattern.
     * This can be used to reference a predefined pattern in a pattern.
     * This is useful for creating patterns that are used multiple times.
     */
    class PRefPattern : public PatternFragment
    {
    private:
        Pattern *pattern;

    public:


        /**
        * Creates a new Reference Pattern.
        * @param pattern The pattern to reference.
        */
        explicit PRefPattern(Pattern *pattern) : pattern(pattern),
                                                 PatternFragment(pattern->patternLength())
        {}

        bool accepts(TokenSet &tokens) override;
    };

    class PExplicit : public PatternFragment
    {
    private:
        token_type_t matcher;
        void *match_dst;

    public:

        /**
        * Creates a new Explicit pattern.
        * Explicit patterns are patterns that must be matched.
        * If the pattern is matched, the token will be stored in the provided destination.
        * @param match_dst The destination to store the matched token.
        */
        PExplicit(token_type_t type, void *match_dst) : match_dst(match_dst), matcher(type)
        {}

        /**
         * Creates a new Explicit pattern without a destination to store the matched token.
         * @param type The type of the token to match.
         */
        PExplicit(token_type_t type) : match_dst(nullptr), matcher(type) {}

        bool accepts(TokenSet &tokens) override;
    };

    /**
     * A pattern that matches a specific token type.
     */
    class POptional : public PatternFragment
    {
    private:

        token_type_t matcher;
        void *match_dst;

    public:

        /**
         * Creates a new Optional pattern.
         * Optional patterns are patterns that can be matched, but don't have to be.
         * If the pattern is matched, the token will be stored in the provided destination.
         * @param match_dst The destination to store the matched token.
         */
        POptional(token_type_t type, void *match_dst) : match_dst(match_dst), matcher(type)
        {}

        /**
         * Creates a new Optional pattern.
         * Optional patterns are patterns that can be matched, but don't have to be.
         * If the pattern is matched, the token will be stored in the provided destination.
         */
        POptional(token_type_t type) : match_dst(nullptr), matcher(type)
        {}


        bool accepts(TokenSet &tokens) override;
    };

    /**
     * A pattern that matches a range of token types.
     */
    class PVarying : public PatternFragment
    {
    private:

        std::vector<token_type_t> matchers;

    public:

        template<typename... Args>
        explicit PVarying(Args... args) : matchers{ args... }
        {}

        bool accepts(TokenSet &tokens) override;
    };

}

#endif //STRIDE_LANGUAGE_TOKENPATTERNS_H
