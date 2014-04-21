/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessRule.cpp
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
#include "ChessRule.h"

#include "ChessEquipmentWnd.h"  //��ߴ��ڶ�����(CChessEquipmentWnd)��ͷ�ļ�
#include "ChessDeskWnd.h"       //�������ڶ�����(CChessEquipmentWnd)��ͷ�ļ�

#include "PawnUpDlg.h"          //"��������"�Ի�����(CPawnUpDlg)��ͷ�ļ�
#include "ResultDlg.h"          //"�������"�Ի�����(CResultDlg)��ͷ�ļ�

#include <float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//�����ɰ׷�����
BOOL CChessRule::m_bWhichColorHasTurn = WHITE_TURN;

CString CChessRule::m_strPassByPawnGridName = _T("");

CTG CChessRule::m_WhiteCTG;
CTG CChessRule::m_BlackCTG;

WORD CChessRule::m_nStepCount = 0;

//Ĭ�����ֶ�ģʽ
BYTE CChessRule::m_nUserMode = MANUALMODE;

CList<CM, CM> CChessRule::m_CMList;

CChessEquipmentWnd* CChessRule::m_pChessEquipmentWnd = NULL;
CChessManualView* CChessRule::m_pChessManalView = NULL;
CIntChessDoc* CChessRule::m_pIntChessDoc = NULL;
CChessDeskWnd* CChessRule::m_pChessDeskWnd = NULL;

BYTE CChessRule::m_nPawnUp = 0;

//�����Ϊ�ַ�����ӳ���
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
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::CChessRule
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CChessRule::CChessRule()
{
}

