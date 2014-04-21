/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : PawnUpDlg.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-29
文件描述    : "兵的升变"对话框类，该类的对象在CChessRule类中被定义
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_PAWNUPDLG_H__557DE6D5_5406_432D_920D_79313E5167A9__INCLUDED_)
#define AFX_PAWNUPDLG_H__557DE6D5_5406_432D_920D_79313E5167A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessMan.h"   //棋子类(CChessMan)的头文件

class CPawnUpDlg : public CDialog
{
// Construction
public:
    CPawnUpDlg(CHESSMANCOLOR enuColor, CWnd* pParent = NULL);
    virtual ~CPawnUpDlg();

    CHESSMANTYPE m_enuUpType;   //作为输出用的成员变量，保存用户所选择的棋子类型

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
    CChessMan* m_pAryChessMan[4];   //四个图例(后、车、马、象)
    CHESSMANCOLOR m_enuPawnColor;   //将要升变的兵的颜色
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAWNUPDLG_H__557DE6D5_5406_432D_920D_79313E5167A9__INCLUDED_)
