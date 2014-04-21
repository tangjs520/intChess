/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : intChessDoc.h
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

#if !defined(AFX_INTCHESSDOC_H__EBD02624_C365_49EA_9B63_02FAE8471814__INCLUDED_)
#define AFX_INTCHESSDOC_H__EBD02624_C365_49EA_9B63_02FAE8471814__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIntChessDoc : public CDocument
{
protected: // create from serialization only
    CIntChessDoc();
    DECLARE_DYNCREATE(CIntChessDoc)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIntChessDoc)
    public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    protected:
    virtual BOOL SaveModified();
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CIntChessDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
    //{{AFX_MSG(CIntChessDoc)
    afx_msg void OnSetupExchange();
    afx_msg void OnUpdateSetupExchange(CCmdUI* pCmdUI);
    afx_msg void OnFileSave();
    afx_msg void OnFileSaveAs();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTCHESSDOC_H__EBD02624_C365_49EA_9B63_02FAE8471814__INCLUDED_)
