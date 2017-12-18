// WindowarraDlg.cpp : ���� ����
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

// CWindowarraDlg ��ȭ ����

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


// CWindowarraDlg �޽��� ó����

BOOL CWindowarraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// hide main dialog;
	RECT rc = {0,};
	MoveWindow(&rc, FALSE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CWindowarraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
			// �������� ������ ���
			if (baserc->bottom == rc_conv.bottom) {
				// ȭ���� �������� ������
				pSplitBG->SendMessage(WM_SPLIT, SS_VERT, 0);
				// Splitter�� ���� ��ġ�� �ű��.
				pt.x = (rc_conv.right - rc_conv.left) + (SPLITTER_WIDTH / 2);
				pSplitBG->m_pSplitter->MoveVertPanes(pt);
				pSplitBG->m_pSplitBG1->GetClientRect(&rc);
				pSplitBG->m_pSplitter->SetLeftPaneWidth(rc.right);
			}
			// �������� ������ ���
			else if (baserc->right == rc_conv.right) {
				// ȭ���� �������� ������
				pSplitBG->SendMessage(WM_SPLIT, SS_HORIZ, 0);		
				// Spliiter�� ���� ��ġ�� �ű��.
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
