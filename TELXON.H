/************************************************************************
 *                                                                      
 * DATE: 06/19/90              WRITTEN BY: Telxon/O.S.             
 *                                                                      
 * FILE NAME:  telxon.h                                            
 *                                                                      
 * DESCRIPTION:  This is the header file for the Telxon 'C' library 
 *               functions.                                
 * REVISIONS:                                                                     *
 * 1.2  11/10/90        LB                                                  
 *      Changed function definition delay() to sysdelay()   
 *
 * 1.2  12/18/90        DLW                                                 
 *      Added constants RF_ROM and ARC_ROM and functions    
 *      flasherase(), flashid(), flashpower(), and flashprogram().                                      
 *
 * 1.2  01/11/91        DLW                                                 
 *      Added constants L_TEXTMODE, L_TEXT21, and           
 *      L_GRAFXMODE to go with functions getvidmode()       
 *      and setvidmode()                                    
 *
 * 1.2  02/13/91        DLW                                                 
 *      Added functions keyrepeat(), setrepeatdur(), and getrepeatdur().                                      
 *
 * 1.2  06/12/91        TLM                                                 
 *      Added function declaration for lowbat()             
 *
 * 1.2  07/16/91        DLW                                                 
 *      Added constant SPEAKER.                             
 *
 * 1.2  08/02/91        DLW                                                 
 *      Added constants PTC_860_8, PTC_860_16, PTC_960_4    
 *      PTC_960_8, PTC_960_16, PTC_710G, and PTC_960G       
 *
 * 1.2a 09/26/91        GRS                                                 
 *      Added the default external communication option,    
 *      RS232, for completeness and to help make application
 *      programming less confusing.                         
 *                                                                                                                        *
 *      Added constants, structures, and function declarations for 
 *      file management functions.               
 *
 * 1.3  01/24/92        DLW                                                 
 *      Added constant PTC_860IM.                           
 *                                                                      
 * 1.3  02/20/92        SFP                                                 
 *      Added constants, mcard_dat structure and function   
 *      function declarations for the magcard functions.    
 *                                                                      
 * 1.3  03/02/92        DLW                                                 
 *      Added kernelver, setlowbatlen, getlowbatlen,        
 *      getkeybrd, and restorekeybrd function declarations, 
 *      and PWRUP_TRIG constant.                            
 *                                                                      
 * 1.3  03/20/92        GRS                                                  
 *	 The setdate and settime functions are defined   
 *  by Borland C++ as void setdate() and                                  
 *	 void settime(). The definitions in this file    
 *  will cause type conflict errors if dos.h is                           
 *	 included in an application that is compiled     
 *  under Borland C++. The easiet and safest work-                        
 *	 around is to remove the reference to these      
 *  functions from this file and to place the                             
 *	 Borland runtime library first in the library    
 *  list when the application is built. This will                         
 *	 force the Borland functions to be called when   
 *  these functions are used.
 *
 * 1.4  06/10/92        DLW                                                   
 *	 Added constants/definitions for the following: 
 *      - POS_5000 constant                                                   
 *		  - display character set selection                    
 *      - status LED control                                 
 *      - touch panel support                                
 *      - internal magnetic card reader support              
 *      - parallel printer support                           
 *      - comm enhancements                                  
 *      - PTC_600 constant                                   
 *  
 * 1.4  09/30/92        DLW                                           
 *      Added decode constant MATRIX_2_5. 
 *    
 * 1.4  04/21/93        SDE
 *      Disabled preprocessor definition of '_FAR' and changed all func-
 *      tions using '_FAR' to 'far'.  '_FAR' was causing a conflict with
 *      the Borland C compiler.
 *
 * 1.4  05/12/93        SDE
 *      Changed function names setdate to tsetdate and settime to tsettime.
 *      This is to eliminate function name conflicts with Borland C func-
 *      tions.
 *
 * 3.1  06/29/93        FJL
 *      Made the following changes:
 *      1.  Added TELX_TRUE and TELX_FALSE.
 *
 * 3.1  07/02/93        ECM
 *      Made the following changes:
 *      1.  Added _TDL for pascal calling convention.
 *
 * 3.1  10/21/93        FJL
 *      Added function declaration for getprtype(). Added void param
 *      to getprquest() and getthpap() function declarations.
 *
 * 3.1  10/21/93        FJL
 *      Made the following changes:
 *      1. Corrected function declaration for getprtype() to return int vs.
 *         unsigned int.
 *      2. Added function declaration of writefstr(). This procedure is  
 *         like writestr() except that it accepts a far pointer to the 
 *         string to be displayed.
 *      3. Changed function declaration of getrepeatdur() to be void.
 *      4. Added function declaration for telxonptc().
 *          
 * 3.1  11/13/93        FJL
 *      Made the following changes:
 *      1. Added function declarations for chkspecfunc(), setspecfunc(),
 *         and getspecfunc().
 *
 * 3.1  11/15/93        FJL
 *      Made the following changes:
 *      1. Added constant SCROLLDEFAULT which defines default scroll
 *         speed.
 *
 * 3.1  11/16/93        FJL
 *      Made the following changes:
 *      1. Added procedure declarations for the following functions:
 *         tfgetrvptr(), tfsetrvptr(), tfgetrvlen(), tfsetrvlen(), and
 *         tfclearrvlen().
 *
 * 3.1  11/17/93        FJL
 *      Made the following changes:
 *      1. Added file i/o constant ABEND_FOUND.
 *
 * 3.1  11/18/93        FJL
 *      Made the following changes:
 *      1. Added procedure declarations for the following functions:
 *         cleardispc(), getcurposc(), readcharc(), readcurcharc(),
 *         setcurposc(), writecharc(), and writestrc().
 *
 * 3.1  11/29/93        FJL
 *      Made the following changes:
 *      1. Added constants SCANNER_TSR_INTERFACE and SCANNER_BIOS_INTER-
 *         FACE.
 *      2. Added procedure declarations for testscanvec() and
 *         scaninterface().
 *
 * 3.1  12/01/93        FJL
 *      Made the following changes:
 *      1. Added WND_AUX_DEF_STRUCT structure definition and procedure 
 *         declaration for scanauxconfig().
 *      2. Added procedure declaration for scanautosense().
 *
 * 3.1  12/01/93        ECM
 *      Made the following changes:
 *      1. Added procedure declarations for setwakectrl() and getwakectrl().
 *      2. Added constants for setwakectrl() and getwakectrl() .
 *
 * 3.1  12/06/93        ECM
 *      Made the following changes:
 *      1. Added procedure declarations for issue_get_cap(), 
 *	        issue_poll_cap() and get_capacity().
 *      2. Added constant TELX_NOT_COMPLETE .
 *
 * 3.1  01/05/94        ECM
 *      Made the following changes:
 *      1. Added the constants TELX_COM1_ON,TELX_COM1_OFF,TELX_COM2_ON,
 *      TELX_COM2_OFF.  These where added to support the TSD interface which
 *      supports com1 and com2 queue's,handshaking, device selects, etc.
 *
 * 3.1  02/23/94		   ECM
 *		  Changed PWRUP_TRIG constant to equal actual value returned from BIOS.
 *		  Original PWRUP_TRIG = 03, should = 04.  Also added new equates for
 *      PWRUP_MICRO_RADIO, PWRUP_DIGITIZER, and PWRUP_OPTICS.  These constants
 *		  are the return values from the pwrupstatus function.
 *
 * 3.1  03/03/94        ECM
 *      Made the following changes:
 *      1. Added the constants TELX_COM1, TELX_COM2 for use with changes made
 *		  to deviceselect() and iobconfig() to support com2.
 *
 ************************************************************************/
				/*  */
