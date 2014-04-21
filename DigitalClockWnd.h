/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : DigitalClockWnd.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2004-9-29
文件描述    : 声明数字时钟窗口类，该类的对象在CChessDeskWnd中被定义
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_DIGITALCLOCKWND_H__69C495DE_80D7_4964_BFC4_AEF10960AE13__INCLUDED_)
#define AFX_DIGITALCLOCKWND_H__69C495DE_80D7_4964_BFC4_AEF10960AE13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int NUMBER_WIDTH = 9;     //数字的宽度
const int NUMBER_HEIGHT = 13;   //数字的高度
const int NUMBER_YPOSITION = 2; //数字显示时的纵坐标

const int NUMBER_XSTARTPOS = 2;         //数字显示时的起始横坐标
const int SPACE_BETWEEN_HIGH_LOW = 12;  //每个时段高低位的间距
const int SPACE_BETWEEN_PERIOD = 30;    //时段之间的距离

const int TIME_EVENT_ID = 1;    //计时事件标识
const UINT TIME_ELAPSE = 1000;  //计时事件间隔毫秒数

class CDigitalClockWnd : public CWnd
{
// Construction
public:
    CDigitalClockWnd();

// Attributes
public:

// Operations
public:
    //设置时钟的计时模式
    void SetTimeMode(BYTE nNewMode)
    {
        m_nTimeMode = nNewMode;
    }

    //开始、停止和暂停计时器
    void Start()
    {
        if (!m_bCounting)
        {
            SetTimer(TIME_EVENT_ID, TIME_ELAPSE, NULL);
            m_bCounting = TRUE;
        }
    }
    void Stop()
    {
        if (m_bCounting)
        {
            KillTimer(TIME_EVENT_ID);
            //此处不宜将m_bCounting赋值为FALSE，因为该函数仅仅是在对局最终结束时才被调用，
            //为了在紧接着调用的CChessRule::OnGameNew函数中对CChessDeskWnd::IsTimerRunning函数
            //调用的正确性，所以对下面的语句应该注释之。
            //m_bCounting = FALSE;
        }
    }
    void Pause()
    {
        if (m_bCounting)
        {
            KillTimer(TIME_EVENT_ID);
            m_bCounting = FALSE;
        }
        else
        {
            SetTimer(TIME_EVENT_ID, TIME_ELAPSE, NULL);
            m_bCounting = TRUE;
        }
    }

    //复位计时器
    void Reset()
    {
        if (m_bCounting)
        {
            KillTimer(TIME_EVENT_ID);
        }

        m_bCounting = FALSE;
        m_nHourHigh = 0;
        m_nHourLow = 0;
        m_nMinuteHigh = 0;
        m_nMinuteLow = 0;
        m_nSecondHigh = 0;
        m_nSecondLow = 0;
        m_nTimeMode = TIMEMODE_UP;

        Invalidate(FALSE);
    }

    //设置计时器的时间
    void SetTime(BYTE nHour, BYTE nMinute, BYTE nSecond)
    {
        //确保参数的合法性
        ASSERT(99 >= nHour);
        ASSERT(59 >= nMinute);
        ASSERT(59 >= nSecond);

        m_nHourHigh = (BYTE)(nHour / 10);
        m_nHourLow = (BYTE)(nHour % 10);

        m_nMinuteHigh = (BYTE)(nMinute / 10);
        m_nMinuteLow = (BYTE)(nMinute % 10);

        m_nSecondHigh = (BYTE)(nSecond / 10);
        m_nSecondLow = (BYTE)(nSecond % 10);
    }

    //给计时器增加秒数
    void AddSecond(BYTE nSecond)
    {
        //确保参数的合法性
        ASSERT(59 >= nSecond);

        #pragma warning(disable : 4244)
        m_nSecondHigh += (BYTE)(nSecond / 10);
        m_nSecondLow += (BYTE)(nSecond % 10);
        #pragma warning(default : 4244)

        if (10 <= m_nSecondLow)
        {
            m_nSecondLow -= 10;
            m_nSecondHigh++;
            if (6 <= m_nSecondHigh)
            {
                m_nSecondHigh -= 6;

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

    //判断是否正在进行计时操作
    BOOL IsCounting() const
    {
        return m_bCounting;
    }

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDigitalClockWnd)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CDigitalClockWnd();

    // Generated message map functions
protected:
    //{{AFX_MSG(CDigitalClockWnd)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    static CBitmap m_BkgndBmp;  //背景位图
    static CDC m_NumberMemDC;   //数字位图内存DC

    BYTE m_nHourHigh;   //小时的高位
    BYTE m_nHourLow;    //小时的低位

    BYTE m_nMinuteHigh; //分钟的高位
    BYTE m_nMinuteLow;  //分钟的低位

    BYTE m_nSecondHigh; //秒数的高位
    BYTE m_nSecondLow;  //秒数的低位

    BYTE m_nTimeMode;   //标识当前的计时模式
    BOOL m_bCounting;   //标识当前是否正在进行计时
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITALCLOCKWND_H__69C495DE_80D7_4964_BFC4_AEF10960AE13__INCLUDED_)
