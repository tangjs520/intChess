/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : intChessView.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-21
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

#include "intChessDoc.h"
#include "intChessView.h"

#include "ChessEquipmentWnd.h"  //��ߴ��ڶ�����(CChessEquipmentWnd)��ͷ�ļ�
#include "ChessDeskWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CIntChessView, CView)

/******************************************************************************************
CIntChessView::CIntChessView()
����      : tangjs520       ��������: 2003-10-21
������    : CIntChessView::CIntChessView
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-10-21
������    : CIntChessView::~CIntChessView
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessView::PreCreateWindow
����ֵ    : BOOL
�����б�  : 
  ����1: CREATESTRUCT& cs
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CIntChessView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}

/******************************************************************************************
void CIntChessView::OnDraw(CDC* pDC)
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessView::OnDraw
����ֵ    : void
�����б�  : 
  ����1: CDC* pDC
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-10-21
������    : CIntChessView::OnEraseBkgnd
����ֵ    : BOOL
�����б�  : 
  ����1: CDC* pDC
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

        //����������Ӱ
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
����      : tangjs520       ��������: 2003-10-21
������    : CIntChessView::OnCreate
����ֵ    : int
�����б�  : 
  ����1: LPCREATESTRUCT lpCreateStruct
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
int CIntChessView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    //����ͼ��������������ڷ����Ϊ����ˢ����ͼ���ڱ���ʱ�������������Ӵ��ڣ�
    //Ҳ����˵���Ӵ��ڸ��ǵı������ֽ�����ˢ�£������Ϳ��Լ�����˸
    ModifyStyle(0, WS_CLIPCHILDREN);

    // TODO: Add your specialized creation code here
    BOOL bResult = FALSE;
    bResult = CIntChessApp::SetBkgndBmp(IDB_STONE, m_pBkgndBmp);
    if (!bResult)
    {
        TRACE0("Failed to create bitmap background\n");
        return -1;
    }

    //������ߴ���
    CString strWndClass = AfxRegisterWndClass(CS_DBLCLKS | CS_SAVEBITS, ::LoadCursor(NULL, IDC_ARROW), 
                                              (HBRUSH)::GetStockObject(NULL_BRUSH));
    m_pChessEquipmentWnd = new CChessEquipmentWnd;
    ASSERT_VALID(m_pChessEquipmentWnd);
    m_pChessEquipmentWnd->Create(strWndClass, NULL, WS_VISIBLE | WS_CHILD, 
                                 CRect(CPoint(136, 14), CSize(8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN, 
                                 8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN)), this, 0);

    //������������
    m_pChessDeskWnd = new CChessDeskWnd;
    ASSERT_VALID(m_pChessDeskWnd);
    m_pChessDeskWnd->Create(strWndClass, NULL, WS_VISIBLE | WS_CHILD, 
                            CRect(CPoint(10, 174), CSize(116, 340)), this, 0);

    return 0;
}

/******************************************************************************************
void CIntChessView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
����      : tangjs520       ��������: 2003-10-21
������    : CIntChessView::OnWindowPosChanged
����ֵ    : void
�����б�  : 
  ����1: WINDOWPOS FAR* lpwndpos
����      : ����ߴ���ʼ�մ�ֱ������ͼ���ڵĿͻ���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
