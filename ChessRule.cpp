/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessRule.cpp
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
#include "ChessRule.h"

#include "ChessEquipmentWnd.h"  //棋具窗口对象类(CChessEquipmentWnd)的头文件
#include "ChessDeskWnd.h"       //棋桌窗口对象类(CChessEquipmentWnd)的头文件

#include "PawnUpDlg.h"          //"兵的升变"对话框类(CPawnUpDlg)的头文件
#include "ResultDlg.h"          //"比赛结果"对话框类(CResultDlg)的头文件

#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//首先由白方走子
BOOL CChessRule::m_bWhichColorHasTurn = WHITE_TURN;

CString CChessRule::m_strPassByPawnGridName = _T("");

CTG CChessRule::m_WhiteCTG;
CTG CChessRule::m_BlackCTG;

WORD CChessRule::m_nStepCount = 0;

//默认是手动模式
BYTE CChessRule::m_nUserMode = MANUALMODE;

CList<CM, CM> CChessRule::m_CMList;

CChessEquipmentWnd* CChessRule::m_pChessEquipmentWnd = NULL;
CChessManualView* CChessRule::m_pChessManalView = NULL;
CIntChessDoc* CChessRule::m_pIntChessDoc = NULL;
CChessDeskWnd* CChessRule::m_pChessDeskWnd = NULL;

BYTE CChessRule::m_nPawnUp = 0;

//填充行为分发机制映射表
ACTMAP CChessRule::_actionEntries[] = 
{
    TA_MOVE, OnMove, 
    TA_KILL, OnKill, 
    TA_KILLPASSBYPAWN, OnKillPassByPawn, 
    TA_KINGROOKSHIFT, OnKingRookShift, 
    TA_ILLEGAL, OnIllegal,
};

