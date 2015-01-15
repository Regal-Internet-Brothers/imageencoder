Strict

Public

#Rem
	DESCRIPTION:
		* This module provides image/bitmap encoding/compression functionality.
		
		The only format supported at this time is PNG.
		
		Support detection can be done by checking against
		the 'IMAGEENCODER_PNG_IMPLEMENTED' preprocessor variable.
		
		PNG support is only provided on C++ based targets currently.
		Support is also dependent on the "libpng" library
		supporting the platform in question; this may change in the future.
	NOTES:
		* This library uses a number of third-party image
		libraries for potentially several targets/languages.
		
		Please view the "README" file for details.
		
		* Support for MinGW/GCC is currently untested,
		but it should theoretically work.
		
		* Image encoding is currently done using 'DataBuffers'.
		
		* Supported languages/targets will have the
		'IMAGEENCODER_NATIVE' preprocessor variable defined.
		
		This may change in the future, as I may write support
		for simpler image formats directly through Monkey.
		
		To stay future proof, check against the 'IMAGEENCODER_IMPLEMENTED'
		preprocessor variable for non-native functionality.
		
		Explicit implementation flags are available
		for each supported image format.
		
		* If you absoultely must use arrays for encoding, then you'll
		have to offload that data onto a 'DataBuffer'.
		
		Direct support for arrays is currently unavailable.
		
		* PNG image-encoding only supports RGBA at this time.
	TODO:
		* Add further PNG compression options.
		* Add support for non-RGBA image formats.
		* Add image-encoding support for BMP files.
		* Add image-encoding support for JPEG files.
		* Add support for arrays (Optimized and unoptimized)
		
		* Add support for image decoding; already provided by 'opengl.gles11', currently.
		If I were to add this, this module's source code would likely be
		merged into a different module for both encoding and decoding.
		
		If such a module were to be made, I would also
		likely use the code from my 'imagedimensions' module.
		
		For now, use the 'opengl.gles11' module's 'LoadImageData' command.
#End

' Preprocessor related:
#IMAGEENCODER_IMPLEMENTED = True

' Libraries (GCC/MinGW)
#CC_LIBS += "-lpng"

' I'm pretty sure this is the correct library name.
#CC_LIBS += "-lz"

#If LANG = "cpp"
	#IMAGEENCODER_NATIVE = True
	#IMAGEENCODER_PNG_IMPLEMENTED = True
#End

' Imports:

' Standard 'DataBuffer' functionality is used for image encoding.
Import brl.databuffer

' Native code:
#If IMAGEENCODER_NATIVE
	Import "native/Encoder.${LANG}"
	
	#If IMAGEENCODER_PNG_IMPLEMENTED
		Import "native/PNG.${LANG}"
	#End
#End

' External bindings:

Extern

' Functions:
#If IMAGEENCODER_PNG_IMPLEMENTED
	#If LANG = "cpp"
		Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Width:Int, Height:Int)="imageEncoder::PNG::save_to_file"
	#Elseif LANG = "cs"
		Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Width:Int, Height:Int)="imageEncoder.PNG.save_to_file"
	#Else
		Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Width:Int, Height:Int)="PNG_ImageEncoder.save_to_file"
	#End
#End

Public

' Functions (Monkey):
#If IMAGEENCODER_PNG_IMPLEMENTED
	' This simply acts as a wrapper for the external version.
	' This is mainly useful for those who are using the 'LoadImageData' command.
	Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Size:Int[])
		Return SavePNG(Path, PixelData, Size[0], Size[1])
	End
#End