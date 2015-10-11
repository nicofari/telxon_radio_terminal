/* 
 * Telxon.c
 * 
 * Routine comuni
 *
 * 11/6/97  Aggiunto nella lista anche il totale dei cartoni
 * 
 * 18/6/97  Modifica a CheckDigitOk. Se il barcode comincia per 0 cambia il punto di partenza
 *          per il calcolo del CD. Le variabili usate per il test sono ora int e non piu' char
 *          Modifico di nuovo perche' i codici parmalat contano lo 0 all'inizio come parte del
 *          messaggio invece che aggiungerlo DOPO aver calcolato il CD
 *          
 */             
 
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <process.h>
#include <stdlib.h>
#include "telxon.h"
#include "tlxcnst.h"
#include "costanti.h"

/* ACHTUNG! Questo e' dichiarato in scan2.c!! */
//extern unsigned char far *fptr;    /* temporary far pointer */ 

// Tabella dei totali per prodotto
#define MAX_PRODOTTI  30

typedef struct 
{
  char  CodPro[CODPRO_LEN+1];
  int   Count;
  int   Cartoni;
} sProdotti;

sProdotti _Tab[MAX_PRODOTTI];

int _LastTab;
 
char _fname[32] = {0};
VRD  _vrd[] = 
{ 
  { "MAGIC", (99) },
  { "BAR",   REC_CARICO_LEN },    
  { 0 , 0 }  
};

char _fbname[32]={0};
VRD _vrb[] =
{
  { "MAGIC", (99)        },
  { "CODP",  REC_CARICO_LEN },
  { 0, 0 }
};

void  GetVal(const char *prompt, const char *format, int *addr);
void  Cls();
void  BarScanOff();
void  BarScanOn();
void  BarScanDev(int device_type);
void  SetupBarcodes(const TipiBarcode cTipoBc);
void  Error(const char* msg);
void  IncProd(const char* CodPro,const char* sCartoni);
int   CercaProd(const char* CodPro);
void  StampaRiepilogo();
int   AddProd(const char* CodPro);
void  Beep();
BOOL  bNumeric(const char* sBarcode);
BOOL  bCheckDigitOK(const char* label, const int debug);
void  ExecPcomm();
void  ListaFile(const int iOrdine); 
//void  ListaFilePerOrdine(const int iOrdine); 
void  ParseRiga(const char* buffer, char* CodPallet, char* CodPro, char* cOrdine, char* sCartoni);
void  CopiaStr(char* sDst, const char* sSrc, const int start, const int stop);
void  MyStrCat(char* sDst, const char* sSrc);
void  Overwrite(char* sDst, const char* sSrc, const int start, const int num);
void  Fill(char* sDst, char c, const int stop);
void  Tasto();
unsigned int get_tmout();
unsigned char far *fptr;    /* temporary far pointer */
 
