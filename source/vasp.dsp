# Microsoft Developer Studio Project File - Name="vasp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=vasp - Win32 Threads Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "vasp.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "vasp.mak" CFG="vasp - Win32 Threads Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "vasp - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vasp - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vasp - Win32 Threads Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "vasp - Win32 Threads Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
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
# PROP Output_Dir "..\pd-msvc\r"
# PROP Intermediate_Dir "..\pd-msvc\r"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VASP_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /O2 /Ob2 /I "c:\programme\audio\pd\src" /I "f:\prog\max\flext\source" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PD" /D "NT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib pthreadVC.lib pd.lib ..\..\flext\pd-msvc\flext-pdwin.lib /nologo /dll /machine:I386 /libpath:"c:\programme\audio\pd\bin"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "vasp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\pd-msvc\d"
# PROP Intermediate_Dir "..\pd-msvc\d"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "VASP_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /ZI /Od /I "c:\programme\audio\pd\src" /I "f:\prog\max\flext\source" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PD" /D "NT" /D "VASP_COMPACT" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib pthreadVC.lib pd.lib ..\..\flext\pd-msvc\flext_d-pdwin.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"c:\programme\audio\pd\bin"

!ELSEIF  "$(CFG)" == "vasp - Win32 Threads Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vasp___Win32_Threads_Debug"
# PROP BASE Intermediate_Dir "vasp___Win32_Threads_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\pd-msvc\td"
# PROP Intermediate_Dir "..\pd-msvc\td"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "c:\programme\audio\pd\src" /I "f:\prog\max\flext" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PD" /D "NT" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /I "c:\programme\audio\pd\src" /I "f:\prog\max\flext\source" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PD" /D "NT" /D "FLEXT_THREADS" /D "VASP_COMPACT" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib pthreadVC.lib pd.lib ..\..\flext\msvc-debug\flext-t-pdwin.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"c:\programme\audio\pd\bin"
# ADD LINK32 kernel32.lib user32.lib pthreadVC.lib pd.lib ..\..\flext\pd-msvc\flext_td-pdwin.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"c:\programme\audio\pd\bin"

!ELSEIF  "$(CFG)" == "vasp - Win32 Threads Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vasp___Win32_Threads_Release"
# PROP BASE Intermediate_Dir "vasp___Win32_Threads_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\pd-msvc\t"
# PROP Intermediate_Dir "..\pd-msvc\t"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /Ob2 /I "c:\programme\audio\pd\src" /I "f:\prog\max\flext" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PD" /D "NT" /YX /FD /c
# ADD CPP /nologo /G6 /MT /W3 /O2 /Ob2 /I "c:\programme\audio\pd\src" /I "f:\prog\max\flext\source" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PD" /D "NT" /D "FLEXT_THREADS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib pthreadVC.lib pd.lib ..\..\flext\msvc-debug\flext-t-pdwin.lib /nologo /dll /machine:I386 /libpath:"c:\programme\audio\pd\bin"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 kernel32.lib user32.lib pthreadVC.lib pd.lib ..\..\flext\pd-msvc\flext_t-pdwin.lib /nologo /dll /machine:I386 /out:"..\pd-msvc/vasp.dll" /libpath:"c:\programme\audio\pd\bin"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "vasp - Win32 Release"
# Name "vasp - Win32 Debug"
# Name "vasp - Win32 Threads Debug"
# Name "vasp - Win32 Threads Release"
# Begin Group "doc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\compile.txt
# End Source File
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

SOURCE=.\arg.cpp
# End Source File
# Begin Source File

SOURCE=.\arg.h
# End Source File
# Begin Source File

SOURCE=.\buflib.cpp
# End Source File
# Begin Source File

SOURCE=.\buflib.h
# End Source File
# Begin Source File

SOURCE=.\classes.cpp
# End Source File
# Begin Source File

SOURCE=.\classes.h
# End Source File
# Begin Source File

SOURCE=.\env.cpp
# End Source File
# Begin Source File

SOURCE=.\env.h
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\vasp.cpp
# End Source File
# Begin Source File

SOURCE=.\vasp.h
# End Source File
# Begin Source File

SOURCE=.\vbuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\vbuffer.h
# End Source File
# Begin Source File

SOURCE=.\vecblk.cpp
# End Source File
# Begin Source File

SOURCE=.\vecblk.h
# End Source File
# End Group
# Begin Group "ops"

# PROP Default_Filter ""
# Begin Group "funcs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ops_arith.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_arith.h
# End Source File
# Begin Source File

SOURCE=.\ops_assign.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_assign.h
# End Source File
# Begin Source File

SOURCE=.\ops_carith.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_carith.h
# End Source File
# Begin Source File

SOURCE=.\ops_cmp.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_cmp.h
# End Source File
# Begin Source File

SOURCE=.\ops_cplx.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_cplx.h
# End Source File
# Begin Source File

SOURCE=.\ops_dft.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_dft.h
# End Source File
# Begin Source File

SOURCE=.\ops_feature.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_feature.h
# End Source File
# Begin Source File

SOURCE=.\ops_flt.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_flt.h
# End Source File
# Begin Source File

SOURCE=.\ops_gate.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_gen.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_gen.h
# End Source File
# Begin Source File

SOURCE=.\ops_qminmax.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_rearr.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_rearr.h
# End Source File
# Begin Source File

SOURCE=.\ops_resmp.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_resmp.h
# End Source File
# Begin Source File

SOURCE=.\ops_search.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_search.h
# End Source File
# Begin Source File

SOURCE=.\ops_trnsc.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_trnsc.h
# End Source File
# Begin Source File

SOURCE=.\ops_wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ops_wnd.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\opbase.cpp
# End Source File
# Begin Source File

SOURCE=.\opbase.h
# End Source File
# Begin Source File

SOURCE=.\opdefs.h
# End Source File
# Begin Source File

SOURCE=.\opparam.cpp
# End Source File
# Begin Source File

SOURCE=.\opparam.h
# End Source File
# Begin Source File

SOURCE=.\oppermute.h
# End Source File
# Begin Source File

SOURCE=.\ops.h
# End Source File
# Begin Source File

SOURCE=.\opvecs.cpp
# End Source File
# End Group
# Begin Group "dft"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mixfft.cpp
# End Source File
# Begin Source File

SOURCE=.\rdx2fft.cpp
# End Source File
# Begin Source File

SOURCE=.\rvfft.cpp
# End Source File
# End Group
# Begin Group "objs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\obj_chns.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_frames.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_imm.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_offs.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_part.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_peaks.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_q.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_radio.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_size.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_split.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_sync.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_vasp.cpp
# End Source File
# Begin Source File

SOURCE=.\obj_vecs.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# End Target
# End Project
