/************************************************************************\
*                                                                        *
*   File:   srfxlib.h                                                    *
*                                                                        *
*   Purpose:                                                             *
*       Extended common header file containing macros and typedefs used  *
*       SRF "C" library interface.                                       *
*                                                                        *
*   Revisions:                                                           *
*       0.1    11/13/90     FJL                                          *
*               File created.                                            *
*                                                                        *
*       0.1     12/07/90        F. Lautenbach                            *
*               Removed errors RF_TX_ENABLE_CMD_ERR and RF_TX_ABORT_CMD_ *
*               ERR.                                                     *
*                                                                        *
*       0.1     01/14/91        F. Lautenbach                            *
*               Removed XP_NO_RECV_DATA_AVAIL error.  Changed declara-   *
*               tion of crfchkrecv() to accept a ptr to a u_int.         *
*                                                                        *
*       0.1     02/26/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1. Added APP_SET_CONT_FUN and APP_ABORT_CONT_FUN.        *
*               2. Removed xp_send_mode from CFG_STRUCT.                 *
*               3. Replaced declaration of srfsend() with srfsendwa()    *
*                  and srfsenddg().                                      *
*               4. Replace APP_SEND_FUN with APP_SEND_SWA_FUN and        *
*                  APP_SEND_DGS_FUN.                                     *
*                                                                        *
*       0.1     02/27/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1. Renamed CFG_STRUCT element xp_tmout to be xp_bg_tmout.*
*                                                                        *
*       0.1     03/05/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1. Renamed macro SRF_OK to APP_OK in order to be consis- *
*                  tent with Cellular TSR.                               *
*                                                                        *
*       0.1     03/08/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Config param cfg.rf_spread_id renamed to be          *
*                   cfg.rf_channel.                                      *
*                                                                        *
*       0.2     04/09/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Added macros DEF_XPRX_HOOK_VEC and DEF_TIMER_HOOK_   *
*                   VEC.                                                 *
*                                                                        *
*       0.2     04/23/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1. Added LAST_XP_ERR.                                    *
*                                                                        *
*       0.3     05/01/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Updated APP_STATS_REC to conform to new statistics.  *
*               2.  Added RF_TX_TMOUT_ERR.                               *
*               3.  Added DINF_T410_TIMER_ER and DINF_ERR_END.           *
*               4.  Added the following elements to CFG_STRUCT:          *
*                   rf_fast_poll_period, rf_poll_decay, and rf_recv_mode.*
*               5.  Renamed the following elements in CFG_STRUCT:        *
*                   rf_poll_rate -> rf_slow_poll_period                  *
*                   rf_refresh_rate -> rf_refresh_period                 *
*                                                                        *
*       0.3     05/09/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Added APP_SHRAM_DUMP_FUN.                            *
*               2.  Added declaration of srfshramdump().                 *
*                                                                        *
*       0.3     05/16/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Changed default application block size macro,        *
*                   DEF_APP_BLK_SIZE, from 1024 to 2000.                 *
*                                                                        *
*       0.3     05/18/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Replaced rf_bcst_addr in CFG_STRUCT with new element *
*                   rf_router_addr.                                      *
*               2.  Changed comment on rf_unit_addr and rf_dest_addr in  *
*                   CFG_STRUCT to indicate they are not read-only.       *
*                                                                        *
*       1.1     06/11/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Added new error, RF_TX_NAK_ERR.                      *
*                                                                        *
*       1.1     06/18/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Updated APP_STATS struct.                            *
*               2.  Removed cfg.rf_recv_mode.                            *
*               3.  Removed cfg.rf_tx_atten.                             *
*               4.  Added cfg.rf_fast_poll_delay.                        *
*               5.  Added cfg.rf_force_slow_poll.                        *
*                                                                        *
*       1.1     06/20/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Added RF_TX_INV_LAN_ADDR_ERR and RF_TX_UNDEF_REG_    *
*                   STAT_ERR.                                            *
*                                                                        *
*       1.1     06/27/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Changed DEF_XP_NUM_PTCS to be 254.                   *
*               2.  Changed DEF_XP_NUM_HOSTS to be 16.                   *
*                                                                        *
*       1.2     07/23/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Added xp_src_id to CFG structure.  This is a read-   *
*                   only parameter which is updated each time a receive  *
*                   function is done and contains the transport address  *
*                   of the source of the received message.               *
*               2.  Renamed APP_GET_CFG_FUN to be APP_READ_CFG_FUN.      *
*               3.  Added APP_WRITE_CFG_FUN.                             *
*               4.  Renamed proc declaration for srfgetcfg() to be       *
*                   srfreadcfg().                                        *
*               5.  Added proc declaration for srfwritecfg().            *
*                                                                        *
*       1.3     08/19/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Added rf_ehl_threshold and rf_ehl_decay to configu-  *
*                   ration structure, CFG_STRUCT.                        *
*               2.  Moved CFG_STRUCT element xp_src_id to reside with    *
*                   rest of transport structure elements.                *
*                                                                        *
*       1.3     08/22/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Added DEF_POST_XPRX_HOOK_VEC.                        *
*                                                                        *
*       1.3     08/27/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Modifed CFG_STRUCT for new interface to channel set  *
*                   feature.                                             *
*               2.  Created new structure typedef CFG_READ_ONLY which    *
*                   contains read only parameters which area valid after *
*                   a config, send, etc.                                 *
*               3.  CFG_READ_ONLY structure now contains xp_src_id in    *
*                   addition to 2 new elements, xp_num_retries and       *
*                   rf_num_retries.                                      *
*                                                                        *
*       1.3     09/15/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Added APP_RF_TSR_TYPE_FUN, SPREAD_RF_TSR_TYPE, and   *
*                   CELLULAR_RF_TSR_TYPE.                                *
*                                                                        *
*       1.3     10/25/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Updated logic with various changes to bring up to    *
*                   same level as ver 1.7 of the TCAL overlay.           *
*                                                                        *
*       1.5     11/18/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Renamed this file to be srfxlib.h vs srflib.h.       *
*               2.  Renamed config param "reserve1" to "rf_err_reset."   *
*                                                                        *
*       1.5     11/20/91        F. Lautenbach                            *
*               Made the following changes:                              *
*               1.  Renamed cfg param xp_bg_tmout to be xp_tmout.        *
*                                                                        *
*       1.6     01/14/92                                                 *
*               Made the following changes:                              *
*               1.  Added macros APP_RESET_RX_QUEUE_FUN and APP_FORCE_   *
*                   SLOW_POLL_FUN.                                       *
*               2.  Added procedure declarations srfresrecv() and        *
*                   srfslowpoll().                                       *
*                                                                        *
*       1.9     02/28/92                                                 *
*               Made the following changes:                              *
*               1.  Added APP_RF_KBHIT_FUN.                              *
*               2.  Added srfkbhit() function declaration.               *
*               3.  Added macros used by srfkbhit() function             *
*                                                                        *
*       1.9     03/03/92                                                 *
*               Made the following changes:                              *
*               1.  Added APP_GET_COP_VER_FUN.                           *
*               2.  Added srfcopver() declaration.                       *
*               3.  Added APP_GET_TSR_VER_FUN.                           *
*               4.  Added srftsrver() declaration.                       *
*                                                                        *
*       1.9     03/04/92                                                 *
*               Made the following changes:                              *
*               1.  Renamed function macros to begin with SRF_ vs APP_.  *
*                   This was done in order to obtain some consistency    *
*                   with documentation.                                  *
*                                                                        *
*       2.6     01/29/93                                                 *
*               Made the following changes:                              *
*               1.  Added XP_RAW_MODE_ERR.                               *
*               2.  Added SRF_SET_XP_RAW_FUNC and SRF_ABORT_XP_RAW_FUNC. *
*                                                                        *
*       2.6     02/01/93                                                 *
*               Made the following changes:                              *
*               1.  Added proc declarations for srfsetxpraw() and        *
*                   srfabortxpraw().                                     *
*               2.  Changed declarations of following functions from     *
*                   "void" to "unsigned char" - srfinit, srfdisconnect,  *
*                   srfreadcfg, srfwritecfg, srfcopver, and srftsrver.   *
*                                                                        *
*       2.6     02/08/93                                                 *
*               Made the following changes:                              *
*               1.  Added SRF_SET_RF_CLEAR_FUNC and SRF_ABORT_RF_CLEAR_  *
*                   FUNC.                                                *
*               2.  Added procedure declarations for srfsetrfclear() and *
*                   srfabortrfclear().                                   *
*                                                                        *
*       2.6     03/01/93                                                 *
*               Made the following changes:                              *
*               1.  Added XP_PING_ERR and XP_INV_SEND_MODE_ERR.          *
*               2.  Added SRF_SEND_PING_FUNC.                            *
*               3.  Added declaration for srfsendping().                 *
*                                                                        *
*       3.1     08/18/93                                                 *
*               Made the following changes:                              *
*               1.  Added error code XP_RETRY_NOT_ALLOWED.               *
*               2.  Added declaration for srfretry().                    *
*               3.  Added SRF_RETRY_FUNC.                                *
*                                                                        *
*       3.1     08/26/93                                                 *
*               Made the following changes:                              *
*               1.  Removed conditional compiles around app retry logic. *
*                                                                        *
\************************************************************************/

