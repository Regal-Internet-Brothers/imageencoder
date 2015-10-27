Strict

Public

' Preprocessor related:
' If this is enabled, external (Likely "unsafe") functionality will
' be delegated to the user; could cause problems with reflection.

'#IMAGEENCODER_FORCE_EXTERNAL_PIXELS = True

#If LANG = "cpp"
	#IMAGEENCODER_NATIVE = True
#End

' Imports (Monkey):
Import imageencoder ' regal.imageencoder

' Internal (Native):
#If IMAGEENCODER_NATIVE
	Import "native/Encoder.${LANG}"
#End

' External bindings:

' Unfortunately, reflection can have problems with these bindings:
#If IMAGEENCODER_NATIVE And ((Not REFLECTION_FILTER) Or IMAGEENCODER_FORCE_EXTERNAL_PIXELS)
	Extern
	
	' Classes / Structures:
	
	' Unlike the native code, this module provides well-defined
	' pixel-functionality based on exact names, not generic names.
	Class @Pixel_RGBA Extends Null = "pixel_RGBA" ' "pixel"
		' Fields:
		Field Red:Int ' Byte
		Field Green:Int ' Byte
		Field Blue:Int ' Byte
		Field Alpha:Int ' Byte
	End
	
	#Rem
		This "alias" provides a manor with which to abstract the format used internally.
		Usage of this alias can be done with several commands provided with this module,
		but such actions are considered non-standard at this time.
	#End
	
	#Rem
		Class @Pixel Extends Null = "pixel"
		End
	#End
	
	' Functions:
	#If LANG = "cpp"
		#Rem
			The following commands are considered non-standard, and language-dependent.
			These do not have fall-backs for unsupported languages.
			
			Use of these commands is "unsafe", and may result in improper use of memory.
			Use at your own risk; if unsure, don't bother:
		#End
		
		' This command will provide 
		Function GetPixel_RGBA:Pixel_RGBA(Data:DataBuffer, Width:Int, X:Int, Y:Int, Offset_InBytes:Int=0)="imageEncoder::pixel_at"
		
		' This command will "encode" a pixel into a raw integer.
		Function Pixel_RGBA_ToInt:Int(Pixel:Pixel_RGBA)="imageEncoder::pixel_toInt"
	#End
	
	Public
#End