/******************************************************************************************
CChessRule::CChessRule()
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::CChessRule
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessRule::CChessRule()
{
}

/******************************************************************************************
CChessRule::~CChessRule()
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::~CChessRule
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessRule::~CChessRule()
{
}

/******************************************************************************************
void CChessRule::DoSomething(CChessGrid* const pDestGrid)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::DoSomething
返回值    : void
参数列表  : 
  参数1: CChessGrid* const pDestGrid
描述      : 根据走棋规则，对用户单击棋盘的行为做出相应的处理
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::DoSomething(CChessGrid* const pDestGrid)
{
    ASSERT(pDestGrid);

    CChessMan* pMan = pDestGrid->GetIncludeChessMan();
    //如果选中了棋子，同时正好轮到该颜色的棋子走棋，则棋子进行缩放操作，
    //表示该棋子是当前正选中的棋子，下面的有效行为都是针对该棋子操作的
    if (pMan && m_bWhichColorHasTurn == (BOOL)pMan->GetColor())
    {
        //如果棋局还尚未开始计时，则启动计时器开始进行计时操作
        if (FALSE == m_pChessDeskWnd->IsTimerRunning())
        {
            m_pChessDeskWnd->StartTimer(m_bWhichColorHasTurn);
        }

        pMan->StartZoom();
    }
    //如果选中了空的棋格或是选中了对方的棋子，则进行解析和分发用户行为的操作
    else if (CChessMan* pSrcMan = CChessMan::GetActiveChessMan())
    {
        ACT act = {0, pSrcMan, pSrcMan->GetInsideChessGrid(), pDestGrid};
        TranslateAction(act);
        DispatchAction(act);
    }
}

/******************************************************************************************
void CChessRule::TranslateAction(ACT& act)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::TranslateAction
返回值    : void
参数列表  : 
  参数1: ACT& act
描述      : 解析用户的行为
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::TranslateAction(ACT& act)
{
    //先进行预解析，如果预解析已经解析了用户的行为，则直接返回
    if (PreTranslateAction(act))
    {
        return;
    }

    //获取被选中棋子可以到达的棋格的集合
    CStringList strLstLegalGridName;
    GetLegalGridName(act.pSrcMan->GetColor(), act.pSrcMan->GetType(), 
                     act.pSrcGrid->GetName(), strLstLegalGridName);

    //如果目标棋格在这个集合内，则接着判断是移动棋子的行为还是吃掉敌子的行为
    if (strLstLegalGridName.Find(act.pDestGrid->GetName()) != NULL)
    {
        if (act.pDestGrid->GetIncludeChessMan())
        {
            act.nAct = TA_KILL;
        }
        else
        {
            act.nAct = TA_MOVE;
        }
    }
    //否则该行为是无效的
    else
    {
        act.nAct = TA_ILLEGAL;
    }
}

/******************************************************************************************
void CChessRule::DispatchAction(ACT& act)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::DispatchAction
返回值    : void
参数列表  : 
  参数1: ACT& act
描述      : 分发用户的行为
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::DispatchAction(ACT& act)
{
    //在分发行为之前，做相关的处理
    PreDispatchAction(act);

    //依据分发机制映射表的内容，分发用户的行为给相应的函数处理
    for (int i = 0; i < DIM(_actionEntries); i++)
    {
        if (act.nAct == _actionEntries[i].nAct)
        {
            (*_actionEntries[i].pfn)(act);

            break;
        }
    }

    //在处理完用户的行为后，做相关的处理
    AfterDispatchAction(act);
}

/******************************************************************************************
BOOL CChessRule::PreTranslateAction(ACT& act)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::PreTranslateAction
返回值    : BOOL
参数列表  : 
  参数1: ACT& act
描述      : 判断是否形成"王车易位"或"吃过路兵"
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::PreTranslateAction(ACT& act)
{
    CHESSMANTYPE chessManType = act.pSrcMan->GetType();
    if (KING == chessManType || PAWN == chessManType)
    {
        CHESSMANCOLOR chessManColor = act.pSrcMan->GetColor();
        CString strDestGridName = act.pDestGrid->GetName();

        //判断是否形成"王车易位"
        if (KING == chessManType)
        {
            PCTG pCTG = (CHESSMAN_WHITE == chessManColor) ? &m_WhiteCTG : &m_BlackCTG;
            //如果王没有移动过，且王翼车或后翼车没有移动过，才进行判断
            //另外，即使王没有移动过，但是如果王翼车和后翼车都已经移动了，也就没有必要进行判断了
            if (!pCTG->nKing_ShiftStep 
                && (!pCTG->nRookOfKingSide_ShiftStep || !pCTG->nRookOfQueenSide_ShiftStep) 
                && IsKingRookShift(chessManColor, strDestGridName))
            {
                act.nAct = TA_KINGROOKSHIFT;
                return TRUE;
            }
        }
        //判断是否形成"吃过路兵"
        else
        {
            //如果存在"过路兵"，则进行判断
            if ((!m_strPassByPawnGridName.IsEmpty()) 
                && IsKillPassByPawn(chessManColor, act.pSrcGrid->GetName(), strDestGridName))
            {
                act.nAct = TA_KILLPASSBYPAWN;
                return TRUE;
            }
        }
    }

    return FALSE;
}

/******************************************************************************************
void CChessRule::PreDispatchAction(ACT& act)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::PreDispatchAction
返回值    : void
参数列表  : 
  参数1: ACT& act
描述      : 1、假设用户的行为合法，判断己方"王"是否会受对方棋子的攻击
            2、判断是否形成"兵的升变"
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::PreDispatchAction(ACT& act)
{
    if (MANUALMODE == m_nUserMode)
    {
        if (TA_ILLEGAL == act.nAct)
        {
            return;
        }

        CHESSMANCOLOR chessManColor = act.pSrcMan->GetColor();

        //首先必须判断，如果处理完该行为后，己方"王"是否会受对方棋子的攻击
        //如果是，则此次行为属于非法操作
        //先暂时模拟棋子处理完相应的行为后，棋格应有的状态
        CChessMan* pKilledMan = NULL;
        CChessGrid* pPassByGrid = NULL;

        switch (act.nAct)
        {
        case TA_KILL:
            pKilledMan = act.pDestGrid->GetIncludeChessMan();
            pKilledMan->ShowChessMan(SCM_HIDE, FALSE);
            break;
        case TA_KILLPASSBYPAWN:
            pPassByGrid = (m_pChessEquipmentWnd->GetChessBoard())->GetGridFromName(m_strPassByPawnGridName);
            pKilledMan = pPassByGrid->GetIncludeChessMan();
            pPassByGrid->SetIncludeChessMan(NULL);
            pKilledMan->ShowChessMan(SCM_HIDE, FALSE);
            break;
        default:
            break;
        }

        act.pSrcGrid->SetIncludeChessMan(NULL);
        act.pDestGrid->SetIncludeChessMan(act.pSrcMan);
        act.pSrcMan->SetInsideChessGrid(act.pDestGrid);

        //接着判断在模拟的状态下，己方"王"是否会受对方棋子的攻击
        BOOL bKingChecked = IsKingChecked(chessManColor);

        //然后还原模拟前的棋格状态
        act.pSrcGrid->SetIncludeChessMan(act.pSrcMan);
        act.pSrcMan->SetInsideChessGrid(act.pSrcGrid);

        switch (act.nAct)
        {
        case TA_MOVE:
            act.pDestGrid->SetIncludeChessMan(NULL);
            break;
        case TA_KILL:
            pKilledMan->ShowChessMan(SCM_SHOW, FALSE);
            act.pDestGrid->SetIncludeChessMan(pKilledMan);
            break;
        case TA_KILLPASSBYPAWN:
            pKilledMan->ShowChessMan(SCM_SHOW, FALSE);
            pPassByGrid->SetIncludeChessMan(pKilledMan);
            act.pDestGrid->SetIncludeChessMan(NULL);
            break;
        default:
            break;
        }

        //如果己方"王"受到对方棋子攻击，则此次行为属于非法操作
        if (bKingChecked)
        {
            act.nAct = TA_ILLEGAL;
            return;
        }

        CHESSMANTYPE chessManType = act.pSrcMan->GetType();
        CString strSrcGridName = act.pSrcGrid->GetName();

        //先清空标识"兵的升变"的变量
        m_nPawnUp = 0;

        //判断是否形成"兵的升变"
        if (PAWN == chessManType && TA_KILLPASSBYPAWN != act.nAct)
        {
            if ((CHESSMAN_WHITE == chessManColor && strSrcGridName.Find(_T('7')) != -1) 
                || (CHESSMAN_BLACK == chessManColor && strSrcGridName.Find(_T('2')) != -1))
            {
                CPawnUpDlg dlg(chessManColor);
                dlg.DoModal();
                act.pSrcMan->SetType(dlg.m_enuUpType, FALSE);

                //更新该变量，标识当前是"兵的升变"
                m_nPawnUp = PAWNUP;
            }
        }
    }
    else if (PAWNUP == m_nPawnUp)
    {
        const CM* pLastCMInfo = GetLastCMInfo();
        ASSERT(pLastCMInfo);

        act.pSrcMan->SetType(pLastCMInfo->chessManType, FALSE);
    }
}

/******************************************************************************************
void CChessRule::AfterDispatchAction(const ACT& act)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::AfterDispatchAction
返回值    : void
参数列表  : 
  参数1: const ACT& act
描述      : 依据不同的行为类型播放不同的声效，同时维护记录"过路兵"位置
            和记录白(黑)方"王"、"王翼车"、"后翼车"移动情况的成员变量
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::AfterDispatchAction(const ACT& act)
{
    CHESSMANTYPE chessManType = act.pSrcMan->GetType();

    if (MANUALMODE == m_nUserMode)
    {
        if (TA_ILLEGAL == act.nAct)
        {
            return;
        }

        //棋局已经被修改
        if (!((CDocument*)m_pIntChessDoc)->IsModified())
        {
            ((CDocument*)m_pIntChessDoc)->SetModifiedFlag(TRUE);
        }

        //步数增一
        m_nStepCount++;

        //绘制棋子位置指示框
        m_pChessEquipmentWnd->DrawIndicator(act.pSrcGrid->GetRect(), act.pDestGrid->GetRect());

        //获取对方"王"的状态
        BYTE nCheckCount = 0;   //"将军"棋子的个数
        BYTE nKingStatus = GetKingStatus((CHESSMANCOLOR)!m_bWhichColorHasTurn, nCheckCount);

        //记录棋谱，同时判断是否形成"三次重复局面"或"50回合规则"和局
        CM CMInfo;
        CMInfo.nAction = act.nAct;
        CMInfo.nKingStatus = nKingStatus;
        CMInfo.nPawnUp = m_nPawnUp;
        CMInfo.nCheckCount = nCheckCount;
        CMInfo.chessManType = chessManType;
        CMInfo.strSrcGridName = act.pSrcGrid->GetName();
        CMInfo.strDestGridName = act.pDestGrid->GetName();

        //存储CM对象(悔棋时使用)
        m_CMList.AddTail(CMInfo);

        //计算局面值
        DWORD dwPhaseValue = CalcPhaseValue();

        LRESULT dwResult = ::SendMessage(((CWnd*)m_pChessManalView)->GetSafeHwnd(), 
                                         WM_WRITECHESSMANUAL, (WPARAM)&CMInfo, (LPARAM)dwPhaseValue);
        //返回1表示形成和局
        if (1L == dwResult)
        {
            nKingStatus = KS_DRAW;
        }

        switch (nKingStatus)
        {
        //正常
        case KS_NORMAL:
            {
                switch (act.nAct)
                {
                case TA_MOVE:
                    CIntChessApp::PlayWaveResource(_T("IDR_MOVED"));
                    break;
                case TA_KILL:
                case TA_KILLPASSBYPAWN:
                    CIntChessApp::PlayWaveResource(_T("IDR_KILLED"));
                    break;
                case TA_KINGROOKSHIFT:
                    CIntChessApp::PlayWaveResource(_T("IDR_SHIFTED"));
                    break;
                default:
                    ASSERT(FALSE);
                    break;
                }
            }
            break;
        //被"将军"
        case KS_CHECKED:
            CIntChessApp::PlayWaveResource(_T("IDR_CHECKED"));
            break;
        //被"将杀"
        case KS_CHECKMATED:
            {
                BYTE nWined = (m_bWhichColorHasTurn ? BLACK_WIN : WHITE_WIN);

                CString strWaveName = _T("IDR_LOSED");
                BOOL bPos = CCoordinateWnd::GetWhiteBelowOrBlackBelow();
                if ((WHITE_BELOW == bPos && WHITE_WIN == nWined) 
                    || (BLACK_BELOW == bPos && BLACK_WIN == nWined))
                {
                    strWaveName = _T("IDR_WINED");
                }
                CIntChessApp::PlayWaveResource(strWaveName);

                m_pChessDeskWnd->StopTimer(m_bWhichColorHasTurn);

                CResultDlg dlg(nWined);
                dlg.DoModal();

                OnGameNew();

                return;
            }
            break;
        //和局
        case KS_DRAW:
            {
                CIntChessApp::PlayWaveResource(_T("IDR_DRAW"));

                m_pChessDeskWnd->StopTimer(m_bWhichColorHasTurn);

                CResultDlg dlg(DRAW);
                dlg.DoModal();

                OnGameNew();

                return;
            }
            break;
        default:
            ASSERT(FALSE);
            break;
        }

        //黑白双方交换计时
        m_pChessDeskWnd->ExchangeTimerCounting();
    }
    else
    {
        const CM* pLastCMInfo = GetLastCMInfo();
        ASSERT(pLastCMInfo);

        //步数增一
        m_nStepCount++;

        //绘制棋子位置指示框
        m_pChessEquipmentWnd->DrawIndicator(act.pSrcGrid->GetRect(), act.pDestGrid->GetRect());

        //计算局面值
        DWORD dwPhaseValue = CalcPhaseValue();

        ::SendMessage(((CWnd*)m_pChessManalView)->GetSafeHwnd(), 
                      WM_WRITECHESSMANUAL, (WPARAM)pLastCMInfo, (LPARAM)dwPhaseValue);
    }

    //下面就该轮着对方走子了
    m_bWhichColorHasTurn = !m_bWhichColorHasTurn;

    //必须清空原来记录"过路兵"位置的变量，原因请参见OnMove函数的相关说明
    if (TA_MOVE != act.nAct)
    {
        m_strPassByPawnGridName = _T("");
    }

    //记录"王"或"车"的移动情况
    if (KING == chessManType || ROOK == chessManType)
    {
        CHESSMANCOLOR chessManColor = act.pSrcMan->GetColor();
        PCTG pCTG = (CHESSMAN_WHITE == chessManColor) ? &m_WhiteCTG : &m_BlackCTG;

        if (KING == chessManType)
        {
            if (!pCTG->nKing_ShiftStep)
            {
                pCTG->nKing_ShiftStep = m_nStepCount;
            }
        }
        //需排除是由于"兵的升变"而升变成"车"的情况
        else if (PAWNUP != m_nPawnUp)
        {
            CChessMan** pChessManAry = (CHESSMAN_WHITE == chessManColor) 
                ? m_pChessEquipmentWnd->GetWhiteChessManAry() 
                : m_pChessEquipmentWnd->GetBlackChessManAry();

            //索引Rook是后翼车，Rook+1是王翼车
            if (act.pSrcMan == pChessManAry[Rook])
            {
                if (!pCTG->nRookOfQueenSide_ShiftStep)
                {
                    pCTG->nRookOfQueenSide_ShiftStep = m_nStepCount;
                }
            }
            else if (act.pSrcMan == pChessManAry[Rook + 1])
            {
                if (!pCTG->nRookOfKingSide_ShiftStep)
                {
                    pCTG->nRookOfKingSide_ShiftStep = m_nStepCount;
                }
            }
        }
    }
}

/******************************************************************************************
void CChessRule::Undo()
作者      : tangjs520       创建日期: 2003-11-23
函数名    : CChessRule::Undo
返回值    : void
参数列表  : 无
描述      : 撤消最近的走棋行为
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::Undo()
{
    const CM* pLastCMInfo = GetLastCMInfo();
    ASSERT(pLastCMInfo);

    ASSERT_VALID(m_pChessEquipmentWnd);
    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    //先判断用户是否已经选中了一个棋子，如果选中了，则需要停止该棋子的闪烁
    CChessMan* pMan = CChessMan::GetActiveChessMan();
    if (pMan)
    {
        if (pMan->IsZooming())
        {
            pMan->StopZoom();
        }

        //现在应该不存在被选中的棋子了
        CChessMan::SetActiveChessMan(NULL);
    }

    //获取最近一步走子棋谱的相关信息
    CString strDestGridName = pLastCMInfo->strDestGridName;
    CString strSrcGridName = pLastCMInfo->strSrcGridName;

    CChessGrid* pDestGrid = pBoard->GetGridFromName(strDestGridName);
    CChessGrid* pSrcGrid = pBoard->GetGridFromName(strSrcGridName);
    pMan = pDestGrid->GetIncludeChessMan();

    PCTG pCTG = NULL;   //需要更新白(黑)方"王"、"王翼车"、"后翼车"的移动情况
    CChessMan** pChessManAry = NULL;
    //轮到黑方走子，则表明撤消的是白方的行为
    if (BLACK_TURN == m_bWhichColorHasTurn)
    {
        pCTG = &m_WhiteCTG;
        pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry();
    }
    //轮到白方走子，则表明撤消的是黑方的行为
    else
    {
        pCTG = &m_BlackCTG;
        pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry();
    }

    //如果撤消的行为是"王车易位"，则还需要将"车"移回原始位置
    if (TA_KINGROOKSHIFT == pLastCMInfo->nAction)
    {
        //被移位的"车"的原始位置所在棋格的名称
        CString strRookOriginalGridName = _T("");
        CChessMan* pRook = NULL;

        TCHAR ch = strDestGridName[0];
        //短易位
        if (_T('g') == ch)
        {
            strRookOriginalGridName = CString(_T('h')) + CString(strDestGridName[1]);
            pCTG->nRookOfKingSide_ShiftStep = 0;    //王翼车现在的状态是没有移动过了
            //索引Rook+1是王翼车
            pRook = pChessManAry[Rook + 1];
        }
        //长易位
        else
        {
            strRookOriginalGridName = CString(_T('a')) + CString(strDestGridName[1]);
            pCTG->nRookOfQueenSide_ShiftStep = 0;   //后翼车现在的状态是没有移动过了
            //索引Rook是后翼车
            pRook = pChessManAry[Rook];
        }

        //更新相关成员变量
        pCTG->nKing_ShiftStep = 0;  //"王"现在的状态也是没有移动过了

        //将"车"移回原始位置
        pRook->SetPosition(pBoard->GetGridFromName(strRookOriginalGridName));
    }
    //当撤消的是仅仅是"王"的行为，则还需判断此次撤消行为的步数
    //是否正好等于以前"王"从没有移动到移动时的步数值，如果相等的话，
    //则表明现在"王"已经最终归为原始位置了
    else if (KING == pLastCMInfo->chessManType)
    {
        if (pCTG->nKing_ShiftStep == m_nStepCount)
        {
            pCTG->nKing_ShiftStep = 0;
        }
    }
    //当撤消的是仅仅是"车"的行为(需要排除是由于"兵的升变"而变成"车"的情况)，
    //则还需判断此次撤消行为的步数是否正好等于以前"车"从没有移动到移动时的步数值，
    //如果相等的话，则表明现在"车"已经最终归为原始位置了
    else if (ROOK == pLastCMInfo->chessManType && PAWNUP != pLastCMInfo->nPawnUp)
    {
        //索引Rook是后翼车
        if (pMan == pChessManAry[Rook])
        {
            if (pCTG->nRookOfQueenSide_ShiftStep == m_nStepCount)
            {
                pCTG->nRookOfQueenSide_ShiftStep = 0;
            }
        }
        //索引Rook+1是王翼车
        else if (pMan == pChessManAry[Rook + 1])
        {
            if (pCTG->nRookOfKingSide_ShiftStep == m_nStepCount)
            {
                pCTG->nRookOfKingSide_ShiftStep = 0;
            }
        }
    }

    //清除当前棋子位置指示框
    m_pChessEquipmentWnd->RemoveIndicator();

    //撤消"兵的升变"行为时，还需将棋子还原成"兵"
    if (PAWNUP == pLastCMInfo->nPawnUp)
    {
        pMan->SetType(PAWN, FALSE);
    }

    //目标棋子需要移回到原来的位置
    pMan->SetPosition(pSrcGrid);

    //如果撤消的行为是"吃子"或"吃过路兵"，则还需要重新显示被该棋子吃掉的棋子
    if (TA_KILL == pLastCMInfo->nAction || TA_KILLPASSBYPAWN == pLastCMInfo->nAction)
    {
        //获取最近被该棋子吃掉的棋子对象指针
        CChessMan* pKilledMan = pMan->GetLastKilledMan();
        ASSERT(pKilledMan);

        //重新显示被该棋子吃掉的棋子
        pKilledMan->ShowChessMan(SCM_SHOW);

        if (TA_KILL == pLastCMInfo->nAction)
        {
            //如果仅仅是"吃子"行为，则目标棋格内包含的棋子就是被吃掉的棋子
            //由于在吃掉棋子时，并未修改棋子的数据成员m_pInsideChessGrid，
            //所以在此无须再设置棋子所位于的棋格对象了
            pDestGrid->SetIncludeChessMan(pKilledMan);
        }
        else
        {
            //如果是"吃过路兵"行为，由于目标棋格同被吃掉的棋子不在同一个棋格，
            //所以需要获取被吃掉棋子原来所位于的棋格，这个棋格所包含的棋子
            //在用户行使"吃过路兵"行为之前正好就是被吃掉的棋子
            CChessGrid* pGrid = pKilledMan->GetInsideChessGrid();
            ASSERT(pGrid);

            pGrid->SetIncludeChessMan(pKilledMan);
        }

        //最后需要通知该棋子删除最近被该棋子吃掉的棋子对象指针
        pMan->RemoveKilledMan(RMK_LAST);
    }

    //棋局已经被修改
    if (!((CDocument*)m_pIntChessDoc)->IsModified())
    {
        ((CDocument*)m_pIntChessDoc)->SetModifiedFlag(TRUE);
    }

    //更新相关成员变量
    m_nStepCount--;
    m_bWhichColorHasTurn = !m_bWhichColorHasTurn;
    m_CMList.RemoveTail();

    //黑白双方交换计时
    m_pChessDeskWnd->ExchangeTimerCounting();

    //恢复显示上一步的棋子位置指示框
    pLastCMInfo = GetLastCMInfo();
    if (pLastCMInfo)
    {
        strDestGridName = pLastCMInfo->strDestGridName;
        strSrcGridName = pLastCMInfo->strSrcGridName;

        pDestGrid = pBoard->GetGridFromName(strDestGridName);
        pSrcGrid = pBoard->GetGridFromName(strSrcGridName);

        m_pChessEquipmentWnd->DrawIndicator(pSrcGrid->GetRect(), pDestGrid->GetRect());

        //根据上一步的棋谱信息，可以准确的判断出在撤消此次走棋行为后，
        //现在是否存在符合"吃过路兵"条件的兵出现
        m_strPassByPawnGridName = _T("");
        if (PAWN == pLastCMInfo->chessManType)
        {
            //注意：m_bWhichColorHasTurn现在已经更新到撤消此次走棋行为后的状态了
            if ((BLACK_TURN == m_bWhichColorHasTurn) && (strSrcGridName.Find(_T('2')) != -1) 
                && (strDestGridName.Find(_T('4')) != -1))
            {
                m_strPassByPawnGridName = strDestGridName;
            }
            else if ((WHITE_TURN == m_bWhichColorHasTurn) && (strSrcGridName.Find(_T('7')) != -1) 
                     && (strDestGridName.Find(_T('5')) != -1))
            {
                m_strPassByPawnGridName = strDestGridName;
            }
        }

        //撤消此次走棋行为后，上一步的棋子状态就是现在棋子应有的状态
        //所以用上一步的nPawnUp变量值更新现在的m_nPawnUp变量值
        m_nPawnUp = pLastCMInfo->nPawnUp;
    }
    //当撤消完所有的走棋行为后，其余的成员变量都还原到它们最初值了，
    //惟有下面两个成员变量没有得到最后的更新，所以在此还要增加复位这两个成员变量的代码
    else
    {
        m_strPassByPawnGridName = _T("");
        m_nPawnUp = 0;

        //棋局已经被还原到最初状态了
        ((CDocument*)m_pIntChessDoc)->SetModifiedFlag(FALSE);
    }
}

/******************************************************************************************
void CChessRule::Save(CArchive& ar)
作者      : tangjs520       创建日期: 2003-11-25
函数名    : CChessRule::Save
返回值    : void
参数列表  : 
  参数1: CArchive& ar
描述      : 保存棋局
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::Save(CArchive& ar)
{
    WORD nCount = (WORD)m_CMList.GetCount();
    ar << nCount;

    for (POSITION pos = m_CMList.GetHeadPosition(); pos != NULL;)
    {
        CM cmInfo = m_CMList.GetNext(pos);

        ar << cmInfo.nAction;
        ar << cmInfo.nKingStatus;
        ar << cmInfo.nPawnUp;
        ar << cmInfo.nCheckCount;
        ar << (BYTE)cmInfo.chessManType;
        ar << cmInfo.strSrcGridName;
        ar << cmInfo.strDestGridName;
    }
}

/******************************************************************************************
BOOL CChessRule::Open(CArchive& ar)
作者      : tangjs520       创建日期: 2003-11-25
函数名    : CChessRule::Open
返回值    : void
参数列表  : 
  参数1: CArchive& ar
描述      : 打开棋局
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::Open(CArchive& ar)
{
    BOOL bRet = TRUE;

    //先还原到开局状态
    OnGameNew();

    ASSERT_VALID(m_pChessEquipmentWnd);
    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    WORD nCount = 0;
    ar >> nCount;

    CM cmInfo = { 0 };
    BYTE nType = 0;

    //开始读取棋局文件了，所以现在应为自动模式
    m_nUserMode = AUTOMODE;

    for (WORD i = 0; i < nCount; i++)
    {
        ar >> cmInfo.nAction;
        ar >> cmInfo.nKingStatus;
        ar >> cmInfo.nPawnUp;
        ar >> cmInfo.nCheckCount;
        ar >> nType;
        cmInfo.chessManType = (CHESSMANTYPE)nType;
        ar >> cmInfo.strSrcGridName;
        ar >> cmInfo.strDestGridName;

        //检查棋局文件的有效性
        CChessGrid* pSrcGrid = pBoard->GetGridFromName(cmInfo.strSrcGridName);
        CChessGrid* pDestGrid = pBoard->GetGridFromName(cmInfo.strDestGridName);
        if (!pSrcGrid || !pDestGrid)
        {
            bRet = FALSE;
            break;
        }

        CChessMan* pSrcMan = pSrcGrid->GetIncludeChessMan();
        if (!pSrcMan)
        {
            bRet = FALSE;
            break;
        }

        ACT act = {cmInfo.nAction, pSrcMan, pSrcGrid, pDestGrid};
        m_nPawnUp = cmInfo.nPawnUp;
        /*TranslateAction(act);
        if (act.nAct != cmInfo.nAction)
        {
            bRet = FALSE;
            break;
        }*/

        m_CMList.AddTail(cmInfo);

        DispatchAction(act);

        m_pChessEquipmentWnd->UpdateWindow();

        //由于读取棋局文件可能是一个非常耗时的任务，时间一旦比较长，
        //就有可能发生窗口来不及刷新的情况，所以为了避免出现这种情况，
        //保证应用程序的消息队列始终继续，以下代码可以做到
        //即使没有任何消息到达应用程序也能立即返回
        MSG message = { 0 };
        if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&message);
            ::DispatchMessage(&message);
        }

        Sleep(100);
    }

    //读取完棋局文件后，应将用户模式还原为默认模式，即手动模式
    m_nUserMode = MANUALMODE;

    if (!bRet)
    {
        //棋局文件已被破坏！
        AfxMessageBox(IDS_FILE_ERR);
        //按道理在此应该加上还原到开局状态的代码OnGameNew();
        //但由于此函数在返回FALSE后，导致CIntChessDoc::OnOpenDocument函数也将返回FALSE，
        //系统会由于CIntChessDoc::OnOpenDocument函数返回FALSE而自动调用
        //CIntChessDoc::OnNewDocument函数，在这个函数内已经调用OnGameNew()了
    }

    return bRet;
}

