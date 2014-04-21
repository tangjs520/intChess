/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : PawnUpDlg.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2003-10-29
�ļ�����    : 
����        : 
�����б�    : 
    1.  ......
�޸ļ�¼    : 
 1.�޸�����   : 
   �޸���     : 
   �޸�����   : 
******************************************************************************************/

#include "stdafx.h"
#include "intChess.h"
#include "PawnUpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/******************************************************************************************
CPawnUpDlg::CPawnUpDlg(CHESSMANCOLOR enuColor, CWnd* pParent)
����      : tangjs520       ��������: 2003-10-29
������    : CPawnUpDlg::CPawnUpDlg
����ֵ    : ��
�����б�  : 
  ����1: CHESSMANCOLOR enuColor
  ����2: CWnd* pParent
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CPawnUpDlg::CPawnUpDlg(CHESSMANCOLOR enuColor, CWnd* pParent/* = NULL*/)
    : CDialog(CPawnUpDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPawnUpDlg)
    m_nRadio = 0;   //Ĭ��ѡ��"��"
    //}}AFX_DATA_INIT
    for (int i = 0; i < 4; i++)
    {
        m_pAryChessMan[i] = NULL;
    }

    m_enuPawnColor = enuColor;

    m_enuUpType = QUEEN;
}

/******************************************************************************************
CPawnUpDlg::~CPawnUpDlg()
����      : tangjs520       ��������: 2003-10-29
������    : CPawnUpDlg::~CPawnUpDlg
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CPawnUpDlg::~CPawnUpDlg()
{
    for (int i = 0; i < 4; i++)
    {
        delete m_pAryChessMan[i];
    }
}

/******************************************************************************************
void CPawnUpDlg::DoDataExchange(CDataExchange* pDX)
����      : tangjs520       ��������: 2003-10-29
������    : CPawnUpDlg::DoDataExchange
����ֵ    : void
�����б�  : 
  ����1: CDataExchange* pDX
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CPawnUpDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPawnUpDlg)
    DDX_Radio(pDX, ID_QUEEN_RADIO, m_nRadio);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPawnUpDlg, CDialog)
    //{{AFX_MSG_MAP(CPawnUpDlg)
    ON_WM_PAINT()
    ON_WM_CLOSE()
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
BOOL CPawnUpDlg::OnInitDialog()
����      : tangjs520       ��������: 2003-10-29
������    : CPawnUpDlg::OnInitDialog
����ֵ    : BOOL
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CPawnUpDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    m_pAryChessMan[0] = new CChessMan(m_enuPawnColor, QUEEN, this);
    m_pAryChessMan[1] = new CChessMan(m_enuPawnColor, ROOK, this);
    m_pAryChessMan[2] = new CChessMan(m_enuPawnColor, KNIGHT, this);
    m_pAryChessMan[3] = new CChessMan(m_enuPawnColor, BISHOP, this);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

/******************************************************************************************
void CPawnUpDlg::OnPaint()
����      : tangjs520       ��������: 2003-10-29
������    : CPawnUpDlg::OnPaint
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CPawnUpDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // TODO: Add your message handler code here
    for (register int i = 0; i < 4; i++)
    {
        m_pAryChessMan[i]->Draw(&dc, 20 + i * 90, 26);
    }

    // Do not call CDialog::OnPaint() for painting messages
}

/******************************************************************************************
void CPawnUpDlg::OnOK()
����      : tangjs520       ��������: 2003-10-29
������    : CPawnUpDlg::OnOK
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CPawnUpDlg::OnOK()
{
    // TODO: Add extra validation here
    if (UpdateData(TRUE))
    {
        switch (m_nRadio)
        {
        case 0:
            m_enuUpType = QUEEN;
            break;
        case 1:
            m_enuUpType = ROOK;
            break;
        case 2:
            m_enuUpType = KNIGHT;
            break;
        case 3:
            m_enuUpType = BISHOP;
            break;
        default:
            ASSERT(FALSE);
            break;
        }
    }

    CDialog::OnOK();
}

/******************************************************************************************
void CPawnUpDlg::OnClose()
����      : tangjs520       ��������: 2003-10-29
������    : CPawnUpDlg::OnClose
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CPawnUpDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    OnOK();
}

/******************************************************************************************
void CPawnUpDlg::OnLButtonDown(UINT nFlags, CPoint point)
����      : tangjs520       ��������: 2003-10-29
������    : CPawnUpDlg::OnLButtonDown
����ֵ    : void
�����б�  : 
  ����1: UINT nFlags
  ����2: CPoint point
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CPawnUpDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    for (int i = 0; i < 4; i++)
    {
        if (m_pAryChessMan[i]->GetRect().PtInRect(point))
        {
            m_nRadio = i;
            UpdateData(FALSE);

            //֮������ͨ��"ID_QUEEN_RADIO + i"���ʽ�����������ѡ��ť��IDֵ��
            //���������ĸ���ѡ��ť��IDֵ��������
            CButton* pRadio = (CButton*)GetDlgItem(ID_QUEEN_RADIO + i);
            ASSERT_VALID(pRadio);
            pRadio->SetFocus();

            break;
        }
    }

    CDialog::OnLButtonDown(nFlags, point);
}
