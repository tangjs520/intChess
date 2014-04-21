/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessManualView.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-11-21
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

#include "ChessRule.h"  //走棋规则类(CChessRule)的头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CChessManualView, CView)

/******************************************************************************************
CChessManualView::CChessManualView()
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CChessManualView::CChessManualView
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessManualView::CChessManualView()
{
    m_pBkgndBmp = NULL;
    //存储最原始局面出现的初始次数
    m_PhaseMap.SetAt(ORIGINAL_PHASE, (BYTE)1);
}

/******************************************************************************************
CChessManualView::~CChessManualView()
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CChessManualView::~CChessManualView
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessManualView::~CChessManualView()
{
    if (m_pBkgndBmp)
    {
        m_pBkgndBmp->DeleteObject();
        delete m_pBkgndBmp;
    }
}

BEGIN_MESSAGE_MAP(CChessManualView, CView)
    //{{AFX_MSG_MAP(CChessManualView)
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_WRITECHESSMANUAL, OnWriteChessManual)
    ON_MESSAGE(WM_CLEARCHESSMANUAL, OnEmptyChessManual)
    ON_BN_CLICKED(IDC_UNDO, OnUndoBtnClicked)
END_MESSAGE_MAP()

/******************************************************************************************
void CChessManualView::OnDraw(CDC* pDC)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CChessManualView::OnDraw
返回值    : void
参数列表  : 
  参数1: CDC* pDC
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessManualView::OnDraw(CDC* pDC)
{
    UNREFERENCED_PARAMETER(pDC);
    //CDocument* pDoc = GetDocument();
    // TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CChessManualView diagnostics

#ifdef _DEBUG
void CChessManualView::AssertValid() const
{
    CView::AssertValid();
}

void CChessManualView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif //_DEBUG

/******************************************************************************************
BOOL CChessManualView::OnEraseBkgnd(CDC* pDC)
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CChessManualView::OnEraseBkgnd
返回值    : BOOL
参数列表  : 
  参数1: CDC* pDC
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessManualView::OnEraseBkgnd(CDC* pDC) 
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

        return TRUE;
    }
    else
    {
        return CView::OnEraseBkgnd(pDC);
    }
}

/******************************************************************************************
int CChessManualView::OnCreate(LPCREATESTRUCT lpCreateStruct)
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CChessManualView::OnCreate
返回值    : int
参数列表  : 
  参数1: LPCREATESTRUCT lpCreateStruct
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
int CChessManualView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
    bResult = CIntChessApp::SetBkgndBmp(IDB_WOOD, m_pBkgndBmp);
    if (!bResult)
    {
        TRACE0("Failed to create bitmap background\n");
        return -1;
    }

    //创建列表框控件使用的字体对象
    //使用宋体9号字体
    CDC* pDC = GetDC();
    ASSERT_VALID(pDC);
    m_ListBoxFont.CreateFont(MulDiv(9, -pDC->GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, 
                             FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                             DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));
    ReleaseDC(pDC);

    //创建列表框控件
    m_ChessManualListBox.CreateEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR 
                                  | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE, _T("ListBox"), NULL, 
                                  WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT, 
                                  CRect(CPoint(80, 10), CSize(115, 124)), this, 0);
    m_ChessManualListBox.SetFont(&m_ListBoxFont, FALSE);

    //创建"悔棋"按钮(由于目前还没有开始走棋，所以该按钮的初始状态是禁止使用状态)
    m_btnUndo.Create(_T("悔棋"), BS_PUSHBUTTON | WS_BORDER | WS_CHILD | WS_VISIBLE | WS_DISABLED, 
                     CRect(CPoint(10, 111), CSize(60, 23)), this, IDC_UNDO);
    m_btnUndo.SetFont(&m_ListBoxFont, FALSE);

    //在使用走棋规则类之前，必须初始化该静态变量
    CChessRule::m_pChessManalView = this;

    return 0;
}

/******************************************************************************************
LRESULT CChessManualView::OnWriteChessManual(WPARAM wParam, LPARAM lParam)
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CChessManualView::OnWriteChessManual
返回值    : LRESULT(返回1表示形成和局)
参数列表  : 
  参数1: WPARAM wParam
  参数2: LPARAM lParam
描述      : 记录棋谱，同时判断是否形成"三次重复局面"与"50回合规则"和局
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
LRESULT CChessManualView::OnWriteChessManual(WPARAM wParam, LPARAM lParam)
{
    if (!m_btnUndo.IsWindowEnabled())
    {
        m_btnUndo.EnableWindow(TRUE);
    }

    LRESULT dwRet = 0L;

    CM* pCMInfo = (CM*)wParam;
    WORD nStepCount = CChessRule::GetStepCount();

    static CString strBoutCount = _T("");
    //如果步数是奇数，则表示在记录棋谱时需要加上回合数
    if (nStepCount % 2)
    {
        strBoutCount.Format(_T("%3u: "), nStepCount / 2 + 1);
    }
    //否则需要在前面填入若干个空格(空格的数量等于最近一次回合数字符串的长度)
    else
    {
        strBoutCount = CString(_T(' '), strBoutCount.GetLength());
    }

    //棋谱的头部分需要填入棋子的类型(兵种)
    //但是，如果棋子的类型是"兵"，或者用户的行为是"王车易位"，或是"兵的升变"，
    //则不用记录棋谱的头部分
    CString strHeader = _T("");
    if (PAWN != pCMInfo->chessManType 
        && TA_KINGROOKSHIFT != pCMInfo->nAction 
        && PAWNUP != pCMInfo->nPawnUp)
    {
        strHeader = strBoutCount + g_strChessManName[pCMInfo->chessManType];
    }
    else
    {
        strHeader = strBoutCount;
    }

    //依据用户的不同行为，选用相应的行为符号
    CString strAction = _T("");
    switch (pCMInfo->nAction)
    {
    case TA_MOVE:
        strAction = _T("－");
        //由于已经走动了"兵"，所以此步数之前的局面是不可能形成50回合规则的和局条件的
        //存储当前的步数值
        if (PAWN == pCMInfo->chessManType || PAWNUP == pCMInfo->nPawnUp)
        {
            m_nNoDrawOf50TurnsList.AddTail(nStepCount);
        }
        break;
    case TA_KILL:
    case TA_KILLPASSBYPAWN:
        strAction = _T("×");
        //由于已经吃子，所以此步数之前的局面是不可能形成50回合规则的和局条件的
        //存储当前的步数值
        m_nNoDrawOf50TurnsList.AddTail(nStepCount);
    default:
        break;
    }

    CString strPawnUpType = _T("");
    //如果是"兵的升变"，则还要在目标棋格名称后面加上升变以后的棋子类型(兵种)
    if (PAWNUP == pCMInfo->nPawnUp)
    {
        strPawnUpType = g_strChessManName[pCMInfo->chessManType];
    }

    //依据对方"王"的不同状态，选用相应的结束符号
    CString strEnd = _T("");
    switch (pCMInfo->nKingStatus)
    {
    case KS_CHECKED:
        strEnd = CString(_T('+'), pCMInfo->nCheckCount);
        break;
    case KS_CHECKMATED:
        strEnd = _T("×");
        break;
    default:
        break;
    }

    //产生并显示最终的棋谱字符串
    CString strResult = _T("");
    if (TA_KINGROOKSHIFT != pCMInfo->nAction)
    {
        strResult = strHeader 
            + pCMInfo->strSrcGridName 
            + strAction 
            + pCMInfo->strDestGridName 
            + strPawnUpType 
            + strEnd;
    }
    else
    {
        TCHAR ch = pCMInfo->strDestGridName[0];
        //短易位
        if (_T('g') == ch)
        {
            strResult = strHeader + _T("0—0") + strEnd;
        }
        //长易位
        else
        {
            strResult = strHeader + _T("0—0—0") + strEnd;
        }
    }
    m_ChessManualListBox.AddString(strResult);

    //列表框内容的索引值
    int nIndex = nStepCount - 1;

    //将当前局面值与列表框的相应棋谱字符串关联起来
    DWORD dwPhaseValue = (DWORD)lParam;
    m_ChessManualListBox.SetItemData(nIndex, dwPhaseValue);
    //判断是否出现了三次重复的局面
    //国际象棋规定：出现或将要出现第三次(无论是连续重复还是前后累加形成的三次)
    //重复相同的局面，每次都是轮到同一方走棋时，轮走的一方可以提请判和。
    //长将和棋，也可按三次重复局面判和。
    BYTE nCount = 1;
    if (m_PhaseMap.Lookup(dwPhaseValue, nCount))
    {
        nCount++;
    }
    m_PhaseMap.SetAt(dwPhaseValue, nCount);

    if (MANUALMODE == CChessRule::GetUserMode())
    {
        if (REPEAT_PHASE_COUNT == nCount)
        {
            pCMInfo->nKingStatus = KS_DRAW;
            dwRet = 1L;
        }

        //国际象棋规定：白、黑双方各走一着称作一个回合。如果在最近连续50回合中，
        //双方均未吃过一子，也未走动过一兵，可提请判和。
        //判断是否符合50回合规则和局
        if (nStepCount >= DRAWOF50TURNS_STEPCOUNT && 
            (KS_NORMAL == pCMInfo->nKingStatus || KS_CHECKED == pCMInfo->nKingStatus))
        {
            WORD nNoDrawOf50Turns_Step = 0;
            if (!m_nNoDrawOf50TurnsList.IsEmpty())
            {
                nNoDrawOf50Turns_Step = m_nNoDrawOf50TurnsList.GetTail();
            }
            if (nNoDrawOf50Turns_Step % 2)
            {
                nNoDrawOf50Turns_Step++;
            }

            if (DRAWOF50TURNS_STEPCOUNT == (nStepCount - nNoDrawOf50Turns_Step))
            {
                pCMInfo->nKingStatus = KS_DRAW;
                dwRet = 1L;
            }
        }
    }

    //记录比赛结果
    switch (pCMInfo->nKingStatus)
    {
    case KS_CHECKMATED:
        if (nStepCount % 2)
        {
            //白方胜
            m_ChessManualListBox.AddString(_T("     1 ： 0"));
        }
        else
        {
            //黑方胜
            m_ChessManualListBox.AddString(_T("     0 ： 1"));
        }
        nIndex++;
        break;
    case KS_DRAW:
        //和局
        m_ChessManualListBox.AddString(_T("   1/2 ： 1/2"));
        nIndex++;
        break;
    default:
        break;
    }

    //选中最后一条列表项
    m_ChessManualListBox.SetCurSel(nIndex);

    return dwRet;
}

/******************************************************************************************
LRESULT CChessManualView::OnEmptyChessManual(WPARAM, LPARAM)
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CChessManualView::OnEmptyChessManual
返回值    : LRESULT
参数列表  : 
  参数1: WPARAM
  参数2: LPARAM
描述      : 清空棋谱以及与棋谱相关的成员变量
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
LRESULT CChessManualView::OnEmptyChessManual(WPARAM, LPARAM)
{
    m_ChessManualListBox.ResetContent();
    m_ChessManualListBox.UpdateWindow();

    m_nNoDrawOf50TurnsList.RemoveAll();

    m_PhaseMap.RemoveAll();
    //存储最原始局面出现的初始次数
    m_PhaseMap.SetAt(ORIGINAL_PHASE, (BYTE)1);

    //禁止"悔棋"按钮的使用
    m_btnUndo.EnableWindow(FALSE);

    return 0L;
}

/******************************************************************************************
void CChessManualView::OnUndoBtnClicked()
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CChessManualView::OnUndoBtnClicked
返回值    : void
参数列表  : 无
描述      : 处理"悔棋"按钮单击的消息响应函数
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessManualView::OnUndoBtnClicked()
{
    if (AUTOMODE == CChessRule::GetUserMode())
    {
        return;
    }

    int nLastIndex = m_ChessManualListBox.GetCount() - 1;

    //获取最近一次走子所形成的局面值
    DWORD dwLastPhaseValue = m_ChessManualListBox.GetItemData(nLastIndex);
    //在m_PhaseMap集合中更新上述局面值出现的次数，若该局面值只出现过一次，则删除此局面值
    BYTE nCount = 0;
    m_PhaseMap.Lookup(dwLastPhaseValue, nCount);
    if (1 == nCount)
    {
        ASSERT(m_PhaseMap.RemoveKey(dwLastPhaseValue));
    }
    else
    {
        m_PhaseMap.SetAt(dwLastPhaseValue, --nCount);
    }

    //删除最近一次走子的棋谱
    m_ChessManualListBox.DeleteString(nLastIndex);
    //选中当前最后一条列表项
    m_ChessManualListBox.SetCurSel(nLastIndex - 1);

    //更新计算50回合时的起始步数
    const CM* pLastCMInfo = CChessRule::GetLastCMInfo();
    ASSERT(pLastCMInfo);
    if (((TA_MOVE == pLastCMInfo->nAction) 
        && (PAWN == pLastCMInfo->chessManType || PAWNUP == pLastCMInfo->nPawnUp)) 
        || TA_KILL == pLastCMInfo->nAction || TA_KILLPASSBYPAWN == pLastCMInfo->nAction)
    {
        if (!m_nNoDrawOf50TurnsList.IsEmpty())
        {
            m_nNoDrawOf50TurnsList.RemoveTail();
        }
    }

    //如果已经悔到第一步棋了，则禁止"悔棋"按钮的继续使用
    if (!nLastIndex)
    {
        m_btnUndo.EnableWindow(FALSE);
    }

    //交由走棋规则类(CChessRule)继续处理"悔棋"的相关事宜
    CChessRule::Undo();
}