/******************************************************************************************
BOOL CChessRule::OnGameNew()
作者      : tangjs520       创建日期: 2003-11-26
函数名    : CChessRule::OnGameNew
返回值    : BOOL
参数列表  : 无
描述      : 开局
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::OnGameNew()
{
    if (!((CDocument*)m_pIntChessDoc)->CDocument::OnNewDocument())
    {
        return FALSE;
    }

    //判断用户是否已经选中了一个棋子，如果选中了，则需要停止该棋子的闪烁
    CChessMan* pMan = CChessMan::GetActiveChessMan();
    if (pMan)
    {
        if (pMan->IsZooming())
        {
            pMan->StopZoom();
        }

        //现在应该不存在被选中的棋子了
        CChessMan::SetActiveChessMan(NULL);
    }

    //如果计时器仍旧在运行，则需要复位计时器
    if (m_pChessDeskWnd->IsTimerRunning())
    {
        m_pChessDeskWnd->ResetTimer();
    }

    //如果当前还没有走动任何棋子，则立即返回
    if (!m_nStepCount)
    {
        return TRUE;
    }

    //复位走棋规则类(恢复所有的成员变量为初始值)
    m_bWhichColorHasTurn = WHITE_TURN;
    m_strPassByPawnGridName = _T("");
    memset(&m_WhiteCTG, 0, sizeof(CTG));
    memset(&m_BlackCTG, 0, sizeof(CTG));
    m_nStepCount = 0;
    m_nPawnUp = 0;
    m_nUserMode = MANUALMODE;
    m_CMList.RemoveAll();

    //清空棋谱
    ::SendMessage(((CWnd*)m_pChessManalView)->GetSafeHwnd(), WM_CLEARCHESSMANUAL, 0L, 0L);

    if (m_pChessEquipmentWnd)
    {
        m_pChessEquipmentWnd->Renew();
    }

    return TRUE;
}

/******************************************************************************************
const BOOL CChessRule::IsStarted()
作者      : tangjs520       创建日期: 2003-12-5
函数名    : CChessRule::IsStarted
返回值    : const BOOL
参数列表  : 无
描述      : 判断棋局是否已经开始了
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
const BOOL CChessRule::IsStarted()
{
    if (m_nStepCount || m_pChessDeskWnd->IsTimerRunning())
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************************
void CChessRule::ProcTimeOut(BOOL bWhichClockTimeOut)
作者      : tangjs520       创建日期: 2003-12-9
函数名    : CChessRule::ProcTimeOut
返回值    : void
参数列表  : 
  参数1: BOOL bWhichClockTimeOut
描述      : 处理棋钟超时的情况
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::ProcTimeOut(BOOL/*bWhichClockTimeOut*/)
{
}

