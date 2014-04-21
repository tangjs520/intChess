/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : CoordinateWnd.h
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-21
文件描述    : 声明坐标窗口类(即出现在棋盘边角上的坐标)，该类的对象在CChessEquipmentWnd中被定义
              把坐标单独放在一个独立的窗口中，而不是在CChessBoard中，有两个方面的原因：
              第一，当进行交换棋子的时候，仅需要刷新坐标，如果把坐标的绘制放在CChessBoard中，
              那么会在刷新坐标时，额外的增加调用CChessBoard画棋盘的代码[尽管它们并没有重画]；
              第二，把不变的棋盘和变化的坐标分开实现，有利于组织代码和增强代码的灵活性
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
******************************************************************************************/

#if !defined(AFX_COORDINATEWND_H__79DADB0F_E08A_45DB_89A1_B83D9A8CD394__INCLUDED_)
#define AFX_COORDINATEWND_H__79DADB0F_E08A_45DB_89A1_B83D9A8CD394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const BOOL WHITE_BELOW = TRUE;      //白棋在棋盘下方
const BOOL BLACK_BELOW = FALSE;     //黑棋在棋盘下方

class CCoordinateWnd : public CWnd
{
// Construction
public:
    CCoordinateWnd(const BOOL bHCoordinateOrVCoordinate);

// Attributes
public:

// Operations
public:
    //获取和设置棋盘中白、黑双方的位置(返回TRUE表示白棋在棋盘的下方，否则表示黑棋在棋盘的下方)
    static const BOOL GetWhiteBelowOrBlackBelow()
    {
        return m_bWhiteBelowOrBlackBelow;
    }
    static void SetWhiteBelowOrBlackBelow(const BOOL bNewValue)
    {
        m_bWhiteBelowOrBlackBelow = bNewValue;
    }

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCoordinateWnd)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CCoordinateWnd();

    // Generated message map functions
protected:
    //{{AFX_MSG(CCoordinateWnd)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    BOOL m_bHCoordinateOrVCoordinate;       //标识坐标窗口的类型(TRUE表示为横坐标，FALSE表示为纵坐标)
    static BOOL m_bWhiteBelowOrBlackBelow;  //标识棋盘中白、黑双方的位置
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COORDINATEWND_H__79DADB0F_E08A_45DB_89A1_B83D9A8CD394__INCLUDED_)
