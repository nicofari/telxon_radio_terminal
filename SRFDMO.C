/*************************************************************************\
*                                                                         *
*   File:   srfdmo.c                                                      *
*                                                                         *
*   Purpose:                                                              *
*       The purpose of this program is to test the MSDOS interface to the *
*       Spread Spectrum RF TSR.  It provides a menu of options            *
*       allowing the user to configure the TSR and test each of           *
*       the available functions.  The program uses the Microsoft C        *
*       library interface to the TSR in order to simplify the TSR         *
*       function calls. (Note that each of the library calls in           *
*       this program could be replaced with the associated TSR call.)     *
*                                                                         *
*       Note:  To make srfdmo.exe, this file should be compiled using     *
*       the -AS (small model) and -Zp (pack structure on byte boundary)   *
*       options.  It must also be linked to the SRF small model library   *
*       srfs.lib.                                                         *
*                                                                         *
*   REVISIONS:                                                            *
*       1.5     11/18/91                                                  *
*               File created for use with ver 1.5 of Spread Spectrum TSR  *
*               and C library interface.                                  *
*                                                                         *
*       1.6     01/13/92                                                  *
*               Bumped version displayed to 1.6.                          *
*                                                                         *
*       1.7     01/13/92                                                  *
*               Bumped version displayed to 1.7.                          *
*                                                                         *
*       1.8     02/17/92                                                  *
*               Bumped version displayed to 1.8.                          *
*                                                                         *
*       1.9     02/26/92                                                  *
*               Bumped version displayed to 1.9.                          *
*                                                                         *
*       2.1     03/23/92                                                  *
*               Bumped version displayed to 2.1.                          *
*                                                                         *
*       2.2     04/09/92                                                  *
*               Bumped version displayed to 2.2.                          *
*                                                                         *
*       2.3     05/19/92                                                  *
*               Bumped version displayed to 2.3.                          *
*                                                                         *
*       2.4     07/13/92                                                  *
*               Bumped version displayed to 2.4.                          *
*                                                                         *
*       2.5     09/24/92                                                  *
*               Bumped version displayed to 2.5.                          *
*                                                                         *
*       2.6     09/28/92                                                  *
*               Bumped version displayed to 2.6.                          *
*                                                                         *
*       3.1     08/13/93                                                  *
*               Bumped version displayed to 3.1.                          *
*                                                                         *
\*************************************************************************/

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "srflib.h"

/* A few useful macros */
#define TRUE    1
#define FALSE   0
#define OK      1
#define ERROR   0
#define BUF_SIZE        100     /* size of utility buffers */
#define VID_VEC         0x10
#define SCROLL_UP_FUNC  0x06
#define NORMAL_VID_MODE 0x07
#define GET_VID_STATE   0x0f
#define SET_CURS_FUNC   0x02

/* Data */
CFG_STRUCT cfg;                 /* configuration structure */
unsigned char send_buf[2048] = {"HELLO WORLD!"}; /* RF send buffer */
unsigned int send_len;          /* Length of send buffer */
unsigned char recv_buf[2048];   /* RF receive buffer */
unsigned int recv_len;          /* Length of recv */
unsigned int usr_tmout;         /* PTC user timeout */

/*  Main menu screens */
char *main_menu_option[] = {
/*0  123456789ABCDEF0 */
    "DOS SRFDMO V3.1",
    "-Select option-",
    "ENTER to review",
/*1  123456789ABCDEF0 */
    "0. EXIT TO DOS",
    "1. SETUP",
    "2. INIT",
/*2  123456789ABCDEF0 */
    "3. CONNECT",
    "4. DISCONNECT",
    "5. TRANSACT",
/*3  123456789ABCDEF0 */
    "6. SEND-SWA",
    "7. SEND-DGS",
    "8. RECEIVE",
/*4  123456789ABCDEF0 */
    "9. CHK RX QUEUE",
    "A. READ CONFIG",
    "B. WRITE CONFIG",
/*5  123456789ABCDEF0 */
    "C. SET CONT",
    "D. ABORT CONT",
    "E. DIAGNOSTICS",
/*6  123456789ABCDEF0 */
    "F. TIMEOUT ENBL",
    "G. TIMEOUT DSBL",
    "               "
};

