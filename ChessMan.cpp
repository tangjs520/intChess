/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessMan.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-6
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
#include "ChessMan.h"

#include "ChessEquipmentWnd.h"  //棋具窗口类(CChessEquipmentWnd)的头文件

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//初始化类的公共变量
BYTE CChessMan::g_nMoveCount = 0;
BYTE CChessMan::g_nMoveSpeed = INITIAL_MOVESPEED;

CChessMan* CChessMan::g_pActiveChessMan = NULL;
int CChessMan::g_nBeforeZoomXPos = 0;
int CChessMan::g_nBeforeZoomYPos = 0;

/******************************************************************************************
CChessMan::CChessMan(const CHESSMANCOLOR color, const CHESSMANTYPE type, 
                     CWnd* pParentWnd, CChessGrid* pChessGrid)
作者      : tangjs520       创建日期: 2003-10-6
函数名    : CChessMan::CChessMan
返回值    : 无
参数列表  : 
  参数1: const CHESSMANCOLOR color
  参数2: const CHESSMANTYPE type
  参数3: CWnd* pParentWnd
  参数4: CChessGrid* pChessGrid
描述      : 构造函数，主要负责初始化棋子的内存DC(m_pBufferDC)、
            棋子掩码位图的内存DC(m_pMaskDC)[当操作系统是Win9x或WinNT时]
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessMan::CChessMan(const CHESSMANCOLOR color, const CHESSMANTYPE type, 
                     CWnd* pParentWnd, CChessGrid* pChessGrid/* = NULL*/)
{
    ASSERT_VALID(pParentWnd);
    m_pParentWnd = pParentWnd;

    //之所以要获取父窗口的DC，是为了以后能创建与父窗口兼容的内存DC
    CDC* pDC = m_pParentWnd->GetDC();
    ASSERT_VALID(pDC);

    //初始化这个数据成员为空，是为了当操作系统不是Win9x或WinNT时，保证数据成员的安全性
    m_pMaskDC = NULL;

    m_bVisible = TRUE;  //棋子的初始状态是可见的

    m_bZooming = FALSE; //棋子的初始状态是不进行缩放操作的

    m_enuColor = color;
    m_enuType = type;

    m_pInsideChessGrid = pChessGrid;
    //如果使用棋格对象来构造棋子对象，则使用棋格对象的相关属性初始化棋子位置的横、纵坐标，
    //同时更新棋格对象的数据成员m_pCurrentChessMan
    if (m_pInsideChessGrid)
    {
        CRect rectGrid = m_pInsideChessGrid->GetRect();
        m_nXPosition = rectGrid.left;
        m_nYPosition = rectGrid.top;

        m_pInsideChessGrid->SetIncludeChessMan(this);
    }
    //否则初始化棋子位置的横、纵坐标均为0，以后可通过调用SetPosition函数来更新这两个属性
    else
    {
        m_nXPosition = 0;
        m_nYPosition = 0;
    }

    m_nAddWidth = INITIAL_ADDWIDTH;
    m_nAddHeight = INITIAL_ADDHEIGHT;

    //创建棋子的内存DC(它的宽度和高度分别等于棋子位图的宽度和高度加上各自的放大数值)
    m_pBufferDC = new CDC;
    ASSERT_VALID(m_pBufferDC);

    m_pBufferDC->CreateCompatibleDC(pDC);

    CBitmap l_BufferBitmap;
    l_BufferBitmap.CreateCompatibleBitmap(pDC, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                                          CIntChessApp::g_nBitmapHeight + m_nAddHeight);
    m_pBufferDC->SelectObject(&l_BufferBitmap);

    //将相应的全局棋子内存DC贴入棋子的内存DC中
    if (CHESSMAN_WHITE == m_enuColor)
    {
        m_pBufferDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                            CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                            CIntChessApp::g_pWhiteChessManBufferDC, 0, 0, SRCCOPY);
    }
    else
    {
        m_pBufferDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                            CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                            CIntChessApp::g_pBlackChessManBufferDC, 0, 0, SRCCOPY);
    }

    //如果操作系统是WIN9X_WINNT，则还需要创建棋子掩码位图的内存DC
    if (WIN9X_WINNT == CIntChessApp::g_nOSType)
    {
        m_pMaskDC = new CDC;
        ASSERT_VALID(m_pMaskDC);

        m_pMaskDC->CreateCompatibleDC(pDC);

        CBitmap l_MaskBitmap;
        l_MaskBitmap.CreateBitmap(CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                                  CIntChessApp::g_nBitmapHeight + m_nAddHeight, 1, 1, NULL);
        m_pMaskDC->SelectObject(&l_MaskBitmap);

        //将相应的全局棋子掩码位图的内存DC贴入棋子掩码位图的内存DC中
        if (CHESSMAN_WHITE == m_enuColor)
        {
            m_pMaskDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                              CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                              CIntChessApp::g_pWhiteChessManMaskDC, 0, 0, SRCCOPY);
        }
        else
        {
            m_pMaskDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                              CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                              CIntChessApp::g_pBlackChessManMaskDC, 0, 0, SRCCOPY);
        }
    }

    m_pParentWnd->ReleaseDC(pDC);
}

