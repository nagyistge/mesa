# Microsoft Developer Studio Generated NMAKE File, Based on mesa_dictionary.dsp
!IF "$(CFG)" == ""
CFG=mesa_dictionary - Win32 Debug
!MESSAGE No configuration specified. Defaulting to mesa_dictionary - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "mesa_dictionary - Win32 Release" && "$(CFG)" != "mesa_dictionary - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mesa_dictionary.mak" CFG="mesa_dictionary - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mesa_dictionary - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "mesa_dictionary - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mesa_dictionary - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\mesa_dictionary.exe"


CLEAN :
	-@erase "$(INTDIR)\mesa_dictionary.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\mesa_dictionary.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\include" /I "..\..\..\..\ctn\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\mesa_dictionary.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mesa_dictionary.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=mesa_lib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\mesa_dictionary.pdb" /machine:I386 /out:"$(OUTDIR)\mesa_dictionary.exe" /libpath:"..\..\..\winmesa\mesa_lib\Release" 
LINK32_OBJS= \
	"$(INTDIR)\mesa_dictionary.obj"

"$(OUTDIR)\mesa_dictionary.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "mesa_dictionary - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\mesa_dictionary.exe"


CLEAN :
	-@erase "$(INTDIR)\mesa_dictionary.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\mesa_dictionary.exe"
	-@erase "$(OUTDIR)\mesa_dictionary.ilk"
	-@erase "$(OUTDIR)\mesa_dictionary.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\include" /I "..\..\..\..\ctn\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\mesa_dictionary.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\mesa_dictionary.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=mesa_lib.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\mesa_dictionary.pdb" /debug /machine:I386 /out:"$(OUTDIR)\mesa_dictionary.exe" /pdbtype:sept /libpath:"..\..\..\winmesa\mesa_lib\Debug" 
LINK32_OBJS= \
	"$(INTDIR)\mesa_dictionary.obj"

"$(OUTDIR)\mesa_dictionary.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("mesa_dictionary.dep")
!INCLUDE "mesa_dictionary.dep"
!ELSE 
!MESSAGE Warning: cannot find "mesa_dictionary.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "mesa_dictionary - Win32 Release" || "$(CFG)" == "mesa_dictionary - Win32 Debug"
SOURCE=..\..\..\apps\mesa_dictionary\mesa_dictionary.cpp

"$(INTDIR)\mesa_dictionary.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

