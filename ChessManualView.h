/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessManualView.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-11-21
�ļ�����    : 
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
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
    //��ͼ���ڱ���λͼ
    CBitmap* m_pBkgndBmp;

    //��ʾ���׵��б��ؼ�
    CListBox m_ChessManualListBox;
    //�б��ؼ�ʹ�õ�����
    CFont m_ListBoxFont;

    //�洢����50�غ�ʱ����ʼ����
    CList<WORD, WORD> m_nNoDrawOf50TurnsList;
    //�洢<����ֵ, ���ִ���>���ݶ�
    CMap<DWORD, DWORD, BYTE, BYTE> m_PhaseMap;

    //"����"��ť
    CButton m_btnUndo;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSMANUALVIEW_H__7FDD1700_311F_4224_B35F_1E3F081BCA50__INCLUDED_)
