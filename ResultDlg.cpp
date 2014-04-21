/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : ResultDlg.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2003-11-13
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
#include "ResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/******************************************************************************************
CResultDlg::CResultDlg(BYTE nResult, CWnd* pParent)
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CResultDlg::CResultDlg
返回值    : 无
参数列表  : 
  参数1: BYTE nResult
  参数2: CWnd* pParent
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CResultDlg::CResultDlg(BYTE nResult, CWnd* pParent/* = NULL*/)
    : CDialog(CResultDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CResultDlg)
    m_strResult = _T("");
    m_strScore = _T("");
    //}}AFX_DATA_INIT

    m_nResult = nResult;
}

/******************************************************************************************
void CResultDlg::DoDataExchange(CDataExchange* pDX)
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CResultDlg::DoDataExchange
返回值    : void
参数列表  : 
  参数1: CDataExchange* pDX
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CResultDlg)
    DDX_Text(pDX, IDC_RESULT, m_strResult);
    DDX_Text(pDX, IDC_SCORE, m_strScore);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResultDlg, CDialog)
    //{{AFX_MSG_MAP(CResultDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
BOOL CResultDlg::OnInitDialog()
作者      : tangjs520       创建日期: 2003-11-22
函数名    : CResultDlg::OnInitDialog
返回值    : BOOL
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CResultDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    CString strWhiteSide, strBlackSide;
    strWhiteSide.LoadString(IDS_WHITE_SIDE);
    strBlackSide.LoadString(IDS_BLACK_SIDE);

    switch (m_nResult)
    {
    case WHITE_WIN:
        {
            CString strWined;
            strWined.LoadString(IDS_HAS_WINED);

            m_strResult.Format(_T("%s %s %s"), strWhiteSide, CIntChessApp::g_strWhitePlayerName, strWined);

            if (99.0 == CIntChessApp::g_fWhitePlayerScore)
            {
                CIntChessApp::g_fWhitePlayerScore = 0.0;
                CIntChessApp::g_fBlackPlayerScore = 0.0;
            }
        }
        CIntChessApp::g_fWhitePlayerScore++;
        break;
    case BLACK_WIN:
        {
            CString strWined;
            strWined.LoadString(IDS_HAS_WINED);

            m_strResult.Format(_T("%s %s %s"), strBlackSide, CIntChessApp::g_strBlackPlayerName, strWined);

            if (99.0 == CIntChessApp::g_fBlackPlayerScore)
            {
                CIntChessApp::g_fWhitePlayerScore = 0.0;
                CIntChessApp::g_fBlackPlayerScore = 0.0;
            }
        }
        CIntChessApp::g_fBlackPlayerScore++;
        break;
    case DRAW:
        if (99.5 == CIntChessApp::g_fWhitePlayerScore)
        {
            CIntChessApp::g_fWhitePlayerScore = 0.0;
            CIntChessApp::g_fBlackPlayerScore = 0.0;
        }
        else if (99.5 == CIntChessApp::g_fBlackPlayerScore)
        {
            CIntChessApp::g_fWhitePlayerScore = 0.0;
            CIntChessApp::g_fBlackPlayerScore = 0.0;
        }
        CIntChessApp::g_fWhitePlayerScore += 0.5;
        CIntChessApp::g_fBlackPlayerScore += 0.5;
        m_strResult.LoadString(IDS_DRAW);
        break;
    default:
        ASSERT(FALSE);
        break;
    }

    m_strScore.Format(_T("%s %s: %-#.1f — %s %s: %-#.1f"), strWhiteSide, 
                      CIntChessApp::g_strWhitePlayerName, CIntChessApp::g_fWhitePlayerScore, 
                      strBlackSide, CIntChessApp::g_strBlackPlayerName, CIntChessApp::g_fBlackPlayerScore);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
