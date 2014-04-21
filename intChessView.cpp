/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : intChessView.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-21
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

#include "intChessDoc.h"
#include "intChessView.h"

#include "ChessEquipmentWnd.h"  //棋具窗口对象类(CChessEquipmentWnd)的头文件
#include "ChessDeskWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CIntChessView, CView)

/******************************************************************************************
CIntChessView::CIntChessView()
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CIntChessView::CIntChessView
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CIntChessView::CIntChessView()
{
    // TODO: add construction code here
    m_pBkgndBmp = NULL;
    m_pChessEquipmentWnd = NULL;
    m_pChessDeskWnd = NULL;
}

/******************************************************************************************
CIntChessView::~CIntChessView()
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CIntChessView::~CIntChessView
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CIntChessView::~CIntChessView()
{
    if (m_pBkgndBmp)
    {
        m_pBkgndBmp->DeleteObject();
        delete m_pBkgndBmp;
    }

    delete m_pChessEquipmentWnd;
    delete m_pChessDeskWnd;
}

BEGIN_MESSAGE_MAP(CIntChessView, CView)
    //{{AFX_MSG_MAP(CIntChessView)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_WINDOWPOSCHANGED()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
BOOL CIntChessView::PreCreateWindow(CREATESTRUCT& cs)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessView::PreCreateWindow
返回值    : BOOL
参数列表  : 
  参数1: CREATESTRUCT& cs
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CIntChessView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

/******************************************************************************************
void CIntChessView::OnDraw(CDC* pDC)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessView::OnDraw
返回值    : void
参数列表  : 
  参数1: CDC* pDC
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessView::OnDraw(CDC* pDC)
{
    UNREFERENCED_PARAMETER(pDC);
    /*
    CIntChessDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    */
    // TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CIntChessView diagnostics

#ifdef _DEBUG
void CIntChessView::AssertValid() const
{
    CView::AssertValid();
}

void CIntChessView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CIntChessDoc* CIntChessView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIntChessDoc)));
    return (CIntChessDoc*)m_pDocument;
}
#endif //_DEBUG

/******************************************************************************************
BOOL CIntChessView::OnEraseBkgnd(CDC* pDC)
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CIntChessView::OnEraseBkgnd
返回值    : BOOL
参数列表  : 
  参数1: CDC* pDC
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CIntChessView::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pBkgndBmp)
    {
        BITMAP bm = { 0 };
        CDC dcMem;

        VERIFY(m_pBkgndBmp->GetObject(sizeof(BITMAP), (LPVOID)&bm));
        dcMem.CreateCompatibleDC(pDC);
        CBitmap* pOldBitmap = dcMem.SelectObject(m_pBkgndBmp);

        CRect rect;
        GetClientRect(&rect);

        for (register int nX = 0; nX < rect.Width(); nX += bm.bmWidth)
        {
            for (register int nY = 0; nY < rect.Height(); nY += bm.bmHeight)
            {
                pDC->BitBlt(nX, nY, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);
            }
        }

        dcMem.SelectObject(pOldBitmap);

        //绘制棋盘阴影
        if (m_pChessEquipmentWnd)
        {
            m_pChessEquipmentWnd->GetWindowRect(rect);
            ScreenToClient(rect);

            pDC->SelectStockObject(BLACK_BRUSH);

            CPoint pts[4];
            pts[0].x = rect.left;
            pts[0].y = rect.bottom;

            pts[1].x = rect.left + SHADOW;
            pts[1].y = rect.bottom + SHADOW;

            pts[2].x = rect.right + SHADOW;
            pts[2].y = rect.bottom + SHADOW;

            pts[3].x = rect.right;
            pts[3].y = rect.bottom;
            pDC->Polygon(pts, 4);

            pts[0].x = rect.right;
            pts[0].y = rect.top;

            pts[1].x = rect.right + SHADOW;
            pts[1].y = rect.top + SHADOW;

            pts[2].x = rect.right + SHADOW;
            pts[2].y = rect.bottom + SHADOW;

            pts[3].x = rect.right;
            pts[3].y = rect.bottom;
            pDC->Polygon(pts, 4);
        }

        return TRUE;
    }
    else
    {
        return CView::OnEraseBkgnd(pDC);
    }
}

/******************************************************************************************
int CIntChessView::OnCreate(LPCREATESTRUCT lpCreateStruct)
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CIntChessView::OnCreate
返回值    : int
参数列表  : 
  参数1: LPCREATESTRUCT lpCreateStruct
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
int CIntChessView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    //给视图窗口增加这个窗口风格，是为了在刷新视图窗口背景时，不覆盖它的子窗口，
    //也就是说，子窗口覆盖的背景部分将不予刷新，这样就可以减少闪烁
    ModifyStyle(0, WS_CLIPCHILDREN);

    // TODO: Add your specialized creation code here
    BOOL bResult = FALSE;
    bResult = CIntChessApp::SetBkgndBmp(IDB_STONE, m_pBkgndBmp);
    if (!bResult)
    {
        TRACE0("Failed to create bitmap background\n");
        return -1;
    }

    //创建棋具窗口
    CString strWndClass = AfxRegisterWndClass(CS_DBLCLKS | CS_SAVEBITS, ::LoadCursor(NULL, IDC_ARROW), 
                                              (HBRUSH)::GetStockObject(NULL_BRUSH));
    m_pChessEquipmentWnd = new CChessEquipmentWnd;
    ASSERT_VALID(m_pChessEquipmentWnd);
    m_pChessEquipmentWnd->Create(strWndClass, NULL, WS_VISIBLE | WS_CHILD, 
                                 CRect(CPoint(136, 14), CSize(8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN, 
                                 8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN)), this, 0);

    //创建棋桌窗口
    m_pChessDeskWnd = new CChessDeskWnd;
    ASSERT_VALID(m_pChessDeskWnd);
    m_pChessDeskWnd->Create(strWndClass, NULL, WS_VISIBLE | WS_CHILD, 
                            CRect(CPoint(10, 174), CSize(116, 340)), this, 0);

    return 0;
}

/******************************************************************************************
void CIntChessView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CIntChessView::OnWindowPosChanged
返回值    : void
参数列表  : 
  参数1: WINDOWPOS FAR* lpwndpos
描述      : 让棋具窗口始终垂直居中视图窗口的客户区
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
    CView::OnWindowPosChanged(lpwndpos);

    // TODO: Add your message handler code here
    int cy = lpwndpos->cy;
    if (cy > 8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN + SHADOW)
    {
        int nNewYPos = (cy - (8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN + SHADOW)) / 2;

        if (m_pChessEquipmentWnd)
        {
            CRect rect;
            m_pChessEquipmentWnd->GetWindowRect(&rect);
            ScreenToClient(&rect);

            if (rect.top != nNewYPos)
            {
                m_pChessEquipmentWnd->SetWindowPos(NULL, rect.left, nNewYPos, 
                                                   0, 0, SWP_NOZORDER | SWP_NOSIZE);

                if (m_pChessDeskWnd)
                {
                    CRect rectDeskWnd;
                    m_pChessDeskWnd->GetWindowRect(&rectDeskWnd);
                    ScreenToClient(&rectDeskWnd);

                    m_pChessDeskWnd->SetWindowPos(NULL, rectDeskWnd.left, 
                                                  nNewYPos + ((rect.Height() + SHADOW - rectDeskWnd.Height()) / 2), 
                                                  0, 0, SWP_NOZORDER | SWP_NOSIZE);
                }
            }
        }
    }
}
