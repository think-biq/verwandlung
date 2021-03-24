/*
    Definitions for hidden template defintions to unify visibility settings
    on macos.

    2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
    See license.md for details.

    https://think-biq.com
*/
#pragma once

#ifdef __APPLE__
// Workaround OSX linker warning: xcode uses different default symbol
// visibilities for static libs vs executables. 
// Thanks to https://github.com/c42f/tinyformat/commit/3913307c283db10c9fd6bb3f30dcc271d4661fd9
#   define ATTRIBUTE_HIDDEN __attribute__((visibility("hidden")))
#else
#   define ATTRIBUTE_HIDDEN
#endif