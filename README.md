# imageencoder
This module provides basic image-encoding functionality to the [Monkey programming language](https://github.com/blitz-research/monkey). Format availability varies between targets and languages.

## **Supported formats:**
* [PNG](http://en.wikipedia.org/wiki/Portable_Network_Graphics) (C++)

### **NOTES:**
* **This module will not work out of the box for most people. You must first install any dependencies a language or target requires.**
* **Not all targets based on a supported language will necessarily work with this module.** At the end of the day, this is up to the underlying libraries' availability.
* **MinGW/GCC support is currently untested.**
* **Experimental functionality can be delegated by defining 'IMAGEENCODER_EXPERIMENTAL' as 'True' with the preprocessor, or using the format-specific experimental flags.** (This mainly applies to usage of Monkey's standard arrays)
* Each supported image-format has its own preprocessor-based implementation-flag(s), these normally can be forced to 'False' if you are unable to use a specific format.
* For more notes and to-do information, please read the comments found in the main module.

## Dependencies:
**C++ Dependencies:**
* *PNG*: [libpng](http://www.libpng.org/pub/png/libpng.html) (And be extension, [zlib](http://www.zlib.net); *zlib is already provided with some distributions of libpng*)