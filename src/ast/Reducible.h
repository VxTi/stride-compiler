//
// Created by Luca Warmenhoven on 12/09/2024.
//

#ifndef STRIDE_LANGUAGE_REDUCIBLE_H
#define STRIDE_LANGUAGE_REDUCIBLE_H

#include <utility>
#include <memory>

/**
 * Represents a reducible object.
 * Reducible objects can be reduced to a simpler form.
 */
template <class Base, class Reduced>
class Reducible
{
public:

    /**
     * Reduces the object to a simpler form.
     * @return The reduced object.
     */
    virtual std::variant<Base, Reduced> reduce() = 0;

    /**
     * Checks whether the object is reducible.
     * This method is used to determine whether the object can be reduced,
     * and needs to be implemented by child classes.
     * @return Whether the object is reducible.
     */
    virtual bool isReducible() = 0;
};

#endif //STRIDE_LANGUAGE_REDUCIBLE_H
