#include "TitlebarUtility.hpp"

#define UNICODE
#define _UNICODE


// include stuff

#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <functional>
#include <hxcpp.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

// whatever this thing is
__declspec(dllexport) void titlebar__initializeNewWndProc();

extern "C"
{
    __declspec(dllexport) void titlebar__registerFontFromPath(String fontPath);
}

enum Titlebar__HoverType
{
    hoverOn_Nothing,
    hoverOn_Close,
    hoverOn_Maximize,
    hoverOn_Minimize
};

typedef struct Titlebar_ButtonRects
{
    RECT closeButton;
    RECT maximizeButton;
    RECT minimizeButton;
} Titlebar_ButtonRects;

// variables

Titlebar_ButtonRects titlebar__buttonRects;
Titlebar__HoverType titlebar__hoveringOn = hoverOn_Nothing;

WNDPROC titlebar__originalWndProc = nullptr;
HFONT titlebar__hTitleFont = nullptr;
HFONT titlebar__hButtonFont = nullptr;

HBRUSH titlebar__titleBarBrush = CreateSolidBrush(RGB(240, 240, 240));
COLORREF titlebar__titleFontColor = RGB(10, 0, 10);
COLORREF titlebar__buttonFontColor = RGB(10, 0, 10);

HBRUSH titlebar__primaryButtonBrush = nullptr;
HBRUSH titlebar__secondaryButtonBrush = nullptr;

HBRUSH titlebar__primaryButtonHoverBrush = CreateSolidBrush(RGB(55, 40, 42));
HBRUSH titlebar__secondaryButtonHoverBrush = CreateSolidBrush(RGB(223, 15, 16));

int titlebar__buttonWidth = 32;
int titlebar__titleBarHeight = 30;
bool titlebar__useButtonText = true;
bool titlebar__centerTitle = false;

bool titlebar__trackingMouseLeave = false;
bool titlebar__useBitmapFrame = false;

HBITMAP titlebar__bitmapFrame = nullptr;

bool initialized = false;

// functions

void titlebar__updateButtonRects(const RECT &clientRect, bool useTop = false)
{
    titlebar__buttonRects.closeButton = {clientRect.right - titlebar__buttonWidth, useTop ? clientRect.top : 0, clientRect.right, (useTop ? clientRect.top : 0) + titlebar__titleBarHeight};
    titlebar__buttonRects.maximizeButton = {clientRect.right - 2 * titlebar__buttonWidth, useTop ? clientRect.top : 0, clientRect.right - titlebar__buttonWidth, (useTop ? clientRect.top : 0) + titlebar__titleBarHeight};
    titlebar__buttonRects.minimizeButton = {clientRect.right - 3 * titlebar__buttonWidth, useTop ? clientRect.top : 0, clientRect.right - 2 * titlebar__buttonWidth, (useTop ? clientRect.top : 0) + titlebar__titleBarHeight};
}

