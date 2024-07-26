#pragma once

#if defined(_WIN32) // note the underscore: without it, it's not msdn official!
    // Windows (x64 and x86)
#elif defined(__unix__) // all unices, not all compilers
  #include <X11/Xlib.h>
#elif defined(__linux__)
  #include <X11/Xlib.h>
#elif defined(__APPLE__)
#endif

// sfml_sandbox hashing algorithms depend on CHAR_BIT (a byte) being 8 bits
static_assert(8 == CHAR_BIT);

namespace Platform
{
  #if defined(_WIN32) // note the underscore: without it, it's not msdn official!
      // Windows (x64 and x86)
  #elif defined(__unix__) // all unices, not all compilers
    void config()
    {
      XInitThreads();
    }
  #elif defined(__linux__)
    void config()
    {
      XInitThreads();
    }
  #elif defined(__APPLE__)
  #endif
}
