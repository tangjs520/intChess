/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : OtherView.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2004-9-29
�ļ�����    : 
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#include "stdafx.h"
#include "intChess.h"
#include "OtherView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(COtherView, CView)

/******************************************************************************************
COtherView::COtherView()
����      : tangjs520       ��������: 2004-9-29
������    : COtherView::COtherView
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
COtherView::COtherView()
{
}

/******************************************************************************************
COtherView::~COtherView()
����      : tangjs520       ��������: 2004-9-29
������    : COtherView::~COtherView
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
COtherView::~COtherView()
{
}

BEGIN_MESSAGE_MAP(COtherView, CView)
    //{{AFX_MSG_MAP(COtherView)
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
void COtherView::OnDraw(CDC* pDC)
����      : tangjs520       ��������: 2004-9-29
������    : COtherView::OnDraw
����ֵ    : void
�����б�  : 
  ����1: CDC* pDC
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void COtherView::OnDraw(CDC* pDC)
{
    //CDocument* pDoc = GetDocument();
    // TODO: add draw code here
    pDC->TextOut(30, 30, _T("The Other View"));
}

/////////////////////////////////////////////////////////////////////////////
// COtherView diagnostics

#ifdef _DEBUG
void COtherView::AssertValid() const
{
    CView::AssertValid();
}

void COtherView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif //_DEBUG
