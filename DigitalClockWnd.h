/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : DigitalClockWnd.h
����        : tangjs520
�汾        : 1.0
��������    : 2004-9-29
�ļ�����    : ��������ʱ�Ӵ����࣬����Ķ�����CChessDeskWnd�б�����
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_DIGITALCLOCKWND_H__69C495DE_80D7_4964_BFC4_AEF10960AE13__INCLUDED_)
#define AFX_DIGITALCLOCKWND_H__69C495DE_80D7_4964_BFC4_AEF10960AE13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int NUMBER_WIDTH = 9;     //���ֵĿ��
const int NUMBER_HEIGHT = 13;   //���ֵĸ߶�
const int NUMBER_YPOSITION = 2; //������ʾʱ��������

const int NUMBER_XSTARTPOS = 2;         //������ʾʱ����ʼ������
const int SPACE_BETWEEN_HIGH_LOW = 12;  //ÿ��ʱ�θߵ�λ�ļ��
const int SPACE_BETWEEN_PERIOD = 30;    //ʱ��֮��ľ���

const int TIME_EVENT_ID = 1;    //��ʱ�¼���ʶ
const UINT TIME_ELAPSE = 1000;  //��ʱ�¼����������

class CDigitalClockWnd : public CWnd
{
// Construction
public:
    CDigitalClockWnd();

// Attributes
public:

// Operations
public:
    //����ʱ�ӵļ�ʱģʽ
    void SetTimeMode(BYTE nNewMode)
    {
        m_nTimeMode = nNewMode;
    }

    //��ʼ��ֹͣ����ͣ��ʱ��
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
            //�˴����˽�m_bCounting��ֵΪFALSE����Ϊ�ú����������ڶԾ����ս���ʱ�ű����ã�
            //Ϊ���ڽ����ŵ��õ�CChessRule::OnGameNew�����ж�CChessDeskWnd::IsTimerRunning����
            //���õ���ȷ�ԣ����Զ���������Ӧ��ע��֮��
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

    //��λ��ʱ��
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

    //���ü�ʱ����ʱ��
    void SetTime(BYTE nHour, BYTE nMinute, BYTE nSecond)
    {
        //ȷ�������ĺϷ���
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

    //����ʱ����������
    void AddSecond(BYTE nSecond)
    {
        //ȷ�������ĺϷ���
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

    //�ж��Ƿ����ڽ��м�ʱ����
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
    static CBitmap m_BkgndBmp;  //����λͼ
    static CDC m_NumberMemDC;   //����λͼ�ڴ�DC

    BYTE m_nHourHigh;   //Сʱ�ĸ�λ
    BYTE m_nHourLow;    //Сʱ�ĵ�λ

    BYTE m_nMinuteHigh; //���ӵĸ�λ
    BYTE m_nMinuteLow;  //���ӵĵ�λ

    BYTE m_nSecondHigh; //�����ĸ�λ
    BYTE m_nSecondLow;  //�����ĵ�λ

    BYTE m_nTimeMode;   //��ʶ��ǰ�ļ�ʱģʽ
    BOOL m_bCounting;   //��ʶ��ǰ�Ƿ����ڽ��м�ʱ
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITALCLOCKWND_H__69C495DE_80D7_4964_BFC4_AEF10960AE13__INCLUDED_)
