package titlebar.native;


@:allow(titlebar.Titlebar)
@:include("TitlebarUtility.hpp")
@:buildXml("<include name='${haxelib:customtitlebar}/titlebar/native/build.xml' />")
extern class TitlebarUtility
{
	@:native('titlebar__initializeNewWndProc') private static function initialize():Void;
	@:native('titlebar__registerFontFromPath') private static function registerFont(path:String):Void;
	
	// customization
	@:native('titlebar__setButtonWidth') private static function setButtonWidth(width:Int):Void;
	@:native('titlebar__setTitleBarHeight') private static function setTitleBarHeight(height:Int):Void;
	@:native('titlebar__setUseButtonText') private static function setUseButtonText(useButtonText:Bool):Void;
	@:native('titlebar__setTitlebarColor') private static function setTitleBarColor(red:Int, green:Int, blue:Int):Void;
	
	@:native('titlebar__setTitleFontColor') private static function setTitleFontColor(red:Int, green:Int, blue:Int):Void;
	@:native('titlebar__setButtonFontColor') private static function setButtonFontColor(red:Int, green:Int, blue:Int):Void;
	@:native('titlebar__setPrimaryButtonColor') private static function setPrimaryButtonColor(red:Int, green:Int, blue:Int):Void;
	@:native('titlebar__setSecondaryButtonColor') private static function setSecondaryButtonColor(red:Int, green:Int, blue:Int):Void;
	@:native('titlebar__setPrimaryButtonHoverColor') private static function setPrimaryButtonHoverColor(red:Int, green:Int, blue:Int):Void;
	@:native('titlebar__setSecondaryButtonHoverColor') private static function setSecondaryButtonHoverColor(red:Int, green:Int, blue:Int):Void;
	@:native('titlebar__setTitlebarImage') private static function setTitlebarImage(imagePath:String):Void;
	@:native('titlebar__setPrimaryButtonImage') private static function setPrimaryButtonImage(imagePath:String):Void;
	@:native('titlebar__setSecondaryButtonImage') private static function setSecondaryButtonImage(imagePath:String):Void;
	@:native('titlebar__setPrimaryButtonHoverImage') private static function setPrimaryButtonHoverImage(imagePath:String):Void;
	@:native('titlebar__setSecondaryButtonHoverImage') private static function setSecondaryButtonHoverImage(imagePath:String):Void;
	@:native('titlebar__setTitleFont') private static function setTitleFont(name:String, size:Int = 16):Void;
	@:native('titlebar__setButtonFont') private static function setButtonFont(name:String, size:Int = 10):Void;
	
	@:native('titlebar__redrawWindow') private static function redrawWindow():Void;
	@:native('titlebar__setCenterTitle') private static function setCenterTitle(centerTitle:Bool):Void;
}