#if !defined(_TELXON)           /* Prevent user from #including twice */
#define _TELXON

/**************************************************************************
*
*  Disabled the following preprocessor code because the '_FAR' definition in
* the Borland section was causing a conflict with Borland definition.  All
* '_FAR' source code (limited to FILE source code) has been changed to 'far'.
*
*  NOTE : Only the '_FAR' lines below have been disabled
*
* 04/29/93 SDE
*
**************************************************************************/

#if !defined(__BORLANDC__)      /* not Borland C++ */
#if !defined(_MSC_VER_)         /* MSC 5.1 */

/***** disabled >>>>>> #define _FAR far ***/
#define _NEAR near
#define _PASCAL pascal

#else                           /* MSC 6.0 & greater */

/***** disabled >>>>>> #define _FAR _far ***/
#define _NEAR _near
#define _PASCAL _pascal
#endif

#else                           /* Borland C++ */

/***** disabled >>>>>> #define _FAR far ***/
#define _NEAR near
#define _PASCAL pascal

#endif   

#define _TDL pascal

/* Constants -- General */
#define TELX_ERR        0xFFFF
#define TELX_ON          1
#define TELX_OFF         0
#define TELX_UP          1
#define TELX_DN          0
#define TELX_TRUE        1
#define TELX_FALSE       0
#define TELX_UNSUPP      -3

/* Constants -- Video */
#define BLOCKCUR        1               /* Cursor types */
#define UNDLINECUR      2
#define NOCUR           3
#define TEXTMODE        0x02            /* Video modes */
#define TEXT21          0x00
#define GRAFXMODE       0x05
#define L_TEXTMODE      0x82            /* Video modes - long char mode  1/91*/
#define L_TEXT21        0x80                                          /* 1/91*/
#define L_GRAFXMODE     0x85                                          /* 1/91*/
#define NORMAL_VID      0x07            /* Video attributes */
#define INVERSE_VID     0x70

/* Constants -- VGA Video  */ 

#define MAX_VGA_MODES   0x13
#define VGA_40X25_VID   0x00
#define VGA_NORMAL_VID  0x03
#define VGA_320X200_VID 0x04
#define VGA_640X200_VID 0x06


#define DONT_CARE       80
#define HIGH_BIT        0x8000
#define D_USA           0x00            /* Display languages */     /* 6/92 */
#define D_DANISH        0x01                                        /* 6/92 */
#define D_GERMAN        0x02                                        /* 6/92 */
#define D_GREEK         0x03                                        /* 6/92 */
#define D_HEBREW        0x04                                        /* 6/92 */
#define D_NORWEGIAN     0x05                                        /* 6/92 */
#define D_SWEDISH       0x06                                        /* 6/92 */
#define D_TURKISH       0x07                                        /* 6/92 */

/* Constants Battery Control */

#define CMD_NOT_AVAILABLE       0x41
#define CMD_ACCEPTED            0x40
#define CMD_NOT_ACCEPTED        0x00
#define RESULT_NOT_ACCURATE     0x81
#define POLL_CMD_COMPLETE       0x80
#define CAPACITY_UNAVAILABLE    -100
#define CYCLE_BATT_ERR          -2

/* Constants -- Rs232 Communications */
#define COM1            0x3f8       /* Communication channel 1  */
#define COM2            0x2f8       /* Communication channel 2  */

#define TELX_COM1_ON    1          /* General Com1 enable  */       /* 1/94 */
#define TELX_COM1_OFF   0          /* General Com1 disable */

#define TELX_COM2_ON    0x81       /* General Com2 enable  */
#define TELX_COM2_OFF   0x80      /* General Com2 disable  */

																						 /* 3/94 */
#define TELX_COM1       0x00       /* Com1 deviceselect(), iobconfig() */
#define TELX_COM2       0x80       /* Com2 deviceselect(), iobconfig() */



/* Comm status constants    */
#define LOST_DSR        0x8000      /* Lost DSR */
#define TIME_OUT        0x8000      /* Time out status bit      */
#define TX_SR_EMPTY     0x4000      /* Transmit shift reg empty */
#define TX_HR_EMPTY     0x2000      /* Transmit holding register empty */
#define BREAK_DETECT    0x1000      /* Break detected */
#define FRAMING_ERR     0x800       /* Framing error  */
#define PARITY_ERR      0x400       /* Parity error */
#define OVRRUN_ERR      0x200       /* Overrun error */
#define DATA_AVAIL      0x100       /* Data Ready   */
#define RLSD            0x80        /* Received line signal detect */
#define RING_IN         0x40        /* Ring indicator */
#define DSR             0x20        /* Data set ready */
#define CTS             0x10        /* Clear to send */
#define DELTA_RLSD      0x8         /* Change in receive line signal */
#define TR_EDGE_RI      0x4         /* Trailing-edge ring indicator */
#define DELTA_DSR       0x2         /* Change in data-set ready status */
#define DELTA_CTS       0x1         /* Change in clear-to-send status */

/*  Data bits */
#define DATA_5          0x00        /* Word length 5 data bits  */
#define DATA_6          0x01        /* Word length 6 data bits  */
#define DATA_7          0x02        /* Word length 7 data bits  */
#define DATA_8          0x03        /* Word lenght 8 data bits  */

/*  Stop bits */
#define STOP_1          0x00        /* Stop bits 1  */
#define STOP_2          0x04        /* Stop bits 2  */

/*  Parity  */
#define NO_PARITY       0x00        /* No parity    */
#define ODD_PARITY      0x08        /* Odd parity   */
#define EVEN_PARITY     0x18        /* Even parity  */

/*  Standard baud rate values */
#define BAUD_110        0x00        /* Baud rate 110 */
#define BAUD_150        0x01        /* Baud rate 115 */
#define BAUD_300        0x02        /* Baud rate 300 */
#define BAUD_600        0x03        /* Baud rate 600 */
#define BAUD_1200       0x04        /* Baud rate 1200 */
#define BAUD_2400       0x05        /* Baud rate 2400 */
#define BAUD_4800       0x06        /* Baud rate 4800 */
#define BAUD_9600       0x07        /* Baud rate 9600 */

/*  Extended baud rate values */
#define BAUD_50         0x08        /* Baud rate 50    */
#define BAUD_75         0x09        /* Baud rate 75    */
#define BAUD_1350       0x0a        /* Baud rate 1350  */
#define BAUD_1800       0x0b        /* Baud rate 1800  */
#define BAUD_19200      0x0c        /* Baud rate 19200 */
#define BAUD_38400      0x0d        /* Baud rate 38400 */

/* Devices */
#define TCM             0x03        /* TCM device */
#define PRINTER         0x00        /* Printer device */

/*  Communication type */
#define POLL            0x00        /* Polled data communications    */
#define INTR            0x01        /* Interrupt data communications */

/*  Receive interrupt Constants */
#define RECV            0x01        /* Initialize receive queue */
#define RECV_QUE_DATA   0x01        /* Receive interrupt queue has data */
#define RECV_QUE_EMPTY  0x02        /* Receive interrupt queue empty */
#define RECV_QUE_OVR    0x03        /* Receive queue overflowed */
#define RECV_ERR        0x04        /* Receive Error */
#define RECV_OK         0x00        /* Receive Ok    */
#define SEND_OK         0x00        /* Send Ok  */


