/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessBoard.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-20
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
#include "ChessBoard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/******************************************************************************************
CChessBoard::CChessBoard(CWnd* pParentWnd)
����      : tangjs520       ��������: 2003-10-20
������    : CChessBoard::CChessBoard
����ֵ    : ��
�����б�  : 
  ����1: CWnd* pParentWnd
����      : ��ʼ�������ڴ�DC(m_pBufferDC)
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CChessBoard::CChessBoard(CWnd* pParentWnd)
{
    ASSERT_VALID(pParentWnd);

    //֮����Ҫ��ȡ�����ڵ�DC����Ϊ���Ժ��ܴ����븸���ڼ��ݵ��ڴ�DC
    CDC* pDC = pParentWnd->GetDC();
    ASSERT_VALID(pDC);

    //���������ڴ�DC(���Ŀ�Ⱥ͸߶Ȱ������̵���Ч����Ϳհ�����)
    m_pBufferDC = new CDC;
    ASSERT_VALID(m_pBufferDC);
    m_pBufferDC->CreateCompatibleDC(pDC);

    CBitmap l_BufferBitmap;
    l_BufferBitmap.CreateCompatibleBitmap(pDC, 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN, 
                                          8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN);
    m_pBufferDC->SelectObject(&l_BufferBitmap);

    //��ʱ�ڴ�DC�������������λͼ��Դ�������������ڴ�DC��
    CDC dcMem;
    dcMem.CreateCompatibleDC(pDC);

    //�������̵İ�ɫ����λͼ
    BITMAP bm = { 0 };
    CBitmap WhiteChessBoard;
    WhiteChessBoard.LoadBitmap(IDB_WHITECHESSBOARD);
    WhiteChessBoard.GetObject(sizeof(BITMAP), (LPVOID)&bm);
    CBitmap* pOldBitmap = dcMem.SelectObject(&WhiteChessBoard);

    //�������̵İ�ɫ����(���̵�32���׸�������һ����ɫ������Ƕ��32���ڸ��ʣ�µ�������ɵ�)
    for (register int nX = 0; nX < 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN; nX += bm.bmWidth)
    {
        for (register int nY = 0; nY < 8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN; nY += bm.bmHeight)
        {
            m_pBufferDC->BitBlt(nX, nY, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);
        }
    }

    //�������̵ĺڸ�λͼ
    CBitmap BlackChessBoard;
    BlackChessBoard.LoadBitmap(IDB_BLACKCHESSBOARD);
    dcMem.SelectObject(&BlackChessBoard);

    //�������̵�32���ڸ�(���̵����½Ǳ����ǰ׸�)
    for (register int i = 0, nXPos = CHESSGRIDWIDTH + CHESSBOARDMARGIN / 2, 
         nYPos = 0 + CHESSBOARDMARGIN / 2, line = 0; i < 32; i++, nXPos += 2 * CHESSGRIDWIDTH)
    {
        if (nXPos >= 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN / 2)     //����
        {
            //�����кڸ��λ�ôӵ�һ�����ӿ�ʼ��ż���кڸ��λ�ôӵڶ������ӿ�ʼ
            line++;
            if (line % 2)
            {
                nXPos = 0 + CHESSBOARDMARGIN / 2;
            }
            else
            {
                nXPos = CHESSGRIDWIDTH + CHESSBOARDMARGIN / 2;
            }

            nYPos += CHESSGRIDHEIGHT;
        }

        m_pBufferDC->BitBlt(nXPos, nYPos, CHESSGRIDWIDTH, CHESSGRIDHEIGHT, &dcMem, 0, 0, SRCCOPY);
    }

    dcMem.SelectObject(pOldBitmap);

    //����������Ч����ı���
    m_pBufferDC->SelectStockObject(NULL_BRUSH);
    m_pBufferDC->Rectangle(CHESSBOARDMARGIN / 2 - 1, CHESSBOARDMARGIN / 2 - 1, 
                           CHESSBOARDMARGIN / 2 + 1 + 8 * CHESSGRIDWIDTH, 
                           CHESSBOARDMARGIN / 2 + 1 + 8 * CHESSGRIDHEIGHT);

    //�������̵ı���(һ��һ��)
    m_pBufferDC->Rectangle(0, 0, CHESSBOARDMARGIN + 8 * CHESSGRIDWIDTH, 
                           CHESSBOARDMARGIN + 8 * CHESSGRIDHEIGHT);
    m_pBufferDC->SelectStockObject(WHITE_PEN);
    m_pBufferDC->Rectangle(1, 1, CHESSBOARDMARGIN + 8 * CHESSGRIDWIDTH - 1, 
                           CHESSBOARDMARGIN + 8 * CHESSGRIDHEIGHT - 1);

    pParentWnd->ReleaseDC(pDC);

    //��󣬳�ʼ������������m_ChessGrid[64]��ģ�弯����m_mapGrid
    InitChessGrid();
}

/******************************************************************************************
CChessBoard::~CChessBoard()
����      : tangjs520       ��������: 2003-10-20
������    : CChessBoard::~CChessBoard
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CChessBoard::~CChessBoard()
{
    delete m_pBufferDC;
    m_mapGrid.RemoveAll();
}

/******************************************************************************************
void CChessBoard::InitChessGrid()
����      : tangjs520       ��������: 2003-10-20
������    : CChessBoard::InitChessGrid
����ֵ    : void
�����б�  : ��
����      : ��ʼ������������m_ChessGrid[64]��ģ�弯����m_mapGrid
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CChessBoard::InitChessGrid()
{
    //FALSE�����ɫ(CHESSGRID_WHITE = 0)��TRUE�����ɫ(CHESSGRID_BLACK = 1)
    BOOL bGridColor = FALSE;

    const CString strHName = _T("abcdefgh");
    const CString strVName = _T("87654321");

    for (int i = 0, nXPos = CHESSBOARDMARGIN / 2, nYPos = CHESSBOARDMARGIN / 2, 
         nH = 0, nV = 0; i < 64; i++, nXPos += CHESSGRIDWIDTH, nH++)
    {
        if (nXPos >= 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN / 2)     //����
        {
            nXPos = CHESSBOARDMARGIN / 2;
            nYPos += CHESSGRIDHEIGHT;

            bGridColor = !bGridColor;

            nH = 0;
            nV++;
        }
        m_ChessGrid[i].SetRect(CPoint(nXPos, nYPos), CSize(CHESSGRIDWIDTH, CHESSGRIDHEIGHT));
        m_ChessGrid[i].SetColor((CHESSGRIDCOLOR)bGridColor);

        //�����������(���磺"a"+"8"="a8")
        CString strH(strHName[nH]);
        CString strV(strVName[nV]);
        m_ChessGrid[i].SetName(strH + strV);

        bGridColor = !bGridColor;

        //ʹ���Ѿ���ʼ����m_ChessGrid[i]������ģ�弯����m_mapGrid
        m_mapGrid.SetAt(m_ChessGrid[i].GetName(), &m_ChessGrid[i]);
    }
}
