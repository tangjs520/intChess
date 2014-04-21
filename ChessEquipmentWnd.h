/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessEquipmentWnd.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-21
�ļ�����    : ������ߴ�����(��ߣ����������̡����ӵ�һ���������йص���Ʒ)��
              ����Ķ�����CIntChessView�б�����
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_CHESSEQUIPMENTWND_H__8A85F38C_476A_47E4_A7ED_1363B6C49A99__INCLUDED_)
#define AFX_CHESSEQUIPMENTWND_H__8A85F38C_476A_47E4_A7ED_1363B6C49A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessBoard.h"     //���̶�����(CChessBoard)��ͷ�ļ�
#include "CoordinateWnd.h"  //���괰����(CCoordinateWnd)��ͷ�ļ�
#include "ChessMan.h"       //���Ӷ�����(CChessMan)��ͷ�ļ�

//��������m_pAryWhiteChessMan��m_pAryBlackChessMan����������
const BYTE Pawn = 0;        //8����
const BYTE Bishop = 8;      //2����
const BYTE Rook = 10;       //2����
const BYTE Queen = 12;      //1����
const BYTE King = 13;       //1����
//֮����Ҫ���������ֵ��Ƶ��������Ϊ����Щ���ӵ��У�Ω������Կ�Խ�������Ӷ�����Ŀ�ĵ�
//���������ֵ������󣬿������ػ�����ʱ����֤����Զ��ʾ��������
const BYTE Knight = 14;     //2����

//����λ��ָʾ��Ŀ�Ⱥ���ɫ
const int INDICATOR_WIDTH = 3;
const COLORREF INDICATOR_COLOR  = RGB(0, 192, 0);

const UINT INDICATOR_FLASH_EVENT =  1;      //����λ��ָʾ����˸��ʱ���¼���ʶ
const UINT INDICATOR_FLASH_SPEED = 400;     //����λ��ָʾ����˸���ٶ�

class CChessEquipmentWnd : public CWnd
{
// Construction
public:
    CChessEquipmentWnd();

// Attributes
public:

// Operations
public:
    //��������
    void ExchangeChessMan();

    //���¿�ʼ��Ϸ
    void Renew();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChessEquipmentWnd)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CChessEquipmentWnd();

    //��ȡ���̶���ָ��
    CChessBoard* GetChessBoard() const
    {
        return m_pChessBoard;
    }

    //��ȡָ����������ָ��
    CChessMan** GetWhiteChessManAry()
    {
        return &m_pAryWhiteChessMan[0];
    }

    //��ȡָ����������ָ��
    CChessMan** GetBlackChessManAry()
    {
        return &m_pAryBlackChessMan[0];
    }

    //��������λ��ָʾ��
    void DrawIndicator(const CRect& rectSrcPos, const CRect& rectDestPos);

    //�������λ��ָʾ��
    void RemoveIndicator()
    {
        //������OnPaint�����е�Դλ�ñ�����Ϊ��ʱ���Ż���Ŀ��λ��ָʾ��
        //�������ʱֻ��Ҫ�ڲ���Դλ��ָʾ��󣬽�Դλ�ñ����ÿռ���
        if (!m_rectSrcPosIndicator.IsRectEmpty())
        {
            //ֹͣĿ��λ��ָʾ�����˸
            ::KillTimer(m_hWnd, INDICATOR_FLASH_EVENT);

            //������Ҫ�����������С
            CRgn rgnOutside;
            rgnOutside.CreateRectRgnIndirect(m_rectSrcPosIndicator);

            CRect rectTemp = m_rectSrcPosIndicator;
            rectTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

            CRgn rgnInside;
            rgnInside.CreateRectRgnIndirect(rectTemp);

            rgnOutside.CombineRgn(&rgnOutside, &rgnInside, RGN_DIFF);

            //��Դλ�ñ����ÿ�
            m_rectSrcPosIndicator.SetRectEmpty();

            //����Դλ��ָʾ��
            InvalidateRgn(&rgnOutside);

            if (!m_rectDestPosIndicator.IsRectEmpty())
            {
                //������Ҫ�����������С
                CRgn rgnDestOutside;
                rgnDestOutside.CreateRectRgnIndirect(m_rectDestPosIndicator);

                CRect rectDestTemp = m_rectDestPosIndicator;
                rectDestTemp.DeflateRect(INDICATOR_WIDTH, INDICATOR_WIDTH);

                CRgn rgnDestInside;
                rgnDestInside.CreateRectRgnIndirect(rectDestTemp);

                rgnDestOutside.CombineRgn(&rgnDestOutside, &rgnDestInside, RGN_DIFF);

                //��Ŀ��λ�ñ����ÿ�
                m_rectDestPosIndicator.SetRectEmpty();

                //����Ŀ��λ��ָʾ��
                InvalidateRgn(&rgnDestOutside);
            }
            else
            {
                //��Ŀ��λ�ñ����ÿ�
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
    CChessBoard* m_pChessBoard;             //����
    CCoordinateWnd* m_pCoordinateWnd[2];    //���괰��
    CChessMan* m_pAryWhiteChessMan[16];     //��ɫ����
    CChessMan* m_pAryBlackChessMan[16];     //��ɫ����

    CRect m_rectSrcPosIndicator;    //����Դλ��ָʾ��
    CRect m_rectDestPosIndicator;   //����Ŀ��λ��ָʾ��

    CRect m_rectDestPos;    //��������Ŀ��λ��ָʾ��
    CRgn m_rgnInvalidate;   //Ŀ��λ��ָʾ��ˢ������

    //����þ�̬������Ϊ�˸���̬��Ա����IndicatorFlashingProcʹ��
    static CChessEquipmentWnd* m_pObject;

    //API����SetTimer�Ļص�����������������λ��ָʾ�����˸����
    static void CALLBACK IndicatorFlashingProc(HWND, UINT, UINT_PTR, DWORD);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSEQUIPMENTWND_H__8A85F38C_476A_47E4_A7ED_1363B6C49A99__INCLUDED_)
