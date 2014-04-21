/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessRule.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-31
文件描述    : 声明走棋规则类，由于该类的公共函数全部是静态的，所以无需生成该类的对象即可
              直接使用该类(前提是，在使用之前，必须给成员变量
              m_pChessEquipmentWnd、m_pChessManalView、m_pIntChessDoc、m_pChessDeskWnd 赋上有效值)
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_CHESSRULE_H__A899C08F_0F4B_4AF4_97FA_555CE0B99C4B__INCLUDED_)
#define AFX_CHESSRULE_H__A899C08F_0F4B_4AF4_97FA_555CE0B99C4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessMan.h"   //棋子类(CChessMan)的头文件

class CChessEquipmentWnd;   //棋具窗口类(CChessEquipmentWnd)的前向声明
class CChessManualView;     //棋谱视图类(CChessManualView)的前向声明
class CIntChessDoc;         //文档类(CIntChessDoc)的前向声明
class CChessDeskWnd;        //棋桌窗口类(CChessDeskWnd)的前向声明

const BOOL WHITE_TURN = FALSE;  //由白方走子
const BOOL BLACK_TURN = TRUE;   //由黑方走子

//标识用户的行为类型
const BYTE TA_MOVE = 0x01;              //移动棋子
const BYTE TA_KILL = 0x02;              //吃掉棋子
const BYTE TA_KILLPASSBYPAWN = 0x03;    //吃过路兵
const BYTE TA_KINGROOKSHIFT = 0x04;     //王车易位
const BYTE TA_ILLEGAL = 0x08;           //无效的行为

const BYTE RG_HLINE = 0x01;     //横线逻辑关系标识符
const BYTE RG_VLINE = 0x02;     //直线逻辑关系标识符
const BYTE RG_DLINE = 0x04;     //斜线逻辑关系标识符
const BYTE RG_NONE = 0x00;      //无逻辑关系标识符

const int ADD_DIRECT = +1;                              //递增方向
const int SUBTRACT_DIRECT = -1;                         //递减方向
const int ALL_DIRECT = ADD_DIRECT + SUBTRACT_DIRECT;    //所有方向

const BYTE EC_EXCLUDE_SELF = 0x01;          //不包含己方的棋子
const BYTE EC_INCLUDE_SELF = 0x02;          //包含己方的棋子
const BYTE EC_MUST_HAVE_CHESSMAN = 0x04;    //棋格内必须要有棋子
const BYTE EC_MUST_EMPTY = 0x08;            //棋格内必须没有棋子

const BYTE NOLIMIT = 0;     //标识步长没有限制

const BYTE LG_ARRIVE = 0;   //可以到达的棋格
const BYTE LG_CONTROL = 1;  //可以控制的棋格

//"王"的状态
const BYTE KS_NORMAL = 0;       //正常
const BYTE KS_CHECKED = 1;      //被"将军"
const BYTE KS_CHECKMATED = 2;   //被"将杀"
const BYTE KS_DRAW = 3;         //和局

//标识兵升变
const BYTE PAWNUP = 1;

//标识用户的行为模式
const BYTE MANUALMODE = 0;
const BYTE AUTOMODE = 1;

//棋子尚在原始位置时的局面值
const DWORD ORIGINAL_PHASE = 15749472L;

//允许出现重复局面的最大次数
const BYTE REPEAT_PHASE_COUNT = 3;
//形成50回合规则和局的最少走棋步数
const BYTE DRAWOF50TURNS_STEPCOUNT = 100;

//自定义消息标识
const UINT WM_WRITECHESSMANUAL = WM_USER + 100;     //记录棋谱
const UINT WM_CLEARCHESSMANUAL = WM_USER + 101;     //清空棋谱

//该结构封装与"王车易位"相关的信息
typedef struct _tagCastling
{
    WORD nKing_ShiftStep;               //"王"从没有移动到移动时的步数值(等于0表示还没有移动过)
    WORD nRookOfKingSide_ShiftStep;     //"王翼车"从没有移动到移动时的步数值(等于0表示还没有移动过)
    WORD nRookOfQueenSide_ShiftStep;    //"后翼车"从没有移动到移动时的步数值(等于0表示还没有移动过)
} CTG, *PCTG;

