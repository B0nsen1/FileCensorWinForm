#pragma once
#include "header.h"
#include <fstream>

class CLoginWindow
{
public:
	CLoginWindow(void);
public:
	~CLoginWindow(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CLoginWindow* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	HWND hBar;
	static HWND hAddDialog;
};
