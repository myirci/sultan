#ifndef COLOR_TRAITS_HPP
#define COLOR_TRAITS_HPP

#include "pieces.hpp"

template<typename T>
class ColorTraits;

template<> class ColorTraits<White> {
public:
    static int const color = 1;
};

template<> class ColorTraits<Black> {
public:
    static int const color = 0;
};

#endif // COLOR_TRAITS_HPP
