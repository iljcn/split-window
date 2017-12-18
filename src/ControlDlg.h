#pragma once

enum wstype_t {
	WSTYPE_NONE = 0,
	WSTYPE_CTRL,
	WSTYPE_ALT,
};

// CControlDlg ��ȭ �����Դϴ�.
class CWindowarraDlg;
class CControlDlg : public CDialog
{
	DECLARE_DYNAMIC(CControlDlg)

public:
	CControlDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CControlDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONTROL };
private:
	CWnd *m_pParent;
	CWindowarraDlg **m_pDlg;
	HMODULE m_hDll;
	HWINEVENTHOOK m_hWinEvent;NOTIFYICONDATA notify_data;

	BOOL m_bShowStatus;
	int m_nMonitorCnt;
	CRect *m_pRc;
	vector_rect_t *m_pvRectCtrl;
	vector_rect_t *m_pvRectAlt;
	wstype_t m_wstype;

	void GetMonitorInformation(void);
	void InstallHookProc(void);
	void UninstallHookProc(void);
	void WriteSettingFile(void);
	void ReadSettingFile(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtHide();
	afx_msg void OnBnClickedBtExit();
	afx_msg void OnBnClickedBtInitial();
	afx_msg LONG OnTrayIconNotify(WPARAM wparam, LPARAM lparam);
	afx_msg void OnTrayExit();
	afx_msg void OnTrayControl();
	afx_msg void OnTrayAlt();
	afx_msg void OnTraySettings();
	afx_msg void OnDestroy();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};
