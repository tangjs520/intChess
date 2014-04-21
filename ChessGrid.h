/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ChessGrid.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-20
文件描述    : 声明棋格类，该类的对象在CChessBoard中被定义
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_CHESSGRID_H__0D7ADE1D_5A6E_40BA_81B4_28F7CBA75931__INCLUDED_)
#define AFX_CHESSGRID_H__0D7ADE1D_5A6E_40BA_81B4_28F7CBA75931__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChessMan;    //棋子类(CChessMan)的前向声明

//棋格的颜色
typedef enum tagChessGridColor
{
    CHESSGRID_WHITE = 0,    //白色
    CHESSGRID_BLACK = 1     //黑色
} CHESSGRIDCOLOR;

class CChessGrid
{
public:
    //构造和析构函数
    CChessGrid();
    ~CChessGrid();

    //获取和设置棋格的范围
    const CRect& GetRect() const
    {
        return m_rect;
    }
    void SetRect(const CRect& rectNewValue)
    {
        m_rect = rectNewValue;
    }
    void SetRect(int l, int t, int r, int b)
    {
        m_rect.left = l;
        m_rect.top = t;
        m_rect.right = r;
        m_rect.bottom = b;

        m_rect.NormalizeRect();
    }
    void SetRect(const CPoint& pt, const CSize& sz)
    {
        m_rect.left = pt.x;
        m_rect.top = pt.y;
        m_rect.right = pt.x + sz.cx;
        m_rect.bottom = pt.y + sz.cy;

        m_rect.NormalizeRect();
    }

    //获取和设置棋格的颜色
    CHESSGRIDCOLOR GetColor() const
    {
        return m_enuColor;
    }
    void SetColor(CHESSGRIDCOLOR enuNewValue)
    {
        m_enuColor = enuNewValue;
    }

    //获取和设置棋格的名称
    const CString& GetName() const
    {
        return m_strName;
    }
    void SetName(const CString& strNewValue)
    {
        m_strName = strNewValue;
    }

    //获取和设置棋格上的棋子(CChessMan类)
    CChessMan* GetIncludeChessMan() const
    {
        return m_pIncludeChessMan;
    }
    void SetIncludeChessMan(CChessMan* pChessMan)
    {
        m_pIncludeChessMan = pChessMan;
    }

private:
    CRect m_rect;                   //棋格的范围
    CHESSGRIDCOLOR m_enuColor;      //棋格的颜色
    CString m_strName;              //棋格的名称
    CChessMan* m_pIncludeChessMan;  //棋格上的棋子
};

#endif // !defined(AFX_CHESSGRID_H__0D7ADE1D_5A6E_40BA_81B4_28F7CBA75931__INCLUDED_)
