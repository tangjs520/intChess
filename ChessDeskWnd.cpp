/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessDeskWnd.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2004-9-29
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
#include "ChessDeskWnd.h"

#include "ChessRule.h"  //���������(CChessRule)��ͷ�ļ�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/******************************************************************************************
CChessDeskWnd::CChessDeskWnd()
����      : tangjs520       ��������: 2004-9-29
������    : CChessDeskWnd::CChessDeskWnd
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2004-9-29
������    : CChessDeskWnd::~CChessDeskWnd
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2004-9-29
������    : CChessDeskWnd::OnCreate
����ֵ    : int
�����б�  : 
  ����1: LPCREATESTRUCT lpCreateStruct
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

    //��ʹ�����������֮ǰ�������ʼ���þ�̬����
    CChessRule::m_pChessDeskWnd = this;

    m_pfnTimeLimitProc = CChessRule::ProcTimeOut;

    return 0;
}

/******************************************************************************************
BOOL CChessDeskWnd::OnEraseBkgnd(CDC* pDC)
����      : tangjs520       ��������: 2004-9-29
������    : CChessDeskWnd::OnEraseBkgnd
����ֵ    : BOOL
�����б�  : 
  ����1: CDC* pDC
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
