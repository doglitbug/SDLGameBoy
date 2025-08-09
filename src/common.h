#pragma once

#include <SDL3/SDL.h>

typedef uint8_t BYTE;
typedef int8_t sBYTE;
typedef uint16_t WORD;

#define NOTYET SDL_Log("Not yet implemented");

inline sBYTE unsignedToSigned(const BYTE input) {
    return static_cast<sBYTE>(input);
}

inline bool testBit(const int bit, const BYTE &value) {
    return (value & (1 << bit)) != 0;
}

inline void setBit(const int bit, BYTE &value) {
    value |= (1 << bit);
}

inline void resetBit(const int bit, BYTE &value) {
    value &= ~(1 << bit);
}