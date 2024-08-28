#include <windows.h>
#define TIMER_ID 1
int xPos = 50;
int yPos = 50;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
   
    //int y = 0;
    switch (msg) {
        case WM_CREATE: {
            SetTimer(hwnd, TIMER_ID, 2000, NULL);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
            RECT rect = { 50, 50, 200, 150 };
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_LBUTTONDOWN: {
            // Close the window on left mouse button click
            MessageBoxA(NULL,"clicked","T", MB_OK);
            
            DestroyWindow(hwnd);
           
            break;
        }
        case WM_TIMER: {
            //MessageBoxA(NULL, "RightClick", "T", MB_OK);
            if (wParam == TIMER_ID) {
                xPos = xPos + 10;
                SetWindowPos(hwnd, HWND_TOP, xPos, 50, 200, 150, SWP_NOZORDER | SWP_NOSIZE);
            }
            
            break;
        }
        case WM_DESTROY:
            KillTimer(hwnd, TIMER_ID);
            PostQuitMessage(0);//deletes Msg
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
//SetWindowPos(hwnd, HWND_TOP, x, 50, 200, 150, SWP_NOZORDER | SWP_NOSIZE);
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