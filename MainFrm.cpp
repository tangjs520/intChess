/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : MainFrm.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-31
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
#include "ChessManualView.h"
#include "OtherView.h"
#include "MainFrm.h"

#include "ChessRule.h"  //走棋规则类(CChessRule)的头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_WM_CLOSE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//由于应用程序不需要显示状态栏，所以也就没有必要存在状态栏指示符
/*
static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};
*/

/******************************************************************************************
CMainFrame::CMainFrame()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CMainFrame::CMainFrame
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CMainFrame::CMainFrame()
{
    // TODO: add member initialization code here
}

/******************************************************************************************
CMainFrame::~CMainFrame()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CMainFrame::~CMainFrame
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CMainFrame::~CMainFrame()
{
}

/******************************************************************************************
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
作者      : tangjs520       创建日期: 2003-10-30
函数名    : CMainFrame::OnCreate
返回值    : int
参数列表  : 
  参数1: LPCREATESTRUCT lpCreateStruct
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    //应用程序不需要显示工具栏和状态栏
    /*
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) 
        || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }
    */

    /*
    if (!m_wndStatusBar.Create(this) 
        || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }
    */

    // TODO: Delete these three lines if you don't want the toolbar to
    //  be dockable
    //m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    //EnableDocking(CBRS_ALIGN_ANY);
    //DockControlBar(&m_wndToolBar);

    return 0;
}

/******************************************************************************************
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT, CCreateContext* pContext)
作者      : tangjs520       创建日期: 2003-10-30
函数名    : CMainFrame::OnCreateClient
返回值    : BOOL
参数列表  : 
  参数1: LPCREATESTRUCT
  参数2: CCreateContext* pContext
描述      : 创建分割窗口和视图
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    if (!m_wndSplitter1.CreateStatic(this, 1, 2))
    {
        TRACE0("Failed to create first static splitter\n");
        return FALSE;
    }
    if (!m_wndSplitter1.CreateView(0, 0, pContext->m_pNewViewClass, CSize(806, 0), pContext))
    {
        TRACE0("Failed to create first pane\n");
        return FALSE;
    }
    if (!m_wndSplitter2.CreateStatic(&m_wndSplitter1, 2, 1, WS_CHILD | WS_VISIBLE, 
        m_wndSplitter1.IdFromRowCol(0, 1)))
    {
        TRACE0("Failed to create second static splitter\n");
        return FALSE;
    }
    if (!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CChessManualView), CSize(0, 200), pContext))
    {
        TRACE0("Failed to create second pane\n");
        return FALSE;
    }
    if (!m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(COtherView), CSize(0, 0), pContext))
    {
        TRACE0("Failed to create third pane\n");
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************************
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
作者      : tangjs520       创建日期: 2003-10-30
函数名    : CMainFrame::PreCreateWindow
返回值    : BOOL
参数列表  : 
  参数1: CREATESTRUCT& cs
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if(!CFrameWnd::PreCreateWindow(cs))
    {
        return FALSE;
    }

    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    cs.style &= ~FWS_ADDTOTITLE;    //去掉标题栏上的文档名称

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/******************************************************************************************
void CMainFrame::OnClose()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CMainFrame::OnClose
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CMainFrame::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    if (AUTOMODE == CChessRule::GetUserMode())
    {
        return;
    }

    CFrameWnd::OnClose();
}
