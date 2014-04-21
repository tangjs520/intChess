/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : OtherView.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2004-9-29
文件描述    : 
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_OTHERVIEW_H__F894C42E_234F_42A7_AFD1_EBFCCF9C366F__INCLUDED_)
#define AFX_OTHERVIEW_H__F894C42E_234F_42A7_AFD1_EBFCCF9C366F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COtherView : public CView
{
protected:
    COtherView();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(COtherView)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COtherView)
    protected:
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~COtherView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:
    //{{AFX_MSG(COtherView)
        // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OTHERVIEW_H__F894C42E_234F_42A7_AFD1_EBFCCF9C366F__INCLUDED_)
