/***********************************************************************\
*                                                                       
*   DATE:  4/9/91           WRITTEN BY:  Telxon O/S                     
*                                                                       
*   FILE NAME:  tosap.h                                                 
*                                                                       
*   DESCRIPTION:                                                        
*       This header file contains the definitions of the various        
*       TOSAP constants structure definitions and routine definitions.  
*       Combined the following files: toslocal.h and tofns.h            
*       into one file called tosap.h                                    
*                                                                       
*   REVISIONS:                                  
*       1.4     10/01/92
*               Made the following changes:     
*               1.  Added TOSAP_APPEND_NOPCK.
*               2.  Added send_append_no_pck() declaration.
*               3.  Added send_no_pck() declaration.
*
*       1.7     06/14/93
*               Made the following changes:
*               1.  Updated function prototypes.
*               2.  Moved PACK macro from toslocal.h to here.
*               3.  Added TCAL_CREATE_TCAL and TCAL_REQUEST_TCAL.
*               4.  Renamed PACK, NO_PACK, and TCAL_PACK to be
*                   TOSAP_PACK_FLAG, TOSAP_NO_PACK_FLAG, and TOSAP_TCAL_
*                   PACK_FLAG respectively.  This was done to avoid
*                   conflicts with macros in telxon.h.
*               5.  Removed NO_UNPACK_MODE, UNPACK_MODE, PACK_MODE, and
*                   NO_PACK_MODE.
*
*       3.1     08/20/93    HH
*               Made the following changes:
*               1.  Added _TDL macro for pascal calling convention.
*               2.  Added _TDL to tosap functions.
*
*       3.1     08/20/93    FJL
*               Made the following changes:
*		1.  Renamed _TDL to _TOSAPTDL.
*		2.  Added _TOSAPTDL to all remaining function decl.
*
\************************************************************************/

#define TOSAP_APPEND_NOPCK      "00"
#define TOSAP_LOGON             "01"
#define TOSAP_LOGOFF            "02"
#define TOSAP_CREATE_NOPCK      "03"
#define TOSAP_CREATE_TCAL       "04"
#define TOSAP_REQUEST_TCAL      "07"
#define TOSAP_EXEC_FILE         "08"
#define TOSAP_REQUEST_STRIP     "09"
#define TOSAP_REQUEST_NO_STRIP  "10"
#define TOSAP_REQUEST_PACK      "11"
#define TOSAP_LINE_TURN_AROUND  "12"
#define TOSAP_DELETE_FILE       "14"
#define TOSAP_CHANGE_BAUD       "15"
#define TOSAP_GOOD_STATUS       "80"
#define TOSAP_BAD_STATUS        "81"
#define LOGON_FUNCTION          "00"
#define MAX_HEX_LINE_LEN         50

#define TOSAP_LOGON_CMD             1
#define TOSAP_LOGOFF_CMD            2
#define TOSAP_CREATE_NO_PACK_CMD    3
#define TOSAP_RCVD_CREATE_PACK_CMD  5
#define TOSAP_CALL_PRGM_CMD         6
#define TOSAP_RCVD_REQ_NO_STRIP_CMD 10
#define TOSAP_LINE_TURNAROUND_CMD   12
#define TOSAP_DELETE_FILE_CMD       14
#define TOSAP_RCVD_CHANGE_BAUD_CMD  15

/* TOSAP required defs for TOSAP LIBRARY */
#define TOSAP_BAUD_300          '5'
#define TOSAP_BAUD_600          '4'
#define TOSAP_BAUD_1200         '3'
#define TOSAP_BAUD_2400         '2'
#define TOSAP_BAUD_4800         '1'
#define TOSAP_BAUD_9600         '0'

#define TOSAP_EIGHT_BITS_CRC    '0'
#define TOSAP_EIGHT_BITS        '8'
#define TOSAP_SEVEN_BITS        '7'

#define TOSAP_NO_PACK_FLAG      0
#define TOSAP_PACK_FLAG         1
#define TOSAP_TCAL_PACK_FLAG    2

