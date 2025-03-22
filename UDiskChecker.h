// MIT License
//
// Copyright (c) 2025 UDiskChecker - xmc0211 <xmc0211@qq.com>xmc0211
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef UCHECKER_H
#define UCHECKER_H

#include <Windows.h>
#include <string>
#include <Dbt.h>

// If the callback function returns False, stop checking.
typedef BOOL(CALLBACK* UDISK_CALLBACK_ARRIVAL)(CHAR cVolume);
typedef BOOL(CALLBACK* UDISK_CALLBACK_REMOVE)(CHAR cVolume);
typedef HWND UDISKCHECKHANDLE;

LRESULT CALLBACK __UDiskCheckWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
DWORD CALLBACK __UDiskCheckWndMessage(LPVOID lpParam);


// Start monitoring USB drive insertion (using the given callback function, which can be NULL)
BOOL UDiskCheckStartup(UDISK_CALLBACK_ARRIVAL cbFuncArrival, UDISK_CALLBACK_REMOVE cbFuncRemove);
// Stop monitoring USB drive insertion
BOOL UDiskCheckQuit();

#endif