/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : PawnUpDlg.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-10-29
文件描述    : 
其它        : 
函数列表    : 
    1.  ......
修改记录    : 
 1.修改日期   : 
   修改人     : 
   修改内容   : 
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
作者      : tangjs520       创建日期: 2003-10-29
函数名    : CPawnUpDlg::CPawnUpDlg
返回值    : 无
参数列表  : 
  参数1: CHESSMANCOLOR enuColor
  参数2: CWnd* pParent
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CPawnUpDlg::CPawnUpDlg(CHESSMANCOLOR enuColor, CWnd* pParent/* = NULL*/)
    : CDialog(CPawnUpDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPawnUpDlg)
    m_nRadio = 0;   //默认选择"后"
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
作者      : tangjs520       创建日期: 2003-10-29
函数名    : CPawnUpDlg::~CPawnUpDlg
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2003-10-29
函数名    : CPawnUpDlg::DoDataExchange
返回值    : void
参数列表  : 
  参数1: CDataExchange* pDX
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2003-10-29
函数名    : CPawnUpDlg::OnInitDialog
返回值    : BOOL
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2003-10-29
函数名    : CPawnUpDlg::OnPaint
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2003-10-29
函数名    : CPawnUpDlg::OnOK
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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
作者      : tangjs520       创建日期: 2003-10-29
函数名    : CPawnUpDlg::OnClose
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CPawnUpDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    OnOK();
}

/******************************************************************************************
void CPawnUpDlg::OnLButtonDown(UINT nFlags, CPoint point)
作者      : tangjs520       创建日期: 2003-10-29
函数名    : CPawnUpDlg::OnLButtonDown
返回值    : void
参数列表  : 
  参数1: UINT nFlags
  参数2: CPoint point
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
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

            //之所以能通过"ID_QUEEN_RADIO + i"表达式计算出各个单选按钮的ID值，
            //是由于这四个单选按钮的ID值是连续的
            CButton* pRadio = (CButton*)GetDlgItem(ID_QUEEN_RADIO + i);
            ASSERT_VALID(pRadio);
            pRadio->SetFocus();

            break;
        }
    }

    CDialog::OnLButtonDown(nFlags, point);
}
