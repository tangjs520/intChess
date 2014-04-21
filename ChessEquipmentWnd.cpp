/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessEquipmentWnd.cpp
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
#include "ChessEquipmentWnd.h"

#include "ChessRule.h"  //走棋规则类(CChessRule)的头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChessEquipmentWnd* CChessEquipmentWnd::m_pObject = NULL;

/******************************************************************************************
CChessEquipmentWnd::CChessEquipmentWnd()
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CChessEquipmentWnd::CChessEquipmentWnd
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessEquipmentWnd::CChessEquipmentWnd()
{
    m_pChessBoard = NULL;

    for (int i = 0; i < 2; i++)
    {
        m_pCoordinateWnd[i] = NULL;
    }

    for (int i = 0; i < 16; i++)
    {
        m_pAryWhiteChessMan[i] = NULL;
        m_pAryBlackChessMan[i] = NULL;
    }

    m_rectSrcPosIndicator.SetRectEmpty();
    m_rectDestPosIndicator.SetRectEmpty();
    m_rectDestPos.SetRectEmpty();
}

/******************************************************************************************
CChessEquipmentWnd::~CChessEquipmentWnd()
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CChessEquipmentWnd::~CChessEquipmentWnd
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessEquipmentWnd::~CChessEquipmentWnd()
{
    delete m_pChessBoard;

    for (int i = 0; i < 2; i++)
    {
        delete m_pCoordinateWnd[i];
    }

    for (int i = 0; i < 16; i++)
    {
        delete m_pAryWhiteChessMan[i];
        delete m_pAryBlackChessMan[i];
    }

    //卸载全局棋子内存DC
    CIntChessApp::UninstallChessManDC();
}

BEGIN_MESSAGE_MAP(CChessEquipmentWnd, CWnd)
    //{{AFX_MSG_MAP(CChessEquipmentWnd)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
void CChessEquipmentWnd::ExchangeChessMan()
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CChessEquipmentWnd::ExchangeChessMan
返回值    : void
参数列表  : 无
描述      : 交换棋子
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessEquipmentWnd::ExchangeChessMan()
{
    LockWindowUpdate();

    //国际象棋规定"双方王、后各在同一条直线上相对；白后位于白格，黑后位于黑格"
    //所以交换棋子时，白、黑双方的"王、后"也必须互相交换位置
    m_pAryWhiteChessMan[Queen]->SetType(KING, FALSE);
    m_pAryWhiteChessMan[King]->SetType(QUEEN, FALSE);
    
    m_pAryBlackChessMan[Queen]->SetType(KING, FALSE);
    m_pAryBlackChessMan[King]->SetType(QUEEN, FALSE);

    for (register int i = 0; i < 16; i++)
    {
        m_pAryWhiteChessMan[i]->ExchangeColor();
        m_pAryBlackChessMan[i]->ExchangeColor();
    }

    //棋盘坐标也要做相应改变
    CCoordinateWnd::SetWhiteBelowOrBlackBelow(!CCoordinateWnd::GetWhiteBelowOrBlackBelow());
    for (int i = 0; i < 2; i++)
    {
        m_pCoordinateWnd[i]->Invalidate();
    }

    UnlockWindowUpdate();

    //由于棋盘坐标已经改变了，所以也必须更新棋格名称
    m_pChessBoard->ExchangeChessGridName();

    //为了保证数组名称[White和Black]和索引名称[Queen和King]在字面上的正确性，
    //两个数组的对应元素也互相交换
    CChessMan* pTemp = NULL;
    pTemp = m_pAryWhiteChessMan[Queen];
    m_pAryWhiteChessMan[Queen] = m_pAryWhiteChessMan[King];
    m_pAryWhiteChessMan[King] = pTemp;

    pTemp = m_pAryBlackChessMan[Queen];
    m_pAryBlackChessMan[Queen] = m_pAryBlackChessMan[King];
    m_pAryBlackChessMan[King] = pTemp;

    //为了在"王车易位"中准确的判断"王翼车"和"后翼车"的位置，
    //双方自己的两个车也必须互相交换位置(具体是如何判断的，
    //请参见CChessRule::AfterDispatchAction函数)
    pTemp = m_pAryWhiteChessMan[Rook];
    m_pAryWhiteChessMan[Rook] = m_pAryWhiteChessMan[Rook + 1];
    m_pAryWhiteChessMan[Rook + 1] = pTemp;

    pTemp = m_pAryBlackChessMan[Rook];
    m_pAryBlackChessMan[Rook] = m_pAryBlackChessMan[Rook + 1];
    m_pAryBlackChessMan[Rook + 1] = pTemp;

    for (int i = 0; i < 16; i++)
    {
        pTemp = m_pAryWhiteChessMan[i];
        m_pAryWhiteChessMan[i] = m_pAryBlackChessMan[i];
        m_pAryBlackChessMan[i] = pTemp;
    }
}

/******************************************************************************************
void CChessEquipmentWnd::Renew()
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessEquipmentWnd::Renew
返回值    : void
参数列表  : 无
描述      : 重新开始游戏
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessEquipmentWnd::Renew()
{
    //恢复棋子到原始棋格
    CString strH = _T("");  //横坐标名称，用于和数字组合成棋格名称

    LockWindowUpdate();

    //清除棋子位置指示框
    RemoveIndicator();

    //白兵的初始位置在"a2、b2、c2、d2、e2、f2、g2、h2"
    //黑兵的初始位置在"a7、b7、c7、d7、e7、f7、g7、h7"
    for (BYTE i = Pawn, HName = _T('a'); i < Pawn + 8; i++, HName++)
    {
        if (PAWN != m_pAryWhiteChessMan[i]->GetType())
        {
            m_pAryWhiteChessMan[i]->SetType(PAWN, FALSE);
        }
        if (PAWN != m_pAryBlackChessMan[i]->GetType())
        {
            m_pAryBlackChessMan[i]->SetType(PAWN, FALSE);
        }

        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("2")));
        m_pAryBlackChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("7")));
    }

    //白象的初始位置在"c1、f1"
    //黑象的初始位置在"c8、f8"
    for (int i = Bishop, HName = _T('c'); i < Bishop + 2; i++, HName += 3)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //白马的初始位置在"b1、g1"
    //黑马的初始位置在"b8、g8"
    for (int i = Knight, HName = _T('b'); i < Knight + 2; i++, HName += 5)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //白车的初始位置在"a1、h1"
    //黑车的初始位置在"a8、h8"
    for (int i = Rook, HName = _T('a'); i < Rook + 2; i++, HName += 7)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //白后的初始位置在"d1"
    //黑后的初始位置在"d8"
    m_pAryWhiteChessMan[Queen]->SetPosition(m_pChessBoard->GetGridFromName(_T("d1")));
    m_pAryBlackChessMan[Queen]->SetPosition(m_pChessBoard->GetGridFromName(_T("d8")));
    
    //白王的初始位置在"e1"
    //黑王的初始位置在"e8"
    m_pAryWhiteChessMan[King]->SetPosition(m_pChessBoard->GetGridFromName(_T("e1")));
    m_pAryBlackChessMan[King]->SetPosition(m_pChessBoard->GetGridFromName(_T("e8")));

    UnlockWindowUpdate();

    //棋子的初始状态是没有吃掉任何其他棋子的
    for (int i = 0; i < 16; i++)
    {
        m_pAryWhiteChessMan[i]->RemoveKilledMan(RMK_ALL);
        m_pAryBlackChessMan[i]->RemoveKilledMan(RMK_ALL);
    }
}

/******************************************************************************************
void CChessEquipmentWnd::DrawIndicator(const CRect& rectSrcPos, const CRect& rectDestPos)
作者      : tangjs520       创建日期: 2003-11-9
函数名    : CChessEquipmentWnd::DrawIndicator
返回值    : void
参数列表  : 
  参数1: const CRect& rectSrcPos
  参数2: const CRect& rectDestPos
描述      : 绘制棋子位置指示框
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessEquipmentWnd::DrawIndicator(const CRect& rectSrcPos, const CRect& rectDestPos)
{
    //首先需要保存老源位置指示框和老目标位置指示框的坐标，
    //因为以后要擦除它们
    CRect rectOldSrcPos = m_rectSrcPosIndicator;
    CRect rectOldDestPos = m_rectDestPos;

    //位置指示框的宽度和高度比棋格要缩小一个像素
    m_rectSrcPosIndicator = rectSrcPos;
    m_rectSrcPosIndicator.DeflateRect(1, 1);

    m_rectDestPosIndicator = m_rectDestPos = rectDestPos;
    m_rectDestPos.DeflateRect(1, 1);
    m_rectDestPosIndicator.DeflateRect(1, 1);

    //擦除老源位置指示框
    if (!rectOldSrcPos.IsRectEmpty())
    {
        //计算需要擦除的区域大小
        CRgn rgnOutside;
        rgnOutside.CreateRectRgnIndirect(rectOldSrcPos);

        CRect rectTemp = rectOldSrcPos;
        rectTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

        CRgn rgnInside;
        rgnInside.CreateRectRgnIndirect(rectTemp);

        rgnOutside.CombineRgn(&rgnOutside, &rgnInside, RGN_DIFF);

        InvalidateRgn(&rgnOutside);
    }

    //擦除老目标位置指示框
    if (!rectOldDestPos.IsRectEmpty())
    {
        //关闭定时器(即先必须停止掉老目标位置指示框的闪烁)
        ::KillTimer(m_hWnd, INDICATOR_FLASH_EVENT);

        //计算需要擦除的区域大小
        CRgn rgnOutside;
        rgnOutside.CreateRectRgnIndirect(rectOldDestPos);

        CRect rectTemp = rectOldDestPos;
        rectTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

        CRgn rgnInside;
        rgnInside.CreateRectRgnIndirect(rectTemp);

        rgnOutside.CombineRgn(&rgnOutside, &rgnInside, RGN_DIFF);

        InvalidateRgn(&rgnOutside);
    }

    //绘制新源位置指示框
    InvalidateRect(m_rectSrcPosIndicator, FALSE);

    //绘制新目标位置指示框
    CClientDC dc(this);
    CPen IndicatorPen(PS_INSIDEFRAME, INDICATOR_WIDTH, INDICATOR_COLOR);
    CPen* pOldPen = dc.SelectObject(&IndicatorPen);
    dc.SelectStockObject(NULL_BRUSH);
    dc.Rectangle(m_rectDestPosIndicator);
    dc.SelectObject(pOldPen);

    //计算新目标位置指示框刷新区域的大小，在定时器内使用
    m_rgnInvalidate.DeleteObject();
    m_rgnInvalidate.CreateRectRgnIndirect(m_rectDestPosIndicator);

    CRect rectDestTemp = m_rectDestPosIndicator;
    rectDestTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

    CRgn rgnDestInside;
    rgnDestInside.CreateRectRgnIndirect(rectDestTemp);

    m_rgnInvalidate.CombineRgn(&m_rgnInvalidate, &rgnDestInside, RGN_DIFF);

    //开启定时器(即在等待INDICATOR_FLASH_SPEED毫秒后，开始新目标位置指示框的闪烁)
    ::SetTimer(m_hWnd, INDICATOR_FLASH_EVENT, INDICATOR_FLASH_SPEED, 
               (TIMERPROC)IndicatorFlashingProc);
}

/******************************************************************************************
void CALLBACK CChessEquipmentWnd::IndicatorFlashingProc(HWND, UINT, UINT_PTR, DWORD)
作者      : tangjs520       创建日期: 2003-11-9
函数名    : CChessEquipmentWnd::IndicatorFlashingProc
返回值    : void CALLBACK
参数列表  : 
  参数1: HWND
  参数2: UINT
  参数3: UINT_PTR
  参数4: DWORD
描述      : 处理棋子位置指示框的闪烁操作
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CALLBACK CChessEquipmentWnd::IndicatorFlashingProc(HWND, UINT, UINT_PTR, DWORD)
{
    BOOL bErase = FALSE;

    //目标位置变量m_rectDestPosIndicator从置空到赋值来回变化，
    //从而产生闪烁效果
    if (m_pObject->m_rectDestPosIndicator.IsRectEmpty())
    {
        m_pObject->m_rectDestPosIndicator = m_pObject->m_rectDestPos;
    }
    else
    {
        m_pObject->m_rectDestPosIndicator.SetRectEmpty();

        bErase = TRUE;
    }

    ::InvalidateRgn(m_pObject->GetSafeHwnd(), (HRGN)m_pObject->m_rgnInvalidate, bErase);
}

/******************************************************************************************
int CChessEquipmentWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CChessEquipmentWnd::OnCreate
返回值    : int
参数列表  : 
  参数1: LPCREATESTRUCT lpCreateStruct
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
int CChessEquipmentWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    //创建棋盘
    m_pChessBoard = new CChessBoard(this);
    ASSERT(m_pChessBoard);

    //创建坐标窗口
    m_pCoordinateWnd[0] = new CCoordinateWnd(TRUE);
    ASSERT_VALID(m_pCoordinateWnd[0]);
    m_pCoordinateWnd[1] = new CCoordinateWnd(FALSE);
    ASSERT_VALID(m_pCoordinateWnd[1]);
    m_pCoordinateWnd[0]->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, 
                                CRect(CHESSBOARDMARGIN / 2, CHESSBOARDMARGIN / 2 + 8 * CHESSGRIDHEIGHT + 1, 
                                CHESSBOARDMARGIN / 2 + 8 * CHESSGRIDWIDTH, 
                                CHESSBOARDMARGIN / 2 + 8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN / 2 - 2), 
                                this, 0);
    m_pCoordinateWnd[1]->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, 
                                CRect(2, CHESSBOARDMARGIN / 2, CHESSBOARDMARGIN / 2 - 1, 
                                CHESSBOARDMARGIN / 2 + 8 * CHESSGRIDHEIGHT), this, 0);

    //在创建棋子前，必须先保证加载棋子位图到相应的全局内存DC中
    CIntChessApp::InstallChessManDC(this);

    //创建棋子
    CString strH = _T("");  //横坐标名称，用于和数字组合成棋格名称

    //白兵的初始位置在"a2、b2、c2、d2、e2、f2、g2、h2"
    //黑兵的初始位置在"a7、b7、c7、d7、e7、f7、g7、h7"
    for (BYTE i = Pawn, HName = _T('a'); i < Pawn + 8; i++, HName++)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i] = new CChessMan(CHESSMAN_WHITE, PAWN, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("2")));
        m_pAryBlackChessMan[i] = new CChessMan(CHESSMAN_BLACK, PAWN, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("7")));
    }

    //白象的初始位置在"c1、f1"
    //黑象的初始位置在"c8、f8"
    for (int i = Bishop, HName = _T('c'); i < Bishop + 2; i++, HName += 3)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i] = new CChessMan(CHESSMAN_WHITE, BISHOP, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i] = new CChessMan(CHESSMAN_BLACK, BISHOP, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //白马的初始位置在"b1、g1"
    //黑马的初始位置在"b8、g8"
    for (int i = Knight, HName = _T('b'); i < Knight + 2; i++, HName += 5)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i] = new CChessMan(CHESSMAN_WHITE, KNIGHT, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i] = new CChessMan(CHESSMAN_BLACK, KNIGHT, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //白车的初始位置在"a1、h1"
    //黑车的初始位置在"a8、h8"
    for (int i = Rook, HName = _T('a'); i < Rook + 2; i++, HName += 7)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i] = new CChessMan(CHESSMAN_WHITE, ROOK, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i] = new CChessMan(CHESSMAN_BLACK, ROOK, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //白后的初始位置在"d1"
    //黑后的初始位置在"d8"
    m_pAryWhiteChessMan[Queen] = new CChessMan(CHESSMAN_WHITE, QUEEN, this, 
                                               m_pChessBoard->GetGridFromName(_T("d1")));
    m_pAryBlackChessMan[Queen] = new CChessMan(CHESSMAN_BLACK, QUEEN, this, 
                                               m_pChessBoard->GetGridFromName(_T("d8")));
    
    //白王的初始位置在"e1"
    //黑王的初始位置在"e8"
    m_pAryWhiteChessMan[King] = new CChessMan(CHESSMAN_WHITE, KING, this, 
                                              m_pChessBoard->GetGridFromName(_T("e1")));
    m_pAryBlackChessMan[King] = new CChessMan(CHESSMAN_BLACK, KING, this, 
                                              m_pChessBoard->GetGridFromName(_T("e8")));

    //在使用走棋规则类之前，必须初始化该静态变量
    CChessRule::m_pChessEquipmentWnd = m_pObject = this;

    return 0;
}

/******************************************************************************************
BOOL CChessEquipmentWnd::OnEraseBkgnd(CDC* pDC)
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CChessEquipmentWnd::OnEraseBkgnd
返回值    : BOOL
参数列表  : 
  参数1: CDC* pDC
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessEquipmentWnd::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pChessBoard)
    {
        //绘制棋盘
        m_pChessBoard->Draw(pDC);

        return TRUE;
    }
    else
    {
        return CWnd::OnEraseBkgnd(pDC);
    }
}

/******************************************************************************************
void CChessEquipmentWnd::OnPaint()
作者      : tangjs520       创建日期: 2003-10-28
函数名    : CChessEquipmentWnd::OnPaint
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessEquipmentWnd::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    
    // TODO: Add your message handler code here
    for (register int i = 0; i < 16; i++)
    {
        //绘制白棋
        if (m_pAryWhiteChessMan[i])
        {
            m_pAryWhiteChessMan[i]->Draw(&dc);
        }

        //绘制黑棋
        if (m_pAryBlackChessMan[i])
        {
            m_pAryBlackChessMan[i]->Draw(&dc);
        }
    }

    //绘制棋子源位置和目标位置指示框
    if (!m_rectSrcPosIndicator.IsRectEmpty())
    {
        CPen IndicatorPen(PS_INSIDEFRAME, INDICATOR_WIDTH, INDICATOR_COLOR);
        CPen* pOldPen = dc.SelectObject(&IndicatorPen);
        dc.SelectStockObject(NULL_BRUSH);

        dc.Rectangle(m_rectSrcPosIndicator);
        //目标位置变量m_rectDestPosIndicator在定时器内从置空到赋值来回变化，
        //从而产生闪烁效果
        if (!m_rectDestPosIndicator.IsRectEmpty())
        {
            dc.Rectangle(m_rectDestPosIndicator);
        }

        dc.SelectObject(pOldPen);
    }

    // Do not call CWnd::OnPaint() for painting messages
}

/******************************************************************************************
void CChessEquipmentWnd::OnLButtonDown(UINT nFlags, CPoint point)
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CChessEquipmentWnd::OnLButtonDown
返回值    : void
参数列表  : 
  参数1: UINT nFlags
  参数2: CPoint point
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessEquipmentWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (AUTOMODE == CChessRule::GetUserMode())
    {
        CWnd::OnLButtonDown(nFlags, point);
        return;
    }

    CRect ValidRect = m_pChessBoard->GetValidRect();
    if (ValidRect.PtInRect(point))
    {
        CChessGrid* pGrid = m_pChessBoard->GetGridFromPoint(point);
        if (pGrid)
        {
            //交由走棋规则类(CChessRule)根据走棋规则来处理用户的行为
            CChessRule::DoSomething(pGrid);
        }
    }

    CWnd::OnLButtonDown(nFlags, point);
}
