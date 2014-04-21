/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : CoordinateWnd.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-21
�ļ�����    : �������괰����(�����������̱߽��ϵ�����)������Ķ�����CChessEquipmentWnd�б�����
              �����굥������һ�������Ĵ����У���������CChessBoard�У������������ԭ��
              ��һ�������н������ӵ�ʱ�򣬽���Ҫˢ�����꣬���������Ļ��Ʒ���CChessBoard�У�
              ��ô����ˢ������ʱ����������ӵ���CChessBoard�����̵Ĵ���[�������ǲ�û���ػ�]��
              �ڶ����Ѳ�������̺ͱ仯������ֿ�ʵ�֣���������֯�������ǿ����������
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_COORDINATEWND_H__79DADB0F_E08A_45DB_89A1_B83D9A8CD394__INCLUDED_)
#define AFX_COORDINATEWND_H__79DADB0F_E08A_45DB_89A1_B83D9A8CD394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const BOOL WHITE_BELOW = TRUE;      //�����������·�
const BOOL BLACK_BELOW = FALSE;     //�����������·�

class CCoordinateWnd : public CWnd
{
// Construction
public:
    CCoordinateWnd(const BOOL bHCoordinateOrVCoordinate);

// Attributes
public:

// Operations
public:
    //��ȡ�����������аס���˫����λ��(����TRUE��ʾ���������̵��·��������ʾ���������̵��·�)
    static const BOOL GetWhiteBelowOrBlackBelow()
    {
        return m_bWhiteBelowOrBlackBelow;
    }
    static void SetWhiteBelowOrBlackBelow(const BOOL bNewValue)
    {
        m_bWhiteBelowOrBlackBelow = bNewValue;
    }

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCoordinateWnd)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CCoordinateWnd();

    // Generated message map functions
protected:
    //{{AFX_MSG(CCoordinateWnd)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    BOOL m_bHCoordinateOrVCoordinate;       //��ʶ���괰�ڵ�����(TRUE��ʾΪ�����꣬FALSE��ʾΪ������)
    static BOOL m_bWhiteBelowOrBlackBelow;  //��ʶ�����аס���˫����λ��
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COORDINATEWND_H__79DADB0F_E08A_45DB_89A1_B83D9A8CD394__INCLUDED_)
