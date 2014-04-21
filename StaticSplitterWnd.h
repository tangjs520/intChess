/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : StaticSplitterWnd.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-21
文件描述    : 声明"静态"的分割窗口类，这里的"静态"指的是不允许通过分割条来改变各个视图的大小
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_STATICSPLITTERWND_H__F9C42872_EF1E_4AE9_B8DF_C7188295BB53__INCLUDED_)
#define AFX_STATICSPLITTERWND_H__F9C42872_EF1E_4AE9_B8DF_C7188295BB53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStaticSplitterWnd : public CSplitterWnd
{
// Construction
public:
    CStaticSplitterWnd();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CStaticSplitterWnd)
    virtual void StartTracking(int ht);
    //}}AFX_VIRTUAL
    
// Implementation
public:
    virtual ~CStaticSplitterWnd();

// Generated message map functions
protected:
    //{{AFX_MSG(CStaticSplitterWnd)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_STATICSPLITTERWND_H__F9C42872_EF1E_4AE9_B8DF_C7188295BB53__INCLUDED_)
