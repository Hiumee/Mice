#pragma once
#include <exception>

class BaseException : public std::exception
{
protected:
    const char* message;

public:
    const char* what() const throw ();
};