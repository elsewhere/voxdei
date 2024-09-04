# Microsoft Developer Studio Generated NMAKE File, Based on 64kentry.dsp
!IF "$(CFG)" == ""
CFG=64kentry - Win32 Debug
!MESSAGE No configuration specified. Defaulting to 64kentry - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "64kentry - Win32 Release" && "$(CFG)" != "64kentry - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "64kentry.mak" CFG="64kentry - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "64kentry - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "64kentry - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "64kentry - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\64kentry.exe"


CLEAN :
	-@erase "$(INTDIR)\alku.obj"
	-@erase "$(INTDIR)\amino.obj"
	-@erase "$(INTDIR)\atomi.obj"
	-@erase "$(INTDIR)\beziercurve.obj"
	-@erase "$(INTDIR)\bigbang.obj"
	-@erase "$(INTDIR)\dna.obj"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\galaksi.obj"
	-@erase "$(INTDIR)\hermitecurve.obj"
	-@erase "$(INTDIR)\ihminen.obj"
	-@erase "$(INTDIR)\kukka.obj"
	-@erase "$(INTDIR)\life.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\matikka.obj"
	-@erase "$(INTDIR)\matrix.obj"
	-@erase "$(INTDIR)\meduusa.obj"
	-@erase "$(INTDIR)\meri.obj"
	-@erase "$(INTDIR)\molekyyli.obj"
	-@erase "$(INTDIR)\primitives.obj"
	-@erase "$(INTDIR)\ranta.obj"
	-@erase "$(INTDIR)\Script1.res"
	-@erase "$(INTDIR)\solut.obj"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\spline.obj"
	-@erase "$(INTDIR)\texture.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vector.obj"
	-@erase "$(INTDIR)\win32.obj"
	-@erase "$(OUTDIR)\64kentry.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\64kentry.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x40b /fo"$(INTDIR)\Script1.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\64kentry.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib opengl32.lib glu32.lib winmm.lib minifmod.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\64kentry.pdb" /machine:I386 /out:"$(OUTDIR)\64kentry.exe" 
LINK32_OBJS= \
	"$(INTDIR)\alku.obj" \
	"$(INTDIR)\amino.obj" \
	"$(INTDIR)\atomi.obj" \
	"$(INTDIR)\bigbang.obj" \
	"$(INTDIR)\dna.obj" \
	"$(INTDIR)\ihminen.obj" \
	"$(INTDIR)\kukka.obj" \
	"$(INTDIR)\life.obj" \
	"$(INTDIR)\meduusa.obj" \
	"$(INTDIR)\meri.obj" \
	"$(INTDIR)\molekyyli.obj" \
	"$(INTDIR)\ranta.obj" \
	"$(INTDIR)\solut.obj" \
	"$(INTDIR)\beziercurve.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\hermitecurve.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\matikka.obj" \
	"$(INTDIR)\matrix.obj" \
	"$(INTDIR)\primitives.obj" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\spline.obj" \
	"$(INTDIR)\texture.obj" \
	"$(INTDIR)\vector.obj" \
	"$(INTDIR)\win32.obj" \
	"$(INTDIR)\Script1.res" \
	"$(INTDIR)\galaksi.obj"

"$(OUTDIR)\64kentry.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "64kentry - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\64kentry.exe"


CLEAN :
	-@erase "$(INTDIR)\alku.obj"
	-@erase "$(INTDIR)\amino.obj"
	-@erase "$(INTDIR)\atomi.obj"
	-@erase "$(INTDIR)\beziercurve.obj"
	-@erase "$(INTDIR)\bigbang.obj"
	-@erase "$(INTDIR)\dna.obj"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\galaksi.obj"
	-@erase "$(INTDIR)\hermitecurve.obj"
	-@erase "$(INTDIR)\ihminen.obj"
	-@erase "$(INTDIR)\kukka.obj"
	-@erase "$(INTDIR)\life.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\matikka.obj"
	-@erase "$(INTDIR)\matrix.obj"
	-@erase "$(INTDIR)\meduusa.obj"
	-@erase "$(INTDIR)\meri.obj"
	-@erase "$(INTDIR)\molekyyli.obj"
	-@erase "$(INTDIR)\primitives.obj"
	-@erase "$(INTDIR)\ranta.obj"
	-@erase "$(INTDIR)\Script1.res"
	-@erase "$(INTDIR)\solut.obj"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\spline.obj"
	-@erase "$(INTDIR)\texture.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\vector.obj"
	-@erase "$(INTDIR)\win32.obj"
	-@erase "$(OUTDIR)\64kentry.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\64kentry.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x40b /fo"$(INTDIR)\Script1.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\64kentry.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\64kentry.pdb" /machine:I386 /out:"$(OUTDIR)\64kentry.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\alku.obj" \
	"$(INTDIR)\amino.obj" \
	"$(INTDIR)\atomi.obj" \
	"$(INTDIR)\bigbang.obj" \
	"$(INTDIR)\dna.obj" \
	"$(INTDIR)\ihminen.obj" \
	"$(INTDIR)\kukka.obj" \
	"$(INTDIR)\life.obj" \
	"$(INTDIR)\meduusa.obj" \
	"$(INTDIR)\meri.obj" \
	"$(INTDIR)\molekyyli.obj" \
	"$(INTDIR)\ranta.obj" \
	"$(INTDIR)\solut.obj" \
	"$(INTDIR)\beziercurve.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\hermitecurve.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\matikka.obj" \
	"$(INTDIR)\matrix.obj" \
	"$(INTDIR)\primitives.obj" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\spline.obj" \
	"$(INTDIR)\texture.obj" \
	"$(INTDIR)\vector.obj" \
	"$(INTDIR)\win32.obj" \
	"$(INTDIR)\Script1.res" \
	"$(INTDIR)\galaksi.obj"

"$(OUTDIR)\64kentry.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("64kentry.dep")
!INCLUDE "64kentry.dep"
!ELSE 
!MESSAGE Warning: cannot find "64kentry.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "64kentry - Win32 Release" || "$(CFG)" == "64kentry - Win32 Debug"
SOURCE=.\Effects\alku.cpp

"$(INTDIR)\alku.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\amino.cpp

"$(INTDIR)\amino.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\atomi.cpp

"$(INTDIR)\atomi.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\bigbang.cpp

"$(INTDIR)\bigbang.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\dna.cpp

"$(INTDIR)\dna.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\galaksi.cpp

"$(INTDIR)\galaksi.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\ihminen.cpp

"$(INTDIR)\ihminen.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\kukka.cpp

"$(INTDIR)\kukka.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\life.cpp

"$(INTDIR)\life.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\meduusa.cpp

"$(INTDIR)\meduusa.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\meri.cpp

"$(INTDIR)\meri.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\molekyyli.cpp

"$(INTDIR)\molekyyli.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\ranta.cpp

"$(INTDIR)\ranta.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Effects\solut.cpp

"$(INTDIR)\solut.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\beziercurve.cpp

"$(INTDIR)\beziercurve.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\font.cpp

"$(INTDIR)\font.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hermitecurve.cpp

"$(INTDIR)\hermitecurve.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\main.cpp

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\matikka.cpp

"$(INTDIR)\matikka.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\matrix.cpp

"$(INTDIR)\matrix.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\primitives.cpp

"$(INTDIR)\primitives.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sound.cpp

"$(INTDIR)\sound.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\spline.cpp

"$(INTDIR)\spline.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\texture.cpp

"$(INTDIR)\texture.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\vector.cpp

"$(INTDIR)\vector.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\win32.cpp

"$(INTDIR)\win32.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Script1.rc

"$(INTDIR)\Script1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