#define MAX_BLOCK_SIZE  1024
#define INFO_BFR_SIZE   16

#define ID_LEN          8
#define CMD_LEN         2
#define FILENAME_LEN    12
#define BAUD_POS        2
#define BITS_POS        3

#define STRIP           1
#define NO_STRIP        0

#define TCAL            1
#define NOT_TCAL        0

/* TOSAP ERROR CODES */
#define GOOD_STATUS       0             /* Good stat returned/everythingok*/
#define BAD_STATUS      -11             /* Bad Status returned from remote as
					   opposed to Good status which will
					   return GOOD_STATUS           */
#define BLOCK_SIZE_ERR  -12             /* Invalid block size requested */
#define BAD_ID_LEN      -13             /* ID length != 8 chars */
#define INV_STAT        -14             /* Inv status returned from remote */
#define DISK_OPEN_ERR   -15             /* Unable to find/open disk file   */
#define DISK_FATAL      -16             /* Unable to read/write to disk */
#define NOT_FOUND       -17             /* Can't find file to execute or del */
#define NOT_EXEC        -18             /* File is not executable. */
#define NO_MEM          -19             /* Insufficient mem to load child pgm*/
#define UNKNOWN         -20             /* Undefined error back from DOS */
#define BAD_CMD         -21             /* expected tosap command not recvd */
#define BAD_INFO        -22             /* Info string to be sent too long */
#define BAD_BAUD        -23             /* baud is invalid */
#define BAD_BITS        -24             /* bits are invalid */
#define INV_BLOCK       -25             /*invalid blk type recvd,etx <-> etb*/


#define _TOSAPTDL pascal

#pragma pack (1)
int _TOSAPTDL send_logon(char *block, int block_size, char *id, char *info);
int _TOSAPTDL send_logoff(char *block, int block_size, char *id);
int _TOSAPTDL send_create_no_pck(char *block, int block_size, char *name,
		char *info);
int _TOSAPTDL send_append_no_pck(char *block, int block_size, char *name,
		char *info);
int _TOSAPTDL send_no_pck(char *block, int block_size, char *name, char *info,
		char *cmd);
int _TOSAPTDL send_request_strip(char *block, int block_size, char *name,
		char *info);
int _TOSAPTDL send_request_no_strip(char *block, int block_size, char *name,
		char *info);
int _TOSAPTDL request_file(char *block, int block_size, char *name, char *info,
		int tosap_cmd);
int _TOSAPTDL get_status(char *block, int block_size, char *info);
int _TOSAPTDL send_delete_file(char *block, int block_size, char *name,
		char *info);
int _TOSAPTDL send_line_turn_around(char *block, int block_size, char *info);
int _TOSAPTDL send_request_exec(char *block, int block_size, char *name,
		char *info);
int _TOSAPTDL send_change_baud(char *block, int block_size, char baud,
		char bits, char *info);
int _TOSAPTDL send_request_tcal(char *block, int block_size, char *name,
		char *info);
int _TOSAPTDL send_request_pack(char *block, int block_size, char *name,
		char *info);
int _TOSAPTDL good_status(char *block, int block_size, char *info);
int _TOSAPTDL bad_status(char *block, int block_size, char *info);
int _TOSAPTDL rcvd_logon(char *block, int block_size, char *id);
int _TOSAPTDL rcvd_create_no_pck(char *block, int block_size);
int _TOSAPTDL rcvd_create_tcal(char *block, int block_size);
int _TOSAPTDL rcvd_create_pck(char *block, int block_size);
int _TOSAPTDL rcvd_create_file(char *block, int block_size, char create_flag);
int _TOSAPTDL rcvd_call_pgm(char *block, int block_size);
int _TOSAPTDL rcvd_delete_file(char *block, int block_size);
int _TOSAPTDL rcvd_change_baud(char *block, int *block_size, char *baud,
		char *bits);
int _TOSAPTDL rcvd_request_no_strip(char *block, int block_size, char *info);

