/*
  Berusci smazba
*/
#define  WM_MOUSEWHEEL  0x020A

#include "mss_on.h"
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <math.h>
#include <direct.h>
#include <time.h>
#include <assert.h>
#include "..\resource.h"

#include "3d_all.h"

#include "..\kofola\Object.h"
#include "..\Kofola\game_main.h"

#include "berusky_universal.h"
#include "berusky3d_castice.h"
#include "berusky3d.h"
#include "berusky3d_ini.h"
#include "berusky3d_load.h"
#include "berusky3d_render.h"
#include "berusky3d_animace.h"
#include "berusky3d_kofola_interface.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glaux.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput.lib")
#pragma comment(lib,"dsound.lib")

#define  BERUSKY_NAME   "Berusky 2"

dword      system_timer = 0;
dword      system_kurzor = 0;
dword      karmin_aktivni = 1;
dword      obsluha_okna = 0;
dword      play_demo = 0;
HWND       hwnd_hry;
HINSTANCE  hinst;
byte       ini_file[300];
int        mouse_move = FALSE;

#ifdef DEBUG_OKNO
  dword      start_debug;
#endif

byte work_dir[200] = ".";

__inline void nacti_polohu_mysi(WPARAM wParam, LPARAM lParam)
{                                     
  mi.dx = LOWORD(lParam)- mi.x;
  mi.dy = HIWORD(lParam)- mi.y;
  mi.x += mi.dx;
  mi.y += mi.dy;
}                                     

__inline void nacti_tlacitka_mysi(WPARAM wParam, LPARAM lParam)
{                                     
  mi.ot1 = mi.dt1 = mi.t1 = LOWORD(wParam) & MK_LBUTTON;
  mi.ot2 = mi.dt2 = mi.t2 = LOWORD(wParam) & MK_RBUTTON;  
}                                     

__inline int filtr_klaves(int scancode)
{   
  return(gl_Allow_Key(scancode) ? scancode : 0);
}

long CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{  
  switch(message) { 
  case WM_MOUSEWHEEL:    
    
    if(!PRCameraFlag)
      break;
    
    PRCameraFlagChange = FALSE;
    
    if((signed short)HIWORD(wParam) > 0)
      key[K_PLUS] = TRUE;
    else
      key[K_MINUS] = TRUE;
    
    break;                     

  case WM_ACTIVATEAPP:   
    if(wParam) {
      karmin_aktivni = TRUE;
      system_kurzor = FALSE;
      if(obsluha_okna)
        maximalizuj_hru();
    } else {
      karmin_aktivni = FALSE;
      system_kurzor = TRUE;
      if(obsluha_okna)
        minimalizuj_hru();
      clip_ret();
    }
    break;
  case WM_CLOSE:
    PostQuitMessage(0);
    return(FALSE);
  case WM_QUIT: // ukonceni aplikace - hodit sem konec!
    break;
  case WM_KEYDOWN:
    {
      int scan = filtr_klaves(MapVirtualKey(wParam,0));
      if(scan) {
        key[scan] = TRUE;
        key[0] = TRUE;
      }      
      key_pressed = wParam;      
      break;
    }

  case WM_KEYUP:
    {
      int scan = filtr_klaves(MapVirtualKey(wParam,0));
      if(scan) {
        key[scan] = FALSE;
        key[0] = FALSE;
      }      
      key_pressed = 0;      
      break;
    }
  case WM_CHAR:
    //key_pressed = (TCHAR) wParam;
    break;
  case WM_MOUSEMOVE:
    nacti_polohu_mysi(wParam, lParam);
    break;
  case WM_LBUTTONUP:
    nacti_tlacitka_mysi(wParam, lParam);
    break;
  case WM_RBUTTONUP:
    nacti_tlacitka_mysi(wParam, lParam);
    break;
  case WM_LBUTTONDOWN: 
    nacti_tlacitka_mysi(wParam, lParam);
    break;
  case WM_RBUTTONDOWN:
    nacti_tlacitka_mysi(wParam, lParam);
    break;
  case WM_SETCURSOR:
    if(!mouse_move)
      mouse_move = FALSE;
    break;
  }
 return DefWindowProc( hwnd, message, wParam, lParam );
}

int registruj_okno(HINSTANCE handle_aplikace)
{
  WNDCLASS    wc;

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = handle_aplikace;
  wc.hIcon = LoadIcon(handle_aplikace, MAKEINTRESOURCE(IDI_ICON1));
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = BERUSKY_NAME;
  
  return(RegisterClass(&wc));
}

HWND otevri_okno(HINSTANCE handle_aplikace, int full_screen)
{
  HWND        hwnd;
  int         dx = GetSystemMetrics(SM_CXSCREEN),
              dy = GetSystemMetrics(SM_CYSCREEN);
  
  if(full_screen) {
    hwnd = CreateWindowEx(
      WS_EX_TOPMOST,
      BERUSKY_NAME,
      BERUSKY_NAME,
      WS_POPUP,
      0,
      0,
      dx,
      dy,
      NULL,
      NULL,
      handle_aplikace,
      NULL);
  } else {
    hwnd = CreateWindowEx(
      FALSE,
      BERUSKY_NAME,
      BERUSKY_NAME,
      WS_POPUP,
      0,
      0,
      dx,
      dy-=2,
      NULL,
      NULL,
      handle_aplikace,
      NULL);
  }

  if (!hwnd) return NULL;

  BringWindowToTop(hwnd);
  SetForegroundWindow(hwnd);
  ShowWindow(hwnd,SW_SHOWNORMAL);
  UpdateWindow(hwnd);
  SetFocus(hwnd);

  return(hwnd);
}

