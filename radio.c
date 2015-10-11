// SCAN 
//
// Lista modifiche
// 
// 7.5.96 Metto I2-5 con lunghezza fissa
//
//            
// 4.2.97  versione 2.1
//
// - Corretta ClearTab per pulire tutto CodPro (per correggere problema in visualizzazione
//   riepilogo bancali letti)     
// - Nella lista carico i pallet sono ora numerati e viene visualizzato il numero dell'ordine
//   a cui appartengono.
// - Possibilita' di cancellazione selettiva (non piu' solo dell'ultimo) di un pallet letto
// - Inserimento manuale di un codice pallet
// - Rafforzato controllo d'errore. Il codice linea (il primo carattere) viene 
//   rifiutato se diverso da 1,2,3. 
// - iOrdineCorrente non e' piu' globale
// - Non e' piu' possibile andare oltre l'ordine n.9
// - Controllo ulteriore: che tutti i caratteri letti siano numeri (v. CheckBarcode)
// - Possibilita' di inserire codici pallet duplicati (ma con ordini DIVERSI)
//   per poter fare piu' volte picking dallo stesso bancale.
//   In questo caso si torna al menu' dopo la prima lettura per evitare di dimenticare
//   lo scanner attivo in questa modalita'
//
// 7.7.97
// 
// - Passaggio da XP a TXP
//
// 21/7/97 - CheckBarcode : Se rimuove lo zero e il CD poi ricopia il barcode in sBarcode
//
            
#define VERSION "RADIO v.3.0\n\n"

#include "..\costanti.h"                
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <process.h>
#include "..\telxon.h"
#include "ptap.h"
#include "snap.h"
#include "tosap.h"
#include "srftxp.h" 
#include "..\tlxcnst.h"
#include <stdlib.h>

// Necessarie per PacketConfigure
#include "pktdrvr.h"
#include "pktss.h"


////////////////////////////////////////////////////////////////////
//
// EXTERNAL
//
////////////////////////////////////////////////////////////////////

extern unsigned char far *fptr;    /* temporary far pointer */

extern char _fname [32]; // usate da ftell, ftopen ecc.. (libreria Telxon)
extern char _fbname[32];
extern VRD _vrd[];
extern VRD _vrb[];

extern void  GetVal(const char *prompt, const char *format, int *addr);
extern void  Cls();
extern void  BarScanOff();
extern void  BarScanOn();
extern void  BarScanDev(int device_type);
extern void  Error(const char* msg);
extern void  IncProd(const char* CodPro);
extern int   CercaProd(const char* CodPro);
extern void  StampaRiepilogo();
extern int   AddProd(const char* CodPro);
extern void  Beep();
extern BOOL  bNumeric(const char* sBarcode);
extern BOOL  bCheckDigitOK(const char* label);
extern void  ExecPcomm();
//extern void  ListaFilePerOrdine(const int iOrdine);
extern void  ListaFile();
extern void  ParseRiga(const char* buffer, char* CodPallet, char* CodPro, char* cOrdine,char* sCartoni);
extern void  CopiaStr(char* sDst, const char* sSrc, const int start, const int stop);
extern void  Overwrite(char* sDst,const char* sSrc,const int start, const int num);
extern void  Tasto();
extern unsigned int  get_tmout();
extern void Fill(char* sDst, char c, const int stop);
extern unsigned int InputArea(char* sCodArea);
extern void MyStrCat(char* sDst, const char* sSrc);
extern void  SetupBarcodes(const TipiBarcode bcTipo);

////////////////////////////////////////////////////////////////////
//
// PROTOTIPI
//
////////////////////////////////////////////////////////////////////
//BOOL ReadScanData(const int iOrdineCorrente, const BOOL bPicking, const char cTipoOp);
BOOL  SendFile();
void  CheckFile(const BOOL bForcedDelete);
void  Elimina();
//BOOL  CheckBarcode(char* sBarcode,const BOOL bPicking, const int iOrdineCorrente, const char cTipoOp);
void  ManualEnter(const int iOrdineCorrente);
//void  WriteBarcode(const char * sBuffer, const int iOrdineCorrente, const int iCartoni);
void  MenuCarichi();
void  MenuCartoni();
int   CausaliCartoni(const BOOL bScarica);
int   ChiudiOrdine(const int iOrdineCorrente);
void  CancellaCarico();
void  ElaboraDatiPallet();
int   PacketConfigure();
BOOL MsgSiNo(const char* msg);
void SetupSpeed();
void TrasmettiBatch();
BOOL Tx();
BOOL  ScriviStr(const int comm_port,char* str, const int len, BOOL bMsg);
BOOL  RiceviStr(const comm_port, const char* str, BOOL bMsg);
// RF functions
unsigned char SrfConnect(const int iScanNum);
BOOL RadioTx(const char cOp, const char* sBuf, const char* sCodArea, const int iCt, int* rc);
int   TrasmettiRadio(const char cTipoOp,const char* send_buf, const char* sCodArea, 
                     const int iOrdineCorrente, const int iCartoni);         
// Legge il numero dello scanner e timeout da utilizzare
int   ReadIni(int* iScanNum, int* iRfTimeOut, int* iNetMode);
void Lettura(const char cTipoOp);
BOOL LeggiPallet();
BOOL PalletLetto();
BOOL PalletOK();
void ScriviPallet(const int iCartoni);
BOOL CaricoVuoto();
          
/////////////////////////////////////////////////////////////////////
//
// GLOBALS
//
/////////////////////////////////////////////////////////////////////
//#define XP_OK 0
#define SUCCESS 0
#define FAILURE -1

// CI STANNO 21 COLONNE                                                        
#define READY_MSG  "- Laser attivo -\nun tasto per menu'\n"
#define RX_BUF_LEN 100
#define _INI_FILE "num"
//#define LEN_RIGA_REC 7
#define LEN_RIGA_REC 9   // usata in ElaboraDatiPallet

// L'ordine aperto!!!
static int _iOrdineCorrente = 1;                                
char _sCodPro[CODPRO_LEN+1];  // compilato in ElaboraDatiPallet

