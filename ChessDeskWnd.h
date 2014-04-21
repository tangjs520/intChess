/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessDeskWnd.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2004-9-29
文件描述    : 声明棋桌窗口类(棋桌，即包括棋钟、棋手名称、棋手头像、指示灯等物品)，
              该类的对象在CIntChessView中被定义
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_CHESSDESKWND_H__1BD8DA80_29CD_4A42_B908_71178D96EE5B__INCLUDED_)
#define AFX_CHESSDESKWND_H__1BD8DA80_29CD_4A42_B908_71178D96EE5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DigitalClockWnd.h"    //数字时钟窗口类(CDigitalClockWnd)的头文件

typedef void (*TimeLimitProc)(BOOL);

class CChessDeskWnd : public CWnd
{
// Construction
public:
    CChessDeskWnd();

// Attributes
public:

// Operations
public:
    //当交换棋子时，同时也需要交换棋钟、棋手名称、棋手头像、指示灯等物品
    void ExchangeObjects()
    {
        CDigitalClockWnd* pTemp = m_pWhiteClock;
        m_pWhiteClock = m_pBlackClock;
        m_pBlackClock = pTemp;
    }

    //开始某一方的计时器
    void StartTimer(BOOL bWhichColorClock)
    {
        if (!bWhichColorClock)
        {
            m_pWhiteClock->Start();
        }
        else
        {
            m_pBlackClock->Start();
        }
    }

    //停止某一方的计时器
    void StopTimer(BOOL bWhichColorClock)
    {
        if (!bWhichColorClock)
        {
            m_pWhiteClock->Stop();
        }
        else
        {
            m_pBlackClock->Stop();
        }
    }

    //复位计时器
    void ResetTimer()
    {
        m_pWhiteClock->Reset();
        m_pBlackClock->Reset();
    }

    //黑白双方交换计时
    void ExchangeTimerCounting()
    {
        m_pWhiteClock->Pause();
        m_pBlackClock->Pause();
    }

    //判断计时器是否正在运行
    BOOL IsTimerRunning() const
    {
        BOOL bWhiteCounting = m_pWhiteClock->IsCounting();
        BOOL bBlackCounting = m_pBlackClock->IsCounting();

        if (bWhiteCounting || bBlackCounting)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    void SetTimerLimit();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChessDeskWnd)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CChessDeskWnd();

    // Generated message map functions
protected:
    //{{AFX_MSG(CChessDeskWnd)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    CBitmap* m_pBkgndBmp;               //棋桌背景图片
    CDigitalClockWnd* m_pWhiteClock;    //白方的棋钟
    CDigitalClockWnd* m_pBlackClock;    //黑方的棋钟

    TimeLimitProc m_pfnTimeLimitProc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSDESKWND_H__1BD8DA80_29CD_4A42_B908_71178D96EE5B__INCLUDED_)