/***************************************************************************\
*                                                                           *
*       Macro definitions.                                                  *
*                                                                           *
\***************************************************************************/

/* Default command line parameter values */
#define DEF_APP_BLK_SIZE        2000
#define DEF_APP_RX_QDEPTH       1
#define DEF_XP_NUM_PTCS         254
#define DEF_XP_NUM_HOSTS        16

/* TSR application software interrupt functions */
#define SRF_INIT_FUNC                   0x00
#define SRF_CONNECT_FUNC                0x01
#define SRF_READ_CFG_FUNC               0x02
#define SRF_DISC_FUNC                   0x03
#define SRF_TRANSACT_FUNC               0x04
#define SRF_SEND_SWA_FUNC               0x05
#define SRF_RECEIVE_FUNC                0x06
#define SRF_SET_CONT_FUNC               0x07
#define SRF_ABORT_CONT_FUNC             0x08
#define SRF_DIAGNOSTICS_FUNC            0x0c
#define SRF_STATISTICS_FUNC             0x0d
#define SRF_STATUS_SCREEN_FUNC          0x0f
#define SRF_CHECK_RECV_FUNC             0x10
#define SRF_SEND_DGS_FUNC               0x11
#define SRF_SHRAM_DUMP_FUNC             0x12
#define SRF_WRITE_CFG_FUNC              0x13
#define SRF_RESET_RX_QUEUE_FUNC         0x14
#define SRF_FORCE_SLOW_POLL_FUNC        0x15
#define SRF_KBHIT_FUNC                  0x16
#define SRF_SET_XP_RAW_FUNC             0x17
#define SRF_ABORT_XP_RAW_FUNC           0x18
#define SRF_SET_RF_CLEAR_FUNC           0x19
#define SRF_ABORT_RF_CLEAR_FUNC         0x1a
#define SRF_SEND_PING_FUNC              0x1b
#define SRF_RETRY_FUNC                  0x1c
#define SRF_TEST_INSTALL_FUNC           0x80
#define SRF_TSR_TYPE_FUNC               0x81
#define SRF_GET_COP_VER_FUNC            0x82
#define SRF_GET_TSR_VER_FUNC            0x83

