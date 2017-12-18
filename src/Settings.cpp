// Settings.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Settings.h"


// CSettingsDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSettingsDlg, CDialog)

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{

}

CSettingsDlg::~CSettingsDlg()
{
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CK_AUTOSTART, m_ckAutoStart);
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSettingsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingsDlg �޽��� ó�����Դϴ�.

BOOL CSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CSettingsDlg::PreTranslateMessage(MSG* pMsg)
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

void CSettingsDlg::OnBnClickedOk()
{
	char szFileName[MAX_PATH];

	if (m_ckAutoStart.GetCheck() == BST_CHECKED) {
		GetModuleFileName(NULL, szFileName, MAX_PATH);

		// regstry ���
	}
	
	OnOK();
}
