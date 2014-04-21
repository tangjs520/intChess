/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessMan.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-6
文件描述    : 声明棋子类，该类的对象在CChessEquipmentWnd、CPawnUpDlg中被定义
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_CHESSMAN_H__B92BFBEC_4A94_49DF_B75B_D6A68C8E8C80__INCLUDED_)
#define AFX_CHESSMAN_H__B92BFBEC_4A94_49DF_B75B_D6A68C8E8C80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessGrid.h"          //棋格对象类(CChessGrid)的头文件

const BYTE CHESSMANTYPE_AMOUNT = 6;     //棋子类型(兵种)的数量

const BYTE INITIAL_MOVESPEED = 12;      //棋子的初始移动速度(数值越大，速度越快)

const UINT WIDTH_ZOOM_DEGRESSION = 13;      //棋子宽度的递减值
const UINT HEIGHT_ZOOM_DEGRESSION = 2;      //棋子高度的递减值
const UINT WIDTH_ZOOM_MINIMIZATION = 69;    //棋子宽度递减后的最小允许值
const UINT ZOOM_EVENT = 0;                  //棋子缩放定时器事件标识
const UINT ZOOM_SPEED = 100;                //棋子缩放的速度

const BOOL SCM_SHOW = TRUE;     //显示棋子的标识符
const BOOL SCM_HIDE = FALSE;    //隐藏棋子的标识符

const BOOL RMK_LAST = TRUE;     //删除最近被该棋子吃掉的棋子对象指针
const BOOL RMK_ALL = FALSE;     //删除所有被该棋子吃掉的棋子对象指针

//棋子的颜色
typedef enum tagChessManColor
{
    CHESSMAN_WHITE = 0, //白方
    CHESSMAN_BLACK = 1  //黑方
} CHESSMANCOLOR;

//棋子的类型(即兵种)
typedef enum tagChessManType
{
    ROOK = 0,   //车
    KNIGHT = 1, //马
    KING = 2,   //王
    QUEEN = 3,  //后
    BISHOP = 4, //象
    PAWN = 5    //兵
} CHESSMANTYPE;

//棋子的中文名称(可以通过CHESSMANTYPE变量进行索引)
const CString g_strChessManName[6] = {_T("车"), _T("马"), _T("王"), _T("后"), _T("象"), _T("兵")};

class CChessMan
{
public:
    //构造和析构函数(将pChessGrid设置为默认参数，是因为棋子不一定都位于棋格上，
    //比如在显示"兵的升变"对话框时)
    CChessMan(const CHESSMANCOLOR color, const CHESSMANTYPE type, CWnd* pParentWnd, 
              CChessGrid* pChessGrid = NULL);

    ~CChessMan();

    //获取棋子的颜色
    CHESSMANCOLOR GetColor() const
    {
        return m_enuColor;
    }