/*  Transmit interrupt Constants */
#define XMIT            0x00        /* Initialize transmit queue */
#define XMIT_QUE_DATA   0x05        /* Transmit queue has data to transmit */
#define XMIT_QUE_EMPTY  0x06        /* Transmit queue is empty */

/*  I/O board Constants */
#define MODEM_202       0x02        /* Internal 202 modem */
#define MODEM_212       0x03        /* Internal 212 modem */
#define EXTERNAL        0x00        /* External device    */
#define SPEAKER         0x01        /* Acoustic coupler                 7/91 */

/*  I/O board Options   */
#define ACOUSTIC        0x08        /* Acoustic/Microphonic mode */
#define ACOUSTIC_LOW    0x20        /* Low acoustic volume */
#define MAGNETIC        0x10        /* Magnetic receive pick-up */
#define MONITOR         0x28        /* Monitor Rj11 transmission */
#define RJ11            0x00        /* Rj11 connect type */
#define RS422           0x05        /* External RS422 data communications */
#define RS232           0x00        /* External RS232 data comm         9/91 */
#define TELEPHONE       0x03        /* 212 Telephone mode */
#define OPTICAL         0x40        /* Optical connect option */

/*  I/O board return constants  */
#define STAT_RF           0x50      /* Rf-I/O board         */ 
#define STAT_202          0x20      /* 202 I/O board        */
#define STAT_212          0x30      /* 212 I/O board        */
#define STAT_202_212      0x60      /* 202-212 I/O board    */
#define STAT_OPTIC        0x70      /* optical I/O board    */

/*  I/O board error codes       */
#define INV_IOB           0xfffe    /* Invalid I/O board for option */


/*  Tone dialing constants */
#define TONE_0  0x10        /* Symbol 0, Freq= 941+1336 */
#define TONE_1  0x11        /*        1,       697+1209 */
#define TONE_2  0x12        /*        2,       697+1336 */
#define TONE_3  0x13        /*        3,       697+1447 */
#define TONE_4  0x14        /*        4,       770+1209 */
#define TONE_5  0x15        /*        5,       770+1336 */
#define TONE_6  0x16        /*        6,       770+1477 */
#define TONE_7  0x17        /*        7,       852+1209 */
#define TONE_8  0x18        /*        8,       852+1336 */
#define TONE_9  0x19        /*        9,       852+1477 */
#define TONE_A  0x1a        /*        A,       697+1633 */
#define TONE_B  0x1b        /*        B,       700+1633 */
#define TONE_C  0x1c        /*        C,       852+1633 */
#define TONE_D  0x1d        /*        D,       941+1633 */
#define TONE_ST 0x1e        /*        *,       941+1209 */
#define TONE_LB 0x1f        /*        #,       941+1477 */

/*  Modem Frequencies   */
#define V_23            0x24        /*  Freq=   1300    */
#define BELL_202        0x26        /*  Freq=   1200    */
#define V_21_ORG        0x28        /*  Freq=   980     */
#define BELL_103__ORG   0x2a        /*  Freq=   1070    */
#define V_21_ANS        0x2c        /*  Freq=   1650    */
#define BELL_103_ANS    0x2e        /*  Freq=   2025    */

/*  Melody Tones    */
#define D5_SH           0x30        /*  Note D#5    Freq=       622.3   */
#define E5              0x31        /*       E5     Freq=       659.3   */
#define F5              0x32        /*       F5     Freq=       698.5   */
#define F5_SH           0x33        /*       F#5    Freq=       740.0   */
#define G5              0x34        /*       G5     Freq=       784.0   */
#define G5_SH           0x35        /*       G#5    Freq=       830.6   */
#define A5              0x36        /*       A5     Freq=       880.0   */
#define A5_SH           0x37        /*       A#5    Freq=       932.3   */
#define B5              0x38        /*       B5     Freq=       987.8   */
#define C6              0x39        /*       C6     Freq=       1046.5  */
#define C6_SH           0x3a        /*       C#6    Freq=       1108.7  */
#define D6              0x29        /*       D6     Freq=       1174.7  */
#define D6_SH           0x3b        /*       D#6    Freq=       1244.5  */
#define E6              0x3c        /*       E6     Freq=       1318.5  */
#define F6              0x3d        /*       F6     Freq=       1396.9  */
#define F6_SH           0x0e        /*       F#6    Freq=       1480.0  */
#define G6              0x3e        /*       G6     Freq=       1568.0  */
#define G6_SH           0x2c        /*       G#6    Freq=       1661.2  */
#define A6              0x3f        /*       A6     Freq=       1760.0  */
#define A6_SH           0x04        /*       A#6    Freq=       1864.7  */
#define B6              0x05        /*       B6     Freq=       1975.5  */
#define C7              0x25        /*       C7     Freq=       2093.0  */
#define C7_SH           0x2f        /*       C#7    Freq=       2217.5  */
#define D7              0x06        /*       D7     Freq=       2349.3  */
#define D7_SH           0x07        /*       D#7    Freq=       2489.0  */


/* Constants -- System Services */
#define PWRUP_NOR        0          /* Normal power up */
#define PWRUP_ALARM      1          /* Power up by alarm */
#define PWRUP_RI         2          /* Power up by external wake up (RI) */
#define PWRUP_TRIG       4          /* Power up by laser trigger		 02/94*/	
#define PWRUP_MICRO_RADIO 8         /* Power up by micro radio 		 02/94*/	
#define PWRUP_DIGITIZER  0x10       /* Power up by digitizer   		 02/94*/	
#define PWRUP_OPTICAL    0x20       /* Power up by optics				 02/94*/	


#define PWR_CLR          0          /* Clear power down switch */
#define PWR_EXAM         1          /* Examine power down switch */
#define DIS_REBOOT       1          /* Bit 0 -- Disable re-boot sequence */
#define DIS_OFFKEY       2          /* Bit 1 -- Disable on/off key */
#define DIS_LITH         4          /* Bit 2 -- Disable lithium battery check*/
#define DIS_BRIDG_BATT   4          /* Bit 2 -- Disable bridge battery check (1180/40)*/    
#define NON_TEL          0          /* Non-Telxon Unit */
#define PTC_75X          1          /* PTC 750 family */
#define PTC_750IM        3          /* PTC 750 IM 8-line */
#define PTC_720ES        4          /* PTC 720 ES */
#define PTC_720          6          /* PTC 720 4.95 MHz */
#define PTC_730_8        7          /* PTC 730 8-line */
#define PTC_730_16       8          /* PTC 730 16-line */
#define PTC_740          9          /* PTC 740 */
#define PTC_710          10         /* PTC 710 non-graphics */
#define PTC_860_8        11         /* PTC 860 8-line */
#define PTC_860_16       12         /* PTC 860 16-line */
#define PTC_960_4        13         /* PTC 960 4-line non-graphics */
#define PTC_960_8        14         /* PTC 960 8-line */
#define PTC_960_16       15         /* PTC 960 16-line */
#define PTC_710G         17         /* PTC 710 graphics */
#define PTC_600          18         /* PTC 600 graphics */
#define PTC_960G         19         /* PTC 960 4-line graphics */
#define PTC_860IM        21         /* PTC 860IM */
#define POS_5000         22         /* POS 5000 */
#define PTC_1180         28         /* PTC 1180 */  
#define PTC_1140         30         /* PTC 1140 */  
#define CODEVIEW         0x80       /* Codeview debugger running */
#define SECOND           50         /* One second delay value */
#define DEL_BOOT         0          /* Cold boot type */
#define ESC_BOOT         1          /* Cold boot type */
#define RF_ROM           0          /* ROM type for flash functions    12/90 */
#define ARC_ROM          1          /* ROM type for flash functions    12/90 */
#define POS_SPEC_COP     1          /* POS-5000 special coprocessor    06/92 */

