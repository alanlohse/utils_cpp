/*
 * exception_w32.cpp
 *
 *  Created on: 17/04/2011
 *      Author: Alan
 */

#include <utils/exception.hpp>
#include <windows.h>

namespace utils {


std::string get_error_message(int err)
{
    HLOCAL hLoc = NULL;

    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_ALLOCATE_BUFFER,
                  NULL,
                  err,
                  0,
                  (LPSTR)&hLoc,
                  0,
                  NULL);

    std::string result = std::string((const char*)LocalLock(hLoc));
    LocalFree(hLoc);
    return result;
}


}  // namespace utils