/* Macros defining types of RF TSRs */
#define SPREAD_RF_TSR_TYPE              1
#define CELLULAR_RF_TSR_TYPE            2

/*  Return codes from the TSR installation to DOS */
#define TI_GOOD_STATUS      0   /* Indicates successful installation */
#define TI_CMD_LINE_ERR     1   /* Error in parsing command line */
#define TI_ALREADY_INST_ERR 2   /* TSR already installed */
#define TI_MEM_ALLOC_ERR    3   /* Error in mem allocation functions */
#define TI_AT_CLASS_ERR     4   /* PC ver of TSR needs AT class PC */

/*  Good status return code */
#define APP_OK                      0

/*  Transport layer status/error codes */
#define XP_OK                       0x00
#define XP_DEST_ERR                 0x01    
#define XP_NET_ERR                  0x02
#define XP_INVD_ERR                 0x03
#define XP_RESET_ERR                0x04
#define XP_TMOUT                    0x05
#define XP_SEND_BFR_OFLOW           0x06
#define XP_RECV_BFR_OFLOW           0x07
#define XP_UNIT_ID_ERR              0x08
#define XP_DEST_ID_ERR              0x09
#define XP_INIT_ERR                 0x0a
#define XP_IOB_ERR                  0x0b
#define XP_NOT_CONNECTED_ERR        0x0c
#define XP_RAW_MODE_ERR             0x0d
#define XP_PING_ERR                 0x0e
#define XP_INV_SEND_MODE_ERR        0x0f
#define XP_RETRY_NOT_ALLOWED        0x10
#define LAST_XP_ERR                 0x3f

