#pragma once
#include "IWindows.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>
#include <string>
#include "CustomExceptions.h"

class WindowException : public BaseException
{
public:
    WindowException(const char* message) { message = message; };
};

class Window
{
private:
    HWND hWindow;
    Mouse mouse1;
    Mouse mouse2;
    std::unique_ptr<Graphics> pGraphics;

public:
    Window(int length, int height, std::string title, HINSTANCE hInstance);
    Window operator=(Window&) = delete;
    ~Window();

    void setTitle(std::string newTitle);
    Mouse& getMouse1();
    Mouse& getMouse2();
    Graphics& getGraphics();

private:
    static LRESULT WINAPI StartupWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT WINAPI RedirectingWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT RealWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};