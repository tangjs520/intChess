/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ResultDlg.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-11-13
�ļ�����    : 
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_RESULTDLG_H__E6EE191A_3758_449E_A85C_60EBC94ADD31__INCLUDED_)
#define AFX_RESULTDLG_H__E6EE191A_3758_449E_A85C_60EBC94ADD31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const BYTE WHITE_WIN = 0;   //�׷�ʤ
const BYTE BLACK_WIN = 1;   //�ڷ�ʤ
const BYTE DRAW = 2;        //�;�

class CResultDlg : public CDialog
{
// Construction
public:
    CResultDlg(BYTE nResult, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CResultDlg)
    enum { IDD = IDD_RESULT };
    CString m_strResult;
    CString m_strScore;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CResultDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    BYTE m_nResult;

    // Generated message map functions
    //{{AFX_MSG(CResultDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTDLG_H__E6EE191A_3758_449E_A85C_60EBC94ADD31__INCLUDED_)