static unsigned char _sRxBuf[RX_BUF_LEN+1];  // RF receive buffer 
static unsigned char _sTxBuf[REC_RF_LEN+1];
static unsigned char _sTmp[REC_RF_LEN];

static char _sCodArea[CODAREA_LEN+1];
char _sCodPallet[CODPALLET_LEN+1];
char _sRiga[REC_CARICO_LEN+1];

// Buffer per il barcode letto
static unsigned char _sScanBuf[SCAN_BUFFER_LEN];
// Etichetta senza CD e 0 iniziale
static unsigned char _sScanBufStripped[SCAN_BUFFER_LEN];

// debug mode
int _DEBUG = 0;
BOOL _SETUP=FALSE;
unsigned long _lBitrate, _lChannel_set;
unsigned int _iFrequency;

// Contatore etichette lette
int _iCtrLetti = 0;
// Numero identificativo dello scanner
int _iScanNum;
// Timeout ulteriore passato a srfrecv, srftransact ecc.
int _iRfTimeOut;
// Network mode: 0 peer-to-peer,1 non-pw-saving,2 pw-saving
int _iNetMode;

// Uso sempre il laser
const int LASER_DEVICE = 1;

// variabili usati da tftell
long far size;
long far rec_num;            

/* RF Data */
CFG_STRUCT cfg;                 /* configuration structure */
unsigned int  usr_tmout;         /* PTC user timeout */
unsigned char app_hdr_buf[100];     /* App message header buffer */
unsigned int  app_hdr_len;           /* Length of message header */

/////////////////////////////////////////////////////////////////////////////


void ManualEnter(const int iOrdineCorrente)
{  
  Cls();                                     
  memset(_sCodPallet, '\0', CODPALLET_LEN);  
  printf("Digita solo il codice pallet:\n");
  printf("Codice: \n");
  GetVal("", "%s", (int*)&_sCodPallet);
  if (strlen(_sCodPallet) != CODPALLET_LEN)
  {
    Error("Lunghezza codpallet errata");
    memset(_sCodPallet, '\0', CODPALLET_LEN);
    return;
  }        
  if (!bNumeric(_sCodPallet))
  {
    Error("Codpallet deve essere numerico");
    memset(_sCodPallet, '\0', CODPALLET_LEN);
    return;
  }
  
  //if (PalletOK() && !PalletLetto()) 
  if (!PalletLetto() && PalletOK())
  {
    ElaboraDatiPallet();        // Elabora _sRxBuf          
    printf("Lettura n.: %d \n", ++_iCtrLetti);            
  }
  
}

void Elimina()
{
  int   stat; 
  long  lNumPallet=0;
  long  lLast=0;
  char  ch, cOrdine;
  char  sCartoni[CARTONI_LEN+1];
    
  Cls();    
// Determina num.di rec. letti
  if (CaricoVuoto())    
  {
    Error("File vuoto");
    return;
  }
  
  lLast = size / REC_CARICO_LEN;  
 
  printf("Bancale (1..%d) ? ", lLast);
// Chiede il numero del bancale da eliminare  
  GetVal("", "%d", (int*)&lNumPallet);
  if (lNumPallet < 0 && lNumPallet > lLast)
    return;
  stat = tfseek(_fname, lNumPallet);
  if (stat != FUNC_SUCC)
  {
    Error("Elimina: fallita seek");
    return;         
  }

//  stat = tfread(_fname, buffer);
  stat = tfread(_fname, _sRiga);
  if (stat != FUNC_SUCC)
  {
    Error("Elimina: fallita read");
    return;
  }

  _sRiga[REC_CARICO_LEN] = '\0';

  ParseRiga(_sRiga, _sCodPallet, _sCodPro, &cOrdine, sCartoni);
  printf("Bancale:\n");
  printf("%3ld %s %s %c    %s\n", lNumPallet, _sCodPallet, _sCodPro, cOrdine, sCartoni);  
  printf("Lo cancello (s/N) ? ");
  GetVal("", "%c", (int*)&ch);  
  if (toupper(ch) == 'S')    
  {
    stat = tfdelete(_fname);
    if (stat == FUNC_SUCC)
    {
      ttycurstr("Codice cancellato\n",0);
// Diminuisce il contatore delle letture fatte      
      if (_iCtrLetti>0) --_iCtrLetti;
    }
    else
    {
      Error("Elimina: fallita delete!");
      return;
    }
  }
}

void CancellaCarico()
{
  int status;
  status = tfreset(_fname);
  if (_DEBUG) { 
    if (status == FUNC_SUCC)
      printf("File cancellato\n");
    else   
      printf("CheckFile rc = %d\n", status);
  }
}

BOOL MsgSiNo(const char* msg)
{
  char  ch='N';
  
  printf("%s (s/N)?", msg);
  GetVal("", "%c", (int*)&ch);  
  return toupper(ch) == 'S';
}

// Controlla se il file esiste gia' e se si' chiede se si vuole cancellarlo
void CheckFile(const BOOL bForcedDelete)
{
  //char  ch='N';
  BOOL bDelete;

  rec_num=0L;
  tftell((char*)_fname, &size, &rec_num);

// Se esiste gia' un file di carico chiede se deve cancellarlo
  if (rec_num > 0L)
  {         
    if (!bForcedDelete) {
      printf("C'e' gia' un carico\n");
      printf("in memoria.\n");
      bDelete = MsgSiNo("Lo cancello");
      //printf("Lo cancello (s/N)?");
      //GetVal("", "%c", &ch);  
    }
    if (bForcedDelete || bDelete)    
      CancellaCarico();
  }
}


