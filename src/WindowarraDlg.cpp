// WindowarraDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Windowarra.h"
#include "WindowarraDlg.h"

#include "ControlDlg.h"
#include "SplitBG.h"
#include "SplitDlg.h"
#include "SSplitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWindowarraDlg 대화 상자

CWindowarraDlg::CWindowarraDlg(CRect *pRc, CWnd* pParent)
	: CDialog(CWindowarraDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSplitBG = NULL;
	m_pRc = pRc;
}

void CWindowarraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWindowarraDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()	
	ON_WM_DESTROY()	
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CWindowarraDlg 메시지 처리기

BOOL CWindowarraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// hide main dialog;
	RECT rc = {0,};
	MoveWindow(&rc, FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CWindowarraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CWindowarraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////////////////////
// private function

BOOL CWindowarraDlg::SplitChildDialog(CSplitBG *pSplitBG, RECT *baserc, unsigned int current, vector_rect_t *pVector) 
{
	unsigned int i;
	RECT rc, rc_conv, rc_base1, rc_base2;
	CPoint pt(0,0);
		
	if (pVector->size() <= current + 1)
		return TRUE;

	for (i = current; i + 1 < pVector->size(); i++)
	{
		rc_conv = ((*pVector)[i]);
		rc_conv.left -= m_pRc->left;
		rc_conv.right -= m_pRc->left;
		rc_conv.top -= m_pRc->top;
		rc_conv.bottom -= m_pRc->top;

		if (baserc->left == rc_conv.left && baserc->top == rc_conv.top) {
			// 수직으로 분할한 경우
			if (baserc->bottom == rc_conv.bottom) {
				// 화면을 수직으로 나누고
				pSplitBG->SendMessage(WM_SPLIT, SS_VERT, 0);
				// Splitter를 원래 위치로 옮긴다.
				pt.x = (rc_conv.right - rc_conv.left) + (SPLITTER_WIDTH / 2);
				pSplitBG->m_pSplitter->MoveVertPanes(pt);
				pSplitBG->m_pSplitBG1->GetClientRect(&rc);
				pSplitBG->m_pSplitter->SetLeftPaneWidth(rc.right);
			}
			// 수평으로 분할한 경우
			else if (baserc->right == rc_conv.right) {
				// 화면을 수평으로 나누고
				pSplitBG->SendMessage(WM_SPLIT, SS_HORIZ, 0);		
				// Spliiter를 원래 위치로 옮긴다.
				pt.y = (rc_conv.bottom - rc_conv.top) + (SPLITTER_WIDTH / 2);
				pSplitBG->m_pSplitter->MoveHorizPanes(pt);				
				pSplitBG->m_pSplitBG2->GetClientRect(&rc);
				pSplitBG->m_pSplitter->SetBottomPaneHeight(rc.bottom);
			}
			else
				return FALSE;

			rc_base1 = (*pVector)[i];
			rc_base2 = (*pVector)[i + 1];
			rc_base1.left -= m_pRc->left;
			rc_base1.right -= m_pRc->left;
			rc_base1.top -= m_pRc->top;
			rc_base1.bottom -= m_pRc->top;
			rc_base2.left -= m_pRc->left;
			rc_base2.right -= m_pRc->left;
			rc_base2.top -= m_pRc->top;
			rc_base2.bottom -= m_pRc->top;
			if (!SplitChildDialog(pSplitBG->m_pSplitBG1, &rc_base1, i + 1, pVector) ||
				!SplitChildDialog(pSplitBG->m_pSplitBG2, &rc_base2, i + 2, pVector))
				return FALSE;
			break;
		}
	}

	return TRUE;
}

void CWindowarraDlg::PushtoRectVector(vector_rect_t *pVector, CSplitBG *pSplitBG)
{
	RECT rc;
	if (pSplitBG->m_pSplitDlg != NULL)
		return;

	pSplitBG->m_pSplitBG1->GetWindowRect(&rc);	
	pVector->push_back(rc);
	pSplitBG->m_pSplitBG2->GetWindowRect(&rc);
	pVector->push_back(rc);
	PushtoRectVector(pVector, pSplitBG->m_pSplitBG1);
	PushtoRectVector(pVector, pSplitBG->m_pSplitBG2);
}

//////////////////////////////////////////////////////////////////////////////////////////
// user message process
BOOL CWindowarraDlg::WindowSplit(vector_rect_t *pVector)
{
	RECT rc = {0,};
	
	rc.right = m_pRc->right - m_pRc->left;
	rc.bottom = m_pRc->bottom - m_pRc->top;

	// create new dialog.
	if (m_pSplitBG) {
		m_pSplitBG->DestroyWindow();
		delete m_pSplitBG;
		m_pSplitBG = NULL;
	}

	MoveWindow(&rc, TRUE);

	m_pSplitBG = new CSplitBG(this);
	m_pSplitBG->Create(IDD_SPLITBG, this);
	m_pSplitBG->ShowWindow(SW_SHOW);
	m_pSplitBG->MoveWindow(&rc, TRUE);

	/*if (bShow) {
		MoveWindow(&rc, TRUE);
		m_pSplitBG->MoveWindow(&rc, TRUE);
	}*/

	if (!SplitChildDialog(m_pSplitBG, &rc, 0, pVector)) {
		pVector->clear();
		SplitChildDialog(m_pSplitBG, &rc, 0, pVector);
		return FALSE;
	}

	SetWindowPos(NULL, m_pRc->left, m_pRc->top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
// public
BOOL CWindowarraDlg::PreTranslateMessage(MSG* pMsg)
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

int CWindowarraDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED); 
	SetLayeredWindowAttributes(RGB(255,255,255), 200, LWA_ALPHA | LWA_COLORKEY);
	
	return 0;
}

void CWindowarraDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_pSplitBG) {
		m_pSplitBG->DestroyWindow();
		delete m_pSplitBG;
		m_pSplitBG = NULL;
	}
}

void CWindowarraDlg::GetRectVector(vector_rect_t *pVector)
{
	if (m_pSplitBG->m_pSplitDlg != NULL) {
		pVector->push_back(*m_pRc);
		return;
	}

	PushtoRectVector(pVector, m_pSplitBG);
}
