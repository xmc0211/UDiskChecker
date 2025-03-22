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

#include "UDiskChecker.h"

#define TIMER 0

UDISK_CALLBACK_ARRIVAL __UDiskCheckUsedArrivalCallbackFunction = NULL;
UDISK_CALLBACK_REMOVE __UDiskCheckUsedRemoveCallbackFunction = NULL;
HWND __UDiskCheckUsedHWND = NULL;

LRESULT CALLBACK __UDiskCheckWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        SetTimer(hWnd, TIMER, 500, 0);
        break;
    }
    case WM_TIMER: {
        SendMessageA(hWnd, WM_DEVICECHANGE, 0, 0);
        break;
    }
    case WM_DEVICECHANGE: {
        if (wParam != DBT_DEVICEARRIVAL && wParam != DBT_DEVICEREMOVECOMPLETE) break;
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
        if (lpdb->dbch_devicetype != DBT_DEVTYP_VOLUME) break;
        PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
        ULONG Mask = lpdbv->dbcv_unitmask;
        CHAR Volume;
        for (Volume = 'A'; Volume <= 'Z'; ++Volume) {
            if (Mask & 0x1) break;
            Mask = Mask >> 1;
        }
        switch (wParam) {
        case DBT_DEVICEARRIVAL:
            if (__UDiskCheckUsedArrivalCallbackFunction != NULL)
                if (!__UDiskCheckUsedArrivalCallbackFunction(Volume)) DestroyWindow(hWnd);
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if (__UDiskCheckUsedRemoveCallbackFunction != NULL)
                if (!__UDiskCheckUsedRemoveCallbackFunction(Volume)) DestroyWindow(hWnd);
            break;
        }
        break;
    }
    case WM_DESTROY: {
        KillTimer(hWnd, TIMER);
        PostQuitMessage(0);
        break;
    }
    default: {
        return DefWindowProcA(hWnd, message, wParam, lParam);
    }
    }
    return 0;
}

DWORD CALLBACK __UDiskCheckWndMessage(LPVOID lpParam) {
    const CHAR szWindowClassName[] = "UDiskCheckerWnd";
    HWND hWnd;
    MSG Msg;
    WNDCLASS WindowClass;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = __UDiskCheckWndProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = hInstance;
    WindowClass.hIcon = 0;
    WindowClass.hCursor = 0;
    WindowClass.hbrBackground = 0;
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = szWindowClassName;

    if (!RegisterClassA(&WindowClass)) return FALSE;
    hWnd = CreateWindowExA(
        NULL, szWindowClassName, NULL, WS_DISABLED,
        0, 0, 0, 0,
        NULL, NULL, hInstance, 0
    );
    __UDiskCheckUsedHWND = hWnd;
    while (GetMessageA(&Msg, NULL, 0, 0)) {
        TranslateMessage(&Msg);
        DispatchMessageA(&Msg);
    }
    return 0;
}

BOOL UDiskCheckStartup(UDISK_CALLBACK_ARRIVAL cbFuncArrival, UDISK_CALLBACK_REMOVE cbFuncRemove) {
    __UDiskCheckUsedArrivalCallbackFunction = cbFuncArrival;
    __UDiskCheckUsedRemoveCallbackFunction = cbFuncRemove;
    CreateThread(NULL, 0, __UDiskCheckWndMessage, NULL, 0, NULL);
    while (__UDiskCheckUsedHWND == NULL) {
        Sleep(10);
    }
    return TRUE;
}

BOOL UDiskCheckQuit() {
    if (!DestroyWindow(__UDiskCheckUsedHWND)) return FALSE;
    __UDiskCheckUsedArrivalCallbackFunction = NULL;
    __UDiskCheckUsedRemoveCallbackFunction = NULL;
    __UDiskCheckUsedHWND = NULL;
    return TRUE;
}