//
// ElaboraDatiPallet()
//
// Salva in pallet.bar CODPALLET,CODPRO,CARTONI
// Scrive nelle GLOBALI:
//  
//  _sCodPro
//
void ElaboraDatiPallet()
{
  int iNumRighe;
  unsigned char sCartoni[3];
  unsigned int  iCartoni;
  int iTer, iStart;
  
// Determina quante righe ha ricevuto
  iNumRighe = strlen(_sRxBuf) / LEN_RIGA_REC;    
  if (_DEBUG) {
    printf("DatiPallet\n");
    printf("Righe: %d\n", iNumRighe);
    printf("|%s|\n",_sRxBuf);
  }
                                  
  iStart = 1;  // Il primo carattere e' il return code                                    
  for (iTer=0 ; iTer < iNumRighe; iTer++)                            
  {        
    memset(sCartoni, '\0', 3);
    CopiaStr(_sCodPro, _sRxBuf, iStart, iStart+CODPRO_LEN);       
    CopiaStr(sCartoni, _sRxBuf,iStart+CODPRO_LEN, iStart+CODPRO_LEN+3);      
    iCartoni = atoi(sCartoni);

    iStart += LEN_RIGA_REC;
    
    if (_DEBUG) 
    {
      printf("%d %s %d %d\n",iTer,_sCodPro,iCartoni,iStart);
      Tasto();
    }
    
    ScriviPallet(iCartoni);
  }
}


// Controlla se bloccato, scaricato, non presente...
BOOL PalletOk()
{                                           
  int rc;
  
  if (!RadioTx(_OP_RIGA,_sCodPallet,_sCodArea,0,&rc))
    return FALSE;
  else
  if (rc==_ERR_NON_PRESENTE) {
    Error("Bancale assente ");           
    return FALSE;
  }
  else
    if (rc==_ERR_BLOCCATO) {
      Error("Bancale bloccato");
      return FALSE;      
    }          
    else 
      if (rc==_ERR_GIA_SCARICATO) {
        Error("Gia' scaricato  ");
        return FALSE;        
      } 
        if (rc==_ERR_SEPARATO) {
          Error("Bancale separato");
          return FALSE;        
        } 
  return TRUE;           
}

//
// LeggiPallet
//
// GLOBALS:
//
// _sScanBuf, _sScanBufStripped, _sCodPallet
//
BOOL LeggiPallet()
{
  int decode_type;
  int direction;
  int label_length;
  int i;   
  int keylen=0;

  _sScanBuf[0] = NULL;
  memset(_sScanBufStripped, '\0', SCAN_BUFFER_LEN);
  BarScanOn();
  ttycurstr(READY_MSG, 0);      
  do   {
    iregs.h.ah = 0x84;
    iregs.x.cx = SCAN_BUFFER_LEN; 
//  Use temporary far pointer to get segment:offset of data buffer 
    fptr = (unsigned char far*)_sScanBuf;
    iregs.x.dx = FP_OFF(fptr);
    segregs.ds = FP_SEG(fptr);
    int86x(INTERFACE_VEC, &iregs, &oregs, &segregs);
  
    switch(oregs.h.al) {
      case 0:
        decode_type  = oregs.h.bh;
        direction    = oregs.h.bl;
        label_length = oregs.x.cx;
        _sScanBuf[label_length] = NULL;               
        
        if ( (label_length != NEW_BARCODE_LEN) && (label_length != OLD_BARCODE_LEN) )
        {
          BarScanOff();
          memset(_sScanBuf, '\0', SCAN_BUFFER_LEN);
          Error("Lunghezza");
          return FALSE;            
        }   
             
// Se e' di tipo nuovo (OPPURE comincia per 0) salto lo 0 iniziale e il CD finale
        if (label_length == NEW_BARCODE_LEN || _sScanBuf[0] == '0')
          CopiaStr(_sScanBufStripped, _sScanBuf, 1, NEW_BARCODE_LEN-1);    
        else 
          if (label_length == OLD_BARCODE_LEN)
            CopiaStr(_sScanBufStripped, _sScanBuf, 0, OLD_BARCODE_LEN-1);    
        
        CopiaStr(_sCodPallet,_sScanBufStripped, START_CODPALLET, START_CODPALLET+CODPALLET_LEN);            
        printf("Codice letto: \n%s\n", _sScanBufStripped);
        BarScanOff();
        return TRUE;
        break;
      case 1:            
        break;
      case 2:
        Error("Errore: scansione");
        BarScanOff();        
        return FALSE;
        break;
      case 3:
        Error("Errore: finito il buffer di lettura");
        BarScanOff();        
        return FALSE;        
        break;
      default:
        Error("Errore senza descrizione nota al programmatore");
        BarScanOff();
        return FALSE;        
        break;
    }
  } while ( kbhit() == FALSE); 
  i=getch();

  BarScanOff();        
// Ritorna TRUE se letto qualcosa       
  return _sScanBuf[0] != NULL;
}

int ChiudiOrdine(const int iOrdineCorrente)
{                          
  int   rc=XP_OK, stat;
  char  ch;
  BOOL  Eof = FALSE;
  char  CodPro    [CODPRO_LEN+1];              
  char  sCartoni  [CARTONI_LEN+1];
  char  cOrdine;
  int   i;
      
  Cls();

  //printf("Ordine attuale: %d\n", iOrdineCorrente);
  printf("CHIUDO ORDINE (s/N) ? ");
  GetVal("", "%c", (int*)&ch);  
  if (toupper(ch) == 'S')    
  {          
    _iCtrLetti = 0;
    tfseek(_fname, FD_FIRST);
    i = 1;
    while (!Eof)
    {
      stat = tfread(_fname,  _sRiga);
      _sRiga[REC_CARICO_LEN] = '\0';
      Eof = stat == END_FOUND;      
      if (Eof) break;
      ParseRiga(_sRiga, _sCodPallet, CodPro, &cOrdine, sCartoni);      
//      CopiaStr(barcode, sBarcode, 0, OLD_BARCODE_LEN);      
      printf("%d ", i);
      i++;             
      
      if (!RadioTx(_OP_RIGA_ORDINE, _sCodPallet, "", 0, &rc))
        return -1;
    }                
// Una X all'inizio segna la fine dell'ordine.......d'altra parte non ne posso piu' che vulite dammia ah                              
    _sCodPallet[0] = 'X';

    if (!RadioTx(_OP_RIGA_ORDINE, _sCodPallet, "", 0, &rc))
    {                                                            
      printf("Pallet n. %d codice : %s\n", i, _sCodPallet);          
      return -1;                                                 
    }
    else  
      return XP_OK; 
  }
  else
    return -1; 
}


