/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : intChess.h
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

#if !defined(AFX_INTCHESS_H__8BECF36A_E9DC_4500_91D7_7DB97AC9C6D4__INCLUDED_)
#define AFX_INTCHESS_H__8BECF36A_E9DC_4500_91D7_7DB97AC9C6D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

const BYTE WIN9X_WINNT = 0;     //Win9x��WinNT����ϵͳ��ʶ
const BYTE WIN2K_WINXP = 1;     //Win2000��WinXP����ϵͳ��ʶ

const UINT INITIAL_ADDWIDTH = 160;      //����λͼ��ȵĳ�ʼ�Ŵ���ֵ
const UINT INITIAL_ADDHEIGHT = 26;      //����λͼ�߶ȵĳ�ʼ�Ŵ���ֵ

const COLORREF TRANSPARENT_COLOR = RGB(255, 0, 255);    //����λͼ��͸��ɫ

const DWORD SRCMASK = 0x00220326;   //λͼ��������ı�ʶ��֮һ

const BYTE TIMEMODE_UP = 0;     //������ʱģʽ
const BYTE TIMEMODE_DOWN = 1;   //����ʱģʽ

class CIntChessApp : public CWinApp
{
public:
    CIntChessApp();

    static BYTE g_nOSType;  //����ϵͳ������

    static CDC* g_pWhiteChessManBufferDC;   //ȫ�ְ�ɫ�����ڴ�DC
    static CDC* g_pBlackChessManBufferDC;   //ȫ�ֺ�ɫ�����ڴ�DC

    static CDC* g_pWhiteChessManMaskDC;     //ȫ�ְ�ɫ��������λͼ�ڴ�DC(������ϵͳ��WIN9X_WINNTʱ��ʹ��)
    static CDC* g_pBlackChessManMaskDC;     //ȫ�ֺ�ɫ��������λͼ�ڴ�DC(������ϵͳ��WIN9X_WINNTʱ��ʹ��)

    static long g_nBitmapWidth;     //����λͼ�Ŀ��
    static long g_nBitmapHeight;    //����λͼ�ĸ߶�

    static CString g_strWhitePlayerName;    //�׷����ֵ�����
    static CString g_strBlackPlayerName;    //�ڷ����ֵ�����

    static float g_fWhitePlayerScore;   //�׷����ֵĵ÷�
    static float g_fBlackPlayerScore;   //�ڷ����ֵĵ÷�

    static void InstallChessManDC(CWnd* pParentWnd);    //��������λͼ����Ӧ�ڴ�DC��
    static void UninstallChessManDC();                  //ж�������ڴ�DC

    //��������Դ�е������ļ�(*.wav)
    static BOOL PlayWaveResource(const CString& strResourceName);

    //������ͼ���ڱ���λͼ
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
    BOOL CheckOSVersion(BYTE& nOSType);     //������ϵͳ������
    BOOL CheckDisplayResolution();          //�����ʾ���ķֱ���

    //��ϵͳע���ļ�����
    void RegisterShellFileType(CDocTemplate* pDocTemplate);

    //����ϵͳע���
    BOOL SetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTCHESS_H__8BECF36A_E9DC_4500_91D7_7DB97AC9C6D4__INCLUDED_)
