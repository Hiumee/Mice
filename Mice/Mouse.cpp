#include "Mouse.h"

Mouse::Mouse()
{
    leftClick = false;
    rightClick = false;
    middleClick = false;
    mouseX = 0;
    mouseY = 0;
    scrollWheel = 0;
    hMouse = NULL;
}

int Mouse::getX() const
{
    return mouseX;
}

int Mouse::getY() const
{
    return mouseY;
}

bool Mouse::leftClickDown() const
{
    return leftClick;
}

bool Mouse::rightClickDown() const
{
    return rightClick;
}

bool Mouse::middleClickDown() const
{
    return middleClick;
}

int Mouse::getScroll()
{
    int oldScroll = scrollWheel;
    scrollWheel = 0;
    return oldScroll;
}

void Mouse::setLocation(int x, int y)
{
    mouseX = x;
    mouseY = y;
}

void Mouse::setLeftClick(bool state)
{
    leftClick = state;
}

void Mouse::setMiddleClick(bool state)
{
    middleClick = state;
}

void Mouse::setRightClick(bool state)
{
    rightClick = state;
}

void Mouse::setScroll(int delta)
{
    scrollWheel += delta;
}

HANDLE Mouse::getHandle()
{
    return hMouse;
}

void Mouse::setHande(HANDLE hMouse)
{
    this->hMouse = hMouse;
}
