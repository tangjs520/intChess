/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : CoordinateWnd.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-21
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
#include "CoordinateWnd.h"

#include "ChessBoard.h"     //棋盘类(CChessBoard)的头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CCoordinateWnd::m_bWhiteBelowOrBlackBelow = WHITE_BELOW;   //初始状态是白棋在棋盘的下方

/******************************************************************************************
CCoordinateWnd::CCoordinateWnd(const BOOL bHCoordinateOrVCoordinate)
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CCoordinateWnd::CCoordinateWnd
返回值    : 无
参数列表  : 
  参数1: const BOOL bHCoordinateOrVCoordinate
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CCoordinateWnd::CCoordinateWnd(const BOOL bHCoordinateOrVCoordinate)
{
    m_bHCoordinateOrVCoordinate = bHCoordinateOrVCoordinate;
}

/******************************************************************************************
CCoordinateWnd::~CCoordinateWnd()
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CCoordinateWnd::~CCoordinateWnd
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CCoordinateWnd::~CCoordinateWnd()
{
}

BEGIN_MESSAGE_MAP(CCoordinateWnd, CWnd)
    //{{AFX_MSG_MAP(CCoordinateWnd)
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
BOOL CCoordinateWnd::OnEraseBkgnd(CDC* pDC)
作者      : tangjs520       创建日期: 2003-10-21
函数名    : CCoordinateWnd::OnEraseBkgnd
返回值    : BOOL
参数列表  : 
  参数1: CDC* pDC
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CCoordinateWnd::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    CString strCoordinate = _T("");

    if (m_bHCoordinateOrVCoordinate && m_bWhiteBelowOrBlackBelow)
    {
        strCoordinate = _T("abcdefgh");
    }
    else if (!m_bHCoordinateOrVCoordinate && m_bWhiteBelowOrBlackBelow)
    {
        strCoordinate = _T("87654321");
    }
    else if (m_bHCoordinateOrVCoordinate && !m_bWhiteBelowOrBlackBelow)
    {
        strCoordinate = _T("hgfedcba");
    }
    else
    {
        strCoordinate = _T("12345678");
    }

    pDC->SetBkMode(TRANSPARENT);

    if (m_bHCoordinateOrVCoordinate)
    {
        for (register int i = 0; i < 8; i++)
        {
            CSize sizeText = pDC->GetTextExtent(strCoordinate[i]);
            pDC->TextOut(i * CHESSGRIDWIDTH + CHESSGRIDWIDTH / 2 - sizeText.cx / 2, 2, strCoordinate[i]);
        }
    }
    else
    {
        for (register int i = 0; i < 8; i++)
        {
            CSize sizeText = pDC->GetTextExtent(strCoordinate[i]);
            pDC->TextOut(6, i * CHESSGRIDHEIGHT + CHESSGRIDHEIGHT / 2 - sizeText.cy / 2, strCoordinate[i]);
        }
    }

    return TRUE;
}