#define WAKE_COM1_RI     4          /* 1180 Wake up control bits       12/93 */
#define WAKE_COM2_RI     8
#define WAKE_LASER       0x10
#define WAKE_MICRO_RADIO 0x20
#define WAKE_DIGITIZER   0x40
#define WAKE_OPTICS      0x80

/* Constants -- Keyboard */
#define KEY_NOT_READY   0xFFFF      /* No character ready to be read */
#define KEY_READY       0x00        /* Character ready to be read */
#define DISP_PANEL      1           /* Touch panel codes */         /* 06/92 */
#define LKEY_PANEL      2                                           /* 06/92 */
#define RKEY_PANEL      3                                           /* 06/92 */
#define SIG_PANEL       4                                           /* 06/92 */
#define KEYS12          0           /* Display touch panel styles *//* 06/92 */
#define KEYS5BDR        1                                           /* 06/92 */
#define KEYS5INV        2                                           /* 06/92 */
#define CALIB_UL        0           /* Calibrate commands */        /* 06/92 */
#define CALIB_LR        1                                           /* 06/92 */
#define CALIB_PANEL     2                                           /* 06/92 */
#define FUNC_SHIFT      0x01        /* FUNC shift/lock bits */      /* 06/92 */
#define FUNC2_SHIFT     0x20                                        /* 06/92 */
#define FUNC_LOCK       0x40                                        /* 06/92 */
#define FUNC2_LOCK      0x80                                        /* 06/92 */

/* Constants -- Real Time Clock */
#define ABS_MODE        0           /* Comparator alarm modes */
#define REL_MODE        1
#define NO_REARM        0           /* Alarm enable parameters */
#define REARM           1
#define ALARM_TRIP      1           /* Alarm status flag */


/* Constants -- Miscellaneous Functions */
#define NO_VOL          0           /* Beeper volumes */
#define SOFT_VOL        1
#define NORMAL_VOL      2
#define LOUD_VOL        3
#define CDROPEN         0x00        /* Cash drawer status */
#define CDRCLOSED       0x40


/* Constants -- Micro Printer */
#define USA             0           /* Language character sets */
#define FRANCE          1
#define GERMANY         2
#define ENGLAND         3
#define DANISH          4                                           /* 6/92 */
#define SWEDISH         5                                           /* 6/92 */
#define ITALIAN         6                                           /* 6/92 */
#define SPANISH         7                                           /* 6/92 */
#define JAPAN           8           /* (Changed from 4 to 8) */     /* 6/92 */
#define ARABIC          0x61                                        /* 6/92 */
#define GREEK           0x62                                        /* 6/92 */
#define HEBREW          0x63                                        /* 6/92 */
#define NORWEGIAN       0x64                                        /* 6/92 */
#define TURKISH         0x65                                        /* 6/92 */
#define PRINT_DONE      5           /* Status codes */
#define PRINT_BUSY      6
#define DOT_LINE        7           /* Paper feed type */
#define TEXT_LINE       8
#define ONEPLYPAPER     0           /* Thermal printer paper */     /* 1/93 */
#define TWOPLYPAPER     1                                           /* 1/93 */
#define NOT_VALID_BLK   0           /* Block status in pr. queue */ /* 1/93 */
#define WAITING_QUE     1                                           /* 1/93 */
#define IN_PROGRESS     2                                           /* 1/93 */
#define DONE_NO_ERRORS  3                                           /* 1/93 */
#define ERROR_IN_PRINT  4                                           /* 1/93 */
#define PR_TIME_OUT     0x01        /* Printer status bits */       /* 6/92 */
#define PR_IO_ERROR     0x08                                        /* 6/92 */
#define PR_SELECTED     0x10                                        /* 6/92 */
#define PR_PAPER_OUT    0x20                                        /* 6/92 */
#define PR_CHAR_ACK     0x40                                        /* 6/92 */
#define PR_NOT_BUSY     0x80                                        /* 6/92 */
#define PRINT_ERROR     (PR_TIME_OUT | PR_IO_ERROR | PR_PAPER_OUT)  /* 6/92 */
#define PRNT_2400_BAUD  0           /* External printer baud rates */
#define PRNT_4800_BAUD  1
#define PRNT_9600_BAUD  2
#define COL_24          0           /* Number of print columns */    /* 6/92 */
#define COL_30          1
#define COL_36          2
#define COL_42          3  
#define IMPACT_PR       0           /* Types of printers */         /* 3/93 */
#define THERMAL_PR      1                                           /* 3/93 */
#define NODATAINQUEUE   0           /* Queue status */              /* 3/93 */
#define SOMEDATA        1                                           /* 3/93 */
#define QUEUEFULL       2                                           /* 3/93 */

/* Printer/communication types  (POLL and INTR defined above) */
//      POLL            0x00        /* External - Polled communication */
//      INTR            0x01        /* External - Interrupt-driven */
#define PARL            0x02        /* Internal parallel printer       06/92 */
#define PSCR            0x03        /* Interal parallel, print screen  06/92 */
#define SPOOLER         0x00        /* Spooler used for text output    06/92 */
#define NO_SPOOLER      0x01        /* Spooler not used for output     06/92 */

/* Constants - Scanner TSR */
#define INT_SCAN          1             /* Scanning methods */
#define WEDGE_SCAN        2
#define WEDGE_NEWLINE     3
#define PENCIL            0             /* Scanning devices */
#define LASER             1
#define L_TO_R            0             /* Scanning directions */
#define R_TO_L            1
#define PACK              0             /* scan_pack() constants */
#define UNPACK            1
#define SCAN_DATA_AVAIL   0             /* Scanning status */
#define NO_DATA_AVAIL     1
#define SCAN_ERROR        2
#define BUFF_OVRFLW       3

#define SCANNER_BIOS_INTERFACE  0
#define SCANNER_TSR_INTERFACE   1

/* Barcode label types */
#define PLESSEY         0x08            /* Regular Plessey */
#define ALPHA_PLESS     0x09            /* Alpha Plessey */
#define ISBN_PLESS      0x0A            /* ISBN Plessey */
#define PURE_PLESS      0x0B            /* Pure Plessey */
#define SAIN_PLESS      0x0C            /* Sainesbury Plessey */
#define UPC             0x10            /* UPC-A and UPC-E */
#define EAN             0x11            /* EAN-8 and EAN-13 */
#define UPC_EAN         0x12            /* UPC-A, UPC-E, EAN-8 and EAN-13 */
#define CODABAR         0x20            /* Codabar */
#define CODE_3_9        0x28            /* Code 3 of 9 */
#define CODE_2_5        0x30            /* Discrete, Interleaved, Industrial */
#define DISCR_2_5       0x31            /* Discrete 2 of 5 */
#define INTERL_2_5      0x32            /* Interleaved 2 of 5 */
#define INDUST_2_5      0x33            /* Industrial 2 of 5 */
#define MATRIX_2_5      0x34            /* Matrix 2 of 5 */
#define CODE_11         0x38            /* Code 11 (not implemented) */
#define CODE_128        0x40            /* Code 128 */

