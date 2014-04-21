/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : intChess.h
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

#if !defined(AFX_INTCHESS_H__8BECF36A_E9DC_4500_91D7_7DB97AC9C6D4__INCLUDED_)
#define AFX_INTCHESS_H__8BECF36A_E9DC_4500_91D7_7DB97AC9C6D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

const BYTE WIN9X_WINNT = 0;     //Win9x、WinNT操作系统标识
const BYTE WIN2K_WINXP = 1;     //Win2000、WinXP操作系统标识

const UINT INITIAL_ADDWIDTH = 160;      //棋子位图宽度的初始放大数值
const UINT INITIAL_ADDHEIGHT = 26;      //棋子位图高度的初始放大数值

const COLORREF TRANSPARENT_COLOR = RGB(255, 0, 255);    //棋子位图的透明色

const DWORD SRCMASK = 0x00220326;   //位图掩码操作的标识符之一

const BYTE TIMEMODE_UP = 0;     //正常计时模式
const BYTE TIMEMODE_DOWN = 1;   //倒计时模式

class CIntChessApp : public CWinApp
{
public:
    CIntChessApp();

    static BYTE g_nOSType;  //操作系统的类型

    static CDC* g_pWhiteChessManBufferDC;   //全局白色棋子内存DC
    static CDC* g_pBlackChessManBufferDC;   //全局黑色棋子内存DC

    static CDC* g_pWhiteChessManMaskDC;     //全局白色棋子掩码位图内存DC(当操作系统是WIN9X_WINNT时才使用)
    static CDC* g_pBlackChessManMaskDC;     //全局黑色棋子掩码位图内存DC(当操作系统是WIN9X_WINNT时才使用)

    static long g_nBitmapWidth;     //棋子位图的宽度
    static long g_nBitmapHeight;    //棋子位图的高度

    static CString g_strWhitePlayerName;    //白方棋手的姓名
    static CString g_strBlackPlayerName;    //黑方棋手的姓名

    static float g_fWhitePlayerScore;   //白方棋手的得分
    static float g_fBlackPlayerScore;   //黑方棋手的得分

    static void InstallChessManDC(CWnd* pParentWnd);    //加载棋子位图到相应内存DC中
    static void UninstallChessManDC();                  //卸载棋子内存DC

    //播放在资源中的声音文件(*.wav)
    static BOOL PlayWaveResource(const CString& strResourceName);

    //载入视图窗口背景位图
    static BOOL SetBkgndBmp(UINT nID, CBitmap*& pBkgndBmp);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIntChessApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation
    //{{AFX_MSG(CIntChessApp)
    afx_msg void OnAppAbout();
    afx_msg void OnFileNew();
    afx_msg void OnFileOpen();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    BOOL CheckOSVersion(BYTE& nOSType);     //检测操作系统的类型
    BOOL CheckDisplayResolution();          //检测显示器的分辨率

    //向系统注册文件类型
    void RegisterShellFileType(CDocTemplate* pDocTemplate);

    //操作系统注册表
    BOOL SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTCHESS_H__8BECF36A_E9DC_4500_91D7_7DB97AC9C6D4__INCLUDED_)
