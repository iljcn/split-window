// Windowarra.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CWindowarraApp:
// �� Ŭ������ ������ ���ؼ��� Windowarra.cpp�� �����Ͻʽÿ�.
//

class CWindowarraApp : public CWinApp
{
public:
	CWindowarraApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CWindowarraApp theApp;