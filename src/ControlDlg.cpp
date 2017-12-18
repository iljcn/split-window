// ControlDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Windowarra.h"
#include "ControlDlg.h"
#include "WindowarraDlg.h"
#include "Settings.h"

typedef void (*SetRectVector)(vector_rect_t *, int);

// CControlDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CControlDlg, CDialog)

CControlDlg::CControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControlDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_wstype = WSTYPE_NONE;
	m_bShowStatus = FALSE;
}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CControlDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_HIDE, &CControlDlg::OnBnClickedBtHide)
	ON_BN_CLICKED(IDC_BT_EXIT, &CControlDlg::OnBnClickedBtExit)
	ON_BN_CLICKED(IDC_BT_INITIAL, &CControlDlg::OnBnClickedBtInitial)
	ON_MESSAGE(WM_TRAYICON_NOTIFY, OnTrayIconNotify)
	ON_COMMAND(ID_TRAY_EXIT, &CControlDlg::OnTrayExit)
	ON_COMMAND(ID_TRAY_CONTROL, &CControlDlg::OnTrayControl)
	ON_COMMAND(ID_TRAY_ALT, &CControlDlg::OnTrayAlt)	
	ON_COMMAND(ID_TRAY_SETTINGS, &CControlDlg::OnTraySettings)
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////
// private function
void CControlDlg::GetMonitorInformation(void)
{
	
}

void CControlDlg::InstallHookProc(void)
{
	HANDLE hProc;
	
	m_hDll = LoadLibrary("./winhook.dll");
	if (m_hDll == NULL) {
		AfxMessageBox("Failed load winhook.dll, Program terminated.");
		PostMessage(WM_CLOSE);
		return;
	}

	hProc = GetProcAddress(m_hDll, "WinEvtHookProc");
	m_hWinEvent = SetWinEventHook(EVENT_SYSTEM_MOVESIZESTART, EVENT_SYSTEM_MOVESIZEEND, m_hDll, (WINEVENTPROC)hProc, 0, 0,  WINEVENT_INCONTEXT | WINEVENT_SKIPOWNPROCESS);
	if (m_hWinEvent == NULL) {
		AfxMessageBox("Unable to setup the hook procedure, Program terminated.");
		PostMessage(WM_CLOSE);
		return;
	}
}