//
// ScriviPallet
//
// Usa GLOBALI: _sCodPallet, e _sCodpro. Devono essere gia' compilate!!
//
void ScriviPallet(const int iCartoni)
{                     
  int rc;      
  static char  sCt[CARTONI_LEN+1];

//  CopiaStr(sRec,_sScanBufStripped,START_CODPALLET,START_CODPALLET+CODPALLET_LEN);
                             
   memset(_sRiga, '\0', REC_CARICO_LEN);
   strcpy(_sRiga, _sCodPallet);  
   strcat(_sRiga, _sCodPro); 
// Aggiunge i cartoni..
   sprintf(sCt,"%03d",iCartoni);
   Overwrite(_sRiga, sCt, CODPALLET_LEN+CODPRO_LEN, CODPALLET_LEN+CODPRO_LEN+4);
  
  _sRiga[REC_CARICO_LEN-2] = _iOrdineCorrente + '0';     
  _sRiga[REC_CARICO_LEN-1]    = '\0';       
  
  if (_DEBUG)
  {
    printf("ScriviPallet\n");
    printf("_sRiga=|%s|\n", _sRiga);
    getch();
  }

  // Finalmente scrive l'etichetta..
  rc = tfwrite(_fname, _sRiga, 0);
  if (rc != FUNC_SUCC)
    Error("Fallita scrittura etichetta");
  else                    
  { 
// Forza scrittura su disco  
    rc = tfclose(_fname);  
// Riapre il file    
    rc = tfopen(FILE_CARICO_NAME, _fname, _vrd, FD_FIXED, REC_CARICO_LEN);            
    if (_DEBUG) {
      printf("Scritto pallet:\n %s\n", _sCodPallet);
      Tasto();     
    }
  }
}

//
// PalletLetto
//                            
// GLOBALS
//
// usa _sCodPallet (compilato in LeggiPallet)
//
BOOL PalletLetto()
{            
  int rc=0;

  rc = tfsearch(_fname, FD_FIRST, 0, _sCodPallet, 0, CODPALLET_LEN);                                  
  
  if (_DEBUG) {
      printf("PalletLetto:\n");
      printf("Cerco:|%s|\n",_sCodPallet);
      printf("rc=%d\n",rc);                               
      getch();
  }
  
  if (rc == FUNC_SUCC) 
    Error("Bancale gia' letto!");

  return rc == FUNC_SUCC;                                  
}

void Lettura(const char cTipoOp)
{           
  BOOL done=FALSE, ok=FALSE;
  int iCt=0, rc;

  BarScanDev(LASER_DEVICE);
  SetupBarcodes(bcI2su5);          
  
  switch(cTipoOp)
  {
    case _OP_RIGA:
      Cls();
      while (!done)
      {        
        ok = LeggiPallet();   
        if (!ok) break;
        //if (PalletOK() && !PalletLetto()) 
        if (!PalletLetto() && PalletOk())
        {
          ElaboraDatiPallet();        // Elabora _sRxBuf          
          printf("Lettura n.: %d \n", ++_iCtrLetti);            
        }
      }
      break;
      
    case _OP_CARICA:
      Cls();
      while (!done)
      {        
        ok = LeggiPallet();   
        if (!ok) break;                                                                             
        if (!RadioTx(_OP_CARICA, _sScanBufStripped, "", 0, &rc))              
          continue;
      }    
      break;

    case _OP_CAMBIA:
      Cls();
      while (!done)
      {
        ok = LeggiPallet();   
        if (!ok) break;
        memset(_sCodArea,'\0',CODAREA_LEN);
// Forza la lettura solo del Code139        
        SetupBarcodes(bcCode39);  
        InputArea(_sCodArea);   
// Ripristina la lettura di I2 su 5
        SetupBarcodes(bcI2su5);          
        printf("\n\n");                                                                                    
        if (!RadioTx(_OP_CAMBIA, _sCodPallet, _sCodArea, 0, &rc))
          continue;
        if (rc==_ERR_NON_PRESENTE) 
          Error("Bancale assente ");           
      }
      break;
            
    default:
      break;
  }
}

BOOL CaricoVuoto()
{
// Determina num.di rec. letti
  tftell((char*)_fname, &size, &rec_num);
  if (size==0) 
    return TRUE;
  else
    return FALSE;
}

void MenuCarichi()
{
  char cmd;                
  BOOL done = FALSE;

  while (!done)
  {         
    Cls();
    printf(VERSION);                          
  //  printf("Scanner n. %02d\n", _iScanNum);  
    if (_iOrdineCorrente >= 1)
      printf("*-Aperto ordine n. %d\n\n", _iOrdineCorrente);        
    printf("1-Lettura pallet\n");
    printf("2-Inserimento manuale");    
    printf("3-Lista carico\n");
    printf("4-Elimina pallet\n");
    printf("5-Chiudi ordine\n");
    printf("6-Annulla ordine\n"); 
    printf("7-Spedisci via cavo\n\n");     
    printf("0-Menu' Comando? ");
  
    GetVal("", "%c", (int*)&cmd);
    if (isalpha(cmd)) cmd=toupper(cmd);
  
    switch(cmd)
    {
      case '0':
        done = TRUE;
        break;  
        
  // Lettura pallet
      case '1':
        Lettura(_OP_RIGA);
        break;

// Immissione manuale di un barcode        
      case '2':
        ManualEnter(_iOrdineCorrente);
        break;

// Lista carico
      case '3':
        if (_iOrdineCorrente < 0) {
          Error("Nessun ordine aperto!");
        }
        else
          ListaFile(_iOrdineCorrente);
        break;

// Elimina 
      case '4':
        if (_iOrdineCorrente < 0) {
          Error("Nessun ordine aperto!");
        }
        else
          Elimina();
        break;                                             

// Chiudi ordine
      case '5':
        if (CaricoVuoto()) {
          Error("Nessun ordine aperto!");
        }
        else {
          if (ChiudiOrdine(_iOrdineCorrente) == XP_OK)
            CancellaCarico();          
          else 
            Error("Chiusura fallita");            
          }
        break;
                
// Annulla ordine
      case '6': 
        if (CaricoVuoto()) {
          Error("Nessun ordine aperto!");
        }
        else 
        {
          Cls();
          if (MsgSiNo("Annullo ordine"))
          { 
            _iCtrLetti=0;
            CheckFile(TRUE); // Se c'era un carico in memoria lo cancella          
          }
        }
        break;

      case '7':
        TrasmettiBatch();
        break;
        
/**************************************************        
// Ordine successivo
      case 6:
        if (_iOrdineCorrente < 9)
          _iOrdineCorrente++;
        break;

// Ordine precedente
      case 8:
        if (_iOrdineCorrente > 1)
          _iOrdineCorrente--;
        break;        
*******************************************************/

      default:
        continue;
    }
  }
}

