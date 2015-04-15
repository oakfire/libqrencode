// qrencode.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "qrencode.h"
#include "GQrcode.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance

// Forward declarations of functions included in this code module:

INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	MainWnd(HWND , UINT , WPARAM , LPARAM );

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainWnd);

	return 0;
}



int W2M(const wchar_t * src, int cch, char * dest, int cbByte, UINT cp)
{
	if (src == NULL || dest == NULL || cbByte <= 0)
		return 0;

	int len = WideCharToMultiByte(cp, 0, src, cch, NULL, 0, NULL, NULL);
	if (len == 0)
		return 0;
	if (len > cbByte)
		return 0;

	return WideCharToMultiByte(cp, 0, src, cch, dest, cbByte, NULL, NULL);
}

INT_PTR CALLBACK MainWnd(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_CONTENT), _T("Please input content to encode "));

		HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_QRENCODE));
		SendMessage(hDlg, WM_SETICON, WPARAM(ICON_SMALL), LPARAM(hIcon));
		
		// center dlg
		// get coordinates of the window relative to its parent
		RECT rcDlg;
		::GetWindowRect(hDlg, &rcDlg);
		RECT rcArea;
		HWND hWndParent;

			// center within screen coordinates
		HMONITOR hMonitor = ::MonitorFromWindow(hDlg, MONITOR_DEFAULTTONEAREST);

		MONITORINFO minfo;
		minfo.cbSize = sizeof(MONITORINFO);
		BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);

		rcArea = minfo.rcWork;

		int DlgWidth = rcDlg.right - rcDlg.left;
		int DlgHeight = rcDlg.bottom - rcDlg.top;

		// find dialog's upper left based on rcCenter
		int xLeft = (rcArea.left + rcArea.right) / 2 - DlgWidth / 2;
		int yTop = (rcArea.top + rcArea.bottom) / 2 - DlgHeight / 2;

		// if the dialog is outside the screen, move it inside
		if (xLeft + DlgWidth > rcArea.right)
			xLeft = rcArea.right - DlgWidth;
		if (xLeft < rcArea.left)
			xLeft = rcArea.left;

		if (yTop + DlgHeight > rcArea.bottom)
			yTop = rcArea.bottom - DlgHeight;
		if (yTop < rcArea.top)
			yTop = rcArea.top;

		// map screen coordinates to child coordinates
		::SetWindowPos(hDlg, NULL, xLeft, yTop, -1, -1,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDOK)
		{

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		if (LOWORD(wParam) == IDC_BTN_BUILD)
		{
			wchar_t content[1024] ;
			ZeroMemory(content, 1024 * 2);
			GetWindowText(GetDlgItem(hDlg, IDC_EDIT_CONTENT), content, 1024);

			char intext[1024 * 4] = { 0 };
			W2M(content, -1, intext, 1024 * 4, CP_UTF8);

			HBITMAP bmp = GQrcode::qrencode(intext, strlen(intext));
			HBITMAP old = (HBITMAP)SendMessage(GetDlgItem(hDlg, IDC_STATIC_QR), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			if (old){
				::DeleteObject(old);
			}
		}
	}
	return (INT_PTR)FALSE;
}