//该结构封装记录棋谱时需要的信息
typedef struct _tagChessManual
{
    BYTE nAction;               //用户的行为
    BYTE nKingStatus;           //对方"王"的状态
    BYTE nPawnUp;               //是否发生"兵的升变"(0表示没有发生，1表示发生)
    BYTE nCheckCount;           //"将军"棋子的数量
    CHESSMANTYPE chessManType;  //棋子的类型(兵种)
    CString strSrcGridName;     //棋子所在的源棋格名称
    CString strDestGridName;    //棋子到达的目标棋格名称
} CM;

//该结构封装用户的行为
typedef struct _tagAction
{
    BYTE nAct;              //行为类型
    CChessMan* pSrcMan;     //源棋子
    CChessGrid* pSrcGrid;   //源棋格
    CChessGrid* pDestGrid;  //目标棋格
} ACT;

//定义行为分发机制映射表结构
typedef struct _tagActionMap
{
    BYTE nAct;                  //行为类型
    void (*pfn)(const ACT&);    //指向行为处理函数的指针
} ACTMAP;

//该宏用于计算数组中的元素个数，在DispatchAction函数中有使用
#define DIM(x) (sizeof(x) / sizeof(x[0]))

class CChessRule
{
public:
    //根据走棋规则，对用户单击棋盘的行为做出相应的处理
    static void DoSomething(CChessGrid* const pDestGrid);

    //获取指定棋子在某一格上可以到达(或可以控制)的符合走棋规则的所有棋格的集合
    static void GetLegalGridName(const CHESSMANCOLOR chessManColor, 
                                 const CHESSMANTYPE chessManType, 
                                 const CString& strSrcGridName, 
                                 CStringList& strLstLegalGridName, 
                                 const BYTE nType = LG_ARRIVE);

    //判断一方棋子在某一格上是否遭受对方棋子的攻击
    static BOOL IsAttacked(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName, 
                           CStringArray* pStrAryAttack = NULL);

    //判断一方的"王"是否遭受对方棋子的攻击(也就是"将军")
    static BOOL IsKingChecked(const CHESSMANCOLOR KingColor);

    //判断一方棋子在某一格是否遭受对方棋子的牵制(即该棋子无法移动，否则将造成己方"王"被送吃)
    static BOOL IsPinedDown(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName);

    //获取"王"的状态(有四种状态：1、正常；2、被"将军"；3、被"将杀"；4、和局)
    static BYTE GetKingStatus(const CHESSMANCOLOR KingColor, BYTE& nCheckedCount);

    //判断是否形成和局
    static BOOL IsDraw();

    //获取走子的步数
    static const WORD GetStepCount()
    {
        return m_nStepCount;
    }

    //判断棋局是否已经开始了(返回TRUE表示棋局已经开始，返回FALSE表示棋局尚未开始)
    static const BOOL IsStarted();

    //获取用户的行为模式
    static const BYTE GetUserMode()
    {
        return m_nUserMode;
    }

    //设置用户的行为模式
    static void SetUserMode(const BYTE nNewValue)
    {
        m_nUserMode = nNewValue;
    }

    //开局
    static BOOL OnGameNew();

    //交换棋子
    static void OnExchangeChessMan();

    //撤消最近的走棋行为(悔棋)
    static void Undo();

    //保存棋局
    static void Save(CArchive& ar);

    //打开棋局
    static BOOL Open(CArchive& ar);

    //获取最近的棋谱信息
    static const CM* GetLastCMInfo()
    {
        if (!m_CMList.IsEmpty())
        {
            return &m_CMList.GetTail();
        }

        return NULL;
    }

    //处理棋钟超时的情况
    static void ProcTimeOut(BOOL bWhichClockTimeOut);

    //棋具窗口对象指针
    //由于该类的绝大部分函数都使用了该指针，
    //所以外部对象在使用该类前，必须初始化该静态变量
    static CChessEquipmentWnd* m_pChessEquipmentWnd;

    //棋谱视图对象指针
    //由于在记录棋谱时需要使用该指针，
    //所以外部对象在使用该类前，必须初始化该静态变量
    static CChessManualView* m_pChessManalView;

