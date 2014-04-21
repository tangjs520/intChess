/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessMan.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-6
�ļ�����    : ���������࣬����Ķ�����CChessEquipmentWnd��CPawnUpDlg�б�����
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_CHESSMAN_H__B92BFBEC_4A94_49DF_B75B_D6A68C8E8C80__INCLUDED_)
#define AFX_CHESSMAN_H__B92BFBEC_4A94_49DF_B75B_D6A68C8E8C80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessGrid.h"          //��������(CChessGrid)��ͷ�ļ�

const BYTE CHESSMANTYPE_AMOUNT = 6;     //��������(����)������

const BYTE INITIAL_MOVESPEED = 12;      //���ӵĳ�ʼ�ƶ��ٶ�(��ֵԽ���ٶ�Խ��)

const UINT WIDTH_ZOOM_DEGRESSION = 13;      //���ӿ�ȵĵݼ�ֵ
const UINT HEIGHT_ZOOM_DEGRESSION = 2;      //���Ӹ߶ȵĵݼ�ֵ
const UINT WIDTH_ZOOM_MINIMIZATION = 69;    //���ӿ�ȵݼ������С����ֵ
const UINT ZOOM_EVENT = 0;                  //�������Ŷ�ʱ���¼���ʶ
const UINT ZOOM_SPEED = 100;                //�������ŵ��ٶ�

const BOOL SCM_SHOW = TRUE;     //��ʾ���ӵı�ʶ��
const BOOL SCM_HIDE = FALSE;    //�������ӵı�ʶ��

const BOOL RMK_LAST = TRUE;     //ɾ������������ӳԵ������Ӷ���ָ��
const BOOL RMK_ALL = FALSE;     //ɾ�����б������ӳԵ������Ӷ���ָ��

//���ӵ���ɫ
typedef enum tagChessManColor
{
    CHESSMAN_WHITE = 0, //�׷�
    CHESSMAN_BLACK = 1  //�ڷ�
} CHESSMANCOLOR;

//���ӵ�����(������)
typedef enum tagChessManType
{
    ROOK = 0,   //��
    KNIGHT = 1, //��
    KING = 2,   //��
    QUEEN = 3,  //��
    BISHOP = 4, //��
    PAWN = 5    //��
} CHESSMANTYPE;

//���ӵ���������(����ͨ��CHESSMANTYPE������������)
const CString g_strChessManName[6] = {_T("��"), _T("��"), _T("��"), _T("��"), _T("��"), _T("��")};

class CChessMan
{
public:
    //�������������(��pChessGrid����ΪĬ�ϲ���������Ϊ���Ӳ�һ����λ������ϣ�
    //��������ʾ"��������"�Ի���ʱ)
    CChessMan(const CHESSMANCOLOR color, const CHESSMANTYPE type, CWnd* pParentWnd, 
              CChessGrid* pChessGrid = NULL);

    ~CChessMan();

    //��ȡ���ӵ���ɫ
    CHESSMANCOLOR GetColor() const
    {
        return m_enuColor;
    }