/* Constants -- Screen functions */

#define WBEEPDEFAULT    1               /* Wand beep default 20ms     */
#define SCROLLDEFAULT   10              /* Default scroll speed 200ms */
#define CLR             0x7a            /* Scan code for CLR key    */
#define INPUT_TYPE_ERR  0xeb            /* Input data type error    */
#define BAR             0x4b            /* Back arrow key           */
#define BSKEY           0x0e            /* Backspace key            */
#define DAR             0x50            /* Down arrow key scan code */
#define DEL             0x53            /* Delete key scan code     */
#define FAR             0x4d            /* Forward arrow key code   */
#define ESC             0x01            /* Escape key scan code     */
#define GOOD_DATA       0xff            /* Good data                */
#define LENGTH_ERR      0x02            /* Field length error       */
#define UAR             0x48            /* Up arrow key scan code   */
#define WND             0xec            /* Key value for wand input */
#define AUTO_ENTER      0x01        /* Automatically enter the data         */
#define WANDABLE        0x02        /* The scanner is enable for this prompt*/
#define MINUS_DELIM     0x04        /* Use '-' as the delimiter             */
#define MAG_CARD        0x08        /* The magnetic card reader is enabled  */
#define INV_PROMPT      0x20        /* Display the prompt in inverse video  */
#define INV_MASK        0x40        /* Display the mask in inverse video    */
#define INV_ACCEPTSCR   0x80        /* Inverse video in acceptscreen only   */
#define HEX_DATA        0x00        /* Hexidecimal data is valid            */
#define ALPHA_DATA      0x01        /* Alpha data is valid                  */
#define ALPNUM_DATA     0x03        /* Alpha/Numeric data is valid          */
#define NUMERIC_DATA    0x02        /* Numeric data is valid                */
#define STRING_FIELD    0x00        /* Store the data as a character string */
#define INT_FIELD       0x08        /* Store the data into a integer        */
#define LONG_FIELD      0x10        /* Store the data into a long           */
#define EDIT_DATA       0x04        /* Edit enabled                         */
#define ACCEPT_LTOR     0x00        /* Accept the data from left to right   */
#define ACCEPT_RTOL     0x80        /* Accept the data from right to left   */

/* Constants -- File Management System */

#define FD_FIXED     0          /* Recording techniques */
#define FD_VARIABLE  1
#define FD_VARIABLE1 2
#define FD_BINARY    1          /* Search/seek types */
#define FD_FIRST     0
#define FD_NEXT     -1
#define FD_LAST     -2
#define FD_PRIOR    -3
#define VRD_ID_LEN   6          /* Length of ID in VRD table */

/* tf function return codes */
#define CREATE_OPEN  1      /* File created and opened */
#define FUNC_SUCC    0      /* Function was successful */
#define INV_INPUT   -1      /* Input is invalid */
#define INV_FILE    -2      /* Invalid file name (DOS error code) */
#define INV_PATH    -3      /* Path not found (DOS error code */
#define NO_HANDLE   -4      /* No handle available (DOS error code */
#define ACC_DENIED  -5      /* Access denied (DOS error code) */
#define INV_HANDLE  -6      /* Invalid handle or file not open (DOS error) */
#define INV_TYPE    -7      /* Invalid record index or file type */
#define NO_MEMORY   -8      /* Can't allocate memory */
#define INV_FCB     -9      /* FCB already in use */
#define END_FOUND   -10     /* EOF encountered or record not found  */
#define BEG_FOUND   -11     /* Beginning of file encountered */
#define BAD_TEMP    -12     /* Error creating/writing to temporary file */
			    /* (tfsort only) */
#define ABEND_FOUND -13     /* Premature end of file found (tfread only) */

/* Constants for magnetic card reader TSR services. */

/* Configuration parameter constants */
#define SW_METHOD           0x01        /* methods for data to be accepted */
#define WEDGE_METHOD        0x02
#define READER_TYPE0        0x00        /* type of track data to be accepted */
#define READER_TYPE1        0x01
#define READER_TYPE2        0x02
#define READER_TYPE3        0x03
#define READER_TYPE4        0x04
#define MCARD_INTERNAL      0x00        /* device cfg parameter */  /* 6/92 */
#define DEFAULT_SW_CFG      0xff
#define FIX_LEN_DISABLE     0x00        /* fixed length parameter */
#define MC_NORM_DATA        0x00        /* magnetic card data modes */
#define MC_RAW_DATA         0x01

/* Magnetic card reader TSR return error codes */
//      NO_DATA_AVAIL       0x01        /* defined above */
#define INV_CFG_FMT         0x02        
//      BUFF_OVRFLW         0x03        /* defined above */
#define MCARD_IFACE_ERR     0x04
#define MCARD_OFF_ERR       0x05
#define APPL_METHOD_ERR     0x06
#define APPL_FIXLEN_ERR     0x07

#define MCARD_INSTALLED     0x01        /* mcardtestvec() return values */
#define MCARD_NOT_INSTALLED 0x00

/*********************************************/
/*  CONSTANTS FOR BARCODE PRINTING ROUTINES  */
/*********************************************/

/*  Constants used for calls to chckdone*/

#define EMPTY_QUEUE     1       /*Using this will cause chckdone to wait for
				  all print jobs to be finished before
				  returning control to the application.  Should
				  be used before exiting any program which
				  has printed a barcode.*/

#define CHECK_ONCE      0       /*Used to update the list of outstanding jobs
				  and free memory which was finished being
				  used.*/
/*******************
 *   Error Codes   *
 *******************/

#define INVD_LEN                1
#define INVD_CHR                2
#define MEM_NOT_AVAIL           4
#define BAR_TOO_BIG             8


/**************************
 *     SPECIAL OPTIONS    *
 * for use in code_dep_fn *
 **************************/


/**********************
 *   2 of 5 options   *
 **********************/

#define INDST25                 0
#define INTER25                 1
#define STRT25                  2

/***********************
 *   Codabar options   *
 ***********************/

#define START_A                 0
#define START_B                 1
#define START_C                 2
#define START_D                 3
#define STOP_A                  0
#define STOP_B                  4
#define STOP_C                  8
#define STOP_D                  0x0c

/***********************
 *   Code 39 options   *
 ***********************/

#define FULLASCII               1

/*********************
 *   UPC/EAN options *
 *********************/

#define UPCA                    0x00
#define UPCE                    0x01
#define EAN13                   0x02
#define EAN8                    0x03
#define ADD2DIG                 0x04
#define ADD5DIG                 0x08
#define SYSTEM0                 0x00
#define SYSTEM1                 0x10

/* Structures -- Real Time Clock */

struct am {                         /* Alarm mode structure */
    int am_mode;                    /* Alarm mode - absolute or relative */
    int am_month;                   /* Month (1-12) - absolute mode only */
    int am_day;                     /* Day (1-31) - absolute mode only */
    int am_hour;                    /* Hour (0-23) - absolute mode only */
    int am_min;                     /* Minute (0-59) - absolute mode */
};                                  /*    (1-65,535) - relative mode */

