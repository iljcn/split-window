// WindowarraDlg.h : 헤더 파일
//
#pragma once

// class define
class CSplitBG;
class CControlDlg;

// CWindowarraDlg 대화 상자
class CWindowarraDlg : public CDialog
{
// 생성입니다.
public:
	CWindowarraDlg(CRect *pRc, CWnd* pParent = NULL);
	
// 대화 상자 데이터입니다.
	enum { IDD = IDD_WINDOWARRA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	CSplitBG *m_pSplitBG;
	RECT *m_pRc;
	vector_rect_t *m_pVector;

	BOOL SplitChildDialog(CSplitBG *pSplitBG, RECT *baserc, unsigned int current, vector_rect_t *pVector);
	void PushtoRectVector(vector_rect_t *pVector, CSplitBG *pSplitBG);

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:	
	void ShowDialog(RECT *prc, vector_rect_t *pVector);
	void GetRectVector(vector_rect_t *pVector);
	BOOL WindowSplit(vector_rect_t *pVector);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};
