// WindowarraDlg.h : ��� ����
//
#pragma once

// class define
class CSplitBG;
class CControlDlg;

// CWindowarraDlg ��ȭ ����
class CWindowarraDlg : public CDialog
{
// �����Դϴ�.
public:
	CWindowarraDlg(CRect *pRc, CWnd* pParent = NULL);
	
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WINDOWARRA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

private:
	CSplitBG *m_pSplitBG;
	RECT *m_pRc;
	vector_rect_t *m_pVector;

	BOOL SplitChildDialog(CSplitBG *pSplitBG, RECT *baserc, unsigned int current, vector_rect_t *pVector);
	void PushtoRectVector(vector_rect_t *pVector, CSplitBG *pSplitBG);

protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
