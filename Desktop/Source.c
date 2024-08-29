#include <windows.h>

int xPos = 50;
#define TIMER_ID 1
HDC hMemDC;         // Memory device context
HBITMAP hBitmap1;   // First bitmap
HBITMAP hBitmap2;   // Second bitmap
HBITMAP hCurrentBitmap; // Currently selected bitmap
HBITMAP hOldBitmap; // Old bitmap in memory DC

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
    {
        HDC hdc = GetDC(hwnd); // Get the window device context
        hMemDC = CreateCompatibleDC(hdc); // Create a compatible memory DC

        // Load and select bitmaps
        
        hBitmap1 = (HBITMAP)LoadImage(NULL,L"\\\\?\\C:\\Users\\chris\\OneDrive\\Pictures\\dog_profileBMP.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);// the \\?\ is used to bypass path limit of olderapis
        hBitmap2 = (HBITMAP)LoadImage(NULL, L"\\\\?\\C:\\Users\\chris\\OneDrive\\Pictures\\dog_profileBMP2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);// the \\?\ is used to bypass path limit of olderapis

        // Select the first bitmap into the memory DC
        hOldBitmap = SelectObject(hMemDC, hBitmap1);

        ReleaseDC(hwnd, hdc); // Release the window DC

        SetTimer(hwnd, TIMER_ID, 500, NULL); // Set up a timer
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Draw the current bitmap
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hMemDC, 0, 0, SRCCOPY);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_TIMER:
        if (wParam == TIMER_ID) {
            xPos = xPos + 10;
            SetWindowPos(hwnd, HWND_TOP, xPos, 50, 100, 100, SWP_NOZORDER | SWP_NOSIZE);
            // Toggle between bitmaps
            if (hCurrentBitmap == hBitmap1) {
                hCurrentBitmap = hBitmap2;
            }
            else {
                hCurrentBitmap = hBitmap1;
            }

            // Update memory DC with the new bitmap
            HDC hdc = GetDC(hwnd);
            SelectObject(hMemDC, hCurrentBitmap); // Select the new bitmap
            ReleaseDC(hwnd, hdc);

            // Trigger a repaint
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
    case WM_LBUTTONDOWN: {
        // Close the window on left mouse button click
        MessageBoxA(NULL,"clicked","T", MB_OK);
                    
        DestroyWindow(hwnd);
                   
        break;
    }
    case WM_DESTROY:
        // Clean up
        SelectObject(hMemDC, hOldBitmap); // Restore the old bitmap
        DeleteDC(hMemDC); // Delete the memory DC
        DeleteObject(hBitmap1);
        DeleteObject(hBitmap2);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
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

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED| WS_EX_TOOLWINDOW,//removing WS_EX_TRANSPARENT works
       "SimpleTransparentClass",
       "Transparent Window",
       WS_POPUP, //for a bordered window use: WS_OVERLAPPEDWINDOW,
       100, 100, 100, 100,
       NULL,
       NULL,
       hInstance,
       NULL
   );
  
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