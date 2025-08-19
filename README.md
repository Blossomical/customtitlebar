# Custom Title Bar
Really simple window title bar / window frame customization tool for Windows target on HXCPP and HashKink

# EXAMPLE USAGE:
```hx
Titlebar.setTitlebarColor(50, 20, 80);
Titlebar.setTitleFontColor(200, 180, 240);
Titlebar.setButtonFontColor(20, 10, 30);
Titlebar.setTitleFont("Pixel Arial 11", 'assets/fonts/pixel-latin.ttf', 16);
Titlebar.initialize();
```

```hx
Titlebar.setTitlebarImage('assets/images/pattern.bmp');
Titlebar.setTitleFontColor(200, 180, 240);
Titlebar.setButtonFontColor(20, 10, 30);
Titlebar.setTitleFont("Pixel Arial 11", 'assets/fonts/pixel-latin.ttf', 16);
Titlebar.initialize();
```

# HashLink Setup
(written by someguywholovescoding)

If you've installed this library, run `haxelib run hxcpp build_hl.xml _D "(your hashlink path)"` at the same path of the titlebar utility class file.

And after that, you move your hdll over to your export/hl/bin folder of your app.