/*  Setup menu screens */
char *setup_menu_option[] = {
/*0  123456789ABCDEF0 */
    "0. EXIT SETUP",
    "1. APP TX HDR",
    "2. APP TX DATA",
/*1  123456789ABCDEF0 */
    "3. XP UNIT ID",
    "4. XP DEST ID",     
    "5. XP TIMEOUT",
/*2  123456789ABCDEF0 */
    "6. XP PRMT ENBL",
    "7. XP PRMT PAGE",
    "8. XP RETRIES",
/*3  123456789ABCDEF0 */
    "9. RF SYSTEM ID",
    "A. RF NUM CHAN",
    "B. RF CHANNEL",
/*4  123456789ABCDEF0 */
    "C. RF CH #2",
    "D. RF CH #3",
    "E. RF CH #4",
/*5  123456789ABCDEF0 */
    "F. RF NET MODE",
    "G. RF FAST POLL",
    "H. RF SLOW POLL",
/*6  123456789ABCDEF0 */
    "I. RF POLL DECAY",
    "J. RF FAST DELAY",
    "K. RF FORCE SLOW",
/*7  123456789ABCDEF0 */
    "L. RF DEST ID",
    "M. RF DGRAM SIZE",
    "N. RF RETRIES",
/*8  123456789ABCDEF0 */
    "O. RF REF PERIOD",
    "P. RF EHL THRSH",
    "Q. RF EHL DECAY"
};

unsigned char app_hdr_buf[100];     /* App message header buffer */
unsigned char tmp_buf[100];
unsigned int app_hdr_len;           /* Length of message header */
unsigned int interface_vec;         /* TSR's application interface vector */

/* Local procedure declarations. */
int getval(char *prompt, char *format, unsigned long *addr);
void action(void);
void clear_screen(void);
void setup(void);
void read_config(void);
void write_config(void);
void init(void);
void connect(void);
void disconnect(void);
void transact(void);
void send_swa(void);
void send_dgs(void);
void recv(void);
void setcont(void);
void abortcont(void);
void diagnostics(void);
void chkrecv(void);
void setup_menu(void);
unsigned int get_tmout(void);
void enable_tmout(unsigned int tmout);
void disable_tmout(void);
void setup_app_tx_data(void);
unsigned int atox(unsigned char *ptr);

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME: main                                                 *
*                                                                      *
* DESCRIPTION:                                                         *
*   This is the main entry point of the program.                       *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

