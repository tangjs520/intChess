/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : StaticSplitterWnd.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-21
�ļ�����    : ����"��̬"�ķָ���࣬�����"��̬"ָ���ǲ�����ͨ���ָ������ı������ͼ�Ĵ�С
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
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