/*  RF status code */
#define RF_OK                       0
#define RF_RECV_MSG_AVAIL           0x41
#define RF_RECV_MSG_NOT_AVAIL       0x42
#define RF_TX_RETRY_ERR             0x43
#define RF_TX_ABORTED_ERR           0x44
#define RF_TX_UNKNOWN_ERR           0x45
#define RF_TMOUT_ERR                0x46
#define RF_TX_NOT_REG_ERR           0x47
#define RF_TX_TMOUT_ERR             0x48
#define RF_TX_NAK_ERR               0x49
#define RF_TX_INV_LAN_ADDR_ERR      0x4a
#define RF_TX_UNDEF_REG_STAT_ERR    0x4b

/*  Coprocessor command status codes */
#define CMD_OK                      0
#define CMD_TMOUT_ERR               0x81
#define CMD_BUSY_ERR                0x82

/*  Coprocessor IO control status codes */
#define CTL_OK                      0 
#define CTL_WARM_START_TMOUT_ERR    0xc1
#define CTL_COLD_START_TMOUT_ERR    0xc2

/*  Diagnostic status codes */
#define DIAG_OK                             0
#define DIAG_NOP_INT_ERR                    0xd1
#define DIAG_NOP_NOINT_ERR                  0xd2
#define DIAG_RESET_FLAG_ERR                 0xd3
#define DIAG_COP_ACTIVATE_ERR               0xd4
#define DIAG_SHARED_RAM_ERR                 0xd5
#define DIAG_UNDEFINED_DIAG_INFO_ERR        0xd6

/*  Coprocessor's header signature diagnostic info byte values */
#define DINF_OK                             0xff
#define DINF_EPROM_CHECKSUM_ERR             0xfe
#define DINF_LOCAL_RAM_TEST_ERR             0xfd
#define DINF_SCC_TEST_ERR                   0xfc
/* 0xfb -> 0xf9 are reserved */
#define DINF_SYS_PARAM_CHECKSUM_ERR         0xf8
#define DINF_SSCODE_HEADER_ERR              0xf7
#define DINF_CONFIG_PARAM_ERR               0xf6
/* 0xf5 reserved */
#define DINF_SPREADING_CODE_CHKSUM_ERR      0xf4
#define DINF_LOAD_CODE_ERR                  0xf3
/* 0xf2 reserved */
#define DINF_BAD_COMMAND_ERR                0xf1
#define DINF_TXENABLE_PARAM_ERR             0xf0
#define DINF_ONCHIP_TIMER_ERR               0xef
#define DINF_T410_TIMER_ERR                 0xee

