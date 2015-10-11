// tlxcnsts.h
// costanti librerie TELXON

/* structures used to set up for interrupts */
union REGS  iregs;
union REGS  oregs;
struct  SREGS segregs;

#define OK  1
//#define ERROR 0

/* vector macros */
#define VIDEO_VEC   0x10  /* BIOS video interrupt */
#define CLEAR_SCREEN  0x83  /* Clear screen function */
#define INTERFACE_VEC 0x7A  /* vector to steal for TSR program */

/* command codes for the TSR */
#define BAR_SCAN_ON   0x80
#define BAR_SCAN_OFF  0x81
#define CONFIG_BAR_SCAN 0x82
#define SETUP_BARCODES  0x83
#define READ_SCAN_DATA  0x84
#define WEDGE_ON        0x01
#define WEDGE_OFF       0x00
#define DATA_BLOCK_SIZE 2048     /* utility buffer size */

unsigned char buf[DATA_BLOCK_SIZE];
unsigned int len;       /* # bytes in buf */

/* structure used for barcode definition */
struct LABEL_DEFINITION
{
  unsigned char label_type;
  unsigned char label_length;
  unsigned char generic_options;
  unsigned char label_options;
  unsigned char drop_options;
  unsigned char gsn;
};

/* array of barcodes defined */
struct LABEL_DEFINITION barcodes[3];

