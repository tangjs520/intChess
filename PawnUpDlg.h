/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : PawnUpDlg.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-29
�ļ�����    : "��������"�Ի����࣬����Ķ�����CChessRule���б�����
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_PAWNUPDLG_H__557DE6D5_5406_432D_920D_79313E5167A9__INCLUDED_)
#define AFX_PAWNUPDLG_H__557DE6D5_5406_432D_920D_79313E5167A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessMan.h"   //������(CChessMan)��ͷ�ļ�

class CPawnUpDlg : public CDialog
{
// Construction
public:
    CPawnUpDlg(CHESSMANCOLOR enuColor, CWnd* pParent = NULL);
    virtual ~CPawnUpDlg();

    CHESSMANTYPE m_enuUpType;   //��Ϊ����õĳ�Ա�����������û���ѡ�����������

// Dialog Data
    //{{AFX_DATA(CPawnUpDlg)
    enum { IDD = IDD_PAWN_UPGRADE };
    int     m_nRadio;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPawnUpDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPawnUpDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    virtual void OnOK();
    afx_msg void OnClose();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    CChessMan* m_pAryChessMan[4];   //�ĸ�ͼ��(�󡢳�������)
    CHESSMANCOLOR m_enuPawnColor;   //��Ҫ����ı�����ɫ
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAWNUPDLG_H__557DE6D5_5406_432D_920D_79313E5167A9__INCLUDED_)
