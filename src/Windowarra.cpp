// Windowarra.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "Windowarra.h"
//#include "WindowarraDlg.h"
#include "ControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWindowarraApp

BEGIN_MESSAGE_MAP(CWindowarraApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWindowarraApp 생성

CWindowarraApp::CWindowarraApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CWindowarraApp 개체입니다.

CWindowarraApp theApp;
CControlDlg *dlg;

// CWindowarraApp 초기화

BOOL CWindowarraApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	dlg = new CControlDlg();
	m_pMainWnd = dlg;
	dlg->Create(IDD_CONTROL, m_pMainWnd);
	dlg->ShowWindow(SW_HIDE);
	dlg->UpdateWindow();
		
	return TRUE;
}

int CWindowarraApp::ExitInstance()
{
	delete dlg;

	return CWinApp::ExitInstance();
}