/******************************************************************************************
void CChessRule::OnExchangeChessMan()
作者      : tangjs520       创建日期: 2003-11-26
函数名    : CChessRule::OnExchangeChessMan
返回值    : void
参数列表  : 无
描述      : 交换棋子
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::OnExchangeChessMan()
{
    if (m_pChessEquipmentWnd)
    {
        m_pChessEquipmentWnd->ExchangeChessMan();
    }

    if (m_pChessDeskWnd)
    {
        m_pChessDeskWnd->ExchangeObjects();
    }
}

/******************************************************************************************
void CChessRule::OnMove(const ACT& act)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::OnMove
返回值    : void
参数列表  : 
  参数1: const ACT& act
描述      : 处理移动棋子的行为
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::OnMove(const ACT& act)
{
    //记录最近一次形成"过路兵"条件的兵的位置
    //首先必须清空原来的记录，因为国际象棋规定，当一方出现了"吃过路兵"的机会时，
    //可以根据自己的战术需要不吃，但放弃这一机会，以后就不能再吃了。
    m_strPassByPawnGridName = _T("");
    if (PAWN == act.pSrcMan->GetType())
    {
        CHESSMANCOLOR chessManColor = act.pSrcMan->GetColor();
        CString strSrcGridName = act.pSrcGrid->GetName();
        CString strDestGridName = act.pDestGrid->GetName();

        if ((CHESSMAN_WHITE == chessManColor) && (strSrcGridName.Find(_T('2')) != -1) 
            && (strDestGridName.Find(_T('4')) != -1))
        {
            m_strPassByPawnGridName = strDestGridName;
        }
        else if ((CHESSMAN_BLACK == chessManColor) && (strSrcGridName.Find(_T('7')) != -1) 
                 && (strDestGridName.Find(_T('5')) != -1))
        {
            m_strPassByPawnGridName = strDestGridName;
        }
    }

    if (MANUALMODE == m_nUserMode)
    {
        act.pSrcMan->Moving(act.pDestGrid);
    }
    else
    {
        act.pSrcMan->SetPosition(act.pDestGrid);
    }
}

/******************************************************************************************
void CChessRule::OnKill(const ACT& act)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::OnKill
返回值    : void
参数列表  : 
  参数1: const ACT& act
描述      : 处理吃掉敌子的行为
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::OnKill(const ACT& act)
{
    CChessMan* pKilledMan = act.pDestGrid->GetIncludeChessMan();
    pKilledMan->ShowChessMan(SCM_HIDE, FALSE);
    if (MANUALMODE == m_nUserMode)
    {
        act.pSrcMan->Moving(act.pDestGrid);
    }
    else
    {
        act.pSrcMan->SetPosition(act.pDestGrid);
    }

    //设置最近被该棋子吃掉的棋子对象指针
    act.pSrcMan->SetLastKilledMan(pKilledMan);
}

/******************************************************************************************
void CChessRule::OnKillPassByPawn(const ACT& act)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::OnKillPassByPawn
返回值    : void
参数列表  : 
  参数1: const ACT& act
描述      : 处理"吃过路兵"的行为
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::OnKillPassByPawn(const ACT& act)
{
    CChessGrid* pPassByGrid = (m_pChessEquipmentWnd->GetChessBoard())->GetGridFromName(m_strPassByPawnGridName);
    CChessMan* pKilledMan = pPassByGrid->GetIncludeChessMan();

    if (MANUALMODE == m_nUserMode)
    {
        act.pSrcMan->Moving(act.pDestGrid);
    }
    else
    {
        act.pSrcMan->SetPosition(act.pDestGrid);
    }
    pKilledMan->ShowChessMan(SCM_HIDE, TRUE);
    pPassByGrid->SetIncludeChessMan(NULL);

    //设置最近被该棋子吃掉的棋子对象指针
    act.pSrcMan->SetLastKilledMan(pKilledMan);
}

/******************************************************************************************
void CChessRule::OnKingRookShift(const ACT& act)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::OnKingRookShift
返回值    : void
参数列表  : 
  参数1: const ACT& act
描述      : 处理"王车易位"的行为
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::OnKingRookShift(const ACT& act)
{
    CChessMan** pChessManAry = NULL;
    CChessMan* pRook = NULL;
    //由于在AfterDispatchAction函数中记录"王"或"车"的移动情况时，
    //如果是"王车易位"行为，则只记录"王"的移动情况，而不会记录与"王"易位的"车"的移动情况，
    //为了维护"车"的移动情况的有效性，所以在此需要记录"车"的移动情况
    PCTG pCTG = NULL;

    CHESSMANCOLOR KingColor = act.pSrcMan->GetColor();
    if (CHESSMAN_WHITE == KingColor)
    {
        pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry();
        pCTG = &m_WhiteCTG;
    }
    else
    {
        pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry();
        pCTG = &m_BlackCTG;
    }

    TCHAR ch = act.pDestGrid->GetName()[0];
    CString strRookNewGridName = _T("");
    if (_T('g') == ch)
    {
        //索引Rook+1是王翼车
        pRook = pChessManAry[Rook + 1];
        strRookNewGridName = CString(_T('f')) + CString(act.pDestGrid->GetName()[1]);
        //由于此时m_nStepCount还未增一，所以在这里要加上一
        pCTG->nRookOfKingSide_ShiftStep = (WORD)(m_nStepCount + 1);
    }
    else
    {
        //索引Rook是后翼车
        pRook = pChessManAry[Rook];
        strRookNewGridName = CString(_T('d')) + CString(act.pDestGrid->GetName()[1]);
        pCTG->nRookOfQueenSide_ShiftStep = (WORD)(m_nStepCount + 1);
    }

    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    if (MANUALMODE == m_nUserMode)
    {
        act.pSrcMan->Moving(act.pDestGrid);
    }
    else
    {
        act.pSrcMan->SetPosition(act.pDestGrid);
    }
    pRook->SetPosition(pBoard->GetGridFromName(strRookNewGridName));
}

/******************************************************************************************
void CChessRule::OnIllegal(const ACT& )
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::OnIllegal
返回值    : void
参数列表  : 
  参数1: const ACT&
描述      : 无效的行为仅需播放特定的声效即可
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::OnIllegal(const ACT&)
{
    CIntChessApp::PlayWaveResource(_T("IDR_ILLEGAL"));
}

/******************************************************************************************
void CChessRule::GetLegalGridName(const CHESSMANCOLOR chessManColor, 
                                  const CHESSMANTYPE chessManType, 
                                  const CString& strSrcGridName, 
                                  CStringList& strLstLegalGridName, 
                                  const BYTE nType)
作者      : tangjs520       创建日期: 2003-11-3
函数名    : CChessRule::GetLegalGridName
返回值    : void
参数列表  : 
  参数1: const CHESSMANCOLOR chessManColor
  参数2: const CHESSMANTYPE chessManType
  参数3: const CString& strSrcGridName
  参数4: CStringList& strLstLegalGridName
  参数5: const BYTE nType
描述      : 获取指定棋子在某一格上可以到达(或可以控制)的符合走棋规则的所有棋格的集合
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::GetLegalGridName(const CHESSMANCOLOR chessManColor, 
                                  const CHESSMANTYPE chessManType, 
                                  const CString& strSrcGridName, 
                                  CStringList& strLstLegalGridName, 
                                  const BYTE nType/* = LG_ARRIVE*/)
{
    BYTE nEndCondition = (LG_ARRIVE == nType) ? EC_EXCLUDE_SELF : EC_INCLUDE_SELF;

    switch (chessManType)
    {
    case PAWN:
        {
            int nDirect = (CHESSMAN_WHITE == chessManColor) ? ADD_DIRECT : SUBTRACT_DIRECT;
            BYTE nCondition = (LG_ARRIVE == nType) 
                ? (BYTE)(nEndCondition | EC_MUST_HAVE_CHESSMAN) 
                : nEndCondition;

            if (LG_ARRIVE == nType)
            {
                TCHAR chOrg = (CHESSMAN_WHITE == chessManColor) ? _T('2') : _T('7');
                BYTE nStep = 1;
                if (strSrcGridName.Find(chOrg) != -1)
                {
                    //兵在原始位置时，第一步棋可以走两步
                    nStep = 2;
                }

                //兵在直线方向上到达的棋格内必须没有棋子
                GetRelationGridName(chessManColor, strSrcGridName, 
                                    strLstLegalGridName, EC_MUST_EMPTY, RG_VLINE, nDirect, nStep);
            }

            //兵在斜线方向上到达的棋格内必须有对方棋子，控制的棋格内可以没有棋子
            GetRelationGridName(chessManColor, strSrcGridName, strLstLegalGridName, 
                                nCondition, RG_DLINE, nDirect, 1);
        }
        break;
    case KNIGHT:
        KnightLegal(chessManColor, strSrcGridName, strLstLegalGridName, nType);
        break;
    case ROOK:
        GetRelationGridName(chessManColor, strSrcGridName, strLstLegalGridName, nEndCondition, RG_HLINE | RG_VLINE);
        break;
    case BISHOP:
        GetRelationGridName(chessManColor, strSrcGridName, strLstLegalGridName, nEndCondition, RG_DLINE);
        break;
    case QUEEN:
        GetRelationGridName(chessManColor, strSrcGridName, strLstLegalGridName, nEndCondition);
        break;
    case KING:
        GetRelationGridName(chessManColor, strSrcGridName, strLstLegalGridName, nEndCondition, 
                            RG_HLINE | RG_VLINE | RG_DLINE, ALL_DIRECT, 1);
        break;
    default:
        ASSERT(FALSE);
        break;
    }
}

