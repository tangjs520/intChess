; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDigitalClockWnd
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "intchess.h"
LastPage=0

ClassCount=14
Class1=CChessDeskWnd
Class2=CChessEquipmentWnd
Class3=CChessManualView
Class4=CCoordinateWnd
Class5=CDigitalClockWnd
Class6=CIntChessApp
Class7=CAboutDlg
Class8=CIntChessDoc
Class9=CIntChessView
Class10=CMainFrame
Class11=COtherView
Class12=CPawnUpDlg
Class13=CResultDlg
Class14=CStaticSplitterWnd

ResourceCount=4
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX
Resource3=IDD_PAWN_UPGRADE
Resource4=IDD_RESULT

[CLS:CChessDeskWnd]
Type=0
BaseClass=CWnd
HeaderFile=ChessDeskWnd.h
ImplementationFile=ChessDeskWnd.cpp

[CLS:CChessEquipmentWnd]
Type=0
BaseClass=CWnd
HeaderFile=ChessEquipmentWnd.h
ImplementationFile=ChessEquipmentWnd.cpp

[CLS:CChessManualView]
Type=0
BaseClass=CView
HeaderFile=ChessManualView.h
ImplementationFile=ChessManualView.cpp

[CLS:CCoordinateWnd]
Type=0
BaseClass=CWnd
HeaderFile=CoordinateWnd.h
ImplementationFile=CoordinateWnd.cpp

[CLS:CDigitalClockWnd]
Type=0
BaseClass=CWnd
HeaderFile=DigitalClockWnd.h
ImplementationFile=DigitalClockWnd.cpp
LastObject=CDigitalClockWnd
Filter=W
VirtualFilter=WC

[CLS:CIntChessApp]
Type=0
BaseClass=CWinApp
HeaderFile=intChess.h
ImplementationFile=intChess.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=intChess.cpp
ImplementationFile=intChess.cpp
LastObject=CAboutDlg

[CLS:CIntChessDoc]
Type=0
BaseClass=CDocument
HeaderFile=intChessDoc.h
ImplementationFile=intChessDoc.cpp

[CLS:CIntChessView]
Type=0
BaseClass=CView
HeaderFile=intChessView.h
ImplementationFile=intChessView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:COtherView]
Type=0
BaseClass=CView
HeaderFile=OtherView.h
ImplementationFile=OtherView.cpp

[CLS:CPawnUpDlg]
Type=0
BaseClass=CDialog
HeaderFile=PawnUpDlg.h
ImplementationFile=PawnUpDlg.cpp

[CLS:CResultDlg]
Type=0
BaseClass=CDialog
HeaderFile=ResultDlg.h
ImplementationFile=ResultDlg.cpp

[CLS:CStaticSplitterWnd]
Type=0
BaseClass=CSplitterWnd
HeaderFile=StaticSplitterWnd.h
ImplementationFile=StaticSplitterWnd.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDOK,button,1342373889
Control2=IDC_STATIC,static,1342177283
Control3=IDC_STATIC,static,1342308480
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_PAWN_UPGRADE]
Type=1
Class=CPawnUpDlg
ControlCount=6
Control1=ID_QUEEN_RADIO,button,1342308361
Control2=ID_ROOK_RADIO,button,1342177289
Control3=ID_KNIGHT_RADIO,button,1342180617
Control4=ID_BISHOP_RADIO,button,1342177289
Control5=IDOK,button,1342242817
Control6=ID_PAWNUP_GROUP,button,1342177287

[DLG:IDD_RESULT]
Type=1
Class=CResultDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_WHITE_KING,static,1342177283
Control3=IDC_BLACK_KING,static,1342177283
Control4=IDC_RESULT,static,1342308353
Control5=IDC_SCORE,static,1342308353

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_APP_EXIT
Command6=ID_SETUP_EXCHANGE
Command7=ID_APP_ABOUT
CommandCount=7

