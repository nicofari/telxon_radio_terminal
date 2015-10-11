/************************************************************************\
*                                                                        *
*   DATE:       4/5/91      WRITTEN BY:  Telxon O/S                      *
*                                                                        *
*   FILE NAME:  ptap.h                                                   *
*                                                                        *
*   DESCRIPTION:                                                         *
*       This include file contains the macro definitions and             *
*       function prototypes for the PTAP libraries.                      *
*       Changed framing_err to framing_err_ptap                          *
*                                                                        *
*   Revisions:                                                           *
*       2.4     09/23/92                                                 *
*               Made the following changes:                              *
*               1.  Renamed the following macros -                       *
*                   HANDSHAKE_ENABLE  -> HSHAKE_ENABLE                   *
*                   HANDSHAKE_DISABLE -> HSHAKE_DISABLE                  *
*                   FORTY_MILLS       -> FORTY_MS_DIAL_DELAY             *
*                   SIXTY_MILLS       -> SIXTY_MS_DIAL_DELAY             *
*                                                                        *
*       2.6     06/14/93                                                 *
*               Made the following changes:                              *
*               1.  Updated function prototypes.                         *
*               2.  Removed NO_PAD. It is defined in ptaplocl.h.         *
*                                                                        *
*       2.6     07/13/93        HH                                       *
*               1. Added _TDL macro for pascal calling convention.       *
*               2. Added _TDL to function prototypes                     *    
*               3. Added TELX_TRUE, TELX_FALSE.                          *
*                                                                        *
*       3.1     08/20/93    FJL                                          *
*               Made the following changes:                              *
*               1.  Renamed _TDL to be _PTAPTDL to avoid conflicts with  *
*                   header files from other Telxon libraries.            *
*               2.  Removed TELX_TRUE and TELX_FALSE.                    *
*                                                                        *
\************************************************************************/

/*      Boolean macros.         */
#define TRUE                    0x01
#define FALSE                   0x00


/*      Macros for CD_LIT structure */
/*      Comm type macros.           */
#define BURST                   0x01
#define PTAP                    0x02
#define SNAP                    0x03
#define INTERRUPT               0x04
#define IP_40                   0x05
#define CRC_BISYNC              0x06
#define CRC_SDLC                0x07
#define SPECIAL                 0x00

/*      CD mode macros. */
#define ASCII                   0x00
#define SMART_MODE              0x02
#define RECV_CR                 0x04
#define NO_TRANSLATE            0x08
#define PAD                     0x10
#define PTAP_MODE               0x40
#define TRANSPARENT             0x80

#define TWO_BYTE_BCC            0x01
#define ONE_BYTE_BCC            0xfe

/*      Func pack/no pack macros */
#define NO_PACK_UNPACK          0x00 
#define PACK_UNPACK             0x01

/*      Tone macros.            */
#define V23_MARK                0x24
#define V23_SPACE               0x25
#define BELL_202_MARK           0x26
#define BELL_202_SPACE          0x27
#define V21O_MARK               0x28
#define V21O_SPACE              0x29
#define BELL_103O_MARK          0x2a
#define BELL_103O_SPACE         0x2b
#define V21A_MARK               0x2c
#define V21A_SPACE              0x2d
#define BELL_103A_MARK          0x2e
#define BELL_103A_SPACE         0x2f

/*
 *  Block size constansts
 */
#define BLOCK_128               128
#define BLOCK_1024              1024

/*
 *  Default First, nth character timeouts. Resolution is 4ms.
 *  
 */
#define FIRST_CHAR_TIME         750         /* First character timeout 3 sec */
#define NTH_CHAR_TIME           10          /* Nth character timeout 40ms    */

/*
 *  Default delay constants for 202 answer pre answer delay,
 *  answer delay, post answer delay    
 */
#define DFLT_ANS_PRE            2000
#define DFLT_ANS_DLY            3000
#define DFLT_ANS_POST           75

/*      Baud rate constants     */
#define PTAP_BAUD_110           0x00
#define PTAP_BAUD_150           0x20
#define PTAP_BAUD_300           0x40
#define PTAP_BAUD_600           0x60
#define PTAP_BAUD_1200          0x80
#define PTAP_BAUD_2400          0xa0
#define PTAP_BAUD_4800          0xc0
#define PTAP_BAUD_9600          0xe0
#define PTAP_BAUD_50            0x01
#define PTAP_BAUD_75            0x21
#define PTAP_BAUD_1350          0x41
#define PTAP_BAUD_1800          0x61
#define PTAP_BAUD_19200         0x81

/*      Uart configuration      */
#define RESET_DATA_BITS         0xfc
#define DATA_BITS_5             0x00
#define DATA_BITS_6             0x01
#define DATA_BITS_7             0x02
#define DATA_BITS_8             0x03
#define STOP_BITS_1             0x00
#define STOP_BITS_2             0x04
#define PARITY_NONE             0x00
#define PARITY_ODD              0x08
#define PARITY_EVEN             0x18

/*      Device type macros.     */
#define EXTERNAL                0x00
#define SPEAKER                 0x01
#define INTERNAL_202            0x02
#define INTERNAL_212            0x03

