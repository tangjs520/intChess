/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessRule.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-31
�ļ�����    : ������������࣬���ڸ���Ĺ�������ȫ���Ǿ�̬�ģ������������ɸ���Ķ��󼴿�
              ֱ��ʹ�ø���(ǰ���ǣ���ʹ��֮ǰ���������Ա����
              m_pChessEquipmentWnd��m_pChessManalView��m_pIntChessDoc��m_pChessDeskWnd ������Чֵ)
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_CHESSRULE_H__A899C08F_0F4B_4AF4_97FA_555CE0B99C4B__INCLUDED_)
#define AFX_CHESSRULE_H__A899C08F_0F4B_4AF4_97FA_555CE0B99C4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessMan.h"   //������(CChessMan)��ͷ�ļ�

class CChessEquipmentWnd;   //��ߴ�����(CChessEquipmentWnd)��ǰ������
class CChessManualView;     //������ͼ��(CChessManualView)��ǰ������
class CIntChessDoc;         //�ĵ���(CIntChessDoc)��ǰ������
class CChessDeskWnd;        //����������(CChessDeskWnd)��ǰ������

const BOOL WHITE_TURN = FALSE;  //�ɰ׷�����
const BOOL BLACK_TURN = TRUE;   //�ɺڷ�����

//��ʶ�û�����Ϊ����
const BYTE TA_MOVE = 0x01;              //�ƶ�����
const BYTE TA_KILL = 0x02;              //�Ե�����
const BYTE TA_KILLPASSBYPAWN = 0x03;    //�Թ�·��
const BYTE TA_KINGROOKSHIFT = 0x04;     //������λ
const BYTE TA_ILLEGAL = 0x08;           //��Ч����Ϊ

const BYTE RG_HLINE = 0x01;     //�����߼���ϵ��ʶ��
const BYTE RG_VLINE = 0x02;     //ֱ���߼���ϵ��ʶ��
const BYTE RG_DLINE = 0x04;     //б���߼���ϵ��ʶ��
const BYTE RG_NONE = 0x00;      //���߼���ϵ��ʶ��

const int ADD_DIRECT = +1;                              //��������
const int SUBTRACT_DIRECT = -1;                         //�ݼ�����
const int ALL_DIRECT = ADD_DIRECT + SUBTRACT_DIRECT;    //���з���

const BYTE EC_EXCLUDE_SELF = 0x01;          //����������������
const BYTE EC_INCLUDE_SELF = 0x02;          //��������������
const BYTE EC_MUST_HAVE_CHESSMAN = 0x04;    //����ڱ���Ҫ������
const BYTE EC_MUST_EMPTY = 0x08;            //����ڱ���û������

const BYTE NOLIMIT = 0;     //��ʶ����û������

const BYTE LG_ARRIVE = 0;   //���Ե�������
const BYTE LG_CONTROL = 1;  //���Կ��Ƶ����

//"��"��״̬
const BYTE KS_NORMAL = 0;       //����
const BYTE KS_CHECKED = 1;      //��"����"
const BYTE KS_CHECKMATED = 2;   //��"��ɱ"
const BYTE KS_DRAW = 3;         //�;�

//��ʶ������
const BYTE PAWNUP = 1;

//��ʶ�û�����Ϊģʽ
const BYTE MANUALMODE = 0;
const BYTE AUTOMODE = 1;

//��������ԭʼλ��ʱ�ľ���ֵ
const DWORD ORIGINAL_PHASE = 15749472L;

//��������ظ������������
const BYTE REPEAT_PHASE_COUNT = 3;
//�γ�50�غϹ���;ֵ��������岽��
const BYTE DRAWOF50TURNS_STEPCOUNT = 100;

//�Զ�����Ϣ��ʶ
const UINT WM_WRITECHESSMANUAL = WM_USER + 100;     //��¼����
const UINT WM_CLEARCHESSMANUAL = WM_USER + 101;     //�������

//�ýṹ��װ��"������λ"��ص���Ϣ
typedef struct _tagCastling
{
    WORD nKing_ShiftStep;               //"��"��û���ƶ����ƶ�ʱ�Ĳ���ֵ(����0��ʾ��û���ƶ���)
    WORD nRookOfKingSide_ShiftStep;     //"����"��û���ƶ����ƶ�ʱ�Ĳ���ֵ(����0��ʾ��û���ƶ���)
    WORD nRookOfQueenSide_ShiftStep;    //"����"��û���ƶ����ƶ�ʱ�Ĳ���ֵ(����0��ʾ��û���ƶ���)
} CTG, *PCTG;

