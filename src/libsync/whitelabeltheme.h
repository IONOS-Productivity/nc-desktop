#ifndef WHITELABELTHEME_H
#define WHITELABELTHEME_H
#define WHITELABEL_IONOS

#include "basetheme.h"
#include "stratotheme.h"
#include "ionostheme.h"

namespace OCC {

// #if defined(WHITELABEL_STRATO)
//     using WL = StratoTheme;
// #elif defined(WHITELABEL_IONOS)
//     using WL = IonosTheme;
// #else
//     using WL = BaseTheme;
// #endif

#if defined(WHITELABEL_IONOS)
    static IonosTheme WLTheme;
#elif defined(WHITELABEL_STRATO)
    static StratoTheme WLTheme;
#else
    static BaseTheme WLTheme;
#endif

} // namespace OCC

#endif // WHITELABELTHEME_H


// #ifndef WHITELABELTHEME_H
// #define WHITELABELTHEME_H
// #define WHITELABEL_IONOS
// #include "basetheme.h"
// #include "ionostheme.h"
// #include "stratotheme.h"

// namespace OCC {

// class WhitelabelTheme {
// public:
//     static BaseTheme& instance() {
//         // This selection happens entirely at compile time
//         #if defined(WHITELABEL_IONOS)
//             static IonosTheme theme;
//         #elif defined(WHITELABEL_STRATO)
//             static StratoTheme theme;
//         // #else
//         //     static BaseTheme theme;
//         #endif
//         return theme;
//     }

// private:
//     WhitelabelTheme() = default; // prevent instantiation
// };

// } // namespace OCC

// #endif // WHITELABELTHEME_H