void titlebar__drawButtons(HDC hdc, const RECT &clientRect, HWND hwnd)
{
    titlebar__updateButtonRects(clientRect);

    HFONT hOldFont;
    if (titlebar__useButtonText)
    {
        hOldFont = (HFONT)SelectObject(hdc, titlebar__hButtonFont);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, titlebar__buttonFontColor);
    }

    if (titlebar__hoveringOn == hoverOn_Close)
        FillRect(hdc, &titlebar__buttonRects.closeButton, titlebar__secondaryButtonHoverBrush);
    else if (titlebar__primaryButtonBrush != nullptr)
        FillRect(hdc, &titlebar__buttonRects.closeButton, titlebar__primaryButtonBrush);
    if (titlebar__hoveringOn == hoverOn_Maximize)
        FillRect(hdc, &titlebar__buttonRects.maximizeButton, titlebar__primaryButtonHoverBrush);
    else if (titlebar__secondaryButtonBrush != nullptr)
        FillRect(hdc, &titlebar__buttonRects.maximizeButton, titlebar__secondaryButtonBrush);
    if (titlebar__hoveringOn == hoverOn_Minimize)
        FillRect(hdc, &titlebar__buttonRects.minimizeButton, titlebar__primaryButtonHoverBrush);
    else if (titlebar__primaryButtonBrush != nullptr)
        FillRect(hdc, &titlebar__buttonRects.minimizeButton, titlebar__primaryButtonBrush);

    if (titlebar__useButtonText)
    {
        DrawTextW(hdc, L"\uE8BB", -1, &titlebar__buttonRects.closeButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        WINDOWPLACEMENT wp = {sizeof(WINDOWPLACEMENT)};
        GetWindowPlacement(hwnd, &wp);

        LPCWSTR maxIcon = wp.showCmd == SW_MAXIMIZE ? L"\uE922" : L"\uE923";
        DrawTextW(hdc, maxIcon, -1, &titlebar__buttonRects.maximizeButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        DrawTextW(hdc, L"\uE921", -1, &titlebar__buttonRects.minimizeButton, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        SelectObject(hdc, hOldFont);
    }
}

HICON titlebar__getCaptionIcon(HWND hwnd)
{
    HICON hIcon = (HICON)SendMessage(hwnd, WM_GETICON, ICON_BIG, 0);
    if (!hIcon)
        hIcon = (HICON)GetClassLongPtr(hwnd, GCLP_HICON);
    if (!hIcon)
        hIcon = (HICON)SendMessage(hwnd, WM_GETICON, ICON_SMALL, 0);
    if (!hIcon)
        hIcon = (HICON)GetClassLongPtr(hwnd, GCLP_HICONSM);
    return hIcon;
}

HBRUSH titlebar__createStretchedBrush(HDC hdc, HBITMAP hSourceBmp, int width, int height)
{
    HDC memDC = CreateCompatibleDC(hdc);
    HDC stretchDC = CreateCompatibleDC(hdc);

    HBITMAP oldSrcBmp = (HBITMAP)SelectObject(memDC, hSourceBmp);

    HBITMAP hStretchedBmp = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldDstBmp = (HBITMAP)SelectObject(stretchDC, hStretchedBmp);

    BITMAP bmp = {};
    GetObject(hSourceBmp, sizeof(BITMAP), &bmp);

    StretchBlt(stretchDC, 0, 0, width, height, memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY); // lifesaver

    SelectObject(memDC, oldSrcBmp);
    SelectObject(stretchDC, oldDstBmp);
    DeleteDC(memDC);
    DeleteDC(stretchDC);

    HBRUSH hBrush = CreatePatternBrush(hStretchedBmp);

    DeleteObject(hStretchedBmp);

    return hBrush;
}

// THE wnd proc 😼

LRESULT CALLBACK titlebar__wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    // handles drawing the frame, buttons etc
    case WM_NCPAINT:
    {
        HDC hdc = GetWindowDC(hwnd);

        RECT rect;
        GetWindowRect(hwnd, &rect);
        OffsetRect(&rect, -rect.left, -rect.top);

        if (titlebar__useBitmapFrame)
            titlebar__titleBarBrush = titlebar__createStretchedBrush(hdc, titlebar__bitmapFrame, rect.right, rect.bottom);
        FillRect(hdc, &rect, titlebar__titleBarBrush); // window frame

        int bufsize = GetWindowTextLength(hwnd) + 1;
        LPWSTR title = new WCHAR[bufsize];
        GetWindowText(hwnd, title, bufsize);

        SIZE textSize;
        HFONT hOldFont;
        if (titlebar__hTitleFont != nullptr)
            hOldFont = (HFONT)SelectObject(hdc, titlebar__hTitleFont);
        GetTextExtentPoint32(hdc, title, wcslen(title), &textSize);
        if (titlebar__hTitleFont != nullptr)
            SelectObject(hdc, hOldFont);

        HICON hIcon = titlebar__getCaptionIcon(hwnd);

        int iconSize = 26;

        UINT dpi = GetDpiForWindow(hwnd);
        iconSize = MulDiv(24, dpi, 96);

        float x = 10;
        if (titlebar__centerTitle)
        {
            int totalWidth = iconSize + 6 + textSize.cx;
            x = (rect.right - 3 * titlebar__buttonWidth - totalWidth) / 2;
        }
        DrawIconEx(hdc, x, (titlebar__titleBarHeight - iconSize) / 2, hIcon, iconSize, iconSize, 0, NULL, DI_NORMAL);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, titlebar__titleFontColor);
        RECT textRect = {(LONG)(x + iconSize + 6), 0, rect.right, (LONG)titlebar__titleBarHeight};

        if (titlebar__hTitleFont != nullptr)
            SelectObject(hdc, titlebar__hTitleFont);
        DrawTextW(hdc, title, -1, &textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        if (titlebar__hTitleFont != nullptr)
            SelectObject(hdc, hOldFont);

        titlebar__drawButtons(hdc, rect, hwnd);

        ReleaseDC(hwnd, hdc);
        return 0;
    }
    // handles button clicks
    case WM_NCLBUTTONUP:
    {
        switch (wParam)
        {
        case HTCLOSE:
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        case HTMAXBUTTON:
            SendMessage(hwnd, WM_SYSCOMMAND, IsZoomed(hwnd) ? SC_RESTORE : SC_MAXIMIZE, 0);
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
            break;
        case HTMINBUTTON:
            SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }
    case WM_NCLBUTTONDOWN:
    {
        switch (wParam)
        {
        case HTCLOSE:
            return 0;
        case HTMAXBUTTON:
            return 0;
        case HTMINBUTTON:
            return 0;
        }
        return CallWindowProc(titlebar__originalWndProc, hwnd, message, wParam, lParam);
    }
    case WM_NCHITTEST:
    {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ScreenToClient(hwnd, &pt);

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        titlebar__updateButtonRects(clientRect);

        switch (titlebar__hoveringOn)
        {
        case hoverOn_Close:
            return HTCLOSE;
        case hoverOn_Maximize:
            return HTMAXBUTTON;
        case hoverOn_Minimize:
            return HTMINBUTTON;
        }

        return CallWindowProc(titlebar__originalWndProc, hwnd, message, wParam, lParam);
    }
    case WM_NCMOUSEMOVE:
    {
        RECT clientRect;
        GetWindowRect(hwnd, &clientRect);

        POINT cursor_point = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        titlebar__updateButtonRects(clientRect, true);

        if (!titlebar__trackingMouseLeave)
        {
            TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT)};
            tme.dwFlags = TME_LEAVE | TME_NONCLIENT;
            tme.hwndTrack = hwnd;
            TrackMouseEvent(&tme);
            titlebar__trackingMouseLeave = true;
        }

        Titlebar__HoverType prevHover = titlebar__hoveringOn;

        if (PtInRect(&titlebar__buttonRects.closeButton, cursor_point))
            titlebar__hoveringOn = hoverOn_Close;
        else if (PtInRect(&titlebar__buttonRects.maximizeButton, cursor_point))
            titlebar__hoveringOn = hoverOn_Maximize;
        else if (PtInRect(&titlebar__buttonRects.minimizeButton, cursor_point))
            titlebar__hoveringOn = hoverOn_Minimize;
        else
            titlebar__hoveringOn = hoverOn_Nothing;

        if (titlebar__hoveringOn != prevHover)
        {
            RECT *rects[] = {&titlebar__buttonRects.closeButton, &titlebar__buttonRects.maximizeButton, &titlebar__buttonRects.minimizeButton};
            for (int i = 0; i < 3; ++i)
            {
                if (titlebar__hoveringOn == i || prevHover == i)
                    InvalidateRect(hwnd, rects[i], FALSE);
            }

            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_FRAME);
        }
        return 0;
    }
    case WM_NCMOUSELEAVE:
    {
        titlebar__trackingMouseLeave = false;
        if (titlebar__hoveringOn != hoverOn_Nothing)
        {
            titlebar__hoveringOn = hoverOn_Nothing;
            RECT *rects[] = {&titlebar__buttonRects.closeButton, &titlebar__buttonRects.maximizeButton, &titlebar__buttonRects.minimizeButton};
            for (int i = 0; i < 3; ++i)
                InvalidateRect(hwnd, rects[i], FALSE);
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_FRAME);
        }
        return 0;
    }
    // to handle redraws n stuff like that
    case WM_NCACTIVATE:
    {
        return 1;
    }
    case WM_SIZE:
    {
        LRESULT result = CallWindowProc(titlebar__originalWndProc, hwnd, message, wParam, lParam);
        SendMessage(hwnd, WM_NCPAINT, 0, 0);
        return result;
    }
    case WM_SETTEXT:
    {
        LRESULT result = CallWindowProc(titlebar__originalWndProc, hwnd, message, wParam, lParam);
        RedrawWindow(hwnd, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
        return result;
    }
    }

    return CallWindowProc(titlebar__originalWndProc, hwnd, message, wParam, lParam);
}