HWND zavri_okno(HWND hwnd)
{
  DestroyWindow(hwnd);
  return(0);
}

void nastav_okno(int dx, int dy)
{ 
  if(!bDXAktivni && hwnd_hry) {
    MoveWindow(hwnd_hry,0,0,dx,dy,TRUE);
    BringWindowToTop(hwnd_hry);
    SetForegroundWindow(hwnd_hry);
    SetFocus(hwnd_hry);
  }
}

void clip_set(int dx, int dy)
{
  RECT rc = {0,0,dx,dy};
  ClipCursor(&rc);
}
void clip_ret(void)
{
  ClipCursor(NULL);
}

void minimalizuj_hru(void)
{
  if(hwnd_hry) {
    gl_Kofola_Minimalize();
  }
}

void maximalizuj_hru(void)
{
  if(hwnd_hry) {
    grf_prehod_mod_hra(&hwconf);
    ShowWindow(hwnd_hry,SW_RESTORE);
    UpdateWindow(hwnd_hry);
    nastav_okno(hwconf.xres,hwconf.yres);
    SetFocus(hwnd_hry);
    gl_Kofola_Maximalize();
    clip_set(hwconf.xres,hwconf.yres);
  }
}

int kom_graf_init(void)
{
  int full;

  kprintf(1,"Starting SuperHouba.");

  /* Nahraje konfiguraci grafiky
  */
  nahraj_device_config(ini_file,"hra",&hwconf);
  nahraj_universal_device_config(ini_file,"hra",&hwconf);
  nahraj_texture_config(ini_file, &txconf);

  full = hwconf.fullscreen;
  
  /* Nahodi graficky rezim
  */
  if(full) {    
    nastav_okno(hwconf.xres,hwconf.yres);
    if(!grf_prehod_mod_hra(&hwconf))
      return(FALSE);
    hwnd_hry = zavri_okno(hwnd_hry);
    hwnd_hry = otevri_okno(hinst,full);
  }
  
  kprintf(1,"Screen mode %d ok",full);
  
  /* Nahodi grafiku
  */
  if(!grf_start(hinst,hwnd_hry,ini_file,NULL,TRUE))
    chyba("Inicializace");

  /* Nahozeni renderovacich funkci
  */ 
  ber_nahod_render_funkce();

  /* Nastavni rozliseni
  */
  p_ber->wx = 0;
  p_ber->wy = 0;
  p_ber->wxres = hwconf.xres;
  p_ber->wyres = hwconf.yres;

  clip_set(hwconf.xres,hwconf.yres);

  /* Vyflushni veci z fronty
  */
  spracuj_spravy(TRUE);

  /* Nahodim svoji obsluhu okna
  */
  obsluha_okna = TRUE;

  return(TRUE);
}

void kom_ret_default_text_config(void)
{ 
 txconf.text_mip_mapping = GetPrivateProfileInt(TXT_SEKCE,"text_mip_mapping",1,ini_file);
 txconf.text_mip_filtr =   GetPrivateProfileInt(TXT_SEKCE,"text_mip_filtr",1,ini_file);
 txconf.text_wrap_x = 1;
 txconf.text_wrap_y = 1;
}

void kom_graf_konec(int menu)
{
  kprintf(TRUE,"kom_graf_konec...");

  /* Shodim svoji obsluhu okna
  */
  obsluha_okna = FALSE;

  /* Shodim GL
  */
  konec(FALSE);
  
  if(menu) {
    nastav_okno(XRES_MENU,YRES_MENU);
    grf_prehod_mod_menu();
    spracuj_spravy(0);
  }

  clip_ret();
}

void konec(int konec)
{
  /* Stop grafiky
  */
  grf_stop(hwnd_hry);
  
  /* Zapne kurzor
  */
  zapni_kurzor();

  if(konec) {    
    exit(0);
  }
}

int efile(byte *p_file);

int WINAPI WinMain( HINSTANCE hi, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{
 byte pom[200];
 byte pom2[200];
 hinst = hi;

 _getcwd(ini_file,500);
 strcat(ini_file,"\\berusky3d.ini");
 if(!efile(ini_file)) {
   strcpy(ini_file,"berusky3d.ini");
 } 

 registruj_okno(hinst);
 hwnd_hry = otevri_okno(hinst,hwconf.fullscreen);

 if(GetPrivateProfileInt("debug","debug_file",0,ini_file)) {
   GetPrivateProfileString("hra","log_file","c:\\berusky2log.txt",pom,500,ini_file);
   p_ber->debug_file = fopen(pom,"a");
   kprintf(TRUE, "-----------------------Start game - date %s time %s-----------------",_strdate(pom),_strtime(pom2));
 }

 kprintf(TRUE,"Komat - sv: %s (%s %s)",VERZE_KOMAT,__DATE__,__TIME__);
 
 ber_konfiguruj_berusky(&ber);
 
 {
   int i;
   for(i = 0; i < 1; i++) {
     winmain_Game_Run(hwnd_hry,lpCmdLine);
   }
 }
 
 //MSS_LOG_BLOCK_LIST;

 return(TRUE);
}

int spracuj_spravy(int param)
{
 MSG  msg;
 
 while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )) {
   TranslateMessage(&msg);
   DispatchMessage(&msg);
   if(msg.message == WM_QUIT) {
     gl_Kofola_End(bDXAktivni);
   }
 }
 return(1);
}
