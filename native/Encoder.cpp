
// Preprocessor reltated:
// Nothing so far.

// Includes:

// C standard library functionality:
#include <cstdlib>

// Due to C++11 dependence, this is no longer used.
//#include <cstdint>

// Namespaces:
namespace imageEncoder
{
	// Typedefs/Aliases:
	
	// This is used for string-management internally.
	typedef char character;
	
	// This acts as the standard color type to be
	// used inside of the default 'pixel' structure.
	typedef unsigned char color;
	
	// This is used as a general purpose byte (Octet) type-name.
	typedef unsigned char byte;
	
	// C++ '11 versions:
	// typedef uint8_t color;
	// typedef uint8_t byte;
	
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
		color red;
		color green;
		color blue;
		color alpha;
	} pixel;
	
	// Functions:
	
	// This acts as a macro for 'String' conversion within this Monkey module.
	String::CString<character> toCString(String s)
	{
		return s.ToCString<character>();
	}
	
	#if defined(CFG_IMAGEENCODER_EXPERIMENTAL)
		// This is currently non-standard, and may cause problems.
		template<typename outputType, typename arrayType> outputType* readPointer(Array<arrayType> MArray, size_t MArray_Offset=0)
		{
			return (outputType*)(&MArray[MArray_Offset]);
		}
	#endif
	
	// The 'offset' argument is dependent on the size of 'T'.
	template<typename T> T* readPointer(BBDataBuffer* data, size_t offset=0)
	{
		return (T*)data->ReadPointer(offset*sizeof(T));
	}
	
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
	
	template<typename dimension> pixel* pixel_at(byte* pixels, dimension width, dimension x, dimension y)
	{
		return pixel_at<dimension>((pixel*)pixels, width, x, y);
	}
	
	// This acts as the standard Monkey wrapper for the main implementation.
	// This, like the above overloads, is completely "unsafe"; use at your own risk.
	pixel* pixel_at(BBDataBuffer* pixelData, int width, int x, int y, int offset_InBytes=0)
	{
		return pixel_at<int>(((byte*)pixelData->ReadPointer())+(offset_InBytes*sizeof(byte)), width, x, y);
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