int CausaliCartoni(const BOOL bScarica)
{
  int cmd;                
  BOOL done = FALSE;

  while (!done)
  {         
    Cls();
    printf(VERSION);                          
    if (bScarica)
    {
      printf("Uscita cartoni per \n\n");
      printf("1-Danni magazzino\n");
      printf("2-Danni meccanici\n");    
      printf("3-Vendita dipendenti\n");        
      printf("4-Omaggi\n");    
      printf("5-Laboratorio\n\n");        
      printf("0-Menu' Comando? ");
    
      GetVal("", "%d", &cmd);
      
      done = (cmd >= 0 && cmd <= 5);
    } 
    else
    {
      printf("Aggiunta cartoni per \n");
      printf("1-Ritorno da lab.\n\n");    
      printf("0-Menu' Comando? ");
    
      GetVal("", "%d", &cmd);
      cmd += 10; // Per non confonderlo con i precedenti
      done = (cmd >= 10 && cmd <= 11);
    } 
    
  }
  return cmd;
}

#define CT_CORREGGI '1'
#define CT_TOGLI    '2'
#define CT_AGGIU    '3'
        
void MenuCartoni()
{
  char cmd;                
  BOOL done = FALSE, ok=FALSE;
  int iCausale=0,iCtOld=0,iCtNew=0;
  unsigned char sCartoni[3], cOp;         
  int rc;             
  char sBuf[(CODPALLET_LEN*2)+1];

  BarScanDev(LASER_DEVICE);
  SetupBarcodes(bcI2su5);          
  
  while (!done)
  {         
    Cls();
    printf(VERSION);                          
    printf("*-Gestione cartoni\n\n");        
    printf("1-Correggi\n");      
    printf("2-Togli\n");    
    printf("3-Aggiungi\n");
    printf("4-Travaso\n");    
    printf("5-Stampa foglio\n\n");    

    printf("0-Menu' Comando? ");
  
    GetVal("", "%c", (int*)&cmd);
    if (isalpha(cmd)) cmd=toupper(cmd);
    
    if (cmd=='0') 
    {
        done = TRUE;
        break;  
    }                        
    
    if (cmd=='5')
    {
// manda comando _CHIUDI
      RadioTx(_OP_CARTONI_CHIUDI, "", "", 0, &rc);              
    }
    else
    {
      switch (cmd)                                                 
      {
        case CT_CORREGGI:
          printf("Leggi il pallet\nda CORREGGERE\n");        
          break;
      
        case CT_TOGLI:
          printf("Leggi il pallet\nda cui PRELEVI\n");        
          break;                                

        case '3':
          printf("Leggi il pallet\na cui AGGIUNGI\n");        
          break;
          
        case '4':
          printf("Leggi il pallet\nda cui TOGLI\n");        
          break;
      }
     
      ok=LeggiPallet();
      if (!ok) break;
          
      if (!PalletOk()) break;
      CopiaStr(sCartoni, _sRxBuf,1+CODPRO_LEN, 1+CODPRO_LEN+3);      
      printf("Pallet  : %s\n",_sCodPallet);
      printf("Cartoni : %s\n", sCartoni);
      Tasto();
      iCtOld = atoi(sCartoni);

// Se ho scelto Travaso o Correggi non faccio vedere sottomenu'                                       
      if (cmd==CT_CORREGGI)                             
        iCausale = 7;
      else
        if (cmd=='4')                             
          iCausale = 6;
        else      
          iCausale = CausaliCartoni(cmd=='2' ? TRUE : FALSE);
      if (iCausale==0) break;
      

      switch (cmd)                                                 
      {
        case CT_CORREGGI:
          printf("Quanti cartoni ha: \n");        
          break;
      
        case CT_TOGLI:
          printf("Quanti cartoni vuoi\n ");
          printf("togliere (max %s):\n", sCartoni);
          break;                                

        case '3':                          
          printf("Quanti cartoni vuoi\n ");        
          printf("aggiungere:\n");    
          break;
          
        case '4':                          
          printf("Quanti cartoni vuoi\n ");        
          printf("travasare (max %s):\n", sCartoni);
          break;
      }
     

      GetVal("", "%d", (int*)&iCtNew);
      if (iCtNew==0) 
        break;
      
      if (cmd==CT_TOGLI && iCtNew > iCtOld) 
      {
        Error("Troppi!");
        break;
      }
      
      strcpy(sBuf,_sCodPallet);
      
      switch (iCausale)
      {              
        case 1:           
          cOp = _OP_CARTONI_S1;
          break;
        case 2:
          cOp = _OP_CARTONI_S2;
          break;
        case 3:
          cOp = _OP_CARTONI_S3;
          break;
        case 4:
          cOp = _OP_CARTONI_S4;
          break;
        case 5:
          cOp = _OP_CARTONI_S5;
          break;
        case 6: // Travaso          
          cOp = _OP_CARTONI_T;
          if (_DEBUG) { 
            printf("Travaso PRIMA DI leggere dst:\nsBuf=|%s|\n_sCodPallet=|%s|\n",sBuf,_sCodPallet);
            Tasto();
          }
// in questo caso Buf contiene i due codici pallet uno dopo l'altro
          strcpy(sBuf, _sCodPallet);
          printf("Leggi pallet a cui AGGIUNGI\n");
          ok=LeggiPallet();
          if (!ok) return;
          strcat(sBuf, _sCodPallet);          
          if (_DEBUG) { 
            printf("Travaso:\nsBuf=|%s|\n_sCodPallet=|%s|\n",sBuf,_sCodPallet);
            Tasto();
          }
          break;
        case 7:           
          cOp = _OP_CARTONI_C1;
          break;
        case 11:
          cOp = _OP_CARTONI_C2;
          break;
        default:
          Error("Op sconosciuta");
          return;
      }                      
      RadioTx(cOp,sBuf,"",iCtNew,&rc);
    }
  }
}

