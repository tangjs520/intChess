/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : intChess.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-20
文件描述    : 
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#include "stdafx.h"
#include "intChess.h"

#include "MainFrm.h"
#include "intChessDoc.h"
#include "intChessView.h"

#pragma warning(disable : 4201)
#include <Mmsystem.h>			//声明API函数sndPlaySound的头文件
#pragma warning(default : 4201)

#include "ChessRule.h"			//走棋规则类(CChessRule)的头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//初始化全局变量
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessApp::CIntChessApp
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CIntChessApp::CIntChessApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CIntChessApp theApp;

/******************************************************************************************
BOOL CIntChessApp::InitInstance()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessApp::InitInstance
返回值    : BOOL
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CIntChessApp::InitInstance()
{
	//首先检测操作系统的类型(之所以要检测操作系统的类型，
	//是因为棋子的绘制操作[CChessMan::Draw]会因操作系统的不同，而采取不同的绘制方法。
	//为何要采用不同的绘制方法，请参见CChessMan::Draw函数的说明)
	if (!CheckOSVersion(g_nOSType))
	{
		AfxMessageBox(IDS_GETVERSION_ERR);
		return FALSE;
	}

	//然后判断显示器的分辨率是否符合应用程序的最低要求
	if (!CheckDisplayResolution())
	{
		AfxMessageBox(IDS_GETRESOLUTION_ERR);
		return FALSE;
	}

	//如果用户不输入棋手的姓名，则默认使用这两个名字
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

	//如果用户通过直接双击棋局文件来运行应用程序，则需要将其所要打开的文件路径名称
	//暂时保存下来，等到应用程序的窗口全部显示出来以后再进行打开文件的操作
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
作者      : tangjs520       创建日期: 2003-10-20
函数名    : CIntChessApp::CheckOSVersion
返回值    : BOOL
参数列表  : 
  参数1: BYTE& nOSType
描述      : 检测操作系统的类型
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CIntChessApp::CheckOSVersion(BYTE& nOSType)
{
	//设定OSVERSIONINFO的大小
	OSVERSIONINFO versionInfo = { sizeof(OSVERSIONINFO) };

	//获取版本信息
	if (!::GetVersionEx(&versionInfo))
	{
		return FALSE;	//获取版本信息失败
	}

	//如果是NT系列，检查是WIN2K_WINXP还是WINNT
	if (VER_PLATFORM_WIN32_NT == versionInfo.dwPlatformId)
	{
		//检查版本号
		if (versionInfo.dwMajorVersion >= 5)
		{
			nOSType = WIN2K_WINXP;	//Windows2000或WindowsXP
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
			nOSType = WIN9X_WINNT;	//Windows98或WindowsMe
		}
		else
		{
			return FALSE;	//Windows95，应用程序可能不支持该操作系统
		}
		*/
	}
	else
	{
		return FALSE;	//Windows 3.1，应用程序不支持该操作系统
	}

	return TRUE;
}