//�ýṹ��װ��¼����ʱ��Ҫ����Ϣ
typedef struct _tagChessManual
{
    BYTE nAction;               //�û�����Ϊ
    BYTE nKingStatus;           //�Է�"��"��״̬
    BYTE nPawnUp;               //�Ƿ���"��������"(0��ʾû�з�����1��ʾ����)
    BYTE nCheckCount;           //"����"���ӵ�����
    CHESSMANTYPE chessManType;  //���ӵ�����(����)
    CString strSrcGridName;     //�������ڵ�Դ�������
    CString strDestGridName;    //���ӵ����Ŀ���������
} CM;

//�ýṹ��װ�û�����Ϊ
typedef struct _tagAction
{
    BYTE nAct;              //��Ϊ����
    CChessMan* pSrcMan;     //Դ����
    CChessGrid* pSrcGrid;   //Դ���
    CChessGrid* pDestGrid;  //Ŀ�����
} ACT;

//������Ϊ�ַ�����ӳ���ṹ
typedef struct _tagActionMap
{
    BYTE nAct;                  //��Ϊ����
    void (*pfn)(const ACT&);    //ָ����Ϊ��������ָ��
} ACTMAP;

//�ú����ڼ��������е�Ԫ�ظ�������DispatchAction��������ʹ��
#define DIM(x) (sizeof(x) / sizeof(x[0]))

class CChessRule
{
public:
    //����������򣬶��û��������̵���Ϊ������Ӧ�Ĵ���
    static void DoSomething(CChessGrid* const pDestGrid);

    //��ȡָ��������ĳһ���Ͽ��Ե���(����Կ���)�ķ������������������ļ���
    static void GetLegalGridName(const CHESSMANCOLOR chessManColor, 
                                 const CHESSMANTYPE chessManType, 
                                 const CString& strSrcGridName, 
                                 CStringList& strLstLegalGridName, 
                                 const BYTE nType = LG_ARRIVE);

    //�ж�һ��������ĳһ�����Ƿ����ܶԷ����ӵĹ���
    static BOOL IsAttacked(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName, 
                           CStringArray* pStrAryAttack = NULL);

    //�ж�һ����"��"�Ƿ����ܶԷ����ӵĹ���(Ҳ����"����")
    static BOOL IsKingChecked(const CHESSMANCOLOR KingColor);

    //�ж�һ��������ĳһ���Ƿ����ܶԷ����ӵ�ǣ��(���������޷��ƶ���������ɼ���"��"���ͳ�)
    static BOOL IsPinedDown(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName);

    //��ȡ"��"��״̬(������״̬��1��������2����"����"��3����"��ɱ"��4���;�)
    static BYTE GetKingStatus(const CHESSMANCOLOR KingColor, BYTE& nCheckedCount);

    //�ж��Ƿ��γɺ;�
    static BOOL IsDraw();

    //��ȡ���ӵĲ���
    static const WORD GetStepCount()
    {
        return m_nStepCount;
    }

    //�ж�����Ƿ��Ѿ���ʼ��(����TRUE��ʾ����Ѿ���ʼ������FALSE��ʾ�����δ��ʼ)
    static const BOOL IsStarted();

    //��ȡ�û�����Ϊģʽ
    static const BYTE GetUserMode()
    {
        return m_nUserMode;
    }

    //�����û�����Ϊģʽ
    static void SetUserMode(const BYTE nNewValue)
    {
        m_nUserMode = nNewValue;
    }

    //����
    static BOOL OnGameNew();

    //��������
    static void OnExchangeChessMan();

    //���������������Ϊ(����)
    static void Undo();

    //�������
    static void Save(CArchive& ar);

    //�����
    static BOOL Open(CArchive& ar);

    //��ȡ�����������Ϣ
    static const CM* GetLastCMInfo()
    {
        if (!m_CMList.IsEmpty())
        {
            return &m_CMList.GetTail();
        }

        return NULL;
    }

    //�������ӳ�ʱ�����
    static void ProcTimeOut(BOOL bWhichClockTimeOut);

    //��ߴ��ڶ���ָ��
    //���ڸ���ľ��󲿷ֺ�����ʹ���˸�ָ�룬
    //�����ⲿ������ʹ�ø���ǰ�������ʼ���þ�̬����
    static CChessEquipmentWnd* m_pChessEquipmentWnd;

    //������ͼ����ָ��
    //�����ڼ�¼����ʱ��Ҫʹ�ø�ָ�룬
    //�����ⲿ������ʹ�ø���ǰ�������ʼ���þ�̬����
    static CChessManualView* m_pChessManalView;

