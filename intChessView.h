/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : intChessView.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-21
�ļ�����    : 
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_INTCHESSVIEW_H__92282F9A_2DA4_49F3_A0DA_EA67CC8F219F__INCLUDED_)
#define AFX_INTCHESSVIEW_H__92282F9A_2DA4_49F3_A0DA_EA67CC8F219F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIntChessDoc;     //�ĵ�������(CIntChessDoc)��ǰ������

const BYTE SHADOW = 4;      //������Ӱ�Ŀ��(����ֵ)

class CChessEquipmentWnd;   //��ߴ�����(CChessEquipmentWnd)��ǰ������
class CChessDeskWnd;

class CIntChessView : public CView
{
protected: // create from serialization only
    CIntChessView();
    DECLARE_DYNCREATE(CIntChessView)

// Attributes
public:
    CIntChessDoc* GetDocument();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIntChessView)
    public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CIntChessView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CIntChessView)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    //��ͼ���ڱ���λͼ
    CBitmap* m_pBkgndBmp;

    //��ߴ���
    CChessEquipmentWnd* m_pChessEquipmentWnd;

    CChessDeskWnd* m_pChessDeskWnd;
};

#ifndef _DEBUG  // debug version in intChessView.cpp
inline CIntChessDoc* CIntChessView::GetDocument()
   { return (CIntChessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTCHESSVIEW_H__92282F9A_2DA4_49F3_A0DA_EA67CC8F219F__INCLUDED_)