main()
{
    int done = FALSE, cmd, i, cont, ch;
    unsigned int install_flag;
    unsigned int far *install_flag_ptr;
    int status, val;

#define DEF_INTERFACE_VEC 0x10  // Boh?!?!
    
#if defined(DEBUG)
/*  Print load segment:offset of this module */
    clear_screen();
    printf("main: %p\n", (unsigned char far*)main);
    action();
#endif

/*  Prompt user for TSR functions vector */
/*    clear_screen();
    interface_vec = DEF_INTERFACE_VEC;
    printf("App vector: %xh\n",interface_vec);
    status = getval("Enter(HEX):\n", "%x", &val);
    if (status == 1) {
  interface_vec = val;
    }
*/

/*  Configure library interface for selected TSR functions vector */
/*    srfsetvec(interface_vec); */

/*  Test if TSR is installed */
    if(srftestvec() == FALSE)
    {
  clear_screen();
  printf("TSR is not\n");
  printf("installed!\n");
  return;
    }

/*  Save the current application timeout value. */
    usr_tmout = get_tmout();
    
/*  Initialize TSR */
    init();

/*  Init send buffer variables to default */
    send_len = strlen(send_buf);

/*  Main menu driver */
    done = FALSE;
    i = 0;
    while(done != TRUE)
    {
  if(i > 6)
      i = 0;
  if(i < 0)
      i = 6;
  clear_screen();
  printf("%s\n", main_menu_option[(3*i)]);
  printf("%s\n", main_menu_option[(3*i)+1]);
  printf("%s\n", main_menu_option[(3*i)+2]);
  printf("Main menu => ");
  ch = getch();
  switch(ch)
  {
      case 0x0d:
    i++;
    break;
      case ' ':
    i--;
    break;
      case '0':
    clear_screen();
    printf("Are you sure\nyou want to\nexit to DOS?\n[y/n] ");
    ch = getch();
    switch(ch)
    {
        case 'y':
        case 'Y':
      done = TRUE;
      enable_tmout(usr_tmout);
      clear_screen();
      break;
        default:
      break;
    }
    break;
      case '1':
    clear_screen();
    setup();
    break;
      case '2':
    clear_screen();
    printf("Init will be\nperformed...\nAre you sure?\n[y/n] ");
    ch = getch();
    switch(ch)
    {
        case 'y':
        case 'Y':
      clear_screen();
      init();
      printf("Init complete!\n");
      action();
      break;
        default:
      clear_screen();
      printf("Init aborted!\n");
      action();
      break;
    }
    break;
      case '3':
    clear_screen();
    connect();
    action();
    break;
      case '4':
    clear_screen();
    disconnect();
    action();
    break;
      case '5':
    clear_screen();
    transact();
    action();
    break;
      case '6':
    clear_screen();
    send_swa();
    action();
    break;
      case '7':
    clear_screen();
    send_dgs();
    action();
    break;
      case '8':
    clear_screen();
    recv();
    action();
    break;
      case '9':
    clear_screen();
    chkrecv();
    action();
    break;
      case 'a':
      case 'A':
    clear_screen();
    read_config();
    action();
    break;
      case 'b':
      case 'B':
    clear_screen();
    write_config();
    action();
    break;
      case 'c':
      case 'C':
    clear_screen();
    setcont();
    action();
    break;
      case 'd':
      case 'D':
    clear_screen();
    abortcont();
    action();
    break;
      case 'e':
      case 'E':
    clear_screen();
    diagnostics();
    action();
    break;
      case 'f':
      case 'F':
    clear_screen();
    enable_tmout(usr_tmout);
    action();
    break;
      case 'g':
      case 'G':
    clear_screen();
    disable_tmout();
    action();
    break;
      default:    
    printf("Bad option\n");
    break;
  }
    }
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   read_config                                        *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure calls the SRF TSR's "SRF READ CONFIG"               *
*   function in order to set up the application's local structure.     *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void read_config()                                                 *    
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void read_config()
{
    srfreadcfg(&cfg);
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   write_config                                       *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure calls the SRF TSR's "SRF WRITE CONFIG"              *
*   function.                                                          *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void write_config()                                                *    
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void write_config()
{
    srfwritecfg(&cfg);
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   setup                                              *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure allows the user to modify certain key SRF para-     *
*   meters.                                                            *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void setup()                                                       *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void setup()
{
    clear_screen();
    setup_menu();
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
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void connect()
{
    unsigned char status;

    printf("Connecting...");    
    status = srfconnect(&cfg);
    printf("\nStatus = %x\n", status);

}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   disconnect                                         *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure disconnects SRF communications.                     *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void disconnect()                                                  *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void disconnect()
{
    srfdisconnect();
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   init                                               *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure initializes SRF.                                    *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void init()                                                        *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void init()
{
    /*  Call SRF INIT function */
    srfinit(&cfg);

    /*  Init application header */
    app_hdr_len = 0;
    cfg.app_hdr = app_hdr_buf;
    cfg.app_hdr_len = &app_hdr_len;
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   transact                                           *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure performs a send/receive using the TSR's SRF TRANS-  *
*   ACT function.                                                      *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void transact()                                                    *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void transact()
{
    unsigned char status;   /* Status of send/recv */

    /*  Set up size of recv buffer */
    recv_len = sizeof(recv_buf);

    /*  Call the TSR function to send and receive */
    clear_screen();
    printf("SRF TRANSACT...");
    status = srftransact(send_buf, send_len, recv_buf, &recv_len, 0); 
    printf("\n");
    clear_screen();

    /*  If good status, print the received data */
    if( status == APP_OK )
    {
  printf("Block received!\n");
  printf("Size: %u bytes\n", recv_len);
  action();
  clear_screen();
  recv_buf[recv_len] = NULL;
  printf("Data is:\n%s\n", recv_buf);
    }

    /*  Otherwise, an error occurred. */
    else
    {
  printf("Error: %x\n", (int)status);
    }
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   send                                               *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure performs a send with acknowledge.                   *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void send_swa()                                                            *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void send_swa()
{
    unsigned char status;       /* Status of send */

    /*  Call the TSR function to send the data */
    printf("SRF Send...");
    status = srfsendswa(send_buf, send_len, 0);
    printf("\n");
    clear_screen();

    /*  Print status */
    if( status == APP_OK )
    {
  printf("Send OK!\n");
    }

    /*  Otherwise, an error occurred. */
    else
    {
  printf("Error: %x\n", (int)status );
    }
}


/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   send_dgs                                           *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure performs a datagram send.                           *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void send_dgs()                                                    *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void send_dgs()
{
    unsigned char status;       /* Status of send */

    /*  Call the TSR function to send the data */
    printf("SRF Dgram...");
    status = srfsenddgs(send_buf, send_len, 0);
    printf("\n");
    clear_screen();

    /*  Print status */
    if( status == APP_OK )
    {
  printf("Send OK!\n");
    }

    /*  Otherwise, an error occurred. */
    else
    {
  printf("Error: %x\n", (int)status );
    }
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   recv                                               *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure performs a receive.                                 *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void recv()                                                        *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void recv()
{
    unsigned char status;       /* Status of receive */

    /*  Set up size of recv buffer */
    recv_len = sizeof(recv_buf);

    /*  Call the TSR function to perform SRF receive */
    printf("SRF Recv...");
    status = srfrecv(recv_buf, &recv_len, 0);
    printf("\n");
    clear_screen();

    /*  If good status, print the received data */
    if( status == APP_OK )
    {
  printf("Block received!\n");
  printf("Size: %u bytes\n", recv_len);
  action();
  clear_screen();
  recv_buf[recv_len] = NULL;
  printf("Data is:\n%s\n", recv_buf);
    }

    /*  Otherwise, an error occurred. */
    else
    {
  printf("Error: %x\n", (int)status);
    }
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   chkrecv                                            *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure performs the SRF CHECK RECV QUEUE function.         *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void chkrecv()                                                     *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void chkrecv()
{
    unsigned char status;       /* Status of command */
    unsigned int num;           /* Number of blocks queued */

    printf("Chk recv...");
    status = srfchkrecv(&num);
    printf("\nStatus = %x\n", status);
    if(status == APP_OK)
    {
  printf("Blocks avail = %d\n", num);
    }
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   setcont                                            *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure performs a SRF SET CONTINUOUS MODE function.        *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void setcont()                                                     *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void setcont()
{
    unsigned char status;       /* Status of command */

    printf("Set cont...");
    status = srfsetcont();
    printf("\nStatus = %x\n", status);
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   abortcont                                          *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure performs a SRF ABORT CONTINUOUS function.           *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void abortcont()                                                   *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void abortcont()
{
    unsigned char status;   /* Status of command */

    printf("Abort cont...");
    status = srfabortcont();
    printf("\nStatus = %x\n", status);
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   diagnostics                                        *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure performs the SRF diagnostics function.              *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void diagnostics()                                                 *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void diagnostics()
{
    unsigned char status;   /* Status of diagnostics */

    printf("Diagnostics...");
    status = srfdiags();
    printf("\n");
    clear_screen();

    /*  Print status */
    if( status == APP_OK )
    {
  printf("Diags OK!\n");
    }

    /*  Otherwise, an error occurred. */
    else
    {
  printf("Error: %x\n", (int)status);
    }
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   setup_menu                                         *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure drives the setup menu.                              *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void setup_menu(void)                                              *
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void setup_menu()
{
    int done, i, j, k, l, ch, status;   /* Assorted temp variables */
    unsigned char x;                    /* Temp storage for hex val */
    unsigned long val;
    unsigned char sval[16];
    unsigned char tval[4];

    done = FALSE;
    i = 0;
    while(done != TRUE)
    {
  if(i > 8)
      i = 0;
  if(i < 0)
      i = 8;
  clear_screen();
  printf("%s\n", setup_menu_option[(3*i)]);
  printf("%s\n", setup_menu_option[(3*i)+1]);
  printf("%s\n", setup_menu_option[(3*i)+2]);
  printf("Setup menu => ");
  ch = getch();
  switch(ch)
  {
      case 0x0d:
    i++;
    break;
      case ' ':
    i--;
    break;
      case '0':
    done = TRUE;
    break;
      case '1':
    clear_screen();
    printf("App header\n");
    printf("Current: %s\n", app_hdr_buf);
    status = getval("==> ","%s", tmp_buf);
    if(status == 1) {
        strcpy(app_hdr_buf, tmp_buf);
        cfg.app_hdr = app_hdr_buf;
        app_hdr_len = strlen(app_hdr_buf);
        cfg.app_hdr_len = &app_hdr_len;
    }
    break;
      case '2':
    clear_screen();
    setup_app_tx_data();
    break;
      case '3':
    clear_screen();
    /*      123456789abcdef0 */
    printf("XP Unit ID\n");
    printf("[Hex]\n");
    printf("Current: %x\n", cfg.xp_unit_id);
    status = getval("==> ", "%x", &val);
    if (status == 1) {
        cfg.xp_unit_id = val;
    }
    break;
      case '4':
    clear_screen();
    printf("XP Dest ID\n");
    printf("[Hex]\n");
    printf("Current: %x\n", cfg.xp_dest_id);
    status = getval("==> ", "%x", &val);
    if (status == 1) {
        cfg.xp_dest_id = val;
    }
    break;
      case '5':
    clear_screen();
    printf("XP Timeout\n");
    printf("[100 ms incr]\n");
    printf("Current: %u\n", cfg.xp_tmout);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.xp_tmout = val;
    }
    break;
      case '6':
    clear_screen();
    printf("XP Prompt Enbl\n");
    printf("[0=off, 1=on]\n");
    printf("Current: %u\n", cfg.xp_prompt_enable);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.xp_prompt_enable = val;
    }
    break;
      case '7':
    clear_screen();
    printf("XP Prompt Page\n");
    printf("[0 - 7]\n");
    printf("Current: %u\n", cfg.xp_prompt_page);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.xp_prompt_page = val;
    }
    break;
      case '8':
    clear_screen();
    printf("XP Retries\n");
    printf("[1 - 255]\n");
    printf("Current: %u\n", cfg.xp_max_retries);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.xp_max_retries = val;
    }
    break;
      case '9':
    clear_screen();
    printf("RF System ID\n");
    printf("[01H - FFH]\n");
    printf("Current: %lx\n", cfg.rf_sys_id);
    status = getval("==> ", "%lx", &val);
    if (status == 1) {
        cfg.rf_sys_id = val;
    }
    break;
      case 'a':
      case 'A':
    clear_screen();
    printf("RF Num Channels\n");
    printf("1 - 4\n");
    printf("Current: %u\n", cfg.rf_num_channels);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_num_channels = val;
    }
    break;
      case 'b':
      case 'B':
    clear_screen();
    printf("RF Channel\n");
    printf("00H - 0FH\n");
    printf("Current: %x\n", cfg.rf_channel);
    status = getval("==> ", "%x", &val);
    if (status == 1) {
        cfg.rf_channel = val;
    }
    break;
      case 'c':
      case 'C':
    clear_screen();
    printf("RF Ch #2\n");
    printf("00H - 0FH\n");
    printf("Current: %x\n", cfg.rf_ch2);
    status = getval("==> ", "%x", &val);
    if (status == 1) {
        cfg.rf_ch2 = val;
    }
    break;
      case 'd':
      case 'D':
    clear_screen();
    printf("RF Ch #3\n");
    printf("00H - 0FH\n");
    printf("Current: %x\n", cfg.rf_ch3);
    status = getval("==> ", "%x", &val);
    if (status == 1) {
        cfg.rf_ch3 = val;
    }
    break;
      case 'e':
      case 'E':
    clear_screen();
    printf("RF Ch #4\n");
    printf("00H - 0FH\n");
    printf("Current: %x\n", cfg.rf_ch4);
    status = getval("==> ", "%x", &val);
    if (status == 1) {
        cfg.rf_ch4 = val;
    }
    break;
      case 'f':
      case 'F':
    clear_screen();
    printf("RF Net Mode\n");
    printf("0=P,1=Nml,2=Psv\n");
    printf("Current: %u\n", cfg.rf_net_mode);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_net_mode = val;
    }
    break;
      case 'g':
      case 'G':
    clear_screen();
    printf("RF Fast Poll\n");
    printf("[1 ms incr]\n");
    printf("Current: %u\n", cfg.rf_fast_poll_period);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_fast_poll_period = val;
    }
    break;
      case 'h':
      case 'H':
    clear_screen();
    printf("RF Slow Poll\n");
    printf("[1 ms incr]\n");
    printf("Current: %u\n", cfg.rf_slow_poll_period);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_slow_poll_period = val;
    }
    break;
      case 'i':
      case 'I':
    clear_screen();
    printf("RF Poll Decay\n");
    printf("[# per step]\n");
    printf("Current: %u\n", cfg.rf_poll_decay);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_poll_decay = val;
    }
    break;
      case 'j':
      case 'J':
    clear_screen();
    printf("RF Fast Delay\n");
    printf("[1 ms incr]\n");
    printf("Current: %u\n", cfg.rf_fast_poll_delay);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_fast_poll_delay = val;
    }
    break;
      case 'k':
      case 'K':
    clear_screen();
    printf("RF Force Slow\n");
    printf("[0=No, 1=Yes]\n");
    printf("Current: %u\n", cfg.rf_force_slow_poll);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_force_slow_poll = val;
    }
    break;
      case 'l':
      case 'L':
    clear_screen();
    /*      123456789abcdef0 */
    printf("RF Dest ID\n");
    printf("Current: %x\n", cfg.rf_dest_id);
    status = getval("==> ", "%x", &val);
    if (status == 1) {
        cfg.rf_dest_id = val;
    }
    break;
      case 'm':
      case 'M':
    clear_screen();
    printf("RF Dgram Size\n");
    printf("[1 - 2048]\n");
    printf("Current: %u\n", cfg.rf_max_dgram_size);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_max_dgram_size = val;
    }
    break;
      case 'n':
      case 'N':
    clear_screen();
    printf("RF Retries\n");
    printf("[1 - 255]\n");
    printf("Current: %u\n", cfg.rf_max_retries);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_max_retries = val;
    }
    break;
      case 'o':
      case 'O':
    clear_screen();
    printf("RF Ref Period\n");
    printf("[10 ms incr]\n");
    printf("Current: %u\n", cfg.rf_refresh_period);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_refresh_period = val;
    }
    break;
      case 'p':
      case 'P':
    clear_screen();
    printf("RF EHL Threshld\n");
    printf("Current: %u\n", cfg.rf_ehl_threshold);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_ehl_threshold = val;
    }
    break;
      case 'q':
      case 'Q':
    clear_screen();
    printf("RF EHL Decay\n");
    printf("Current: %u\n", cfg.rf_ehl_decay);
    status = getval("==> ", "%u", &val);
    if (status == 1) {
        cfg.rf_ehl_decay = val;
    }
    break;
      default:    
    printf("Bad option\n");
    break;
  }
    }
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME: setup_app_tx_data                                    *
*                                                                      *
* DESCRIPTION:                                                         *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*       void setup_app_tx_data(void)                                   *    
*                                                                      *
* INPUT:                                                               *
*                                                                      *
* OUTPUT:                                                              *
*                                                                      *
\**********************************************************************/