/*      Option type macros  */
#define RJ11                    0x00
#define RJ11_BELL_150           0x03
#define RJ11_BELL_1200          0x04
#define RJ11_V23_75             0x02
#define RJ11_V23_600            0x07
#define RJ11_MONITOR            0x01
#define RJ11_TELEPHONE          0x03
#define MAGNETIC                0x10            
#define MICROPHONIC_212         0x30
#define MICROPHONIC_202         0x08
#define TELEPHONE_MODE          0x1B
#define MONITOR_202             0x28
#define MONITOR_212             0x39
#define OPTICAL                 0x40
#define ACOUSTIC_LOW            0x20
#define RS_422_485              0x05
#define RS_232                  0x00

/*      Comm in function macros.        */
#define DSR_IN                  0x00
#define CTS_IN                  0x01
#define MARK_IN                 0x02
#define CARRIER_IN              0x03

/*      Comm out function macros.       */
#define DTR_HIGH                0x00
#define DTR_LOW                 0x01
#define RTS_HIGH                0x02
#define RTS_LOW                 0x03
#define XMT_MARK                0x04
#define XMT_SPACE               0x05
#define FSK_HIGH                0x06
#define FSK_LOW                 0x07
#define CD_ENABLE               0x08
#define CD_DISABLE              0x09
#define HSHAKE_ENABLE           0x0a
#define HSHAKE_DISABLE          0x0b
#define XONXOFF_ENABLE          0x0c
#define XONXOFF_DISABLE         0x0d

/*      Send/receive block function macros  */
#define ETB_TYPE                0x00
#define ETX_TYPE                0x01

/*      PTAP comm return values, error codes    */
#define GOOD_STATUS              0
#define NO_EOT                   2
#define DLE_EOT                  1
#define PARITY_ERROR            -1
#define TOO_MANY_CHARS          -2
#define DSR_LOST                -3
#define BCC_ERR                 -4
#define FIRST_CHAR              -5
#define NTH_CHAR                -6
#define BUS_ERR                 -7
#define OVERRUN_ERR             -8
#define FRAMING_ERR_PTAP        -9
#define TOO_MANY_RETRIES        -10

/*
 *  Default constants 202 pulse dialing, dial_mdelay, dial_bdelay
 */
#define SIXTY_MS_DIAL_DELAY     60
#define FORTY_MS_DIAL_DELAY     40

/*
**  Typedefs
*/

/* Configuration parameter defintion structure. */

typedef struct  cd_def
{
    char comm_type;     /* SPECIAL, BURST, PTAP, SNAP, INTERRUPT, IP40 */
    char mode;          /* ASCII, TRANSPARENT */
    char block_delay;   /* Delay between blocks */
    char mark_tone;     /* Frequency of mark tone */
    char space_tone;    /* Frequency of space tone */
    char baud_rate;     /* 300,600,1200,1350,2400,4800,9600,19200,76800 */
    char uart_control;  /* data bits,stop bits,parity, & idle tone bit map */
    int first_time;     /* Time will wait for the first character */
    int nth_time;       /* Time will wait between characters */
    char pad_char;      /* Block pad character */
    int block_size;     /* First byte of block size */
    char device_type;   /* EXTERNAL, INTERNAL, SPEAKER*/
    char option;        /* 202 and 212 options  */
} CD_LIT;

/* Datalink control character structure. */

typedef struct {
    unsigned char soh;
    unsigned char stx;
    unsigned char etx[2];
    unsigned char eot[2];
    unsigned char enq[2];
    unsigned char nak[2];
    unsigned char etb[2];
    unsigned char ack0[2];
    unsigned char ack1[2];
    unsigned char disc[2];
    unsigned char wack[2];
    unsigned char ttd[2];
    unsigned char end_blk_chars[2];
} DLCC;

#define _PTAPTDL pascal

/* PTAP function prototype  */
int _PTAPTDL dos_connect(CD_LIT *cd_ptr) ;
void _PTAPTDL dos_config(CD_LIT *cd_ptr) ;
void _PTAPTDL dos_hang_up(void) ;
void _PTAPTDL dos_disconnect(void) ;
unsigned int _PTAPTDL dos_comm_in(unsigned char signal) ;
void _PTAPTDL dos_comm_out(unsigned char signal) ;
int _PTAPTDL dos_bid(void) ;
int _PTAPTDL dos_wait_for_bid(void) ;
int _PTAPTDL dos_receive_block(unsigned char *recv_buff_ptr,
    unsigned int recv_buff_size, unsigned char mode, unsigned int *done,
    unsigned int *number_recvd_ptr);
int _PTAPTDL dos_send_block(unsigned char *send_buff_ptr,
    unsigned int send_len, unsigned char type, unsigned char mode );
int _PTAPTDL dos_wait_queue(void) ;
int _PTAPTDL dos_check_carrier_202(void) ;
int _PTAPTDL dos_answer_202(unsigned int pre_delay, unsigned int delay,
    unsigned int post_delay ) ;
int _PTAPTDL dos_dial_202(unsigned char *dial_ptr, unsigned int dial_len,
    char dial_mdelay, char dial_bdelay ) ;
void _PTAPTDL dos_read_dlcc(DLCC *ptr);
void _PTAPTDL dos_write_dlcc(DLCC *ptr);

