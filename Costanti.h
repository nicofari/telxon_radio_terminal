// Constanti.h 9/4/97
// Costanti legate all'applicazione

#ifndef _COSTANTI_H
#define _COSTANTI_H

typedef enum {FALSE, TRUE} BOOL;

#define MAX_RIGHE 15

// Codice prodotto di 4 + check digit
//#define BARCODE_LEN 14    
// Nel file finiscono sempre a 28 (se hanno zero e CD vengono strippati)
#define OLD_BARCODE_LEN 28
#define NEW_BARCODE_LEN 30
#define BARCODE_LEN     28

// Il record del carico e' BARCODE_LEN + NUMERO_ORDINE
//#define REC_CARICO_LEN  15
// 28 del barcode + n.ordine(A1) + Cartoni(A2) 
//#define REC_CARICO_LEN  31

// 22/7/97 i cartoni di TRE!!!
//#define REC_CARICO_LEN  32      8        6         3         1
#define REC_CARICO_LEN 19 // Codpallet + Codpro + Cartoni + Ordine

// Codice prodotto di 4
//#define CODPRO_LEN   4
#define CODPRO_LEN   6

// Lunghezza Codice pallet
//#define CODPALLET_LEN 9
#define CODPALLET_LEN 8
                       
// Offset nel barcode del codpro e codpallet                       
#define START_CODPRO      4
#define START_CODPALLET   20
// I cartoni stanno dopo barcode + ordine
#define START_CARTONI     (OLD_BARCODE_LEN+1)
#define CARTONI_LEN       3

// Nome del file dati
//#define FILE_CARICO_NAME "PALLET.BAR" 
#define FILE_CARICO_NAME "A:PALLET.BAR"

// File pallet bloccati
//#define FILE_BLOCCATI_NAME  "BLOCCATI.BAR" 
#define FILE_BLOCCATI_NAME  "A:BLOCCATI.BAR"

// Il record dei bloccati e' solo il codice pallet
#define REC_BLOCCATI_LEN  CODPALLET_LEN

// Lunghezza del buffer della routine di scansione
#define SCAN_BUFFER_LEN 50

///////////////////////////////////////
// COSTANTI RF
///////////////////////////////////////
//
// Record per tx RF: 
//
// 1(TipoOp) + 28(barcode) +1(ordine) + 1(cartoni) + 4(area) + 1(null)
//
#define REC_RF_LEN  35  // Tolto scannum e cartoni solo un byte

// Costanti per operazioni RADIO
#define _OP_CARICA  'A'   // Prendi in carico                         
#define _OP_CAMBIA  'B'   // Cambia area
#define _OP_RIGA    'C'   // Riga di carico
#define _OP_RIGA_ORDINE  'D'   // Chiudi carico

// Scarico cartoni
#define _OP_CARTONI_S1 'E'
#define _OP_CARTONI_S2 'F'
#define _OP_CARTONI_S3 'G'
#define _OP_CARTONI_S4 'H'
#define _OP_CARTONI_S5 'I'
// Carico cartoni
#define _OP_CARTONI_C1 'J'
#define _OP_CARTONI_C2 'K'
#define _OP_CARTONI_T  'L'  // Travaso
#define _OP_CARTONI_CHIUDI 'M' // Chiudi e stampa foglio

// Vedi costanti.pas         
#define _RET_OK            '0'
#define _RET_NON_PRESENTE  '1'
#define _RET_BLOCCATO      '2'
#define _RET_GIA_SCARICATO '3'

#define _ERR_NON_PRESENTE   -1
#define _ERR_BLOCCATO       -2
#define _ERR_GIA_SCARICATO  -3

#define _MSG_ERR           'X'
#define _ERR_SERVER        -4

#define CODAREA_LEN   4                   

#define _START_ORDINE 29 

typedef enum enTipiBc { bcI2su5, bcCode39, bcCode128 } TipiBarcode;

#endif