// Front end per TrasmettiRadio()
BOOL RadioTx(const char cOp, const char* sBuf, const char* sCodArea, const int iCt, int* rc)
{                               
  *rc = TrasmettiRadio(cOp, sBuf, sCodArea, _iOrdineCorrente, iCt);              
  if (*rc > 0) 
  {
    printf("Errore RF: %x\n", (int)*rc);        
    return FALSE;
  } 
  else
    if (*rc==_ERR_SERVER) {
      Error("Fallita: ripetere");
      return FALSE;
    }           
  return TRUE;
}

int ReadIni(int* iScanNum, int* iRfTimeOut, int* iNetMode)
{    
  FILE* fConf;
  
  fConf = fopen(_INI_FILE, "r");
  if (fConf==NULL)
  {
    Error("Ini FILE!");
    return -1;
  }             
  fscanf(fConf,"%d", iScanNum);
  fscanf(fConf,"%d", iRfTimeOut);   
  fscanf(fConf,"%d", iNetMode);    
  fclose(fConf);
  if ( (*iNetMode != 0) && (*iNetMode != 1) && (*iNetMode != 2) )
    return -1;

  return 0;
}

void SetupSpeed()
{                                        
  Cls();
  GetVal("Bitrate (1,2,3,4): ", "%ld", (int*)&_lBitrate); 
  GetVal("Freq    (3)      : ", "%d",  (int*)&_iFrequency);
  GetVal("ChanSet (10)     : ", "%ld", (int*)&_lChannel_set);  
  srfdisconnect();
  SrfConnect(_iScanNum);
}

BOOL RiceviStr(const comm_port, const char* str, BOOL bMsg)
{
  int i, len, rc;                    
  char cRec;
  
  len = strlen(str);
  
  for (i=0; i<len; i++) {
    rc = recvchar(comm_port, &cRec);
    if (rc != RECV_OK) {
      if (bMsg)
        writestr(0, "Errore Rx", 0, NORMAL_VID);
      return FALSE;
    }
    else
      if (cRec != str[i])
        return FALSE;
  }
  return TRUE;
}

BOOL ScriviStr(const comm_port, char* str, const int len, BOOL bMsg)
{
  int status;
  status=sendstr(comm_port, str, len);
  if (bMsg) {
    if (status == TELX_ERR)
      writestr(0, "INVALID DATA", 0, NORMAL_VID);
    else 
      if (status == LOST_DSR)                    
        writestr(0, "DSR ERR", 0, NORMAL_VID);
      else
        if (status == SEND_OK)  
          printf("OK\n");
  }        
  return status==SEND_OK;
}

BOOL Tx()
{                            
  const int MAX_TRIES = 3;
  int comm_port, comm_mode, type, baud, on_off, i;
  unsigned int status;
  BOOL Eof=FALSE;                              
  BOOL bOkTx, bOkRx;
  char buffer[REC_CARICO_LEN+1];     
  static char *comm_init_err=  "Comm initialize error";
  static char *cts_message=    "CTS True             ";
  static char *dsr_message=    "DSR True             "; 
  static char *dtr_error=      "Dtr ctrl   error     ";
  static char *rts_error=      "Rts ctrl   error     ";
  static char *inv_data=       "Invalid input data";
  static char *data_ok=        "OK";
  static char *dsr_err=        "DSR err";
  static char *_BOT   =        "BOT";
  static char *_EOT   =        "EOT";

  cleardisp(0);  
  comm_port = COM1;
  comm_mode = DATA_7 | STOP_1 | ODD_PARITY;
  baud = BAUD_9600;
  type = POLL;
  if (comminit(comm_port, comm_mode, baud, type) == TELX_ERR){
      writestr(0, comm_init_err, 0, NORMAL_VID);
  }
    
  on_off = TELX_ON ;  /* Raise DTR */
  if ( (status = dtrctrl (comm_port, on_off)) != 0 ){
      writestr(0, dtr_error, 0, NORMAL_VID);
  }
            /* and  RTS */
  if ( status = rtsctrl (comm_port, on_off) != 0){
      writestr (0, rts_error, 0, NORMAL_VID); 
  }
      /* Loop DTR to DSR, RTS to CTS with loop-back test */
  status = commstatus ( comm_port); /* Get status for port */

  if ((status & DSR) == DSR){      /* Now check for DSR */
      writestr(0, dsr_message, 0, NORMAL_VID);
  }

  if ((status & CTS) == CTS){       /* and check for CTS */
      writestr(0, cts_message, 0, NORMAL_VID);
  }
  on_off = TELX_OFF ; /* Drop DTR */
  if ( status = dtrctrl (comm_port, on_off) != 0 ){
      writestr(0, dtr_error, 0, NORMAL_VID);
  }
            /* and  RTS */
  if ( status = rtsctrl (comm_port, on_off) != 0){
      writestr (0, rts_error, 0, NORMAL_VID); 
  }


// Handshake iniziale spedisce BOT e aspetta BOT
  for (i=0; i<MAX_TRIES; i++) {
    bOkTx = ScriviStr(comm_port, _BOT, 3, FALSE);
    if (!bOkTx) Tasto();
    bOkRx = RiceviStr(comm_port, _BOT, FALSE);
    if (!bOkRx) Tasto();
    if (bOkTx && bOkRx) break;
  }

// Se fallito il dialogo esce
  if (!bOkTx || !bOkRx) {
    commpwroff();
    return FALSE;       
  }
          
  tfseek(_fname, FD_FIRST);
  while (!Eof)
  {
    status = tfread(_fname,  buffer);
    printf("%s ",buffer);
    buffer [REC_CARICO_LEN] = '\0';
    Eof = status == END_FOUND;      
    if (Eof) break;
//    bOkTx=ScriviStr(comm_port,buffer, REC_CARICO_LEN, TRUE); 
// 31.7.97 trasmette anche il null finale provo a toglierlo mettendo REC_CAR-1
    bOkTx=ScriviStr(comm_port,buffer, REC_CARICO_LEN-1, TRUE);    
    if (!bOkTx) Tasto();
  }
   
// Handshake finale spedisce EOT e aspetta EOT
  for (i=0; i<MAX_TRIES; i++) {
    bOkTx = ScriviStr(comm_port, _EOT, 3, FALSE);
    bOkRx = RiceviStr(comm_port, _EOT, FALSE);
    if (bOkTx && bOkRx) break;
  }

// Se fallisce l'handshake finale importa meno...

  commpwroff() ;        /* Turn comm power off */
  
  return TRUE;
}

