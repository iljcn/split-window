#include <windows.h>

#include <vector>
using namespace std;

#pragma data_seg(".shared")
HINSTANCE g_hModule = NULL;
BOOL g_bMoving = FALSE;
RECT g_rcCtrlList[100] = {0,};		// �������� �����ϸ� �޸� ������ �ȵǴ� ������ ���� �迭�� ó��..
RECT g_rcAltList[100] = {0,};
int g_rcCtrlListSize = 0;
int g_rcAltListSize = 0;
#pragma data_seg()
#pragma comment(linker, "/SECTION:.shared,RWS")

__declspec(dllexport) VOID CALLBACK WinEvtHookProc(
	HWINEVENTHOOK hWinEventHook,
	DWORD evt,
	HWND hwnd,
	LONG idObject,
	LONG idChild,
	DWORD dwEventThread,
	DWORD dwmsEventTime)
{
	DWORD dwStyle;
	POINT pt;

	if (idObject != OBJID_WINDOW || !hwnd)
        return;
	
	dwStyle  = GetWindowLong(hwnd, GWL_STYLE);
    if (dwStyle & WS_CHILD || !(dwStyle & WS_VISIBLE))
        return;

	if (evt == EVENT_SYSTEM_MOVESIZESTART) {
		//GetCursorPos(&pt);

		//POINTS hitpt = {(SHORT)pt.x, (SHORT)pt.y};
		//DWORD hitresult = (DWORD) SendMessage(hwnd, WM_NCHITTEST, NULL, *((LPARAM *) &hitpt) );

		//if (hitresult == HTCAPTION)
		g_bMoving = TRUE;
	}	
	else if (evt == EVENT_SYSTEM_MOVESIZEEND) {
		if (!g_bMoving)
			return;
		g_bMoving = FALSE;

		/* Check control key state */
		BOOL bCtrlKeyDown = GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1);
		BOOL bAltKeyDown = GetAsyncKeyState(VK_MENU)>>((sizeof(SHORT) * 8) - 1);
		int i;

		GetCursorPos(&pt);
		if (bCtrlKeyDown) {
			for (i = 0; i < g_rcCtrlListSize; i++) {
				RECT *prc = &g_rcCtrlList[i];
				if (pt.x > prc->left && pt.x < prc->right && pt.y > prc->top && pt.y < prc->bottom) {
					// �ּ�ȭ �ߴٰ� �ִ�ȭ�� �Ѱ�� ��üȭ������ ���� ������ �־ �ϴ� ���Ƶ�..
					// �ִ�ȭ �̺�Ʈ ����ä�� ���� ��ġ�� �����ϵ��� �ؾ���.
					//LONG style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
					//style |= WS_MAXIMIZE;
					//::SetWindowLongPtr(hwnd, GWL_STYLE, style);
					::SetWindowPos(hwnd, NULL, prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
				}
			}
		}
		if (bAltKeyDown) {
			for (i = 0; i < g_rcAltListSize; i++) {
				RECT *prc = &g_rcAltList[i];
				if (pt.x > prc->left && pt.x < prc->right && pt.y > prc->top && pt.y < prc->bottom) {
					::SetWindowPos(hwnd, NULL, prc->left, prc->top, prc->right - prc->left, prc->bottom - prc->top, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
				}
			}
		}
	}

}

extern "C" __declspec(dllexport) void SetRectVector(vector<RECT> *pVector, int wstype)
{
	int i;
	
	switch (wstype)
	{
	case 1:
		g_rcCtrlListSize = pVector->size();
		// Copy vector
		for (i = 0; i < g_rcCtrlListSize; i++)
			g_rcCtrlList[i] = (*pVector)[i];		
		break;
	case 2:
		g_rcAltListSize = pVector->size();
		// Copy vector
		for (i = 0; i < g_rcAltListSize; i++)
			g_rcAltList[i] = (*pVector)[i];		
		break;
	}
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpRes)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hInst;
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}