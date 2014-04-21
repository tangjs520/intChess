/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : DigitalClockWnd.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2004-9-29
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CDigitalClockWnd::CDigitalClockWnd
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CDigitalClockWnd::~CDigitalClockWnd
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CDigitalClockWnd::OnCreate
返回值    : int
参数列表  : 
  参数1: LPCREATESTRUCT lpCreateStruct
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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

        //载入数字位图
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CDigitalClockWnd::OnEraseBkgnd
返回值    : BOOL
参数列表  : 
  参数1: CDC* pDC
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CDigitalClockWnd::OnPaint
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CDigitalClockWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    //绘制小时
    dc.BitBlt(NUMBER_XSTARTPOS, NUMBER_YPOSITION, NUMBER_WIDTH, NUMBER_HEIGHT, 
              &m_NumberMemDC, m_nHourHigh * 9, 0, SRCCOPY);
    dc.BitBlt(NUMBER_XSTARTPOS + SPACE_BETWEEN_HIGH_LOW, NUMBER_YPOSITION, 
              NUMBER_WIDTH, NUMBER_HEIGHT, &m_NumberMemDC, m_nHourLow * 9, 0, SRCCOPY);

    //绘制分钟
    dc.BitBlt(NUMBER_XSTARTPOS + SPACE_BETWEEN_PERIOD, NUMBER_YPOSITION, 
              NUMBER_WIDTH, NUMBER_HEIGHT, &m_NumberMemDC, m_nMinuteHigh * 9, 0, SRCCOPY);
    dc.BitBlt(NUMBER_XSTARTPOS + SPACE_BETWEEN_PERIOD + SPACE_BETWEEN_HIGH_LOW, 
              NUMBER_YPOSITION, NUMBER_WIDTH, NUMBER_HEIGHT, 
              &m_NumberMemDC, m_nMinuteLow * 9, 0, SRCCOPY);

    //绘制秒数
    dc.BitBlt(NUMBER_XSTARTPOS + 2 * SPACE_BETWEEN_PERIOD, NUMBER_YPOSITION, 
              NUMBER_WIDTH, NUMBER_HEIGHT, &m_NumberMemDC, m_nSecondHigh * 9, 0, SRCCOPY);
    dc.BitBlt(NUMBER_XSTARTPOS + 2 * SPACE_BETWEEN_PERIOD + SPACE_BETWEEN_HIGH_LOW, 
              NUMBER_YPOSITION, NUMBER_WIDTH, NUMBER_HEIGHT, 
              &m_NumberMemDC, m_nSecondLow * 9, 0, SRCCOPY);

    // Do not call CWnd::OnPaint() for painting messages
}

/******************************************************************************************
void CDigitalClockWnd::OnTimer(UINT nIDEvent)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CDigitalClockWnd::OnTimer
返回值    : void
参数列表  : 
  参数1: UINT nIDEvent
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CDigitalClockWnd::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    //正常计时
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
    //倒计时
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CDigitalClockWnd::OnDestroy
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CDigitalClockWnd::OnDestroy()
{
    // TODO: Add your message handler code here
    //在销毁窗口之前，如果定时器还没有关闭，则关闭之
    if (m_bCounting)
    {
        KillTimer(TIME_EVENT_ID);
    }

    CWnd::OnDestroy();
}
