#include <windows.h>
#include <stdio.h>
int xPos = 50;
int yPos = 50;
boolean move = TRUE;
#define TIMER_ID 1
HDC hMemDC;         // Memory device context
HBITMAP hBitmap1;   // First bitmap
/*HBITMAP hBitmap2;*/   // Second bitmap
/*HBITMAP hCurrentBitmap;*/ // Currently selected bitmap
HBITMAP hOldBitmap; // Old bitmap in memory DC
int xx = 0;
int yy = 0;

boolean drag = FALSE;
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int desktopWidth = GetSystemMetrics(SM_CXSCREEN);

    int rand = 0;
    switch (uMsg) {
    case WM_CREATE:
    {
        HDC hdc2 = GetDC(hwnd); // Get the window device context
        hMemDC = CreateCompatibleDC(hdc2); // Create a compatible memory DC

        // Load and select bitmaps
        
        hBitmap1 = (HBITMAP)LoadImage(NULL,L"\\\\?\\C:\\Users\\chris\\OneDrive\\Pictures\\dogmap.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);// the \\?\ is used to bypass path limit of olderapis
        //hBitmap2 = (HBITMAP)LoadImage(NULL, L"\\\\?\\C:\\Users\\chris\\OneDrive\\Pictures\\dog_profileBMP2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);// the \\?\ is used to bypass path limit of olderapis

        // Select the first bitmap into the memory DC
        hOldBitmap = SelectObject(hMemDC, hBitmap1);

        ReleaseDC(hwnd, hdc2); // Release the window DC

        SetTimer(hwnd, TIMER_ID, 500, NULL); // Set up a timer
        break;
    }
    case WM_PAINT: // Triggered when the window needs to be repainted. This happens when the window is first shown, when it is resized, or when InvalidateRect is called.
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps); //BeginPaint: Prepares the window for painting and retrieves a device context (HDC).
  
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hMemDC, xx, yy, SRCCOPY);
   
        
        EndPaint(hwnd, &ps);
        
        break;
    }
    case WM_TIMER:
        if (wParam == TIMER_ID) {
            if (xPos < desktopWidth-100 &&move==TRUE) {
                xPos = xPos + 10;
            }
            
            SetWindowPos(hwnd, HWND_TOP, xPos, yPos, 100, 100, SWP_NOZORDER | SWP_NOSIZE);
           /* ClientToScreen(hwnd, &rect);
            MoveWindow(hwnd, xPos, yPos, 100, 100, TRUE);*/
            // Toggle between bitmaps
           
            if (xx == 0) {
               xx=xx+100;
            }
            else {
                xx = 0;
            }
            
            if (yPos % 2 ==0) {
                yy = 100;
                move = TRUE;
            }
            else if (yPos % 3 == 0) {
                yy = 200;
                move = FALSE;
                
            }
            else {
                yy = 0;
                move = FALSE;
             
            }
            
            // Trigger a repaint
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
    case WM_RBUTTONDOWN: {
        // Close the window on left mouse button click
        MessageBoxA(NULL,"clicked","T", MB_OK);
                    
        DestroyWindow(hwnd);
        /*GetWindowRect(hwnd, &rect);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Position:\nLeft: %ld\nTop: %ld\nRight: %ld\nBottom: %ld",
            rect.left, rect.top, rect.right, rect.bottom);
        MessageBoxA(hwnd, buffer, "Window Position", MB_OK | MB_ICONINFORMATION);*/
                   
        break;
    }
    case WM_LBUTTONDOWN:
        drag = TRUE;
        SetCapture(hwnd);
        break;
    case WM_LBUTTONUP:
        ReleaseCapture();
        drag = FALSE;
        break;
    case WM_MOUSEMOVE:
        //https://codingmisadventures.wordpress.com/2009/03/06/dragging-or-moving-a-window-using-mouse-win32/
        if (drag)
        {
            RECT mainWindowRect;
            POINT pos;
            int windowWidth, windowHeight;

            pos.x = (int)(short)LOWORD(lParam);
            pos.y = (int)(short)HIWORD(lParam);

            GetWindowRect(hwnd, &mainWindowRect);
            windowHeight = mainWindowRect.bottom - mainWindowRect.top;
            windowWidth = mainWindowRect.right - mainWindowRect.left;

            ClientToScreen(hwnd, &pos);
            MoveWindow(hwnd, pos.x, pos.y, windowWidth, windowHeight, TRUE);
            xPos = pos.x;
            yPos = pos.y;
        }
        break;
    case WM_DESTROY:
        // Clean up
        SelectObject(hMemDC, hOldBitmap); // Restore the old bitmap
        DeleteDC(hMemDC); // Delete the memory DC
        DeleteObject(hBitmap1);
      /*  DeleteObject(hBitmap2);*/
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
       50, 50, 100, 100,
       NULL,
       NULL,
       hInstance,
       NULL
   );
  
    //SetLayeredWindowAttributes(hwnd, 0, 200, LWA_ALPHA); // Semi-transparent
    SetLayeredWindowAttributes(hwnd, RGB(255,0,255), 255, LWA_COLORKEY);
 

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
 

    return (int)msg.wParam;
}