struct dt {
    int dt_year;                    /* Year (1980-2099) */
    int dt_month;                   /* Month within year (1-12) */
    int dt_day;                     /* Day of month (1-31) */
    int dt_dow;                     /* Day of week (0=Sunday, 1=Monday,...) */
    int dt_hour;                    /* Hours (0-23) */
    int dt_min;                     /* Minutes within hour (0-59) */
    int dt_sec;                     /* Seconds within minute (0-59) */
};

/* Structures -- Scanner functions */

typedef struct label_def {
    char label_type;                /* Barcode label type code */
    char label_length;              /* 0=variable length, 1-64=fixed length*/
    char generic_options;           /* Not used, set to 0 */
    char label_options;             /* Decode options for specific label */
    char drop_options;              /* Number of chars to drop off data */
    char gsn;                       /* Not used, set to 0 */
} LABEL_DEFINITION_STRUCT;

typedef struct bar_stat {
    int type;                       /* Barcode label type code */
    int direction;                  /* Direction of scan: L_TO_R, R_TO_L */
    int length;                     /* Actual number of characters in label */
} BAR_STAT_STRUCT;

typedef struct wnd_def {   /* Wand type structure       */
    char wandtype;          /* Indicates the type of barcode scanning device */
    char reserved1;
    char reserved2;
    char reserved3;
    char reserved4;
    char reserved5;
    char reserved6;
    char reserved7;
} WND_DEF_STRUCT;

typedef struct wnd_aux_def {   
    char wandtype;          
    char wand_time_out;
    char peck_rate;
    char clock_rate;
    char double_buffer;
    char auto_select;
    char wand_spare1;
    char wand_spare2;
} WND_AUX_DEF_STRUCT;

#define SCAN_CONFIG_RESERVED_SIZE 0x08    /* Size of wnd_def structure      */


/* Structures -- Screen I/O functions */
 
typedef struct dft_record           /* Define data format table record      */
{
	unsigned int    df1 ;       /* bit mapped data format #1 :          */
				    /*  bit 0 = auto record                 */
				    /*      1 = wand                        */
				    /*      2 = use '-' as deliminter       */
				    /*      3 = not used                    */
				    /*      4 = not used                    */
				    /*  bit 5 - 1 = Inverse video prompt    */
				    /*  bit 6 - 1 = Inverse video mask      */
				    /*  bit 7 - 0 = Inverse in ACCEPT/SHOW  */
				    /*          1 = Inverse in only ACCEPT  */
				
	unsigned int    df2 ;       /* bit mapped data format #2 :          */
				    /*  bit 1,0 = type of input --          */
				    /*            0 = hex                   */
				    /*            1 = alpha                 */
				    /*            2 = numeric               */
				    /*            3 = alpha/numeric         */
				    /*      2  = edit                       */
				    /*      5,4,3 = field storage --        */
				    /*            0 = char strings          */
				    /*            1 = integer               */
				    /*            2 = long                  */
				    /*            3 = not used              */
				    /*            4 = not used              */
				    /*      6  = reserved                   */
				    /*      7  = input direction            */
				
	unsigned char *prompt_str ;     /* offset to prompt literal         */
	unsigned int prompt_len ;       /* length of prompt                 */
	unsigned char *mask_str ;       /* offset to mask literal           */
	unsigned int mask_str_len ;     /* length of the mask               */
	unsigned int prompt_col ;       /* prompt location's x coordinate   */
	unsigned int prompt_row ;       /* prompt location's y coordinate   */
	unsigned int mask_col ;         /* mask location's x coordinate     */
	unsigned int mask_row ;         /* mask location's y coordinate     */
	unsigned char *default_str;     /* default data string offset adr   */
	unsigned int default_str_len ;  /* default string length            */
	unsigned int min_input ;        /* minimum number of char to input  */
	unsigned int max_input ;        /* maximum number of char to input  */
					/* Pointer to field where data will */
	union field_union{              /* be stored. One of three types:   */
	    unsigned char *fieldchar ;  /* field storage: character strings */
	    unsigned int  *fieldint ;   /* field storage: integer           */
	    unsigned long *fieldlong ;  /* field storage: long              */
	} field;

	unsigned int field_len ;        /* Maximum length of the field      */
	unsigned int clear_start_line;  /* beginning line number to clear   */
	unsigned int clear_end_line ;   /* ending line number to clear      */
	unsigned int status_line;       /* actuator status line             */
	struct dft_record *dft_link ;   /* link to the next dft             */
} DFT_STRUCT ;

/* Macros -- File Management System */

#define FD(fname)    char fname[32] = {0};  /* allocates space for the
					       FCB, transparent to user */

/* Structures -- File Management System */

struct _VRD                  /* Variable-length file record description */
{                            /*    i.e. (ID/count table) */
    char rec_id[VRD_ID_LEN]; /* Record ID, 1-5 characters in length */
    unsigned rec_len;        /* Length of record */
};
typedef struct _VRD VRD;
typedef struct _VRD far *PVRD;

#define RECORD_DEF_START(name)     VRD name[] = { {"MAGIC",(99)},
#define RECORD_DEF(a,b)            { a, (b)},  
#define RECORD_DEF_END             { 0,  0 } };


/* magtsr mcard_data structure.                                             */

typedef struct mcard_data_fmt{
	unsigned int method;
	unsigned int reader_type;
	unsigned int switch_cfg;
	unsigned int postamble;
	unsigned int post_len;
	unsigned int preamble[2];
	unsigned int pre_len;
	unsigned int fixed_len;
	unsigned int fill_ch;
} MC_DATAFMT_DEF_STRUCT;

/***************************************
 *   Structure to construct barcodes   *
 ***************************************/

#pragma pack(2)
struct barcode_def {
    char far *label;        /*Text for label*/

    int layout;             /*layout of barcode 1 = vertical 0 = horizontal*/

    int scale;              /*Number of pixel per narrow bar*/

    int supp_hr;            /*Suppress the output of human readable text*/

    int check_char;         /*Use the check character option*/

    int length;             /*Length of label string to be printed*/

    int  height;            /*Height of characters to be printed*/

    int  code_dep_fn;       /*Used to pass special information like system 0*/
};
#pragma pack()


/****************************************/
/* Telxon library function definitions  */
/****************************************/

/* Common functions */

/* Video procedure definitions */
unsigned int _TDL backlite(int on_off);
unsigned int _TDL cleardisp(int page);
unsigned int _TDL clearline(int beg_line, int end_line);
void _TDL disprefresh(void);
void _TDL dispreset(void);
unsigned int _TDL getcurpos(int page, int *row, int *col, int *mode);
void _TDL getvidmode(int *mode, int *cols, int *page);
void _TDL getvidsize(int *trow, int *tcol, int *grow, int *gcol);
unsigned int _TDL lcdadj(int up_dn, int num);
unsigned int _TDL readchar(int page, int row, int col);
unsigned int _TDL readcurchar(int page);
unsigned int _TDL readdot(int row, int col);
unsigned int _TDL scroll(int up_dn, int lines, int attr, int rowul, int colul, 
		    int rowlr, int collr);
