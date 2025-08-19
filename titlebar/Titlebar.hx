package titlebar;

#if windows
import titlebar.native.TitlebarUtility;

class Titlebar
{
	public static var buttonWidth(default, set):Int = 32;

	/**
		dimensions of the titlebar
	**/
	public static var height(default, set):Int = 30;

	/**
		size of the icon (gets scaled by DPI)
	**/
	public static var iconSize(default, set):Int = 24;

	/**
		disables the button icons
	**/
	public static var useButtonText(default, set):Bool = true;

	public static var centerTitle(default, set):Bool = false;

	public static var initialized(default, null):Bool = false;
	public static var gdiLoaded(default, null):Bool = false;

	// init functions

	public static inline function initialize():Void
	{
		if (initialized)
			return;
		TitlebarUtility.initialize();
		#if lime
		lime.app.Application.current.window.width++;
		lime.app.Application.current.window.width--;
		#end
		initialized = true;
	}

	public static inline function registerFont(path:String):Void
		TitlebarUtility.registerFont(path);

	public static inline function redrawWindow():Void
		TitlebarUtility.redrawWindow();

	public static inline function loadGDI():Void
	{
		if (gdiLoaded)
			return;
		TitlebarUtility.loadGDI();
		gdiLoaded = true;
	}

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

	public static inline function setTitlebarImage(imagePath:String):Void
	{
		loadGDI();
		TitlebarUtility.setTitlebarImage(imagePath);
	}

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

	public static inline function setFrameDimensions(?left:Int, ?top:Int, ?right:Int, ?bottom:Int):Void // top is titlebar height!
		TitlebarUtility.setFrameDimensions(left, top, right, bottom);

	public static inline function setZoomedFrameDimensions(?left:Int, ?top:Int, ?right:Int, ?bottom:Int):Void
		TitlebarUtility.setZoomedFrameDimensions(left, top, right, bottom);

	public static inline function setFrameMargins(left:Int, ?top:Int, ?right:Int, ?bottom:Int):Void
		TitlebarUtility.setFrameMargins(left, top, right, bottom);

	@:noCompletion
	private static inline function set_buttonWidth(width:Int):Int
	{
		TitlebarUtility.setButtonWidth(width);
		return buttonWidth = width;
	}

	@:noCompletion
	private static inline function set_height(height:Int):Int
	{
		TitlebarUtility.setFrameDimensions(0, height, 0, 0);
		Titlebar.setZoomedFrameDimensions(0, height, 0, 0);
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

	@:noCompletion
	private static inline function set_iconSize(size:Int):Int
	{
		TitlebarUtility.setIconSize(size);
		return Titlebar.iconSize = size;
	}
}
#end