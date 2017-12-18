// SplitBG.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Windowarra.h"
#include "SplitBG.h"
#include "SplitDlg.h"
#include "SSplitter.h"

// CSplitBG ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSplitBG, CDialog)

CSplitBG::CSplitBG(CWnd* pParent /*=NULL*/)
	: CDialog(CSplitBG::IDD, pParent)
{
	m_pSplitter = NULL;
	m_pSplitDlg = NULL;
	m_pSplitBG1 = NULL;
	m_pSplitBG2 = NULL;
}

CSplitBG::~CSplitBG()
{
	if (m_pSplitDlg) {
		m_pSplitDlg->DestroyWindow();
		delete m_pSplitDlg;
		m_pSplitDlg = NULL;
	}
	if (m_pSplitter) {
		m_pSplitter->DestroyWindow();
		delete m_pSplitter;
		m_pSplitter = NULL;
	}
	if (m_pSplitBG1) {
		m_pSplitBG1->DestroyWindow();
		delete m_pSplitBG1;
		m_pSplitBG1 = NULL;
	}
	if (m_pSplitBG2) {
		m_pSplitBG2->DestroyWindow();
		delete m_pSplitBG2;
		m_pSplitBG2 = NULL;
	}
}

void CSplitBG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSplitBG, CDialog)
	ON_MESSAGE(WM_SPLIT, OnSplit)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSplitBG �޽��� ó�����Դϴ�.

BOOL CSplitBG::OnInitDialog()
{
	CDialog::OnInitDialog();
	RECT rc;
	
	m_pSplitDlg = new CSplitDlg(this);
	m_pSplitDlg->Create(IDD_SPLIT, this);

	GetWindowRect(&rc);

	m_pSplitDlg->ShowWindow(SW_SHOW);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

LONG CSplitBG::OnSplit(WPARAM wParam, LPARAM lParam)
{
	if (m_pSplitDlg) {
		m_pSplitDlg->DestroyWindow();
		delete m_pSplitDlg;
		m_pSplitDlg = NULL;
	}
		
	int sstyle;
	RECT rc;
	
	GetClientRect(&rc);
	switch (wParam)
	{
	case SS_VERT: // Vertical
		sstyle = SS_VERT;
		break;
	case SS_HORIZ: // Horizontal
		sstyle = SS_HORIZ;
		break;
	default:
		return NULL;
	}

	m_pSplitBG1 = new CSplitBG(this);
	m_pSplitBG1->Create(IDD_SPLITBG, this);
	m_pSplitBG1->ShowWindow(SW_SHOW);

	m_pSplitBG2 = new CSplitBG(this);
	m_pSplitBG2->Create(IDD_SPLITBG, this);
	m_pSplitBG2->ShowWindow(SW_SHOW);

	m_pSplitter = new CSSplitter;
	if (!m_pSplitter->Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | sstyle,
		this, m_pSplitBG1, m_pSplitBG2, 0, rc, 10, 10)) {
		TRACE("Failed SSplitter->Create()");
		return NULL;
	}

	return 0;
}

void CSplitBG::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);	

	if (m_pSplitDlg) {
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = cx;
		rc.bottom = cy;
		m_pSplitDlg->MoveWindow(&rc);
	}

	if (m_pSplitter)
		m_pSplitter->MoveWindow(0,0,cx,cy);
}

BOOL CSplitBG::PreTranslateMessage(MSG* pMsg)
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