/******************************************************************************************
CChessMan::~CChessMan()
作者      : tangjs520       创建日期: 2003-10-6
函数名    : CChessMan::~CChessMan
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessMan::~CChessMan()
{
    //如果还没有关闭定时器，则关闭之
    if (m_bZooming)
    {
        ::KillTimer(m_pParentWnd->GetSafeHwnd(), ZOOM_EVENT);
    }

    delete m_pBufferDC;
    delete m_pMaskDC;
}

/******************************************************************************************
void CALLBACK CChessMan::MovingProc(int nX, int nY, LPARAM lpData)
作者      : tangjs520       创建日期: 2003-10-6
函数名    : CChessMan::MovingProc
返回值    : void CALLBACK
参数列表  : 
  参数1: int X
  参数2: int Y
  参数3: LPARAM lpData
描述      : 处理棋子的移动操作
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CALLBACK CChessMan::MovingProc(int nX, int nY, LPARAM lpData)
{
    //参数nX和nY是API函数LineDDA每次回调MovingProc函数时传入的表示从源位置到目标位置所经过的
    //某一点的横、纵坐标值，参数lpData是外部调用API函数LineDDA时传入的参数

    //当达到指定的步长后再实际移动棋子(也就是说，如果棋子移动的步长越长，则移动速度就越快)
    if (g_nMoveCount >= g_nMoveSpeed)
    {
        CChessMan* pChessMan = (CChessMan*)lpData;

        //以下调用Sleep函数进行延时，只是为了增强移动效果而已

        //刷新移动前的棋子
        ::InvalidateRect(pChessMan->m_pParentWnd->GetSafeHwnd(), pChessMan->GetRect(), TRUE);
        Sleep(1);

        //重画移动后的棋子
        pChessMan->SetPosition(nX, nY);
        ::RedrawWindow(pChessMan->m_pParentWnd->GetSafeHwnd(), pChessMan->GetRect(), NULL, 
                       RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASENOW);
        Sleep(1);

        //移动完棋子后，记得归零g_nMoveCount，以便下次移动棋子时重新计算步长
        g_nMoveCount = 0;
    }

    //增加步长，以便达到指定的步长数值(g_nMoveSpeed)
    g_nMoveCount++;
}

/******************************************************************************************
void CChessMan::Moving(const CPoint& ptNewPosition)
作者      : tangjs520       创建日期: 2003-10-6
函数名    : CChessMan::Moving
返回值    : void
参数列表  : 
  参数1: const CPoint& ptNewPosition
描述      : 移动棋子到指定坐标点
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessMan::Moving(const CPoint& ptNewPosition)
{
    //如果棋子正在进行缩放操作，则停止棋子的缩放操作
    if (m_bZooming)
    {
        StopZoom();
    }

    //移动棋子前，记得归零g_nMoveCount，以便移动棋子时能正确计算步长
    g_nMoveCount = 0;

    ::LineDDA(m_nXPosition, m_nYPosition, ptNewPosition.x, ptNewPosition.y, 
              (LINEDDAPROC)MovingProc, (LPARAM)this);

    //由于棋子的移动步长不等于1，所以在调用完API函数LineDDA后，棋子的实际位置不一定等于ptNewPosition
    //因此当出现这种情况时，还需再将棋子一步到位的移动到ptNewPosition
    CPoint pt = GetPosition();
    if (pt != ptNewPosition)
    {
        ::InvalidateRect(m_pParentWnd->GetSafeHwnd(), GetRect(), TRUE);

        SetPosition(ptNewPosition);

        ::RedrawWindow(m_pParentWnd->GetSafeHwnd(), GetRect(), NULL, 
                       RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASENOW);
    }
}

/******************************************************************************************
void CALLBACK CChessMan::ZoomingProc(HWND, UINT, UINT_PTR, DWORD)
作者      : tangjs520       创建日期: 2003-10-8
函数名    : CChessMan::ZoomingProc
返回值    : void CALLBACK
参数列表  : 
  参数1: HWND
  参数2: UINT
  参数3: UINT_PTR
  参数4: DWORD
描述      : 处理棋子的缩放操作(其实是父窗口负责响应WM_TIMER[定时器]的消息响应函数)
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CALLBACK CChessMan::ZoomingProc(HWND, UINT, UINT_PTR, DWORD)
{
    //只有被选中的棋子才能进行缩放操作
    ASSERT(g_pActiveChessMan);

    //之所以要获取父窗口的DC，是为了以后能创建与父窗口兼容的内存DC
    CDC* pDC = g_pActiveChessMan->m_pParentWnd->GetDC();
    ASSERT_VALID(pDC);

    //取得棋子缩放前的位置，以做后续刷新的范围(这样可以擦除缩放前的棋子，并显示缩放后的棋子)
    CRect rectOld = g_pActiveChessMan->GetRect();

    //获得当前棋子宽度和高度的缩放数值
    UINT l_nDegressionAddWidth = g_pActiveChessMan->m_nAddWidth;
    UINT l_nDegressionAddHeight = g_pActiveChessMan->m_nAddHeight;

    //更新棋子位置
    g_pActiveChessMan->m_nXPosition++;
    g_pActiveChessMan->m_nYPosition++;

    //递减当前棋子宽度和高度的缩放数值，如果递减后的宽度缩放数值
    //小于棋子宽度递减后的最小允许值，则还原棋子最初的位置和缩放数值
    l_nDegressionAddWidth -= WIDTH_ZOOM_DEGRESSION;
    l_nDegressionAddHeight -= HEIGHT_ZOOM_DEGRESSION;
    if (l_nDegressionAddWidth < WIDTH_ZOOM_MINIMIZATION)
    {
        l_nDegressionAddWidth = INITIAL_ADDWIDTH;
        l_nDegressionAddHeight = INITIAL_ADDHEIGHT;

        g_pActiveChessMan->m_nXPosition = g_nBeforeZoomXPos;
        g_pActiveChessMan->m_nYPosition = g_nBeforeZoomYPos;

        //不能再通过g_pActiveChessMan->GetRect()获得刷新范围，因为此时m_nAddWidth和m_nAddHeight
        //还未还原，所以必须显式的指定缩放前最初的棋子范围
        rectOld = CRect(CPoint(g_nBeforeZoomXPos, g_nBeforeZoomYPos), 
                        CSize((CIntChessApp::g_nBitmapWidth + INITIAL_ADDWIDTH) / CHESSMANTYPE_AMOUNT, 
                        CIntChessApp::g_nBitmapHeight + INITIAL_ADDHEIGHT));
    }

    //创建临时内存DC，负责从全局棋子内存DC中贴入缩放了相应数值的棋子位图
    CDC l_CopyDC;
    l_CopyDC.CreateCompatibleDC(pDC);

    CBitmap l_CopyBitmap;
    l_CopyBitmap.CreateCompatibleBitmap(pDC, CIntChessApp::g_nBitmapWidth + l_nDegressionAddWidth, 
                                        CIntChessApp::g_nBitmapHeight + l_nDegressionAddHeight);
    CBitmap* l_pOldCopyBitmap = l_CopyDC.SelectObject(&l_CopyBitmap);

    if (CHESSMAN_WHITE == g_pActiveChessMan->m_enuColor)
    {
        l_CopyDC.StretchBlt(0, 0, CIntChessApp::g_nBitmapWidth + l_nDegressionAddWidth, 
                            CIntChessApp::g_nBitmapHeight + l_nDegressionAddHeight, 
                            CIntChessApp::g_pWhiteChessManBufferDC, 0, 0, 
                            CIntChessApp::g_nBitmapWidth + INITIAL_ADDWIDTH, 
                            CIntChessApp::g_nBitmapHeight + INITIAL_ADDHEIGHT, SRCCOPY);
    }
    else
    {
        l_CopyDC.StretchBlt(0, 0, CIntChessApp::g_nBitmapWidth + l_nDegressionAddWidth, 
                            CIntChessApp::g_nBitmapHeight + l_nDegressionAddHeight, 
                            CIntChessApp::g_pBlackChessManBufferDC, 0, 0, 
                            CIntChessApp::g_nBitmapWidth + INITIAL_ADDWIDTH, 
                            CIntChessApp::g_nBitmapHeight + INITIAL_ADDHEIGHT, SRCCOPY);
    }

    //将已经缩放了相应数值的棋子位图贴回棋子的内存DC中
    g_pActiveChessMan->m_pBufferDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + l_nDegressionAddWidth, 
                                           CIntChessApp::g_nBitmapHeight + l_nDegressionAddHeight, 
                                           &l_CopyDC, 0, 0, SRCCOPY);

    //如果操作系统是WIN9X_WINNT，则还要从全局棋子掩码位图中
    //贴入缩放了相应数值的棋子掩码位图到临时内存DC中
    if (WIN9X_WINNT == CIntChessApp::g_nOSType)
    {
        if (CHESSMAN_WHITE == g_pActiveChessMan->m_enuColor)
        {
            l_CopyDC.StretchBlt(0, 0, CIntChessApp::g_nBitmapWidth + l_nDegressionAddWidth, 
                                CIntChessApp::g_nBitmapHeight + l_nDegressionAddHeight, 
                                CIntChessApp::g_pWhiteChessManMaskDC, 0, 0, 
                                CIntChessApp::g_nBitmapWidth + INITIAL_ADDWIDTH, 
                                CIntChessApp::g_nBitmapHeight + INITIAL_ADDHEIGHT, SRCCOPY);
        }
        else
        {
            l_CopyDC.StretchBlt(0, 0, CIntChessApp::g_nBitmapWidth + l_nDegressionAddWidth, 
                                CIntChessApp::g_nBitmapHeight + l_nDegressionAddHeight, 
                                CIntChessApp::g_pBlackChessManMaskDC, 0, 0, 
                                CIntChessApp::g_nBitmapWidth + INITIAL_ADDWIDTH, 
                                CIntChessApp::g_nBitmapHeight + INITIAL_ADDHEIGHT, SRCCOPY);
        }

        //将已经缩放了相应数值的棋子掩码位图贴回棋子掩码位图的内存DC中
        g_pActiveChessMan->m_pMaskDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + l_nDegressionAddWidth, 
                                             CIntChessApp::g_nBitmapHeight + l_nDegressionAddHeight, 
                                             &l_CopyDC, 0, 0, SRCCOPY);
    }

    l_CopyDC.SelectObject(l_pOldCopyBitmap);

    //更新当前棋子宽度和高度的缩放数值
    g_pActiveChessMan->m_nAddWidth = l_nDegressionAddWidth;
    g_pActiveChessMan->m_nAddHeight = l_nDegressionAddHeight;

    //刷新棋子
    g_pActiveChessMan->ZoomDraw(pDC, rectOld);

    g_pActiveChessMan->m_pParentWnd->ReleaseDC(pDC);
}

/******************************************************************************************
void CChessMan::ZoomDraw(CDC* pDC, const CRect& rectZoom)
作者      : tangjs520       创建日期: 2003-10-31
函数名    : CChessMan::ZoomDraw
返回值    : void
参数列表  : 
  参数1: CDC* pDC
  参数2: const CRect& rectZoom
描述      : 在缩放操作时，用于刷新显示棋子，可避免闪烁
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessMan::ZoomDraw(CDC* pDC, const CRect& rectZoom)
{
    ASSERT_VALID(pDC);

    //创建临时内存DC，用于加载缩放棋子所在的棋盘背景位图，
    //同时还需加载缩放后的透明棋子位图
    CDC l_MemDC;
    l_MemDC.CreateCompatibleDC(pDC);

    int nZoomWidth = rectZoom.Width();
    int nZoomHeight = rectZoom.Height();

    CBitmap l_MemBitmap;
    l_MemBitmap.CreateCompatibleBitmap(pDC, nZoomWidth, nZoomHeight);
    CBitmap* pOldBitmap = l_MemDC.SelectObject(&l_MemBitmap);

    CDC* pChessBoardDC = ((CChessEquipmentWnd*)m_pParentWnd)->GetChessBoard()->GetDC();
    ASSERT_VALID(pChessBoardDC);

    l_MemDC.BitBlt(0, 0, nZoomWidth, nZoomHeight, pChessBoardDC, rectZoom.left, 
                   rectZoom.top, SRCCOPY);

    if (WIN9X_WINNT == CIntChessApp::g_nOSType)
    {
        l_MemDC.BitBlt(0, 0, (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                       CIntChessApp::g_nBitmapHeight + m_nAddHeight, m_pMaskDC, 
                       m_enuType * (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                       0, SRCAND);

        l_MemDC.BitBlt(0, 0, (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                       CIntChessApp::g_nBitmapHeight + m_nAddHeight, m_pBufferDC, 
                       m_enuType * (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                       0, SRCPAINT);
    }
    else
    {
        ::TransparentBlt(l_MemDC.m_hDC, 0, 0, 
                         (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                         CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                         m_pBufferDC->m_hDC, 
                         m_enuType * (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 0, 
                         (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                         CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                         (UINT)TRANSPARENT_COLOR);
    }

    pDC->BitBlt(rectZoom.left, rectZoom.top, nZoomWidth, nZoomHeight, &l_MemDC, 0, 0, SRCCOPY);

    l_MemDC.SelectObject(pOldBitmap);
}

/******************************************************************************************
void CChessMan::StartZoom()
作者      : tangjs520       创建日期: 2003-10-8
函数名    : CChessMan::StartZoom
返回值    : void
参数列表  : 无
描述      : 开始棋子的缩放操作
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessMan::StartZoom()
{
    //如果棋子已经正在进行缩放操作了，则什么也不做，立即返回
    if (!m_bZooming)
    {
        //如果先前已经有一个棋子正在进行缩放操作，则必须先停止它的缩放操作
        if (g_pActiveChessMan)
        {
            g_pActiveChessMan->StopZoom();
        }

        //更新相关变量
        g_pActiveChessMan = this;
        g_nBeforeZoomXPos = m_nXPosition;
        g_nBeforeZoomYPos = m_nYPosition;
        m_bZooming = TRUE;

        //开启定时器
        ::SetTimer(m_pParentWnd->GetSafeHwnd(), ZOOM_EVENT, ZOOM_SPEED, (TIMERPROC)ZoomingProc);
    }
}

/******************************************************************************************
void CChessMan::StopZoom()
作者      : tangjs520       创建日期: 2003-10-8
函数名    : CChessMan::StopZoom
返回值    : void
参数列表  : 无
描述      : 停止棋子的缩放操作
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessMan::StopZoom()
{
    //关闭定时器
    ::KillTimer(m_pParentWnd->GetSafeHwnd(), ZOOM_EVENT);

    //还原相关变量
    m_bZooming = FALSE;
    g_pActiveChessMan = NULL;
    m_nAddWidth = INITIAL_ADDWIDTH;
    m_nAddHeight = INITIAL_ADDHEIGHT;
    m_nXPosition = g_nBeforeZoomXPos;
    m_nYPosition = g_nBeforeZoomYPos;

    //将棋子内存DC(和棋子掩码位图的内存DC)还原
    if (CHESSMAN_WHITE == m_enuColor)
    {
        m_pBufferDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                            CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                            CIntChessApp::g_pWhiteChessManBufferDC, 0, 0, SRCCOPY);
    }
    else
    {
        m_pBufferDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                            CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                            CIntChessApp::g_pBlackChessManBufferDC, 0, 0, SRCCOPY);
    }

    if (WIN9X_WINNT == CIntChessApp::g_nOSType)
    {
        if (CHESSMAN_WHITE == m_enuColor)
        {
            m_pMaskDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                              CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                              CIntChessApp::g_pWhiteChessManMaskDC, 0, 0, SRCCOPY);
        }
        else
        {
            m_pMaskDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                              CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                              CIntChessApp::g_pBlackChessManMaskDC, 0, 0, SRCCOPY);
        }
    }

    //刷新棋子
    CDC* pDC = m_pParentWnd->GetDC();
    ASSERT_VALID(pDC);

    ZoomDraw(pDC, GetRect());

    m_pParentWnd->ReleaseDC(pDC);
}