void TrasmettiBatch()
{                        
  int rc;
// Forza la scrittura su disco del carico
  rc = tfclose(_fname);        
  rc = tfopen(FILE_CARICO_NAME, _fname, _vrd, FD_FIXED, REC_CARICO_LEN);        
// Simula il beep del pcomm() per non sconvolgere il carrellista
  Beep(); 
  printf("Un tasto per cont. ");
  getch();
  Tx();
// Riapro il file subito dopo        
  rc = tfopen(FILE_CARICO_NAME, _fname, _vrd, FD_FIXED, REC_CARICO_LEN);
  if (_DEBUG) {
    if (rc != FUNC_SUCC && rc != CREATE_OPEN)
    {
      printf("main: rc=%d\n", rc);
      getch();
    }
  }  
}

main(int argc, char** argv)
{
  int   rc;
  char  cmd;
  BOOL  done = FALSE;
 

  _lBitrate = 1;
  _iFrequency = 3;
  _lChannel_set = 10;
   
  if ( (argc > 1) && (toupper(*argv[1]) == 'D') )
    _DEBUG=1;
  else
    _DEBUG=0;
    
  if ( (argc > 1) && (toupper(*argv[1]) == 'S') )
    _SETUP=TRUE;
  else
    _SETUP=FALSE;
    
// Sarebbe bello poter fare lo stesso test anche per WANDTSR....  
  if(srftestvec() == FALSE) {
    Cls();
    printf("TSR is not\n");
    printf("installed!\n");
    return 0;
  }

/*  Save the current application timeout value. */
  usr_tmout = get_tmout();

  if (ReadIni(&_iScanNum, &_iRfTimeOut, &_iNetMode) < 0) 
  {
    Error("CONFIG mancante o errato");
    return -1;
  }

// Non esce se fallisce la connessione iniziale (c'e' setup per cambiare i parametri)
  if (!_SETUP)
    if (SrfConnect(_iScanNum) != XP_OK)
      Error("\nErrore connessione");
             
    
// Apro il file sempre
  rc = tfopen(FILE_CARICO_NAME, _fname, _vrd, FD_FIXED, REC_CARICO_LEN);

  if (_DEBUG) {
    if (rc != FUNC_SUCC && rc != CREATE_OPEN) {
      printf("main: rc=%d\n", rc);
      getch();
    }
  }  
  
  while(!done)
  { 
    Cls();        
    printf(VERSION);
    printf("Scanner n. %02d\n\n", _iScanNum);
    printf("1-Prendi in carico\n");
    printf("2-Spedizioni\n");
    printf("3-Cartoni\n");    
    if (_SETUP) {
      printf("4-Cambia area\n");
      printf("5-Setup\n\n");    
    } else                      
      printf("4-Cambia area\n\n");    
    printf("0-Fine  Comando? ");

    GetVal("", "%c", (int*)&cmd);
    if (isalpha(cmd)) cmd = toupper(cmd); 

    switch(cmd)
    {
      case '0':     
        if (MsgSiNo("Confermi l'uscita   "))
          done = TRUE;
        break;         
        
// Prendi in carico
      case '1': 
        Lettura(_OP_CARICA);
        break;

// Menu' carichi
      case '2':        
        MenuCarichi();
        break;

      case '3':
        MenuCartoni();
        break;
                                                    
// Cambia area
      case '4':
        Lettura(_OP_CAMBIA);
        break;

      case '5':
        if (_SETUP) SetupSpeed();
        break;
        
      default:  
        continue;
    }
    Cls();
  }

  printf("Per rientrare nel programma digita: \nR <INVIO>\n");
  rc = tfclose(_fname);
  if (rc != FUNC_SUCC)
    printf("main: fallita tfclose rc = %d\n", rc);
  
  srfdisconnect();
  
  return 0;
}