/******************************************************************************************
void CChessRule::KnightLegal(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName, 
                             CStringList& strLstLegalGridName, const BYTE nType)
作者      : tangjs520       创建日期: 2003-11-4
函数名    : CChessRule::KnightLegal
返回值    : void
参数列表  : 
  参数1: const CHESSMANCOLOR chessManColor
  参数2: const CString& strSrcGridName
  参数3: CStringList& strLstLegalGridName
  参数4: const BYTE nType
描述      : 获取合乎"马"的走法的棋格集合
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::KnightLegal(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName, 
                             CStringList& strLstLegalGridName, const BYTE nType/* = LG_ARRIVE*/)
{
    ASSERT_VALID(m_pChessEquipmentWnd);
    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    int nAlphabet = (int)strSrcGridName[0]; //表示源棋格名称的字母部分
    int nNumber = (int)strSrcGridName[1];   //表示源棋格名称的数字部分

    CString strGridName = _T("");
    CChessMan* pMan = NULL;

    //以下三个循环基于的原理是：
    //1、从源格的名称(包含字母和数字部分)推算"马步"的棋格名称(包含字母和数字部分)：
    //(1)、字母-1{数字+2、数字-2}
    //(2)、字母+1{数字+2、数字-2}
    //(3)、字母-2{数字+1、数字-1}
    //(4)、字母+2{数字+1、数字-1}
    //2、运算式i*(2n-1)[i(1,2)、n(0,1)]可得到(-1,+1,-2,+2)四个数字的八种组合
    for (int i = 1; i < 3; i++)
    {
        for (int n = 0; n < 2; n++)
        {
            int x = i * (2 * n - 1);

            for (int m = 0; m < 2; m++)
            {
                int y = (3 - i) * (2 * m - 1);

                int nAlp = nAlphabet + x;
                int nNum = nNumber + y;

                if ((nAlp >= (int)_T('a') && nAlp <= (int)_T('h')) 
                    && (nNum >= (int)_T('1') && nNum <= (int)_T('8')))
                {
                    CString strGridName = CString((TCHAR)nAlp) + CString((TCHAR)nNum);

                    pMan = pBoard->GetGridFromName(strGridName)->GetIncludeChessMan();
                    if (pMan)
                    {
                        if ((LG_ARRIVE == nType) && (pMan->GetColor() == chessManColor))
                        {
                            continue;
                        }
                    }

                    strLstLegalGridName.AddTail(strGridName);
                }
            }
        }
    }
}

/******************************************************************************************
BOOL CChessRule::IsAttacked(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName, 
                            CStringArray* pStrAryAttack)
作者      : tangjs520       创建日期: 2003-11-12
函数名    : CChessRule::IsAttacked
返回值    : BOOL
参数列表  : 
  参数1: const CHESSMANCOLOR chessManColor
  参数2: const CString& strSrcGridName
  参数3: CStringArray* pStrAryAttack(对方攻击棋子所在棋格的名称)
描述      : 判断一方棋子在某一格上是否遭受对方棋子的攻击
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::IsAttacked(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName, 
                            CStringArray* pStrAryAttack/* = NULL*/)
{
    ASSERT_VALID(m_pChessEquipmentWnd);

    CChessMan** pChessManAry = NULL;
    CStringList strLstControlGridName;

    if (CHESSMAN_WHITE == chessManColor)
    {
        pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry();
    }
    else
    {
        pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry();
    }

    BOOL bRet = FALSE;

    //轮询对方16个棋子的控制范围，根据源棋格是否在此范围内来做出判断
    for (int i = 0; i < 16; i++)
    {
        //如果棋子已经被"吃掉"了，则不用参与判断了
        if (!(pChessManAry[i]->IsVisible()))
        {
            continue;
        }

        //如果源棋格就是对方棋子所在的棋格，也不用参与判断
        CString strGridName = pChessManAry[i]->GetInsideChessGrid()->GetName();
        if (strGridName == strSrcGridName)
        {
            continue;
        }

        strLstControlGridName.RemoveAll();
        GetLegalGridName((CHESSMANCOLOR)!chessManColor, pChessManAry[i]->GetType(), 
                         strGridName, strLstControlGridName, LG_CONTROL);
        if (strLstControlGridName.Find(strSrcGridName) != NULL)
        {
            bRet = TRUE;

            if (pStrAryAttack)
            {
                pStrAryAttack->Add(strGridName);
            }
            else
            {
                return bRet;
            }
        }
    }

    return bRet;
}

