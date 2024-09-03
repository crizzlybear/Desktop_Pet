#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int xPos = 50;
int yPos = 50;
boolean move = FALSE;
#define TIMER_ID 1
#define TIMER_ID2 2

HDC hMemDC;         // Memory device context
HBITMAP hBitmap1;   // First bitmap
/*HBITMAP hBitmap2;*/   // Second bitmap
/*HBITMAP hCurrentBitmap;*/ // Currently selected bitmap
HBITMAP hOldBitmap; // Old bitmap in memory DC
int xx = 0;
int yy = 0;
int walk = 10;
int r=0;
boolean drag = FALSE;
int neg = -1;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
    int desktopHeight = GetSystemMetrics(SM_CYSCREEN);
    srand(time(0));
    switch (uMsg) {
    case WM_CREATE:
    {
        HDC hdc2 = GetDC(hwnd); // Get the window device context
        hMemDC = CreateCompatibleDC(hdc2); // Create a compatible memory DC

        // Load and select bitmaps
        
        hBitmap1 = (HBITMAP)LoadImage(NULL,L"\\\\?\\C:\\Users\\chris\\OneDrive\\Pictures\\dogmap3.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);// the \\?\ is used to bypass path limit of olderapis
        //hBitmap2 = (HBITMAP)LoadImage(NULL, L"\\\\?\\C:\\Users\\chris\\OneDrive\\Pictures\\dog_profileBMP2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);// the \\?\ is used to bypass path limit of olderapis

        // Select the first bitmap into the memory DC
        hOldBitmap = SelectObject(hMemDC, hBitmap1);

        ReleaseDC(hwnd, hdc2); // Release the window DC

        //SetTimer(hwnd, TIMER_ID, 500, NULL); // Set up a timer
        //SetTimer(hwnd, TIMER_ID2, 3000, NULL); // Set up a timer

        SetTimer(hwnd, TIMER_ID, 100, NULL); // Set up a timer
        SetTimer(hwnd, TIMER_ID2, 1000, NULL); // Set up a timer
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
    case WM_TIMER:{
        if (wParam == TIMER_ID) {
            //if (xPos < desktopWidth-100 && move==TRUE) {
            //    xPos = xPos + walk;
            //    yPos = yPos + (walk / 2)* neg;
            //}
            //else if(xPos >= desktopWidth - 100 && move==TRUE){
            //    yy = 300;//walk left
            //    move = TRUE;
            //    walk = -10;
            //    xPos = xPos + walk;
            //    yPos = yPos + (walk / 2) * neg;
            //    
            //}
            
            if (move) {
                xPos = xPos + walk;
                yPos = yPos + ((walk / 2) * neg);
                if (xPos >= desktopWidth - 100) {
                    //MessageBoxA(NULL,"out", "Right", MB_OK);
                    yy = 300;//walk left
                    walk = -10;

                }
                else if (xPos <= 0) {
                    //MessageBoxA(NULL, "out", "Left", MB_OK);
                    yy = 100;//walk right
                    walk = 10;

                }
                else if (yPos <= 0) {
                    //MessageBoxA(NULL, "out", "Top", MB_OK);
                   
                    yy = 100;//walk right
                    walk = 10;
                    neg = 1;

                }
                else if (yPos >= desktopHeight -50) {
                    //MessageBoxA(NULL, "out", "Bot", MB_OK);
                    //walk = 10;
                    
                    yy = 100;
                    walk = 10;
                    neg = -1;

                }
                
                   
                
            }
            

            SetWindowPos(hwnd, HWND_TOP, xPos, yPos, 100, 100, SWP_NOZORDER | SWP_NOSIZE);
            if (xx == 0) {
                xx = xx + 100;
            }
            else {
                xx = 0;
            }
        }
        if (wParam == TIMER_ID2) {
            if (r>=3) {
                r = rand() % (7-2+1) +2; //2-7
            }
            else { r = rand() % (4 + 1); }//0-4
            
            switch (r) {
          
            case 1:
                yy = 200;//sleep right
                move = FALSE;
                neg = -1;
                break;
            case 2:
                yy = 100;//walk right
                move = TRUE;
                walk = 10;
                
                break;
            case 3:
                yy = 0;//standing
                move = FALSE;
                neg = -1;
                break;
            case 4:
                yy = 300;//walk left
                move = TRUE;
                walk = -10;
    
                break;
            case 5:
                yy = 400;//sleep left
                move = FALSE;
                neg = -1;
                break;
            default:
                yy = 0;//standing
                move = FALSE;
                neg = 1;
                break;
            }

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
        KillTimer(hwnd, TIMER_ID);
        KillTimer(hwnd, TIMER_ID2);
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