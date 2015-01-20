
// Includes:

// C standard library functionality:
#include <cstdlib>
#include <cstdint>

// Namespaces:
namespace imageEncoder
{
	// Structures:
	
	/*
		The 'pixel' structure is defined as the default
		pixel format, and can technically change.
		
		Changes made here will be taken into account
		with the external Monkey-code.
		
		It's best to consider the 'pixel' structure as
		more of a concept, than an actual structure.
	*/
	
	// This acts as the standard pixel structure.
	typedef struct pixel_RGBA
	{
		// Colors:
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t alpha;
	} pixel;
	
	// Functions:
	
	/*
		This command will retrieve a pixel at the location specified.
		
		The return-value is an RGBA-pixel location (pixel/pixel_RGBA pointer),
		which can be used to read a single pixel.
		
		Reading past the location specified is undefined, and there are no limits placed upon
		this command to ensure that the pointer returned is proper to begin with.
		
		Usage of this command is considered "unsafe", and should only be used with full understanding.
	*/
	
	template<typename dimension> pixel* pixel_at(pixel* pixels, dimension width, dimension x, dimension y)
	{
		return pixels + (width * y) + x;
	}
	
	template<typename dimension> pixel* pixel_at(uint8_t* pixels, dimension width, dimension x, dimension y)
	{
		return pixel_at<dimension>((pixel*)pixels, width, x, y);
	}
	
	// This acts as the standard Monkey wrapper for the main implementation.
	// This, like the above overloads, is completely "unsafe"; use at your own risk.
	pixel* pixel_at(BBDataBuffer* pixelData, int width, int x, int y, int offset_InBytes=0)
	{
		return pixel_at<int>(((uint8_t*)pixelData->ReadPointer())+(offset_InBytes*sizeof(uint8_t)), width, x, y);
	}
	
	// For a description of this command, please see the 'imageencoder' module in Monkey.
	// Basically, this will just encode the pixel as an integer.
	int pixel_toInt(pixel* p)
	{
		return *((int*)p);
	}
	
	int pixel_toInt(pixel p)
	{
		return pixel_toInt(&p);
	}
}
