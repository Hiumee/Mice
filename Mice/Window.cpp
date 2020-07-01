#include "Window.h"

Window::Window(int length, int height, std::string title, HINSTANCE hInstance)
{
    // Register the window class
    const char* pClassName = title.c_str();

    WNDCLASSEX wClass = { 0 };
    wClass.cbSize = sizeof(wClass);
    wClass.style = CS_OWNDC;
    wClass.lpfnWndProc = this->StartupWindowProc;
    wClass.cbClsExtra = 0;
    wClass.cbWndExtra = sizeof(this);
    wClass.hInstance = hInstance;
    wClass.hIcon = nullptr;
    wClass.hCursor = nullptr;
    wClass.hbrBackground = nullptr;
    wClass.lpszMenuName = nullptr;
    wClass.lpszClassName = pClassName;
    wClass.hIconSm = nullptr;

    if (RegisterClassEx(&wClass) == 0)
    {
        throw WindowException("Could now reigster window class");
    }

    // Calculate window size
    RECT areaSize;
    areaSize.left = 200;
    areaSize.right = 200 + length;
    areaSize.top = 200;
    areaSize.bottom = 200 + height;
    AdjustWindowRectEx(&areaSize, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE, FALSE, 0);

    // Creating the window
    this->hWindow = CreateWindowEx(
        0, pClassName, pClassName,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
        200, 200, areaSize.right - areaSize.left, areaSize.bottom - areaSize.top,
        nullptr, nullptr,
        hInstance,
        this
    );

    if (hWindow == nullptr)
    {
        throw WindowException("Could not create window");
    }

    // Register raw input for mouse

    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01; // mouse page
    rid.usUsage = 0x02; // mouse usage
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;

    if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
    {
        throw WindowException("Could not register mouse control");
    }

    //pGraphics = std::make_unique<Graphics>(hWindow);
}

Window::~Window()
{
    DestroyWindow(hWindow);
}

void Window::setTitle(std::string newTitle)
{
    SetWindowText(hWindow, newTitle.c_str());
}

Mouse& Window::getMouse1()
{
    return mouse1;
}

Mouse& Window::getMouse2()
{
    return mouse2;
}

Graphics& Window::getGraphics()
{
    return *pGraphics;
}

LRESULT __stdcall Window::StartupWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE)
    {
        CREATESTRUCT* cStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* window = reinterpret_cast<Window*>(cStruct->lpCreateParams);
        SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONGLONG>(window));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONGLONG>(RedirectingWindowProc));
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT __stdcall Window::RedirectingWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, 0));
    return window->RealWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::RealWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        OutputDebugString("Closing\n");
        PostQuitMessage(0);
        return 0;
        /********* START MOUSE MESSAGES **************/
    case WM_INPUT:
        UINT cbSize;
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &cbSize, sizeof(RAWINPUTHEADER));
        LPBYTE lpb = new BYTE[cbSize];
        if (lpb == nullptr)
        {
            return 0;
        }

        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &cbSize, sizeof(RAWINPUTHEADER));

        RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

        if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            // Detect mouse
            HANDLE mouse1Handle = mouse1.getHandle();
            HANDLE mouse2Handle = mouse2.getHandle();

            if (mouse1Handle == NULL)
            {
                mouse1.setHande(raw->header.hDevice);
            }
            else if (mouse2Handle == NULL)
            {
                mouse2.setHande(raw->header.hDevice);
            }

            Mouse* currentMouse;

            if (mouse1Handle == raw->header.hDevice)
            {
                currentMouse = &mouse1;
                OutputDebugString("Mouse 1\n");
            }
            else
            {
                currentMouse = &mouse2;
                OutputDebugString("Mouse 2\n");
            }

            // Set mouse attributes
            auto flags = raw->data.mouse.usButtonFlags;
            if (flags & RI_MOUSE_LEFT_BUTTON_DOWN)
            {
                currentMouse->setLeftClick(true);
            }
            else if (flags & RI_MOUSE_LEFT_BUTTON_UP)
            {
                currentMouse->setLeftClick(false);
            }

            if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
            {
                currentMouse->setMiddleClick(true);
            }
            else if (flags & RI_MOUSE_MIDDLE_BUTTON_UP)
            {
                currentMouse->setMiddleClick(false);
            }

            if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN)
            {
                currentMouse->setRightClick(true);
            }
            else if (flags & RI_MOUSE_RIGHT_BUTTON_UP)
            {
                currentMouse->setRightClick(false);
            }

            currentMouse->setLocation(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
        }

        delete[] lpb;
        return 0;
        /********* END MOUSE MESSAGES ****************/

    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
