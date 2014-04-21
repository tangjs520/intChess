/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : MainFrm.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-31
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
#include "ChessManualView.h"
#include "OtherView.h"
#include "MainFrm.h"

#include "ChessRule.h"  //���������(CChessRule)��ͷ�ļ�

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

//����Ӧ�ó�����Ҫ��ʾ״̬��������Ҳ��û�б�Ҫ����״̬��ָʾ��
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
����      : tangjs520       ��������: 2004-9-29
������    : CMainFrame::CMainFrame
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CMainFrame::CMainFrame()
{
    // TODO: add member initialization code here
}

/******************************************************************************************
CMainFrame::~CMainFrame()
����      : tangjs520       ��������: 2004-9-29
������    : CMainFrame::~CMainFrame
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CMainFrame::~CMainFrame()
{
}

/******************************************************************************************
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
����      : tangjs520       ��������: 2003-10-30
������    : CMainFrame::OnCreate
����ֵ    : int
�����б�  : 
  ����1: LPCREATESTRUCT lpCreateStruct
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    //Ӧ�ó�����Ҫ��ʾ��������״̬��
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
����      : tangjs520       ��������: 2003-10-30
������    : CMainFrame::OnCreateClient
����ֵ    : BOOL
�����б�  : 
  ����1: LPCREATESTRUCT
  ����2: CCreateContext* pContext
����      : �����ָ�ں���ͼ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-10-30
������    : CMainFrame::PreCreateWindow
����ֵ    : BOOL
�����б�  : 
  ����1: CREATESTRUCT& cs
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if(!CFrameWnd::PreCreateWindow(cs))
    {
        return FALSE;
    }

    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs
    cs.style &= ~FWS_ADDTOTITLE;    //ȥ���������ϵ��ĵ�����

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
����      : tangjs520       ��������: 2004-9-29
������    : CMainFrame::OnClose
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
