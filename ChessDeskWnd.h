/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessDeskWnd.h
����        : tangjs520
�汾        : 1.0
��������    : 2004-9-29
�ļ�����    : ��������������(���������������ӡ��������ơ�����ͷ��ָʾ�Ƶ���Ʒ)��
              ����Ķ�����CIntChessView�б�����
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_CHESSDESKWND_H__1BD8DA80_29CD_4A42_B908_71178D96EE5B__INCLUDED_)
#define AFX_CHESSDESKWND_H__1BD8DA80_29CD_4A42_B908_71178D96EE5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DigitalClockWnd.h"    //����ʱ�Ӵ�����(CDigitalClockWnd)��ͷ�ļ�

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
    //����������ʱ��ͬʱҲ��Ҫ�������ӡ��������ơ�����ͷ��ָʾ�Ƶ���Ʒ
    void ExchangeObjects()
    {
        CDigitalClockWnd* pTemp = m_pWhiteClock;
        m_pWhiteClock = m_pBlackClock;
        m_pBlackClock = pTemp;
    }

    //��ʼĳһ���ļ�ʱ��
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

    //ֹͣĳһ���ļ�ʱ��
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

    //��λ��ʱ��
    void ResetTimer()
    {
        m_pWhiteClock->Reset();
        m_pBlackClock->Reset();
    }

    //�ڰ�˫��������ʱ
    void ExchangeTimerCounting()
    {
        m_pWhiteClock->Pause();
        m_pBlackClock->Pause();
    }

    //�жϼ�ʱ���Ƿ���������
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
    CBitmap* m_pBkgndBmp;               //��������ͼƬ
    CDigitalClockWnd* m_pWhiteClock;    //�׷�������
    CDigitalClockWnd* m_pBlackClock;    //�ڷ�������

    TimeLimitProc m_pfnTimeLimitProc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSDESKWND_H__1BD8DA80_29CD_4A42_B908_71178D96EE5B__INCLUDED_)
