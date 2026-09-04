#include <windows.h>
#include <gl/GL.h>

// TODO!!!!!
#include "../../../amjulib/Source/SoundBass/bass.h"
#include "../../../amjulib/Source/SoundBass/bassmidi.h"

// Amjulib includes
#include <AmjuGL.h>
#include <AmjuGL-OpenGL.h>
#include <AmjuGLWindowInfo.h>
#include <CommandLineArgs.h>
#include <DoOnce.h>
#include <EventPoller.h>
#include <Game.h>
#include <MessageQueue.h>
#include <ResourceManager.h>
#include <Screen.h>
#include <SoundManager.h>
#include "resource.h" // For icon; should be part of game-specific info

// Enable console for debug builds
#ifdef _DEBUG
#pragma comment(linker, "/subsystem:console /entry:WinMainCRTStartup")
#else
#pragma comment(linker, "/subsystem:windows")
#endif

// Link OpenGL libraries via code pragmas (avoids configuring linker settings manually)
#pragma comment(lib, "opengl32.lib")

// Aim for this frame rate: don't run as fast as poss.
static const int TARGET_FPS = 60;

// Aspect ratio: we want to maintain this as far as poss,
//  but not in fullscreen for now.
const double TARGET_ASPECT = 16.0 / 9.0;

static auto WINDOW_CLASS = L"OpenGLWin32Class";

// TODO Game-specific
static auto WindowTitle = L"Amju Piano Fest";

namespace Amju
{
  // Definitions in game-specific Startup code
  void StartUpBeforeCreateWindow();
  void StartUpAfterCreateWindow();
  // Game-specific shut down function
  void ShutDown();

  static void QueueEvent(Event* e)
  {
    TheEventPoller::Instance()->GetImpl()->QueueEvent(e);
  }

  static void GameSpecificPreWindowTasks()
  {
    // Amjulib - set default event poller
    TheEventPoller::Instance()->SetImpl(new EventPollerImpl);

    SetCommandLineArgs({ __argc, __argv }); // windows-specific replacements for argc/argv

    StartUpBeforeCreateWindow(); // game-specific code
  }

  static void GameSpecificPostWindowTasks()
  {
    // Set OpenGL AmjuGL impl, with no window create func
    AmjuGL::SetImpl(new AmjuGLOpenGL(nullptr));

    AmjuGL::Init();

    StartUpAfterCreateWindow(); // game-specific code
  }

  static void ShutDownGameAndLibs()
  {
    do_once
    {
      ShutDown(); // game-specific

      // Amjulib shutdowns
      // Release the resources held by the current game state.
      TheGame::Instance()->GetState()->OnDeactive();
      // Kill any events and messages in flight.
      TheEventPoller::Instance()->Clear();
      TheMessageQueue::Instance()->Clear();
      // Shut down sound
      TheSoundManager::Instance()->ShutDown();
      // Free up resources
      TheResourceManager::Instance()->Clear();
      // Free up graphics impl.
      AmjuGL::Destroy();
    }
  }

  // Convert screen space coords into Amju screen space (-1 .. 1 in both axes)
  static std::pair<float, float> ConvertCoords(int x, int y)
  {
    float scrX2 = static_cast<float>(Amju::Screen::X() / 2);
    float scrY2 = static_cast<float>(Amju::Screen::Y() / 2);
    return { static_cast<float>(x) / scrX2 - 1.0f, 1.0f - static_cast<float>(y) / scrY2 };
  }

  static Event* MakeCursorEvent(int x, int y)
  {
    auto ce = new CursorEvent;
    const auto [x1, y1] = ConvertCoords(x, y);
    ce->x = x1;
    ce->y = y1;
    ce->controller = 0;
    return ce;
  }

  static Event* MakeMouseButtonEvent(
    MouseButton button, int x, int y, bool down, bool ctrl, bool shift)
  {
    auto mbe = new MouseButtonEvent;
    const auto [x1, y1] = ConvertCoords(x, y);
    mbe->x = x1;
    mbe->y = y1;
    mbe->button = button;
    mbe->isDown = down;
    return mbe;
  }
} // namespace Amju

void UpdateDrawFlip()
{
  Amju::TheGame::Instance()->RunOneLoop();
}

void DrawAndFlip()
{
  Amju::TheGame::Instance()->Draw();
  Amju::AmjuGL::Flip();
}

// Vertical sync function ptr
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

static void SetUpVSync()
{
  // Try to lock to monitor refresh rate
  auto wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
  bool vsyncActive = false;
  if (wglSwapIntervalEXT) 
  {
    wglSwapIntervalEXT(1); 
    vsyncActive = true;
  }
}

// Global variables for window and rendering contexts
static bool g_running = false;
static int g_width = 0; // these are set before we use them!
static int g_height = 0;
static bool g_active = true;
static bool g_isFullscreen = false;
// Window placement, for returning from full screen
WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };

