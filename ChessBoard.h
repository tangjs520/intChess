/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessBoard.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-20
文件描述    : 声明棋盘类，该类的对象在CChessEquipmentWnd中被定义
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_CHESSBOARD_H__2AE6B66C_8549_4CD0_86D4_FF83B7466786__INCLUDED_)
#define AFX_CHESSBOARD_H__2AE6B66C_8549_4CD0_86D4_FF83B7466786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessGrid.h"  //棋格对象类(CChessGrid)的头文件

const BYTE CHESSGRIDWIDTH = 76;     //棋格的宽度
const BYTE CHESSGRIDHEIGHT = 76;    //棋格的高度
const BYTE CHESSBOARDMARGIN = 46;   //棋盘的空白部分(指纵横两个方向各自的空白区域总和[23+23])

//以下代码出现的8和64是指棋盘是由8×8＝64个棋格组成的

class CChessBoard
{
public:
    //构造和析构函数
    CChessBoard(CWnd* pParentWnd);
    ~CChessBoard();

    //获得棋盘的内存DC
    CDC* GetDC() const
    {
        return m_pBufferDC;
    }

    //绘制棋盘(实际绘制棋盘的工作是在构造函数中进行的，只不过构造函数是将棋盘绘制在内存DC中)
    void Draw(CDC* pDC)
    {
        ASSERT_VALID(pDC);

        //棋盘的内存DC(m_pBufferDC)在构造函数中初始化
        pDC->BitBlt(0, 0, 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN, 
                    8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN, 
                    m_pBufferDC, 0, 0, SRCCOPY);
    }

    //获取棋盘的有效区域，即除去棋盘空白部分的区域
    CRect GetValidRect()
    {
        return CRect(CHESSBOARDMARGIN / 2, CHESSBOARDMARGIN / 2, 
                     CHESSBOARDMARGIN / 2 + 8 * CHESSGRIDWIDTH, 
                     CHESSBOARDMARGIN / 2 + 8 * CHESSGRIDHEIGHT);
    }

    //通过坐标点来获取对应的棋格对象
    CChessGrid* GetGridFromPoint(const CPoint& pt)
    {
        for (int i = 0; i < 64; i++)
        {
            CRect rect = m_ChessGrid[i].GetRect();
            if (rect.PtInRect(pt))
            {
                return &m_ChessGrid[i];
            }
        }

        return NULL;
    }

    //通过棋格的名称(比如："a1")来获取对应的棋格对象
    CChessGrid* GetGridFromName(const CString& strGridName)
    {
        CChessGrid* pChessGrid = NULL;
        m_mapGrid.Lookup(strGridName, pChessGrid);

        return pChessGrid;
    }

    //交换棋格的名称(当白、黑两方交换棋子时，棋盘的坐标要做相应改变，
    //因此棋格的名称也必然要跟着做相应变化)
    void ExchangeChessGridName()
    {
        //交换棋格名称的原理："a1"<-->"h8","b1"<-->"g8"...以此类推，所以只需循环64 / 2 = 32次
        for (int i = 0; i < 32; i++)
        {
            CString strTemp;
            strTemp = m_ChessGrid[i].GetName();
            m_ChessGrid[i].SetName(m_ChessGrid[63 - i].GetName());
            m_ChessGrid[63 - i].SetName(strTemp);

            //模板集合类同样也要做相应的变化
            m_mapGrid.SetAt(m_ChessGrid[i].GetName(), &m_ChessGrid[i]);
            m_mapGrid.SetAt(m_ChessGrid[63 - i].GetName(), &m_ChessGrid[63 - i]);
        }
    }

private:
    //棋盘的内存DC，所有的绘图操作均在此内存DC上进行
    CDC* m_pBufferDC;

    //棋盘由64个棋格对象组成
    CChessGrid m_ChessGrid[64];

    //为了能通过棋格的名称(比如："a1")来获取对应的棋格对象，所以定义这个模板集合类
    CTypedPtrMap<CMapStringToPtr, CString, CChessGrid*> m_mapGrid;

    //初始化棋格对象数组m_ChessGrid[64]和模板集合类m_mapGrid
    void InitChessGrid();
};

#endif // !defined(AFX_CHESSBOARD_H__2AE6B66C_8549_4CD0_86D4_FF83B7466786__INCLUDED_)
