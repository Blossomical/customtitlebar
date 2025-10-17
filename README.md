# Custom Title Bar
Really simple window title bar / window frame customization tool for Windows target on HXCPP and HashLink

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

# USING setTitleFont:
To get the font's name for the first argument of the function `setTitleFont` you need to open your font file, and grab the name from there, it's found in the top left corner
<img width="298" height="62" alt="image" src="https://github.com/user-attachments/assets/bb502d5d-80f6-49cd-80f2-ddeaecca367d" />

# HashLink Setup
(written by someguywholovescoding)

If you've installed this library, run `haxelib run hxcpp build_hl.xml -D HASHLINK="your\hashlink\path"` at the same path of the titlebar utility class file. Then, drag the hdll over to your export bin.


And after that, you move your hdll over to your export/hl/bin folder of your app.