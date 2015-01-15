
// Includes:

// Standard libpng functionality.
#include <png.h>

// C standard library functionality:
#include <cstdlib>
#include <cstdint>

// Namespaces:
namespace imageEncoder
{
	// Structures:
	typedef struct pixel_ARGB
	{
		// Colors:
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;
	} pixel;
}
