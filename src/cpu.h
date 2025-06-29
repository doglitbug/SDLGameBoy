#pragma once
#include <cstdint>

typedef uint8_t BYTE;
typedef uint16_t WORD;

struct registers {
    union {
        struct {
            BYTE flag;
            BYTE a;
        };

        WORD af;
    };

    union {
        struct {
            BYTE c;
            BYTE b;
        };

        WORD bc;
    };

    union {
        struct {
            BYTE e;
            BYTE d;
        };

        WORD de;
    };

    union {
        struct {
            BYTE l;
            BYTE h;
        };

        WORD hl;
    };
};