// initialization functions

void titlebar__initializeNewWndProc()
{
    if(initialized){
        return;
    }
    if (!titlebar__hButtonFont)
        titlebar__hButtonFont = CreateFontW(10, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                            DEFAULT_PITCH | FF_DONTCARE, L"Segoe MDL2 Assets");
    HWND hwnd = GetActiveWindow();
    titlebar__originalWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)titlebar__wndProc);

    MARGINS margins = {0};
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    initialized = true;
}

extern "C" __declspec(dllexport) void titlebar__registerFontFromPath(String fontPath)
{
    const wchar_t *path = reinterpret_cast<const wchar_t *>(fontPath.wc_str());
    AddFontResourceEx(path, FR_PRIVATE, 0);
}

// customization functions

void titlebar__setButtonWidth(int width)
{
    titlebar__buttonWidth = width;
}
void titlebar__setTitleBarHeight(int height)
{
    titlebar__titleBarHeight = height;
}
void titlebar__setUseButtonText(bool useButtonText)
{
    titlebar__useButtonText = useButtonText;
}
void titlebar__setTitlebarColor(int red, int green, int blue)
{
    titlebar__useBitmapFrame = false;
    DeleteObject(titlebar__titleBarBrush);
    titlebar__titleBarBrush = CreateSolidBrush(RGB(red, green, blue));
}
void titlebar__setTitleFontColor(int red, int green, int blue)
{
    titlebar__titleFontColor = RGB(red, green, blue);
}
void titlebar__setButtonFontColor(int red, int green, int blue)
{
    titlebar__buttonFontColor = RGB(red, green, blue);
}

