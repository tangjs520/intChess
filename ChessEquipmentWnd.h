/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessEquipmentWnd.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-21
文件描述    : 声明棋具窗口类(棋具，即包括棋盘、棋子等一切与下棋有关的物品)，
              该类的对象在CIntChessView中被定义
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_CHESSEQUIPMENTWND_H__8A85F38C_476A_47E4_A7ED_1363B6C49A99__INCLUDED_)
#define AFX_CHESSEQUIPMENTWND_H__8A85F38C_476A_47E4_A7ED_1363B6C49A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessBoard.h"     //棋盘对象类(CChessBoard)的头文件
#include "CoordinateWnd.h"  //坐标窗口类(CCoordinateWnd)的头文件
#include "ChessMan.h"       //棋子对象类(CChessMan)的头文件

//用来索引m_pAryWhiteChessMan和m_pAryBlackChessMan这两个数组
const BYTE Pawn = 0;        //8个兵
const BYTE Bishop = 8;      //2个象
const BYTE Rook = 10;       //2个车
const BYTE Queen = 12;      //1个后
const BYTE King = 13;       //1个王
//之所以要把马的索引值设计得最大，是因为在这些棋子当中，惟有马可以跨越其他棋子而到达目的地
//把马的索引值放在最后，可以在重画背景时，保证马永远显示在最上面
const BYTE Knight = 14;     //2个马

//棋子位置指示框的宽度和颜色
const int INDICATOR_WIDTH = 3;
const COLORREF INDICATOR_COLOR  = RGB(0, 192, 0);

const UINT INDICATOR_FLASH_EVENT =  1;      //棋子位置指示框闪烁定时器事件标识
const UINT INDICATOR_FLASH_SPEED = 400;     //棋子位置指示框闪烁的速度

class CChessEquipmentWnd : public CWnd
{
// Construction
public:
    CChessEquipmentWnd();

// Attributes
public:

// Operations
public:
    //交换棋子
    void ExchangeChessMan();

    //重新开始游戏
    void Renew();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChessEquipmentWnd)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CChessEquipmentWnd();

    //获取棋盘对象指针
    CChessBoard* GetChessBoard() const
    {
        return m_pChessBoard;
    }

    //获取指向白棋数组的指针
    CChessMan** GetWhiteChessManAry()
    {
        return &m_pAryWhiteChessMan[0];
    }

    //获取指向黑棋数组的指针
    CChessMan** GetBlackChessManAry()
    {
        return &m_pAryBlackChessMan[0];
    }

    //绘制棋子位置指示框
    void DrawIndicator(const CRect& rectSrcPos, const CRect& rectDestPos);

    //清除棋子位置指示框
    void RemoveIndicator()
    {
        //由于在OnPaint函数中当源位置变量不为空时，才绘制目标位置指示框，
        //所以清除时只需要在擦除源位置指示框后，将源位置变量置空即可
        if (!m_rectSrcPosIndicator.IsRectEmpty())
        {
            //停止目标位置指示框的闪烁
            ::KillTimer(m_hWnd, INDICATOR_FLASH_EVENT);

            //计算需要擦除的区域大小
            CRgn rgnOutside;
            rgnOutside.CreateRectRgnIndirect(m_rectSrcPosIndicator);

            CRect rectTemp = m_rectSrcPosIndicator;
            rectTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

            CRgn rgnInside;
            rgnInside.CreateRectRgnIndirect(rectTemp);

            rgnOutside.CombineRgn(&rgnOutside, &rgnInside, RGN_DIFF);

            //将源位置变量置空
            m_rectSrcPosIndicator.SetRectEmpty();

            //擦除源位置指示框
            InvalidateRgn(&rgnOutside);

            if (!m_rectDestPosIndicator.IsRectEmpty())
            {
                //计算需要擦除的区域大小
                CRgn rgnDestOutside;
                rgnDestOutside.CreateRectRgnIndirect(m_rectDestPosIndicator);

                CRect rectDestTemp = m_rectDestPosIndicator;
                rectDestTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

                CRgn rgnDestInside;
                rgnDestInside.CreateRectRgnIndirect(rectDestTemp);

                rgnDestOutside.CombineRgn(&rgnDestOutside, &rgnDestInside, RGN_DIFF);

                //将目标位置变量置空
                m_rectDestPosIndicator.SetRectEmpty();

                //擦除目标位置指示框
                InvalidateRgn(&rgnDestOutside);
            }
            else
            {
                //将目标位置变量置空
                m_rectDestPos.SetRectEmpty();
            }
        }
    }

    // Generated message map functions
protected:
    //{{AFX_MSG(CChessEquipmentWnd)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    CChessBoard* m_pChessBoard;             //棋盘
    CCoordinateWnd* m_pCoordinateWnd[2];    //坐标窗口
    CChessMan* m_pAryWhiteChessMan[16];     //白色棋子
    CChessMan* m_pAryBlackChessMan[16];     //黑色棋子

    CRect m_rectSrcPosIndicator;    //棋子源位置指示框
    CRect m_rectDestPosIndicator;   //棋子目标位置指示框

    CRect m_rectDestPos;    //备份棋子目标位置指示框
    CRgn m_rgnInvalidate;   //目标位置指示框刷新区域

    //定义该静态变量是为了给静态成员函数IndicatorFlashingProc使用
    static CChessEquipmentWnd* m_pObject;

    //API函数SetTimer的回调函数，负责处理棋子位置指示框的闪烁操作
    static void CALLBACK IndicatorFlashingProc(HWND, UINT, UINT_PTR, DWORD);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSEQUIPMENTWND_H__8A85F38C_476A_47E4_A7ED_1363B6C49A99__INCLUDED_)