    //�ĵ�����ָ��
    //�����ڱ���ʹ�����ļ�ʱ��Ҫʹ�ø�ָ�룬
    //�����ⲿ������ʹ�ø���ǰ�������ʼ���þ�̬����
    static CIntChessDoc* m_pIntChessDoc;

    //�������ڶ���ָ��
    //�����ڼ�ʱ�ȷ�����Ҫʹ�ø�ָ�룬
    //�����ⲿ������ʹ�ø���ǰ�������ʼ���þ�̬����
    static CChessDeskWnd* m_pChessDeskWnd;

private:
    CChessRule();
    ~CChessRule();

    //�������ֵ
    static DWORD CalcPhaseValue();

    //Ԥ�����û�����Ϊ����Ϊ"������λ"��"�Թ�·��"
    //�ڹ�����������������߷���������Ҫ��ǰ�ж�
    static BOOL PreTranslateAction(ACT& act);

    //�����û�����Ϊ
    static void TranslateAction(ACT& act);

    //�ڷַ���Ϊ֮ǰ������صĴ���
    static void PreDispatchAction(ACT& act);

    //�ַ��û�����Ϊ
    static void DispatchAction(ACT& act);

    //�ڴ������û�����Ϊ������صĴ���
    static void AfterDispatchAction(const ACT& act);

    //��Ϊ������
    static void OnMove(const ACT&);
    static void OnKill(const ACT&);
    static void OnKillPassByPawn(const ACT&);
    static void OnKingRookShift(const ACT&);
    static void OnIllegal(const ACT&);

    //�ж��Ƿ��γ�"�Թ�·��"
    static BOOL IsKillPassByPawn(const CHESSMANCOLOR PawnColor, const CString& strSrcGridName, 
                                 const CString& strDestGridName);
    //�ж��Ƿ��γ�"������λ"
    static BOOL IsKingRookShift(const CHESSMANCOLOR KingColor, const CString& strDestGridName);

    //�ж��Ƿ��γ�"�ƺ�"
    static BOOL IsForceDraw();

    //�ж��Ƿ��γɾ���;�[��"�ٺ�"]
    static BOOL IsSituationDraw();

    //����"��"���߷��ϵ������ԣ����Խ���������Ϊһ��������ʵ��
    //���������GetLegalGridName�������
    static void KnightLegal(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName, 
                            CStringList& strLstLegalGridName, const BYTE nType = LG_ARRIVE);

    //��ȡ��ָ�����������߼���ϵ���������ļ���
    static void GetRelationGridName(const CHESSMANCOLOR chessManColor, 
                                    const CString& strSrcGridName, 
                                    CStringList& strLstGridName, 
                                    const BYTE nEndCondition = EC_EXCLUDE_SELF, 
                                    BYTE nRGType = RG_HLINE | RG_VLINE | RG_DLINE, 
                                    const int nDirection = ALL_DIRECT, 
                                    const BYTE nStep = NOLIMIT);

    //��ȡ��Դ����Ŀ�����֮��(��ָ���ߡ�ֱ�߻�б�����������е�һ��)���������ļ���
    static BYTE GetBetweenGridName(const CString& strSrcGridName, const CString& strDestGridName, 
                                   CStringList& strLstGridName);

    //��ʶ��ǰӦ������һ������(FALSE��ʾ�׷����ӣ�TRUE��ʾ�ڷ�����)
    static BOOL m_bWhichColorHasTurn;

    //�������һ���γ�"��·��"�����ı���λ��
    static CString m_strPassByPawnGridName;

    //��ʶ��(��)��"��"��"����"��"����"���ƶ����
    static CTG m_WhiteCTG;
    static CTG m_BlackCTG;

    //�������ӵĲ���(�����ǻغ������غ��� = ���� / 2)
    static WORD m_nStepCount;

    //��ʶ�Ƿ���"��������"(0��ʾû�з�����1��ʾ����)
    static BYTE m_nPawnUp;

    //�洢���������Ϣ�ļ���
    static CList<CM, CM> m_CMList;

    //��ʶ�û���ǰ����Ϊģʽ(1���ֶ�ģʽ[�û���������]��2���Զ�ģʽ[��ȡ����ļ�])
    static BYTE m_nUserMode;

    //��Ϊ�ַ�����ӳ���
    static ACTMAP _actionEntries[];
};

#endif // !defined(AFX_CHESSRULE_H__A899C08F_0F4B_4AF4_97FA_555CE0B99C4B__INCLUDED_)