void titlebar__setPrimaryButtonColor(int red, int green, int blue)
{
    DeleteObject(titlebar__primaryButtonBrush);
    titlebar__primaryButtonBrush = CreateSolidBrush(RGB(red, green, blue));
}
void titlebar__setSecondaryButtonColor(int red, int green, int blue)
{
    DeleteObject(titlebar__secondaryButtonBrush);
    titlebar__secondaryButtonBrush = CreateSolidBrush(RGB(red, green, blue));
}
void titlebar__setPrimaryButtonHoverColor(int red, int green, int blue)
{
    DeleteObject(titlebar__primaryButtonHoverBrush);
    titlebar__primaryButtonHoverBrush = CreateSolidBrush(RGB(red, green, blue));
}
void titlebar__setSecondaryButtonHoverColor(int red, int green, int blue)
{
    DeleteObject(titlebar__secondaryButtonHoverBrush);
    titlebar__secondaryButtonHoverBrush = CreateSolidBrush(RGB(red, green, blue));
}

void titlebar__setTitlebarImage(String imagePath)
{
    titlebar__useBitmapFrame = true;
    DeleteObject(titlebar__titleBarBrush);
    const wchar_t *path = reinterpret_cast<const wchar_t *>(imagePath.wc_str());
    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (hBitmap == NULL)
        titlebar__setTitlebarColor(255, 0, 0); // i've gotten a headache trying to figure out how to make this
    else
        titlebar__bitmapFrame = (HBITMAP)hBitmap;
}
void titlebar__setPrimaryButtonImage(String imagePath)
{
    DeleteObject(titlebar__primaryButtonBrush);
    const wchar_t *path = reinterpret_cast<const wchar_t *>(imagePath.wc_str());
    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    titlebar__primaryButtonBrush = CreatePatternBrush((HBITMAP)hBitmap);
    DeleteObject(hBitmap);
}
void titlebar__setSecondaryButtonImage(String imagePath)
{
    DeleteObject(titlebar__secondaryButtonBrush);
    const wchar_t *path = reinterpret_cast<const wchar_t *>(imagePath.wc_str());
    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    titlebar__secondaryButtonBrush = CreatePatternBrush((HBITMAP)hBitmap);
    DeleteObject(hBitmap);
}
void titlebar__setPrimaryButtonHoverImage(String imagePath)
{
    DeleteObject(titlebar__primaryButtonHoverBrush);
    const wchar_t *path = reinterpret_cast<const wchar_t *>(imagePath.wc_str());
    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    titlebar__primaryButtonHoverBrush = CreatePatternBrush((HBITMAP)hBitmap);
    DeleteObject(hBitmap);
}
void titlebar__setSecondaryButtonHoverImage(String imagePath)
{
    DeleteObject(titlebar__secondaryButtonHoverBrush);
    const wchar_t *path = reinterpret_cast<const wchar_t *>(imagePath.wc_str());
    HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    titlebar__secondaryButtonHoverBrush = CreatePatternBrush((HBITMAP)hBitmap);
    DeleteObject(hBitmap);
}

void titlebar__setTitleFont(String name, int size)
{
    titlebar__hTitleFont = CreateFontW(size, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                       DEFAULT_PITCH | FF_DONTCARE, reinterpret_cast<const wchar_t *>(name.wc_str()));
}

void titlebar__setButtonFont(String name, int size)
{
    titlebar__hButtonFont = CreateFontW(size, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                        DEFAULT_PITCH | FF_DONTCARE, reinterpret_cast<const wchar_t *>(name.wc_str()));
}

void titlebar__redrawWindow()
{
    RedrawWindow(GetActiveWindow(), NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_FRAME);
}

void titlebar__setCenterTitle(bool centerTitle)
{
    titlebar__centerTitle = centerTitle;
}
