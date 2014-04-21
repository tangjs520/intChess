/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessManualView.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2003-11-21
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

#include "ChessRule.h"  //���������(CChessRule)��ͷ�ļ�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CChessManualView, CView)

/******************************************************************************************
CChessManualView::CChessManualView()
����      : tangjs520       ��������: 2003-11-22
������    : CChessManualView::CChessManualView
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CChessManualView::CChessManualView()
{
    m_pBkgndBmp = NULL;
    //�洢��ԭʼ������ֵĳ�ʼ����
    m_PhaseMap.SetAt(ORIGINAL_PHASE, (BYTE)1);
}

/******************************************************************************************
CChessManualView::~CChessManualView()
����      : tangjs520       ��������: 2003-11-22
������    : CChessManualView::~CChessManualView
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2004-9-29
������    : CChessManualView::OnDraw
����ֵ    : void
�����б�  : 
  ����1: CDC* pDC
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-11-22
������    : CChessManualView::OnEraseBkgnd
����ֵ    : BOOL
�����б�  : 
  ����1: CDC* pDC
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-11-22
������    : CChessManualView::OnCreate
����ֵ    : int
�����б�  : 
  ����1: LPCREATESTRUCT lpCreateStruct
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
int CChessManualView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
    bResult = CIntChessApp::SetBkgndBmp(IDB_WOOD, m_pBkgndBmp);
    if (!bResult)
    {
        TRACE0("Failed to create bitmap background\n");
        return -1;
    }

    //�����б��ؼ�ʹ�õ��������
    //ʹ������9������
    CDC* pDC = GetDC();
    ASSERT_VALID(pDC);
    m_ListBoxFont.CreateFont(MulDiv(9, -pDC->GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, 
                             FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                             DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("����"));
    ReleaseDC(pDC);

    //�����б��ؼ�
    m_ChessManualListBox.CreateEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR 
                                  | WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE, _T("ListBox"), NULL, 
                                  WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT, 
                                  CRect(CPoint(80, 10), CSize(115, 124)), this, 0);
    m_ChessManualListBox.SetFont(&m_ListBoxFont, FALSE);

    //����"����"��ť(����Ŀǰ��û�п�ʼ���壬���Ըð�ť�ĳ�ʼ״̬�ǽ�ֹʹ��״̬)
    m_btnUndo.Create(_T("����"), BS_PUSHBUTTON | WS_BORDER | WS_CHILD | WS_VISIBLE | WS_DISABLED, 
                     CRect(CPoint(10, 111), CSize(60, 23)), this, IDC_UNDO);
    m_btnUndo.SetFont(&m_ListBoxFont, FALSE);

    //��ʹ�����������֮ǰ�������ʼ���þ�̬����
    CChessRule::m_pChessManalView = this;

    return 0;
}

/******************************************************************************************
LRESULT CChessManualView::OnWriteChessManual(WPARAM wParam, LPARAM lParam)
����      : tangjs520       ��������: 2003-11-22
������    : CChessManualView::OnWriteChessManual
����ֵ    : LRESULT(����1��ʾ�γɺ;�)
�����б�  : 
  ����1: WPARAM wParam
  ����2: LPARAM lParam
����      : ��¼���ף�ͬʱ�ж��Ƿ��γ�"�����ظ�����"��"50�غϹ���"�;�
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
    //������������������ʾ�ڼ�¼����ʱ��Ҫ���ϻغ���
    if (nStepCount % 2)
    {
        strBoutCount.Format(_T("%3u: "), nStepCount / 2 + 1);
    }
    //������Ҫ��ǰ���������ɸ��ո�(�ո�������������һ�λغ����ַ����ĳ���)
    else
    {
        strBoutCount = CString(_T(' '), strBoutCount.GetLength());
    }

    //���׵�ͷ������Ҫ�������ӵ�����(����)
    //���ǣ�������ӵ�������"��"�������û�����Ϊ��"������λ"������"��������"��
    //���ü�¼���׵�ͷ����
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

    //�����û��Ĳ�ͬ��Ϊ��ѡ����Ӧ����Ϊ����
    CString strAction = _T("");
    switch (pCMInfo->nAction)
    {
    case TA_MOVE:
        strAction = _T("��");
        //�����Ѿ��߶���"��"�����Դ˲���֮ǰ�ľ����ǲ������γ�50�غϹ���ĺ;�������
        //�洢��ǰ�Ĳ���ֵ
        if (PAWN == pCMInfo->chessManType || PAWNUP == pCMInfo->nPawnUp)
        {
            m_nNoDrawOf50TurnsList.AddTail(nStepCount);
        }
        break;
    case TA_KILL:
    case TA_KILLPASSBYPAWN:
        strAction = _T("��");
        //�����Ѿ����ӣ����Դ˲���֮ǰ�ľ����ǲ������γ�50�غϹ���ĺ;�������
        //�洢��ǰ�Ĳ���ֵ
        m_nNoDrawOf50TurnsList.AddTail(nStepCount);
    default:
        break;
    }

    CString strPawnUpType = _T("");
    //�����"��������"����Ҫ��Ŀ��������ƺ�����������Ժ����������(����)
    if (PAWNUP == pCMInfo->nPawnUp)
    {
        strPawnUpType = g_strChessManName[pCMInfo->chessManType];
    }

    //���ݶԷ�"��"�Ĳ�ͬ״̬��ѡ����Ӧ�Ľ�������
    CString strEnd = _T("");
    switch (pCMInfo->nKingStatus)
    {
    case KS_CHECKED:
        strEnd = CString(_T('+'), pCMInfo->nCheckCount);
        break;
    case KS_CHECKMATED:
        strEnd = _T("��");
        break;
    default:
        break;
    }

    //��������ʾ���յ������ַ���
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
        //����λ
        if (_T('g') == ch)
        {
            strResult = strHeader + _T("0��0") + strEnd;
        }
        //����λ
        else
        {
            strResult = strHeader + _T("0��0��0") + strEnd;
        }
    }
    m_ChessManualListBox.AddString(strResult);

    //�б�����ݵ�����ֵ
    int nIndex = nStepCount - 1;

    //����ǰ����ֵ���б�����Ӧ�����ַ�����������
    DWORD dwPhaseValue = (DWORD)lParam;
    m_ChessManualListBox.SetItemData(nIndex, dwPhaseValue);
    //�ж��Ƿ�����������ظ��ľ���
    //��������涨�����ֻ�Ҫ���ֵ�����(�����������ظ�����ǰ���ۼ��γɵ�����)
    //�ظ���ͬ�ľ��棬ÿ�ζ����ֵ�ͬһ������ʱ�����ߵ�һ�����������к͡�
    //�������壬Ҳ�ɰ������ظ������к͡�
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

        //��������涨���ס���˫������һ�ų���һ���غϡ�������������50�غ��У�
        //˫����δ�Թ�һ�ӣ�Ҳδ�߶���һ�����������к͡�
        //�ж��Ƿ����50�غϹ���;�
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

    //��¼�������
    switch (pCMInfo->nKingStatus)
    {
    case KS_CHECKMATED:
        if (nStepCount % 2)
        {
            //�׷�ʤ
            m_ChessManualListBox.AddString(_T("     1 �� 0"));
        }
        else
        {
            //�ڷ�ʤ
            m_ChessManualListBox.AddString(_T("     0 �� 1"));
        }
        nIndex++;
        break;
    case KS_DRAW:
        //�;�
        m_ChessManualListBox.AddString(_T("   1/2 �� 1/2"));
        nIndex++;
        break;
    default:
        break;
    }

    //ѡ�����һ���б���
    m_ChessManualListBox.SetCurSel(nIndex);

    return dwRet;
}

/******************************************************************************************
LRESULT CChessManualView::OnEmptyChessManual(WPARAM, LPARAM)
����      : tangjs520       ��������: 2003-11-22
������    : CChessManualView::OnEmptyChessManual
����ֵ    : LRESULT
�����б�  : 
  ����1: WPARAM
  ����2: LPARAM
����      : ��������Լ���������صĳ�Ա����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
LRESULT CChessManualView::OnEmptyChessManual(WPARAM, LPARAM)
{
    m_ChessManualListBox.ResetContent();
    m_ChessManualListBox.UpdateWindow();

    m_nNoDrawOf50TurnsList.RemoveAll();

    m_PhaseMap.RemoveAll();
    //�洢��ԭʼ������ֵĳ�ʼ����
    m_PhaseMap.SetAt(ORIGINAL_PHASE, (BYTE)1);

    //��ֹ"����"��ť��ʹ��
    m_btnUndo.EnableWindow(FALSE);

    return 0L;
}

/******************************************************************************************
void CChessManualView::OnUndoBtnClicked()
����      : tangjs520       ��������: 2003-11-22
������    : CChessManualView::OnUndoBtnClicked
����ֵ    : void
�����б�  : ��
����      : ����"����"��ť��������Ϣ��Ӧ����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessManualView::OnUndoBtnClicked()
{
    if (AUTOMODE == CChessRule::GetUserMode())
    {
        return;
    }

    int nLastIndex = m_ChessManualListBox.GetCount() - 1;

    //��ȡ���һ���������γɵľ���ֵ
    DWORD dwLastPhaseValue = m_ChessManualListBox.GetItemData(nLastIndex);
    //��m_PhaseMap�����и�����������ֵ���ֵĴ��������þ���ֵֻ���ֹ�һ�Σ���ɾ���˾���ֵ
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

    //ɾ�����һ�����ӵ�����
    m_ChessManualListBox.DeleteString(nLastIndex);
    //ѡ�е�ǰ���һ���б���
    m_ChessManualListBox.SetCurSel(nLastIndex - 1);

    //���¼���50�غ�ʱ����ʼ����
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

    //����Ѿ��ڵ���һ�����ˣ����ֹ"����"��ť�ļ���ʹ��
    if (!nLastIndex)
    {
        m_btnUndo.EnableWindow(FALSE);
    }

    //�������������(CChessRule)��������"����"���������
    CChessRule::Undo();
}
