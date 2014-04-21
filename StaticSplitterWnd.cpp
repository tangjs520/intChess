/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : StaticSplitterWnd.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-21
文件描述    : 要实现"静态"的分割窗口，其实很简单，只需重载虚函数StartTracking和添加响应
              WM_MOUSEMOVE消息的函数，同时都去掉它们各自调用基类的代码即可
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#include "stdafx.h"
#include "StaticSplitterWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/******************************************************************************************
CStaticSplitterWnd::CStaticSplitterWnd()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CStaticSplitterWnd::CStaticSplitterWnd
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CStaticSplitterWnd::CStaticSplitterWnd()
{
}

/******************************************************************************************
CStaticSplitterWnd::~CStaticSplitterWnd()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CStaticSplitterWnd::~CStaticSplitterWnd
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CStaticSplitterWnd::~CStaticSplitterWnd()
{
}

/******************************************************************************************
void CStaticSplitterWnd::StartTracking(int ht)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CStaticSplitterWnd::StartTracking
返回值    : void
参数列表  : 
  参数1: int ht
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CStaticSplitterWnd::StartTracking(int ht)
{
    UNREFERENCED_PARAMETER(ht);
    //CSplitterWnd::StartTracking(ht);
}

BEGIN_MESSAGE_MAP(CStaticSplitterWnd, CSplitterWnd)
    //{{AFX_MSG_MAP(CStaticSplitterWnd)
    ON_WM_MOUSEMOVE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
void CStaticSplitterWnd::OnMouseMove(UINT nFlags, CPoint pt)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CStaticSplitterWnd::OnMouseMove
返回值    : void
参数列表  : 
  参数1: UINT nFlags
  参数2: CPoint pt
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CStaticSplitterWnd::OnMouseMove(UINT nFlags, CPoint pt) 
{
    UNREFERENCED_PARAMETER(nFlags);
    UNREFERENCED_PARAMETER(pt);
    //CSplitterWnd::OnMouseMove(nFlags, pt);
}
