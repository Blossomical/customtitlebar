#pragma once

#include <hxcpp.h>

extern "C"
{
#ifdef HX_WINDOWS
    __declspec(dllexport) void titlebar__initializeNewWndProc();
    __declspec(dllexport) void titlebar__registerFontFromPath(String fontPath);
#else
    void titlebar__initializeNewWndProc();
    void titlebar__registerFontFromPath(String fontPath);
#endif
}

void titlebar__initializeNewWndProc();
void titlebar__registerFontFromPath(String fontPath);

void titlebar__setButtonWidth(int width);
void titlebar__setTitleBarHeight(int height);
void titlebar__setUseButtonText(bool useButtonText);
void titlebar__setTitlebarColor(int red, int green, int blue);
void titlebar__setTitleFontColor(int red, int green, int blue);
void titlebar__setButtonFontColor(int red, int green, int blue);
void titlebar__setPrimaryButtonColor(int red, int green, int blue);
void titlebar__setSecondaryButtonColor(int red, int green, int blue);
void titlebar__setPrimaryButtonHoverColor(int red, int green, int blue);
void titlebar__setSecondaryButtonHoverColor(int red, int green, int blue);
void titlebar__setTitlebarImage(String imagePath);
void titlebar__setPrimaryButtonImage(String imagePath);
void titlebar__setSecondaryButtonImage(String imagePath);
void titlebar__setPrimaryButtonHoverImage(String imagePath);
void titlebar__setSecondaryButtonHoverImage(String imagePath);
void titlebar__setTitleFont(String name, int size = 16);
void titlebar__setButtonFont(String name, int size = 10);
void titlebar__redrawWindow();
void titlebar__setCenterTitle(bool centerTitle);