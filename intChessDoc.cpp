/******************************************************************************************
版权所有    : 2003-2004, 雨林软件工作室, 保留所有权利
文件名      : intChessDoc.cpp
作者        : tangjs520
版本        : 1.0
生成日期    : 2004-9-29
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
#include "intChessDoc.h"

#include "ChessRule.h"          //走棋规则类(CChessRule)的头文件

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CIntChessDoc, CDocument)

BEGIN_MESSAGE_MAP(CIntChessDoc, CDocument)
    //{{AFX_MSG_MAP(CIntChessDoc)
    ON_COMMAND(ID_SETUP_EXCHANGE, OnSetupExchange)
    ON_UPDATE_COMMAND_UI(ID_SETUP_EXCHANGE, OnUpdateSetupExchange)
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
    ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************************
CIntChessDoc::CIntChessDoc()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::CIntChessDoc
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CIntChessDoc::CIntChessDoc()
{
    //在使用走棋规则类之前，必须初始化该静态变量
    CChessRule::m_pIntChessDoc = this;
}

/******************************************************************************************
CIntChessDoc::~CIntChessDoc()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::~CIntChessDoc
返回值    : 无
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
CIntChessDoc::~CIntChessDoc()
{
}

/******************************************************************************************
BOOL CIntChessDoc::OnNewDocument()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::OnNewDocument
返回值    : BOOL
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CIntChessDoc::OnNewDocument()
{
    return CChessRule::OnGameNew();
}

/******************************************************************************************
void CIntChessDoc::Serialize(CArchive& ar)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::Serialize
返回值    : void
参数列表  : 
  参数1: CArchive& ar
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
        CChessRule::Save(ar);
    }
    else
    {
        // TODO: add loading code here
        //打开棋局文件的操作由CIntChessDoc::OnOpenDocument函数完成
        //CChessRule::Open(ar);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CIntChessDoc diagnostics

#ifdef _DEBUG
void CIntChessDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CIntChessDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/******************************************************************************************
void CIntChessDoc::OnSetupExchange()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::OnSetupExchange
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessDoc::OnSetupExchange()
{
    // TODO: Add your command handler code here
    CChessRule::OnExchangeChessMan();
}

/******************************************************************************************
void CIntChessDoc::OnUpdateSetupExchange(CCmdUI* pCmdUI)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::OnUpdateSetupExchange
返回值    : void
参数列表  : 
  参数1: CCmdUI* pCmdUI
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessDoc::OnUpdateSetupExchange(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(!CChessRule::IsStarted());
}

/******************************************************************************************
BOOL CIntChessDoc::SaveModified()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::SaveModified
返回值    : BOOL
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CIntChessDoc::SaveModified()
{
    // TODO: Add your specialized code here and/or call the base class
    if (!IsModified())
    {
        return TRUE;        // ok to continue
    }

    // get name/title of document
    CString name;
    if (m_strPathName.IsEmpty())
    {
        // get name based on caption
        name = m_strTitle;
        if (name.IsEmpty())
        {
            VERIFY(name.LoadString(AFX_IDS_UNTITLED));
        }
    }
    else
    {
        // get name based on file title of path name
        name = m_strPathName;
    }

    CString prompt;
    AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
    switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
    {
    case IDCANCEL:
        return FALSE;       // don't continue

    case IDYES:
        // If so, either Save or Update, as appropriate
        if (!DoFileSave())
        {
            return FALSE;       // don't continue
        }
        break;

    case IDNO:
        // If not saving changes, revert the document
        break;

    default:
        ASSERT(FALSE);
        break;
    }

    return TRUE;    // keep going
}

/******************************************************************************************
BOOL CIntChessDoc::OnOpenDocument(LPCTSTR lpszPathName)
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::OnOpenDocument
返回值    : BOOL
参数列表  : 
  参数1: LPCTSTR lpszPathName
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
BOOL CIntChessDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    // TODO: Add your specialized creation code here
    if (IsModified())
    {
        TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");
    }

    BOOL bRet = TRUE;

    CFileException fe;
    CFile* pFile = GetFile(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe);
    if (NULL == pFile)
    {
        ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
        return FALSE;
    }

    DeleteContents();
    SetModifiedFlag();  // dirty during de-serialize

    CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
    loadArchive.m_pDocument = this;
    loadArchive.m_bForceFlat = FALSE;
    try
    {
        CWaitCursor wait;
        if (pFile->GetLength() != 0)
        {
            bRet = CChessRule::Open(loadArchive);   // load me
        }
        loadArchive.Close();
        ReleaseFile(pFile, FALSE);
    }
    catch (CException* e)
    {
        //读取棋局文件发生异常，应将用户模式还原为默认模式，即手动模式
        //否则用户将无法正常关闭应用程序
        CChessRule::SetUserMode(MANUALMODE);
        SetModifiedFlag(TRUE);

        ASSERT(e->IsKindOf(RUNTIME_CLASS(CException)));
        UNUSED(e);

        ReleaseFile(pFile, TRUE);
        DeleteContents();   // remove failed contents

        try
        {
            ReportSaveLoadException(lpszPathName, e, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
        }
        catch (CException* e)
        {
            ASSERT(e->IsKindOf(RUNTIME_CLASS(CException)));
            e->Delete();
        }

        e->Delete();

        return FALSE;
    }

    SetModifiedFlag(!bRet);     // start off with unmodified

    return bRet;
}

/******************************************************************************************
void CIntChessDoc::OnFileSave()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::OnFileSave
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessDoc::OnFileSave()
{
    // TODO: Add your command handler code here
    if (AUTOMODE == CChessRule::GetUserMode())
    {
        return;
    }

    CDocument::OnFileSave();
}

/******************************************************************************************
void CIntChessDoc::OnFileSaveAs()
作者      : tangjs520       创建日期: 2004-9-29
函数名    : CIntChessDoc::OnFileSaveAs
返回值    : void
参数列表  : 无
描述      : 
调用关系  : 
被调用关系: 
备注      : 
修改记录  : 
******************************************************************************************/
void CIntChessDoc::OnFileSaveAs()
{
    // TODO: Add your command handler code here
    if (AUTOMODE == CChessRule::GetUserMode())
    {
        return;
    }

    CDocument::OnFileSaveAs();
}