/*****************************************************************************
*                                                                            *
* DATE: 04-05-95 11:52am              WRITTEN BY: Systems Development        *
*                                                                            *
* PROCEDURE NAME:     PacketConfigure                                        *
*                                                                            *
* DESCRIPTION:        Configures the Packet Driver                           *
*                                                                            *
* CALLING SEQUENCE:   PacketConfigure();                                     *
*                                                                            *
* INPUT:                                                                     *
*    None.                                                                   *
*                                                                            *
* OUTPUT:                                                                    *
*    None.                                                                   *
*                                                                            *
* VARIABLES:                                                                 *
*    None.                                                                   *
*                                                                            *
* SUBROUTINES:                                                               *
*    None.                                                                   *
*                                                                            *
* NOTES:                                                                     *
*    This routine is only a shell to show how the packet driver can be       *
*    configured.                                                             *
*                                                                            *
*****************************************************************************/
int PacketConfigure()
{
  SSPktCfg far *PktDrvrCfg;
     
  // First we need to find the Packet Driver
  if(!PktFindDriver()) 
  {
   printf("PD non trovato\n");
   return FAILURE;
  }
     
// Get a pointer to the current configuration
  if (!(PktDrvrCfg = PktGetConfiguration())) 
  {
    printf("PD GetConf\n");
    return (FAILURE);
  }
  
  PktDrvrCfg->RadioNodeID = 1; // Add your UnitID here
  // For 900MHZ 
  //         PktDrvrCfg->ChannelNum  = // Add your Channel Number Here  
    
  // For 2.4GHZ
  PktDrvrCfg->SpreadCode  = 4; // Add your BitRate Index  Here  
  PktDrvrCfg->ChannelNum  = 3; // Add your Frequency Index Here  
             
  PktDrvrCfg->SystemID    = 1; // Add your System ID Here 
    
// Add any other config values here
           
// Once all of the config values have been filled in, 
// call PktRestart() to have them take effect
  if(!PktRestart())
  {
   printf("PD restart\n");
   return(FAILURE);
  }

  return(SUCCESS);   
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   connect                                            *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure connects for SRF communications.                    *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void connect()                                                     *
*                                                                       *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/
unsigned char SrfConnect(const int iScanNum)
{
  unsigned char status;
  unsigned long TempSpreadingCode;
  unsigned long TempLong;

  srfinit(&cfg);     
// Aggiunte per TXP
// Usa ARP  
  ArpFlag = 1;
/*  if (PacketConfigure() == FAILURE) {
    Error("Fallita packet driver");
    return -1;
  } */
// Fine aggiunte TXP
  cfg.xp_unit_id = iScanNum;
  cfg.xp_dest_id = 0x4001;
  cfg.rf_sys_id = 1;
  cfg.rf_net_mode = _iNetMode;
 
  // Check for and set-up channel sets
//  if(cfg.channel_mode == CHANNEL_SET_MODE)  {
    cfg.rf_spread_pgm_mode = 0;  // Pgm mode is Raw
   
    // Convert BitRate, Frequency, Channel Set into rf_spread_id
    TempSpreadingCode  = 0;
    TempLong           = 0;
    TempLong           = (unsigned long) _lBitrate;
    TempSpreadingCode += TempLong << 16;
    TempLong           = (unsigned long) _lChannel_set;  // Can use 0
    TempSpreadingCode += TempLong << 8;
    TempSpreadingCode += _iFrequency;
    cfg.rf_spread_id = TempSpreadingCode;
  //}

  printf("Connessione...");    
  status = srfconnect(&cfg);
// TBI gestire l'errore qui NO lo gestisco sopra
  if (status == XP_OK)
    printf(" OK\n");
  return status;
}

// Costruisce e trasmette un pacchetto. Mette la risposta in _sRxBuf
int TrasmettiRadio(const char cTipoOp,const char* sSendBuf,const char* sCodArea,
                   const int iOrdineCorrente, const int iCartoni)
{
  unsigned int  iRxLen;         // Length of recv 
  int iTxLen;
  unsigned char cStatus;        // Status of send/recv
  const int iSendBufLen = strlen(sSendBuf);  

// 1. Pulisce i vari record
  memset(_sTxBuf, '\0', REC_RF_LEN);
  memset(_sRxBuf, '\0', REC_RF_LEN);
  memset(_sTmp,   '\0', REC_RF_LEN);

// 2. Aggiunge TipoOp
  _sTxBuf[0] = cTipoOp;

// 4. Aggiunge il barcode al record di tx
  strcat(_sTxBuf,sSendBuf);

// 5. Aggiungo al buffer il numero d'ordine
  _sTxBuf[iSendBufLen+1]   = toascii(iOrdineCorrente + '0');                                                   

// 6. Aggiunge i cartoni
  //_sTxBuf[iSendBufLen+2] = toascii(iCartoni + ' '); // Per renderlo un car.stampabile 
  _sTxBuf[iSendBufLen+2] = iCartoni + ' '; // Per renderlo un car.stampabile

// 7. Se necessario aggiunge il codice area
  if (cTipoOp == _OP_CAMBIA) {     
// pad a 4 del codice area  
    Fill(_sTmp,   ' ', CODAREA_LEN);
    _sTmp[CODAREA_LEN] = '\0';
    CopiaStr(_sTmp, sCodArea, 0, -1);
    strcat(_sTxBuf,_sTmp);    
  }
  else
    strcat(_sTxBuf, "0000");
  
  _sTxBuf[iSendBufLen+2+5] = '@'; // Terminatore per ConsoleRf
  
// Il record di Tx e' gia' terminato perche' riempito di null prima
  iTxLen = strlen(_sTxBuf);

// Set up size of recv buffer 
  iRxLen = sizeof(_sRxBuf);

  if (_DEBUG) {
    printf("TrasmettiRadio spedisce:\n");
    printf("sTmp = |%s|\n",_sTmp);
    printf("sTxBuf %d |%s|\n",iTxLen, _sTxBuf);
    Tasto();
  } 

  cStatus = srftransact(_sTxBuf, iTxLen, _sRxBuf, &iRxLen, _iRfTimeOut); 
  
  if (cStatus != XP_OK) 
    Error("Tx/Rx fallita!");
  else
  {
    _sRxBuf[iRxLen] = NULL;         
    printf("Rx OK\n");
    switch(_sRxBuf[0]) {
  
      case _RET_BLOCCATO:   
        return _ERR_BLOCCATO;
          
      case _RET_GIA_SCARICATO:
        return _ERR_GIA_SCARICATO;
          
      case _RET_NON_PRESENTE:     
        return _ERR_NON_PRESENTE;
  
      case _RET_SEPARATO:     
        return _ERR_SEPARATO;
        
      case _MSG_ERR:
        return _ERR_SERVER;
                  
      case 'O': // risposta OK
        return cStatus;
          
      default:
        return cStatus;
    }
  }

  return cStatus;
}



