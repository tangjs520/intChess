/******************************************************************************************
��Ȩ����    : 2003-2004, �������������, ��������Ȩ��
�ļ���      : intChessDoc.cpp
����        : tangjs520
�汾        : 1.0
��������    : 2004-9-29
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
#include "intChessDoc.h"

#include "ChessRule.h"          //���������(CChessRule)��ͷ�ļ�

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
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::CIntChessDoc
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CIntChessDoc::CIntChessDoc()
{
    //��ʹ�����������֮ǰ�������ʼ���þ�̬����
    CChessRule::m_pIntChessDoc = this;
}

/******************************************************************************************
CIntChessDoc::~CIntChessDoc()
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::~CIntChessDoc
����ֵ    : ��
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
CIntChessDoc::~CIntChessDoc()
{
}

/******************************************************************************************
BOOL CIntChessDoc::OnNewDocument()
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::OnNewDocument
����ֵ    : BOOL
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
BOOL CIntChessDoc::OnNewDocument()
{
    return CChessRule::OnGameNew();
}

/******************************************************************************************
void CIntChessDoc::Serialize(CArchive& ar)
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::Serialize
����ֵ    : void
�����б�  : 
  ����1: CArchive& ar
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
        //������ļ��Ĳ�����CIntChessDoc::OnOpenDocument�������
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
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::OnSetupExchange
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CIntChessDoc::OnSetupExchange()
{
    // TODO: Add your command handler code here
    CChessRule::OnExchangeChessMan();
}

/******************************************************************************************
void CIntChessDoc::OnUpdateSetupExchange(CCmdUI* pCmdUI)
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::OnUpdateSetupExchange
����ֵ    : void
�����б�  : 
  ����1: CCmdUI* pCmdUI
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
******************************************************************************************/
void CIntChessDoc::OnUpdateSetupExchange(CCmdUI* pCmdUI)
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(!CChessRule::IsStarted());
}

/******************************************************************************************
BOOL CIntChessDoc::SaveModified()
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::SaveModified
����ֵ    : BOOL
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::OnOpenDocument
����ֵ    : BOOL
�����б�  : 
  ����1: LPCTSTR lpszPathName
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
        //��ȡ����ļ������쳣��Ӧ���û�ģʽ��ԭΪĬ��ģʽ�����ֶ�ģʽ
        //�����û����޷������ر�Ӧ�ó���
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
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::OnFileSave
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
����      : tangjs520       ��������: 2004-9-29
������    : CIntChessDoc::OnFileSaveAs
����ֵ    : void
�����б�  : ��
����      : 
���ù�ϵ  : 
�����ù�ϵ: 
��ע      : 
�޸ļ�¼  : 
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