/******************************************************************************************
CChessRule::~CChessRule()
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::~CChessRule
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CChessRule::~CChessRule()
{
}

/******************************************************************************************
void CChessRule::DoSomething(CChessGrid* const pDestGrid)
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::DoSomething
����ֵ    : void
�����б�  : 
  ����1: CChessGrid* const pDestGrid
����      : ����������򣬶��û��������̵���Ϊ������Ӧ�Ĵ���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessRule::DoSomething(CChessGrid* const pDestGrid)
{
    ASSERT(pDestGrid);

    CChessMan* pMan = pDestGrid->GetIncludeChessMan();
    //���ѡ�������ӣ�ͬʱ�����ֵ�����ɫ���������壬�����ӽ������Ų�����
    //��ʾ�������ǵ�ǰ��ѡ�е����ӣ��������Ч��Ϊ������Ը����Ӳ�����
    if (pMan && m_bWhichColorHasTurn == (BOOL)pMan->GetColor())
    {
        //�����ֻ���δ��ʼ��ʱ����������ʱ����ʼ���м�ʱ����
        if (FALSE == m_pChessDeskWnd->IsTimerRunning())
        {
            m_pChessDeskWnd->StartTimer(m_bWhichColorHasTurn);
        }

        pMan->StartZoom();
    }
    //���ѡ���˿յ�������ѡ���˶Է������ӣ�����н����ͷַ��û���Ϊ�Ĳ���
    else if (CChessMan* pSrcMan = CChessMan::GetActiveChessMan())
    {
        ACT act = {0, pSrcMan, pSrcMan->GetInsideChessGrid(), pDestGrid};
        TranslateAction(act);
        DispatchAction(act);
    }
}

/******************************************************************************************
void CChessRule::TranslateAction(ACT& act)
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::TranslateAction
����ֵ    : void
�����б�  : 
  ����1: ACT& act
����      : �����û�����Ϊ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessRule::TranslateAction(ACT& act)
{
    //�Ƚ���Ԥ���������Ԥ�����Ѿ��������û�����Ϊ����ֱ�ӷ���
    if (PreTranslateAction(act))
    {
        return;
    }

    //��ȡ��ѡ�����ӿ��Ե�������ļ���
    CStringList strLstLegalGridName;
    GetLegalGridName(act.pSrcMan->GetColor(), act.pSrcMan->GetType(), 
                     act.pSrcGrid->GetName(), strLstLegalGridName);

    //���Ŀ���������������ڣ�������ж����ƶ����ӵ���Ϊ���ǳԵ����ӵ���Ϊ
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
    //�������Ϊ����Ч��
    else
    {
        act.nAct = TA_ILLEGAL;
    }
}

/******************************************************************************************
void CChessRule::DispatchAction(ACT& act)
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::DispatchAction
����ֵ    : void
�����б�  : 
  ����1: ACT& act
����      : �ַ��û�����Ϊ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessRule::DispatchAction(ACT& act)
{
    //�ڷַ���Ϊ֮ǰ������صĴ���
    PreDispatchAction(act);

    //���ݷַ�����ӳ�������ݣ��ַ��û�����Ϊ����Ӧ�ĺ�������
    for (int i = 0; i < DIM(_actionEntries); i++)
    {
        if (act.nAct == _actionEntries[i].nAct)
        {
            (*_actionEntries[i].pfn)(act);

            break;
        }
    }

    //�ڴ������û�����Ϊ������صĴ���
    AfterDispatchAction(act);
}

/******************************************************************************************
BOOL CChessRule::PreTranslateAction(ACT& act)
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::PreTranslateAction
����ֵ    : BOOL
�����б�  : 
  ����1: ACT& act
����      : �ж��Ƿ��γ�"������λ"��"�Թ�·��"
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CChessRule::PreTranslateAction(ACT& act)
{
    CHESSMANTYPE chessManType = act.pSrcMan->GetType();
    if (KING == chessManType || PAWN == chessManType)
    {
        CHESSMANCOLOR chessManColor = act.pSrcMan->GetColor();
        CString strDestGridName = act.pDestGrid->GetName();

        //�ж��Ƿ��γ�"������λ"
        if (KING == chessManType)
        {
            PCTG pCTG = (CHESSMAN_WHITE == chessManColor) ? &m_WhiteCTG : &m_BlackCTG;
            //�����û���ƶ����������������û���ƶ������Ž����ж�
            //���⣬��ʹ��û���ƶ�����������������ͺ������Ѿ��ƶ��ˣ�Ҳ��û�б�Ҫ�����ж���
            if (!pCTG->nKing_ShiftStep 
                && (!pCTG->nRookOfKingSide_ShiftStep || !pCTG->nRookOfQueenSide_ShiftStep) 
                && IsKingRookShift(chessManColor, strDestGridName))
            {
                act.nAct = TA_KINGROOKSHIFT;
                return TRUE;
            }
        }
        //�ж��Ƿ��γ�"�Թ�·��"
        else
        {
            //�������"��·��"��������ж�
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
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::PreDispatchAction
����ֵ    : void
�����б�  : 
  ����1: ACT& act
����      : 1�������û�����Ϊ�Ϸ����жϼ���"��"�Ƿ���ܶԷ����ӵĹ���
            2���ж��Ƿ��γ�"��������"
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

        //���ȱ����жϣ�������������Ϊ�󣬼���"��"�Ƿ���ܶԷ����ӵĹ���
        //����ǣ���˴���Ϊ���ڷǷ�����
        //����ʱģ�����Ӵ�������Ӧ����Ϊ�����Ӧ�е�״̬
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

        //�����ж���ģ���״̬�£�����"��"�Ƿ���ܶԷ����ӵĹ���
        BOOL bKingChecked = IsKingChecked(chessManColor);

        //Ȼ��ԭģ��ǰ�����״̬
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

        //�������"��"�ܵ��Է����ӹ�������˴���Ϊ���ڷǷ�����
        if (bKingChecked)
        {
            act.nAct = TA_ILLEGAL;
            return;
        }

        CHESSMANTYPE chessManType = act.pSrcMan->GetType();
        CString strSrcGridName = act.pSrcGrid->GetName();

        //����ձ�ʶ"��������"�ı���
        m_nPawnUp = 0;

        //�ж��Ƿ��γ�"��������"
        if (PAWN == chessManType && TA_KILLPASSBYPAWN != act.nAct)
        {
            if ((CHESSMAN_WHITE == chessManColor && strSrcGridName.Find(_T('7')) != -1) 
                || (CHESSMAN_BLACK == chessManColor && strSrcGridName.Find(_T('2')) != -1))
            {
                CPawnUpDlg dlg(chessManColor);
                dlg.DoModal();
                act.pSrcMan->SetType(dlg.m_enuUpType, FALSE);

                //���¸ñ�������ʶ��ǰ��"��������"
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
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::AfterDispatchAction
����ֵ    : void
�����б�  : 
  ����1: const ACT& act
����      : ���ݲ�ͬ����Ϊ���Ͳ��Ų�ͬ����Ч��ͬʱά����¼"��·��"λ��
            �ͼ�¼��(��)��"��"��"����"��"����"�ƶ�����ĳ�Ա����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

        //����Ѿ����޸�
        if (!((CDocument*)m_pIntChessDoc)->IsModified())
        {
            ((CDocument*)m_pIntChessDoc)->SetModifiedFlag(TRUE);
        }

        //������һ
        m_nStepCount++;

        //��������λ��ָʾ��
        m_pChessEquipmentWnd->DrawIndicator(act.pSrcGrid->GetRect(), act.pDestGrid->GetRect());

        //��ȡ�Է�"��"��״̬
        BYTE nCheckCount = 0;   //"����"���ӵĸ���
        BYTE nKingStatus = GetKingStatus((CHESSMANCOLOR)!m_bWhichColorHasTurn, nCheckCount);

        //��¼���ף�ͬʱ�ж��Ƿ��γ�"�����ظ�����"��"50�غϹ���"�;�
        CM CMInfo;
        CMInfo.nAction = act.nAct;
        CMInfo.nKingStatus = nKingStatus;
        CMInfo.nPawnUp = m_nPawnUp;
        CMInfo.nCheckCount = nCheckCount;
        CMInfo.chessManType = chessManType;
        CMInfo.strSrcGridName = act.pSrcGrid->GetName();
        CMInfo.strDestGridName = act.pDestGrid->GetName();

        //�洢CM����(����ʱʹ��)
        m_CMList.AddTail(CMInfo);

        //�������ֵ
        DWORD dwPhaseValue = CalcPhaseValue();

        LRESULT dwResult = ::SendMessage(((CWnd*)m_pChessManalView)->GetSafeHwnd(), 
                                         WM_WRITECHESSMANUAL, (WPARAM)&CMInfo, (LPARAM)dwPhaseValue);
        //����1��ʾ�γɺ;�
        if (1L == dwResult)
        {
            nKingStatus = KS_DRAW;
        }

        switch (nKingStatus)
        {
        //����
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
        //��"����"
        case KS_CHECKED:
            CIntChessApp::PlayWaveResource(_T("IDR_CHECKED"));
            break;
        //��"��ɱ"
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
        //�;�
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

        //�ڰ�˫��������ʱ
        m_pChessDeskWnd->ExchangeTimerCounting();
    }
    else
    {
        const CM* pLastCMInfo = GetLastCMInfo();
        ASSERT(pLastCMInfo);

        //������һ
        m_nStepCount++;

        //��������λ��ָʾ��
        m_pChessEquipmentWnd->DrawIndicator(act.pSrcGrid->GetRect(), act.pDestGrid->GetRect());

        //�������ֵ
        DWORD dwPhaseValue = CalcPhaseValue();

        ::SendMessage(((CWnd*)m_pChessManalView)->GetSafeHwnd(), 
                      WM_WRITECHESSMANUAL, (WPARAM)pLastCMInfo, (LPARAM)dwPhaseValue);
    }

    //����͸����ŶԷ�������
    m_bWhichColorHasTurn = !m_bWhichColorHasTurn;

    //�������ԭ����¼"��·��"λ�õı�����ԭ����μ�OnMove���������˵��
    if (TA_MOVE != act.nAct)
    {
        m_strPassByPawnGridName = _T("");
    }

    //��¼"��"��"��"���ƶ����
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
        //���ų�������"��������"�������"��"�����
        else if (PAWNUP != m_nPawnUp)
        {
            CChessMan** pChessManAry = (CHESSMAN_WHITE == chessManColor) 
                ? m_pChessEquipmentWnd->GetWhiteChessManAry() 
                : m_pChessEquipmentWnd->GetBlackChessManAry();

            //����Rook�Ǻ�����Rook+1������
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
����      : tangjs520       ��������: 2003-11-23
������    : CChessRule::Undo
����ֵ    : void
�����б�  : ��
����      : ���������������Ϊ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessRule::Undo()
{
    const CM* pLastCMInfo = GetLastCMInfo();
    ASSERT(pLastCMInfo);

    ASSERT_VALID(m_pChessEquipmentWnd);
    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    //���ж��û��Ƿ��Ѿ�ѡ����һ�����ӣ����ѡ���ˣ�����Ҫֹͣ�����ӵ���˸
    CChessMan* pMan = CChessMan::GetActiveChessMan();
    if (pMan)
    {
        if (pMan->IsZooming())
        {
            pMan->StopZoom();
        }

        //����Ӧ�ò����ڱ�ѡ�е�������
        CChessMan::SetActiveChessMan(NULL);
    }

    //��ȡ���һ���������׵������Ϣ
    CString strDestGridName = pLastCMInfo->strDestGridName;
    CString strSrcGridName = pLastCMInfo->strSrcGridName;

    CChessGrid* pDestGrid = pBoard->GetGridFromName(strDestGridName);
    CChessGrid* pSrcGrid = pBoard->GetGridFromName(strSrcGridName);
    pMan = pDestGrid->GetIncludeChessMan();

    PCTG pCTG = NULL;   //��Ҫ���°�(��)��"��"��"����"��"����"���ƶ����
    CChessMan** pChessManAry = NULL;
    //�ֵ��ڷ����ӣ�������������ǰ׷�����Ϊ
    if (BLACK_TURN == m_bWhichColorHasTurn)
    {
        pCTG = &m_WhiteCTG;
        pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry();
    }
    //�ֵ��׷����ӣ�������������Ǻڷ�����Ϊ
    else
    {
        pCTG = &m_BlackCTG;
        pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry();
    }

    //�����������Ϊ��"������λ"������Ҫ��"��"�ƻ�ԭʼλ��
    if (TA_KINGROOKSHIFT == pLastCMInfo->nAction)
    {
        //����λ��"��"��ԭʼλ��������������
        CString strRookOriginalGridName = _T("");
        CChessMan* pRook = NULL;

        TCHAR ch = strDestGridName[0];
        //����λ
        if (_T('g') == ch)
        {
            strRookOriginalGridName = CString(_T('h')) + CString(strDestGridName[1]);
            pCTG->nRookOfKingSide_ShiftStep = 0;    //�������ڵ�״̬��û���ƶ�����
            //����Rook+1������
            pRook = pChessManAry[Rook + 1];
        }
        //����λ
        else
        {
            strRookOriginalGridName = CString(_T('a')) + CString(strDestGridName[1]);
            pCTG->nRookOfQueenSide_ShiftStep = 0;   //�������ڵ�״̬��û���ƶ�����
            //����Rook�Ǻ���
            pRook = pChessManAry[Rook];
        }

        //������س�Ա����
        pCTG->nKing_ShiftStep = 0;  //"��"���ڵ�״̬Ҳ��û���ƶ�����

        //��"��"�ƻ�ԭʼλ��
        pRook->SetPosition(pBoard->GetGridFromName(strRookOriginalGridName));
    }
    //���������ǽ�����"��"����Ϊ�������жϴ˴γ�����Ϊ�Ĳ���
    //�Ƿ����õ�����ǰ"��"��û���ƶ����ƶ�ʱ�Ĳ���ֵ�������ȵĻ���
    //���������"��"�Ѿ����չ�Ϊԭʼλ����
    else if (KING == pLastCMInfo->chessManType)
    {
        if (pCTG->nKing_ShiftStep == m_nStepCount)
        {
            pCTG->nKing_ShiftStep = 0;
        }
    }
    //���������ǽ�����"��"����Ϊ(��Ҫ�ų�������"��������"�����"��"�����)��
    //�����жϴ˴γ�����Ϊ�Ĳ����Ƿ����õ�����ǰ"��"��û���ƶ����ƶ�ʱ�Ĳ���ֵ��
    //�����ȵĻ������������"��"�Ѿ����չ�Ϊԭʼλ����
    else if (ROOK == pLastCMInfo->chessManType && PAWNUP != pLastCMInfo->nPawnUp)
    {
        //����Rook�Ǻ���
        if (pMan == pChessManAry[Rook])
        {
            if (pCTG->nRookOfQueenSide_ShiftStep == m_nStepCount)
            {
                pCTG->nRookOfQueenSide_ShiftStep = 0;
            }
        }
        //����Rook+1������
        else if (pMan == pChessManAry[Rook + 1])
        {
            if (pCTG->nRookOfKingSide_ShiftStep == m_nStepCount)
            {
                pCTG->nRookOfKingSide_ShiftStep = 0;
            }
        }
    }

    //�����ǰ����λ��ָʾ��
    m_pChessEquipmentWnd->RemoveIndicator();

    //����"��������"��Ϊʱ�����轫���ӻ�ԭ��"��"
    if (PAWNUP == pLastCMInfo->nPawnUp)
    {
        pMan->SetType(PAWN, FALSE);
    }

    //Ŀ��������Ҫ�ƻص�ԭ����λ��
    pMan->SetPosition(pSrcGrid);

    //�����������Ϊ��"����"��"�Թ�·��"������Ҫ������ʾ�������ӳԵ�������
    if (TA_KILL == pLastCMInfo->nAction || TA_KILLPASSBYPAWN == pLastCMInfo->nAction)
    {
        //��ȡ����������ӳԵ������Ӷ���ָ��
        CChessMan* pKilledMan = pMan->GetLastKilledMan();
        ASSERT(pKilledMan);

        //������ʾ�������ӳԵ�������
        pKilledMan->ShowChessMan(SCM_SHOW);

        if (TA_KILL == pLastCMInfo->nAction)
        {
            //���������"����"��Ϊ����Ŀ������ڰ��������Ӿ��Ǳ��Ե�������
            //�����ڳԵ�����ʱ����δ�޸����ӵ����ݳ�Աm_pInsideChessGrid��
            //�����ڴ�����������������λ�ڵ���������
            pDestGrid->SetIncludeChessMan(pKilledMan);
        }
        else
        {
            //�����"�Թ�·��"��Ϊ������Ŀ�����ͬ���Ե������Ӳ���ͬһ�����
            //������Ҫ��ȡ���Ե�����ԭ����λ�ڵ����������������������
            //���û���ʹ"�Թ�·��"��Ϊ֮ǰ���þ��Ǳ��Ե�������
            CChessGrid* pGrid = pKilledMan->GetInsideChessGrid();
            ASSERT(pGrid);

            pGrid->SetIncludeChessMan(pKilledMan);
        }

        //�����Ҫ֪ͨ������ɾ������������ӳԵ������Ӷ���ָ��
        pMan->RemoveKilledMan(RMK_LAST);
    }

    //����Ѿ����޸�
    if (!((CDocument*)m_pIntChessDoc)->IsModified())
    {
        ((CDocument*)m_pIntChessDoc)->SetModifiedFlag(TRUE);
    }

    //������س�Ա����
    m_nStepCount--;
    m_bWhichColorHasTurn = !m_bWhichColorHasTurn;
    m_CMList.RemoveTail();

    //�ڰ�˫��������ʱ
    m_pChessDeskWnd->ExchangeTimerCounting();

    //�ָ���ʾ��һ��������λ��ָʾ��
    pLastCMInfo = GetLastCMInfo();
    if (pLastCMInfo)
    {
        strDestGridName = pLastCMInfo->strDestGridName;
        strSrcGridName = pLastCMInfo->strSrcGridName;

        pDestGrid = pBoard->GetGridFromName(strDestGridName);
        pSrcGrid = pBoard->GetGridFromName(strSrcGridName);

        m_pChessEquipmentWnd->DrawIndicator(pSrcGrid->GetRect(), pDestGrid->GetRect());

        //������һ����������Ϣ������׼ȷ���жϳ��ڳ����˴�������Ϊ��
        //�����Ƿ���ڷ���"�Թ�·��"�����ı�����
        m_strPassByPawnGridName = _T("");
        if (PAWN == pLastCMInfo->chessManType)
        {
            //ע�⣺m_bWhichColorHasTurn�����Ѿ����µ������˴�������Ϊ���״̬��
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

        //�����˴�������Ϊ����һ��������״̬������������Ӧ�е�״̬
        //��������һ����nPawnUp����ֵ�������ڵ�m_nPawnUp����ֵ
        m_nPawnUp = pLastCMInfo->nPawnUp;
    }
    //�����������е�������Ϊ������ĳ�Ա��������ԭ���������ֵ�ˣ�
    //Ω������������Ա����û�еõ����ĸ��£������ڴ˻�Ҫ���Ӹ�λ��������Ա�����Ĵ���
    else
    {
        m_strPassByPawnGridName = _T("");
        m_nPawnUp = 0;

        //����Ѿ�����ԭ�����״̬��
        ((CDocument*)m_pIntChessDoc)->SetModifiedFlag(FALSE);
    }
}

/******************************************************************************************
void CChessRule::Save(CArchive& ar)
����      : tangjs520       ��������: 2003-11-25
������    : CChessRule::Save
����ֵ    : void
�����б�  : 
  ����1: CArchive& ar
����      : �������
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-11-25
������    : CChessRule::Open
����ֵ    : void
�����б�  : 
  ����1: CArchive& ar
����      : �����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CChessRule::Open(CArchive& ar)
{
    BOOL bRet = TRUE;

    //�Ȼ�ԭ������״̬
    OnGameNew();

    ASSERT_VALID(m_pChessEquipmentWnd);
    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    WORD nCount = 0;
    ar >> nCount;

    CM cmInfo = { 0 };
    BYTE nType = 0;

    //��ʼ��ȡ����ļ��ˣ���������ӦΪ�Զ�ģʽ
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

        //�������ļ�����Ч��
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

        //���ڶ�ȡ����ļ�������һ���ǳ���ʱ������ʱ��һ���Ƚϳ���
        //���п��ܷ�������������ˢ�µ����������Ϊ�˱���������������
        //��֤Ӧ�ó������Ϣ����ʼ�ռ��������´����������
        //��ʹû���κ���Ϣ����Ӧ�ó���Ҳ����������
        MSG message = { 0 };
        if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&message);
            ::DispatchMessage(&message);
        }

        Sleep(100);
    }

    //��ȡ������ļ���Ӧ���û�ģʽ��ԭΪĬ��ģʽ�����ֶ�ģʽ
    m_nUserMode = MANUALMODE;

    if (!bRet)
    {
        //����ļ��ѱ��ƻ���
        AfxMessageBox(IDS_FILE_ERR);
        //�������ڴ�Ӧ�ü��ϻ�ԭ������״̬�Ĵ���OnGameNew();
        //�����ڴ˺����ڷ���FALSE�󣬵���CIntChessDoc::OnOpenDocument����Ҳ������FALSE��
        //ϵͳ������CIntChessDoc::OnOpenDocument��������FALSE���Զ�����
        //CIntChessDoc::OnNewDocument������������������Ѿ�����OnGameNew()��
    }

    return bRet;
}

/******************************************************************************************
BOOL CChessRule::OnGameNew()
����      : tangjs520       ��������: 2003-11-26
������    : CChessRule::OnGameNew
����ֵ    : BOOL
�����б�  : ��
����      : ����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CChessRule::OnGameNew()
{
    if (!((CDocument*)m_pIntChessDoc)->CDocument::OnNewDocument())
    {
        return FALSE;
    }

    //�ж��û��Ƿ��Ѿ�ѡ����һ�����ӣ����ѡ���ˣ�����Ҫֹͣ�����ӵ���˸
    CChessMan* pMan = CChessMan::GetActiveChessMan();
    if (pMan)
    {
        if (pMan->IsZooming())
        {
            pMan->StopZoom();
        }

        //����Ӧ�ò����ڱ�ѡ�е�������
        CChessMan::SetActiveChessMan(NULL);
    }

    //�����ʱ���Ծ������У�����Ҫ��λ��ʱ��
    if (m_pChessDeskWnd->IsTimerRunning())
    {
        m_pChessDeskWnd->ResetTimer();
    }

    //�����ǰ��û���߶��κ����ӣ�����������
    if (!m_nStepCount)
    {
        return TRUE;
    }

    //��λ���������(�ָ����еĳ�Ա����Ϊ��ʼֵ)
    m_bWhichColorHasTurn = WHITE_TURN;
    m_strPassByPawnGridName = _T("");
    memset(&m_WhiteCTG, 0, sizeof(CTG));
    memset(&m_BlackCTG, 0, sizeof(CTG));
    m_nStepCount = 0;
    m_nPawnUp = 0;
    m_nUserMode = MANUALMODE;
    m_CMList.RemoveAll();

    //�������
    ::SendMessage(((CWnd*)m_pChessManalView)->GetSafeHwnd(), WM_CLEARCHESSMANUAL, 0L, 0L);

    if (m_pChessEquipmentWnd)
    {
        m_pChessEquipmentWnd->Renew();
    }

    return TRUE;
}

/******************************************************************************************
const BOOL CChessRule::IsStarted()
����      : tangjs520       ��������: 2003-12-5
������    : CChessRule::IsStarted
����ֵ    : const BOOL
�����б�  : ��
����      : �ж�����Ƿ��Ѿ���ʼ��
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-12-9
������    : CChessRule::ProcTimeOut
����ֵ    : void
�����б�  : 
  ����1: BOOL bWhichClockTimeOut
����      : �������ӳ�ʱ�����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessRule::ProcTimeOut(BOOL/*bWhichClockTimeOut*/)
{
}

