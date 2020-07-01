#pragma once
#include "IWindows.h"

class Mouse
{
friend class Window;
private:
    // Down = true; Up = false
    HANDLE hMouse;
    bool leftClick;
    bool rightClick;
    bool middleClick;
    int mouseX;
    int mouseY;
    int scrollWheel;

public:
    Mouse();

    int getX() const;
    int getY() const;
    bool leftClickDown() const;
    bool rightClickDown() const;
    bool middleClickDown() const;
    int getScroll();

private:
    void setLocation(int x, int y);
    void setLeftClick(bool state);
    void setMiddleClick(bool state);
    void setRightClick(bool state);
    void setScroll(int delta);
    HANDLE getHandle();
    void setHande(HANDLE hMouse);
};