#define _WIN32_WINNT 0x0500

#include <tchar.h>
#include <windows.h>
#include <WindowsX.h>

// OpenGL��Ҫ��ͷ�ļ�
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

//�����������ʱ����Ҫ���õ�OpenGL�����,�򻯹�������
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )

// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME _T("WINCLASS1")
#define WINDOW_TITLE _T("OpenGL")
// MACROS /////////////////////////////////////////////////

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// GLOBALS ////////////////////////////////////////////////

HWND ghWnd = NULL;           // globally track main window

HINSTANCE ghInstance = NULL; // globally track hinstance
HDC ghDC;                    // GDI�豸����
HGLRC ghRC = NULL;           // ��Ⱦ����

#define FRAME_PER_SECOND (60)
#define TIME_IN_FRAME (1000/FRAME_PER_SECOND)
#define WIDTH (800)
#define HEIGHT (600)

//�����OpenGL����

void EnableOpenGL()
{
    PIXELFORMATDESCRIPTOR pfd;
        int iFormat;

    ghDC = GetDC(ghWnd);

    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;                        //�汾��һ����Ϊ1
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |       //һ��������ػ������Եı�־λ
                      PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;          //����������������RGBA������ɫ����;
    pfd.cColorBits = 32;                     //ÿ����ɫ����������ɫλƽ�����Ŀ������ɫ������ʽ�ǻ�������С
    pfd.iLayerType = PFD_MAIN_PLANE;         //�����ԣ�Ϊ��һ���Զ�������

    iFormat = ChoosePixelFormat(ghDC, &pfd); //ѡ��һ�����ظ�ʽ

    SetPixelFormat(ghDC, iFormat, &pfd);     //���õ�DC��

    ghRC = wglCreateContext(ghDC);           //������ͼ������
    wglMakeCurrent(ghDC, ghRC);              //ʹ֮��Ϊ��ǰ��ͼ������
}

//OpenGL��ʼ����ʼ
void SceneInit(int w, int h)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // ��ɫ���� 

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

//����������еĻ�ͼ����
void SceneShow(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(0.25, 0.25, 0.0);
    glVertex3f(0.75, 0.25, 0.0);
    glVertex3f(0.75, 0.75, 0.0);
    glVertex3f(0.25, 0.75, 0.0);
    glEnd();
    glFlush();
}

// ȡ�� OpenGL ���ڳ������ǰ���ã��ͷ���Ⱦ�������豸�����Լ����մ��ھ����
void DisableOpenGL()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(ghRC);
    ReleaseDC(ghWnd, ghDC);
}

// FUNCTIONS //////////////////////////////////////////////
LRESULT
CALLBACK
WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // this is the main message handler of the system
    PAINTSTRUCT ps; // used in WM_PAINT
    HDC hdc;        // handle to a device context

    // what is the message 
    switch (msg)
    {
        case WM_CREATE:
            {
            // do initialization stuff here
            // return success
            return (0);
            }

            break;

        case WM_PAINT:
            {
            // simply validate the window 
            hdc = BeginPaint(hwnd, &ps);

            // end painting
            EndPaint(hwnd, &ps);

            // return success
            return (0);
            }

            break;

        case WM_DESTROY:
            {

            // kill the application, this sends a WM_QUIT message 
            PostQuitMessage(0);

            // return success
            return (0);
            }

            break;

        default:
            break;
    } // end switch

    // process any messages that we didn't take care of 
    return (DefWindowProc(hwnd, msg, wparam, lparam));
} // end WinProc

///////////////////////////////////////////////////////////

int Game_Main(void *parms = NULL, int num_parms = 0)
{
    DWORD dwStartTime;

    dwStartTime = GetTickCount();

    // this is the main loop of the game, do all your processing
    // here

    // for now test if user is hitting ESC and send WM_CLOSE
    if (KEYDOWN(VK_ESCAPE))
        SendMessage(ghWnd, WM_CLOSE, 0, 0);

    SceneShow();

    // ����֡��
    while (GetTickCount() - dwStartTime < TIME_IN_FRAME)
    {
        Sleep(1);
    }

    // return success or failure or your own return code here
    return (1);
} // end Game_Main

////////////////////////////////////////////////////////////

int Game_Init(void *parms = NULL, int num_parms = 0)
{
    // this is called once after the initial window is created and
    // before the main event loop is entered, do all your initialization
    // here
    ghDC = GetDC(ghWnd);

    EnableOpenGL();
    SceneInit(WIDTH, HEIGHT);

    // return success or failure or your own return code here
    return (1);
} // end Game_Init

/////////////////////////////////////////////////////////////

int Game_Shutdown(void *parms = NULL, int num_parms = 0)
{
    // this is called after the game is exited and the main event
    // loop while is exited, do all you cleanup and shutdown here
    DisableOpenGL();

    // return success or failure or your own return code here
    return (1);
} // end Game_Shutdown

// WINMAIN ////////////////////////////////////////////////
int
WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
    WNDCLASSEX winclass; // this will hold the class we create
    HWND hwnd;           // generic window handle
    MSG msg;             // generic message

    // first fill in the window class stucture
    winclass.cbSize = sizeof(WNDCLASSEX);
    winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc = WindowProc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = hinstance;
    winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName = NULL;
    winclass.lpszClassName = WINDOW_CLASS_NAME;
    winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // save hinstance in global
    ghInstance = hinstance;

    // register the window class
    if (!RegisterClassEx(&winclass))
        return (0);

    // create the window
    if (!(hwnd = CreateWindowEx(NULL,              // extended style
                                WINDOW_CLASS_NAME, // class
                                WINDOW_TITLE,      // title
                                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                0,
                                0,                 // initial x,y
                                WIDTH,
                                HEIGHT,            // initial width, height
                                NULL,              // handle to parent 
                                NULL,              // handle to menu
                                hinstance,         // instance of this application
                                NULL)))            // extra creation parms
        return (0);

    // save main window handle
    ghWnd = hwnd;

    // initialize game here
    Game_Init();

    // enter main event loop
    while (TRUE)
    {
        // test if there is a message in queue, if so get it
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // test if this is a quit
            if (msg.message == WM_QUIT)
                break;

            // translate any accelerator keys
            TranslateMessage(&msg);

            // send the message to the window proc
            DispatchMessage(&msg);
        } // end if

        // main game processing goes here
        Game_Main();
    } // end while

    // closedown game here
    Game_Shutdown();

    // return to Windows like this
    return (msg.wParam);
} // end WinMain

///////////////////////////////////////////////////////////  