/******************************************************************************************
void CChessRule::OnExchangeChessMan()
����      : tangjs520       ��������: 2003-11-26
������    : CChessRule::OnExchangeChessMan
����ֵ    : void
�����б�  : ��
����      : ��������
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::OnMove
����ֵ    : void
�����б�  : 
  ����1: const ACT& act
����      : �����ƶ����ӵ���Ϊ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessRule::OnMove(const ACT& act)
{
    //��¼���һ���γ�"��·��"�����ı���λ��
    //���ȱ������ԭ���ļ�¼����Ϊ��������涨����һ��������"�Թ�·��"�Ļ���ʱ��
    //���Ը����Լ���ս����Ҫ���ԣ���������һ���ᣬ�Ժ�Ͳ����ٳ��ˡ�
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
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::OnKill
����ֵ    : void
�����б�  : 
  ����1: const ACT& act
����      : ����Ե����ӵ���Ϊ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

    //��������������ӳԵ������Ӷ���ָ��
    act.pSrcMan->SetLastKilledMan(pKilledMan);
}

/******************************************************************************************
void CChessRule::OnKillPassByPawn(const ACT& act)
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::OnKillPassByPawn
����ֵ    : void
�����б�  : 
  ����1: const ACT& act
����      : ����"�Թ�·��"����Ϊ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

    //��������������ӳԵ������Ӷ���ָ��
    act.pSrcMan->SetLastKilledMan(pKilledMan);
}

/******************************************************************************************
void CChessRule::OnKingRookShift(const ACT& act)
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::OnKingRookShift
����ֵ    : void
�����б�  : 
  ����1: const ACT& act
����      : ����"������λ"����Ϊ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessRule::OnKingRookShift(const ACT& act)
{
    CChessMan** pChessManAry = NULL;
    CChessMan* pRook = NULL;
    //������AfterDispatchAction�����м�¼"��"��"��"���ƶ����ʱ��
    //�����"������λ"��Ϊ����ֻ��¼"��"���ƶ�������������¼��"��"��λ��"��"���ƶ������
    //Ϊ��ά��"��"���ƶ��������Ч�ԣ������ڴ���Ҫ��¼"��"���ƶ����
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
        //����Rook+1������
        pRook = pChessManAry[Rook + 1];
        strRookNewGridName = CString(_T('f')) + CString(act.pDestGrid->GetName()[1]);
        //���ڴ�ʱm_nStepCount��δ��һ������������Ҫ����һ
        pCTG->nRookOfKingSide_ShiftStep = (WORD)(m_nStepCount + 1);
    }
    else
    {
        //����Rook�Ǻ���
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
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::OnIllegal
����ֵ    : void
�����б�  : 
  ����1: const ACT&
����      : ��Ч����Ϊ���貥���ض�����Ч����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-11-3
������    : CChessRule::GetLegalGridName
����ֵ    : void
�����б�  : 
  ����1: const CHESSMANCOLOR chessManColor
  ����2: const CHESSMANTYPE chessManType
  ����3: const CString& strSrcGridName
  ����4: CStringList& strLstLegalGridName
  ����5: const BYTE nType
����      : ��ȡָ��������ĳһ���Ͽ��Ե���(����Կ���)�ķ������������������ļ���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
                    //����ԭʼλ��ʱ����һ�������������
                    nStep = 2;
                }

                //����ֱ�߷����ϵ��������ڱ���û������
                GetRelationGridName(chessManColor, strSrcGridName, 
                                    strLstLegalGridName, EC_MUST_EMPTY, RG_VLINE, nDirect, nStep);
            }

            //����б�߷����ϵ��������ڱ����жԷ����ӣ����Ƶ�����ڿ���û������
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
����      : tangjs520       ��������: 2003-11-4
������    : CChessRule::KnightLegal
����ֵ    : void
�����б�  : 
  ����1: const CHESSMANCOLOR chessManColor
  ����2: const CString& strSrcGridName
  ����3: CStringList& strLstLegalGridName
  ����4: const BYTE nType
����      : ��ȡ�Ϻ�"��"���߷�����񼯺�
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessRule::KnightLegal(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName, 
                             CStringList& strLstLegalGridName, const BYTE nType/* = LG_ARRIVE*/)
{
    ASSERT_VALID(m_pChessEquipmentWnd);
    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    int nAlphabet = (int)strSrcGridName[0]; //��ʾԴ������Ƶ���ĸ����
    int nNumber = (int)strSrcGridName[1];   //��ʾԴ������Ƶ����ֲ���

    CString strGridName = _T("");
    CChessMan* pMan = NULL;

    //��������ѭ�����ڵ�ԭ���ǣ�
    //1����Դ�������(������ĸ�����ֲ���)����"��"���������(������ĸ�����ֲ���)��
    //(1)����ĸ-1{����+2������-2}
    //(2)����ĸ+1{����+2������-2}
    //(3)����ĸ-2{����+1������-1}
    //(4)����ĸ+2{����+1������-1}
    //2������ʽi*(2n-1)[i(1,2)��n(0,1)]�ɵõ�(-1,+1,-2,+2)�ĸ����ֵİ������
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
����      : tangjs520       ��������: 2003-11-12
������    : CChessRule::IsAttacked
����ֵ    : BOOL
�����б�  : 
  ����1: const CHESSMANCOLOR chessManColor
  ����2: const CString& strSrcGridName
  ����3: CStringArray* pStrAryAttack(�Է���������������������)
����      : �ж�һ��������ĳһ�����Ƿ����ܶԷ����ӵĹ���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

    //��ѯ�Է�16�����ӵĿ��Ʒ�Χ������Դ����Ƿ��ڴ˷�Χ���������ж�
    for (int i = 0; i < 16; i++)
    {
        //��������Ѿ���"�Ե�"�ˣ����ò����ж���
        if (!(pChessManAry[i]->IsVisible()))
        {
            continue;
        }

        //���Դ�����ǶԷ��������ڵ����Ҳ���ò����ж�
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
����      : tangjs520       ��������: 2003-11-12
������    : CChessRule::IsKingChecked
����ֵ    : BOOL
�����б�  : 
  ����1: const CHESSMANCOLOR KingColor
����      : �ж�һ����"��"�Ƿ����ܶԷ����ӵĹ���(Ҳ����"����")
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-11-13
������    : CChessRule::IsPinedDown
����ֵ    : BOOL
�����б�  : 
  ����1: const CHESSMANCOLOR chessManColor
  ����2: const CString& strSrcGridName
����      : �ж�һ��������ĳһ���Ƿ����ܶԷ����ӵ�ǣ��
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CChessRule::IsPinedDown(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName)
{
    ASSERT_VALID(m_pChessEquipmentWnd);

    //���Ȼ�ȡ����"��"���ڵ��������
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
    //�����Ҫ�жϵ������Ǽ���"��"���ڵ���������ڷǷ�����
    ASSERT(strKingGridName != strSrcGridName);

    CStringList strLstGridName;
    BYTE nRelation = GetBetweenGridName(strSrcGridName, strKingGridName, strLstGridName);
    //�����Ҫ�жϵ�����뼺��"��"���ڵ����û���κ��߼���ϵ������������FALSE
    if (RG_NONE == nRelation)
    {
        return FALSE;
    }

    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    int nCount = strLstGridName.GetCount();
    //�����Ҫ�жϵ�����뼺��"��"���ڵ����֮�仹�����������
    //������ж�����֮���������Ƿ񻹴����������ӣ�������ڵĻ���
    //����Ҫ�жϵ�����ǲ��ܶԷ�ǣ�Ƶ�
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
    //�����Ҫ�жϵ�����뼺��"��"���ڵ��������ţ�
    //��������֮�������������񣬵���Щ�����û���κ����ӣ�
    //����Ҫ�����жϣ�����������߼����������ϣ��Ƿ������ǣ�Ƹ���������
    int nSrcAlphabet = (int)strSrcGridName[0];  //��ʾԴ������Ƶ���ĸ����
    int nSrcNumber = (int)strSrcGridName[1];    //��ʾԴ������Ƶ����ֲ���

    if (RG_HLINE == nRelation)
    {
        int nSub = (strSrcGridName > strKingGridName) ? 1 : -1;

        int nEnd = (1 == nSub) ? (int)_T('h') + 1 : (int)_T('a') - 1;

        for (int i = nSrcAlphabet + nSub; i != nEnd; i += nSub)
        {
            //�����µ��������
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
            //�����µ��������
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
        int nKingAlphabet = (int)strKingGridName[0];    //��ʾ����"��"���ڵ�������Ƶ���ĸ����
        int nKingNumber = (int)strKingGridName[1];      //��ʾ����"��"���ڵ�������Ƶ����ֲ���

        int nSubAlp = (nSrcAlphabet > nKingAlphabet) ? 1 : -1;

        int nSubNum = (nSrcNumber > nKingNumber) ? 1 : -1;

        int nEndAlp = (1 == nSubAlp) ? (int)_T('h') + 1 : (int)_T('a') - 1;

        int nEndNum = (1 == nSubNum) ? (int)_T('8') + 1 : (int)_T('1') - 1;

        for (int i = nSrcAlphabet + nSubAlp, j = nSrcNumber + nSubNum; 
             i != nEndAlp && j != nEndNum; i += nSubAlp, j += nSubNum)
        {
            //�����µ��������
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
����      : tangjs520       ��������: 2003-11-12
������    : CChessRule::GetKingStatus
����ֵ    : BYTE
�����б�  : 
  ����1: const CHESSMANCOLOR KingColor
  ����1: BYTE& nCheckCount
����      : ��ȡ"��"��״̬(KS_NORMAL��KS_CHECKED��KS_CHECKMATED, KS_DRAW)
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BYTE CChessRule::GetKingStatus(const CHESSMANCOLOR KingColor, BYTE& nCheckedCount)
{
    ASSERT_VALID(m_pChessEquipmentWnd);
    CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
    ASSERT(pBoard);

    //���Ȼ�ȡ����"��"���ڵ��������
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

    //�ж�"��"�Ƿ񱻶Է����ӹ���
    CStringArray strAryCheck;
    BOOL bChecked = IsAttacked(KingColor, strKingGridName, &strAryCheck);
    if (bChecked)
    {
        nCheckedCount = (BYTE)strAryCheck.GetSize();
        //���ж��ܷ�ܽ�
        //��ȡ"��"�ܵ�������
        CStringList strLstArrive;
        GetLegalGridName(KingColor, KING, strKingGridName, strLstArrive);
        for (POSITION pos = strLstArrive.GetHeadPosition(); pos != NULL;)
        {
            //���"��"���ܵ��������ܶԷ����ӵĹ�����
            //��ģ��"��"�ƶ����������Բ��ܹ��������ʾ"��"���Աܽ�
            CString strArriveGridName = strLstArrive.GetNext(pos);
            if (!IsAttacked(KingColor, strArriveGridName))
            {
                //����ʱģ��"��"�ƶ�(��Ե�)�����������Ӧ�е�״̬
                CChessGrid* pDestGrid = pBoard->GetGridFromName(strArriveGridName);
                CChessMan* pKilledMan = pDestGrid->GetIncludeChessMan();
                if (pKilledMan)
                {
                    pKilledMan->ShowChessMan(SCM_HIDE, FALSE);
                }

                pKingGrid->SetIncludeChessMan(NULL);
                pDestGrid->SetIncludeChessMan(pChessManAry[King]);
                pChessManAry[King]->SetInsideChessGrid(pDestGrid);

                //�����ж���ģ���״̬�£�����"��"�Ƿ���ܶԷ����ӵĹ���
                BOOL bKingChecked = IsKingChecked(KingColor);

                //Ȼ��ԭģ��ǰ�����״̬
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

                //�������"��"���ܵ��Է����ӹ��������ʾ"��"���Աܽ�
                if (!bKingChecked)
                {
                    return KS_CHECKED;
                }
            }
        }
        //����Է�"����"�����Ӳ�ֹһ������"��"��û���ܽ�����"��"��"��ɱ"
        if (nCheckedCount >= 2)
        {
            return KS_CHECKMATED;
        }

        //���ж��ܷ�����
        //�жϼ����������ܷ���ƶԷ�"����"�������ڵ����
        CStringArray strAryControl;
        BOOL bControl = IsAttacked((CHESSMANCOLOR)!KingColor, strAryCheck[0], &strAryControl);
        if (bControl)
        {
            int nControlCount = strAryControl.GetSize();
            for (int i = 0; i < nControlCount; i++)
            {
                //�������"��"�ܿ��Ƹ�������ò���˴��жϣ�
                //��Ϊ�������ıܽ��У��Ѿ��Դ���������ж���
                //ͬʱ����"��"���ò����Ƿ���ǣ�Ƶ��ж�
                if (KING == pBoard->GetGridFromName(strAryControl[i])->GetIncludeChessMan()->GetType())
                {
                    //����ܳԵ��Է�"����"���ӵ�ֻ�м���"��"�ˣ�Ҳ�Ͳ�����Ҫ�ж��ܷ�潫�ˣ�
                    //���Կ϶�����"��"��"��ɱ"��
                    if (1 == nControlCount)
                    {
                        return KS_CHECKMATED;
                    }

                    continue;
                }

                //�жϸ������Ƿ���ǣ�ƣ������ǣ�����ʾ�������޷��Ե��Է�"����"������
                if (!IsPinedDown(KingColor, strAryControl[i]))
                {
                    return KS_CHECKED;
                }
            }
        }

        //����ж��ܷ�潫
        //���"����"��������"��"��"��"���򲻿��ܵ潫
        CHESSMANTYPE checkType = pBoard->GetGridFromName(strAryCheck[0])->GetIncludeChessMan()->GetType();
        if (KNIGHT == checkType || PAWN == checkType)
        {
            return KS_CHECKMATED;
        }
        else
        {
            //��ȡ����"��"�������ͶԷ�"����"�����������֮����������
            CStringList strLstBetweenGridName;
            GetBetweenGridName(strKingGridName, strAryCheck[0], strLstBetweenGridName);
            for (POSITION pos = strLstBetweenGridName.GetHeadPosition(); pos != NULL;)
            {
                CString strGridName = strLstBetweenGridName.GetNext(pos);
                //��ѯ����16�����ӵĵ��ﷶΧ�����ݸղ�����õ�����Ƿ��ڴ˷�Χ���������ж�
                for (int i = 0; i < 16; i++)
                {
                    //��������Ѿ���"�Ե�"�ˣ����ò����ж���
                    if (!(pChessManAry[i]->IsVisible()))
                    {
                        continue;
                    }
                    //����Ǽ��������������������ж�
                    if (pChessManAry[King] == pChessManAry[i])
                    {
                        continue;
                    }

                    strLstArrive.RemoveAll();
                    CString strName = pChessManAry[i]->GetInsideChessGrid()->GetName();
                    GetLegalGridName(KingColor, pChessManAry[i]->GetType(), strName, strLstArrive);
                    if (strLstArrive.Find(strGridName) != NULL)
                    {
                        //��������������Է�ǣ���ţ����ʾ�������޷�"�潫"
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
        //���"��"û�б�"����"�����ж��Ƿ��γɺ;�
        if (IsDraw())
        {
            return KS_DRAW;
        }
    }

    return KS_NORMAL;
}

/******************************************************************************************
BOOL CChessRule::IsDraw()
����      : tangjs520       ��������: 2003-11-16
������    : CChessRule::IsDraw
����ֵ    : BOOL
�����б�  : ��
����      : �ж��Ƿ��γɺ;�
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CChessRule::IsDraw()
{
    ASSERT_VALID(m_pChessEquipmentWnd);

    //�����ж��Ƿ��γ�"�ƺ�"
    if (IsForceDraw())
    {
        return TRUE;
    }
    //�����ж��Ƿ��γɾ���;�[��"�ٺ�"]
    if (IsSituationDraw())
    {
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************************
BOOL CChessRule::IsForceDraw()
����      : tangjs520       ��������: 2003-11-16
������    : CChessRule::IsForceDraw
����ֵ    : BOOL
�����б�  : ��
����      : �ж��Ƿ��γ�"�ƺ�"
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CChessRule::IsForceDraw()
{
    CChessMan** pChessManAry;
    CHESSMANCOLOR chessManColor;
    m_bWhichColorHasTurn ? (pChessManAry = m_pChessEquipmentWnd->GetWhiteChessManAry(), 
        chessManColor = CHESSMAN_WHITE) 
        : (pChessManAry = m_pChessEquipmentWnd->GetBlackChessManAry(), 
        chessManColor = CHESSMAN_BLACK);

    //�ж϶Է������Ƿ����ƶ�������������ƶ������γ�"�ƺ�"
    CStringList strLstArriveGridName;
    for (int i = 0; i < 16; i++)
    {
        //��������Ѿ���"�Ե�"�ˣ����ò����ж�
        if (!(pChessManAry[i]->IsVisible()))
        {
            continue;
        }

        strLstArriveGridName.RemoveAll();
        CHESSMANTYPE chessManType = pChessManAry[i]->GetType();
        CString strGridName = pChessManAry[i]->GetInsideChessGrid()->GetName();
        GetLegalGridName(chessManColor, chessManType, strGridName, strLstArriveGridName);

        //����������ƶ����ٿ����Ƿ��ܶԷ�ǣ�ƣ�������ܶԷ�ǣ��
        //����Կ϶����ڻ�δ�γ�"�ƺ�"����
        if (!strLstArriveGridName.IsEmpty())
        {
            //�����"��"�����ƶ�����Ҫ�����ж�"��"���ܵ��������Ƿ��ܶԷ����ӹ���
            //���⣬"��"�ǲ��ò����Ƿ���ǣ�Ƶ��ж�
            if (KING == chessManType)
            {
                for (POSITION pos = strLstArriveGridName.GetHeadPosition(); pos != NULL;)
                {
                    //���"��"���ܵ��������ܶԷ����ӵĹ�����
                    //���ʾ"��"�����߶�
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
����      : tangjs520       ��������: 2003-11-16
������    : CChessRule::IsSituationDraw
����ֵ    : BOOL
�����б�  : ��
����      : �ж��Ƿ��γɾ���;�[��"�ٺ�"]
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CChessRule::IsSituationDraw()
{
    //���¼��������Ϊ"�ٺ�"��
    //1��˫��ֻʣ��
    //2��һ����������һ���������
    //3��˫��ʣͬɫ����
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
            //��������Ѿ���"�Ե�"�ˣ����ò����ж�
            if (!(pChessManAry[i]->IsVisible()))
            {
                continue;
            }

            CHESSMANTYPE chessManType = pChessManAry[i]->GetType();
            //���һ�������ڳ���������֮������ӣ��򲻿���"�ٺ�"
            if (KING != chessManType && BISHOP != chessManType && KNIGHT != chessManType)
            {
                return FALSE;
            }

            switch (chessManType)
            {
            case BISHOP:
                {
                    (*pBishopCount)++;
                    //���һ��������˫����������򲻿���"�ٺ�"
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
                    //���һ��������˫����������򲻿���"�ٺ�"
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

    //һ��ʣ��(����)����һ��ʣ��(����)��Ҳ������"�ٺ�"
    if ((1 == nWhiteBishopCount && 1 == nBlackKnightCount) 
        || (1 == nWhiteKnightCount && 1 == nBlackBishopCount))
    {
        return FALSE;
    }
    //���˫����ʣһ����Ҳ������"�ٺ�"
    if (1 == nWhiteKnightCount && 1 == nBlackKnightCount)
    {
        return FALSE;
    }
    //���˫����ʣһ������Ϊ��ɫ��Ҳ������"�ٺ�"
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
����      : tangjs520       ��������: 2003-11-3
������    : CChessRule::IsKillPassByPawn
����ֵ    : BOOL
�����б�  : 
  ����1: const CHESSMANCOLOR PawnColor
  ����2: const CString& strSrcGridName
  ����3: const CString& strDestGridName
����      : �ж��Ƿ��γ�"�Թ�·��"
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CChessRule::IsKillPassByPawn(const CHESSMANCOLOR PawnColor, const CString& strSrcGridName, 
                                  const CString& strDestGridName)
{
    int nDirect = 0;
    //���ױ��ڵ�5���ߡ��ڱ��ڵ�4����ʱ���п����߳�"�Թ�·��"�Ĳ���
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

    //����"�Թ�·��"�ĳԷ���ͬб�ԣ��������ȵ��ж�Ŀ������Ƿ���Դ���
    //��ǰ�������ϵ�бһ��
    GetRelationGridName(PawnColor, strSrcGridName, strLstGridName, 
                        EC_MUST_EMPTY, RG_DLINE, nDirect, 1);

    if (strLstGridName.Find(strDestGridName) != NULL)
    {
        //�����"�Թ�·��"�Ļ�����Ŀ�������ֱ�߷����Ϻ���һ��ǡ����
        //��Ա����m_strPassByPawnGridName��¼�����
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
����      : tangjs520       ��������: 2003-11-7
������    : CChessRule::IsKingRookShift
����ֵ    : BOOL
�����б�  : 
  ����1: const CHESSMANCOLOR KingColor
  ����2: const CString& strDestGridName
����      : �ж��Ƿ��γ�"������λ"
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

    //ʩ��"������λ"�Ĺ涨����Ϊ��
    //1�����ͳ���������ԭʼλ�ò����ƶ�
    //2�����복֮������������
    //3�����ӳ����������λ��3�����Ӿ�δ�ܹ���
    if (!pCTG->nKing_ShiftStep)
    {
        ASSERT_VALID(m_pChessEquipmentWnd);
        CChessBoard* pBoard = m_pChessEquipmentWnd->GetChessBoard();
        ASSERT(pBoard);

        //����λ
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
        //����λ
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
����      : tangjs520       ��������: 2003-10-27
������    : CChessRule::GetRelationGridName
����ֵ    : void
�����б�  : 
  ����1: const CHESSMANCOLOR chessManColor(���ӵ���ɫ)
  ����2: const CString& strSrcGridName(�����������ʾԴ�������)
  ����3: CStringList& strLstGridName(�����������������������������ƶ�����ü�����)
  ����4: const BYTE nEndCondition(ֹͣ����������)
  ����5: BYTE nRGType(�����������ʾ�߼���ϵ��������RG_HLINE��RG_VLINE��RG_DLINE���������)
  ����6: const int nDirection(�����������ʾ���򣬿����ǵ������ݼ������з���)
  ����7: BYTE nStep(�����������ʾ����Ӧ�������������[�༴�������])
����      : ��ȡ��ָ�����������߼���ϵ���������ļ���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

    //��������з�����ֽ�ɵ����͵ݼ�����������еݹ����
    if (ALL_DIRECT == nDirection)
    {
        GetRelationGridName(chessManColor, strSrcGridName, strLstGridName, 
                            nEndCondition, nRGType, ADD_DIRECT, nStep);
        GetRelationGridName(chessManColor, strSrcGridName, strLstGridName, 
                            nEndCondition, nRGType, SUBTRACT_DIRECT, nStep);

        return;
    }

    int nAlphabet = 0;  //��ʾԴ������Ƶ���ĸ����
    int nNumber = 0;    //��ʾԴ������Ƶ����ֲ���

    //���Ǹ����õı�������������Ϊ���ں����do-whileѭ���У�
    //���߼���ϵ����RG_DLINEʱ������nAlphabet(RG_HLINE)��nNumber(RG_VLINE)����
    int nUseless = 0;

    //pAlphabet��pNumber����ָ��nAlphabet��nNumber��nUseless
    //pRefָ�򵱱Ƚϲ���ʱ�Ĳ��ձ�������������nAlphabet����nNumber
    int *pAlphabet = NULL, *pNumber = NULL, *pRef = NULL;

    //��ʾ��ĸ���ֺ����ֲ����ǵ���(nDirection = +1)���ǵݼ�(nDirection = -1)
    int nAlphabetBase = nDirection;
    int nNumberBase = nDirection;

    //forѭ���Ĵ��������߼���ϵ��RG_DLINEʱ��forѭ������ѭ������
    //��ΪΨ��б���߼���ϵ�ڵ�����ݼ������ϴ���������
    //�����߼���ϵʱ���ñ�����ֵΪ1
    int nLoopCount = 0;

    //�����ڽ��в����Ƚ�ʱ�ı��Ƚ�ֵ���ҳƺ���Ϊ����"�ڱ�"
    //��nStep����NOLIMIT(=0)ʱ���ñ������κ�ʵ������
    //��Ϊ�����ı��ʽ"nEndSentinel = nAlphabet(nNumber) + nDirection * nStep;"ʹ��
    //"while ((*pRef) != nEndSentinel);"��Զ����
    int nEndSentinel = 0;

    //�����е��߼���ϵ��������(nRGType = 0)����������ѭ������ɺ�����ʹ��
    while (0 != nRGType)
    {
        //ÿ��ѭ��ǰ������������nAlphabet��nNumber
        //��Ϊ��������һ���߼���ϵ������nAlphabet��(��)nNumber��Ȼ�����仯
        nAlphabet = (int)strSrcGridName[0];
        nNumber = (int)strSrcGridName[1];
        //��nAlphabetBase�����������ã��ǿ��ǵ����߼���ϵ��RG_DLINEʱ��
        //nAlphabetBaseҲ��Ȼ�����仯
        nAlphabetBase = nDirection;

        //���Ǻ����߼���ϵʱ
        if (nRGType & RG_HLINE)
        {
            //������Ҫ����ĸ������������ݼ�����
            pAlphabet = &nAlphabet;
            pNumber = &nUseless;

            nLoopCount = 1;

            nEndSentinel = nAlphabet + nDirection * nStep;
            pRef = &nAlphabet;

            //ȥ������������߼���ϵ��������һ��ѭ������ͬ���Ĵ���
            //������γ�"��"ѭ��
            nRGType &= ~RG_HLINE;
        }
        //����ֱ���߼���ϵʱ
        else if (nRGType & RG_VLINE)
        {
            //������Ҫ�����ֲ�����������ݼ�����
            pNumber = &nNumber;
            pAlphabet = &nUseless;

            nLoopCount = 1;

            nEndSentinel = nNumber + nDirection * nStep;
            pRef = &nNumber;

            nRGType &= ~RG_VLINE;
        }
        //����б���߼���ϵʱ
        else if (nRGType & RG_DLINE)
        {
            //��Ҫͬʱ����ĸ���ֺ����ֲ�����������ݼ�����
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
                //����ĸ���ֺ�(��)���ֲ�����������ݼ�����
                (*pAlphabet) += nAlphabetBase;
                (*pNumber) += nNumberBase;

                //�����µ��������
                CString strGridName = CString((TCHAR)nAlphabet) + CString((TCHAR)nNumber);

                //�ж��µ���������Ƿ��������ϴ���
                //��������ڣ�������������������������
                //������ڣ������ݲ���nEndCondition���ж�
                //�Ƿ�Ӧ�ý���������ƴ������������
                CChessGrid* pGrid = NULL;
                pGrid = pBoard->GetGridFromName(strGridName);
                if (pGrid)
                {
                    CChessMan* pMan = NULL;
                    pMan = pGrid->GetIncludeChessMan();

                    if (pMan)
                    {
                        //���ֹͣ������������"����ڱ���û������"
                        //����"����������������"����������ڵ�����
                        //ǡ���Ǽ����ģ��򲻼�¼����������Ͻ���
                        //����������ϵ�����
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
            } while ((*pRef) != nEndSentinel);  //�ж��Ƿ��Ѿ�����ָ������

            //���߼���ϵ��RG_DLINEʱ(����ѭ������)�����뻹ԭ����nAlphabet��nNumber��
            //��Ϊ����nAlphabet��nNumber���Ѿ������仯�ˣ�
            //���⻹�轫ԭ����ĸ���ֵı仯�����÷�
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
����      : tangjs520       ��������: 2003-11-2
������    : CChessRule::GetBetweenGridName
����ֵ    : BYTE(����Դ���ͬĿ�������߼���ϵ)
�����б�  : 
  ����1: const CString& strSrcGridName
  ����2: const CString& strDestGridName
  ����3: CStringList& strLstGridName
����      : ��ȡ��Դ����Ŀ�����֮��(��ָ���ߡ�ֱ�߻�б�����������е�һ��)���������ļ���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BYTE CChessRule::GetBetweenGridName(const CString& strSrcGridName, 
                                    const CString& strDestGridName, 
                                    CStringList& strLstGridName)
{
    //���뱣֤Դ����Ŀ�������ͬһ��
    ASSERT(strSrcGridName != strDestGridName);

    int nSrcAlphabet = (int)strSrcGridName[0];      //��ʾԴ������Ƶ���ĸ����
    int nSrcNumber = (int)strSrcGridName[1];        //��ʾԴ������Ƶ����ֲ���

    int nDestAlphabet = (int)strDestGridName[0];    //��ʾĿ��������Ƶ���ĸ����
    int nDestNumber = (int)strDestGridName[1];      //��ʾĿ��������Ƶ����ֲ���

    //ֱ�߷���
    if (nSrcAlphabet == nDestAlphabet)
    {
        if (1 != abs(nSrcNumber - nDestNumber))
        {
            for (int i = min(nSrcNumber, nDestNumber) + 1; 
                 i < max(nSrcNumber, nDestNumber); i++)
            {
                //�����µ��������
                CString strGridName = CString((TCHAR)nSrcAlphabet) + CString((TCHAR)i);
                strLstGridName.AddTail(strGridName);
            }
        }

        return RG_VLINE;
    }
    //���߷���
    else if (nSrcNumber == nDestNumber)
    {
        if (1 != abs(nSrcAlphabet - nDestAlphabet))
        {
            for (int i = min(nSrcAlphabet, nDestAlphabet) + 1; 
                 i < max(nSrcAlphabet, nDestAlphabet); i++)
            {
                //�����µ��������
                CString strGridName = CString((TCHAR)i) + CString((TCHAR)nSrcNumber);
                strLstGridName.AddTail(strGridName);
            }
        }

        return RG_HLINE;
    }
    //б�߷���
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
                    //�����µ��������
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
����      : tangjs520       ��������: 2003-11-22
������    : CChessRule::CalcPhaseValue
����ֵ    : DWORD
�����б�  : ��
����      : �������ֵ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
            //���ֵ = ���������ĸ���ֵ�ASCII���ƽ�� + ����������ֲ��ֵ�ASCII���ƽ��
            DWORD dwGridValue = (DWORD)(strGridName[0]) * (DWORD)(strGridName[0]) 
                + (DWORD)(strGridName[1]) * (DWORD)(strGridName[1]);

            //double _scalb(double x, long exp);
            //The _scalb function calculates the value of x * 2��exp����.
            //�������ֵ
            dwRet += (DWORD)_scalb(dwGridValue, (DWORD)pChessManAry[i]->GetType() + 1);
        }
    }

    return dwRet;
}
