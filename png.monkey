Strict

Public

' Preprocessor related:
#If LANG = "cpp"
	#IMAGEENCODER_PNG_IMPLEMENTED = True
#End

' Imports (Monkey):
Import imageencoder

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
		#Elseif LANG = "cs"
			Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Width:Int, Height:Int, PixelData_Offset_InBytes:Int=0, Bit_Depth:Int=PNG_DEFAULT_IMAGE_DEPTH, Color_Type:Int=PNG_COLOR_TYPE_RGB_ALPHA, Interlace_Type:Int=PNG_INTERLACE_NONE, Compression_Type:Int=PNG_COMPRESSION_TYPE_DEFAULT, Filter_Type:Int=PNG_FILTER_TYPE_DEFAULT)="imageEncoder.PNG.save_to_file"
		#Else
			Function SavePNG:Bool(Path:String, PixelData:DataBuffer, Width:Int, Height:Int, PixelData_Offset_InBytes:Int=0, Bit_Depth:Int=PNG_DEFAULT_IMAGE_DEPTH, Color_Type:Int=PNG_COLOR_TYPE_RGB_ALPHA, Interlace_Type:Int=PNG_INTERLACE_NONE, Compression_Type:Int=PNG_COMPRESSION_TYPE_DEFAULT, Filter_Type:Int=PNG_FILTER_TYPE_DEFAULT)="PNG_ImageEncoder.save_to_file"
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
	#End
'#Else
	' Nothing so far.
#End