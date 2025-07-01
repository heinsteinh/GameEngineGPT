#ifndef MOUSECODES_H
#define MOUSECODES_H

#include <stdint.h>

namespace fts
{
    using MouseCode = uint16_t;

    namespace Mouse
    {
        enum MouseCode
        {
            // from SDL.h
            BUTTON_LEFT   = 1,
            BUTTON_MIDDLE = 2,
            BUTTON_RIGHT  = 3,
            BUTTON_X1     = 4,
            BUTTON_X2     = 5,
            BUTTON_LAST
        };
    }
} // namespace fts

#endif // MOUSECODES_H