// Forward declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreateOpenGLContext(HWND hwnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC);

static void SleepIfFrameRateTooHigh(LARGE_INTEGER timeStart, LARGE_INTEGER timeEnd, LARGE_INTEGER frequency)
{
  double elapsedSeconds = (double)(timeEnd.QuadPart - timeStart.QuadPart) / frequency.QuadPart;

  const double targetFrameTime = 1.0 / static_cast<double>(TARGET_FPS); 

  // Sleep if the frame finished faster than our target limit.
  // If V-Sync worked, elapsedSeconds will already naturally match or exceed targetFrameTime.
  if (elapsedSeconds < targetFrameTime)
  {
    // Calculate remaining milliseconds and sleep
    DWORD sleepTimeMs = (DWORD)((targetFrameTime - elapsedSeconds) * 1000.0);
    if (sleepTimeMs > 0)
    {
      Sleep(sleepTimeMs);
    }
  }
}

static std::tuple<int, int, int, int>
GetCentredPosOnCurrentMonitor()
{
  // 1. Find out where the mouse cursor is *right now* before creating the window
  POINT mousePos;
  GetCursorPos(&mousePos);

  // 2. Identify the monitor under the cursor
  HMONITOR hMonitor = MonitorFromPoint(mousePos, MONITOR_DEFAULTTONEAREST);
  MONITORINFO mi = { sizeof(mi) };
  GetMonitorInfo(hMonitor, &mi);

  // 3. Calculate a centered position within that specific monitor's workspace
  int windowWidth = 800; // TODO Hard coded size, get from required window size
  int windowHeight = static_cast<int>(windowWidth / TARGET_ASPECT);
  int monitorWidth = mi.rcWork.right - mi.rcWork.left;
  int monitorHeight = mi.rcWork.bottom - mi.rcWork.top;
  int posX = mi.rcWork.left + (monitorWidth - windowWidth) / 2;
  int posY = mi.rcWork.top + (monitorHeight - windowHeight) / 2;

  return { posX, posY, windowWidth, windowHeight };
}

static int CreateWindow(HINSTANCE hInstance, HWND& hWnd)
{
  WNDCLASSEX wc{};

  // Register Window Class (Replaces glutInit/glutInitDisplayMode)
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // CS_OWNDC is required for OpenGL
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  // LARGE icon (for Alt+Tab and taskbar spreads)
  wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1),
    IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
  // SMALL icon (for Titlebar and Minimized Taskbar Pins)
  wc.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1),
    IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = NULL; // NULL prevents screen flickering before rendering
  wc.lpszMenuName = NULL;
  wc.lpszClassName = WINDOW_CLASS;

  if (!RegisterClassEx(&wc)) {
    MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  // Create Window 
  const auto [x, y, w, h] = GetCentredPosOnCurrentMonitor();
  g_width = w;
  g_height = h;

  hWnd = CreateWindowEx(
    WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
    WINDOW_CLASS,
    WindowTitle,
    WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
    x, y, w, h, // Native multi-monitor positioning
    NULL, NULL, hInstance, NULL
  );

  if (hWnd == NULL) {
    MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }
  return 1; 
}

