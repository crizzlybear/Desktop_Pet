#include <windows.h>


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Red color
            RECT rect = {50, 50, 200, 150};
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            EndPaint(hwnd, &ps);
            /*Sleep(2000);
            DestroyWindow(hwnd);*/ //auto delete
          
            break;
        }
        case WM_LBUTTONDOWN: {
            // Close the window on left mouse button click
            MessageBoxA(NULL,"clicked","T", MB_OK);
            DestroyWindow(hwnd);
           
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SimpleTransparentClass";
    RegisterClass(&wc);

    /*HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT,
        "SimpleTransparentClass",
        "Transparent Window",
        WS_POPUP,
        100, 100, 400, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );*/
    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED| WS_EX_TOOLWINDOW,//removing WS_EX_TRANSPARENT works
       "SimpleTransparentClass",
       "Transparent Window",
       WS_POPUP, //for a bordered window use: WS_OVERLAPPEDWINDOW,
       100, 100, 400, 300,
       NULL,
       NULL,
       hInstance,
       NULL
   );
    // Set a region for the window to handle clicks
   /* HRGN hrgn = CreateRectRgn(0, 0, 400, 300);
    SetWindowRgn(hwnd, hrgn, TRUE);
    DeleteObject(hrgn);*/

    //SetLayeredWindowAttributes(hwnd, 0, 200, LWA_ALPHA); // Semi-transparent
    SetLayeredWindowAttributes(hwnd, 0, 150, LWA_ALPHA);
 

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
 

    return (int)msg.wParam;
}