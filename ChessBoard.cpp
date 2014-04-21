/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessBoard.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-20
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
#include "ChessBoard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/******************************************************************************************
CChessBoard::CChessBoard(CWnd* pParentWnd)
作者      : tangjs520       创建日期: 2003-10-20
函数名    : CChessBoard::CChessBoard
返回值    : 无
参数列表  : 
  参数1: CWnd* pParentWnd
描述      : 初始化棋盘内存DC(m_pBufferDC)
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessBoard::CChessBoard(CWnd* pParentWnd)
{
    ASSERT_VALID(pParentWnd);

    //之所以要获取父窗口的DC，是为了以后能创建与父窗口兼容的内存DC
    CDC* pDC = pParentWnd->GetDC();
    ASSERT_VALID(pDC);

    //创建棋盘内存DC(它的宽度和高度包括棋盘的有效区域和空白区域)
    m_pBufferDC = new CDC;
    ASSERT_VALID(m_pBufferDC);
    m_pBufferDC->CreateCompatibleDC(pDC);

    CBitmap l_BufferBitmap;
    l_BufferBitmap.CreateCompatibleBitmap(pDC, 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN, 
                                          8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN);
    m_pBufferDC->SelectObject(&l_BufferBitmap);

    //临时内存DC，负责载入棋格位图资源后，再贴入棋盘内存DC中
    CDC dcMem;
    dcMem.CreateCompatibleDC(pDC);

    //载入棋盘的白色区域位图
    BITMAP bm = { 0 };
    CBitmap WhiteChessBoard;
    WhiteChessBoard.LoadBitmap(IDB_WHITECHESSBOARD);
    WhiteChessBoard.GetObject(sizeof(BITMAP), (LPVOID)&bm);
    CBitmap* pOldBitmap = dcMem.SelectObject(&WhiteChessBoard);

    //绘制棋盘的白色区域(棋盘的32个白格是由在一大块白色区域中嵌入32个黑格后剩下的区域组成的)
    for (register int nX = 0; nX < 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN; nX += bm.bmWidth)
    {
        for (register int nY = 0; nY < 8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN; nY += bm.bmHeight)
        {
            m_pBufferDC->BitBlt(nX, nY, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);
        }
    }

    //载入棋盘的黑格位图
    CBitmap BlackChessBoard;
    BlackChessBoard.LoadBitmap(IDB_BLACKCHESSBOARD);
    dcMem.SelectObject(&BlackChessBoard);

    //绘制棋盘的32个黑格(棋盘的右下角必须是白格)
    for (register int i = 0, nXPos = CHESSGRIDWIDTH + CHESSBOARDMARGIN / 2, 
         nYPos = 0 + CHESSBOARDMARGIN / 2, line = 0; i < 32; i++, nXPos += 2 * CHESSGRIDWIDTH)
    {
        if (nXPos >= 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN / 2)     //换行
        {
            //奇数行黑格的位置从第一个格子开始，偶数行黑格的位置从第二个格子开始
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

    //绘制棋盘有效区域的边线
    m_pBufferDC->SelectStockObject(NULL_BRUSH);
    m_pBufferDC->Rectangle(CHESSBOARDMARGIN / 2 - 1, CHESSBOARDMARGIN / 2 - 1, 
                           CHESSBOARDMARGIN / 2 + 1 + 8 * CHESSGRIDWIDTH, 
                           CHESSBOARDMARGIN / 2 + 1 + 8 * CHESSGRIDHEIGHT);

    //绘制棋盘的边线(一黑一白)
    m_pBufferDC->Rectangle(0, 0, CHESSBOARDMARGIN + 8 * CHESSGRIDWIDTH, 
                           CHESSBOARDMARGIN + 8 * CHESSGRIDHEIGHT);
    m_pBufferDC->SelectStockObject(WHITE_PEN);
    m_pBufferDC->Rectangle(1, 1, CHESSBOARDMARGIN + 8 * CHESSGRIDWIDTH - 1, 
                           CHESSBOARDMARGIN + 8 * CHESSGRIDHEIGHT - 1);

    pParentWnd->ReleaseDC(pDC);

    //最后，初始化棋格对象数组m_ChessGrid[64]和模板集合类m_mapGrid
    InitChessGrid();
}

/******************************************************************************************
CChessBoard::~CChessBoard()
作者      : tangjs520       创建日期: 2003-10-20
函数名    : CChessBoard::~CChessBoard
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CChessBoard::~CChessBoard()
{
    delete m_pBufferDC;
    m_mapGrid.RemoveAll();
}

/******************************************************************************************
void CChessBoard::InitChessGrid()
作者      : tangjs520       创建日期: 2003-10-20
函数名    : CChessBoard::InitChessGrid
返回值    : void
参数列表  : 无
描述      : 初始化棋格对象数组m_ChessGrid[64]和模板集合类m_mapGrid
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CChessBoard::InitChessGrid()
{
    //FALSE代表白色(CHESSGRID_WHITE = 0)，TRUE代表黑色(CHESSGRID_BLACK = 1)
    BOOL bGridColor = FALSE;

    const CString strHName = _T("abcdefgh");
    const CString strVName = _T("87654321");

    for (int i = 0, nXPos = CHESSBOARDMARGIN / 2, nYPos = CHESSBOARDMARGIN / 2, 
         nH = 0, nV = 0; i < 64; i++, nXPos += CHESSGRIDWIDTH, nH++)
    {
        if (nXPos >= 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN / 2)     //换行
        {
            nXPos = CHESSBOARDMARGIN / 2;
            nYPos += CHESSGRIDHEIGHT;

            bGridColor = !bGridColor;

            nH = 0;
            nV++;
        }
        m_ChessGrid[i].SetRect(CPoint(nXPos, nYPos), CSize(CHESSGRIDWIDTH, CHESSGRIDHEIGHT));
        m_ChessGrid[i].SetColor((CHESSGRIDCOLOR)bGridColor);

        //产生棋格名称(比如："a"+"8"="a8")
        CString strH(strHName[nH]);
        CString strV(strVName[nV]);
        m_ChessGrid[i].SetName(strH + strV);

        bGridColor = !bGridColor;

        //使用已经初始化的m_ChessGrid[i]来构建模板集合类m_mapGrid
        m_mapGrid.SetAt(m_ChessGrid[i].GetName(), &m_ChessGrid[i]);
    }
}
