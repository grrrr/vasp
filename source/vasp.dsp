# Microsoft Developer Studio Project File - Name="vasp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=vasp - Win32 Debug
!MESSAGE Dies ist kein g�ltiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und f�hren Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "vasp.mak".
!MESSAGE 
!MESSAGE Sie k�nnen beim Ausf�hren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "vasp.mak" CFG="vasp - Win32 Debug"
!MESSAGE 
!MESSAGE F�r die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "vasp - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vasp - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "max/vasp/source"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "vasp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\msvc"
# PROP Intermediate_Dir "..\msvc"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VASP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /Ob2 /I "c:\programme\audio\pd\src" /I "f:\prog\max\flext" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PD" /D "NT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib c:\programme\audio\pd\bin\pd.lib ..\..\flext\msvc\flext-pdwin.lib /nologo /dll /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "vasp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\msvc-debug"
# PROP Intermediate_Dir "..\msvc-debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VASP_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "c:\programme\audio\pd\src" /I "f:\prog\max\flext" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PD" /D "NT" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib c:\programme\audio\pd\bin\pd.lib ..\..\flext\msvc-debug\flext-pdwin.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "vasp - Win32 Release"
# Name "vasp - Win32 Debug"
# Begin Group "doc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\license.txt
# End Source File
# Begin Source File

SOURCE=..\readme.txt
# End Source File
# End Group
# Begin Group "vasp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mixfft.cpp
# End Source File
# Begin Source File

SOURCE=.\rdx2fft.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__ctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__ext.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__fft.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__flt.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__gen.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__math.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__op.h
# End Source File
# Begin Source File

SOURCE=.\vasp__resmp.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__rng.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__simple.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp__util.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\imp.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp_base.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp_op.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp_unbin.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp_v.cpp
# End Source File
# End Target
# End Project