#define DINF_ERR_END                        0xeb

/*  Other TSR function return codes */
#define BAD_TSR_FUNCTION            0xff    /* Undefined TSR function called */

/*  Macros used by srfkbhit() lib function */
#define RF_DATA                 0
#define KB_DATA                 1
#define NO_DATA                 2
#define WAND_DATA               3
#define _SRFTDL pascal

/***************************************************************************\
*                                                                           *
*       Typedefs                                                            *
*                                                                           *
\***************************************************************************/

/*  Transport retry prompt definition. */
typedef struct {
    unsigned char line_1[16];
    unsigned char line_2[16];
    unsigned char retry_k;
    unsigned char abort_k;
} XP_RETRY_PROMPT;

/*  Status screen definition */
typedef struct {
    unsigned char xps[4][8];
    unsigned char rfs[2][8];
} STATUS_SCREEN;

/*  Read only config parameters */
typedef struct {
    unsigned int xp_src_id;             /* Transport source id */
    unsigned int xp_num_retries;        /* Number xp retries on a send */    
    unsigned int rf_num_retries;        /* Number of rf retries on a send */
} CFG_READ_ONLY;

/* Configuration structure */
typedef struct {
    unsigned char far *app_hdr;         /* Application message header */
    unsigned int far *app_hdr_len;      /* Ptr to length of app msg header */
    unsigned int xp_unit_id;            /* Transport unit ID */
    unsigned int xp_dest_id;            /* Transport dest ID */
    unsigned int xp_tmout;              /* Transport timemout */
    XP_RETRY_PROMPT far *xp_prompt_ptr; /* Ptr to xport prompt definition */
    unsigned int xp_prompt_enable;      /* Enable/Disable xport retry prompt */
    unsigned int xp_prompt_page;        /* Transport retry prompt video page */
    unsigned int xp_max_retries;        /* Max auto XP retries */
    STATUS_SCREEN far *stat_screen_ptr; /* Ptr to status screen text */
    unsigned int stat_scr_enable;       /* Enable/Disable status screen */
    unsigned int ss_page;               /* Status screen video page */
    unsigned long rf_sys_id;            /* System identifier */
    unsigned int rf_max_dgram_size;     /* Max size of a datagram */
    unsigned int rf_max_retries;        /* Max retries on RF LAN */
    unsigned long rf_spread_id;         /* Spread Spectrum code id */
    unsigned int rf_rx_bdcst;           /* Receipt of broadcasts allowed */
    unsigned char far *rf_sscode_ptr;   /* Ptr to spreading code */
    unsigned int rf_net_mode;           /* Peer-peer vs TMA mode */
    unsigned int rf_fast_poll_period;   /* Backoff fast poll period */
    unsigned int rf_slow_poll_period;   /* Backoff slow poll period */
    unsigned int rf_fast_poll_delay;    /* Fast poll delay time */
    unsigned int rf_force_slow_poll;    /* Force slow poll after transact */
    unsigned int rf_refresh_period;     /* Refresh reg period (10's of ms) */
    unsigned int rf_poll_decay;         /* Backoff poll decay parameter */
    unsigned int rf_spread_fmt;         /* Spreading code format */
    unsigned int rf_unit_id;            /* RF unit id parameter */
    unsigned int rf_dest_id;            /* RF dest id parameter */
    unsigned int rf_send_mode;          /* RF send mode (bdcst or with ack) */
    unsigned int rf_wire_mode;          /* Wire test mode enable */
    unsigned char rf_unit_addr[6];      /* RF unit address override */
    unsigned char rf_dest_addr[6];      /* RF dest address override */
    unsigned char rf_router_addr[6];    /* RF router address override */
    unsigned int rf_ehl_threshold;      /* RF early handoff threshold */
    unsigned int rf_ehl_decay;          /* RF early handoff decay */
    unsigned int rf_spread_pgm_mode;    /* RF spread code pgm mode */
    unsigned int rf_num_channels;       /* RF num of channels in channel set */
    unsigned int rf_channel;            /* RF primary channel */
    unsigned int rf_ch2;                /* RF second channel in channel set */
    unsigned int rf_ch3;                /* RF third channel in channel set */
    unsigned int rf_ch4;                /* RF fourth channel in channel set */
    unsigned int rf_err_reset;          /* RF reset on internal errors */
    CFG_READ_ONLY read_params;          /* READ-ONLY params */
} CFG_STRUCT;

