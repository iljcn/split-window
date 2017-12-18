#pragma once

class  CSplitDlg;
class  CSSplitter;
class CSplitBG : public CDialog
{
	DECLARE_DYNAMIC(CSplitBG)

public:
	CSplitDlg *m_pSplitDlg;
	CSSplitter *m_pSplitter;
	CSplitBG *m_pSplitBG1;
	CSplitBG *m_pSplitBG2;
public:
	CSplitBG(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSplitBG();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPLITBG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LONG OnSplit(WPARAM wparam, LPARAM lparam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
