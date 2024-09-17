#include <windows.h>
#include <time.h>

#define TIMER_ID 1
#define TIMER_ID2 2

HDC hMemDC;         // Memory device context
HBITMAP hBitmap1;   // bitmap that holds sprite sheet
HBITMAP hOldBitmap; // Old bitmap in memory DC

int xPos = 50; //window position x coord
int yPos = 50; //window position y coord

int xx = 0; //sprite x coord
int yy = 0; //sprite y coord

int walk = 10; //walk rate
int r=0;//action selection number
boolean drag = FALSE; //for click and drag
int neg = -1;
int n = 0;//array index
int nArray[3] = {-1,0,1};//walk direction: down, horizontal, up
boolean move = FALSE;
boolean bump = FALSE;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
    int desktopHeight = GetSystemMetrics(SM_CYSCREEN);
    srand((unsigned int)time(NULL));//seed random
    switch (uMsg) {
    case WM_CREATE: 
    {
        //When window is created
        HDC hdc2 = GetDC(hwnd); // Get the window device context
        hMemDC = CreateCompatibleDC(hdc2); // Create a compatible memory DC
        // Load and select bitmaps 
        hBitmap1 = (HBITMAP)LoadImage(NULL,L"SpriteSheet.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

        // Select bitmap into the memory DC
        hOldBitmap = SelectObject(hMemDC, hBitmap1);
        ReleaseDC(hwnd, hdc2); // Release the window DC

        //Create timers
        SetTimer(hwnd, TIMER_ID, 300, NULL); // Framerate for animation
        SetTimer(hwnd, TIMER_ID2, 1000, NULL); //Timer for changing actions
        break;
    }
    case WM_PAINT: 
    {
        // WM_PAINT: Triggered when the window needs to be repainted. 
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps); //BeginPaint: Prepares the window for painting and retrieves a device context (HDC).
  
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hMemDC, xx, yy, SRCCOPY);//Draw to window from MDC
        //ps.rcPaint: Window dimensions
        //xx,yy are the set coordinates on the spritemap
        EndPaint(hwnd, &ps);     
        break;
    }
    case WM_TIMER:{
        //When timer is triggered
        if (wParam == TIMER_ID) {//on framerate 
            //Check if action is moving then check boundaries then walk
            if (move) { 
                if (xPos >= desktopWidth - 100) {
                    //Right boundary
                    bump = TRUE;
                    yy = 300;//walk left
                    walk = -10;
                    neg = neg * (-1);
                }
                else if (xPos <= 0) {
                    //Left boundary
                    bump = TRUE;
                    neg = neg * (-1);
                    yy = 100;//walk right
                    walk = 10;
                }
                else if (yPos <= 0) {
                    //Top boundary
                    bump = TRUE;
                    yy = 100;//walk right
                    walk = 10;
                    neg = 1; 
                }
                else if (yPos >= desktopHeight -150) {
                    //Bottom boundary
                    bump = TRUE;
                    yy = 100;
                    walk = 10;
                    neg = -1;
                }
                //set movement
                xPos = xPos + walk;
                yPos = yPos + ((walk / 2) * neg);
            }
            SetWindowPos(hwnd, HWND_TOP, xPos, yPos, 100, 100, SWP_NOZORDER | SWP_NOSIZE);//update window movement
            //Animate by alternating between frame x=0 and x=100 from sprite sheet
            if (xx == 0) {
                xx = 100;
            }
            else {
                xx = 0;
            }
        }
        if (wParam == TIMER_ID2) {//for action changes
            //Randomly select direction from array
            n = rand() % (2 + 1);//0,1,2
            neg = nArray[n];
            //Randomly select number for action
            if ((r ==2) || (r==1)) {
                r = rand() % (3-1+1) +1; //1,2,3  
            }
            else if ((r == 4) || (r==5)) {
                r = rand() % (5 - 3 + 1) + 3; //3,4,5
            }
            else { r = rand() % (6 -1+ 1) +1; }//1,2,3,4,5,6
            
            switch (r) {
          
            case 1:
                yy = 200;//sleep right
                move = FALSE;
                bump = FALSE;
                break;
            case 2:
                if (bump) {
                    break;
                }
                yy = 100;//walk right
                move = TRUE;
                walk = 10;
                
                break;
            case 3:
                yy = 0;//standing
                move = FALSE;
                bump = FALSE;
                break;
            case 4:
                if (bump) {
                    break;
                }
                yy = 300;//walk left
                move = TRUE;
                walk = -10;
                
                break;
            case 5:
                yy = 400;//sleep left
                move = FALSE;
                bump = FALSE;
                break;
            default:
                yy = 0;//standing
                move = FALSE;
                bump = FALSE;               
                break;
            }

        }
            // Trigger a repaint
            InvalidateRect(hwnd, NULL, TRUE);
            break;
    }
    case WM_RBUTTONDOWN: {
        // Close the window on right mouse button click
        MessageBoxA(NULL,"Closing Desktop Pet...","Goodbye", MB_OK);             
        DestroyWindow(hwnd);
        break;
    }
    case WM_LBUTTONDOWN:
        drag = TRUE;
        SetCapture(hwnd); //captures mouse input
        break;
    case WM_LBUTTONUP:
        ReleaseCapture();
        drag = FALSE;
        break;
    case WM_MOUSEMOVE:
        //SOURCE: https://codingmisadventures.wordpress.com/2009/03/06/dragging-or-moving-a-window-using-mouse-win32/
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
        KillTimer(hwnd, TIMER_ID);
        KillTimer(hwnd, TIMER_ID2);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
 

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    //Create window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SimpleTransparentClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED,//removing | WS_EX_TOOLWINDOW so taskbar button shows,//removing WS_EX_TRANSPARENT works
       L"SimpleTransparentClass",
       L"Desktop Pet",
       WS_POPUP, //for a bordered window use: WS_OVERLAPPEDWINDOW,
       50, 50, 100, 100, //xPos,yPos,width,height
       NULL,
       NULL,
       hInstance,
       NULL
   );
    //Apply transparency
    SetLayeredWindowAttributes(hwnd, RGB(255,0,255), 255, LWA_COLORKEY);//Set magenta to be transparent
 
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    //Message loop to pass on to window procedure
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
 
    return (int)msg.wParam;
}