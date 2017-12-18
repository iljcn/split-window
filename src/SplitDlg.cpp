// SplitDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Windowarra.h"
#include "SplitDlg.h"
#include "SSplitter.h"

#define BTN_GAP        16
#define BTN_WIDTH      100
#define BTN_HEIGHT     26

// CSplitDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSplitDlg, CDialog)

CSplitDlg::CSplitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplitDlg::IDD, pParent)	
{
	m_hParentWnd = pParent->m_hWnd;	
	memset(&m_rect, 0, sizeof(RECT));
}

void CSplitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERTICAL, m_VerticalBT);
	DDX_Control(pDX, IDC_HORIZONTAL, m_HorizontalBT);
}


BEGIN_MESSAGE_MAP(CSplitDlg, CDialog)
	ON_BN_CLICKED(IDC_VERTICAL, &CSplitDlg::OnBnClickedVertical)
	ON_BN_CLICKED(IDC_HORIZONTAL, &CSplitDlg::OnBnClickedHorizontal)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CSplitDlg 메시지 처리기입니다.

void CSplitDlg::OnBnClickedVertical()
{
	::PostMessage(m_hParentWnd, WM_SPLIT, SS_VERT, (LPARAM)this);
}

void CSplitDlg::OnBnClickedHorizontal()
{
	::PostMessage(m_hParentWnd, WM_SPLIT, SS_HORIZ, (LPARAM)this);
}

void CSplitDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rc;
	CWnd *pTextWnd;
	CString strWindowRect;
	int x,y;
	int button_gap = BTN_GAP;

	if (cy < BTN_GAP + (BTN_HEIGHT * 2))
		button_gap = 2;

	if (m_VerticalBT.m_hWnd) {
		// Vertical Button location.
		x = (cx / 2) - (BTN_WIDTH / 2);
		y = (cy / 2) - (button_gap / 2) - BTN_HEIGHT;	
		m_VerticalBT.MoveWindow(x, y, BTN_WIDTH, BTN_HEIGHT);

		// Horizontal Button location.
		y = (cy / 2) + (button_gap / 2);
		m_HorizontalBT.MoveWindow(x, y, BTN_WIDTH, BTN_HEIGHT);
	}

	GetClientRect(&rc);
	if (GetDlgItem(IDC_LINE)) {
		GetDlgItem(IDC_LINE)->MoveWindow(&rc);
		Invalidate();
	}
	
	pTextWnd = GetDlgItem(IDC_SPLIT_STATIC);
	if (pTextWnd) {
		ClientToScreen(&rc);

		RECT bgrc;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &bgrc, 0);
		strWindowRect.Format("x: %ld, y: %ld\n(%ld, %ld, %ld, %ld)",
			(rc.right - rc.left), (rc.bottom - rc.top), rc.left, rc.top, rc.right, rc.bottom);
		m_rect = rc;

		pTextWnd->MoveWindow(7, 7, 200, 30);
		pTextWnd->SetWindowText(strWindowRect);
	}
}

int CSplitDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CSplitDlg::PreTranslateMessage(MSG* pMsg)
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
