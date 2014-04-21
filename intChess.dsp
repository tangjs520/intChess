# Microsoft Developer Studio Project File - Name="intChess" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=intChess - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "intChess.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "intChess.mak" CFG="intChess - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "intChess - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "intChess - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "intChess - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Bin\Release"
# PROP Intermediate_Dir "..\Obj\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\Obj\Release/intChess.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Msimg32.lib Winmm.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "intChess - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Bin\Debug"
# PROP Intermediate_Dir "..\Obj\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\Obj\Debug/intChess.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Msimg32.lib Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "intChess - Win32 Release"
# Name "intChess - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChessBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\ChessDeskWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChessEquipmentWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ChessGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ChessMan.cpp
# End Source File
# Begin Source File

SOURCE=.\ChessManualView.cpp
# End Source File
# Begin Source File

SOURCE=.\ChessRule.cpp
# End Source File
# Begin Source File

SOURCE=.\CoordinateWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DigitalClockWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\intChess.cpp
# End Source File
# Begin Source File

SOURCE=.\intChess.rc
# End Source File
# Begin Source File

SOURCE=.\intChessDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\intChessView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherView.cpp
# End Source File
# Begin Source File

SOURCE=.\PawnUpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChessBoard.h
# End Source File
# Begin Source File

SOURCE=.\ChessDeskWnd.h
# End Source File
# Begin Source File

SOURCE=.\ChessEquipmentWnd.h
# End Source File
# Begin Source File

SOURCE=.\ChessGrid.h
# End Source File
# Begin Source File

SOURCE=.\ChessMan.h
# End Source File
# Begin Source File

SOURCE=.\ChessManualView.h
# End Source File
# Begin Source File

SOURCE=.\ChessRule.h
# End Source File
# Begin Source File

SOURCE=.\CoordinateWnd.h
# End Source File
# Begin Source File

SOURCE=.\DigitalClockWnd.h
# End Source File
# Begin Source File

SOURCE=.\intChess.h
# End Source File
# Begin Source File

SOURCE=.\intChessDoc.h
# End Source File
# Begin Source File

SOURCE=.\intChessView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\OtherView.h
# End Source File
# Begin Source File

SOURCE=.\PawnUpDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResultDlg.h
# End Source File
# Begin Source File

SOURCE=.\StaticSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BlackChessBoard.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BlackChessMan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BlackKing.ico
# End Source File
# Begin Source File

SOURCE=.\res\ClockBackground.BMP
# End Source File
# Begin Source File

SOURCE=.\res\Demo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Grass.bmp
# End Source File
# Begin Source File

SOURCE=.\res\intChess.ico
# End Source File
# Begin Source File

SOURCE=.\res\intChess.rc2
# End Source File
# Begin Source File

SOURCE=.\res\intChessDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Number.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Stone.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\wave1.bin
# End Source File
# Begin Source File

SOURCE=.\res\white_ch.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WhiteChessBoard.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WhiteChessMan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WhiteKing.ico
# End Source File
# Begin Source File

SOURCE=.\res\Wood.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\Checked.wav
# End Source File
# Begin Source File

SOURCE=.\res\Draw.wav
# End Source File
# Begin Source File

SOURCE=.\res\Illegal.wav
# End Source File
# Begin Source File

SOURCE=.\intChess.reg
# End Source File
# Begin Source File

SOURCE=.\res\Killed.wav
# End Source File
# Begin Source File

SOURCE=.\res\Losed.wav
# End Source File
# Begin Source File

SOURCE=.\res\Moved.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\Shifted.wav
# End Source File
# Begin Source File

SOURCE=.\res\Wined.wav
# End Source File
# End Target
# End Project
