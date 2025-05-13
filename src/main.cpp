#define OEMRESOURCE

#include "ofMain.h"
#include "ofApp.h"

#include <Windows.h>
#include <chrono>
#include <thread>


//========================================================================
int main(){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLFWWindowSettings settings;
	settings.setSize(640*2, 480*2);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
	settings.decorated = false;
	settings.transparent = true;
	

	// try version 4.6
	settings.glVersionMajor = 3.0;
	settings.glVersionMinor = 2.0;



	//cout << settings.getPosition() << endl;

	
	
	auto window = ofCreateWindow(settings);

	// set window icon
	HICON hWindowIcon = NULL;
	HICON hWindowIconBig = NULL;
	HWND hwnd = ofGetWin32Window();
	


	string stricon = "data/cur.cur";


	if (hWindowIcon != NULL) DestroyIcon(hWindowIcon);
	if (hWindowIconBig != NULL) DestroyIcon(hWindowIconBig);
	hWindowIcon = (HICON)LoadImageA(GetModuleHandle(NULL), stricon.c_str(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	hWindowIconBig = (HICON)LoadImageA(GetModuleHandle(NULL), stricon.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hWindowIcon);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hWindowIconBig);


	//::SetWindowExtEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, LPCWSTR(""), LPCWSTR(""), NULL, 0, 0, 640, 480, NULL, 0, GetModuleHandle(NULL), 0);
	int extendedStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	

	//const HCURSOR cursor = static_cast<HCURSOR>(LoadImage(nullptr, L"cur.cur", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE));


	// ----- Change cursor ---- 

	//const HCURSOR cursor = LoadCursorFromFile(L"data/cur.cur");
	//
	////const HCURSOR copyCursor = CopyCursor(customCursor);
	////::SetSystemCursor(copyCursor, OCR_NORMAL);

	//HCURSOR cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_APPSTARTING);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_NORMAL);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_CROSS);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_HAND);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_IBEAM);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_NO);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_SIZEALL);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_SIZENESW);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_SIZENS);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_SIZENWSE);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_SIZEWE);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_UP);

	//cursorCopy = CopyCursor(cursor);
	//SetSystemCursor(cursorCopy, OCR_WAIT);
	

	//HWND hwnd = ofGetWin32Window();
	//HICON hMyIcon = LoadIcon(, MAKEINTRESOURCE("skyBoxTexture.png"));
	//SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hMyIcon);
	

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	//glClearColor(0.0, 0.0, 0.0, 0.0);

	ofRunApp(window, make_shared<ofApp>());

	
	


	ofRunMainLoop();
	
}
