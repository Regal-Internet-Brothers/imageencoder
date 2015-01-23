Strict

Public

' Imports (Monkey):
Import imageencoder

' Preprocessor related:
#If IMAGEENCODER_EXPERIMENTAL
	#IMAGEENCODER_PNG_EXPERIMENTAL = True
#End

#If LANG = "cpp"
	#IMAGEENCODER_PNG_IMPLEMENTED = True
	
	' Enable any C++ specific experimental functionality:
	#If IMAGEENCODER_PNG_EXPERIMENTAL
		' Enabling this will provide experimental functionality for dealing with arrays.
		#IMAGEENCODER_PNG_ARRAYS = True
	#End
#End

#If IMAGEENCODER_PNG_IMPLEMENTED
	' Libraries (GCC/MinGW):
	
	' I'm pretty sure these are the correct library names:
	#CC_LIBS += "-lpng"
	#CC_LIBS += "-lz"
	
	' Imports (Native):
	Import "native/PNG.${LANG}"
	
	' External bindings:
	Extern
	
	' Constant/Global variable(s):
	
	#Rem
		Due to Monkey's weird behavior with external constants,
		these have to be globals from Monkey's perspective;
		DO NOT modify these variables in any way.
		
		Such an attempt will result in undefined behavior,
		which will likely be caught by the target's compiler/other.
	#End
	
	Global PNG_DEFAULT_IMAGE_DEPTH:Int="imageEncoder::PNG::DEFAULT_IMAGE_DEPTH"
	
	' PNG Color types:
	Global PNG_COLOR_TYPE_GRAY:Int
	Global PNG_COLOR_TYPE_PALETTE:Int
	Global PNG_COLOR_TYPE_RGB:Int
	Global PNG_COLOR_TYPE_RGB_ALPHA:Int
	Global PNG_COLOR_TYPE_GRAY_ALPHA:Int
	
	' Color Aliases:
	Global PNG_COLOR_TYPE_RGBA:Int ' = "PNG_COLOR_TYPE_RGB_ALPHA"
	Global PNG_COLOR_TYPE_GA:Int ' = "PNG_COLOR_TYPE_GRAY_ALPHA"
	
	' PNG Compression types:
	Global PNG_COMPRESSION_TYPE_BASE:Int
	Global PNG_COMPRESSION_TYPE_DEFAULT:Int ' = "PNG_COMPRESSION_TYPE_BASE"
	
	' PNG Filtering types:
	Global PNG_FILTER_TYPE_BASE:Int
	Global PNG_INTRAPIXEL_DIFFERENCING:Int
	Global PNG_FILTER_TYPE_DEFAULT:Int ' = "PNG_FILTER_TYPE_BASE"
	
	' PNG Interlacing types:
	Global PNG_INTERLACE_NONE:Int
	Global PNG_INTERLACE_ADAM7:Int
	
	' This is not a valid interlace-type.
	Global PNG_INTERLACE_LAST:Int
	
	' Functions:
	#If IMAGEENCODER_PNG_IMPLEMENTED
		#If LANG = "cpp"
			Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Width:Int, Height:Int, PixelData_Offset_InBytes:Int=0, Bit_Depth:Int=PNG_DEFAULT_IMAGE_DEPTH, Color_Type:Int=PNG_COLOR_TYPE_RGB_ALPHA, Interlace_Type:Int=PNG_INTERLACE_NONE, Compression_Type:Int=PNG_COMPRESSION_TYPE_DEFAULT, Filter_Type:Int=PNG_FILTER_TYPE_DEFAULT)="imageEncoder::PNG::save_to_file"
			
			#Rem
				The data specified to this overload must be in an encoded form,
				just as the main implementation which takes a 'DataBuffer'.
				
				Data-encoding must be split between the integers used.
				This means that if the color-type is less than 32-bit,
				the next pixel has to start within the same integer as
				the previous one.
				
				As an example, if you were to use 24-bit pixels,
				then the first pixel would need to be held in the first element of the array,
				but the second would need to start at that same index, only 24 bits over,
				then continuing on to take up 16 bits of the next element in the array.
				
				This command is currently experimental, and may cause problems.
				
				Monkey does not currently support proper memory manipulation
				with arrays, so support may be dropped at any point.
			#End
			
			#If IMAGEENCODER_PNG_ARRAYS
				#Rem
					The 'PixelData_Offset' argument does not work like the 'PixelData_Offset_InBytes'
					argument that's provided by the main implementation.
					This offset is used to designate the start position in the array,
					meaning it will move over the 'PixelData_Offset' integers
					(Multiply by four to get the number of bytes; assuming 32-bit integers, of course).
				#End
				
				Function SavePNG:Bool(Path:String, PixelData:Int[], Width:Int, Height:Int, PixelData_Offset:Int=0, Bit_Depth:Int=PNG_DEFAULT_IMAGE_DEPTH, Color_Type:Int=PNG_COLOR_TYPE_RGB_ALPHA, Interlace_Type:Int=PNG_INTERLACE_NONE, Compression_Type:Int=PNG_COMPRESSION_TYPE_DEFAULT, Filter_Type:Int=PNG_FILTER_TYPE_DEFAULT)="imageEncoder::PNG::save_to_file"
			#End
		'#Elseif LANG = "cs"
			'Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Width:Int, Height:Int, PixelData_Offset_InBytes:Int=0, Bit_Depth:Int=PNG_DEFAULT_IMAGE_DEPTH, Color_Type:Int=PNG_COLOR_TYPE_RGB_ALPHA, Interlace_Type:Int=PNG_INTERLACE_NONE, Compression_Type:Int=PNG_COMPRESSION_TYPE_DEFAULT, Filter_Type:Int=PNG_FILTER_TYPE_DEFAULT)="imageEncoder.PNG.save_to_file"
		'#Else
			'Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Width:Int, Height:Int, PixelData_Offset_InBytes:Int=0, Bit_Depth:Int=PNG_DEFAULT_IMAGE_DEPTH, Color_Type:Int=PNG_COLOR_TYPE_RGB_ALPHA, Interlace_Type:Int=PNG_INTERLACE_NONE, Compression_Type:Int=PNG_COMPRESSION_TYPE_DEFAULT, Filter_Type:Int=PNG_FILTER_TYPE_DEFAULT)="PNG_ImageEncoder.save_to_file"
		#End
	#End
	
	Public
	
	' Functions (Monkey):
	#If IMAGEENCODER_PNG_IMPLEMENTED
		' This simply acts as a wrapper for the external version.
		' This is mainly useful for those who are using the 'LoadImageData' command.
		Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Size:Int[], PixelData_Offset_InBytes:Int=0, Bit_Depth:Int=PNG_DEFAULT_IMAGE_DEPTH, Color_Type:Int=PNG_COLOR_TYPE_RGB_ALPHA, Interlace_Type:Int=PNG_INTERLACE_NONE, Compression_Type:Int=PNG_COMPRESSION_TYPE_DEFAULT, Filter_Type:Int=PNG_FILTER_TYPE_DEFAULT)
			Return SavePNG(Path, PixelData, Size[0], Size[1], PixelData_Offset_InBytes, Bit_Depth, Color_Type, Interlace_Type, Compression_Type, Filter_Type)
		End
		
		#If IMAGEENCODER_PNG_ARRAYS
			' This simply acts as a wrapper for the external version.
			Function SavePNG:Bool(Path:String, PixelData:Int[], Size:Int[], PixelData_Offset_InBytes:Int=0, Bit_Depth:Int=PNG_DEFAULT_IMAGE_DEPTH, Color_Type:Int=PNG_COLOR_TYPE_RGB_ALPHA, Interlace_Type:Int=PNG_INTERLACE_NONE, Compression_Type:Int=PNG_COMPRESSION_TYPE_DEFAULT, Filter_Type:Int=PNG_FILTER_TYPE_DEFAULT)
				Return SavePNG(Path, PixelData, Size[0], Size[1], PixelData_Offset_InBytes, Bit_Depth, Color_Type, Interlace_Type, Compression_Type, Filter_Type)
			End
		#End
	#End
'#Else
	' Nothing so far.
#End