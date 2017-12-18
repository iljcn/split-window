#pragma once
//#include "foobutton.h"

// CSplitDlg 대화 상자입니다.

class CSplitDlg : public CDialog
{
	DECLARE_DYNAMIC(CSplitDlg)
private:
	HWND m_hParentWnd;
	RECT m_rect;
public:
	CSplitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPLIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CButton m_VerticalBT;
	CButton m_HorizontalBT;
public:
	afx_msg void OnBnClickedVertical();
	afx_msg void OnBnClickedHorizontal();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	RECT GetSplitDlgRect(void) { return m_rect; }
};
