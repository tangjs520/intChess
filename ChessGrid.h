/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : ChessGrid.h
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-20
�ļ�����    : ��������࣬����Ķ�����CChessBoard�б�����
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#if !defined(AFX_CHESSGRID_H__0D7ADE1D_5A6E_40BA_81B4_28F7CBA75931__INCLUDED_)
#define AFX_CHESSGRID_H__0D7ADE1D_5A6E_40BA_81B4_28F7CBA75931__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChessMan;    //������(CChessMan)��ǰ������

//������ɫ
typedef enum tagChessGridColor
{
    CHESSGRID_WHITE = 0,    //��ɫ
    CHESSGRID_BLACK = 1     //��ɫ
} CHESSGRIDCOLOR;

class CChessGrid
{
public:
    //�������������
    CChessGrid();
    ~CChessGrid();

    //��ȡ���������ķ�Χ
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

    //��ȡ������������ɫ
    CHESSGRIDCOLOR GetColor() const
    {
        return m_enuColor;
    }
    void SetColor(CHESSGRIDCOLOR enuNewValue)
    {
        m_enuColor = enuNewValue;
    }

    //��ȡ��������������
    const CString& GetName() const
    {
        return m_strName;
    }
    void SetName(const CString& strNewValue)
    {
        m_strName = strNewValue;
    }

    //��ȡ����������ϵ�����(CChessMan��)
    CChessMan* GetIncludeChessMan() const
    {
        return m_pIncludeChessMan;
    }
    void SetIncludeChessMan(CChessMan* pChessMan)
    {
        m_pIncludeChessMan = pChessMan;
    }

private:
    CRect m_rect;                   //���ķ�Χ
    CHESSGRIDCOLOR m_enuColor;      //������ɫ
    CString m_strName;              //��������
    CChessMan* m_pIncludeChessMan;  //����ϵ�����
};

#endif // !defined(AFX_CHESSGRID_H__0D7ADE1D_5A6E_40BA_81B4_28F7CBA75931__INCLUDED_)