unsigned int _TDL setbklitetmr(int count);
unsigned int _TDL setchar(int chr, char *str);
unsigned int _TDL setcurpos(int page, int row, int col);
unsigned int _TDL setcurtype(int type);
unsigned int _TDL setpage(int page);
unsigned int _TDL setvidmode(int mode);
void _TDL ttychar(char chr);
void _TDL ttycurstr(char *str, int len);
unsigned int _TDL ttystr(char *str, int len, int row, int col);
unsigned int _TDL writechar(char chr, int attr, int page, int count);
unsigned int _TDL writedot(int dot, int row, int col);
unsigned int _TDL writedotstr(unsigned char str, int row, int col);
unsigned int _TDL writeint(int n, int radix, int format);
unsigned int _TDL writestr(int page, char *str, int len, int attr);
unsigned int _TDL writefstr(int page, char far *str, int len, int attr);
unsigned int _TDL getlang(void); 
unsigned int _TDL setlang(unsigned int lang);
unsigned int _TDL cleardispc(void);
unsigned int _TDL getcurposc(int *row, int *col, int *mode);
unsigned int _TDL readcharc(int row, int col);
unsigned int _TDL readcurcharc(void);
unsigned int _TDL setcurposc(int row, int col);
unsigned int _TDL writecharc(char chr, int attr, int count);
unsigned int _TDL writestrc(char *str, int len, int attr);

/* Communications procedure definitions */
unsigned int _TDL carrdetectctrl( unsigned int on_off);
unsigned int _TDL ckrecvque(void);
unsigned int _TDL ckxmitque(void);
unsigned int _TDL ckrecvquep(int com_port);
unsigned int _TDL ckxmitquep(int com_port);

unsigned int _TDL comminit(int comm_port, int comm_mode, unsigned int baud,
		       int type);
void _TDL commpwroff(void);
unsigned int _TDL commpwroffp(int com_port);
unsigned int _TDL commstatus(int comm_port);
unsigned int _TDL deque(char *data);
unsigned int _TDL dequep(char *data, int com_port);

unsigned int _TDL deviceselect(unsigned int device);

unsigned int _TDL dtrctrl(int comm_port, int on_off);
unsigned int _TDL enque(int comm_port, char data);
unsigned int _TDL handshakectrl(unsigned  int on_off);
unsigned int _TDL inituserque(int comm_port, int type, unsigned char *queue,
			 unsigned int len);
unsigned int _TDL iobconfig(int device, unsigned int option);
int _TDL iobstatus(void);
unsigned int _TDL recvchar(int comm_port, char *data);
unsigned int _TDL recvintrctrl(int comm_port, int on_off);
unsigned int _TDL rs232timeout(int comm_port, unsigned int time_out);
unsigned int _TDL rtsctrl(int comm_port, int on_off);
unsigned int _TDL sendchar(int comm_port, unsigned char data);
unsigned int _TDL sendstr(int comm_port, char *str, int len);
unsigned int _TDL tonectrl(int comm_port);
unsigned int _TDL xonxoffctrl(int comm_port, int on_off);
void _TDL xmitmark(void);
void _TDL xmitspace(void);
unsigned int _TDL xmitmarkp(int com_port);
unsigned int _TDL xmitspacep(int com_port);

unsigned int _TDL enquestr(int comm_port, char far *str, unsigned len);  /* 06/92 */
unsigned int _TDL dequestr(int comm_port, char far *str,                 /* 06/92 */
		      unsigned far *len);                           /* 06/92 */
unsigned int _TDL ckquerem(int comm_port, int queue, unsigned *rem);     /* 06/92 */

/* System Service procedure definitions */
int _TDL biosver(void);
void _TDL setwakectrl(int type);                                         /* 12/93 */
unsigned int _TDL getwakectrl(int *status);
void _TDL kernelver(char *ver);                                          /* 03/92 */
void _TDL coldboot(int type);
void _TDL coldbootdown(void);
void _TDL sysdelay(unsigned int num);
void _TDL errormessage(unsigned char *message, int len);
unsigned int _TDL flasherase(int rom);                                   /* 12/90 */
unsigned int _TDL flashid(int rom, int *size);                           /* 12/90 */
unsigned int _TDL flashpower(int rom, int on_off);                       /* 12/90 */
unsigned int _TDL flashprogram(int rom, unsigned int device, int bank, int offset,
			  char *data, int len);                     /* 12/90 */
int _TDL getctrlstatus(void);
int _TDL gethaltrate(void);
unsigned int _TDL getrombank(void);
int _TDL gettimeout(void);
void _TDL poweroff(void);
int _TDL pwrupstatus(void);
unsigned int _TDL pwrdwnswch(int mode);
unsigned int _TDL ramprotect(int on_off);
unsigned int _TDL setctrlstatus(int cflag);
unsigned int _TDL sethaltrate(int rate);
unsigned int _TDL setrombank(int bank_handle);
unsigned int _TDL settimeout(int seconds);
int _TDL termid(void);
int _TDL copver(int cop);                                                /* 6/92 */
unsigned int _TDL telxonptc(void);                                       /* 6/93 */

/* Battery control services */

int _TDL issue_poll_cap(int *capacity);                                   /*12/93*/
int _TDL issue_get_cap(void);
int _TDL get_capacity(void);

/* Keyboard procedure definitions */
unsigned int _TDL chkkeybrd(unsigned char *character, unsigned char *scan_code);
unsigned int _TDL clickdur(int num_ms);
int _TDL getkeybrdsize(void);
unsigned int _TDL keybrdclick(int on_off);
unsigned int _TDL keybrdint(int on_off);
void _TDL loadkeybrd(unsigned char *keybrd_table);
unsigned char _TDL readkeybrd(unsigned char *scan_code);
void _TDL resetkeybrd(void);
unsigned int _TDL shiftstatus(void);
unsigned int _TDL keyrepeat(int on_off);
unsigned int _TDL setrepeatdur(int first, int nth);
void _TDL getrepeatdur(int *first, int *nth);
void _TDL restorekeybrd(void);
void _TDL getkeybrd(unsigned char *keybrd_table);
unsigned int _TDL calibtouch(int panel, int cmd);                        /* 6/92 */
unsigned int _TDL deftouchkey(int panel, char far *table);               /* 6/92 */
unsigned int _TDL disptouch(int page, int style, char far *title,        /* 6/92 */
		       int tlen, char far *text, char far *codes);  /* 6/92 */
unsigned int _TDL gettouchdef(int panel, char far *table);               /* 6/92 */
unsigned int _TDL gettouchkey(int panel, char far *table, int *num_keys);/* 6/92 */
unsigned int _TDL loadtouchkey(int panel, char far *table, int num_keys);/* 6/92 */
unsigned int _TDL touchpanel(int panel, int on_off);                     /* 6/92 */
unsigned int _TDL setfunc(int state);                                    /* 6/92 */
unsigned int _TDL getfunc(void);                                         /* 6/92 */

/* Real Time Clock procedure definitions */
unsigned int _TDL alarmconfig(struct am *str);
unsigned int _TDL alarmset(int on_off, int re_arm);
unsigned int _TDL alarmstatus(struct am *str);
void _TDL datetime(struct dt *clock);
unsigned int _TDL datetimestr(struct dt *clock, char *ptr);

unsigned int _TDL tsetdate(struct dt *clock);        /* 5-12-93 chg setdate to tsetdate */
unsigned int _TDL tsettime(struct dt *clock);        /* 5-12-93 chg settime to tsettime */

