//main.cpp
#include "std.h"

#define PROGRAM_WIDTH 900
#define PROGRAM_HEIGHT 670



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void WinCheck(HWND hwnd);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	TCHAR ProgramTitle[128] = L"����";


	// -1�� �ʱ�ȭ
	memset(panStatus, -1, sizeof(panStatus));


	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = ProgramTitle;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(ProgramTitle, ProgramTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, PROGRAM_WIDTH, PROGRAM_HEIGHT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	HACCEL hAccel;

	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));
	while (GetMessage(&Message, 0, 0, 0)) {
		if (!TranslateAccelerator(hWnd, hAccel, &Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	int cx = 0, cy = 0;
	TCHAR buf[128];

	switch (iMessage) {
	case WM_CREATE:
	{
		LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;
		//lParam �� ���� LPCRATESTRUCT ����ü �����ͷ�

		hWhite = (HBITMAP)LoadImage(cs->hInstance, L"image//white.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBlack = (HBITMAP)LoadImage(cs->hInstance, L"image//black.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// pt �ʱ�ȭ
		pt.x = 0;
		pt.y = 0;

		//����Ʈ �ڽ� �߰�
		hList = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
			LBS_NOTIFY, 620, 50, 255, 440, hwnd, (HMENU)listbox, g_hInst, NULL);

		CreateWindow(L"button", L"�� ���� ����", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
			620, 550, 255, 50, hwnd, (HMENU)newgame, g_hInst, NULL);

		CreateWindow(L"button", L"�� �� ������", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
			620, 495, 255, 50, hwnd, (HMENU)back, g_hInst, NULL);

		MoveWindow(hwnd, 100, 100, PROGRAM_WIDTH, PROGRAM_HEIGHT, TRUE);
	}
	break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO *)lParam)->ptMaxTrackSize.x = PROGRAM_WIDTH;
		((MINMAXINFO *)lParam)->ptMaxTrackSize.y = PROGRAM_HEIGHT;

		((MINMAXINFO *)lParam)->ptMinTrackSize.x = PROGRAM_WIDTH;
		((MINMAXINFO *)lParam)->ptMinTrackSize.y = PROGRAM_HEIGHT;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case newgame:
			initGame(&hwnd);

			break;

		case ID_NEWGAME:
			initGame(&hwnd);
			break;

		case back:
			if (WinStatus != true) {
				if (list.size() == 0) {
					MessageBox(hwnd, L"�� ���� ������ ���� �ʾұ� ������ ���� �� �����ϴ�.", L"�˸�", MB_OK);
				}
				else {
					list.pop_back();
					stone = !stone;
					SubBackListStatus();
					InvalidateRect(hwnd, NULL, true);
				}
			}
			else {
				MessageBox(hwnd, L"������ ���� �����ϼ���.", L"�˸�", MB_OK);
			}
			break;

		case ID_BACKSTONE:
			SendMessage(hwnd, WM_COMMAND, back, 0);
			break;

		case ID_EXIT:
			SendMessage(hwnd, WM_DESTROY, 0, 0);
		}
		break;

	case WM_LBUTTONDOWN:
	{
		if (WinStatus == false) {

			pt = MAKEPOINTS(lParam);
			HDC hdc = GetDC(hwnd);
			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP old;
			int countX, countY;
			save temp;

			//���࿡ ���� �� �������� ���а� �ȳ��ٸ�
			if (defeatCount == 0) {
				MessageBox(hwnd, L"���º��Դϴ�.", L"���� ���", MB_OK);
			}


			// �ܺ� ó��
			if ((pt.x < 20) || (pt.x >= 590) || (pt.y < 20) || (pt.y >= 590)) {
				break;
			}

			// ��ǥ�� ī��Ʈ
			countX = (pt.x + 20) / 30;
			countY = (pt.y + 20) / 30;

			pt.x = (30 * countX) - 23;
			pt.y = (30 * countY) - 23;

			temp.pt.x = pt.x;
			temp.pt.y = pt.y;

			// �ߺ�üũ
			if (panStatus[countX - 1][countY - 1] != -1) {
				break;
			}

			// �ѵ��� �ٲ㼭 ���
			if (stone) { // true�̸� ������

				temp.stone = true;

				list.push_back(temp);
				old = (HBITMAP)SelectObject(memDC, hBlack);
				panStatus[countX - 1][countY - 1] = 1;

				AddListStatus(countX - 1, countY - 1);

				// ī��Ʈ ����
				defeatCount -= 1;

			}
			else {

				temp.stone = false;
				list.push_back(temp);
				old = (HBITMAP)SelectObject(memDC, hWhite);
				panStatus[countX - 1][countY - 1] = 2;

				AddListStatus(countX - 1, countY - 1);

				//ī��Ʈ ����
				defeatCount -= 1;
			}

			// ���� �ٵϾ�
			TransparentBlt(hdc, pt.x, pt.y, 29, 29, memDC, 0, 0, 29, 29, RGB(255, 0, 0));

			// ��鵹 ü����
			stone = !stone;
			SelectObject(memDC, old);

			// ���� ���� ���� üũ
			WinCheck(hwnd);

			ReleaseDC(hwnd, hdc);

		}

		else {

		}
	}
	InvalidateRect(hwnd, NULL, false);
	break;

	case WM_PAINT:
	{


		HDC hdc = BeginPaint(hwnd, &ps);



		wsprintf(buf, L"pt.x : %d  pt.Y : %d  �� �� : %d                     ", pt.x, pt.y, defeatCount);
		TextOut(hdc, 620, 0, buf, wcslen(buf));

		if (stone) {
			wsprintf(buf, L"���� ���� : ��");
			TextOut(hdc, 620, 20, buf, wcslen(buf));
		}
		else {
			wsprintf(buf, L"���� ���� : ��");
			TextOut(hdc, 620, 20, buf, wcslen(buf));
		}

		HBITMAP Bitmap, oldBitmap;
		HDC memDC = CreateCompatibleDC(hdc);

		// �ٵ��� �׸���
		Bitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PAN));
		oldBitmap = (HBITMAP)SelectObject(memDC, Bitmap);
		BitBlt(hdc, 0, 0, 610, 610, memDC, 0, 0, SRCCOPY);

		// 
		for (int i = 0; i < list.size(); i++) {
			if (list[i].stone) {
				Bitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BLACK));
				oldBitmap = (HBITMAP)SelectObject(memDC, Bitmap);
			}
			else {
				Bitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_WHITE));
				oldBitmap = (HBITMAP)SelectObject(memDC, Bitmap);
			}
			TransparentBlt(hdc, list[i].pt.x, list[i].pt.y, 29, 29, memDC, 0, 0, 29, 29, RGB(255, 0, 0));
		}



		SelectObject(memDC, Bitmap);
		DeleteObject(oldBitmap);
		DeleteDC(memDC);
		EndPaint(hwnd, &ps);

	}
	return 0;


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}