/******************************************************************************************
BOOL CChessRule::IsKingChecked(const CHESSMANCOLOR KingColor)
作者      : tangjs520       创建日期: 2003-11-12
函数名    : CChessRule::IsKingChecked
返回值    : BOOL
参数列表  : 
  参数1: const CHESSMANCOLOR KingColor
描述      : 判断一方的"王"是否遭受对方棋子的攻击(也就是"将军")
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::IsKingChecked(const CHESSMANCOLOR KingColor)
{
    ASSERT_VALID(m_pChessEquipmentWnd);

    CString strKingGridName = _T("");
    CChessMan** pChessManAry = NULL;

    if (CHESSMAN_WHITE == KingColor)
    {
        pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry();
    }
    else
    {
        pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry();
    }

    strKingGridName = pChessManAry[King]->GetInsideChessGrid()->GetName();

    return IsAttacked(KingColor, strKingGridName);
}

/******************************************************************************************
BOOL CChessRule::IsPinedDown(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName)
作者      : tangjs520       创建日期: 2003-11-13
函数名    : CChessRule::IsPinedDown
返回值    : BOOL
参数列表  : 
  参数1: const CHESSMANCOLOR chessManColor
  参数2: const CString& strSrcGridName
描述      : 判断一方棋子在某一格是否遭受对方棋子的牵制
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::IsPinedDown(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName)
{
    ASSERT_VALID(m_pChessEquipmentWnd);

    //首先获取己方"王"所在的棋格名称
    CString strKingGridName = _T("");
    CChessMan** pChessManAry = NULL;

    if (CHESSMAN_WHITE == chessManColor)
    {
        pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry();
    }
    else
    {
        pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry();
    }

    strKingGridName = pChessManAry[King]->GetInsideChessGrid()->GetName();
    //如果需要判断的棋格就是己方"王"所在的棋格，则属于非法调用
    ASSERT(strKingGridName != strSrcGridName);

    CStringList strLstGridName;
    BYTE nRelation = GetBetweenGridName(strSrcGridName, strKingGridName, strLstGridName);
    //如果需要判断的棋格与己方"王"所在的棋格没有任何逻辑关系，则立即返回FALSE
    if (RG_NONE == nRelation)
    {
        return FALSE;
    }

    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    int nCount = strLstGridName.GetCount();
    //如果需要判断的棋格与己方"王"所在的棋格之间还存在其它棋格，
    //则继续判断它们之间的棋格上是否还存在其他棋子，如果存在的话，
    //则需要判断的棋格是不受对方牵制的
    if (nCount > 0)
    {
        for (POSITION pos = strLstGridName.GetHeadPosition(); pos != NULL;)
        {
            CString strName = strLstGridName.GetNext(pos);
            CChessMan* pMan = pBoard->GetGridFromName(strName)->GetIncludeChessMan();
            if (pMan)
            {
                return FALSE;
            }
        }
    }
    //如果需要判断的棋格与己方"王"所在的棋格紧挨着，
    //或是它们之间虽存在其他棋格，但这些棋格上没有任何棋子，
    //则还需要继续判断：在沿着这个逻辑方向的棋格上，是否存在能牵制该棋格的棋子
    int nSrcAlphabet = (int)strSrcGridName[0];  //表示源棋格名称的字母部分
    int nSrcNumber = (int)strSrcGridName[1];    //表示源棋格名称的数字部分

    if (RG_HLINE == nRelation)
    {
        int nSub = (strSrcGridName > strKingGridName) ? 1 : -1;

        int nEnd = (1 == nSub) ? (int)_T('h') + 1 : (int)_T('a') - 1;

        for (int i = nSrcAlphabet + nSub; i != nEnd; i += nSub)
        {
            //产生新的棋格名称
            CString strGridName = CString((TCHAR)i) + CString((TCHAR)nSrcNumber);
            CChessMan* pMan = pBoard->GetGridFromName(strGridName)->GetIncludeChessMan();
            if (pMan)
            {
                if (chessManColor != pMan->GetColor())
                {
                    CHESSMANTYPE enuType = pMan->GetType();
                    if (QUEEN == enuType || ROOK == enuType) 
                    {
                        return TRUE;
                    }
                }

                return FALSE;
            }
        }
    }
    else if (RG_VLINE == nRelation)
    {
        int nSub = (strSrcGridName > strKingGridName) ? 1 : -1;

        int nEnd = (1 == nSub) ? (int)_T('8') + 1 : (int)_T('1') - 1;

        for (int i = nSrcNumber + nSub; i != nEnd; i += nSub)
        {
            //产生新的棋格名称
            CString strGridName = CString((TCHAR)nSrcAlphabet) + CString((TCHAR)i);
            CChessMan* pMan = pBoard->GetGridFromName(strGridName)->GetIncludeChessMan();
            if (pMan)
            {
                if (chessManColor != pMan->GetColor())
                {
                    CHESSMANTYPE enuType = pMan->GetType();
                    if (QUEEN == enuType || ROOK == enuType)
                    {
                        return TRUE;
                    }
                }

                return FALSE;
            }
        }
    }
    else
    {
        int nKingAlphabet = (int)strKingGridName[0];    //表示己方"王"所在的棋格名称的字母部分
        int nKingNumber = (int)strKingGridName[1];      //表示己方"王"所在的棋格名称的数字部分

        int nSubAlp = (nSrcAlphabet > nKingAlphabet) ? 1 : -1;

        int nSubNum = (nSrcNumber > nKingNumber) ? 1 : -1;

        int nEndAlp = (1 == nSubAlp) ? (int)_T('h') + 1 : (int)_T('a') - 1;

        int nEndNum = (1 == nSubNum) ? (int)_T('8') + 1 : (int)_T('1') - 1;

        for (int i = nSrcAlphabet + nSubAlp, j = nSrcNumber + nSubNum; 
             i != nEndAlp && j != nEndNum; i += nSubAlp, j += nSubNum)
        {
            //产生新的棋格名称
            CString strGridName = CString((TCHAR)i) + CString((TCHAR)j);
            CChessMan* pMan = pBoard->GetGridFromName(strGridName)->GetIncludeChessMan();
            if (pMan)
            {
                if (chessManColor != pMan->GetColor())
                {
                    CHESSMANTYPE enuType = pMan->GetType();
                    if (QUEEN == enuType || BISHOP == enuType)
                    {
                        return TRUE;
                    }
                }

                return FALSE;
            }
        }
    }

    return FALSE;
}

/******************************************************************************************
BYTE CChessRule::GetKingStatus(const CHESSMANCOLOR KingColor, BYTE& nCheckedCount)
作者      : tangjs520       创建日期: 2003-11-12
函数名    : CChessRule::GetKingStatus
返回值    : BYTE
参数列表  : 
  参数1: const CHESSMANCOLOR KingColor
  参数1: BYTE& nCheckCount
描述      : 获取"王"的状态(KS_NORMAL、KS_CHECKED、KS_CHECKMATED, KS_DRAW)
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BYTE CChessRule::GetKingStatus(const CHESSMANCOLOR KingColor, BYTE& nCheckedCount)
{
    ASSERT_VALID(m_pChessEquipmentWnd);
    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    //首先获取己方"王"所在的棋格名称
    CChessMan** pChessManAry = NULL;
    if (CHESSMAN_WHITE == KingColor)
    {
        pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry();
    }
    else
    {
        pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry();
    }

    CChessGrid* pKingGrid = pChessManAry[King]->GetInsideChessGrid();
    CString strKingGridName = pKingGrid->GetName();

    //判断"王"是否被对方棋子攻击
    CStringArray strAryCheck;
    BOOL bChecked = IsAttacked(KingColor, strKingGridName, &strAryCheck);
    if (bChecked)
    {
        nCheckedCount = (BYTE)strAryCheck.GetSize();
        //先判断能否避将
        //获取"王"能到达的棋格
        CStringList strLstArrive;
        GetLegalGridName(KingColor, KING, strKingGridName, strLstArrive);
        for (POSITION pos = strLstArrive.GetHeadPosition(); pos != NULL;)
        {
            //如果"王"所能到达的棋格不受对方棋子的攻击，
            //且模拟"王"移动到此棋格后仍不受攻击，则表示"王"可以避将
            CString strArriveGridName = strLstArrive.GetNext(pos);
            if (!IsAttacked(KingColor, strArriveGridName))
            {
                //先暂时模拟"王"移动(或吃到)到此棋格后，棋格应有的状态
                CChessGrid* pDestGrid = pBoard->GetGridFromName(strArriveGridName);
                CChessMan* pKilledMan = pDestGrid->GetIncludeChessMan();
                if (pKilledMan)
                {
                    pKilledMan->ShowChessMan(SCM_HIDE, FALSE);
                }

                pKingGrid->SetIncludeChessMan(NULL);
                pDestGrid->SetIncludeChessMan(pChessManAry[King]);
                pChessManAry[King]->SetInsideChessGrid(pDestGrid);

                //接着判断在模拟的状态下，己方"王"是否会受对方棋子的攻击
                BOOL bKingChecked = IsKingChecked(KingColor);

                //然后还原模拟前的棋格状态
                pKingGrid->SetIncludeChessMan(pChessManAry[King]);
                pChessManAry[King]->SetInsideChessGrid(pKingGrid);

                if (pKilledMan)
                {
                    pKilledMan->ShowChessMan(SCM_SHOW, FALSE);
                    pDestGrid->SetIncludeChessMan(pKilledMan);
                }
                else
                {
                    pDestGrid->SetIncludeChessMan(NULL);
                }

                //如果己方"王"不受到对方棋子攻击，则表示"王"可以避将
                if (!bKingChecked)
                {
                    return KS_CHECKED;
                }
            }
        }
        //如果对方"将军"的棋子不止一个，而"王"又没法避将，则"王"被"将杀"
        if (nCheckedCount >= 2)
        {
            return KS_CHECKMATED;
        }

        //再判断能否消将
        //判断己方的棋子能否控制对方"将军"棋子所在的棋格
        CStringArray strAryControl;
        BOOL bControl = IsAttacked((CHESSMANCOLOR)!KingColor, strAryCheck[0], &strAryControl);
        if (bControl)
        {
            int nControlCount = strAryControl.GetSize();
            for (int i = 0; i < nControlCount; i++)
            {
                //如果己方"王"能控制该棋格，则不用参与此次判断，
                //因为在上述的避将中，已经对此情况做出判断了
                //同时己方"王"不得参与是否受牵制的判断
                if (KING == pBoard->GetGridFromName(strAryControl[i])->GetIncludeChessMan()->GetType())
                {
                    //如果能吃掉对方"将军"棋子的只有己方"王"了，也就不再需要判断能否垫将了，
                    //可以肯定己方"王"被"将杀"了
                    if (1 == nControlCount)
                    {
                        return KS_CHECKMATED;
                    }

                    continue;
                }

                //判断该棋子是否受牵制，如果受牵制则表示该棋子无法吃掉对方"将军"的棋子
                if (!IsPinedDown(KingColor, strAryControl[i]))
                {
                    return KS_CHECKED;
                }
            }
        }

        //最后判断能否垫将
        //如果"将军"的棋子是"马"或"兵"，则不可能垫将
        CHESSMANTYPE checkType = pBoard->GetGridFromName(strAryCheck[0])->GetIncludeChessMan()->GetType();
        if (KNIGHT == checkType || PAWN == checkType)
        {
            return KS_CHECKMATED;
        }
        else
        {
            //获取己方"王"所在棋格和对方"将军"棋子所在棋格之间的棋格名称
            CStringList strLstBetweenGridName;
            GetBetweenGridName(strKingGridName, strAryCheck[0], strLstBetweenGridName);
            for (POSITION pos = strLstBetweenGridName.GetHeadPosition(); pos != NULL;)
            {
                CString strGridName = strLstBetweenGridName.GetNext(pos);
                //轮询己方16个棋子的到达范围，根据刚才所获得的棋格是否在此范围内来做出判断
                for (int i = 0; i < 16; i++)
                {
                    //如果棋子已经被"吃掉"了，则不用参与判断了
                    if (!(pChessManAry[i]->IsVisible()))
                    {
                        continue;
                    }
                    //如果是己方王，则不能拿它来做判断
                    if (pChessManAry[King] == pChessManAry[i])
                    {
                        continue;
                    }

                    strLstArrive.RemoveAll();
                    CString strName = pChessManAry[i]->GetInsideChessGrid()->GetName();
                    GetLegalGridName(KingColor, pChessManAry[i]->GetType(), strName, strLstArrive);
                    if (strLstArrive.Find(strGridName) != NULL)
                    {
                        //如果该棋子正被对方牵制着，则表示该棋子无法"垫将"
                        if (IsPinedDown(KingColor, strName))
                        {
                            continue;
                        }

                        return KS_CHECKED;
                    }
                }
            }

            return KS_CHECKMATED;
        }
    }
    else
    {
        //如果"王"没有被"将军"，则判断是否形成和局
        if (IsDraw())
        {
            return KS_DRAW;
        }
    }

    return KS_NORMAL;
}

/******************************************************************************************
BOOL CChessRule::IsDraw()
作者      : tangjs520       创建日期: 2003-11-16
函数名    : CChessRule::IsDraw
返回值    : BOOL
参数列表  : 无
描述      : 判断是否形成和局
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::IsDraw()
{
    ASSERT_VALID(m_pChessEquipmentWnd);

    //首先判断是否形成"逼和"
    if (IsForceDraw())
    {
        return TRUE;
    }
    //接着判断是否形成局面和局[即"官和"]
    if (IsSituationDraw())
    {
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************************
BOOL CChessRule::IsForceDraw()
作者      : tangjs520       创建日期: 2003-11-16
函数名    : CChessRule::IsForceDraw
返回值    : BOOL
参数列表  : 无
描述      : 判断是否形成"逼和"
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::IsForceDraw()
{
    CChessMan** pChessManAry;
    CHESSMANCOLOR chessManColor;
    m_bWhichColorHasTurn ? (pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry(), 
        chessManColor = CHESSMAN_WHITE) 
        : (pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry(), 
        chessManColor = CHESSMAN_BLACK);

    //判断对方棋子是否还能移动，如果均不能移动，则形成"逼和"
    CStringList strLstArriveGridName;
    for (int i = 0; i < 16; i++)
    {
        //如果棋子已经被"吃掉"了，则不用参与判断
        if (!(pChessManAry[i]->IsVisible()))
        {
            continue;
        }

        strLstArriveGridName.RemoveAll();
        CHESSMANTYPE chessManType = pChessManAry[i]->GetType();
        CString strGridName = pChessManAry[i]->GetInsideChessGrid()->GetName();
        GetLegalGridName(chessManColor, chessManType, strGridName, strLstArriveGridName);

        //如果棋子能移动，再看它是否受对方牵制，如果不受对方牵制
        //则可以肯定现在还未形成"逼和"局面
        if (!strLstArriveGridName.IsEmpty())
        {
            //如果是"王"可以移动，则还要继续判断"王"所能到达的棋格是否受对方棋子攻击
            //另外，"王"是不得参与是否受牵制的判断
            if (KING == chessManType)
            {
                for (POSITION pos = strLstArriveGridName.GetHeadPosition(); pos != NULL;)
                {
                    //如果"王"所能到达的棋格不受对方棋子的攻击，
                    //则表示"王"可以走动
                    if (!IsAttacked(chessManColor, strLstArriveGridName.GetNext(pos)))
                    {
                        return FALSE;
                    }
                }

                continue;
            }

            if (!IsPinedDown(chessManColor, strGridName))
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/******************************************************************************************
BOOL CChessRule::IsSituationDraw()
作者      : tangjs520       创建日期: 2003-11-16
函数名    : CChessRule::IsSituationDraw
返回值    : BOOL
参数列表  : 无
描述      : 判断是否形成局面和局[即"官和"]
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::IsSituationDraw()
{
    //以下几种情况均为"官和"：
    //1、双方只剩王
    //2、一方单王，另一方单象或单马
    //3、双方剩同色格单象
    CChessMan** pChessManAry = NULL;
    BYTE nWhiteBishopCount = 0, nWhiteKnightCount = 0;
    BYTE nBlackBishopCount = 0, nBlackKnightCount = 0;
    BYTE *pBishopCount = NULL, *pKnightCount = NULL;
    CHESSGRIDCOLOR WhiteBishop_GridColor = CHESSGRID_WHITE, 
    BlackBishop_GridColor = CHESSGRID_WHITE;
    CHESSGRIDCOLOR *pGridColor = NULL;

    for (int n = 0; n < 2; n++)
    {
        if (0 == n)
        {
            pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry();
            pBishopCount = &nWhiteBishopCount;
            pKnightCount = &nWhiteKnightCount;
            pGridColor = &WhiteBishop_GridColor;
        }
        else
        {
            pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry();
            pBishopCount = &nBlackBishopCount;
            pKnightCount = &nBlackKnightCount;
            pGridColor = &BlackBishop_GridColor;
        }

        for (int i = 0; i < 16; i++)
        {
            //如果棋子已经被"吃掉"了，则不用参与判断
            if (!(pChessManAry[i]->IsVisible()))
            {
                continue;
            }

            CHESSMANTYPE chessManType = pChessManAry[i]->GetType();
            //如果一方还存在除王、象、马之外的棋子，则不可能"官和"
            if (KING != chessManType && BISHOP != chessManType && KNIGHT != chessManType)
            {
                return FALSE;
            }

            switch (chessManType)
            {
            case BISHOP:
                {
                    (*pBishopCount)++;
                    //如果一方还存在双象或是马象，则不可能"官和"
                    if (2 == (*pBishopCount) || 1 == (*pKnightCount))
                    {
                        return FALSE;
                    }

                    (*pGridColor) = pChessManAry[i]->GetInsideChessGrid()->GetColor();
                }
                break;
            case KNIGHT:
                {
                    (*pKnightCount)++;
                    //如果一方还存在双马或是马象，则不可能"官和"
                    if (2 == (*pKnightCount) || 1 == (*pBishopCount))
                    {
                        return FALSE;
                    }
                }
                break;
            default:
                break;
            }
        }
    }

    //一方剩象(或马)，另一方剩马(或象)，也不可能"官和"
    if ((1 == nWhiteBishopCount && 1 == nBlackKnightCount) 
        || (1 == nWhiteKnightCount && 1 == nBlackBishopCount))
    {
        return FALSE;
    }
    //如果双方各剩一个马，也不可能"官和"
    if (1 == nWhiteKnightCount && 1 == nBlackKnightCount)
    {
        return FALSE;
    }
    //如果双方各剩一个象且为异色格，也不可能"官和"
    if ((1 == nWhiteBishopCount && 1 == nBlackBishopCount) 
        && (WhiteBishop_GridColor != BlackBishop_GridColor))
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************************
BOOL CChessRule::IsKillPassByPawn(const CHESSMANCOLOR PawnColor, const CString& strSrcGridName, 
                                  const CString& strDestGridName)
作者      : tangjs520       创建日期: 2003-11-3
函数名    : CChessRule::IsKillPassByPawn
返回值    : BOOL
参数列表  : 
  参数1: const CHESSMANCOLOR PawnColor
  参数2: const CString& strSrcGridName
  参数3: const CString& strDestGridName
描述      : 判断是否形成"吃过路兵"
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::IsKillPassByPawn(const CHESSMANCOLOR PawnColor, const CString& strSrcGridName, 
                                  const CString& strDestGridName)
{
    int nDirect = 0;
    //当白兵在第5横线、黑兵在第4横线时才有可能走出"吃过路兵"的步法
    if ((CHESSMAN_WHITE == PawnColor) && (strSrcGridName.Find(_T("5")) != -1))
    {
        nDirect = ADD_DIRECT;
    }
    else if ((CHESSMAN_BLACK == PawnColor) && (strSrcGridName.Find(_T("4")) != -1))
    {
        nDirect = SUBTRACT_DIRECT;
    }
    else
    {
        return FALSE;
    }

    CStringList strLstGridName;

    //由于"吃过路兵"的吃法仍同斜吃，所以首先得判断目标棋格是否是源棋格
    //在前进方向上的斜一格
    GetRelationGridName(PawnColor, strSrcGridName, strLstGridName, 
                        EC_MUST_EMPTY, RG_DLINE, nDirect, 1);

    if (strLstGridName.Find(strDestGridName) != NULL)
    {
        //如果是"吃过路兵"的话，则目标棋格在直线方向上后退一格恰好是
        //成员变量m_strPassByPawnGridName记录的棋格
        strLstGridName.RemoveAll();
        GetRelationGridName(PawnColor, strDestGridName, strLstGridName, 
                            EC_EXCLUDE_SELF | EC_MUST_HAVE_CHESSMAN, RG_VLINE, (-nDirect), 1);

        if (strLstGridName.Find(m_strPassByPawnGridName) != NULL)
        {
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************************
BOOL CChessRule::IsKingRookShift(const CHESSMANCOLOR KingColor, const CString& strDestGridName)
作者      : tangjs520       创建日期: 2003-11-7
函数名    : CChessRule::IsKingRookShift
返回值    : BOOL
参数列表  : 
  参数1: const CHESSMANCOLOR KingColor
  参数2: const CString& strDestGridName
描述      : 判断是否形成"王车易位"
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CChessRule::IsKingRookShift(const CHESSMANCOLOR KingColor, const CString& strDestGridName)
{
    PCTG pCTG = NULL;
    TCHAR chHorz;
    if (CHESSMAN_WHITE == KingColor)
    {
        pCTG = &m_WhiteCTG;
        chHorz = _T('1');
    }
    else
    {
        pCTG = &m_BlackCTG;
        chHorz = _T('8');
    }

    //施行"王车易位"的规定条件为：
    //1、王和车必须尚在原始位置不曾移动
    //2、王与车之间已无其他子
    //3、王从出发到完成易位的3个格子均未受攻击
    if (!pCTG->nKing_ShiftStep)
    {
        ASSERT_VALID(m_pChessEquipmentWnd);
        CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
        ASSERT(pBoard);

        //短易位
        if ((strDestGridName == (CString(_T('g')) + CString(chHorz))) 
            && (!pCTG->nRookOfKingSide_ShiftStep))
        {
            CChessMan* pMan = pBoard->GetGridFromName(CString(_T('f')) + CString(chHorz))->GetIncludeChessMan();
            if (!pMan)
            {
                for (TCHAR ch = _T('e'); ch <= _T('g'); ch++)
                {
                    if (IsAttacked(KingColor, CString(ch) + CString(chHorz)))
                    {
                        return FALSE;
                    }
                }

                return TRUE;
            }
        }
        //长易位
        else if ((strDestGridName == (CString(_T('c')) + CString(chHorz))) 
                 && (!pCTG->nRookOfQueenSide_ShiftStep))
        {
            CChessMan* pMan1 = pBoard->GetGridFromName(CString(_T('d')) + CString(chHorz))->GetIncludeChessMan();
            CChessMan* pMan2 = pBoard->GetGridFromName(CString(_T('b')) + CString(chHorz))->GetIncludeChessMan();
            if (!pMan1 && !pMan2)
            {
                for (TCHAR ch = _T('e'); ch >= _T('c'); ch--)
                {
                    if (IsAttacked(KingColor, CString(ch) + CString(chHorz)))
                    {
                        return FALSE;
                    }
                }

                return TRUE;
            }
        }
    }

    return FALSE;
}

/******************************************************************************************
void CChessRule::GetRelationGridName(const CHESSMANCOLOR chessManColor, 
                                     const CString& strSrcGridName, const BYTE nEndCondition, 
                                     CStringList& strLstGridName, BYTE nRGType, 
                                     const int nDirection, const BYTE nStep)
作者      : tangjs520       创建日期: 2003-10-27
函数名    : CChessRule::GetRelationGridName
返回值    : void
参数列表  : 
  参数1: const CHESSMANCOLOR chessManColor(棋子的颜色)
  参数2: const CString& strSrcGridName(输入参数，表示源棋格名称)
  参数3: CStringList& strLstGridName(输出参数，符合条件的所有棋格名称都存入该集合中)
  参数4: const BYTE nEndCondition(停止搜索的条件)
  参数5: BYTE nRGType(输入参数，表示逻辑关系，可以是RG_HLINE、RG_VLINE和RG_DLINE的任意组合)
  参数6: const int nDirection(输入参数，表示方向，可以是递增、递减或所有方向)
  参数7: BYTE nStep(输入参数，表示在相应方向的搜索步长[亦即棋格数量])
描述      : 获取与指定棋格有相关逻辑关系的所有棋格的集合
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessRule::GetRelationGridName(const CHESSMANCOLOR chessManColor, 
                                     const CString& strSrcGridName, 
                                     CStringList& strLstGridName, 
                                     const BYTE nEndCondition/* = EC_EXCLUDE_SELF*/, 
                                     BYTE nRGType/* = RG_HLINE | RG_VLINE | RG_DLINE*/, 
                                     const int nDirection/* = ALL_DIRECT*/, 
                                     const BYTE nStep/* = NOLIMIT*/)
{
    ASSERT_VALID(m_pChessEquipmentWnd);
    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    //如果是所有方向，则分解成递增和递减两个方向进行递归调用
    if (ALL_DIRECT == nDirection)
    {
        GetRelationGridName(chessManColor, strSrcGridName, strLstGridName, 
                            nEndCondition, nRGType, ADD_DIRECT, nStep);
        GetRelationGridName(chessManColor, strSrcGridName, strLstGridName, 
                            nEndCondition, nRGType, SUBTRACT_DIRECT, nStep);

        return;
    }

    int nAlphabet = 0;  //表示源棋格名称的字母部分
    int nNumber = 0;    //表示源棋格名称的数字部分

    //这是个无用的变量，定义它是为了在后面的do-while循环中，
    //当逻辑关系不是RG_DLINE时，仅对nAlphabet(RG_HLINE)或nNumber(RG_VLINE)操作
    int nUseless = 0;

    //pAlphabet和pNumber用以指向nAlphabet、nNumber或nUseless
    //pRef指向当比较步长时的参照变量，它可以是nAlphabet或是nNumber
    int *pAlphabet = NULL, *pNumber = NULL, *pRef = NULL;

    //表示字母部分和数字部分是递增(nDirection = +1)还是递减(nDirection = -1)
    int nAlphabetBase = nDirection;
    int nNumberBase = nDirection;

    //for循环的次数，当逻辑关系是RG_DLINE时，for循环必须循环两次
    //因为唯有斜线逻辑关系在递增或递减方向上存在两条线
    //其他逻辑关系时，该变量的值为1
    int nLoopCount = 0;

    //用于在进行步长比较时的被比较值，我称呼它为结束"哨兵"
    //当nStep等于NOLIMIT(=0)时，该变量无任何实际意义
    //因为后续的表达式"nEndSentinel = nAlphabet(nNumber) + nDirection * nStep;"使得
    //"while ((*pRef) != nEndSentinel);"永远成立
    int nEndSentinel = 0;

    //当所有的逻辑关系都处理完(nRGType = 0)，结束整个循环，完成函数的使命
    while (0 != nRGType)
    {
        //每次循环前必须重新设置nAlphabet和nNumber
        //因为当进行完一次逻辑关系搜索后，nAlphabet和(或)nNumber必然发生变化
        nAlphabet = (int)strSrcGridName[0];
        nNumber = (int)strSrcGridName[1];
        //对nAlphabetBase进行重新设置，是考虑到当逻辑关系是RG_DLINE时，
        //nAlphabetBase也必然发生变化
        nAlphabetBase = nDirection;

        //当是横线逻辑关系时
        if (nRGType & RG_HLINE)
        {
            //仅仅需要对字母部分做递增或递减操作
            pAlphabet = &nAlphabet;
            pNumber = &nUseless;

            nLoopCount = 1;

            nEndSentinel = nAlphabet + nDirection * nStep;
            pRef = &nAlphabet;

            //去掉即将处理的逻辑关系，避免下一次循环再做同样的处理
            //否则会形成"死"循环
            nRGType &= ~RG_HLINE;
        }
        //当是直线逻辑关系时
        else if (nRGType & RG_VLINE)
        {
            //仅仅需要对数字部分做递增或递减操作
            pNumber = &nNumber;
            pAlphabet = &nUseless;

            nLoopCount = 1;

            nEndSentinel = nNumber + nDirection * nStep;
            pRef = &nNumber;

            nRGType &= ~RG_VLINE;
        }
        //当是斜线逻辑关系时
        else if (nRGType & RG_DLINE)
        {
            //需要同时对字母部分和数字部分做递增或递减操作
            pAlphabet = &nAlphabet;
            pNumber = &nNumber;

            nLoopCount = 2;

            nEndSentinel = nNumber + nDirection * nStep;
            pRef = &nNumber;

            nRGType &= ~RG_DLINE;
        }

        for (int i = 0; i < nLoopCount; i++)
        {
            do
            {
                //对字母部分和(或)数字部分做递增或递减操作
                (*pAlphabet) += nAlphabetBase;
                (*pNumber) += nNumberBase;

                //产生新的棋格名称
                CString strGridName = CString((TCHAR)nAlphabet) + CString((TCHAR)nNumber);

                //判断新的棋格名称是否在棋盘上存在
                //如果不存在，就立即结束在这个方向的搜索
                //如果存在，再依据参数nEndCondition来判断
                //是否应该将该棋格名称存入输出参数中
                CChessGrid* pGrid = NULL;
                pGrid = pBoard->GetGridFromName(strGridName);
                if (pGrid)
                {
                    CChessMan* pMan = NULL;
                    pMan = pGrid->GetIncludeChessMan();

                    if (pMan)
                    {
                        //如果停止搜索的条件是"棋格内必须没有棋子"
                        //或是"不包含己方的棋子"，但是棋格内的棋子
                        //恰好是己方的，则不记录此棋格并且马上结束
                        //在这个方向上的搜索
                        if ((nEndCondition & EC_MUST_EMPTY) 
                            || ((nEndCondition & EC_EXCLUDE_SELF) 
                            && (pMan->GetColor() == chessManColor)))
                        {
                            break;
                        }

                        strLstGridName.AddTail(strGridName);

                        break;
                    }
                    else if (nEndCondition & EC_MUST_HAVE_CHESSMAN)
                    {
                        break;
                    }

                    strLstGridName.AddTail(strGridName);
                }
                else
                {
                    break;
                }
            } while ((*pRef) != nEndSentinel);  //判断是否已经到达指定步长

            //当逻辑关系是RG_DLINE时(必须循环两次)，必须还原设置nAlphabet和nNumber，
            //因为现在nAlphabet和nNumber都已经发生变化了，
            //另外还需将原来字母部分的变化方向置反
            if (2 == nLoopCount)
            {
                nAlphabet = (int)strSrcGridName[0];
                nNumber = (int)strSrcGridName[1];
                nAlphabetBase = -nAlphabetBase;
            }
        }
    }
}

