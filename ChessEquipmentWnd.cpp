/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessEquipmentWnd.cpp
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
#include "ChessEquipmentWnd.h"

#include "ChessRule.h"  //���������(CChessRule)��ͷ�ļ�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChessEquipmentWnd* CChessEquipmentWnd::m_pObject = NULL;

/******************************************************************************************
CChessEquipmentWnd::CChessEquipmentWnd()
����      : tangjs520       ��������: 2003-10-21
������    : CChessEquipmentWnd::CChessEquipmentWnd
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-10-21
������    : CChessEquipmentWnd::~CChessEquipmentWnd
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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

    //ж��ȫ�������ڴ�DC
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
����      : tangjs520       ��������: 2003-10-21
������    : CChessEquipmentWnd::ExchangeChessMan
����ֵ    : void
�����б�  : ��
����      : ��������
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessEquipmentWnd::ExchangeChessMan()
{
    LockWindowUpdate();

    //��������涨"˫�����������ͬһ��ֱ������ԣ��׺�λ�ڰ׸񣬺ں�λ�ںڸ�"
    //���Խ�������ʱ���ס���˫����"������"Ҳ���뻥�ཻ��λ��
    m_pAryWhiteChessMan[Queen]->SetType(KING, FALSE);
    m_pAryWhiteChessMan[King]->SetType(QUEEN, FALSE);
    
    m_pAryBlackChessMan[Queen]->SetType(KING, FALSE);
    m_pAryBlackChessMan[King]->SetType(QUEEN, FALSE);

    for (register int i = 0; i < 16; i++)
    {
        m_pAryWhiteChessMan[i]->ExchangeColor();
        m_pAryBlackChessMan[i]->ExchangeColor();
    }

    //��������ҲҪ����Ӧ�ı�
    CCoordinateWnd::SetWhiteBelowOrBlackBelow(!CCoordinateWnd::GetWhiteBelowOrBlackBelow());
    for (int i = 0; i < 2; i++)
    {
        m_pCoordinateWnd[i]->Invalidate();
    }

    UnlockWindowUpdate();

    //�������������Ѿ��ı��ˣ�����Ҳ��������������
    m_pChessBoard->ExchangeChessGridName();

    //Ϊ�˱�֤��������[White��Black]����������[Queen��King]�������ϵ���ȷ�ԣ�
    //��������Ķ�ӦԪ��Ҳ���ཻ��
    CChessMan* pTemp = NULL;
    pTemp = m_pAryWhiteChessMan[Queen];
    m_pAryWhiteChessMan[Queen] = m_pAryWhiteChessMan[King];
    m_pAryWhiteChessMan[King] = pTemp;

    pTemp = m_pAryBlackChessMan[Queen];
    m_pAryBlackChessMan[Queen] = m_pAryBlackChessMan[King];
    m_pAryBlackChessMan[King] = pTemp;

    //Ϊ����"������λ"��׼ȷ���ж�"����"��"����"��λ�ã�
    //˫���Լ���������Ҳ���뻥�ཻ��λ��(����������жϵģ�
    //��μ�CChessRule::AfterDispatchAction����)
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
����      : tangjs520       ��������: 2003-11-7
������    : CChessEquipmentWnd::Renew
����ֵ    : void
�����б�  : ��
����      : ���¿�ʼ��Ϸ
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessEquipmentWnd::Renew()
{
    //�ָ����ӵ�ԭʼ���
    CString strH = _T("");  //���������ƣ����ں�������ϳ��������

    LockWindowUpdate();

    //�������λ��ָʾ��
    RemoveIndicator();

    //�ױ��ĳ�ʼλ����"a2��b2��c2��d2��e2��f2��g2��h2"
    //�ڱ��ĳ�ʼλ����"a7��b7��c7��d7��e7��f7��g7��h7"
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

    //����ĳ�ʼλ����"c1��f1"
    //����ĳ�ʼλ����"c8��f8"
    for (int i = Bishop, HName = _T('c'); i < Bishop + 2; i++, HName += 3)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //����ĳ�ʼλ����"b1��g1"
    //����ĳ�ʼλ����"b8��g8"
    for (int i = Knight, HName = _T('b'); i < Knight + 2; i++, HName += 5)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //�׳��ĳ�ʼλ����"a1��h1"
    //�ڳ��ĳ�ʼλ����"a8��h8"
    for (int i = Rook, HName = _T('a'); i < Rook + 2; i++, HName += 7)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i]->SetPosition(m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //�׺�ĳ�ʼλ����"d1"
    //�ں�ĳ�ʼλ����"d8"
    m_pAryWhiteChessMan[Queen]->SetPosition(m_pChessBoard->GetGridFromName(_T("d1")));
    m_pAryBlackChessMan[Queen]->SetPosition(m_pChessBoard->GetGridFromName(_T("d8")));
    
    //�����ĳ�ʼλ����"e1"
    //�����ĳ�ʼλ����"e8"
    m_pAryWhiteChessMan[King]->SetPosition(m_pChessBoard->GetGridFromName(_T("e1")));
    m_pAryBlackChessMan[King]->SetPosition(m_pChessBoard->GetGridFromName(_T("e8")));

    UnlockWindowUpdate();

    //���ӵĳ�ʼ״̬��û�гԵ��κ��������ӵ�
    for (int i = 0; i < 16; i++)
    {
        m_pAryWhiteChessMan[i]->RemoveKilledMan(RMK_ALL);
        m_pAryBlackChessMan[i]->RemoveKilledMan(RMK_ALL);
    }
}

/******************************************************************************************
void CChessEquipmentWnd::DrawIndicator(const CRect& rectSrcPos, const CRect& rectDestPos)
����      : tangjs520       ��������: 2003-11-9
������    : CChessEquipmentWnd::DrawIndicator
����ֵ    : void
�����б�  : 
  ����1: const CRect& rectSrcPos
  ����2: const CRect& rectDestPos
����      : ��������λ��ָʾ��
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessEquipmentWnd::DrawIndicator(const CRect& rectSrcPos, const CRect& rectDestPos)
{
    //������Ҫ������Դλ��ָʾ�����Ŀ��λ��ָʾ������꣬
    //��Ϊ�Ժ�Ҫ��������
    CRect rectOldSrcPos = m_rectSrcPosIndicator;
    CRect rectOldDestPos = m_rectDestPos;

    //λ��ָʾ��Ŀ�Ⱥ͸߶ȱ����Ҫ��Сһ������
    m_rectSrcPosIndicator = rectSrcPos;
    m_rectSrcPosIndicator.DeflateRect(1, 1);

    m_rectDestPosIndicator = m_rectDestPos = rectDestPos;
    m_rectDestPos.DeflateRect(1, 1);
    m_rectDestPosIndicator.DeflateRect(1, 1);

    //������Դλ��ָʾ��
    if (!rectOldSrcPos.IsRectEmpty())
    {
        //������Ҫ�����������С
        CRgn rgnOutside;
        rgnOutside.CreateRectRgnIndirect(rectOldSrcPos);

        CRect rectTemp = rectOldSrcPos;
        rectTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

        CRgn rgnInside;
        rgnInside.CreateRectRgnIndirect(rectTemp);

        rgnOutside.CombineRgn(&rgnOutside, &rgnInside, RGN_DIFF);

        InvalidateRgn(&rgnOutside);
    }

    //������Ŀ��λ��ָʾ��
    if (!rectOldDestPos.IsRectEmpty())
    {
        //�رն�ʱ��(���ȱ���ֹͣ����Ŀ��λ��ָʾ�����˸)
        ::KillTimer(m_hWnd, INDICATOR_FLASH_EVENT);

        //������Ҫ�����������С
        CRgn rgnOutside;
        rgnOutside.CreateRectRgnIndirect(rectOldDestPos);

        CRect rectTemp = rectOldDestPos;
        rectTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

        CRgn rgnInside;
        rgnInside.CreateRectRgnIndirect(rectTemp);

        rgnOutside.CombineRgn(&rgnOutside, &rgnInside, RGN_DIFF);

        InvalidateRgn(&rgnOutside);
    }

    //������Դλ��ָʾ��
    InvalidateRect(m_rectSrcPosIndicator, FALSE);

    //������Ŀ��λ��ָʾ��
    CClientDC dc(this);
    CPen IndicatorPen(PS_INSIDEFRAME, INDICATOR_WIDTH, INDICATOR_COLOR);
    CPen* pOldPen = dc.SelectObject(&IndicatorPen);
    dc.SelectStockObject(NULL_BRUSH);
    dc.Rectangle(m_rectDestPosIndicator);
    dc.SelectObject(pOldPen);

    //������Ŀ��λ��ָʾ��ˢ������Ĵ�С���ڶ�ʱ����ʹ��
    m_rgnInvalidate.DeleteObject();
    m_rgnInvalidate.CreateRectRgnIndirect(m_rectDestPosIndicator);

    CRect rectDestTemp = m_rectDestPosIndicator;
    rectDestTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

    CRgn rgnDestInside;
    rgnDestInside.CreateRectRgnIndirect(rectDestTemp);

    m_rgnInvalidate.CombineRgn(&m_rgnInvalidate, &rgnDestInside, RGN_DIFF);

    //������ʱ��(���ڵȴ�INDICATOR_FLASH_SPEED����󣬿�ʼ��Ŀ��λ��ָʾ�����˸)
    ::SetTimer(m_hWnd, INDICATOR_FLASH_EVENT, INDICATOR_FLASH_SPEED, 
               (TIMERPROC)IndicatorFlashingProc);
}

/******************************************************************************************
void CALLBACK CChessEquipmentWnd::IndicatorFlashingProc(HWND, UINT, UINT_PTR, DWORD)
����      : tangjs520       ��������: 2003-11-9
������    : CChessEquipmentWnd::IndicatorFlashingProc
����ֵ    : void CALLBACK
�����б�  : 
  ����1: HWND
  ����2: UINT
  ����3: UINT_PTR
  ����4: DWORD
����      : ��������λ��ָʾ�����˸����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CALLBACK CChessEquipmentWnd::IndicatorFlashingProc(HWND, UINT, UINT_PTR, DWORD)
{
    BOOL bErase = FALSE;

    //Ŀ��λ�ñ���m_rectDestPosIndicator���ÿյ���ֵ���ر仯��
    //�Ӷ�������˸Ч��
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
����      : tangjs520       ��������: 2003-10-21
������    : CChessEquipmentWnd::OnCreate
����ֵ    : int
�����б�  : 
  ����1: LPCREATESTRUCT lpCreateStruct
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
int CChessEquipmentWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    //��������
    m_pChessBoard = new CChessBoard(this);
    ASSERT(m_pChessBoard);

    //�������괰��
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

    //�ڴ�������ǰ�������ȱ�֤��������λͼ����Ӧ��ȫ���ڴ�DC��
    CIntChessApp::InstallChessManDC(this);

    //��������
    CString strH = _T("");  //���������ƣ����ں�������ϳ��������

    //�ױ��ĳ�ʼλ����"a2��b2��c2��d2��e2��f2��g2��h2"
    //�ڱ��ĳ�ʼλ����"a7��b7��c7��d7��e7��f7��g7��h7"
    for (BYTE i = Pawn, HName = _T('a'); i < Pawn + 8; i++, HName++)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i] = new CChessMan(CHESSMAN_WHITE, PAWN, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("2")));
        m_pAryBlackChessMan[i] = new CChessMan(CHESSMAN_BLACK, PAWN, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("7")));
    }

    //����ĳ�ʼλ����"c1��f1"
    //����ĳ�ʼλ����"c8��f8"
    for (int i = Bishop, HName = _T('c'); i < Bishop + 2; i++, HName += 3)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i] = new CChessMan(CHESSMAN_WHITE, BISHOP, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i] = new CChessMan(CHESSMAN_BLACK, BISHOP, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //����ĳ�ʼλ����"b1��g1"
    //����ĳ�ʼλ����"b8��g8"
    for (int i = Knight, HName = _T('b'); i < Knight + 2; i++, HName += 5)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i] = new CChessMan(CHESSMAN_WHITE, KNIGHT, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i] = new CChessMan(CHESSMAN_BLACK, KNIGHT, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //�׳��ĳ�ʼλ����"a1��h1"
    //�ڳ��ĳ�ʼλ����"a8��h8"
    for (int i = Rook, HName = _T('a'); i < Rook + 2; i++, HName += 7)
    {
        strH = CString(static_cast<TCHAR>(HName));
        m_pAryWhiteChessMan[i] = new CChessMan(CHESSMAN_WHITE, ROOK, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("1")));
        m_pAryBlackChessMan[i] = new CChessMan(CHESSMAN_BLACK, ROOK, this, 
                                               m_pChessBoard->GetGridFromName(strH + _T("8")));
    }

    //�׺�ĳ�ʼλ����"d1"
    //�ں�ĳ�ʼλ����"d8"
    m_pAryWhiteChessMan[Queen] = new CChessMan(CHESSMAN_WHITE, QUEEN, this, 
                                               m_pChessBoard->GetGridFromName(_T("d1")));
    m_pAryBlackChessMan[Queen] = new CChessMan(CHESSMAN_BLACK, QUEEN, this, 
                                               m_pChessBoard->GetGridFromName(_T("d8")));
    
    //�����ĳ�ʼλ����"e1"
    //�����ĳ�ʼλ����"e8"
    m_pAryWhiteChessMan[King] = new CChessMan(CHESSMAN_WHITE, KING, this, 
                                              m_pChessBoard->GetGridFromName(_T("e1")));
    m_pAryBlackChessMan[King] = new CChessMan(CHESSMAN_BLACK, KING, this, 
                                              m_pChessBoard->GetGridFromName(_T("e8")));

    //��ʹ�����������֮ǰ�������ʼ���þ�̬����
    CChessRule::m_pChessEquipmentWnd = m_pObject = this;

    return 0;
}

/******************************************************************************************
BOOL CChessEquipmentWnd::OnEraseBkgnd(CDC* pDC)
����      : tangjs520       ��������: 2003-10-21
������    : CChessEquipmentWnd::OnEraseBkgnd
����ֵ    : BOOL
�����б�  : 
  ����1: CDC* pDC
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CChessEquipmentWnd::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pChessBoard)
    {
        //��������
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
����      : tangjs520       ��������: 2003-10-28
������    : CChessEquipmentWnd::OnPaint
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessEquipmentWnd::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    
    // TODO: Add your message handler code here
    for (register int i = 0; i < 16; i++)
    {
        //���ư���
        if (m_pAryWhiteChessMan[i])
        {
            m_pAryWhiteChessMan[i]->Draw(&dc);
        }

        //���ƺ���
        if (m_pAryBlackChessMan[i])
        {
            m_pAryBlackChessMan[i]->Draw(&dc);
        }
    }

    //��������Դλ�ú�Ŀ��λ��ָʾ��
    if (!m_rectSrcPosIndicator.IsRectEmpty())
    {
        CPen IndicatorPen(PS_INSIDEFRAME, INDICATOR_WIDTH, INDICATOR_COLOR);
        CPen* pOldPen = dc.SelectObject(&IndicatorPen);
        dc.SelectStockObject(NULL_BRUSH);

        dc.Rectangle(m_rectSrcPosIndicator);
        //Ŀ��λ�ñ���m_rectDestPosIndicator�ڶ�ʱ���ڴ��ÿյ���ֵ���ر仯��
        //�Ӷ�������˸Ч��
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
����      : tangjs520       ��������: 2003-10-21
������    : CChessEquipmentWnd::OnLButtonDown
����ֵ    : void
�����б�  : 
  ����1: UINT nFlags
  ����2: CPoint point
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
            //�������������(CChessRule)������������������û�����Ϊ
            CChessRule::DoSomething(pGrid);
        }
    }

    CWnd::OnLButtonDown(nFlags, point);
}