void CControlDlg::UninstallHookProc(void)
{
	if (m_hWinEvent != NULL) {
		UnhookWinEvent(m_hWinEvent);
		m_hWinEvent = NULL;
	}

	if (m_hDll != NULL) {
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}

void CControlDlg::ReadSettingFile(void)
{
	FILE *fp;
	RECT rc;
	int i, j, current = 0;
	char buf[256];
	wstype_t wstype = WSTYPE_NONE;
	ULONG ulVersion;
	vector_rect_t vCtrl, vAlt;
	
	for (i = 0; i < m_nMonitorCnt; i++) {
		m_pvRectCtrl[i].clear();
		m_pvRectAlt[i].clear();
	}

	fp = fopen("./setting.cfg", "r");
	if (fp != NULL) {
		while (fgets(buf, 256, fp) != NULL) {
			if (buf[0] == '#' && strlen(buf) > 3) {
				if (strncmp(&buf[2], "VERSION", 7) == 0) {
					if (fgets(buf, 256, fp) == NULL)
						break;
					ulVersion = strtoul(buf, NULL, 10);
					if (ulVersion < APP_VERSION) {
						//MessageBox("Old version .cfg file. It can fail to loading.", "Warning", MB_OK | MB_ICONWARNING);
					}
				}
				else if (strncmp(&buf[2], "MONITOR", 7) == 0) {
					current = atoi(&buf[9]);
					if (current >= m_nMonitorCnt) {
						MessageBox("Monitor count dosen't correspond.", "Warning", MB_OK | MB_ICONWARNING);
						break;
					}
				}
				else if (strncmp(&buf[2], "CTRL", 4) == 0)
					wstype = WSTYPE_CTRL;
				else if (strncmp(&buf[2], "ALT", 3) == 0)
					wstype = WSTYPE_ALT;
				else
					goto onerror;

				continue;
			}

			if (sscanf(buf, "%ld,%ld,%ld,%ld", &rc.left, &rc.top, &rc.right, &rc.bottom) == 4) {				
				switch (wstype) {
				case WSTYPE_CTRL:
					m_pvRectCtrl[current].push_back(rc);  // m_vRectCtrl vector 를 만들어 넣자.
					break;
				case WSTYPE_ALT:
					m_pvRectAlt[current].push_back(rc);  // alt bector 에 넣자.
					break;
				}
			}
		}
		fclose(fp);
	}

	// 로드한 좌표가 유효한지.. 실제로 한번 만들어보는 테스트 코드.
	// 적절한 유효테스트가 생각 안나서 이렇게 함.
	//if (!WindowSplit(WSTYPE_CTRL, FALSE) || !WindowSplit(WSTYPE_ALT, FALSE))
		//goto onerror;
	
	// Set to global hook dll.	
	SetRectVector lpSetRectVector;
	lpSetRectVector = (SetRectVector)GetProcAddress(m_hDll, "SetRectVector");
	for (i = 0; i < m_nMonitorCnt; i++) {
		for (j = 0; j < (int)m_pvRectCtrl[i].size(); j++) {
			// splitter 때문에 4 pixel 만큼의 공간이 생기게 되는데 그걸 제거하는게 낫다.
			rc = m_pvRectCtrl[i][j];
			if (rc.left != m_pRc[i].left) rc.left -= 2;
			if (rc.top != m_pRc[i].top)	rc.top -= 2;
			if (rc.right != m_pRc[i].right)	rc.right += 2;
			if (rc.bottom != m_pRc[i].bottom) rc.bottom += 2;
			vCtrl.push_back(rc);
		}
		for (j = 0; j < (int)m_pvRectAlt[i].size(); j++) {
			// splitter 때문에 4 pixel 만큼의 공간이 생기게 되는데 그걸 제거하는게 낫다.
			rc = m_pvRectAlt[i][j];
			if (rc.left != m_pRc[i].left) rc.left -= 2;
			if (rc.top != m_pRc[i].top)	rc.top -= 2;
			if (rc.right != m_pRc[i].right)	rc.right += 2;
			if (rc.bottom != m_pRc[i].bottom) rc.bottom += 2;
			vAlt.push_back(rc);
		}
	}
	lpSetRectVector(&vCtrl, WSTYPE_CTRL);
	lpSetRectVector(&vAlt, WSTYPE_ALT);

	return;
onerror:
	MessageBox("Invalid setting file.", "Warning", MB_OK | MB_ICONWARNING);
	for (i = 0; i < m_nMonitorCnt; i++) {
		m_pvRectCtrl[i].clear();
		m_pvRectAlt[i].clear();
	}
}

void CControlDlg::WriteSettingFile()
{
	FILE *fp;
	size_t i;
	int m;
	char buf[256];
	
	fp = fopen("./setting.cfg", "w");
	if (fp != NULL) {
		/* Write  version */
		_snprintf(buf, 256, "# VERSION\n");
		fwrite(buf, 1, strlen(buf), fp);
		_snprintf(buf, 250, "%u\n", APP_VERSION);
		fwrite(buf, 1, strlen(buf), fp);

		for (m = 0; m < m_nMonitorCnt; m++)
		{
			/* Write Monitor number */
			_snprintf(buf, 256, "# MONITOR%d\n", m);
			fwrite(buf, 1, strlen(buf), fp);
			
			/* Write  CTRL */
			_snprintf(buf, 256, "# CTRL\n");
			fwrite(buf, 1, strlen(buf), fp);
			for (i = 0; i < m_pvRectCtrl[m].size(); i++) {
				_snprintf(buf, 256, "%ld,%ld,%ld,%ld\n",
						  m_pvRectCtrl[m][i].left,  m_pvRectCtrl[m][i].top,  m_pvRectCtrl[m][i].right, m_pvRectCtrl[m][i].bottom);
				fwrite(buf, 1, strlen(buf), fp);
			}
			/* Write  ALT */
			_snprintf(buf, 256, "# ALT\n");
			fwrite(buf, 1, strlen(buf), fp);
			for (i = 0; i < m_pvRectAlt[m].size(); i++) {
				_snprintf(buf, 256, "%ld,%ld,%ld,%ld\n",
						  m_pvRectAlt[m][i].left,  m_pvRectAlt[m][i].top,  m_pvRectAlt[m][i].right, m_pvRectAlt[m][i].bottom);
				fwrite(buf, 1, strlen(buf), fp);
			}
		}
		fclose(fp);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// 
void CControlDlg::OnBnClickedBtHide()
{
	int i, j;
	vector_rect_t *pVector, rcVector;
	RECT rc;

	switch (m_wstype) 
	{
	case WSTYPE_CTRL:
		pVector = m_pvRectCtrl;
		break;
	case WSTYPE_ALT:
		pVector = m_pvRectAlt;
		break;
	}

	for (i = 0; i < m_nMonitorCnt; i++) {	
		pVector[i].clear();
		// Get RECT vector of all dialog
		m_pDlg[i]->GetRectVector(&pVector[i]);
		m_pDlg[i]->ShowWindow(SW_HIDE);

		// It makes one vector from multiple vector.
		for (j = 0; j < (int)pVector[i].size(); j++) {
			// splitter 때문에 4 pixel 만큼의 공간이 생기게 되는데 그걸 제거하는게 낫다.
			rc = pVector[i][j];
			if (rc.left != m_pRc[i].left) rc.left -= 2;
			if (rc.top != m_pRc[i].top)	rc.top -= 2;
			if (rc.right != m_pRc[i].right)	rc.right += 2;
			if (rc.bottom != m_pRc[i].bottom) rc.bottom += 2;
			rcVector.push_back(rc);
		}
	}

	// Write rect vector to file.
 	WriteSettingFile();

	// Set to global hook dll.
	SetRectVector lpSetRectVector;
	lpSetRectVector = (SetRectVector)GetProcAddress(m_hDll, "SetRectVector");
	lpSetRectVector(&rcVector, m_wstype);

	m_bShowStatus = FALSE;
	this->ShowWindow(SW_HIDE);
}

void CControlDlg::OnBnClickedBtExit()
{
	if (MessageBox("프로그램을 종료합니다.", "확인", MB_ICONINFORMATION | MB_YESNO) == IDYES) {
		this->DestroyWindow();
	}
}

void CControlDlg::OnBnClickedBtInitial()
{
	int i;
	vector_rect_t *pVector;

	if (MessageBox("초기화 합니다.", "확인", MB_ICONINFORMATION | MB_YESNO) == IDYES) {
		switch (m_wstype) 
		{
		case WSTYPE_CTRL:
			pVector = m_pvRectCtrl;
			break;
		case WSTYPE_ALT:
			pVector = m_pvRectAlt;
			break;
		}

		for (i = 0; i < m_nMonitorCnt; i++) {
			pVector[i].clear();
			m_pDlg[i]->WindowSplit(&pVector[i]);
		}
	}
}

BOOL CControlDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return FALSE;
		break;
	default:
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DISPLAY_DEVICE dd;
	DEVMODE dm;
	int i, j;

	m_nMonitorCnt = GetSystemMetrics(SM_CMONITORS);

	m_pRc = new CRect[m_nMonitorCnt];
	m_pvRectCtrl = new vector_rect_t[m_nMonitorCnt];
	m_pvRectAlt = new vector_rect_t[m_nMonitorCnt];	

	for (i=0, j=0; j < m_nMonitorCnt; i++) {
		memset(&dd, 0, sizeof(dd));
		memset(&dm, 0, sizeof(dm));
		dd.cb = sizeof(dd);

		if (i == 0) {
			// 첫번째 모니터는 작업표시줄을 빼고 구하는게 낫다.
			SystemParametersInfo(SPI_GETWORKAREA, 0, &m_pRc[i], 0);
			j++;
		}
		else {
			EnumDisplayDevices(NULL, i, &dd, 0);
			if (EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm)) {
				m_pRc[j].left = dm.dmPosition.x;
				m_pRc[j].top = dm.dmPosition.y;
				m_pRc[j].right = dm.dmPosition.x + dm.dmPelsWidth;
				m_pRc[j].bottom = dm.dmPosition.y + dm.dmPelsHeight;
				j++;
			}
		}
	}

	InstallHookProc();

	// Load setting.cfg file.
	ReadSettingFile();

	// 트레이아이콘 :http://blog.daum.net/alwaysnr/5 블로그 참조함
	notify_data.cbSize = sizeof(notify_data);
	notify_data.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	notify_data.hWnd = this->m_hWnd;
	strcpy(notify_data.szTip, "SplitWindow" );
	notify_data.uCallbackMessage = WM_TRAYICON_NOTIFY;
	notify_data.uFlags = NIF_TIP | NIF_MESSAGE | NIF_ICON;
	notify_data.uID = IDR_MAINFRAME;
	notify_data.uVersion = NOTIFYICON_VERSION;
    notify_data.dwInfoFlags = NIIF_USER;   
	::Shell_NotifyIcon(NIM_ADD, &notify_data);

	// create WindowarraDlg dialogs.
	m_pDlg = new CWindowarraDlg*[m_nMonitorCnt];
	for (i = 0; i < m_nMonitorCnt; i++) {
		m_pDlg[i] = new CWindowarraDlg(&m_pRc[i]);
		m_pDlg[i]->Create(IDD_WINDOWARRA_DIALOG, GetDesktopWindow());
		m_pDlg[i]->ShowWindow(SW_HIDE);
		m_pDlg[i]->UpdateWindow();
	}

	RECT rc;
	GetWindowRect(&rc);
	SetWindowPos(&wndTopMost, 200, 200, rc.right - rc.left, rc.bottom - rc.top, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LONG CControlDlg::OnTrayIconNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDR_MAINFRAME) // notify_data.uID = IDR_MAINFRAME 를 사용
	{
		if (lParam == WM_LBUTTONDOWN || lParam == WM_RBUTTONDOWN)
		{
			POINT pos;
			CMenu menu;
			menu.LoadMenu(IDR_TRAYPOPUP);
			::GetCursorPos(&pos);                     
			::SetForegroundWindow(this->GetSafeHwnd());
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pos.x, pos.y, this);
		}
	}

	return 0;
}