/************************************************************ 
*
* Routine TELXON
*
*************************************************************/

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   Tasto                                              *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure waits for a key to be hit.                          *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void action()                                                      *    
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/
void Tasto()
{
  printf("Premi un tasto");
//printf("< Hit any key >");
  while (!kbhit());
  getch();
  printf("\n");
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   get_tmout                                          *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure returns the PTC timeout value.                      *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   int get_tmout()                                                    *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   Returns timeout.                                                   *
*                                                                      *
\**********************************************************************/
unsigned int get_tmout()
{
    union REGS iregs;
    union REGS oregs;

    /*  Get current user timeout */
    iregs.h.ah = 0xd2;
    int86(0x15, &iregs, &oregs);    

    /* Return it */
    return(oregs.x.cx);
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   enable_tmout                                       *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure enables PTC inactivity timeouts.                    *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void enable_tmout(tmout)                                           *
*                                                                      *
* INPUT:                                                               *
*   int tmout   -   Timeout value                                      *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/
void enable_tmout(tmout)
unsigned int tmout;
{
    union REGS iregs;
    union REGS oregs;

    iregs.h.ah = 0xd1;
    iregs.x.cx = tmout;
    int86(0x15, &iregs, &oregs);    
}                                   

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   disable_tmout                                      *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure disables the PTC inactivity timeout.                *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void disable_tmout()                                               *    
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/
void disable_tmout()
{
    union REGS iregs;
    union REGS oregs;
    
    iregs.h.ah = 0xd1;
    iregs.x.cx = 0;
    int86(0x15, &iregs, &oregs);    
}


/************************************************************
*                 *
* PROCEDURE NAME: ConfigBarScan         *
*                 *
* DESCRIPTION:                *
*   This routine configures the bar code scanner to be      *
*   pencil or laser using the TSR functionn 82h.      *
*                 *
************************************************************/
void BarScanDev(int dev_type) /* returns nothing */
{
  /* set up to call the TSR */
  iregs.h.ah = CONFIG_BAR_SCAN;
  iregs.h.dl = dev_type;
  /* call the TSR */
  int86(INTERFACE_VEC, &iregs, &oregs);
}

void BarScanOn()
{
  iregs.h.ah = BAR_SCAN_ON;
  iregs.h.al = WEDGE_OFF;
  int86x(INTERFACE_VEC, &iregs, &oregs, &segregs); 
}


/************************************************************
*                 *
* PROCEDURE NAME: BarScanOff            *
*                 *
* DESCRIPTION:                *
*   This routine turns off the bar code scanner using the   *
*   wand TSR function 81h.            *
*                 *
************************************************************/
void BarScanOff() /* returns nothing */
{
  /* set up to call the TSR */
  iregs.h.ah = BAR_SCAN_OFF;
  /* call the TSR */
  int86x(INTERFACE_VEC, &iregs, &oregs, &segregs);
}

/**********************************************************************\
*                        *
* DATE: 09/19/89    WRITTEN BY:    Telxon O/S        *
*                        *
* PROCEDURE NAME: clear_screen                 *
*                        *
* DESCRIPTION:                     *
* This procedure makes a BIOS video interrupt to clear the       *
*   screen.                    *
*                        *
* CALLING SEQUENCE:                  *
* void clear_screen()                *
*                        *
* INPUT:  None                     *
*                        *
* OUTPUT: None                     *
*                        *
* VARIABLES: None.                   *
*                        *
* SUBROUTINES: None.                   *
*                        *
\**********************************************************************/
void Cls()
{
/* structures used to set up for interrupts */
  union REGS  iregs;
  union REGS  oregs;
  iregs.h.ah = CLEAR_SCREEN;
  iregs.h.al = 0;
  int86(VIDEO_VEC, &iregs, &oregs);
}

/**********************************************************************\
*                        *
* DATE: 5/24/88     WRITTEN BY:    Telxon O/S        *
*                        *
* PROCEDURE NAME: GetVal                 *
*                        *
* DESCRIPTION:                     *
*   This routine issues a prompt for input of any kind and         *
* assigns the value to valaddr.  If the input stream cannot      *
* be matched to the given format, the routine re-issues the      *
* prompt.  If a carriage return is entered, a null string is     *
* returned in valaddr.                 *
*                        *
* CALLING SEQUENCE:                  *
*   void GetVal(prompt, format, valaddr)             *
*   char *prompt, *format;                 *
*   unsigned *valaddr;                 *
*                        *
* INPUT:                     *
*   prompt    prompt to be issued before read          *
*   format    scanf() format to be used          *
*   valaddr   address of data to be input          *
*                        *
* OUTPUT: None, but reads input data into valaddr.           *
*                        *
\**********************************************************************/
void GetVal(const char* prompt, const char* format, int* valaddr)
{
    int done = 0;
    char buff[100];

    while(done != 1)
    {
    printf(prompt);
      gets(buff);
      if(buff[0] == '\0') 
      {       /* check for carriage return */
      *valaddr = '\0';
//      *valaddr = 13;
      done = 1;
    }
    else 
      if(sscanf(buff, format, valaddr) != 0)
      {
      done = 1;
      printf("\n");
      } 
      else 
          printf("Dato inserito non valido\n");
    }                   
}

void ClearTab()
{
  int i, j;
  for (i=0; i<MAX_PRODOTTI; i++)
  { 
    for (j=0; j<CODPRO_LEN; j++)
      _Tab[i].CodPro[j] = '\0';
    _Tab[i].Count     = 0;
    _Tab[i].Cartoni   = 0;
  }                                   
}

// Trova il primo libero e lo compila con il codice CodPro
int AddProd(const char* CodPro)
{
  int i;
  for (i=0; i<MAX_PRODOTTI; i++)
    if (_Tab[i].CodPro[0] == '\0')
    {
      strcpy(_Tab[i].CodPro, CodPro);
      _LastTab = i;
      return i;
    }
  _LastTab = i;
  return i;
}
  
// Incrementa di 1 il contatore per il codice CodPro. 
// Aggiunge il codice se non gia' presente in tabella
void IncProd(const char* CodPro, const char* sCartoni)
{
  int i, iCartoni;
  
  iCartoni = atoi(sCartoni);
  i = CercaProd(CodPro);
  if (i < 0)
    i = AddProd(CodPro);
  _Tab[i].Count++;      
  _Tab[i].Cartoni = _Tab[i].Cartoni + iCartoni;
  
}

// Ritorna la posizione del codice CodPro nell'array _Tab. -1 se non presente
int CercaProd(const char* CodPro)
{
  int i;
  for (i=0; i<MAX_PRODOTTI; i++)
  {
    if ( strncmp(_Tab[i].CodPro, CodPro, CODPRO_LEN) == 0 )
      return i;
  }
  return -1;
}


// Stampa il riepilogo per codice prodotto
void StampaRiepilogo()
{
  int i, Riga;
  Riga = 0;     
  printf("\nCodice Pallet Cartoni");
  printf("---------------------");
  for (i=0; i<_LastTab+1; i++, Riga++)
  {
    printf("%s %6d %7d", _Tab[i].CodPro, _Tab[i].Count, _Tab[i].Cartoni);
    if (Riga == MAX_RIGHE-1)
    {   
      ttycurstr("Un tasto per cont. ", 0);
      getch();
      ttycurstr("\n", 0);
      Riga = 0;
    }
  }
  ttycurstr("Un tasto per cont. ", 0);
  getch();
  ttycurstr("\n", 0);
  Riga = 0;
}

void Beep()
{
  ttychar(0x07);  //  beep
}

void Error(const char* msg)
{
  Beep();
  writestr(0,msg,0,INVERSE_VID);
  ttycurstr("\nPremere un tasto per continuare..\n",36);
  getch();
}


void ExecPcomm()
{
//  _spawnl(_P_WAIT, "PCOMM.EXE", "PCOMM.EXE", "PCOMM.DAT", NULL);
  spawnl(P_WAIT, "PCOMM.EXE", "PCOMM.EXE", "PCOMM.DAT", NULL);
}

BOOL bNumeric(const char* sBarcode)
{
  int i;
  for (i=0; sBarcode[i] != NULL; i++)
    if (!isdigit(sBarcode[i]))
      return FALSE;
  return TRUE;
}

// Suddivide una riga letta dal file pallet in codpallet, codpro, ordine
void ParseRiga(const char* buffer, char* CodPallet, char* CodPro, char* cOrdine, char* sCartoni)
{
// This doesn't work!                                                              
//  strncpy (CodPallet, buffer[iStartBarcode], CODPALLET_LEN);
  /*CopiaStr(CodPallet, buffer, START_CODPALLET, START_CODPALLET+CODPALLET_LEN);
  CopiaStr(CodPro,    buffer, START_CODPRO,    START_CODPRO+CODPRO_LEN);
  CopiaStr(sCartoni,  buffer, START_CARTONI,   START_CARTONI+CARTONI_LEN);    */
  //*cOrdine = buffer[OLD_BARCODE_LEN];
  CopiaStr(CodPallet, buffer, 0, CODPALLET_LEN);  
  CopiaStr(CodPro,    buffer, CODPALLET_LEN,   CODPALLET_LEN+CODPRO_LEN);
  CopiaStr(sCartoni,  buffer, CODPALLET_LEN+CODPRO_LEN, CODPALLET_LEN+CODPRO_LEN+CARTONI_LEN);    
// Prendo l'ultimo carattere che e' l'ordine       
  *cOrdine = buffer[REC_CARICO_LEN-1];  
}

void ListaFile(const int iOrdine)
{
  int Riga, stat, iNumPallet;
  char  cOrdine;
  char  buffer    [REC_CARICO_LEN+1]; 
  char  CodPallet [CODPALLET_LEN+1];
  char  CodPro    [CODPRO_LEN+1];              
  char  sCartoni  [CARTONI_LEN+1];
  BOOL  Eof = FALSE;
  char  cOrdineRichiesto = iOrdine + '0';
  
  Cls();
  ClearTab();
// Puntatore alla tabella
  _LastTab = 0;    
// Conta righe sullo schermo
  Riga = 0;
// Numera i pallet
  iNumPallet = 1;  
  tfseek(_fname, FD_FIRST);
  while (!Eof)
  {
    stat = tfread(_fname,  buffer);
    buffer [REC_CARICO_LEN] = '\0';
    Eof = stat == END_FOUND;      
    if (Eof) break;
    ParseRiga(buffer, CodPallet, CodPro, &cOrdine, sCartoni);
    printf("%3d %s %s %c    %s\n", iNumPallet, CodPallet, CodPro, cOrdine, sCartoni);
// Incrementa di due perche' ora c'e' anche la riga con i cartoni    
    Riga+=2;                        
    iNumPallet++;
    if (Riga == MAX_RIGHE-1)
    {
      ttycurstr("Un tasto per cont. ",0);
      getch();
      ttycurstr("\n",0);
      Riga = 0;
    }
    IncProd(CodPro,sCartoni);    
  }
  ttycurstr("Un tasto per cont.",0);
  getch();   
  ttycurstr("\n",0);
  Riga = 0;

  StampaRiepilogo();
}

/*
void ListaFilePerOrdine(const int iOrdine)
{
  int Riga, stat, iNumPallet;
  char  cOrdine, cOrdineRichiesto;
  char  buffer    [REC_CARICO_LEN+1]; 
  char  CodPallet [CODPALLET_LEN+1];
  char  CodPro    [CODPRO_LEN+1];              
  char  sCartoni  [3];
  BOOL  Eof = FALSE;

  cOrdineRichiesto = iOrdine + '0';
  Cls();
  ClearTab();
// Puntatore alla tabella
  _LastTab = 0;    
// Conta righe sullo schermo
  Riga = 0;
// Numera i pallet
  iNumPallet = 1;  
  tfseek(_fname, FD_FIRST);
  while (!Eof)
  {
    stat = tfread(_fname,  buffer);
    buffer [REC_CARICO_LEN] = '\0';
    Eof = stat == END_FOUND;      
    if (Eof) break;
    ParseRiga(buffer, CodPallet, CodPro, &cOrdine);
#ifdef _DEBUG
  printf("cOrdRic=%c cOrd=%c\n",cOrdineRichiesto,cOrdine); 
  Tasto(); 
#endif    
    if (cOrdine == cOrdineRichiesto) {
      CopiaStr(sCartoni, buffer, START_CARTONI, START_CARTONI+2);    
      printf("%3d %s %s %c    %s\n", iNumPallet, CodPallet, CodPro, cOrdine, sCartoni);
  // Incrementa di due perche' ora c'e' anche la riga con i cartoni    
      Riga+=2;                        
      iNumPallet++;
      if (Riga == MAX_RIGHE-1)
      {
        ttycurstr("Un tasto per cont. ",0);
        getch();
        ttycurstr("\n",0);
        Riga = 0;
      }
      IncProd(CodPro,sCartoni);    
    }
  }
  ttycurstr("Un tasto per cont.",0);
  getch();   
  ttycurstr("\n",0);
  Riga = 0;

  StampaRiepilogo();
}
******************/

void MyStrCat(char* sDst, const char* sSrc)
{
  int j,i,fine;

  j = strlen(sDst);  
  fine = strlen(sSrc);
  
  for (j=0,i=0; i<fine; i++)
    sDst[j++] = sSrc[i];

  sDst[j] = NULL;
}

void Overwrite(char* sDst, const char* sSrc, const int start, const int num)
{             
  int i, j=0;
  for (i=start; i<num; i++)
    sDst[i] = sSrc[j++];
}

// Fatta perche' stncpy sembra NON funzionare...
void CopiaStr(char* sDst, const char* sSrc, const int start, const int stop)
{
  int i, j, fine;

  if (stop < 0)
    fine = strlen(sSrc);
  else
    fine = stop;
      
  for (j=0,i=start; i<fine; i++)
    sDst[j++] = sSrc[i];
    
  sDst[j] = '\0';
}

// Riempie sDst con il carattere c. 
// Se stop=-1 riempie fino a strlen, senno' fino a stop posizioni
void Fill(char* sDst, char c, const int stop)
{
  int i;
  const int len = stop == -1 ? strlen(sDst) : stop;

  for (i=0; i<len; i++)
    sDst[i] = c;  
}

// Calcola il check digit per il barcode passato come parametro. 
// Torna TRUE se il cd calcolato e' uguale a quello letto
BOOL bCheckDigitOK(const char* label, const int debug)
{                                                                
// Usa la costante NEW_BARCODE_LEN perche' solo quelli da 30 avranno il CD
  int iCD = label[NEW_BARCODE_LEN-1] - '0';
  int iCDCalc;
  int iTmp=0,iPari=0,iDispari=0,iTot=0,i,iUnita;
  int iStartPari,iStartDispari;

  iStartPari = 0;
  iStartDispari = 1;      
  for (i=iStartPari; i<NEW_BARCODE_LEN-1; i+=2)
    iPari += label[i] - '0';
  iTmp = iPari;
  iPari *= 3;               

  for (i=iStartDispari; i<NEW_BARCODE_LEN-1; i+=2)
    iDispari += label[i]-'0';

  iTot = iDispari+iPari;    
  iUnita = iTot % 10;
  if (iUnita == 0) iUnita = 10;
  iCDCalc = 10 - iUnita;
  
  if (debug) {
    printf("Metodo 1 (pari per tre) :\n");
    printf("iPari=%d,iDisp=%d\n",iPari,iDispari);
    printf("CD calc=%d cCD=%d\n\n", iCDCalc, iCD);
    printf("Metodo 2 (dispari per tre) :\n");      
    iTot = (iDispari*3)+iTmp;    
    iUnita = iTot % 10;
    if (iUnita == 0) iUnita = 10;
    iCDCalc = 10 - iUnita;
    printf("iPari=%d,iDisp=%d\n",iPari,iDispari);
    printf("CD calc=%d cCD=%d\n\n", iCDCalc, iCD);
  }
  
  return iCDCalc == iCD;
}

/************************************************************
*                                                           *
* PROCEDURE NAME: SetupBarcodes()                           *
*                                                           *
* DESCRIPTION:                                              *
*   This routine configures the scanning software for the   *
*   types of barcodes to be decoded using TSR function 83h. *
*                                 
* 1996    Modificata per leggere SOLO I2/5 e con lunghezza fissata a BARCODE_LEN
* 8.4.97  Lasciata libera la lunghezza per gestire le etichette a 28 o a 30 car.
* 
* 5.5.97  Il parametro cOp determina se includere o meno anche
*         il code 139 usato per Cambia Area                            
*
* 7.5.97  Tolto il tipo operazione. Passo il tipo di barcode da settare
*         Suppongo di accettare solo UN tipo di barcode alla volta
************************************************************/
void SetupBarcodes(const TipiBarcode bcTipo)
{                                          
  switch (bcTipo)
  {
    case bcI2su5:
//  Configure for Interleaved 2 of 5 
      barcodes[0].label_type      = 0x32;
      barcodes[0].label_length    = OLD_BARCODE_LEN;
      barcodes[0].label_length    = 0;  
//      barcodes[0].generic_options = 0;
      barcodes[0].generic_options = 1;
      barcodes[0].label_options   = 0;
      barcodes[0].drop_options    = 0;
      barcodes[0].gsn = 0;

      barcodes[1].label_type      = 0x32;
      barcodes[1].label_length    = NEW_BARCODE_LEN;
      barcodes[1].label_length    = 0;  
//      barcodes[1].generic_options = 0;
      barcodes[0].generic_options = 1;
      barcodes[1].label_options   = 0;
      barcodes[1].drop_options    = 0;
      barcodes[1].gsn = 0;

      barcodes[2].label_type = 0;
      
      
      break;
      
    case bcCode39:
//  Configure for Code 39 
      barcodes[0].label_type      = 0x28;
      barcodes[0].label_length    = 0;
      barcodes[0].generic_options = 0;
      barcodes[0].label_options   = 0;
      barcodes[0].drop_options    = 0x11;
      barcodes[0].gsn             = 0;
    //  Mark end of structure list 
      barcodes[1].label_type      = 0;
      break;
      
    default:
      printf("Barcode ignoto");
      break;
  }    
//    case bcCode128:  
/*  Configure for Code 128 */
/******************
  barcodes[2].label_type = 0x40;
  barcodes[2].label_length = 0;
  barcodes[2].generic_options = 0;
  barcodes[2].label_options = 0;
  barcodes[2].drop_options = 0;
  barcodes[2].gsn = 0;
/*  Mark end of structure list *
  barcodes[3].label_type = 0;
*****************************/    
    
//  Call tsr to setup barcodes selected 
  iregs.h.ah = SETUP_BARCODES;
//  Use temporary far pointer to get segment:offset of barcodes array
  fptr = (unsigned char far *)barcodes;
  iregs.x.dx = FP_OFF(fptr);
  segregs.ds = FP_SEG(fptr);
  int86x(INTERFACE_VEC, &iregs, &oregs, &segregs);
}
