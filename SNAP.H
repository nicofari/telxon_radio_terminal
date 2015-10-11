/***********************************************************************\
*                                                                       
*   DATE:  06/15/93           WRITTEN BY:  Telxon O/S                     
*                                                                       
*   FILE NAME:  snap.h                                                 
*                                                                       
*   DESCRIPTION:                                                        
*       SNAP library header file.
*                                                                       
*   REVISIONS:                                  
*       1.4     06/15/93
*               Made the following changes:     
*               1.  Added this file comment block.
*               2.  Renamed function prototype for sleep() to be
*                   _ptc_sleep().
*               3.  Added function prototypes for receive_commands() and
*                   pwrup_status().
*
*       3.1     07/15/93        HH
*               Made the following changes:
*               1.  Added _TDL macro for pascal calling convention.
*               2.  Added _TDL to snap functions.
*
*       3.1     08/20/93    FJL
*               Made the following changes:     
*		1.  Renamed _TDL to be _SNAPTDL.
*		2.  Added _SNAPTDL declaration to all procedures.
*
\************************************************************************/

/*  Constants -- SNAP communications    */
#define SNAP_OK     0       /* Good status returned from SNAP function      */
#define NOT_POLL_ID 98      /* Not this units's Poll I.D.                   */
#define POWER_ON    99      /* The on/off key powered the PTC up            */
#define ERR         0xffff  /* Error from wake-up                           */
#define BLK_SIZE    1024    /* Size of Send/Receive buffer                  */
#define SECOND_2    2       /* Two Second delay                             */
#define SEC_CNV     50      /* Converts number of 20ms delays to seconds    */

/* Constants -- System Services */
#define PWRUP_NOR   0       /* Normal power up                              */
#define PWRUP_ALARM 1       /* Power up by alarm                            */
#define PWRUP_RI    2       /* Power up by external wake up (RI)            */

#define SYS_VEC     0x15    /* System Services                              */
#define DELAY       0xf6    /* Delay interrupt call                         */
#define PWRUPSTATUS 0xe6    /* Power up status interrupt call               */
#define POWEROFF    0xe2    /* Power off function call                      */
#define FORTY_MILLS 50      /* Nth character timeout for SNAP               */

#define DUMB_MODE   0x00 
#define LOW         0x00
#define DONE        0x01
#define NOT_DONE    0x00
#define BCC            -4
#define FRAMING_ERROR  -9

#define SEC_10      2500    /* First character timeout for SNAP             */

#define BLK_512     512     /* 512 byte block size  */
#define BLK_1024    1024    /* 1k byte block size   */
#define BLK_2048    2048    /* 2k byte block size   */
#define DSR_SIGNAL  0

/* Set date/time function constants */
#define DOS_VEC 0x21
#define SET_DATE 0x2B
#define SET_TIME 0x2D

#define _SNAPTDL pascal

/*  SNAP communications protocol procedure definitions  */
int _SNAPTDL snap_connect( int comm_mode, int baud, char *poll_id,
		int blk_size ) ;
int _SNAPTDL start_tosap( char *poll_id ) ;
int _SNAPTDL transparent_mode( int tosap_baud ) ;
int _SNAPTDL master_send( char *filename ) ; 
int _SNAPTDL master_receive( char *filename ) ;
int _SNAPTDL end_tosap( char *poll_id ) ;
int _SNAPTDL slave_mode( void ) ;
int _SNAPTDL slave_exec_mode( char *filename ) ;
int _SNAPTDL receive_commands( void ) ;
char * _SNAPTDL comm_error( int status ) ;
void _SNAPTDL power_off( void ) ;
int _SNAPTDL pwrup_status( void ) ;
void _SNAPTDL _ptc_sleep ( int seconds ) ;
int _SNAPTDL set_date_time( char far *str ) ;
unsigned int _SNAPTDL ascii_to_numeric( unsigned char far *str, int base,
			unsigned int size ) ;
