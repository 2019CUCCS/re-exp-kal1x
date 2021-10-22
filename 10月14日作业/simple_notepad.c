#include <windows.h> 

// Global variable 

HINSTANCE hinst;

// Function prototypes. 

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
InitApplication(HINSTANCE);
InitInstance(HINSTANCE, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

#define IDM_EDUNDO  0 
#define IDM_EDCUT   1 
#define IDM_EDCOPY  2
#define IDM_EDPASTE 3
#define IDM_EDDEL   4
#define IDM_ABOUT   5
//�����ļ��Ĵ򿪺ͱ������
#define IDM_OPENFILE 6
#define IDM_SAVE 7

// Application entry point. 

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if (!InitApplication(hinstance))
		return FALSE;

	if (!InitInstance(hinstance, nCmdShow))
		return FALSE;

	BOOL fGotMessage;
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	UNREFERENCED_PARAMETER(lpCmdLine);
}

BOOL InitApplication(HINSTANCE hinstance)
{
	WNDCLASSEX wcx;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	wcx.cbSize = sizeof(wcx);          // size of structure 
	wcx.style = CS_HREDRAW |
		CS_VREDRAW;                    // redraw if size changes 
	wcx.lpfnWndProc = MainWndProc;     // points to window procedure 
	wcx.cbClsExtra = 0;                // no extra class memory 
	wcx.cbWndExtra = 0;                // no extra window memory 
	wcx.hInstance = hinstance;         // handle to instance 
	wcx.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);              // predefined app. icon 
	wcx.hCursor = LoadCursor(NULL,
		IDC_ARROW);                    // predefined arrow 
	wcx.hbrBackground = GetStockObject(
		WHITE_BRUSH);                  // white background brush 
	wcx.lpszMenuName = "MainMenu";    // name of menu resource 
	wcx.lpszClassName = "MainWClass";  // name of window class 
	wcx.hIconSm = LoadImage(hinstance, // small class icon 
		MAKEINTRESOURCE(5),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	// Register the window class. 

	return RegisterClassEx(&wcx);
}

BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
{
	HWND hwnd;

	// Save the application-instance handle. 

	hinst = hinstance;

	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",        // name of window class 
		"�й���ý��ѧ",            // title-bar string 
		WS_OVERLAPPEDWINDOW, // top-level window 
		0,       // default horizontal position 
		0,       // default vertical position 
		800,       // default width 
		500,       // default height 
		(HWND)NULL,         // no owner window 
		(HMENU)NULL,        // use class menu 
		hinstance,           // handle to application instance 
		(LPVOID)NULL);      // no window-creation data 

	if (!hwnd)
		return FALSE;

	// Show the window and send a WM_PAINT message to the window 
	// procedure. 

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return TRUE;

}

#define ID_EDITCHILD 100
HWND hwndEdit;