    //文档对象指针
    //由于在保存和打开棋局文件时需要使用该指针，
    //所以外部对象在使用该类前，必须初始化该静态变量
    static CIntChessDoc* m_pIntChessDoc;

    //棋桌窗口对象指针
    //由于在计时等方面需要使用该指针，
    //所以外部对象在使用该类前，必须初始化该静态变量
    static CChessDeskWnd* m_pChessDeskWnd;

private:
    CChessRule();
    ~CChessRule();

    //计算局面值
    static DWORD CalcPhaseValue();

    //预解析用户的行为，因为"王车易位"和"吃过路兵"
    //在国际象棋中是特殊的走法，所以需要提前判断
    static BOOL PreTranslateAction(ACT& act);

    //解析用户的行为
    static void TranslateAction(ACT& act);

    //在分发行为之前，做相关的处理
    static void PreDispatchAction(ACT& act);

    //分发用户的行为
    static void DispatchAction(ACT& act);

    //在处理完用户的行为后，做相关的处理
    static void AfterDispatchAction(const ACT& act);

    //行为处理函数
    static void OnMove(const ACT&);
    static void OnKill(const ACT&);
    static void OnKillPassByPawn(const ACT&);
    static void OnKingRookShift(const ACT&);
    static void OnIllegal(const ACT&);

    //判断是否形成"吃过路兵"
    static BOOL IsKillPassByPawn(const CHESSMANCOLOR PawnColor, const CString& strSrcGridName, 
                                 const CString& strDestGridName);
    //判断是否形成"王车易位"
    static BOOL IsKingRookShift(const CHESSMANCOLOR KingColor, const CString& strDestGridName);

    //判断是否形成"逼和"
    static BOOL IsForceDraw();

    //判断是否形成局面和局[即"官和"]
    static BOOL IsSituationDraw();

    //由于"马"在走法上的特殊性，所以将它单独作为一个函数来实现
    //这个函数由GetLegalGridName负责调用
    static void KnightLegal(const CHESSMANCOLOR chessManColor, const CString& strSrcGridName, 
                            CStringList& strLstLegalGridName, const BYTE nType = LG_ARRIVE);

    //获取与指定棋格有相关逻辑关系的所有棋格的集合
    static void GetRelationGridName(const CHESSMANCOLOR chessManColor, 
                                    const CString& strSrcGridName, 
                                    CStringList& strLstGridName, 
                                    const BYTE nEndCondition = EC_EXCLUDE_SELF, 
                                    BYTE nRGType = RG_HLINE | RG_VLINE | RG_DLINE, 
                                    const int nDirection = ALL_DIRECT, 
                                    const BYTE nStep = NOLIMIT);

    //获取在源棋格和目标棋格之间(仅指横线、直线或斜线三个方向中的一个)的所有棋格的集合
    static BYTE GetBetweenGridName(const CString& strSrcGridName, const CString& strDestGridName, 
                                   CStringList& strLstGridName);

    //标识当前应该由哪一方走子(FALSE表示白方走子，TRUE表示黑方走子)
    static BOOL m_bWhichColorHasTurn;

    //保存最近一次形成"过路兵"条件的兵的位置
    static CString m_strPassByPawnGridName;

    //标识白(黑)方"王"、"王翼车"、"后翼车"的移动情况
    static CTG m_WhiteCTG;
    static CTG m_BlackCTG;

    //保存走子的步数(并不是回合数，回合数 = 步数 / 2)
    static WORD m_nStepCount;

    //标识是否发生"兵的升变"(0表示没有发生，1表示发生)
    static BYTE m_nPawnUp;

    //存储走棋相关信息的集合
    static CList<CM, CM> m_CMList;

    //标识用户当前的行为模式(1、手动模式[用户参与下棋]；2、自动模式[读取棋局文件])
    static BYTE m_nUserMode;

    //行为分发机制映射表
    static ACTMAP _actionEntries[];
};

#endif // !defined(AFX_CHESSRULE_H__A899C08F_0F4B_4AF4_97FA_555CE0B99C4B__INCLUDED_)
