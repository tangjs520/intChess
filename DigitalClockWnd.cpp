/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : DigitalClockWnd.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2004-9-29
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
#include "DigitalClockWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBitmap CDigitalClockWnd::m_BkgndBmp;
CDC CDigitalClockWnd::m_NumberMemDC;

/******************************************************************************************
CDigitalClockWnd::CDigitalClockWnd()
����      : tangjs520       ��������: 2004-9-29
������    : CDigitalClockWnd::CDigitalClockWnd
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CDigitalClockWnd::CDigitalClockWnd()
{
    m_nHourHigh = 0;
    m_nHourLow = 0;

    m_nMinuteHigh = 0;
    m_nMinuteLow = 0;

    m_nSecondHigh = 0;
    m_nSecondLow = 0;

    m_nTimeMode = TIMEMODE_UP;
    m_bCounting = FALSE;
}

/******************************************************************************************
CDigitalClockWnd::~CDigitalClockWnd()
����      : tangjs520       ��������: 2004-9-29
������    : CDigitalClockWnd::~CDigitalClockWnd
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CDigitalClockWnd::~CDigitalClockWnd()
{
}

BEGIN_MESSAGE_MAP(CDigitalClockWnd, CWnd)
    //{{AFX_MSG_MAP(CDigitalClockWnd)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
int CDigitalClockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
����      : tangjs520       ��������: 2004-9-29
������    : CDigitalClockWnd::OnCreate
����ֵ    : int
�����б�  : 
  ����1: LPCREATESTRUCT lpCreateStruct
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
int CDigitalClockWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // TODO: Add your specialized creation code here
    if (!m_BkgndBmp.m_hObject)
    {
        m_BkgndBmp.LoadBitmap(IDB_CLOCKBACKGROUND);
    }

    if (!m_NumberMemDC.m_hDC && !m_NumberMemDC.m_hAttribDC)
    {
        CDC* pDC = GetDC();
        ASSERT_VALID(pDC);

        //��������λͼ
        CBitmap NumberBitmap;
        NumberBitmap.LoadBitmap(IDB_NUMBER);

        m_NumberMemDC.CreateCompatibleDC(pDC);
        m_NumberMemDC.SelectObject(&NumberBitmap);

        ReleaseDC(pDC);
    }

    return 0;
}

/******************************************************************************************
BOOL CDigitalClockWnd::OnEraseBkgnd(CDC* pDC)
����      : tangjs520       ��������: 2004-9-29
������    : CDigitalClockWnd::OnEraseBkgnd
����ֵ    : BOOL
�����б�  : 
  ����1: CDC* pDC
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CDigitalClockWnd::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    if (m_BkgndBmp.m_hObject)
    {
        CDC dcMem;
        dcMem.CreateCompatibleDC(pDC);
        CBitmap* pOldBitmap = dcMem.SelectObject(&m_BkgndBmp);

        CRect rect;
        GetClientRect(&rect);

        pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);

        dcMem.SelectObject(pOldBitmap);

        return TRUE;
    }
    else
    {
        return CWnd::OnEraseBkgnd(pDC);
    }
}

/******************************************************************************************
void CDigitalClockWnd::OnPaint()
����      : tangjs520       ��������: 2004-9-29
������    : CDigitalClockWnd::OnPaint
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CDigitalClockWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    //����Сʱ
    dc.BitBlt(NUMBER_XSTARTPOS, NUMBER_YPOSITION, NUMBER_WIDTH, NUMBER_HEIGHT, 
              &m_NumberMemDC, m_nHourHigh * 9, 0, SRCCOPY);
    dc.BitBlt(NUMBER_XSTARTPOS + SPACE_BETWEEN_HIGH_LOW, NUMBER_YPOSITION, 
              NUMBER_WIDTH, NUMBER_HEIGHT, &m_NumberMemDC, m_nHourLow * 9, 0, SRCCOPY);

    //���Ʒ���
    dc.BitBlt(NUMBER_XSTARTPOS + SPACE_BETWEEN_PERIOD, NUMBER_YPOSITION, 
              NUMBER_WIDTH, NUMBER_HEIGHT, &m_NumberMemDC, m_nMinuteHigh * 9, 0, SRCCOPY);
    dc.BitBlt(NUMBER_XSTARTPOS + SPACE_BETWEEN_PERIOD + SPACE_BETWEEN_HIGH_LOW, 
              NUMBER_YPOSITION, NUMBER_WIDTH, NUMBER_HEIGHT, 
              &m_NumberMemDC, m_nMinuteLow * 9, 0, SRCCOPY);

    //��������
    dc.BitBlt(NUMBER_XSTARTPOS + 2 * SPACE_BETWEEN_PERIOD, NUMBER_YPOSITION, 
              NUMBER_WIDTH, NUMBER_HEIGHT, &m_NumberMemDC, m_nSecondHigh * 9, 0, SRCCOPY);
    dc.BitBlt(NUMBER_XSTARTPOS + 2 * SPACE_BETWEEN_PERIOD + SPACE_BETWEEN_HIGH_LOW, 
              NUMBER_YPOSITION, NUMBER_WIDTH, NUMBER_HEIGHT, 
              &m_NumberMemDC, m_nSecondLow * 9, 0, SRCCOPY);

    // Do not call CWnd::OnPaint() for painting messages
}

/******************************************************************************************
void CDigitalClockWnd::OnTimer(UINT nIDEvent)
����      : tangjs520       ��������: 2004-9-29
������    : CDigitalClockWnd::OnTimer
����ֵ    : void
�����б�  : 
  ����1: UINT nIDEvent
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CDigitalClockWnd::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    //������ʱ
    if (TIMEMODE_UP == m_nTimeMode)
    {
        m_nSecondLow++;
        if (10 == m_nSecondLow)
        {
            m_nSecondLow = 0;

            m_nSecondHigh++;
            if (6 == m_nSecondHigh)
            {
                m_nSecondHigh = 0;

                m_nMinuteLow++;
                if (10 == m_nMinuteLow)
                {
                    m_nMinuteLow = 0;

                    m_nMinuteHigh++;
                    if (6 == m_nMinuteHigh)
                    {
                        m_nMinuteHigh = 0;

                        m_nHourLow++;
                        if (10 == m_nHourLow)
                        {
                            m_nHourLow = 0;

                            m_nHourHigh++;
                            if (10 == m_nHourHigh)
                            {
                                m_nHourHigh = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    //����ʱ
    else
    {
        m_nSecondLow--;
        if (0xFF == m_nSecondLow)
        {
            m_nSecondLow = 9;

            m_nSecondHigh--;
            if (0xFF == m_nSecondHigh)
            {
                m_nSecondHigh = 5;

                m_nMinuteLow--;
                if (0xFF == m_nMinuteLow)
                {
                    m_nMinuteLow = 9;

                    m_nMinuteHigh--;
                    if (0xFF == m_nMinuteHigh)
                    {
                        m_nMinuteHigh = 5;

                        m_nHourLow--;
                        if (0xFF == m_nHourLow)
                        {
                            m_nHourLow = 9;

                            m_nHourHigh--;
                            if (0xFF == m_nHourHigh)
                            {
                                m_nHourHigh = 9;
                            }
                        }
                    }
                }
            }
        }
    }

    Invalidate(FALSE);

    CWnd::OnTimer(nIDEvent);
}

/******************************************************************************************
void CDigitalClockWnd::OnDestroy()
����      : tangjs520       ��������: 2004-9-29
������    : CDigitalClockWnd::OnDestroy
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CDigitalClockWnd::OnDestroy()
{
    // TODO: Add your message handler code here
    //�����ٴ���֮ǰ�������ʱ����û�йرգ���ر�֮
    if (m_bCounting)
    {
        KillTimer(TIME_EVENT_ID);
    }

    CWnd::OnDestroy();
}