LRESULT CALLBACK MainWndProc(HWND hwnd,      // window handle 
	UINT message,   // type of message 
	WPARAM wParam,  // additional information 
	LPARAM lParam)  // additional information 
{
	HMENU hMenu;

	CHAR lpszLatin[] = "��ѧ����";

	switch (message)
	{
	case WM_CREATE:

		hMenu = CreateMenu();
		if (hMenu != NULL)
		{
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_OPENFILE, "��");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_SAVE, "����");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDUNDO, "����");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDCUT, "����");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDCOPY, "����");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDPASTE, "ճ��");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_EDDEL, "ɾ��");
			InsertMenuA(hMenu, -1, MF_BYPOSITION, IDM_ABOUT, "����");


			SetMenu(hwnd, hMenu);
		}
		hwndEdit = CreateWindowEx(
			0, "EDIT",   // predefined class 
			NULL,         // no window title 
			WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			0, 0, 200, 100,   // set size in WM_SIZE message 
			hwnd,         // parent window 
			(HMENU)ID_EDITCHILD,   // edit control ID 
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);        // pointer not needed 

						  // Add text to the window. 
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)lpszLatin);

		return 0;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDM_EDUNDO:
			// Send WM_UNDO only if there is something to be undone. 

			if (SendMessage(hwndEdit, EM_CANUNDO, 0, 0))
				SendMessage(hwndEdit, WM_UNDO, 0, 0);
			else
			{
				MessageBox(hwndEdit,
					"Nothing to undo.",
					"Undo notification",
					MB_OK);
			}
			break;

		case IDM_EDCUT:
			SendMessage(hwndEdit, WM_CUT, 0, 0);
			break;

		case IDM_EDCOPY:
			SendMessage(hwndEdit, WM_COPY, 0, 0);
			break;

		case IDM_EDPASTE:
			SendMessage(hwndEdit, WM_PASTE, 0, 0);
			break;

		case IDM_EDDEL:
			SendMessage(hwndEdit, WM_CLEAR, 0, 0);
			break;

		case IDM_ABOUT:
			MessageBoxA(hwnd, "��ѧ����1.0", "��ѧ����1.0", MB_OK);
			break;

		case IDM_OPENFILE:	
			//DoFileOpenSave�����ڶ�����������ΪFALSE��ʱ���Ǵ��ļ�
			DoFileOpenSave(hwndEdit, FALSE);			
			break;
					 
		case IDM_SAVE: 
			//DoFileOpenSave�����ڶ�����������ΪSAVE��ʱ���Ǳ����ļ�
			DoFileOpenSave(hwndEdit, TRUE);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);

        
		}
		break;

	case WM_SETFOCUS:
		SetFocus(hwndEdit);
		return 0;

	case WM_SIZE:
		// Make the edit control the size of the window's client area. 

		MoveWindow(hwndEdit,
			0, 0,                  // starting x- and y-coordinates 
			LOWORD(lParam),        // width of client area 
			HIWORD(lParam),        // height of client area 
			TRUE);                 // repaint window 
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}


//�Զ��庯��

BOOL LoadFile(HWND hEdit, LPSTR pszFileName)
{//���ڼ����ļ�
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize;
		dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != 0xFFFFFFFF)
		{
			LPSTR pszFileText;
			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (pszFileText != NULL)
			{
				DWORD dwRead;
				if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{
					pszFileText[dwFileSize] = 0;
					if (SetWindowText(hEdit, pszFileText))
						bSuccess = TRUE;
				}
				GlobalFree(pszFileText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}


BOOL SaveFile(HWND hEdit, LPSTR pszFileName)
{//���ڱ����ļ�
	HANDLE hFile;
	BOOL bSuccess = FALSE;
	hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength;
		dwTextLength = GetWindowTextLength(hEdit);
		if (dwTextLength > 0)
		{
			LPSTR pszText;
			pszText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
			if (pszText != NULL)
			{
				if (GetWindowText(hEdit, pszText, dwTextLength + 1))
				{
					DWORD dwWritten;
					if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
						bSuccess = TRUE;
				}
				GlobalFree(pszText);
			}
		}
		CloseHandle(hFile);
	}
	return bSuccess;
}


BOOL DoFileOpenSave(HWND hwnd, BOOL bSave){
	OPENFILENAME ofn;
	char szFileName[MAX_PATH];


	ZeroMemory(&ofn, sizeof(ofn));
	szFileName[0] = 0;


	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "�ı��ļ� (*.txt)\0*.txt\0�����ļ� (*.*)\0*.*\0\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "txt";//ָ���ļ���׺Ϊtxt


	if (bSave){
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
			OFN_OVERWRITEPROMPT;

		if (GetSaveFileName(&ofn))
		{
			//����ָ���Ի����пؼ��ľ����
			GetDlgItem(hwnd, IDM_SAVE);
			if (!SaveFile(hwnd, szFileName))
			{
				MessageBox(hwnd, "�����ļ�ʧ��.", "������Ϣ",
					MB_OK | MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
	}
	else{
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		if (GetOpenFileName(&ofn))
		{
			//����ָ���Ի����пؼ��ľ����
			GetDlgItem(hwnd, IDM_OPENFILE);
			if (!LoadFile(hwnd, szFileName))
			{
				MessageBox(hwnd, "���ļ�ʧ��.", "������Ϣ",
					MB_OK | MB_ICONEXCLAMATION);
				return FALSE;
			}
		}
	}
	return TRUE;
}