/******************************************************************************************
BOOL CIntChessApp::CheckDisplayResolution()
作者      : tangjs520       创建日期: 2003-10-20
函数名    : CIntChessApp::CheckDisplayResolution
返回值    : BOOL
参数列表  : 无
描述      : 检测显示器的分辨率
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CIntChessApp::CheckDisplayResolution()
{
	int nWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nHeight = ::GetSystemMetrics(SM_CYSCREEN);

	if (nWidth < 1024 || nHeight < 768)	//应用程序需要显示器的分辨率至少为1024×768
	{
		return FALSE;
	}

	return TRUE;
}

/******************************************************************************************
void CIntChessApp::InstallChessManDC(CWnd* pParentWnd)
作者      : tangjs520       创建日期: 2003-10-20
函数名    : CIntChessApp::InstallChessManDC
返回值    : void
参数列表  : 
  参数1: CWnd* pParentWnd
描述      : 加载棋子位图到相应内存DC中
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessApp::InstallChessManDC(CWnd* pParentWnd)
{
	ASSERT_VALID(pParentWnd);

	//之所以要获取父窗口的DC，是为了以后能创建与父窗口兼容的内存DC
	CDC* pDC = pParentWnd->GetDC();
	ASSERT_VALID(pDC);

	//载入白色棋子的位图
	CBitmap WhiteChessManBitmap;
	WhiteChessManBitmap.LoadBitmap(IDB_WHITECHESSMAN);

	//载入黑色棋子的位图
	CBitmap BlackChessManBitmap;
	BlackChessManBitmap.LoadBitmap(IDB_BLACKCHESSMAN);

	//获取棋子位图的宽度和高度，同时更新相应的全局变量
	BITMAP bm = { 0 };
	VERIFY(WhiteChessManBitmap.GetObject(sizeof(BITMAP), (LPVOID)&bm));
	g_nBitmapWidth = bm.bmWidth;
    g_nBitmapHeight = bm.bmHeight;

	//分别创建全局白色和黑色棋子内存DC
	//(它们的宽度和高度分别等于棋子位图的宽度和高度加上各自的放大数值)
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

	//临时内存DC，负责分别载入白、黑棋子位图资源后，再放大的贴入相应的全局棋子内存DC中
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

	//如果操作系统是Win9x或WinNT，则还需要分别创建全局白色和黑色棋子掩码位图的内存DC
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

		//产生白色棋子掩码位图
		COLORREF l_crOldBack = g_pWhiteChessManBufferDC->SetBkColor(TRANSPARENT_COLOR);
		g_pWhiteChessManMaskDC->BitBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                           g_nBitmapHeight + INITIAL_ADDHEIGHT, g_pWhiteChessManBufferDC, 0, 0, SRCCOPY);
		g_pWhiteChessManBufferDC->SetBkColor(l_crOldBack);
		
		//同时还需要将生成后的白色棋子掩码位图与白色棋子源位图做一次SRCMASK操作
		g_pWhiteChessManBufferDC->BitBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                             g_nBitmapHeight + INITIAL_ADDHEIGHT, g_pWhiteChessManMaskDC, 0, 0, SRCMASK);

		//产生黑色棋子掩码位图
		l_crOldBack = g_pBlackChessManBufferDC->SetBkColor(TRANSPARENT_COLOR);
		g_pBlackChessManMaskDC->BitBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                           g_nBitmapHeight + INITIAL_ADDHEIGHT, g_pBlackChessManBufferDC, 0, 0, SRCCOPY);
		g_pBlackChessManBufferDC->SetBkColor(l_crOldBack);
		
		//同时还需要将生成后的黑色棋子掩码位图与黑色棋子源位图做一次SRCMASK操作
		g_pBlackChessManBufferDC->BitBlt(0, 0, g_nBitmapWidth + INITIAL_ADDWIDTH, 
			                             g_nBitmapHeight + INITIAL_ADDHEIGHT, g_pBlackChessManMaskDC, 0, 0, SRCMASK);
	}

	pParentWnd->ReleaseDC(pDC);
}

/******************************************************************************************
void CIntChessApp::UninstallChessManDC()
作者      : tangjs520       创建日期: 2003-10-20
函数名    : CIntChessApp::UninstallChessManDC
返回值    : void
参数列表  : 无
描述      : 卸载棋子内存DC
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CIntChessApp::PlayWaveResource
返回值    : BOOL
参数列表  : 
  参数1: const CString& strResourceName
描述      : 播放在资源中的声音文件(*.wav)
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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

				//32位操作系统已经不需要显式调用该函数来释放内存块了
				//32位操作系统会自动释放不再使用的资源内存
				FreeResource(hMem);
			}
		}
	}

	return bRet;
}

/******************************************************************************************
BOOL CIntChessApp::SetBkgndBmp(UINT nID, CBitmap*& pBkgndBmp)
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CIntChessApp::SetBkgndBmp
返回值    : BOOL
参数列表  : 
  参数1: UINT nID
  参数2: CBitmap*& pBkgndBmp
描述      : 载入视图窗口背景位图
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2003-11-25
函数名    : CIntChessApp::RegisterShellFileType
返回值    : void
参数列表  : 
  参数1: CDocTemplate* pDocTemplate
描述      : 向系统注册文件类型
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessApp::RegisterShellFileType(CDocTemplate* pDocTemplate)
{
	ASSERT_VALID(pDocTemplate);

	//获取应用程序的完整路径名
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
作者      : tangjs520       创建日期: 2003-11-25
函数名    : CIntChessApp::SetRegKey
返回值    : BOOL
参数列表  : 
  参数1: LPCTSTR lpszKey
  参数2: LPCTSTR lpszValue
  参数3: LPCTSTR lpszValueName
描述      : 操作系统注册表
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessApp::OnAppAbout
返回值    : // App command to run the dialog void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/******************************************************************************************
void CIntChessApp::OnFileNew()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessApp::OnFileNew
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessApp::OnFileOpen
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
