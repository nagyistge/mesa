# Microsoft Developer Studio Generated NMAKE File, Based on dcm_to_text.dsp
!IF "$(CFG)" == ""
CFG=dcm_to_text - Win32 Debug
!MESSAGE No configuration specified. Defaulting to dcm_to_text - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dcm_to_text - Win32 Release" && "$(CFG)" != "dcm_to_text - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dcm_to_text.mak" CFG="dcm_to_text - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dcm_to_text - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "dcm_to_text - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "dcm_to_text - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\dcm_to_text.exe"


CLEAN :
	-@erase "$(INTDIR)\dcm_to_text.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\dcm_to_text.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\..\ctn\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\dcm_to_text.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dcm_to_text.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=mesa_lib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\dcm_to_text.pdb" /machine:I386 /out:"$(OUTDIR)\dcm_to_text.exe" /libpath:"..\..\..\winmesa\mesa_lib\Release" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_to_text.obj"

"$(OUTDIR)\dcm_to_text.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_to_text - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\dcm_to_text.exe"


CLEAN :
	-@erase "$(INTDIR)\dcm_to_text.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\dcm_to_html.pdb"
	-@erase "$(OUTDIR)\dcm_to_text.exe"
	-@erase "$(OUTDIR)\dcm_to_text.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\ctn\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\dcm_to_text.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dcm_to_text.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=mesa_lib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\dcm_to_html.pdb" /debug /machine:I386 /out:"$(OUTDIR)\dcm_to_text.exe" /pdbtype:sept /libpath:"..\..\..\winmesa\mesa_lib\Debug" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_to_text.obj"

"$(OUTDIR)\dcm_to_text.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("dcm_to_text.dep")
!INCLUDE "dcm_to_text.dep"
!ELSE 
!MESSAGE Warning: cannot find "dcm_to_text.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "dcm_to_text - Win32 Release" || "$(CFG)" == "dcm_to_text - Win32 Debug"
SOURCE=..\..\..\..\ctn\apps\dcm_to_text\dcm_to_text.c

"$(INTDIR)\dcm_to_text.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 
