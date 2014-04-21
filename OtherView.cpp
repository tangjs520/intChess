/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : OtherView.cpp
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : COtherView::COtherView
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
COtherView::COtherView()
{
}

/******************************************************************************************
COtherView::~COtherView()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : COtherView::~COtherView
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2004-9-29
函数名    : COtherView::OnDraw
返回值    : void
参数列表  : 
  参数1: CDC* pDC
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
