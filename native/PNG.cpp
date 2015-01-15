
// Preprocessor related:

// Libraries (MSVC):

/*
	This is mainly for MSVC, the needed libraries have
	already been added for GCC/MinGW via 'CC_LIBS' in Monkey.
	
	Change the names of these libraries to
	reflect the versions you decide to use.
*/

#pragma comment(lib, "libpng16.lib")
#pragma comment(lib, "zlib.lib")

// Includes:

// Standard libpng functionality.
#include <png.h>

// C standard library functionality:
#include <cstdio>
#include <cstdlib>

// Namespaces:
namespace imageEncoder
{
	namespace PNG
	{
		// Constant variable(s):
		
		// This acts as the internal bit-depth used for PNG encoding.
		static const int IMAGE_DEPTH = 8; // 16;
		
		// Functions:
		
		// This is currently unused, and may removed.
		pixel* pixel_at(pixel* pixels, size_t width, size_t x, size_t y)
		{
			return pixels + (width * y) + x;
		}
		
		/*
			In the event saving wasn't successful, this command will return 'false'.
			In the case of streams meant specifically for this,
			this should be an indicator to close the file-stream.
			
			The 'stream' argument should point to a standard C stream.
			This argument is not checked if it is 'NULL'.
			
			The 'imageData' argument should be an array of 'pixels', however,
			data-pointers are commonly statically casted to work with this type.
			
			A standard raw RGBA bitmap is already in the
			proper format, and can be casted without issues.
			
			The 'width' and 'height' arguments should specify
			the dimensions of the 'imageData' argument.
		*/
		
		bool save_to_stream(FILE* stream, pixel* imageData, size_t width, size_t height)
		{
			// Local variable(s):
			
			// I'm leaving these variables at the top,
			// just for the sake of better potential C compatibility.
			png_byte** row_pointers;
			png_structp png_ptr;
			png_infop info_ptr;
			
			// Allocate the needed PNG data:
			
			// Attempt to allocate a "write-structure".
			png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			
			// Ensure we were able to allocate a "write-structure":
			if (png_ptr == NULL)
				return false;
			
			// Attempt to allocate an "info-structure".
			info_ptr = png_create_info_struct (png_ptr);
			
			// Ensure we were able to allocate an "info-structure":
			if (info_ptr == NULL)
				return false;
			
			// Standard error handling:
			if (setjmp(png_jmpbuf(png_ptr)))
			{
				png_destroy_write_struct (&png_ptr, &info_ptr);
				
				return false;
			}
			
			// Assign the data specified to the "info-structure":
			// 'PNG_COLOR_TYPE_RGBA' may be switched out at some point for a real system:
			png_set_IHDR(png_ptr, info_ptr, width, height, IMAGE_DEPTH,
			PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
			
			// Allocate an array of pointers for libpng's 'png_set_rows' command.
			// This is allocated on the heap due to its variable size.
			row_pointers = (png_byte**)png_malloc(png_ptr, height * sizeof(png_byte*));
			
			// Initialize each row of the image:
			for (size_t y = 0; y < height; ++y)
			{
				//png_byte* row = (png_byte*)png_malloc(png_ptr, sizeof(uint8_t) * width * sizeof(pixel));
				row_pointers[y] = (png_byte*)(imageData + (width * y));
				
				// Deprecated:
				/*
				for (size_t x = 0; x < width; ++x)
				{
					pixel* pixel = pixel_at(imageData, width, x, y);
					
					*row++ = pixel->red;
					*row++ = pixel->green;
					*row++ = pixel->blue;
					*row++ = pixel->alpha;
				}
				*/
			}
			
			// Write the encoded image-data to the file descriptor:
			png_init_io(png_ptr, stream);
			png_set_rows(png_ptr, info_ptr, row_pointers);
			png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
			
			// Free any heap-allocated data:

			// Deprecated:
			/*
			for (size_t y = 0; y < height; y++)
			{
				png_free(png_ptr, row_pointers[y]);
			}
			*/
			
			// Free the main PNG data.
			png_free(png_ptr, row_pointers);
			
			// Destroy the PNG data we initially retrieved:
			
			// This will destroy both the "write-structure", and the "info-structure".
			png_destroy_write_struct(&png_ptr, &info_ptr);
			//png_destroy_info_struct(png_ptr, &info_ptr);
			
			// Return the default response.
			return true;
		}
		
		// This acts as a standard file/disk I/O wrapper for the main 'save_to_stream' function.
		bool save_to_file(const char* path, pixel* imageData, size_t width, size_t height)
		{
			// This will act as our file-descriptor.
			FILE* fp;
			
			// Attempt to open a file-stream.
			fp = fopen(path, "wb");
			
			// Check if we could open a file-stream:
			if (!fp) // fp == NULL
				return false;
			
			// Execute the main routine.
			bool response = save_to_stream(fp, imageData, width, height);
			
			// Close the file descriptor.
			fclose(fp);
			
			// Return the calculated response.
			return response;
		}
		
		// This acts as the Monkey-wrapper for the main implementation of 'save_to_file'.
		int save_to_file(String path, BBDataBuffer* imageData, int width, int height)
		{
			return save_to_file(path.ToCString<char>(), (pixel*)imageData->ReadPointer(), (size_t)width, (size_t)height);
		}
	}
}