void setup_app_tx_data()
{
    int i,num;

    /*  Prompt user for method of keying in block to be transmitted. */    
    getval("Select type:\n1 - Key string\n2 - Repeat char\n-> ", "%d", &i);

    /*  If "1", prompt user to key in string .*/
    if ( i == 1 )
    {
  getval("Data to send:\n", "%s", send_buf);
  send_len = strlen(send_buf);
    }

    /*  Else, prompt user to key in hex character to repeat. */
    else
    {
  getval("Enter hex val\nof char: ", "%x", send_buf);
  getval("Number of reps? ", "%d", &i);
  for(num = 1; num < i; num++) {
      send_buf[num] = send_buf[0];
  }
  send_len = num;
    }
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME: getval                                               *
*                                                                      *
* DESCRIPTION:                                                         *
*       This routine issues a prompt for input of any kind and         *
*       assigns the value to valaddr.  If the input stream cannot      *
*       be matched to the given format, the routine reissues the       *
*       prompt.  If a carriage return is entered, a null string is     *
*       returned in valaddr.                                           *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*       void getval(prompt, format, valaddr)                           *
*       char *prompt, *format;                                         *
*       unsigned *valaddr;                                             *
*                                                                      *
* INPUT:                                                               *
*       prompt          prompt to be issued before read                *
*       format          scanf() format to be used                      *
*       valaddr         address of data to be input                    *
*                                                                      *
* OUTPUT: None, but reads input data into valaddr.                     *
*                                                                      *
\**********************************************************************/

int getval(prompt, format, valaddr)
char *prompt, *format;
unsigned long *valaddr;
{
    int status = 0;
    int done = 0;
    char buff[100];

    while(done != 1){
  printf(prompt);

  gets(buff);
  if(buff[0] == '\0') {               /* check for carriage return */
      *valaddr = '\0';
      done = 1;
      status = 0;
  }
  else {
      if(sscanf(buff, format, valaddr) != 0){
    done = 1;
    status = 1;
      } else {
    printf("Invalid entry\n");
      }
  }
    }
    return(status);
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   action                                             *
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

void action()
{
    printf("< Hit any key >");
    while (!kbhit());
    getch();
    printf("\n");
}

/**********************************************************************\
*                                                                      *
* PROCEDURE NAME:   clear_screen                                       *
*                                                                      *
* DESCRIPTION:                                                         *
*   This procedure clears the display.                                 *
*                                                                      *
* CALLING SEQUENCE:                                                    *
*   void clear_screen()                                                *    
*                                                                      *
* INPUT:                                                               *
*   None.                                                              *
*                                                                      *
* OUTPUT:                                                              *
*   None.                                                              *
*                                                                      *
\**********************************************************************/

void clear_screen()
{
    union REGS iregs;
    union REGS oregs;

    iregs.h.ah=SCROLL_UP_FUNC;  /* Scroll 0 lines to clear display */
    iregs.h.al=00;
    iregs.h.bh=NORMAL_VID_MODE;
    iregs.x.cx=00;
    iregs.h.dl=0x50;            /* Default to 80x25 text display */
    iregs.h.dh=0x19;

    int86(VID_VEC, &iregs, &iregs);

    /* Get current video page */
    iregs.h.ah = GET_VID_STATE;
    int86(VID_VEC, &iregs, &oregs);

    /* Reset cursor */
    iregs.h.ah = SET_CURS_FUNC;
    iregs.h.bh = oregs.h.bh;
    iregs.x.dx = 0;
    int86(VID_VEC, &iregs, &oregs);
    
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

/************************************************************************\
*                                                                        *
* PROCEDURE NAME: atox                                                   *
*                                                                        *
* DESCRIPTION:                                                           *
*       This routine takes an ascii character string containing hex      *
* digits and converts it to an integer.  If successful, the integer      *
* value is returned.  If unsuccesful, 0 is returned.  This routine       *
* was modifed from the original "ascii_to_hex" function in order to have *
* a similar invocation as the comparable library routines atoi() and     *
* atof().                                                                *
*                                                                        *
* CALLING SEQUENCE:                                                      *
*       hex_val = atox(ptr)                                              *
*                                                                        *
* INPUT:                                                                 *
*       u_char *ptr -   Ptr to character string containing hex digits    *
*                                                                        *
* OUTPUT:                                                                *
*       Returns converted hex value if succesful.  Otherwise, it returns *
*       0.                                                               *
*                                                                        *
* VARIABLES: None.                                                       *
*                                                                        *
* SUBROUTINES: None.                                                     *
*                                                                        *
\************************************************************************/
unsigned int atox(ptr)
unsigned char *ptr;
{
    int hex;
    int digit;
    int status = 1;

    hex = 0;
    while (*ptr != '\0' && *ptr != 'h' && *ptr != 'H' && status == OK)
    {
  if (*ptr >= '0' && *ptr <= '9')
  {
      digit = *ptr - 0x30;            /* convert 0-9 to hex */
  }
  else
  {
      if (*ptr >= 'A' && *ptr <= 'F')
      {
    digit = *ptr - 0x37;        /* convert A-F to hex */
      }
      else
      {
    if (*ptr >= 'a' && *ptr <= 'f')
    {
        digit = *ptr - 0x57;    /* convert a-f to hex */
    }
    else
    {
        status = 0;
    }
      }
  }
  hex *= 0x10;
  hex += digit;
  ptr++;
    }

    if(status == 1)
    {
  if (hex < 0 || hex >= 0x100)
  {
      hex = 0;
  }
    }
    else
    {
  hex = 0;
    }

    return(hex);
}

