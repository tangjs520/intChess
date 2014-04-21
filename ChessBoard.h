/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessBoard.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-20
�ļ�����    : ���������࣬����Ķ�����CChessEquipmentWnd�б�����
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_CHESSBOARD_H__2AE6B66C_8549_4CD0_86D4_FF83B7466786__INCLUDED_)
#define AFX_CHESSBOARD_H__2AE6B66C_8549_4CD0_86D4_FF83B7466786__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChessGrid.h"  //��������(CChessGrid)��ͷ�ļ�

const BYTE CHESSGRIDWIDTH = 76;     //���Ŀ��
const BYTE CHESSGRIDHEIGHT = 76;    //���ĸ߶�
const BYTE CHESSBOARDMARGIN = 46;   //���̵Ŀհײ���(ָ�ݺ�����������ԵĿհ������ܺ�[23+23])

//���´�����ֵ�8��64��ָ��������8��8��64�������ɵ�

class CChessBoard
{
public:
    //�������������
    CChessBoard(CWnd* pParentWnd);
    ~CChessBoard();

    //������̵��ڴ�DC
    CDC* GetDC() const
    {
        return m_pBufferDC;
    }

    //��������(ʵ�ʻ������̵Ĺ������ڹ��캯���н��еģ�ֻ�������캯���ǽ����̻������ڴ�DC��)
    void Draw(CDC* pDC)
    {
        ASSERT_VALID(pDC);

        //���̵��ڴ�DC(m_pBufferDC)�ڹ��캯���г�ʼ��
        pDC->BitBlt(0, 0, 8 * CHESSGRIDWIDTH + CHESSBOARDMARGIN, 
                    8 * CHESSGRIDHEIGHT + CHESSBOARDMARGIN, 
                    m_pBufferDC, 0, 0, SRCCOPY);
    }

    //��ȡ���̵���Ч���򣬼���ȥ���̿հײ��ֵ�����
    CRect GetValidRect()
    {
        return CRect(CHESSBOARDMARGIN / 2, CHESSBOARDMARGIN / 2, 
                     CHESSBOARDMARGIN / 2 + 8 * CHESSGRIDWIDTH, 
                     CHESSBOARDMARGIN / 2 + 8 * CHESSGRIDHEIGHT);
    }

    //ͨ�����������ȡ��Ӧ��������
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

    //ͨ����������(���磺"a1")����ȡ��Ӧ��������
    CChessGrid* GetGridFromName(const CString& strGridName)
    {
        CChessGrid* pChessGrid = NULL;
        m_mapGrid.Lookup(strGridName, pChessGrid);

        return pChessGrid;
    }

    //������������(���ס���������������ʱ�����̵�����Ҫ����Ӧ�ı䣬
    //�����������Ҳ��ȻҪ��������Ӧ�仯)
    void ExchangeChessGridName()
    {
        //����������Ƶ�ԭ��"a1"<-->"h8","b1"<-->"g8"...�Դ����ƣ�����ֻ��ѭ��64 / 2 = 32��
        for (int i = 0; i < 32; i++)
        {
            CString strTemp;
            strTemp = m_ChessGrid[i].GetName();
            m_ChessGrid[i].SetName(m_ChessGrid[63 - i].GetName());
            m_ChessGrid[63 - i].SetName(strTemp);

            //ģ�弯����ͬ��ҲҪ����Ӧ�ı仯
            m_mapGrid.SetAt(m_ChessGrid[i].GetName(), &m_ChessGrid[i]);
            m_mapGrid.SetAt(m_ChessGrid[63 - i].GetName(), &m_ChessGrid[63 - i]);
        }
    }

private:
    //���̵��ڴ�DC�����еĻ�ͼ�������ڴ��ڴ�DC�Ͻ���
    CDC* m_pBufferDC;

    //������64�����������
    CChessGrid m_ChessGrid[64];

    //Ϊ����ͨ����������(���磺"a1")����ȡ��Ӧ�����������Զ������ģ�弯����
    CTypedPtrMap<CMapStringToPtr, CString, CChessGrid*> m_mapGrid;

    //��ʼ������������m_ChessGrid[64]��ģ�弯����m_mapGrid
    void InitChessGrid();
};

#endif // !defined(AFX_CHESSBOARD_H__2AE6B66C_8549_4CD0_86D4_FF83B7466786__INCLUDED_)
