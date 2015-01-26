
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
//#include <cstdlib>

// Namespaces:
namespace imageEncoder
{
	namespace PNG
	{
		// Constant variable(s):
		
		// This acts as the internal bit-depth used for PNG encoding.
		static const int DEFAULT_IMAGE_DEPTH = 8; // 16;
		
		// Functions:		
		/*
			In the event saving wasn't successful, this command will return 'false'.
			In the case of streams meant specifically for this,
			this should be an indicator to close the file-stream.
			
			The 'stream' argument should point to a standard C stream.
			This argument is not checked if it is 'NULL'.
			
			The 'imageData' argument should be an array of pixels,
			formatted according to the arguments you pass in.
			
			A standard raw RGBA bitmap is already in the
			proper format by default, and can be used without issues.
			
			The 'width' and 'height' arguments should specify
			the dimensions of the 'imageData' argument.
			
			This command is considered "unsafe" under specific situations;
			please read the 'save_to_file_safe' command's documentation.
		*/
		
		bool save_to_stream(FILE* stream, png_byte* imageData, size_t width, size_t height, int bit_depth=DEFAULT_IMAGE_DEPTH, int color_type=PNG_COLOR_TYPE_RGB_ALPHA, int interlace_type=PNG_INTERLACE_NONE, int compression_type=PNG_COMPRESSION_TYPE_DEFAULT, int filter_type=PNG_FILTER_TYPE_DEFAULT)
		{
			// Local variable(s):
			
			// I'm leaving these variables at the top, just for the
			// sake of better potential C compatibility:
			
			// For safety reasons, this has to be manually set to 'NULL'.
			png_byte** row_pointers = NULL;
			
			png_structp png_ptr;
			png_infop info_ptr;
			
			// Allocate the needed PNG data:
			
			// Attempt to allocate a "write-structure".
			png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			
			// Ensure we were able to allocate a "write-structure":
			if (png_ptr == NULL)
			{
				// Tell the user that we couldn't save to the output-stream.
				return false;
			}
			
			// Attempt to allocate an "info-structure".
			info_ptr = png_create_info_struct(png_ptr);
			
			// Ensure we were able to allocate an "info-structure":
			if (info_ptr == NULL)
			{
				// Since we were unable to create an "info-structure", we need to
				// destroy our already allocated "write-structure", then return 'false'.
				png_destroy_write_struct(&png_ptr, NULL);
				
				// Tell the user that we couldn't save to the output-stream.
				return false;
			}
			
			// Standard error handling:
			if (setjmp(png_jmpbuf(png_ptr)))
			{
				// Check if we should free the row-pointers:
				if (row_pointers != NULL)
					png_free(png_ptr, row_pointers);
				
				png_destroy_write_struct(&png_ptr, &info_ptr);
				
				// Tell the user that execution was not successful.
				return false;
			}
			
			// Assign the data specified to the "info-structure":
			png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth,
			color_type, interlace_type, compression_type, filter_type);
			
			// Allocate an array of pointers for libpng's 'png_set_rows' command.
			// This is allocated on the heap due to its variable size.
			row_pointers = (png_byte**)png_malloc(png_ptr, height * sizeof(png_byte*));
			
			// Initialize each row of the image:
			for (size_t y = 0; y < height; ++y)
			{
				//png_byte* row = (png_byte*)png_malloc(png_ptr, sizeof(png_byte) * width * sizeof(pixel));
				row_pointers[y] = (png_byte*)(imageData + ((width * y) * sizeof(pixel)));
				
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
			
			// Write and encode the image-data using the output-stream:
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
			
			// Free the main PNG-data.
			
			// Free the row-pointers.
			png_free(png_ptr, row_pointers);
			
			// Destroy the PNG data we initially retrieved:
			
			// This will destroy both the "write-structure", and the "info-structure".
			png_destroy_write_struct(&png_ptr, &info_ptr);
			
			//png_destroy_info_struct(png_ptr, &info_ptr);
			
			// Return the default response.
			return true;
		}
		
		// This acts as a standard file/disk I/O wrapper for the main 'save_to_stream' function.
		// This command is considered "unsafe" under specific situations;
		// please read the 'save_to_file_safe' command's documentation.
		bool save_to_file(const character* path, png_byte* imageData, size_t width, size_t height, int bit_depth=DEFAULT_IMAGE_DEPTH, int color_type=PNG_COLOR_TYPE_RGB_ALPHA, int interlace_type=PNG_INTERLACE_NONE, int compression_type=PNG_COMPRESSION_TYPE_DEFAULT, int filter_type=PNG_FILTER_TYPE_DEFAULT)
		{
			// Local variable(s):
			
			// This will act as our file-descriptor.
			FILE* fp;
			
			// Attempt to open a file-stream.
			fp = fopen(path, "wb");
			
			// Check if we could open a file-stream:
			if (!fp) // fp == NULL
				return false;
			
			// Execute the main routine.
			bool response = save_to_stream(fp, imageData, width, height, bit_depth,
			color_type, interlace_type, compression_type, filter_type);
			
			// Close the file descriptor.
			fclose(fp);
			
			// Return the calculated response.
			return response;
		}
		