/******************************************************************************************
BYTE CChessRule::GetBetweenGridName(const CString& strSrcGridName, 
                                    const CString& strDestGridName, 
                                    CStringList& strLstGridName)
作者      : tangjs520       创建日期: 2003-11-2
函数名    : CChessRule::GetBetweenGridName
返回值    : BYTE(返回源棋格同目标棋格的逻辑关系)
参数列表  : 
  参数1: const CString& strSrcGridName
  参数2: const CString& strDestGridName
  参数3: CStringList& strLstGridName
描述      : 获取在源棋格和目标棋格之间(仅指横线、直线或斜线三个方向中的一个)的所有棋格的集合
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BYTE CChessRule::GetBetweenGridName(const CString& strSrcGridName, 
                                    const CString& strDestGridName, 
                                    CStringList& strLstGridName)
{
    //必须保证源棋格和目标棋格不是同一格
    ASSERT(strSrcGridName != strDestGridName);

    int nSrcAlphabet = (int)strSrcGridName[0];      //表示源棋格名称的字母部分
    int nSrcNumber = (int)strSrcGridName[1];        //表示源棋格名称的数字部分

    int nDestAlphabet = (int)strDestGridName[0];    //表示目标棋格名称的字母部分
    int nDestNumber = (int)strDestGridName[1];      //表示目标棋格名称的数字部分

    //直线方向
    if (nSrcAlphabet == nDestAlphabet)
    {
        if (1 != abs(nSrcNumber - nDestNumber))
        {
            for (int i = min(nSrcNumber, nDestNumber) + 1; 
                 i < max(nSrcNumber, nDestNumber); i++)
            {
                //产生新的棋格名称
                CString strGridName = CString((TCHAR)nSrcAlphabet) + CString((TCHAR)i);
                strLstGridName.AddTail(strGridName);
            }
        }

        return RG_VLINE;
    }
    //横线方向
    else if (nSrcNumber == nDestNumber)
    {
        if (1 != abs(nSrcAlphabet - nDestAlphabet))
        {
            for (int i = min(nSrcAlphabet, nDestAlphabet) + 1; 
                 i < max(nSrcAlphabet, nDestAlphabet); i++)
            {
                //产生新的棋格名称
                CString strGridName = CString((TCHAR)i) + CString((TCHAR)nSrcNumber);
                strLstGridName.AddTail(strGridName);
            }
        }

        return RG_HLINE;
    }
    //斜线方向
    else
    {
        int nSubAlphbet = nSrcAlphabet - nDestAlphabet;
        int nSubNumber = nSrcNumber - nDestNumber;

        int nAbsSubAlp = abs(nSubAlphbet);

        if (nAbsSubAlp == abs(nSubNumber))
        {
            if (1 != nAbsSubAlp)
            {
                int nSub = 0, nInitNum = 0;
                if (nSubAlphbet == nSubNumber)
                {
                    nSub = 1;
                    nInitNum = min(nSrcNumber, nDestNumber) + 1;
                }
                else
                {
                    nSub = -1;
                    nInitNum = max(nSrcNumber, nDestNumber) - 1;
                }

                int nEndAlp = max(nSrcAlphabet, nDestAlphabet);

                for (int i = min(nSrcAlphabet, nDestAlphabet) + 1, 
                     j = nInitNum; i < nEndAlp; i++, j += nSub)
                {
                    //产生新的棋格名称
                    CString strGridName = CString((TCHAR)i) + CString((TCHAR)j);
                    strLstGridName.AddTail(strGridName);
                }
            }

            return RG_DLINE;
        }
    }

    return RG_NONE;
}

/******************************************************************************************
DWORD CChessRule::CalcPhaseValue()
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CChessRule::CalcPhaseValue
返回值    : DWORD
参数列表  : 无
描述      : 计算局面值
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
DWORD CChessRule::CalcPhaseValue()
{
    ASSERT_VALID(m_pChessEquipmentWnd);

    DWORD dwRet = 0L;

    CChessMan** pChessManAry = NULL;
    for (int n = 0; n < 2; n++)
    {
        if (0 == n)
        {
            pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry();
        }
        else
        {
            pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry();
        }

        for (int i = 0; i < 16; i++)
        {
            if (!(pChessManAry[i]->IsVisible()))
            {
                continue;
            }

            CString strGridName = pChessManAry[i]->GetInsideChessGrid()->GetName();
            //棋格值 = 棋格名称字母部分的ASCII码的平方 + 棋格名称数字部分的ASCII码的平方
            DWORD dwGridValue = (DWORD)(strGridName[0]) * (DWORD)(strGridName[0]) 
                + (DWORD)(strGridName[1]) * (DWORD)(strGridName[1]);

            //double _scalb(double x, long exp);
            //The _scalb function calculates the value of x * 2的exp次幂.
            //计算局面值
            dwRet += (DWORD)_scalb(dwGridValue, (DWORD)pChessManAry[i]->GetType() + 1);
        }
    }

    return dwRet;
}