void WinCheck(HWND hwnd) {
	//����
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (panStatus[j][i] == 1 && panStatus[j + 1][i] == 1 && panStatus[j + 2][i] == 1 && panStatus[j + 3][i] == 1 && panStatus[j + 4][i] == 1) {
				MessageBox(hwnd, L"���� �¸��Դϴ�.", L"�¸� : ��", MB_OK);
				WinStatus = true;
				PrintWin();
				break;
			}
			else if (panStatus[j][i] == 2 && panStatus[j + 1][i] == 2 && panStatus[j + 2][i] == 2 && panStatus[j + 3][i] == 2 && panStatus[j + 4][i] == 2)
			{
				MessageBox(hwnd, L"���� �¸��Դϴ�.", L"�¸� : ��", MB_OK);
				WinStatus = true;
				PrintWin();
				break;
			}
		}
	}

	//����
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			if (panStatus[i][j] == 1 && panStatus[i][j + 1] == 1 && panStatus[i][j + 2] == 1 && panStatus[i][j + 3] == 1 && panStatus[i][j + 4] == 1) {
				MessageBox(hwnd, L"���� �¸��Դϴ�.", L"�¸� : ��", MB_OK);
				WinStatus = true;
				PrintWin();
				break;
			}
			else if (panStatus[i][j] == 2 && panStatus[i][j + 1] == 2 && panStatus[i][j + 2] == 2 && panStatus[i][j + 3] == 2 && panStatus[i][j + 4] == 2)
			{
				MessageBox(hwnd, L"���� �¸��Դϴ�.", L"�¸� : ��", MB_OK);
				WinStatus = true;
				PrintWin();
				break;
			}
		}
	}

	// �� �밢��
	for (int i = 0; i < 20; i++) {
		for (int j = 0, k = 0; j < 20; j++, k++)
			if (panStatus[j][i] == 1 && panStatus[j + 1][i + 1] == 1 && panStatus[j + 2][i + 2] == 1 && panStatus[j + 3][i + 3] == 1 && panStatus[j + 4][i + 4] == 1) {
				MessageBox(hwnd, L"���� �¸��Դϴ�.", L"�¸� : ��", MB_OK);
				WinStatus = true;
				PrintWin();
				break;
			}
			else if (panStatus[j][i] == 2 && panStatus[j + 1][i + 1] == 2 && panStatus[j + 2][i + 2] == 2 && panStatus[j + 3][i + 3] == 2 && panStatus[j + 4][i + 4] == 2)
			{
				MessageBox(hwnd, L"���� �¸��Դϴ�.", L"�¸� : ��", MB_OK);
				WinStatus = true;
				PrintWin();
				break;
			}

	}
	// �´밢��
	for (int i = 0; i < 20; i++) {
		for (int j = 0, k = 0; j < 20; j++, k++)
			if (panStatus[j][i] == 1 && panStatus[j - 1][i + 1] == 1 && panStatus[j - 2][i + 2] == 1 && panStatus[j - 3][i + 3] == 1 && panStatus[j - 4][i + 4] == 1) {
				MessageBox(hwnd, L"���� �¸��Դϴ�.", L"�¸� : ��", MB_OK);
				WinStatus = true;
				PrintWin();
				break;
			}
			else if (panStatus[j][i] == 2 && panStatus[j - 1][i + 1] == 2 && panStatus[j - 2][i + 2] == 2 && panStatus[j - 3][i + 3] == 2 && panStatus[j - 4][i + 4] == 2)
			{
				MessageBox(hwnd, L"���� �¸��Դϴ�.", L"�¸� : ��", MB_OK);
				WinStatus = true;
				PrintWin();
				break;
			}

	}
}


