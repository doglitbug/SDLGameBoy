#pragma once

struct registers {
        union {
            struct {
                byte flag;
                byte a;
            };
            word af;
        };

        union {
            struct {
                byte c;
                byte b;
            };
            word bc;
        };

        union {
            struct {
                byte e;
                byte d;
            };
            word de;
        };

        union {
            struct {
                byte l;
                byte h;
            };
            word hl;
        };

};