/* Micro printer procedure definitions */
unsigned int _TDL acceptlines(int lines, int comm_type);
unsigned int _TDL bitimage(int horz, int lines, int comm_type);
unsigned int _TDL characterset(int char_set, int comm_type);
int _TDL checkprint(void);
unsigned int _TDL paperfeed(int lines, int type, int comm_type);
unsigned int _TDL printcol(int num_cols, int comm_type);
unsigned int _TDL printerbaud(int baud, int comm_type);
void _TDL clrpwrfail(void);                                          /* 6/92 */
int _TDL getcharset(int type);                                       /* 6/92 */
int _TDL getformlen(void);                                           /* 6/92 */
int _TDL getprintcol(void);                                          /* 6/92 */
unsigned int _TDL getprintstat(unsigned int *stat);                  /* 6/92 */
int _TDL getprmode(void);                                            /* 6/92 */
int _TDL getprtmout(void);                                           /* 6/92 */
unsigned int _TDL initprinter(void);                                 /* 6/92 */
void _TDL printscreen(void);                                         /* 6/92 */
unsigned int _TDL printstr(char far *str, unsigned int len);         /* 6/92 */
unsigned int _TDL prnpower(int on_off);                              /* 6/92 */
void _TDL setformlen(int lines);                                     /* 6/92 */
unsigned int _TDL setprmode(int mode);                               /* 6/92 */
unsigned int _TDL setprtmout(int tmout);                             /* 6/92 */
unsigned int _TDL bckpaperfeed(int lines, int type);                 /* 3/93 */
unsigned int _TDL blockgrph(int height, int width, char far *buffer); /* 3/93 */
unsigned int _TDL charimage(int char_set, int size, int char_num, char 
    far *buffer);                                               /* 3/93 */
unsigned int _TDL getblockst(char block);                            /* 3/93 */
unsigned int _TDL getprquest(void);                                  /* 3/93 */
unsigned int _TDL getthpap(void);                                    /* 3/93 */
unsigned int _TDL linespace(int lines);                              /* 3/93 */
unsigned int _TDL setcntst(int mode);                                /* 3/93 */
unsigned int _TDL setthpap(int mode);                                /* 3/93 */
int _TDL getprtype(void);                                            /* 6/93 */

/* Miscellaneous procedure definitions */
unsigned int _TDL beeper(int on_off);
int _TDL memsize(void);
unsigned int _TDL setvolume(int vol);
unsigned int _TDL lowbat(void);                                      /* 06/91 */
void _TDL setlowbatlen(int val);                                     /* 03/92 */
int _TDL getlowbatlen(void);                                         /* 03/92 */
unsigned int _TDL statusled(int on_off);                             /* 06/92 */
unsigned int _TDL cashdrstat(void);                                  /* 10/92 */
unsigned int _TDL opencashdr(void);                                  /* 10/92 */

/* Scanner procedure definitions */
unsigned int _TDL scanner(int on_off);
unsigned int _TDL scanconfig(int device, struct label_def *types);
unsigned int _TDL scanread(char *buffer, int len, struct bar_stat *status);
unsigned int _TDL setscanvec(int vec);
void _TDL scanpack(unsigned char *cfg_ptr, int n);
unsigned int _TDL testscanvec(void);
unsigned int _TDL scaninterface(void);
unsigned int _TDL scanauxconfig(WND_AUX_DEF_STRUCT *wnd_def_ptr,
				LABEL_DEFINITION_STRUCT *label_def_ptr);
unsigned int _TDL scanautosense(int on_off);

/* Screen I/O procedure definitions     */
unsigned int _TDL acceptscreen(DFT_STRUCT *dft);
void _TDL showscreen(DFT_STRUCT *dft);
unsigned int _TDL alphacheck(unsigned char c);
unsigned int _TDL numericcheck(unsigned char c);
unsigned int _TDL hexcheck(unsigned char c);
void _TDL setmaskchars(char *mstr, int num);
unsigned int _TDL setbarcodes(WND_DEF_STRUCT *wandparms, LABEL_DEFINITION_STRUCT *types);
unsigned int _TDL getlabeltype(unsigned char *type); 
void _TDL setdelayvals(unsigned int scrl_speed, unsigned int w_delay);
void _TDL disablewand(void);

/* File Management System definitions */
int _TDL tfopen(char far *filename, char far *fcb_ptr, PVRD id_cnt_ptr,
	   int rec_tech, unsigned len);
int _TDL tfclose(char far *fcb_ptr);
int _TDL tfread(char far *fcb_ptr, char far *buffer);
int _TDL tfwrite(char far *fcb_ptr, char far *buffer, int index);
int _TDL tfrewrite(char far *fcb_ptr, char far *buffer, int index);
int _TDL tfinsert(char far *fcb_ptr, char far *buffer, int index);
int _TDL tfdelete(char far *fcb_ptr);
int _TDL tfsearch(char far *fcb_ptr, int mode, char wchar, char far *key,
	     unsigned offset, unsigned len);
int _TDL tfseek(char far *fcb_ptr, long mode);
int _TDL tfreset(char far *fcb_ptr);
long _TDL tfdiskfree(char far *fcb_ptr, long far *disk);
long _TDL tftell(char far *fcb_ptr, long far *size, long far *rec_num);
int _TDL tfsort(char far *fcb_ptr, unsigned offset, unsigned len);
int _TDL tfgetrvptr(char far *fcb_ptr, long *rv_ptr); 
int _TDL tfsetrvptr(char far *fcb_ptr, long rv_ptr);
int _TDL tfgetrvlen(char far *fcb_ptr, unsigned int *rv_len);
int _TDL tfsetrvlen(char far *fcb_ptr, unsigned int rv_len);
int _TDL tfclearrvlen(char far *fcb_ptr);

/* Magcard definitions */
unsigned int _TDL magcard(int on_off);
unsigned int _TDL mcardcfg(struct mcard_data_fmt *data_fmt);
unsigned int _TDL mcardread( char * buffer, int buf_len, int *data_len);
unsigned int _TDL mcardsetvec( int vec);
unsigned int _TDL mcardtestvec(void);
unsigned int _TDL mcgetbeeplen(void);
unsigned int _TDL mcsetbeeplen(int set_value);
unsigned int _TDL mcgetmode(unsigned int *mode);                     /* 6/92 */
unsigned int _TDL mcgettmout(unsigned int *tmout);                   /* 6/92 */
unsigned int _TDL mcsetmode(unsigned int mode);                      /* 6/92 */
unsigned int _TDL mcsettmout(unsigned int tmout);                    /* 6/92 */


/*********************************
 *   Barcode Printing Routines   *
 *********************************/

void _TDL chckdone(char limit);                                      /* 3/93 */
int _TDL codabar(struct barcode_def far *values);                    /* 3/93 */
int _TDL code39(struct barcode_def far *values);                     /* 3/93 */
int _TDL code93(struct barcode_def far *values);                     /* 3/93 */
int _TDL code11(struct barcode_def far *values);                     /* 3/93 */
int _TDL code128(struct barcode_def far *values);                    /* 3/93 */
int _TDL i2of5(struct barcode_def far *values);                      /* 3/93 */
void _TDL initbrcd(struct barcode_def far *values);                  /* 6/93 */
int _TDL plessey(struct barcode_def far *values);                    /* 3/93 */
int _TDL upc(struct barcode_def far *values);                        /* 3/93 */

int _TDL chkspecfunc(unsigned char key_val);
void _TDL setspecfunc(unsigned char *ptr);
unsigned char * _TDL getspecfunc(void);

#endif                                  /* End #if !defined(_TELXON) */
