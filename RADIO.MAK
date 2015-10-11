# Microsoft Visual C++ generated build script - Do not modify

PROJ = RADIO
DEBUG = 1
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\USERS\NIK\SRC\FELEGARA\SCANNER\RADIO\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = RADIO.C     
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /W3 /AM /Od /D "_DOS" /FR 
CFLAGS_R_DEXE = /nologo /Gs /G2 /W3 /AM /Ox /D "NDEBUG" /D "_DOS" /FR 
LFLAGS_D_DEXE = /NOLOGO /NOE /STACK:5120 /ONERROR:NOEXE 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE 
LIBS_D_DEXE = oldnames mlibce 
LIBS_R_DEXE = oldnames mlibce 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = ..\..\..\..\..\..\PRG16BIT\TELXON\RF_NEW\PKTDRVR\MSCLIBS\PKTM.LIB ..\..\..\..\..\..\PRG16BIT\TELXON\RF_NEW\SRFTXP\MSCLIBS\SRFTXPM.LIB \
	..\..\..\..\..\..\PRG16BIT\TELXON\TLXNLIBS\TLIBS\TLIBM.LIB ..\..\..\..\..\..\PRG16BIT\TELXON\TLXNLIBS\TSDLIBS\TSDM.LIB 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = RADIO.SBR \
		TELXON.SBR \
		INPAREA.SBR


RADIO_DEP = c:\users\nik\src\felegara\scanner\costanti.h \
	c:\users\nik\src\felegara\scanner\telxon.h \
	c:\users\nik\src\felegara\scanner\radio\ptap.h \
	c:\users\nik\src\felegara\scanner\radio\snap.h \
	c:\users\nik\src\felegara\scanner\radio\tosap.h \
	c:\users\nik\src\felegara\scanner\radio\srftxp.h \
	c:\users\nik\src\felegara\scanner\tlxcnst.h \
	c:\users\nik\src\felegara\scanner\radio\pktdrvr.h \
	c:\users\nik\src\felegara\scanner\radio\pktss.h


TELXON_DEP = c:\users\nik\src\felegara\scanner\telxon.h \
	c:\users\nik\src\felegara\scanner\tlxcnst.h \
	c:\users\nik\src\felegara\scanner\costanti.h


INPAREA_DEP = c:\users\nik\src\felegara\scanner\telxon.h \
	c:\users\nik\src\felegara\scanner\costanti.h


PKTM_DEP = 

SRFTXPM_DEP = 

TLIBM_DEP = 

TSDM_DEP = 

all:	$(PROJ).EXE $(PROJ).BSC

RADIO.OBJ:	RADIO.C $(RADIO_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c RADIO.C

TELXON.OBJ:	..\TELXON.C $(TELXON_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\TELXON.C

INPAREA.OBJ:	..\INPAREA.C $(INPAREA_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ..\INPAREA.C

$(PROJ).EXE::	RADIO.OBJ TELXON.OBJ INPAREA.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
RADIO.OBJ +
TELXON.OBJ +
INPAREA.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\prg16bit\pxeng30\c\lib\+
..\..\..\..\..\..\PRG16BIT\TELXON\RF_NEW\PKTDRVR\MSCLIBS\PKTM.LIB+
..\..\..\..\..\..\PRG16BIT\TELXON\RF_NEW\SRFTXP\MSCLIBS\SRFTXPM.LIB+
..\..\..\..\..\..\PRG16BIT\TELXON\TLXNLIBS\TLIBS\TLIBM.LIB+
..\..\..\..\..\..\PRG16BIT\TELXON\TLXNLIBS\TSDLIBS\TSDM.LIB+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