    //交换棋子的颜色(参数bErase代表是否立即刷新棋子)
    void ExchangeColor(const BOOL bErase = TRUE)
    {
        //如果棋子正在进行缩放操作，则停止棋子的缩放操作
        if (m_bZooming)
        {
            StopZoom();
        }
        //交换的原理是用相应颜色的全局棋子内存DC贴入棋子的内存DC中(m_pBufferDC)，
        //同时更新数据成员m_enuColor；如果操作系统是Win9x或WinNT，
        //则还需要用相应颜色的全局棋子掩码位图的内存DC贴入棋子的掩码位图内存DC中(m_pMaskDC)
        if (CHESSMAN_WHITE == m_enuColor)
        {
            m_pBufferDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                                CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                                CIntChessApp::g_pBlackChessManBufferDC, 0, 0, SRCCOPY);

            m_enuColor = CHESSMAN_BLACK;

            if (WIN9X_WINNT == CIntChessApp::g_nOSType)
            {
                m_pMaskDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                                  CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                                  CIntChessApp::g_pBlackChessManMaskDC, 0, 0, SRCCOPY);
            }
        }
        else
        {
            m_pBufferDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                                CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                                CIntChessApp::g_pWhiteChessManBufferDC, 0, 0, SRCCOPY);

            m_enuColor = CHESSMAN_WHITE;

            if (WIN9X_WINNT == CIntChessApp::g_nOSType)
            {
                m_pMaskDC->BitBlt(0, 0, CIntChessApp::g_nBitmapWidth + m_nAddWidth, 
                                  CIntChessApp::g_nBitmapHeight + m_nAddHeight, 
                                  CIntChessApp::g_pWhiteChessManMaskDC, 0, 0, SRCCOPY);
            }
        }

        if (bErase)
        {
            ::InvalidateRect(m_pParentWnd->GetSafeHwnd(), GetRect(), TRUE);
        }
    }

    //获取棋子的类型(兵种)
    CHESSMANTYPE GetType() const
    {
        return m_enuType;
    }

    //设置棋子的类型(兵种)[参数bErase代表是否立即刷新棋子]
    void SetType(const CHESSMANTYPE enuNewValue, const BOOL bErase = TRUE)
    {
        m_enuType = enuNewValue;

        if (bErase)
        {
            ::InvalidateRect(m_pParentWnd->GetSafeHwnd(), GetRect(), TRUE);
        }
    }

    //获取和设置棋子位置的坐标值
    CPoint GetPosition() const
    {
        return CPoint(m_nXPosition, m_nYPosition);
    }
    void SetPosition(int nXPos, int nYPos)
    {
        m_nXPosition = nXPos;
        m_nYPosition = nYPos;
    }
    void SetPosition(const CPoint& newPosition)
    {
        m_nXPosition = newPosition.x;
        m_nYPosition = newPosition.y;
    }
    void SetPosition(CChessGrid* pDestChessGrid, const BOOL bErase = TRUE)
    {
        ASSERT(pDestChessGrid);

        if (m_pInsideChessGrid)
        {
            //如果棋子正在进行缩放操作，则停止棋子的缩放操作
            if (m_bZooming)
            {
                StopZoom();
            }

            //如果棋子改变前所位于的棋格和要求改变成的棋格相同
            //且棋子可见，则不做任何处理
            if (m_pInsideChessGrid == pDestChessGrid && m_bVisible)
            {
                return;
            }

            //原来位置上的棋子将被擦除
            if (bErase)
            {
                ShowChessMan(SCM_HIDE);
            }

            //改变棋子位置
            CRect rect = pDestChessGrid->GetRect();
            SetPosition(rect.left, rect.top);

            //刷新显示新位置上的棋子
            if (bErase)
            {
                ShowChessMan(SCM_SHOW);
            }

            //棋子位置改变前所位于的棋格对象的m_pIncludeChessMan数据成员为对象本身时，
            //才将此数据成员设置为NULL，加上这个前提条件是因为当隐藏棋子时，并未对棋子的
            //数据成员m_pInsideChessGrid做相应调整，
            //而且有可能一个棋格内有多个棋子存在(当然只有一个棋子是可见的)
            if (m_pInsideChessGrid->GetIncludeChessMan() == this)
            {
                m_pInsideChessGrid->SetIncludeChessMan(NULL);
            }

            //现在棋子所位于的棋格对象是pDestChessGrid了
            m_pInsideChessGrid = pDestChessGrid;

            //更新棋子位置改变后所位于的棋格对象的数据成员m_pIncludeChessMan
            m_pInsideChessGrid->SetIncludeChessMan(this);
        }
    }

    //获取棋子位图的范围
    CRect GetRect() const
    {
        return CRect(CPoint(m_nXPosition, m_nYPosition), 
                     CSize((CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                     CIntChessApp::g_nBitmapHeight + m_nAddHeight));
    }

    //获取棋子的父窗口指针
    CWnd* GetParentWnd() const
    {
        return m_pParentWnd;
    }

    //绘制棋子
    void Draw(CDC* pDC)
    {
        //只绘制可见的棋子
        if (m_bVisible)
        {
            ASSERT_VALID(pDC);

            //棋子的内存DC(m_pBufferDC)、棋子掩码位图的内存DC(m_pMaskDC)在构造函数中初始化

            //通过表达式"m_enuType * (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT"
            //的计算，就能依据不同的棋子类型(兵种)，显示位图的不同部分

            //之所以要针对不同的操作系统，采取不同的绘制方法，是因为API函数TransparentBlt在Win9x和
            //WinMe下存在内存泄漏的问题，很容易死机。至于在WinNT下是否存在此现象，并未得到验证。
            //因此，针对WIN9X_WINNT系统采用传统的绘制透明位图的方法[需要两个位图，一个源位图和一个
            //掩码位图]，当然在透明位图显示速度上比直接调用API函数TransparentBlt要慢一点。
            if (WIN9X_WINNT == CIntChessApp::g_nOSType)
            {
                pDC->BitBlt(m_nXPosition, m_nYPosition, 
                            (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                            CIntChessApp::g_nBitmapHeight + m_nAddHeight, m_pMaskDC, 
                            m_enuType * (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                            0, SRCAND);

                pDC->BitBlt(m_nXPosition, m_nYPosition, 
                            (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                            CIntChessApp::g_nBitmapHeight + m_nAddHeight, m_pBufferDC, 
                            m_enuType * (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                            0, SRCPAINT);
            }
            else
            {
                ::TransparentBlt(pDC->m_hDC, m_nXPosition, m_nYPosition, 
                                 (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                                 CIntChessApp::g_nBitmapHeight + m_nAddHeight, m_pBufferDC->m_hDC, 
                                 m_enuType * (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 0, 
                                 (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                                 CIntChessApp::g_nBitmapHeight + m_nAddHeight, (UINT)TRANSPARENT_COLOR);
            }
        }
    }
    void Draw(CDC* pDC, const int nX, const int nY)
    {
        m_nXPosition = nX;
        m_nYPosition = nY;

        Draw(pDC);
    }

    //移动棋子到指定坐标点
    void Moving(const CPoint& ptNewPosition);

    //移动棋子到指定棋格
    void Moving(CChessGrid* pDestChessGrid)
    {
        ASSERT(pDestChessGrid);
        //如果棋子移动前所位于的棋格和要求移动到的棋格相同，则不做任何处理
        if (m_pInsideChessGrid && m_pInsideChessGrid != pDestChessGrid)
        {
            //首先将棋子移动到棋格所在的坐标点
            CRect rect = pDestChessGrid->GetRect();
            Moving(CPoint(rect.left, rect.top));

            //更新棋子移动前和移动后所位于的棋格对象的属性
            //棋子位置改变前所位于的棋格对象的m_pIncludeChessMan数据成员为对象本身时，
            //才将此数据成员设置为NULL，加上这个前提条件是因为当隐藏棋子时，并未对棋子的
            //数据成员m_pInsideChessGrid做相应调整，
            //而且有可能一个棋格内有多个棋子存在(当然只有一个棋子是可见的)
            if (m_pInsideChessGrid->GetIncludeChessMan() == this)
            {
                m_pInsideChessGrid->SetIncludeChessMan(NULL);
            }

            //现在棋子所位于的棋格对象是pDestChessGrid了
            m_pInsideChessGrid = pDestChessGrid;

            //更新棋子移动后所位于的棋格对象的数据成员m_pIncludeChessMan
            m_pInsideChessGrid->SetIncludeChessMan(this);
        }
    }

    //获取和设置棋子的移动速度
    static const BYTE GetMoveSpeed()
    {
        return g_nMoveSpeed;
    }
    static void SetMoveSpeed(const BYTE nNewValue)
    {
        g_nMoveSpeed = nNewValue;
    }

    //获取和设置当前被选中的棋子指针
    static CChessMan* GetActiveChessMan()
    {
        return g_pActiveChessMan;
    }
    static void SetActiveChessMan(CChessMan* pNewChessMan)
    {
        g_pActiveChessMan = pNewChessMan;
    }

    //开始和停止棋子的缩放操作
    void StartZoom();
    void StopZoom();

    //判断棋子是否可见(返回TRUE表示可见，否则表示不可见)
    BOOL IsVisible() const
    {
        return m_bVisible;
    }

    //显示或隐藏棋子(参数bErase代表是否立即刷新棋子)
    void ShowChessMan(const BOOL bShowOrHide, const BOOL bErase = TRUE)
    {
        m_bVisible = bShowOrHide;

        if (bErase)
        {
            ::InvalidateRect(m_pParentWnd->GetSafeHwnd(), GetRect(), TRUE);
        }
    }

    //判断棋子是否正在进行缩放操作(返回TRUE表示正在进行缩放操作，否则表示没有进行缩放操作)
    BOOL IsZooming() const
    {
        return m_bZooming;
    }

    //获取和设置棋子当前所位于的棋格对象
    CChessGrid* GetInsideChessGrid() const
    {
        return m_pInsideChessGrid;
    }
    void SetInsideChessGrid(CChessGrid* pChessGrid)
    {
        m_pInsideChessGrid = pChessGrid;
    }

    //获取和设置最近被该棋子吃掉的棋子对象指针
    CChessMan* GetLastKilledMan() const
    {
        CChessMan* pKilledMan = NULL;
        if (!m_pKilledManList.IsEmpty())
        {
            return m_pKilledManList.GetTail();
        }
        return pKilledMan;
    }
    void SetLastKilledMan(CChessMan* pKilledMan)
    {
        m_pKilledManList.AddTail(pKilledMan);
    }

    //删除最近或全部被该棋子吃掉的棋子对象指针
    void RemoveKilledMan(BOOL bLastOrALL = RMK_LAST)
    {
        switch (bLastOrALL)
        {
        case RMK_LAST:
            if (!m_pKilledManList.IsEmpty())
            {
                m_pKilledManList.RemoveTail();
            }
            break;
        case RMK_ALL:
            m_pKilledManList.RemoveAll();
            break;
        default:
            ASSERT(FALSE);
            break;
        }
    }

private:
    BOOL m_bVisible;    //标识棋子是否可见(TRUE表示可见，FALSE表示不可见)

    BOOL m_bZooming;    //标识棋子是否正在进行缩放操作(TRUE表示正在进行缩放操作，FALSE表示没有进行缩放操作)

    CHESSMANCOLOR m_enuColor;   //棋子的颜色
    CHESSMANTYPE m_enuType;     //棋子的类型(兵种)

    int m_nXPosition;   //棋子位置的横坐标
    int m_nYPosition;   //棋子位置的纵坐标

    CChessGrid* m_pInsideChessGrid; //棋子当前所位于的棋格对象

    CList<CChessMan*, CChessMan*&> m_pKilledManList;    //被该棋子吃掉的所有棋子对象指针

    UINT m_nAddWidth;  //棋子宽度的放大数值
    UINT m_nAddHeight; //棋子高度的放大数值

    CWnd* m_pParentWnd; //棋子的父窗口

    CDC* m_pBufferDC;   //棋子的内存DC

    //这个数据成员是为当操作系统是Win9x或WinNT时定义的，代表棋子掩码位图的内存DC
    CDC* m_pMaskDC;

    static BYTE g_nMoveCount;   //类的公共变量，表示棋子移动的步长，用于和g_nMoveSpeed做比较
    static BYTE g_nMoveSpeed;   //类的公共变量，表示棋子移动的速度

    static CChessMan* g_pActiveChessMan;    //类的公共变量，表示当前被选中的棋子

    static int g_nBeforeZoomXPos;   //类的公共变量，表示棋子缩放前的横坐标
    static int g_nBeforeZoomYPos;   //类的公共变量，表示棋子缩放前的纵坐标

    //在缩放操作时，用于刷新显示棋子，可避免闪烁
    void ZoomDraw(CDC* pDC, const CRect& rectZoom);

    //API函数LineDDA的回调函数，负责处理棋子的移动操作
    static void CALLBACK MovingProc(int nX, int nY, LPARAM lpData);

    //API函数SetTimer的回调函数，负责处理棋子的缩放操作
    static void CALLBACK ZoomingProc(HWND, UINT, UINT_PTR, DWORD);
};

#endif // !defined(AFX_CHESSMAN_H__B92BFBEC_4A94_49DF_B75B_D6A68C8E8C80__INCLUDED_)
