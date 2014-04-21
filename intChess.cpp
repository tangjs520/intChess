/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : intChess.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-20
�ļ�����    : 
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#include "stdafx.h"
#include "intChess.h"

#include "MainFrm.h"
#include "intChessDoc.h"
#include "intChessView.h"

#pragma warning(disable : 4201)
#include <Mmsystem.h>			//����API����sndPlaySound��ͷ�ļ�
#pragma warning(default : 4201)

#include "ChessRule.h"			//���������(CChessRule)��ͷ�ļ�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//��ʼ��ȫ�ֱ���
BYTE CIntChessApp::g_nOSType = WIN9X_WINNT;

CDC* CIntChessApp::g_pWhiteChessManBufferDC = NULL;
CDC* CIntChessApp::g_pBlackChessManBufferDC = NULL;

CDC* CIntChessApp::g_pWhiteChessManMaskDC = NULL;
CDC* CIntChessApp::g_pBlackChessManMaskDC = NULL;

long CIntChessApp::g_nBitmapWidth = 0L;
long CIntChessApp::g_nBitmapHeight = 0L;

CString CIntChessApp::g_strWhitePlayerName = _T("");
CString CIntChessApp::g_strBlackPlayerName = _T("");

float CIntChessApp::g_fWhitePlayerScore = 0.0;
float CIntChessApp::g_fBlackPlayerScore = 0.0;

BEGIN_MESSAGE_MAP(CIntChessApp, CWinApp)
	//{{AFX_MSG_MAP(CIntChessApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/******************************************************************************************
CIntChessApp::CIntChessApp()
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessApp::CIntChessApp
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CIntChessApp::CIntChessApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CIntChessApp theApp;

/******************************************************************************************
BOOL CIntChessApp::InitInstance()
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessApp::InitInstance
����ֵ    : BOOL
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CIntChessApp::InitInstance()
{
	//���ȼ�����ϵͳ������(֮����Ҫ������ϵͳ�����ͣ�
	//����Ϊ���ӵĻ��Ʋ���[CChessMan::Draw]�������ϵͳ�Ĳ�ͬ������ȡ��ͬ�Ļ��Ʒ�����
	//Ϊ��Ҫ���ò�ͬ�Ļ��Ʒ�������μ�CChessMan::Draw������˵��)
	if (!CheckOSVersion(g_nOSType))
	{
		AfxMessageBox(IDS_GETVERSION_ERR);
		return FALSE;
	}

	//Ȼ���ж���ʾ���ķֱ����Ƿ����Ӧ�ó�������Ҫ��
	if (!CheckDisplayResolution())
	{
		AfxMessageBox(IDS_GETRESOLUTION_ERR);
		return FALSE;
	}

	//����û����������ֵ���������Ĭ��ʹ������������
	g_strWhitePlayerName.LoadString(IDS_DEFAULT_WHITE_PLAYER_NAME);
	g_strBlackPlayerName.LoadString(IDS_DEFAULT_BLACK_PLAYER_NAME);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CIntChessDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CIntChessView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileType(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//����û�ͨ��ֱ��˫������ļ�������Ӧ�ó�������Ҫ������Ҫ�򿪵��ļ�·������
	//��ʱ�����������ȵ�Ӧ�ó���Ĵ���ȫ����ʾ�����Ժ��ٽ��д��ļ��Ĳ���
	CString strOpenFileName = _T("");
	if (CCommandLineInfo::FileOpen == cmdInfo.m_nShellCommand)
	{
		strOpenFileName = cmdInfo.m_strFileName;
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
	}

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
    {
        return FALSE;
    }

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	if (!strOpenFileName.IsEmpty())
	{
		OpenDocumentFile(strOpenFileName);
	}

	return TRUE;
}

/******************************************************************************************
BOOL CIntChessApp::CheckOSVersion(BYTE& nOSType)
����      : tangjs520       ��������: 2003-10-20
������    : CIntChessApp::CheckOSVersion
����ֵ    : BOOL
�����б�  : 
  ����1: BYTE& nOSType
����      : ������ϵͳ������
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CIntChessApp::CheckOSVersion(BYTE& nOSType)
{
	//�趨OSVERSIONINFO�Ĵ�С
	OSVERSIONINFO versionInfo = { sizeof(OSVERSIONINFO) };

	//��ȡ�汾��Ϣ
	if (!::GetVersionEx(&versionInfo))
	{
		return FALSE;	//��ȡ�汾��Ϣʧ��
	}

	//�����NTϵ�У������WIN2K_WINXP����WINNT
	if (VER_PLATFORM_WIN32_NT == versionInfo.dwPlatformId)
	{
		//���汾��
		if (versionInfo.dwMajorVersion >= 5)
		{
			nOSType = WIN2K_WINXP;	//Windows2000��WindowsXP
		}
		else
		{
			nOSType = WIN9X_WINNT;	//WindowsNT
		}
	}
	else if (VER_PLATFORM_WIN32_WINDOWS == versionInfo.dwPlatformId)
	{
		nOSType = WIN9X_WINNT;
		/*
		if ((versionInfo.dwMajorVersion > 4) || ((4 == versionInfo.dwMajorVersion)
			&& (versionInfo.dwMinorVersion > 0)))
		{
			nOSType = WIN9X_WINNT;	//Windows98��WindowsMe
		}
		else
		{
			return FALSE;	//Windows95��Ӧ�ó�����ܲ�֧�ָò���ϵͳ
		}
		*/
	}
	else
	{
		return FALSE;	//Windows 3.1��Ӧ�ó���֧�ָò���ϵͳ
	}

	return TRUE;
}

