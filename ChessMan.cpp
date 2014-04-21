/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessMan.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-6
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
#include "ChessMan.h"

#include "ChessEquipmentWnd.h"  //��ߴ�����(CChessEquipmentWnd)��ͷ�ļ�

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//��ʼ����Ĺ�������
BYTE CChessMan::g_nMoveCount = 0;
BYTE CChessMan::g_nMoveSpeed = INITIAL_MOVESPEED;

CChessMan* CChessMan::g_pActiveChessMan = NULL;
int CChessMan::g_nBeforeZoomXPos = 0;
int CChessMan::g_nBeforeZoomYPos = 0;

/******************************************************************************************
CChessMan::CChessMan(const CHESSMANCOLOR color, const CHESSMANTYPE type, 
                     CWnd* pParentWnd, CChessGrid* pChessGrid)
����      : tangjs520       ��������: 2003-10-6
������    : CChessMan::CChessMan
����ֵ    : ��
�����б�  : 
  ����1: const CHESSMANCOLOR color
  ����2: const CHESSMANTYPE type
  ����3: CWnd* pParentWnd
  ����4: CChessGrid* pChessGrid
����      : ���캯������Ҫ�����ʼ�����ӵ��ڴ�DC(m_pBufferDC)��
            ��������λͼ���ڴ�DC(m_pMaskDC)[������ϵͳ��Win9x��WinNTʱ]
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CChessMan::CChessMan(const CHESSMANCOLOR color, const CHESSMANTYPE type, 
                     CWnd* pParentWnd, CChessGrid* pChessGrid/* = NULL*/)
{
    ASSERT_VALID(pParentWnd);
    m_pParentWnd = pParentWnd;

    //֮����Ҫ��ȡ�����ڵ�DC����Ϊ���Ժ��ܴ����븸���ڼ��ݵ��ڴ�DC
    CDC* pDC = m_pParentWnd->GetDC();
    ASSERT_VALID(pDC);

    //��ʼ��������ݳ�ԱΪ�գ���Ϊ�˵�����ϵͳ����Win9x��WinNTʱ����֤���ݳ�Ա�İ�ȫ��
    m_pMaskDC = NULL;

    m_bVisible = TRUE;  //���ӵĳ�ʼ״̬�ǿɼ���

    m_bZooming = FALSE; //���ӵĳ�ʼ״̬�ǲ��������Ų�����

    m_enuColor = color;
    m_enuType = type;

    m_pInsideChessGrid = pChessGrid;
    //���ʹ�����������������Ӷ�����ʹ���������������Գ�ʼ������λ�õĺᡢ�����꣬
    //ͬʱ��������������ݳ�Աm_pCurrentChessMan
    if (m_pInsideChessGrid)
    {
        CRect rectGrid = m_pInsideChessGrid->GetRect();
        m_nXPosition = rectGrid.left;
        m_nYPosition = rectGrid.top;

        m_pInsideChessGrid->SetIncludeChessMan(this);
    }
    //�����ʼ������λ�õĺᡢ�������Ϊ0���Ժ��ͨ������SetPosition��������������������
    else
    {
        m_nXPosition = 0;
        m_nYPosition = 0;
    }

    m_nAddWidth = INITIAL_ADDWIDTH;
    m_nAddHeight = INITIAL_ADDHEIGHT;

    //�������ӵ��ڴ�DC(���Ŀ�Ⱥ͸߶ȷֱ��������λͼ�Ŀ�Ⱥ͸߶ȼ��ϸ��ԵķŴ���ֵ)
    m_pBufferDC = new CDC;
    ASSERT_VALID(m_pBufferDC);

    m_pBufferDC->CreateCompatibleDC(pDC);

    CBitmap l_BufferBitmap;
    l_BufferBitmap.CreateCompatibleBitmap(pDC, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                                          CIntChessApp::g_nBitmapHeight + m_nAddHeight);
    m_pBufferDC->SelectObject(&l_BufferBitmap);

    //����Ӧ��ȫ�������ڴ�DC�������ӵ��ڴ�DC��
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

    //�������ϵͳ��WIN9X_WINNT������Ҫ������������λͼ���ڴ�DC
    if (WIN9X_WINNT == CIntChessApp::g_nOSType)
    {
        m_pMaskDC = new CDC;
        ASSERT_VALID(m_pMaskDC);

        m_pMaskDC->CreateCompatibleDC(pDC);

        CBitmap l_MaskBitmap;
        l_MaskBitmap.CreateBitmap(CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                                  CIntChessApp::g_nBitmapHeight + m_nAddHeight, 1, 1, NULL);
        m_pMaskDC->SelectObject(&l_MaskBitmap);

        //����Ӧ��ȫ����������λͼ���ڴ�DC������������λͼ���ڴ�DC��
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
����      : tangjs520       ��������: 2003-10-6
������    : CChessMan::~CChessMan
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CChessMan::~CChessMan()
{
    //�����û�йرն�ʱ������ر�֮
    if (m_bZooming)
    {
        ::KillTimer(m_pParentWnd->GetSafeHwnd(), ZOOM_EVENT);
    }

    delete m_pBufferDC;
    delete m_pMaskDC;
}

/******************************************************************************************
void CALLBACK CChessMan::MovingProc(int nX, int nY, LPARAM lpData)
����      : tangjs520       ��������: 2003-10-6
������    : CChessMan::MovingProc
����ֵ    : void CALLBACK
�����б�  : 
  ����1: int X
  ����2: int Y
  ����3: LPARAM lpData
����      : �������ӵ��ƶ�����
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CALLBACK CChessMan::MovingProc(int nX, int nY, LPARAM lpData)
{
    //����nX��nY��API����LineDDAÿ�λص�MovingProc����ʱ����ı�ʾ��Դλ�õ�Ŀ��λ����������
    //ĳһ��ĺᡢ������ֵ������lpData���ⲿ����API����LineDDAʱ����Ĳ���

    //���ﵽָ���Ĳ�������ʵ���ƶ�����(Ҳ����˵����������ƶ��Ĳ���Խ�������ƶ��ٶȾ�Խ��)
    if (g_nMoveCount >= g_nMoveSpeed)
    {
        CChessMan* pChessMan = (CChessMan*)lpData;

        //���µ���Sleep����������ʱ��ֻ��Ϊ����ǿ�ƶ�Ч������

        //ˢ���ƶ�ǰ������
        ::InvalidateRect(pChessMan->m_pParentWnd->GetSafeHwnd(), pChessMan->GetRect(), TRUE);
        Sleep(1);

        //�ػ��ƶ��������
        pChessMan->SetPosition(nX, nY);
        ::RedrawWindow(pChessMan->m_pParentWnd->GetSafeHwnd(), pChessMan->GetRect(), NULL, 
                       RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASENOW);
        Sleep(1);

        //�ƶ������Ӻ󣬼ǵù���g_nMoveCount���Ա��´��ƶ�����ʱ���¼��㲽��
        g_nMoveCount = 0;
    }

    //���Ӳ������Ա�ﵽָ���Ĳ�����ֵ(g_nMoveSpeed)
    g_nMoveCount++;
}

/******************************************************************************************
void CChessMan::Moving(const CPoint& ptNewPosition)
����      : tangjs520       ��������: 2003-10-6
������    : CChessMan::Moving
����ֵ    : void
�����б�  : 
  ����1: const CPoint& ptNewPosition
����      : �ƶ����ӵ�ָ�������
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessMan::Moving(const CPoint& ptNewPosition)
{
    //����������ڽ������Ų�������ֹͣ���ӵ����Ų���
    if (m_bZooming)
    {
        StopZoom();
    }

    //�ƶ�����ǰ���ǵù���g_nMoveCount���Ա��ƶ�����ʱ����ȷ���㲽��
    g_nMoveCount = 0;

    ::LineDDA(m_nXPosition, m_nYPosition, ptNewPosition.x, ptNewPosition.y, 
              (LINEDDAPROC)MovingProc, (LPARAM)this);

    //�������ӵ��ƶ�����������1�������ڵ�����API����LineDDA�����ӵ�ʵ��λ�ò�һ������ptNewPosition
    //��˵������������ʱ�������ٽ�����һ����λ���ƶ���ptNewPosition
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
����      : tangjs520       ��������: 2003-10-8
������    : CChessMan::ZoomingProc
����ֵ    : void CALLBACK
�����б�  : 
  ����1: HWND
  ����2: UINT
  ����3: UINT_PTR
  ����4: DWORD
����      : �������ӵ����Ų���(��ʵ�Ǹ����ڸ�����ӦWM_TIMER[��ʱ��]����Ϣ��Ӧ����)
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CALLBACK CChessMan::ZoomingProc(HWND, UINT, UINT_PTR, DWORD)
{
    //ֻ�б�ѡ�е����Ӳ��ܽ������Ų���
    ASSERT(g_pActiveChessMan);

    //֮����Ҫ��ȡ�����ڵ�DC����Ϊ���Ժ��ܴ����븸���ڼ��ݵ��ڴ�DC
    CDC* pDC = g_pActiveChessMan->m_pParentWnd->GetDC();
    ASSERT_VALID(pDC);

    //ȡ����������ǰ��λ�ã���������ˢ�µķ�Χ(�������Բ�������ǰ�����ӣ�����ʾ���ź������)
    CRect rectOld = g_pActiveChessMan->GetRect();

    //��õ�ǰ���ӿ�Ⱥ͸߶ȵ�������ֵ
    UINT l_nDegressionAddWidth = g_pActiveChessMan->m_nAddWidth;
    UINT l_nDegressionAddHeight = g_pActiveChessMan->m_nAddHeight;

    //��������λ��
    g_pActiveChessMan->m_nXPosition++;
    g_pActiveChessMan->m_nYPosition++;

    //�ݼ���ǰ���ӿ�Ⱥ͸߶ȵ�������ֵ������ݼ���Ŀ��������ֵ
    //С�����ӿ�ȵݼ������С����ֵ����ԭ���������λ�ú�������ֵ
    l_nDegressionAddWidth -= WIDTH_ZOOM_DEGRESSION;
    l_nDegressionAddHeight -= HEIGHT_ZOOM_DEGRESSION;
    if (l_nDegressionAddWidth < WIDTH_ZOOM_MINIMIZATION)
    {
        l_nDegressionAddWidth = INITIAL_ADDWIDTH;
        l_nDegressionAddHeight = INITIAL_ADDHEIGHT;

        g_pActiveChessMan->m_nXPosition = g_nBeforeZoomXPos;
        g_pActiveChessMan->m_nYPosition = g_nBeforeZoomYPos;

        //������ͨ��g_pActiveChessMan->GetRect()���ˢ�·�Χ����Ϊ��ʱm_nAddWidth��m_nAddHeight
        //��δ��ԭ�����Ա�����ʽ��ָ������ǰ��������ӷ�Χ
        rectOld = CRect(CPoint(g_nBeforeZoomXPos, g_nBeforeZoomYPos), 
                        CSize((CIntChessApp::g_nBitmapWidth + INITIAL_ADDWIDTH) / CHESSMANTYPE_AMOUNT, 
                        CIntChessApp::g_nBitmapHeight + INITIAL_ADDHEIGHT));
    }

    //������ʱ�ڴ�DC�������ȫ�������ڴ�DC��������������Ӧ��ֵ������λͼ
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

    //���Ѿ���������Ӧ��ֵ������λͼ�������ӵ��ڴ�DC��
    g_pActiveChessMan->m_pBufferDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + l_nDegressionAddWidth, 
                                           CIntChessApp::g_nBitmapHeight + l_nDegressionAddHeight, 
                                           &l_CopyDC, 0, 0, SRCCOPY);

    //�������ϵͳ��WIN9X_WINNT����Ҫ��ȫ����������λͼ��
    //������������Ӧ��ֵ����������λͼ����ʱ�ڴ�DC��
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

        //���Ѿ���������Ӧ��ֵ����������λͼ������������λͼ���ڴ�DC��
        g_pActiveChessMan->m_pMaskDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + l_nDegressionAddWidth, 
                                             CIntChessApp::g_nBitmapHeight + l_nDegressionAddHeight, 
                                             &l_CopyDC, 0, 0, SRCCOPY);
    }

    l_CopyDC.SelectObject(l_pOldCopyBitmap);

    //���µ�ǰ���ӿ�Ⱥ͸߶ȵ�������ֵ
    g_pActiveChessMan->m_nAddWidth = l_nDegressionAddWidth;
    g_pActiveChessMan->m_nAddHeight = l_nDegressionAddHeight;

    //ˢ������
    g_pActiveChessMan->ZoomDraw(pDC, rectOld);

    g_pActiveChessMan->m_pParentWnd->ReleaseDC(pDC);
}

/******************************************************************************************
void CChessMan::ZoomDraw(CDC* pDC, const CRect& rectZoom)
����      : tangjs520       ��������: 2003-10-31
������    : CChessMan::ZoomDraw
����ֵ    : void
�����б�  : 
  ����1: CDC* pDC
  ����2: const CRect& rectZoom
����      : �����Ų���ʱ������ˢ����ʾ���ӣ��ɱ�����˸
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessMan::ZoomDraw(CDC* pDC, const CRect& rectZoom)
{
    ASSERT_VALID(pDC);

    //������ʱ�ڴ�DC�����ڼ��������������ڵ����̱���λͼ��
    //ͬʱ����������ź��͸������λͼ
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
����      : tangjs520       ��������: 2003-10-8
������    : CChessMan::StartZoom
����ֵ    : void
�����б�  : ��
����      : ��ʼ���ӵ����Ų���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessMan::StartZoom()
{
    //��������Ѿ����ڽ������Ų����ˣ���ʲôҲ��������������
    if (!m_bZooming)
    {
        //�����ǰ�Ѿ���һ���������ڽ������Ų������������ֹͣ�������Ų���
        if (g_pActiveChessMan)
        {
            g_pActiveChessMan->StopZoom();
        }

        //������ر���
        g_pActiveChessMan = this;
        g_nBeforeZoomXPos = m_nXPosition;
        g_nBeforeZoomYPos = m_nYPosition;
        m_bZooming = TRUE;

        //������ʱ��
        ::SetTimer(m_pParentWnd->GetSafeHwnd(), ZOOM_EVENT, ZOOM_SPEED, (TIMERPROC)ZoomingProc);
    }
}

/******************************************************************************************
void CChessMan::StopZoom()
����      : tangjs520       ��������: 2003-10-8
������    : CChessMan::StopZoom
����ֵ    : void
�����б�  : ��
����      : ֹͣ���ӵ����Ų���
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessMan::StopZoom()
{
    //�رն�ʱ��
    ::KillTimer(m_pParentWnd->GetSafeHwnd(), ZOOM_EVENT);

    //��ԭ��ر���
    m_bZooming = FALSE;
    g_pActiveChessMan = NULL;
    m_nAddWidth = INITIAL_ADDWIDTH;
    m_nAddHeight = INITIAL_ADDHEIGHT;
    m_nXPosition = g_nBeforeZoomXPos;
    m_nYPosition = g_nBeforeZoomYPos;

    //�������ڴ�DC(����������λͼ���ڴ�DC)��ԭ
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

    //ˢ������
    CDC* pDC = m_pParentWnd->GetDC();
    ASSERT_VALID(pDC);

    ZoomDraw(pDC, GetRect());

    m_pParentWnd->ReleaseDC(pDC);
}
