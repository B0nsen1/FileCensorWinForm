
#include "LoginWindow.h"
#include "MainWindow.h"
#include "header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include <windowsX.h>
#include <commctrl.h>
#include <filesystem>

namespace fs = std::filesystem;

CLoginWindow* CLoginWindow::ptr = NULL;
HWND CLoginWindow::hAddDialog = NULL;
static wchar_t str1[150], str2[150];
static int fileSizeInt = 0;
			

CLoginWindow::CLoginWindow(void)
{
	ptr = this;
}

CLoginWindow::~CLoginWindow(void)
{

}

void CLoginWindow::Cls_OnClose(HWND hwnd)
{
	DestroyWindow(hwnd);
	hAddDialog = NULL;
}

DWORD WINAPI SizeCheck(LPVOID lp)
{

	std::streampos fileSize;
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{A3F5484B-543E-4C5D-8B0C-8A850F9421FF}"));
	for (const auto& entry : fs::directory_iterator(str2)) {
		if (fs::is_regular_file(entry.path())) {

			std::ifstream in(entry.path(), std::ios::ate);
			if (!in) {
				continue;
			}
			fileSize = in.tellg();
			fileSizeInt += static_cast<int>(fileSize);
			in.close();
		}
	}
	ReleaseMutex(hMutex);
	return 0;
}

BOOL CLoginWindow::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	return TRUE;
}



DWORD WINAPI Change(LPVOID lp)
{
	WIN32_FIND_DATA findFileData;
	HWND hBar = (HWND)lp;

	if (!fs::exists(str2) || !fs::is_directory(str2)) {
		MessageBox(NULL, L"Дериктории не существует", L"Ошибка", MB_OK);
		return 1; 
	}
	wchar_t str[50];
	wsprintf(str, L"%d", fileSizeInt);
	MessageBox(NULL, str, L"1", MB_OK);
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{A3F5484B-543E-4C5D-8B0C-8A850F9421FF}"));
	std::wifstream file;
	std::wifstream file1(str1);
	std::wofstream file2;
	std::wofstream logfile("log.txt");

	_TCHAR logchar[150];



	file.seekg(0, std::ios::beg);
	std::size_t sizeInBytes;
	
	wchar_t ban_word[50];
	int i = 0;

	int progress_bar_value = 0;
	int size;

	SendMessage(hBar, PBM_SETRANGE, 0, MAKELPARAM(0, fileSizeInt));

	
	

	for (const auto& entry : fs::directory_iterator(str2)) {
		if (fs::is_regular_file(entry.path())) {

			file.open(entry.path());
			if (!file) {
				continue;
			}
			wsprintf(str, L"Result\\result%d.txt", i);
			file2.open(str);
			while (!file.eof())
			{
				file >> str;
				while (!file1.eof())
				{
					file1 >> ban_word;
					if (!_tcscmp(str, ban_word))
					{
						wsprintf(logchar, L"In file word %s was found", str);
						logfile << logchar;
						logfile << "\n";
						wsprintf(str, L"*******");
						break;
					}
				}
				int strsize = wcslen(str);

				sizeInBytes = strsize * sizeof(wchar_t);
				size = static_cast<int>(sizeInBytes);
				progress_bar_value += size / 2 + 1;
				SendMessage(hBar, PBM_SETPOS, progress_bar_value, 0);
				file2 << str;
				file2 << ' ';
				file1.seekg(0, std::ios::beg);
			}
			file.close();
			file2.close();
			i++;
		}
	}
	
	
	
	file1.close();
	file2.close();
	file.close();
	ReleaseMutex(hMutex);
	return 0;
}

void CLoginWindow::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	hBar = GetDlgItem(hwnd, IDC_PROGRESS1);
	static HANDLE hThreadSize = CreateThread(NULL, 0, SizeCheck, 0, 0, NULL);
	static HANDLE hThread = CreateThread(NULL, 0, Change, hBar, 0, NULL);
	static HWND hwndParent = GetParent(hwnd);
	static HWND hEdit1 = GetDlgItem(hwndParent, IDC_EDIT1);
	static HWND hEdit2 = GetDlgItem(hwndParent, IDC_EDIT2);
	GetWindowText(hEdit1, str1, 150);
	GetWindowText(hEdit2, str2, 150);
	
	if (id == IDC_BUTTON5)
	{

		ResumeThread(hThread);
	}
	if (id == IDC_BUTTON4)
	{
		SuspendThread(hThread);
	}
	if (id == IDC_BUTTON6)
	{
		TerminateThread(hThread, 0);
	}
	CloseHandle(hThread);
}

BOOL CALLBACK CLoginWindow::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}