static void ToggleFullscreen(HWND hwnd, bool& isFullscreen, WINDOWPLACEMENT& wpPrev) 
{
  DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

  if (isFullscreen == false) 
  {
    // Moving to FULLSCREEN
    if (GetWindowPlacement(hwnd, &wpPrev)) 
    {
      HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
      MONITORINFO mi = { sizeof(mi) };
      if (GetMonitorInfo(hMonitor, &mi)) 
      {
        // Strip window borders
        SetWindowLong(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
        // Resize to absolute monitor coordinates
        SetWindowPos(hwnd, HWND_TOP,
          mi.rcMonitor.left, mi.rcMonitor.top,
          mi.rcMonitor.right - mi.rcMonitor.left,
          mi.rcMonitor.bottom - mi.rcMonitor.top,
          SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        isFullscreen = true;
      }
    }
  }
  else 
  {
    // Restoring to WINDOWED mode
    SetWindowLong(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
    SetWindowPlacement(hwnd, &wpPrev);
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
      SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
      SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    isFullscreen = false;
  }
}

static void MainLoop(MSG& msg)
{
  // Hi res timer to throttle frame rate if v-sync doesn't work.
  LARGE_INTEGER frequency;
  LARGE_INTEGER timeStart;
  LARGE_INTEGER timeEnd;

  QueryPerformanceFrequency(&frequency);

  // Message/Render Loop
  while (g_running)
  {
    // Record exactly when the frame started processing
    QueryPerformanceCounter(&timeStart);

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
      {
        g_running = false;
      }
      else
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    else
    {
      if (g_active)
      {
        UpdateDrawFlip();

        // Don't SwapBuffers here: we are already flipping in AmjuGL.
        // Sleep to throttle frame rate, (if vsync didn't work).
        QueryPerformanceCounter(&timeEnd);
        SleepIfFrameRateTooHigh(timeStart, timeEnd, frequency);
      }
      else
      {
        // App is minimized/unfocused: Put thread to sleep.
        WaitMessage(); // blocks!

        // When waking back up, reset high-res timers 
        // to prevent 'dt' from spiking due to the long sleep!
        QueryPerformanceCounter(&timeStart);
      }
    }
  }
}

// Main Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
  HWND hWnd = nullptr;
  if (!CreateWindow(hInstance, hWnd)) return 0;

  HDC deviceContext = nullptr;
  HGLRC renderingContext = nullptr;

  CreateOpenGLContext(hWnd, &deviceContext, &renderingContext);

  SetUpVSync();

  Amju::GameSpecificPreWindowTasks();

  ShowWindow(hWnd, nCmdShow);

  Amju::GameSpecificPostWindowTasks();

  // We need one update before we start the main loop, which will 
  //  cause an immediate Draw (WM_PAINT message).
  Amju::TheGame::Instance()->Update();

  UpdateWindow(hWnd);

  g_running = true;

  MSG msg {};
  MainLoop(msg);

  // Cleanup, to avoid crashes on exit.
  Amju::ShutDownGameAndLibs();

  DisableOpenGL(hWnd, deviceContext, renderingContext);
  DestroyWindow(hWnd);

  return static_cast<int>(msg.wParam);
}

static void OnActivation(bool active)
{
  g_active = active;

  // Resume or pause audio: this is very convenient but horrendously non-portable
  if (active)
  {
    BASS_Start();
  }
  else
  {
    BASS_Pause();
  }

  // If we are actually playing the game (not in a menu etc), go to the pause state.
  if (!active)
  {
    Amju::TheGame::Instance()->PauseGame();
  }
}

static void OnMouseMove(LPARAM lParam)
{
  int x = LOWORD(lParam);
  int y = HIWORD(lParam);
  Amju::QueueEvent(Amju::MakeCursorEvent(x, y));
}

static void OnMouseButton(Amju::MouseButton button, WPARAM wParam, LPARAM lParam)
{
  bool down = (wParam & MK_LBUTTON) != 0;
  bool ctrl = (wParam & MK_CONTROL) != 0;
  bool shift = (wParam & MK_SHIFT) != 0;
  int x = LOWORD(lParam);
  int y = HIWORD(lParam);

  Amju::QueueEvent(Amju::MakeMouseButtonEvent(
    button, x, y, down, ctrl, shift));
}

static void SetAmjuViewport(int width, int height)
{
  if (height == 0) height = 1; // Prevent division by zero
  Amju::Screen::SetSize(width, height);
}

static void OnSize(int width, int height)
{
  using namespace Amju;

  if (height == 0) height = 1; // Prevent division by zero
  SetAmjuViewport(width, height);

  ResizeEvent* e = new ResizeEvent;
  e->type = AMJU_RESIZE;
  e->x = width;
  e->y = height;
  QueueEvent(e);
}

void OnKeyEvent(char k, bool down)
{
  using namespace Amju;

  auto ke = new Amju::KeyEvent;
  //ke->modifier = glutGetModifiers();
  ke->keyDown = down;

  if (k == 127) // backspace
  {
#ifdef WIN32
    ke->keyType = AMJU_KEY_DELETE;
#else
    ke->keyType = AMJU_KEY_BACKSPACE;
#endif
  }
  else if (k == 8) // delete
  {
#ifdef WIN32
    ke->keyType = AMJU_KEY_BACKSPACE;
#else
    ke->keyType = AMJU_KEY_DELETE;
#endif
  }
  else if (k == 13)
  {
    ke->keyType = AMJU_KEY_ENTER;
  }
  else if (k == 27) // esc
  {
    ke->keyType = AMJU_KEY_ESC;
  }
  else if (k == ' ')
  {
    ke->keyType = AMJU_KEY_SPACE;
  }
  else
  {
    ke->keyType = AMJU_KEY_CHAR;
    ke->key = k;
  }

  QueueEvent(ke);
}

static void OnResizing(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
  // Only enforce this if we are not currently fullscreen
  if (g_isFullscreen) return;

  RECT* rect = (RECT*)lParam;

  // Calculate the current window border overhead (borders + title bar)
  RECT winRect = { 0, 0, 0, 0 };
  AdjustWindowRectEx(&winRect, GetWindowLong(hwnd, GWL_STYLE), FALSE, GetWindowLong(hwnd, GWL_EXSTYLE));
  int borderWidth = winRect.right - winRect.left;
  int borderHeight = winRect.bottom - winRect.top;

  // Extract the proposed internal client dimensions
  int proposedClientWidth = (rect->right - rect->left) - borderWidth;
  int proposedClientHeight = (rect->bottom - rect->top) - borderHeight;

  // Adjust based on which side/corner the user is dragging
  switch (wParam) 
  {
  case WMSZ_LEFT:
  case WMSZ_RIGHT:
  case WMSZ_BOTTOMLEFT:
  case WMSZ_BOTTOMRIGHT:
    // User adjusted width; update height to match
    proposedClientHeight = (int)(proposedClientWidth / TARGET_ASPECT);
    rect->bottom = rect->top + proposedClientHeight + borderHeight;
    break;

  case WMSZ_TOP:
  case WMSZ_BOTTOM:
  case WMSZ_TOPLEFT:
  case WMSZ_TOPRIGHT:
    // User adjusted height; update width to match
    proposedClientWidth = (int)(proposedClientHeight * TARGET_ASPECT);
    rect->right = rect->left + proposedClientWidth + borderWidth;
    break;
  }

  // Update size, redraw with no update.
  g_width = (rect->right - rect->left) - borderWidth;
  g_height = (rect->bottom - rect->top) - borderHeight;

  SetAmjuViewport(g_width, g_height);
  // Don't DrawAndFlip(). Leave this to OnPaint. It looks smoother.
}

static void OnPaint(HWND hwnd)
{
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hwnd, &ps);
  DrawAndFlip();
  EndPaint(hwnd, &ps);
}

// Event Callback 
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
  switch (msg) 
  {
  case WM_MOUSEMOVE:
    OnMouseMove(lParam);
    break;

  case WM_LBUTTONDOWN:
    // SetCapture() means we still get mouse events when the
    // mouse leaves the client area.
    SetCapture(hwnd);
    OnMouseButton(Amju::AMJU_BUTTON_MOUSE_LEFT, wParam, lParam);
    break;

  case WM_LBUTTONUP:
    // We must now release the mouse - end of SetCapture()
    // mouse ownership.
    ReleaseCapture();
    OnMouseButton(Amju::AMJU_BUTTON_MOUSE_LEFT, wParam, lParam);
    break;

  case WM_ACTIVATE:
    // LOWORD(wParam) tells us the activation state.
    // HIWORD(wParam) tells us if the window is minimized (non-zero means minimized).
    OnActivation(!(LOWORD(wParam) == WA_INACTIVE || HIWORD(wParam) != 0));
    break;

  case WM_SIZE:
    // New window size
    g_width = LOWORD(lParam);
    g_height = HIWORD(lParam);
    OnSize(g_width, g_height);
    break;

  case WM_SIZING: 
    // Player is dragging a window corner or edge to resize. 
    OnResizing(hwnd, wParam, lParam);
    break;

  case WM_PAINT: 
    // We need to hndle this to redraw the window nicely when resizing.
    if (g_running) OnPaint(hwnd);
    break;

  case WM_KEYDOWN:
    OnKeyEvent(static_cast<char>(wParam & 0xff), true);
    break;

  case WM_KEYUP:
    OnKeyEvent(static_cast<char>(wParam & 0xff), false);
    break;

  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN && (lParam & (1 << 29)))
    {
      // We check bit 29 of lParam to confirm the ALT key is actually held down
      ToggleFullscreen(hwnd, g_isFullscreen, g_wpPrev);
      return 0; // Handled, prevent Windows from making an error ding
    }
    // For other key combos, do the DefWindowProc
    return DefWindowProc(hwnd, msg, wParam, lParam);

  case WM_POWERBROADCAST:
    if (wParam == PBT_APMSUSPEND) 
    {
      // The computer is going to sleep! 
      // Force-save user data and completely pause your loops.
      OnActivation(false);
    }
    else if (wParam == PBT_APMRESUMEAUTOMATIC) {
      // The computer just woke back up.
      // Re-anchor your high-resolution timer to prevent giant dt spikes!
      OnActivation(true);
    }
    break;

  case WM_CLOSE:
    // Make sure we exit gracefully -- we can call this multiple times ok.
    Amju::ShutDownGameAndLibs(); 

    g_running = false;
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

void CreateOpenGLContext(HWND hwnd, HDC* hDC, HGLRC* hRC) 
{
  PIXELFORMATDESCRIPTOR pfd;
  int format;

  // Get the device context
  *hDC = GetDC(hwnd);

  // Zero out and define the pixel format descriptor
  ZeroMemory(&pfd, sizeof(pfd));
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;

  // Match and set the format
  format = ChoosePixelFormat(*hDC, &pfd);
  SetPixelFormat(*hDC, format, &pfd);

  // Create and activate the rendering context
  *hRC = wglCreateContext(*hDC);
  wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC) 
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hwnd, hDC);
}
