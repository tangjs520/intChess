/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : StaticSplitterWnd.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-21
�ļ�����    : Ҫʵ��"��̬"�ķָ�ڣ���ʵ�ܼ򵥣�ֻ�������麯��StartTracking�������Ӧ
              WM_MOUSEMOVE��Ϣ�ĺ�����ͬʱ��ȥ�����Ǹ��Ե��û���Ĵ��뼴��
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
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
����      : tangjs520       ��������: 2004-9-29
������    : CStaticSplitterWnd::CStaticSplitterWnd
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CStaticSplitterWnd::CStaticSplitterWnd()
{
}

/******************************************************************************************
CStaticSplitterWnd::~CStaticSplitterWnd()
����      : tangjs520       ��������: 2004-9-29
������    : CStaticSplitterWnd::~CStaticSplitterWnd
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CStaticSplitterWnd::~CStaticSplitterWnd()
{
}

/******************************************************************************************
void CStaticSplitterWnd::StartTracking(int ht)
����      : tangjs520       ��������: 2004-9-29
������    : CStaticSplitterWnd::StartTracking
����ֵ    : void
�����б�  : 
  ����1: int ht
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2004-9-29
������    : CStaticSplitterWnd::OnMouseMove
����ֵ    : void
�����б�  : 
  ����1: UINT nFlags
  ����2: CPoint pt
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CStaticSplitterWnd::OnMouseMove(UINT nFlags, CPoint pt) 
{
    UNREFERENCED_PARAMETER(nFlags);
    UNREFERENCED_PARAMETER(pt);
    //CSplitterWnd::OnMouseMove(nFlags, pt);
}