void CControlDlg::OnTrayControl()
{
	m_wstype = WSTYPE_CTRL;

	for (int i = 0; i < m_nMonitorCnt; i++) {
		m_pDlg[i]->WindowSplit(&m_pvRectCtrl[i]);
		m_pDlg[i]->ShowWindow(SW_SHOW);
	}
	m_bShowStatus = TRUE;
	this->ShowWindow(SW_SHOW);	
}

void CControlDlg::OnTrayAlt()
{
	m_wstype = WSTYPE_ALT;

	for (int i = 0; i < m_nMonitorCnt; i++) {
		m_pDlg[i]->WindowSplit(&m_pvRectAlt[i]);
		m_pDlg[i]->ShowWindow(SW_SHOW);
	}
	m_bShowStatus = TRUE;
	this->ShowWindow(SW_SHOW);
}

void CControlDlg::OnTraySettings()
{	
	CSettingsDlg pSettings;
	pSettings.DoModal();
}

void CControlDlg::OnTrayExit()
{
	this->DestroyWindow();
}

void CControlDlg::OnDestroy()
{
	CDialog::OnDestroy();

	int i;

	UninstallHookProc();

	NOTIFYICONDATA nodifyicondata = {0,};
	nodifyicondata.hWnd = this->GetSafeHwnd();
	nodifyicondata.uID = IDR_MAINFRAME;
	::Shell_NotifyIcon(NIM_DELETE, &nodifyicondata);

	for (i = 0; i < m_nMonitorCnt; i++) {
		m_pDlg[i]->DestroyWindow();
		delete m_pDlg[i];
	}
	delete m_pDlg;

	delete [] m_pRc;
	delete [] m_pvRectCtrl;
	delete [] m_pvRectAlt;
}

void CControlDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanging(lpwndpos);

	if (m_bShowStatus)
		lpwndpos->flags |= SWP_SHOWWINDOW;
    else
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
}