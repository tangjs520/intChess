/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessDeskWnd.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2004-9-29
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
#include "ChessDeskWnd.h"

#include "ChessRule.h"  //走棋规则类(CChessRule)的头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/******************************************************************************************
CChessDeskWnd::CChessDeskWnd()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CChessDeskWnd::CChessDeskWnd
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessDeskWnd::CChessDeskWnd()
{
    m_pBkgndBmp = NULL;
    m_pWhiteClock = NULL;
    m_pBlackClock = NULL;
    m_pfnTimeLimitProc = NULL;
}

/******************************************************************************************
CChessDeskWnd::~CChessDeskWnd()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CChessDeskWnd::~CChessDeskWnd
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessDeskWnd::~CChessDeskWnd()
{
    if (m_pBkgndBmp)
    {
        m_pBkgndBmp->DeleteObject();
        delete m_pBkgndBmp;
    }

    delete m_pWhiteClock;
    delete m_pBlackClock;
}

BEGIN_MESSAGE_MAP(CChessDeskWnd, CWnd)
    //{{AFX_MSG_MAP(CChessDeskWnd)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
int CChessDeskWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CChessDeskWnd::OnCreate
返回值    : int
参数列表  : 
  参数1: LPCREATESTRUCT lpCreateStruct
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
int CChessDeskWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    ModifyStyle(0, WS_CLIPCHILDREN);

    // TODO: Add your specialized creation code here
    BOOL bResult = FALSE;
    bResult = CIntChessApp::SetBkgndBmp(IDB_CHESSDESK, m_pBkgndBmp);
    if (!bResult)
    {
        TRACE0("Failed to create bitmap background\n");
        return -1;
    }

    m_pWhiteClock = new CDigitalClockWnd;
    ASSERT_VALID(m_pWhiteClock);
    m_pWhiteClock->CreateEx(WS_EX_DLGMODALFRAME, NULL, NULL, WS_CHILD | WS_VISIBLE, 
                            CRect(CPoint(12, 250), CSize(91, 23)), this, 0);

    m_pBlackClock = new CDigitalClockWnd;
    ASSERT_VALID(m_pBlackClock);
    m_pBlackClock->CreateEx(WS_EX_DLGMODALFRAME, NULL, NULL, WS_CHILD | WS_VISIBLE, 
                            CRect(CPoint(12, 70), CSize(91, 23)), this, 0);

    //在使用走棋规则类之前，必须初始化该静态变量
    CChessRule::m_pChessDeskWnd = this;

    m_pfnTimeLimitProc = CChessRule::ProcTimeOut;

    return 0;
}

/******************************************************************************************
BOOL CChessDeskWnd::OnEraseBkgnd(CDC* pDC)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CChessDeskWnd::OnEraseBkgnd
返回值    : BOOL
参数列表  : 
  参数1: CDC* pDC
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessDeskWnd::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pBkgndBmp)
    {
        CDC dcMem;
        dcMem.CreateCompatibleDC(pDC);
        CBitmap* pOldBitmap = dcMem.SelectObject(m_pBkgndBmp);

        CRect rect;
        GetClientRect(&rect);

        pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);

        dcMem.SelectObject(pOldBitmap);

        return TRUE;
    }
    else
    {
        return CWnd::OnEraseBkgnd(pDC);
    }
}
