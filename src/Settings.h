#pragma once

#include "resource.h"
#include "afxwin.h"

// CSettingsDlg ��ȭ �����Դϴ�.

class CSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingsDlg)

public:
	CSettingsDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSettingsDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_ckAutoStart;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
};