    //�������ӵ���ɫ(����bErase�����Ƿ�����ˢ������)
    void ExchangeColor(const BOOL bErase = TRUE)
    {
        //����������ڽ������Ų�������ֹͣ���ӵ����Ų���
        if (m_bZooming)
        {
            StopZoom();
        }
        //������ԭ��������Ӧ��ɫ��ȫ�������ڴ�DC�������ӵ��ڴ�DC��(m_pBufferDC)��
        //ͬʱ�������ݳ�Աm_enuColor���������ϵͳ��Win9x��WinNT��
        //����Ҫ����Ӧ��ɫ��ȫ����������λͼ���ڴ�DC�������ӵ�����λͼ�ڴ�DC��(m_pMaskDC)
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

    //��ȡ���ӵ�����(����)
    CHESSMANTYPE GetType() const
    {
        return m_enuType;
    }

    //�������ӵ�����(����)[����bErase�����Ƿ�����ˢ������]
    void SetType(const CHESSMANTYPE enuNewValue, const BOOL bErase = TRUE)
    {
        m_enuType = enuNewValue;

        if (bErase)
        {
            ::InvalidateRect(m_pParentWnd->GetSafeHwnd(), GetRect(), TRUE);
        }
    }

    //��ȡ����������λ�õ�����ֵ
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
            //����������ڽ������Ų�������ֹͣ���ӵ����Ų���
            if (m_bZooming)
            {
                StopZoom();
            }

            //������Ӹı�ǰ��λ�ڵ�����Ҫ��ı�ɵ������ͬ
            //�����ӿɼ��������κδ���
            if (m_pInsideChessGrid == pDestChessGrid && m_bVisible)
            {
                return;
            }

            //ԭ��λ���ϵ����ӽ�������
            if (bErase)
            {
                ShowChessMan(SCM_HIDE);
            }

            //�ı�����λ��
            CRect rect = pDestChessGrid->GetRect();
            SetPosition(rect.left, rect.top);

            //ˢ����ʾ��λ���ϵ�����
            if (bErase)
            {
                ShowChessMan(SCM_SHOW);
            }

            //����λ�øı�ǰ��λ�ڵ��������m_pIncludeChessMan���ݳ�ԱΪ������ʱ��
            //�Ž������ݳ�Ա����ΪNULL���������ǰ����������Ϊ����������ʱ����δ�����ӵ�
            //���ݳ�Աm_pInsideChessGrid����Ӧ������
            //�����п���һ��������ж�����Ӵ���(��Ȼֻ��һ�������ǿɼ���)
            if (m_pInsideChessGrid->GetIncludeChessMan() == this)
            {
                m_pInsideChessGrid->SetIncludeChessMan(NULL);
            }

            //����������λ�ڵ���������pDestChessGrid��
            m_pInsideChessGrid = pDestChessGrid;

            //��������λ�øı����λ�ڵ�����������ݳ�Աm_pIncludeChessMan
            m_pInsideChessGrid->SetIncludeChessMan(this);
        }
    }

    //��ȡ����λͼ�ķ�Χ
    CRect GetRect() const
    {
        return CRect(CPoint(m_nXPosition, m_nYPosition), 
                     CSize((CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT, 
                     CIntChessApp::g_nBitmapHeight + m_nAddHeight));
    }

    //��ȡ���ӵĸ�����ָ��
    CWnd* GetParentWnd() const
    {
        return m_pParentWnd;
    }

    //��������
    void Draw(CDC* pDC)
    {
        //ֻ���ƿɼ�������
        if (m_bVisible)
        {
            ASSERT_VALID(pDC);

            //���ӵ��ڴ�DC(m_pBufferDC)����������λͼ���ڴ�DC(m_pMaskDC)�ڹ��캯���г�ʼ��

            //ͨ�����ʽ"m_enuType * (CIntChessApp::g_nBitmapWidth + m_nAddWidth) / CHESSMANTYPE_AMOUNT"
            //�ļ��㣬�������ݲ�ͬ����������(����)����ʾλͼ�Ĳ�ͬ����

            //֮����Ҫ��Բ�ͬ�Ĳ���ϵͳ����ȡ��ͬ�Ļ��Ʒ���������ΪAPI����TransparentBlt��Win9x��
            //WinMe�´����ڴ�й©�����⣬������������������WinNT���Ƿ���ڴ����󣬲�δ�õ���֤��
            //��ˣ����WIN9X_WINNTϵͳ���ô�ͳ�Ļ���͸��λͼ�ķ���[��Ҫ����λͼ��һ��Դλͼ��һ��
            //����λͼ]����Ȼ��͸��λͼ��ʾ�ٶ��ϱ�ֱ�ӵ���API����TransparentBltҪ��һ�㡣
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

    //�ƶ����ӵ�ָ�������
    void Moving(const CPoint& ptNewPosition);

    //�ƶ����ӵ�ָ�����
    void Moving(CChessGrid* pDestChessGrid)
    {
        ASSERT(pDestChessGrid);
        //��������ƶ�ǰ��λ�ڵ�����Ҫ���ƶ����������ͬ�������κδ���
        if (m_pInsideChessGrid && m_pInsideChessGrid != pDestChessGrid)
        {
            //���Ƚ������ƶ���������ڵ������
            CRect rect = pDestChessGrid->GetRect();
            Moving(CPoint(rect.left, rect.top));

            //���������ƶ�ǰ���ƶ�����λ�ڵ������������
            //����λ�øı�ǰ��λ�ڵ��������m_pIncludeChessMan���ݳ�ԱΪ������ʱ��
            //�Ž������ݳ�Ա����ΪNULL���������ǰ����������Ϊ����������ʱ����δ�����ӵ�
            //���ݳ�Աm_pInsideChessGrid����Ӧ������
            //�����п���һ��������ж�����Ӵ���(��Ȼֻ��һ�������ǿɼ���)
            if (m_pInsideChessGrid->GetIncludeChessMan() == this)
            {
                m_pInsideChessGrid->SetIncludeChessMan(NULL);
            }

            //����������λ�ڵ���������pDestChessGrid��
            m_pInsideChessGrid = pDestChessGrid;

            //���������ƶ�����λ�ڵ�����������ݳ�Աm_pIncludeChessMan
            m_pInsideChessGrid->SetIncludeChessMan(this);
        }
    }

    //��ȡ���������ӵ��ƶ��ٶ�
    static const BYTE GetMoveSpeed()
    {
        return g_nMoveSpeed;
    }
    static void SetMoveSpeed(const BYTE nNewValue)
    {
        g_nMoveSpeed = nNewValue;
    }

    //��ȡ�����õ�ǰ��ѡ�е�����ָ��
    static CChessMan* GetActiveChessMan()
    {
        return g_pActiveChessMan;
    }
    static void SetActiveChessMan(CChessMan* pNewChessMan)
    {
        g_pActiveChessMan = pNewChessMan;
    }

    //��ʼ��ֹͣ���ӵ����Ų���
    void StartZoom();
    void StopZoom();

    //�ж������Ƿ�ɼ�(����TRUE��ʾ�ɼ��������ʾ���ɼ�)
    BOOL IsVisible() const
    {
        return m_bVisible;
    }

    //��ʾ����������(����bErase�����Ƿ�����ˢ������)
    void ShowChessMan(const BOOL bShowOrHide, const BOOL bErase = TRUE)
    {
        m_bVisible = bShowOrHide;

        if (bErase)
        {
            ::InvalidateRect(m_pParentWnd->GetSafeHwnd(), GetRect(), TRUE);
        }
    }

    //�ж������Ƿ����ڽ������Ų���(����TRUE��ʾ���ڽ������Ų����������ʾû�н������Ų���)
    BOOL IsZooming() const
    {
        return m_bZooming;
    }

    //��ȡ���������ӵ�ǰ��λ�ڵ�������
    CChessGrid* GetInsideChessGrid() const
    {
        return m_pInsideChessGrid;
    }
    void SetInsideChessGrid(CChessGrid* pChessGrid)
    {
        m_pInsideChessGrid = pChessGrid;
    }

    //��ȡ����������������ӳԵ������Ӷ���ָ��
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

    //ɾ�������ȫ���������ӳԵ������Ӷ���ָ��
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
    BOOL m_bVisible;    //��ʶ�����Ƿ�ɼ�(TRUE��ʾ�ɼ���FALSE��ʾ���ɼ�)

    BOOL m_bZooming;    //��ʶ�����Ƿ����ڽ������Ų���(TRUE��ʾ���ڽ������Ų�����FALSE��ʾû�н������Ų���)

    CHESSMANCOLOR m_enuColor;   //���ӵ���ɫ
    CHESSMANTYPE m_enuType;     //���ӵ�����(����)

    int m_nXPosition;   //����λ�õĺ�����
    int m_nYPosition;   //����λ�õ�������

    CChessGrid* m_pInsideChessGrid; //���ӵ�ǰ��λ�ڵ�������

    CList<CChessMan*, CChessMan*&> m_pKilledManList;    //�������ӳԵ����������Ӷ���ָ��

    UINT m_nAddWidth;  //���ӿ�ȵķŴ���ֵ
    UINT m_nAddHeight; //���Ӹ߶ȵķŴ���ֵ

    CWnd* m_pParentWnd; //���ӵĸ�����

    CDC* m_pBufferDC;   //���ӵ��ڴ�DC

    //������ݳ�Ա��Ϊ������ϵͳ��Win9x��WinNTʱ����ģ�������������λͼ���ڴ�DC
    CDC* m_pMaskDC;

    static BYTE g_nMoveCount;   //��Ĺ�����������ʾ�����ƶ��Ĳ��������ں�g_nMoveSpeed���Ƚ�
    static BYTE g_nMoveSpeed;   //��Ĺ�����������ʾ�����ƶ����ٶ�

    static CChessMan* g_pActiveChessMan;    //��Ĺ�����������ʾ��ǰ��ѡ�е�����

    static int g_nBeforeZoomXPos;   //��Ĺ�����������ʾ��������ǰ�ĺ�����
    static int g_nBeforeZoomYPos;   //��Ĺ�����������ʾ��������ǰ��������

    //�����Ų���ʱ������ˢ����ʾ���ӣ��ɱ�����˸
    void ZoomDraw(CDC* pDC, const CRect& rectZoom);

    //API����LineDDA�Ļص����������������ӵ��ƶ�����
    static void CALLBACK MovingProc(int nX, int nY, LPARAM lpData);

    //API����SetTimer�Ļص����������������ӵ����Ų���
    static void CALLBACK ZoomingProc(HWND, UINT, UINT_PTR, DWORD);
};

#endif // !defined(AFX_CHESSMAN_H__B92BFBEC_4A94_49DF_B75B_D6A68C8E8C80__INCLUDED_)
