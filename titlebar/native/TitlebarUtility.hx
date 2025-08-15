package titlebar.native;

import cpp.Int64;

#if windows
@:include("./TitlebarUtility.cpp")
extern class TitlebarUtility {
    @:native('titlebar__initializeNewWndProc') public static function initialize():Void;
    @:native('titlebar__registerFontFromPath') public static function registerFont(path:String):Void;

    // customization

    @:native('titlebar__setButtonWidth') public static function setButtonWidth(width:Int64):Void;
    @:native('titlebar__setTitleBarHeight') public static function setTitleBarHeight(height:Int64):Void;
    @:native('titlebar__setUseButtonText') public static function setUseButtonText(useButtonText:Bool):Void;
    @:native('titlebar__setTitlebarColor') public static function setTitleBarColor(red:Int64, green:Int64, blue:Int64):Void;

    @:native('titlebar__setTitleFontColor') public static function setTitleFontColor(red:Int64, green:Int64, blue:Int64):Void;
    @:native('titlebar__setButtonFontColor') public static function setButtonFontColor(red:Int64, green:Int64, blue:Int64):Void;
    @:native('titlebar__setPrimaryButtonColor') public static function setPrimaryButtonColor(red:Int64, green:Int64, blue:Int64):Void;
    @:native('titlebar__setSecondaryButtonColor') public static function setSecondaryButtonColor(red:Int64, green:Int64, blue:Int64):Void;
    @:native('titlebar__setPrimaryButtonHoverColor') public static function setPrimaryButtonHoverColor(red:Int64, green:Int64, blue:Int64):Void;
    @:native('titlebar__setSecondaryButtonHoverColor') public static function setSecondaryButtonHoverColor(red:Int64, green:Int64, blue:Int64):Void;
    @:native('titlebar__setTitlebarImage') public static function setTitlebarImage(imagePath:String):Void;
    @:native('titlebar__setPrimaryButtonImage') public static function setPrimaryButtonImage(imagePath:String):Void;
    @:native('titlebar__setSecondaryButtonImage') public static function setSecondaryButtonImage(imagePath:String):Void;
    @:native('titlebar__setPrimaryButtonHoverImage') public static function setPrimaryButtonHoverImage(imagePath:String):Void;
    @:native('titlebar__setSecondaryButtonHoverImage') public static function setSecondaryButtonHoverImage(imagePath:String):Void;
    @:native('titlebar__setTitleFont') public static function setTitleFont(name:String, size:Int64 = 16):Void;
    @:native('titlebar__setButtonFont') public static function setButtonFont(name:String, size:Int64 = 10):Void;

    @:native('titlebar__redrawWindow') public static function redrawWindow():Void;
    @:native('titlebar__setCenterTitle') public static function setCenterTitle(centerTitle:Bool):Void;
}
#end
