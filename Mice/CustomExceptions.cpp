#include "CustomExceptions.h"

const char* BaseException::what() const throw()
{
    return message;
}
