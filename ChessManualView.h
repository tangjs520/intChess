/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessManualView.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-11-21
文件描述    : 
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_CHESSMANUALVIEW_H__7FDD1700_311F_4224_B35F_1E3F081BCA50__INCLUDED_)
#define AFX_CHESSMANUALVIEW_H__7FDD1700_311F_4224_B35F_1E3F081BCA50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChessManualView : public CView
{
protected:
    CChessManualView();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CChessManualView)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChessManualView)
    protected:
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CChessManualView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
    afx_msg LRESULT OnWriteChessManual(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnEmptyChessManual(WPARAM, LPARAM);
    afx_msg void OnUndoBtnClicked();
    //{{AFX_MSG(CChessManualView)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    //视图窗口背景位图
    CBitmap* m_pBkgndBmp;

    //显示棋谱的列表框控件
    CListBox m_ChessManualListBox;
    //列表框控件使用的字体
    CFont m_ListBoxFont;

    //存储计算50回合时的起始步数
    CList<WORD, WORD> m_nNoDrawOf50TurnsList;
    //存储<局面值, 出现次数>数据对
    CMap<DWORD, DWORD, BYTE, BYTE> m_PhaseMap;

    //"悔棋"按钮
    CButton m_btnUndo;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSMANUALVIEW_H__7FDD1700_311F_4224_B35F_1E3F081BCA50__INCLUDED_)