void AddListStatus(int x, int y) {

	TCHAR buf[128];
	int count;
	if (stone) {
		wsprintf(buf, L"�浹�� [%d,%d]�� �����Ͽ����ϴ�.", x, y);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buf);
	}
	else {
		wsprintf(buf, L"�鵹�� [%d,%d]�� �����Ͽ����ϴ�.", x, y);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buf);
	}

	count = SendMessage(hList, LB_GETCOUNT, 0, 0);
	SendMessage(hList, LB_SETCARETINDEX, (LPARAM)count, 1);

}

void AddListStatus(int x, int y, bool status) {

	TCHAR buf[128];
	int count;
	if (status) {
		wsprintf(buf, L"�浹�� [%d,%d]�� �����Ͽ����ϴ�.", x, y);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buf);
	}
	else {
		wsprintf(buf, L"�鵹�� [%d,%d]�� �����Ͽ����ϴ�.", x, y);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buf);
	}

	count = SendMessage(hList, LB_GETCOUNT, 0, 0);
	SendMessage(hList, LB_SETCARETINDEX, (LPARAM)count, 1);

}


void SubBackListStatus() {
	SendMessage(hList, LB_RESETCONTENT, 0, 0);

	for (int i = 0; i < list.size(); i++) {
		AddListStatus(((list[i].pt.x+20)/30), ((list[i].pt.y + 20) / 30), list[i].stone);
	}
	defeatCount++;
}


void PrintWin() {

	TCHAR buf[128];

	if (stone) {
		wsprintf(buf, L"�鵹�� �¸��Ͽ����ϴ�.");
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buf);
	}
	else {
		wsprintf(buf, L"�浹�� �¸��Ͽ����ϴ�.");
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buf);
	}


	int count;
	count = SendMessage(hList, LB_GETCOUNT, 0, 0);
	SendMessage(hList, LB_SETCARETINDEX, (LPARAM)count, 1);
}

void initGame(HWND *hwnd) {

	list.clear();
	defeatCount = 400;
	WinStatus = false;
	pt.x = 0;
	pt.y = 0;

	InvalidateRect(*hwnd, NULL, true);

	memset(panStatus, -1, sizeof(panStatus));

	SendMessage(hList, LB_RESETCONTENT, 0, 0);

}