		/*
			The following implementation is meant to be a "border-corssing-safe" version of 'save_to_file'.
			
			The problem with 'save_to_file' is that it uses standard C I/O,
			which is problematic when dealing with a DLL (With Visual Studio).
			
			This implementation is provided in order to delegate a "macro" of sorts
			provided by libpng, which provides limited encoding functionality.
			
			Visual C++ tends to hate dealing with the C standard
			library as far as DLLs go, so this is provided.
			
			Assuming you're on Windows, you're not using any custom encoding settings via Monkey, and
			'IMAGEENCODER_PNG_PREFER_SAFETY' is defined as 'True' by Monkey's preprocessor,
			this will act as the default implementation.
			
			Support for 'color_type' is more or less available.
		*/
		bool save_to_file_safe(const character* path, png_byte* imageData, png_uint_32 width, png_uint_32 height, int depth=DEFAULT_IMAGE_DEPTH, int color_type=PNG_COLOR_TYPE_RGB_ALPHA, png_int_32 internal__row_stride=0)
		{
			// Local variable(s):
			int format;
			
			// Attempt to re-encode the color-type specified into proper flags:
			switch (color_type)
			{
				case PNG_COLOR_TYPE_GRAY:
					format = PNG_FORMAT_GRAY;
					
					break;
				case PNG_COLOR_TYPE_GA:
					format = PNG_FORMAT_GA;
					
					break;
				case PNG_COLOR_TYPE_RGB:
					format = PNG_FORMAT_RGB;
					
					break;
				case PNG_COLOR_TYPE_RGBA:
					format = PNG_FORMAT_RGBA;
					
					break;
				default:
					return false;
					
					//break;
			}
			
			// This acts as the standard container used for image I/O.
			png_image img;
			
			// "Zero-out" the structure before doing anything else.
			memset(&img, 0, sizeof(img));
			
			img.width = width;
			img.height = height;
			img.format = format;
			img.version = PNG_IMAGE_VERSION;
			
			// Write and encode the image-data to the file-path specified, then return an appropriate response.
			return (png_image_write_to_file(&img, path, (int)(depth == 16),
			(const void*)imageData, internal__row_stride, NULL) != 0);
		}
		
		/*
			These act as the Monkey-wrappers for the main implementation
			of 'save_to_file_safe', which are needed in specific situations.
			
			These wrappers are only delegated when the 'IMAGEENCODER_PNG_PREFER_SAFETY'
			variable is defined as 'True' with Monkey's preprocessor.
		*/
		
		#if defined(CFG_IMAGEENCODER_PNG_PREFER_SAFETY)
			bool save_to_file_safe(String path, BBDataBuffer* imageData, int width, int height, int imageData_Offset_InBytes=0, int depth=DEFAULT_IMAGE_DEPTH, int color_type=PNG_COLOR_TYPE_RGB_ALPHA)
			{
				return save_to_file_safe(toCString(path), readPointer<png_byte>(imageData, (size_t)imageData_Offset_InBytes), (png_uint_32)width, (png_uint_32)height, depth, color_type);
			}
			
			#if defined(CFG_IMAGEENCODER_PNG_EXPERIMENTAL)
				bool save_to_file_safe(String path, Array<int> imageData, int width, int height, int imageData_Offset=0, int depth=DEFAULT_IMAGE_DEPTH, int color_type=PNG_COLOR_TYPE_RGB_ALPHA)
				{
					return save_to_file_safe(toCString(path), readPointer<png_byte, int>(imageData, (size_t)imageData_Offset), (png_uint_32)width, (png_uint_32)height, depth, color_type);
				}
			#endif
		#endif
		
		// This acts as the Monkey-wrapper for the main implementation of 'save_to_file':
		
		// The 'imageData_Offset_InBytes' argument is technically dependent on the size of 'png_byte'.
		bool save_to_file(String path, BBDataBuffer* imageData, int width, int height, int imageData_Offset_InBytes=0, int bit_depth=DEFAULT_IMAGE_DEPTH, int color_type=PNG_COLOR_TYPE_RGB_ALPHA, int interlace_type=PNG_INTERLACE_NONE, int compression_type=PNG_COMPRESSION_TYPE_DEFAULT, int filter_type=PNG_FILTER_TYPE_DEFAULT)
		{
			return save_to_file(toCString(path), readPointer<png_byte>(imageData, (size_t)imageData_Offset_InBytes), (size_t)width, (size_t)height, bit_depth, color_type, interlace_type, compression_type, filter_type);
		}
		
		#if defined(CFG_IMAGEENCODER_PNG_EXPERIMENTAL)
			// This acts as an experimental Monkey-wrapper for the main implementation of 'save_to_file':
			
			// This currently uses non-standard means of retrieving data.
			bool save_to_file(String path, Array<int> imageData, int width, int height, int imageData_Offset=0, int bit_depth=DEFAULT_IMAGE_DEPTH, int color_type=PNG_COLOR_TYPE_RGB_ALPHA, int interlace_type=PNG_INTERLACE_NONE, int compression_type=PNG_COMPRESSION_TYPE_DEFAULT, int filter_type=PNG_FILTER_TYPE_DEFAULT)
			{
				return save_to_file(toCString(path), readPointer<png_byte, int>(imageData, (size_t)imageData_Offset), (size_t)width, (size_t)height, bit_depth, color_type, interlace_type, compression_type, filter_type);
			}
		#endif
	}
}
