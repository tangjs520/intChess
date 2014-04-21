/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : CoordinateWnd.cpp
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

#include "stdafx.h"
#include "CoordinateWnd.h"

#include "ChessBoard.h"     //������(CChessBoard)��ͷ�ļ�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CCoordinateWnd::m_bWhiteBelowOrBlackBelow = WHITE_BELOW;   //��ʼ״̬�ǰ��������̵��·�

/******************************************************************************************
CCoordinateWnd::CCoordinateWnd(const BOOL bHCoordinateOrVCoordinate)
����      : tangjs520       ��������: 2003-10-21
������    : CCoordinateWnd::CCoordinateWnd
����ֵ    : ��
�����б�  : 
  ����1: const BOOL bHCoordinateOrVCoordinate
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CCoordinateWnd::CCoordinateWnd(const BOOL bHCoordinateOrVCoordinate)
{
    m_bHCoordinateOrVCoordinate = bHCoordinateOrVCoordinate;
}

/******************************************************************************************
CCoordinateWnd::~CCoordinateWnd()
����      : tangjs520       ��������: 2003-10-21
������    : CCoordinateWnd::~CCoordinateWnd
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2003-10-21
������    : CCoordinateWnd::OnEraseBkgnd
����ֵ    : BOOL
�����б�  : 
  ����1: CDC* pDC
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