/* Statistics structure setup by statistics function */
typedef struct {
    unsigned long tx_dgrams;    /* # datagrams transmitted since last reset */
    unsigned long rexmits;          /* # re-xmits since last reset */
    unsigned long rx_dgrams;    /* # datagrams received since last reset */
    unsigned long dup_rx_frames;    /* # dup recvd frames since last reset */
    unsigned long crc_errors;       /* # crc errors since last reset */
    unsigned long length_errors;    /* # length errors since last reset */
    unsigned long abort_errors;     /* # abort errors since last reset */
    unsigned long hold_offs;        /* # hold offs since last reset */
    unsigned long tx_frames;        /* # frames transmitted since last reset */
    unsigned long rx_frames;        /* # frames received since last reset */
    unsigned long frames_discarded; /* # frames discarded since last reset */
} APP_STATS;

/***************************************************************************\
*                                                                           *
*       Procedure declarations.                                             *
*                                                                           *
\***************************************************************************/

void _SRFTDL srfsetvec(unsigned int vector);
unsigned char _SRFTDL srfinit(CFG_STRUCT *cfg_ptr);
unsigned char _SRFTDL srfreadcfg(CFG_STRUCT *cfg_ptr);
unsigned char _SRFTDL srfwritecfg(CFG_STRUCT *cfg_ptr);
unsigned int _SRFTDL srftestvec(void);
unsigned char _SRFTDL srfconnect(CFG_STRUCT *cfg_ptr);
unsigned char _SRFTDL srfdisconnect(void);
unsigned char _SRFTDL srftransact(unsigned char *send_ptr, unsigned int send_len,
    unsigned char *recv_ptr, unsigned int *recv_len, unsigned int tmout);
unsigned char _SRFTDL srfrecv(unsigned char *recv_ptr, unsigned int *recv_len,
    unsigned int tmout);
unsigned char _SRFTDL srfsendswa(unsigned char *send_ptr, unsigned int send_len,
    unsigned int tmout);
unsigned char _SRFTDL srfsenddgs(unsigned char *send_ptr, unsigned int send_len,
    unsigned int tmout);
unsigned char _SRFTDL srfsendping(unsigned char *send_ptr, unsigned int send_len,
    unsigned int tmout);
unsigned char _SRFTDL srfretry(unsigned int *len);
unsigned char _SRFTDL srfsetcont(void);
unsigned char _SRFTDL srfabortcont(void);
unsigned char _SRFTDL srfsetxpraw(void);
unsigned char _SRFTDL srfabortxpraw(void);
unsigned char _SRFTDL srfsetrfclear(void);
unsigned char _SRFTDL srfabortrfclear(void);
unsigned char _SRFTDL srfdiags(void);
unsigned char _SRFTDL srfstatistics(APP_STATS *stats_ptr);
unsigned char _SRFTDL srfstatscr(void);
unsigned char _SRFTDL srfchkrecv(unsigned int *num);
unsigned char _SRFTDL srfshramdump(unsigned int shram_offset,
    unsigned int *shram_len_ptr, unsigned char *dest_ptr,
    unsigned int dest_len);
unsigned int _SRFTDL srftestvec(void);
unsigned char _SRFTDL srftsrtype(void);
unsigned char _SRFTDL srfresrecv(void);
unsigned char _SRFTDL srfslowpoll(void);
unsigned int _SRFTDL srfkbhit(void);
unsigned char _SRFTDL srfcopver(char *ver);
unsigned char _SRFTDL srftsrver(char *ver);