/******************************************************************************************
BOOL CIntChessApp::CheckDisplayResolution()
����      : tangjs520       ��������: 2003-10-20
������    : CIntChessApp::CheckDisplayResolution
����ֵ    : BOOL
�����б�  : ��
����      : �����ʾ���ķֱ���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CIntChessApp::CheckDisplayResolution()
{
	int nWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nHeight = ::GetSystemMetrics(SM_CYSCREEN);

	if (nWidth < 1024 || nHeight < 768)	//Ӧ�ó�����Ҫ��ʾ���ķֱ�������Ϊ1024��768
	{
		return FALSE;
	}

	return TRUE;
}

/******************************************************************************************
void CIntChessApp::InstallChessManDC(CWnd* pParentWnd)
����      : tangjs520       ��������: 2003-10-20
������    : CIntChessApp::InstallChessManDC
����ֵ    : void
�����б�  : 
  ����1: CWnd* pParentWnd
����      : ��������λͼ����Ӧ�ڴ�DC��
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CIntChessApp::InstallChessManDC(CWnd* pParentWnd)
{
	ASSERT_VALID(pParentWnd);

	//֮����Ҫ��ȡ�����ڵ�DC����Ϊ���Ժ��ܴ����븸���ڼ��ݵ��ڴ�DC
	CDC* pDC = pParentWnd->GetDC();
	ASSERT_VALID(pDC);

	//�����ɫ���ӵ�λͼ
	CBitmap WhiteChessManBitmap;
	WhiteChessManBitmap.LoadBitmap(IDB_WHITECHESSMAN);

	//�����ɫ���ӵ�λͼ
	CBitmap BlackChessManBitmap;
	BlackChessManBitmap.LoadBitmap(IDB_BLACKCHESSMAN);

	//��ȡ����λͼ�Ŀ�Ⱥ͸߶ȣ�ͬʱ������Ӧ��ȫ�ֱ���
	BITMAP bm = { 0 };
	VERIFY(WhiteChessManBitmap.GetObject(sizeof(BITMAP), (LPVOID)&bm));
	g_nBitmapWidth = bm.bmWidth;
    g_nBitmapHeight = bm.bmHeight;

	//�ֱ𴴽�ȫ�ְ�ɫ�ͺ�ɫ�����ڴ�DC
	//(���ǵĿ�Ⱥ͸߶ȷֱ��������λͼ�Ŀ�Ⱥ͸߶ȼ��ϸ��ԵķŴ���ֵ)
	g_pWhiteChessManBufferDC = new CDC;
	ASSERT_VALID(g_pWhiteChessManBufferDC);

	g_pBlackChessManBufferDC = new CDC;
	ASSERT_VALID(g_pBlackChessManBufferDC);

	g_pWhiteChessManBufferDC->CreateCompatibleDC(pDC);
	g_pBlackChessManBufferDC->CreateCompatibleDC(pDC);

	CBitmap l_WhiteBufferBitmap;
	l_WhiteBufferBitmap.CreateCompatibleBitmap(pDC, g_nBitmapWidth + INITIAL_ADDWIDTH, 
		                                       g_nBitmapHeight + INITIAL_ADDHEIGHT);
	g_pWhiteChessManBufferDC->SelectObject(&l_WhiteBufferBitmap);

	CBitmap l_BlackBufferBitmap;
	l_BlackBufferBitmap.CreateCompatibleBitmap(pDC, g_nBitmapWidth + INITIAL_ADDWIDTH, 
		                                       g_nBitmapHeight + INITIAL_ADDHEIGHT);
	g_pBlackChessManBufferDC->SelectObject(&l_BlackBufferBitmap);

	//��ʱ�ڴ�DC������ֱ�����ס�������λͼ��Դ���ٷŴ��������Ӧ��ȫ�������ڴ�DC��
	CDC l_MemoryDC;
	l_MemoryDC.CreateCompatibleDC(pDC);

	CBitmap* l_pOldMemoryBitmap = l_MemoryDC.SelectObject(&WhiteChessManBitmap);
	g_pWhiteChessManBufferDC->StretchBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
		                                 g_nBitmapHeight + INITIAL_ADDHEIGHT, &l_MemoryDC, 0, 0, g_nBitmapWidth, 
		                                 g_nBitmapHeight, SRCCOPY);

	l_MemoryDC.SelectObject(&BlackChessManBitmap);
	g_pBlackChessManBufferDC->StretchBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
		                                 g_nBitmapHeight + INITIAL_ADDHEIGHT, &l_MemoryDC, 0, 0, g_nBitmapWidth, 
		                                 g_nBitmapHeight, SRCCOPY);

	l_MemoryDC.SelectObject(l_pOldMemoryBitmap);

	//�������ϵͳ��Win9x��WinNT������Ҫ�ֱ𴴽�ȫ�ְ�ɫ�ͺ�ɫ��������λͼ���ڴ�DC
	if (WIN9X_WINNT == g_nOSType)
	{
		g_pWhiteChessManMaskDC = new CDC;
		ASSERT_VALID(g_pWhiteChessManMaskDC);

		g_pBlackChessManMaskDC = new CDC;
		ASSERT_VALID(g_pBlackChessManMaskDC);

		g_pWhiteChessManMaskDC->CreateCompatibleDC(pDC);
		g_pBlackChessManMaskDC->CreateCompatibleDC(pDC);

		CBitmap l_WhiteMaskBitmap;
		l_WhiteMaskBitmap.CreateBitmap(g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                           g_nBitmapHeight + INITIAL_ADDHEIGHT, 1, 1, NULL);
		g_pWhiteChessManMaskDC->SelectObject(&l_WhiteMaskBitmap);

		CBitmap l_BlackMaskBitmap;
		l_BlackMaskBitmap.CreateBitmap(g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                           g_nBitmapHeight + INITIAL_ADDHEIGHT, 1, 1, NULL);
		g_pBlackChessManMaskDC->SelectObject(&l_BlackMaskBitmap);

		//������ɫ��������λͼ
		COLORREF l_crOldBack = g_pWhiteChessManBufferDC->SetBkColor(TRANSPARENT_COLOR);
		g_pWhiteChessManMaskDC->BitBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                           g_nBitmapHeight + INITIAL_ADDHEIGHT, g_pWhiteChessManBufferDC, 0, 0, SRCCOPY);
		g_pWhiteChessManBufferDC->SetBkColor(l_crOldBack);
		
		//ͬʱ����Ҫ�����ɺ�İ�ɫ��������λͼ���ɫ����Դλͼ��һ��SRCMASK����
		g_pWhiteChessManBufferDC->BitBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                             g_nBitmapHeight + INITIAL_ADDHEIGHT, g_pWhiteChessManMaskDC, 0, 0, SRCMASK);

		//������ɫ��������λͼ
		l_crOldBack = g_pBlackChessManBufferDC->SetBkColor(TRANSPARENT_COLOR);
		g_pBlackChessManMaskDC->BitBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                           g_nBitmapHeight + INITIAL_ADDHEIGHT, g_pBlackChessManBufferDC, 0, 0, SRCCOPY);
		g_pBlackChessManBufferDC->SetBkColor(l_crOldBack);
		
		//ͬʱ����Ҫ�����ɺ�ĺ�ɫ��������λͼ���ɫ����Դλͼ��һ��SRCMASK����
		g_pBlackChessManBufferDC->BitBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                             g_nBitmapHeight + INITIAL_ADDHEIGHT, g_pBlackChessManMaskDC, 0, 0, SRCMASK);
	}

	pParentWnd->ReleaseDC(pDC);
}

/******************************************************************************************
void CIntChessApp::UninstallChessManDC()
����      : tangjs520       ��������: 2003-10-20
������    : CIntChessApp::UninstallChessManDC
����ֵ    : void
�����б�  : ��
����      : ж�������ڴ�DC
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CIntChessApp::UninstallChessManDC()
{
    delete g_pWhiteChessManBufferDC;
    g_pWhiteChessManBufferDC = NULL;

    delete g_pBlackChessManBufferDC;
    g_pBlackChessManBufferDC = NULL;

    delete g_pWhiteChessManMaskDC;
    g_pWhiteChessManMaskDC = NULL;

    delete g_pBlackChessManMaskDC;
    g_pBlackChessManMaskDC = NULL;
}

/******************************************************************************************
BOOL CIntChessApp::PlayWaveResource(const CString& strResourceName)
����      : tangjs520       ��������: 2003-10-21
������    : CIntChessApp::PlayWaveResource
����ֵ    : BOOL
�����б�  : 
  ����1: const CString& strResourceName
����      : ��������Դ�е������ļ�(*.wav)
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CIntChessApp::PlayWaveResource(const CString& strResourceName)
{
	BOOL bRet = FALSE;

	HINSTANCE hModule = NULL;
	hModule = AfxGetResourceHandle();
	if (hModule)
	{
		HRSRC hRes = NULL;
		hRes = ::FindResource(hModule, strResourceName, _T("WAVE"));
		if (hRes)
		{
			HGLOBAL hMem = NULL;
			hMem = ::LoadResource(hModule, hRes);
			if (hMem)
			{
				LPSTR lpWave = NULL;
				lpWave = (LPSTR)::LockResource(hMem);
				if (lpWave)
				{
					bRet = ::sndPlaySound(lpWave, SND_MEMORY | SND_ASYNC);
				}

				//32λ����ϵͳ�Ѿ�����Ҫ��ʽ���øú������ͷ��ڴ����
				//32λ����ϵͳ���Զ��ͷŲ���ʹ�õ���Դ�ڴ�
				FreeResource(hMem);
			}
		}
	}

	return bRet;
}

/******************************************************************************************
BOOL CIntChessApp::SetBkgndBmp(UINT nID, CBitmap*& pBkgndBmp)
����      : tangjs520       ��������: 2003-11-22
������    : CIntChessApp::SetBkgndBmp
����ֵ    : BOOL
�����б�  : 
  ����1: UINT nID
  ����2: CBitmap*& pBkgndBmp
����      : ������ͼ���ڱ���λͼ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CIntChessApp::SetBkgndBmp(UINT nID, CBitmap*& pBkgndBmp)
{
	BOOL bResult = TRUE;

	if (pBkgndBmp)
	{
		pBkgndBmp->DeleteObject();
		delete pBkgndBmp;
		pBkgndBmp = NULL;
	}

	pBkgndBmp = new CBitmap();
	bResult = pBkgndBmp->LoadBitmap(nID);

	return bResult;
}

/******************************************************************************************
void CIntChessApp::RegisterShellFileType(CDocTemplate* pDocTemplate)
����      : tangjs520       ��������: 2003-11-25
������    : CIntChessApp::RegisterShellFileType
����ֵ    : void
�����б�  : 
  ����1: CDocTemplate* pDocTemplate
����      : ��ϵͳע���ļ�����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CIntChessApp::RegisterShellFileType(CDocTemplate* pDocTemplate)
{
	ASSERT_VALID(pDocTemplate);

	//��ȡӦ�ó��������·����
	CString strPathName = _T("");
	TCHAR szLongPathName[_MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(), szLongPathName, _MAX_PATH);
	if (::GetShortPathName(szLongPathName, strPathName.GetBuffer(_MAX_PATH), _MAX_PATH) == 0)
	{
		strPathName = szLongPathName;
	}
	strPathName.ReleaseBuffer();

	CString strDefaultIconCommandLine = strPathName + _T(",0");
	CString strOpenCommandLine = strPathName + _T(" \"%1\"");

	CString strFilterExt = _T(""), strFileTypeId = _T(""), strFileTypeName = _T("");
	if (pDocTemplate->GetDocString(strFileTypeId, CDocTemplate::regFileTypeId) 
		&& !strFileTypeId.IsEmpty())
	{
		// enough info to register it
		if (!pDocTemplate->GetDocString(strFileTypeName, CDocTemplate::regFileTypeName))
		{
			strFileTypeName = strFileTypeId;    // use id name
		}

		ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

		// first register the type ID of our server
		if (!SetRegKey(strFileTypeId, strFileTypeName))
		{
			return;
		}

		CString strTemp = _T("");

		// path\DefaultIcon = path,0
		strTemp.Format(_T("%s\\DefaultIcon"), (LPCTSTR)strFileTypeId);
		if (!SetRegKey(strTemp, strDefaultIconCommandLine))
		{
			return;
		}

		// path\Shell\Open\Command = path filename
		strTemp.Format(_T("%s\\Shell\\Open\\%s"), (LPCTSTR)strFileTypeId, 
			(LPCTSTR)_T("Command"));
		if (!SetRegKey(strTemp, strOpenCommandLine))
		{
			return;
		}

		pDocTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
		if (!strFilterExt.IsEmpty())
		{
			ASSERT(strFilterExt[0] == '.');

			LONG lSize = _MAX_PATH * 2;
			LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt, 
				strTemp.GetBuffer(lSize), &lSize);
			strTemp.ReleaseBuffer();

			if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() || 
				strTemp == strFileTypeId)
			{
				// no association for that suffix
				if (!SetRegKey(strFilterExt, strFileTypeId))
				{
					return;
				}
			}
		}
	}
}

/******************************************************************************************
BOOL CIntChessApp::SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName)
����      : tangjs520       ��������: 2003-11-25
������    : CIntChessApp::SetRegKey
����ֵ    : BOOL
�����б�  : 
  ����1: LPCTSTR lpszKey
  ����2: LPCTSTR lpszValue
  ����3: LPCTSTR lpszValueName
����      : ����ϵͳע���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CIntChessApp::SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, 
							 LPCTSTR lpszValueName/* = NULL*/)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ, 
			lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
			return FALSE;
		}

		return TRUE;
	}
	else
	{
        HKEY hKey = { 0 };

		if (::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ, (const BYTE*)lpszValue, 
                                           (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if (::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
            {
                return TRUE;
            }
		}

		TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
		return FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
void CIntChessApp::OnAppAbout()
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessApp::OnAppAbout
����ֵ    : // App command to run the dialog void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CIntChessApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/******************************************************************************************
void CIntChessApp::OnFileNew()
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessApp::OnFileNew
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CIntChessApp::OnFileNew()
{
	// TODO: Add your command handler code here
	if (AUTOMODE == CChessRule::GetUserMode())
	{
		return;
	}

	CWinApp::OnFileNew();
}

/******************************************************************************************
void CIntChessApp::OnFileOpen()
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessApp::OnFileOpen
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CIntChessApp::OnFileOpen()
{
	// TODO: Add your command handler code here
	if (AUTOMODE == CChessRule::GetUserMode())
	{
		return;
	}

	CWinApp::OnFileOpen();
}
