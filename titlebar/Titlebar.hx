package titlebar;

import titlebar.native.TitlebarUtility;

class Titlebar
{
	public static var buttonWidth(default, set):Int = 32;
	
	/**
		NOTE: this does NOT change the height of the titlebar, it's just used for calculating the positions of the title, buttons and icons
	**/
	public static var height(default, set):Int = 30;
	
	/**
		disables the button icons
	**/
	public static var useButtonText(default, set):Bool = true;
	
	public static var centerTitle(default, set):Bool = false;
	
	public static var initialized(default, null):Bool = false;
	
	// init functions
	
	public static inline function initialize():Void
	{
		if (initialized)
			return;
		TitlebarUtility.initialize();
		#if lime
		lime.app.Application.current.window.width ++;
		lime.app.Application.current.window.width --;
		#end
		initialized = true;
	}
	
	public static inline function registerFont(path:String):Void
		TitlebarUtility.registerFont(path);
		
	public static inline function redrawWindow():Void
		TitlebarUtility.redrawWindow();
		
	// colors
	
	public static inline function setTitlebarColor(red:Int, green:Int, blue:Int):Void
		TitlebarUtility.setTitleBarColor(red, green, blue);
		
	public static inline function setTitleFontColor(red:Int, green:Int, blue:Int):Void
		TitlebarUtility.setTitleFontColor(red, green, blue);
		
	public static inline function setButtonFontColor(red:Int, green:Int, blue:Int):Void
		TitlebarUtility.setButtonFontColor(red, green, blue);
		
	public static inline function setPrimaryButtonColor(red:Int, green:Int, blue:Int):Void
		TitlebarUtility.setPrimaryButtonColor(red, green, blue);
		
	public static inline function setSecondaryButtonColor(red:Int, green:Int, blue:Int):Void
		TitlebarUtility.setSecondaryButtonColor(red, green, blue);
		
	public static inline function setPrimaryButtonHoverColor(red:Int, green:Int, blue:Int):Void
		TitlebarUtility.setPrimaryButtonHoverColor(red, green, blue);
		
	public static inline function setSecondaryButtonHoverColor(red:Int, green:Int, blue:Int):Void
		TitlebarUtility.setSecondaryButtonHoverColor(red, green, blue);
		
	// images
	
	/**
		MUST use bit 24 .bmp file types
	**/
	public static inline function setTitlebarImage(imagePath:String):Void
		TitlebarUtility.setTitlebarImage(imagePath);
		
	public static inline function setPrimaryButtonImage(imagePath:String):Void // i do not recommend these button images because they will not stretch and will tile instead
		TitlebarUtility.setPrimaryButtonImage(imagePath);
		
	public static inline function setSecondaryButtonImage(imagePath:String):Void
		TitlebarUtility.setSecondaryButtonImage(imagePath);
		
	public static inline function setPrimaryButtonHoverImage(imagePath:String):Void
		TitlebarUtility.setPrimaryButtonHoverImage(imagePath);
		
	public static inline function setSecondaryButtonHoverImage(imagePath:String):Void
		TitlebarUtility.setSecondaryButtonHoverImage(imagePath);
		
	// fonts
	
	public static inline function setTitleFont(name:String, path:String, size:Int = 16):Void
	{
		registerFont(path);
		TitlebarUtility.setTitleFont(name, size);
	}
	
	public static inline function setButtonFont(name:String, path:String, size:Int = 10):Void
	{
		registerFont(path);
		TitlebarUtility.setButtonFont(name, size);
	}
	
	@:noCompletion
	private static inline function set_buttonWidth(width:Int):Int
	{
		TitlebarUtility.setButtonWidth(width);
		return buttonWidth = width;
	}
	
	@:noCompletion
	private static inline function set_height(height:Int):Int
	{
		TitlebarUtility.setTitleBarHeight(height);
		return Titlebar.height = height;
	}
	
	@:noCompletion
	private static inline function set_useButtonText(useButtonText:Bool):Bool
	{
		TitlebarUtility.setUseButtonText(useButtonText);
		return Titlebar.useButtonText = useButtonText;
	}
	
	@:noCompletion
	private static inline function set_centerTitle(centerTitle:Bool):Bool
	{
		TitlebarUtility.setCenterTitle(centerTitle);
		return Titlebar.centerTitle = centerTitle;
	}
}