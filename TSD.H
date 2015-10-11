/**********************************************************************\
*                                                                    
*   DATE: 08/20/93                WRITTEN BY: Telxon O.S.           
*   
*   FILE:   tsd.h
*
*   DESCRIPTION:
*       This is the TSD library header file.
*
*   REVISIONS:
*       1.1     08/30/93    FJL
*               File created.
*
*       1.1     08/24/93    ECM
*               Added tsd_bioscan_vec and TSD_BIOSCAN_SVC.
*
*       1.1     09/01/93    ECM
*               Added vector variables and equates for Spread support.
*                                                       
*       1.1     12/06/93    ECM
*               Added vector variable and equate for battery control.
*
\**********************************************************************/

#define TSD_TRUE        1
#define TSD_FALSE       0

extern int far pascal GetTSDVec(void);
extern void far initgblvecs(void);

extern void far * near pascal TSDADD;
extern unsigned char near pascal TSDVEC;

extern int TSDpresent;
extern int GblVecInit;

extern unsigned int tsd_video_vec; 
extern unsigned int tsd_sys_vec;
extern unsigned int tsd_keybd_vec;    
extern unsigned int tsd_print_vec;
extern unsigned int tsd_scan_vec;
extern unsigned int tsd_mcard_vec;
extern unsigned int tsd_rtc_vec; 
extern unsigned int tsd_comm_vec;
extern unsigned int tsd_bioscan_vec;
extern unsigned int tsd_srf_int_vec;
extern unsigned int tsd_srf_xprx_vec; 
extern unsigned int tsd_srf_tmr_vec; 
extern unsigned int tsd_srf_post_vec; 
extern unsigned int tsd_batt_vec;


#define TSD_BATT_SVC            0x08        /* Battery control services */
#define TSD_VIDEO_SVC           0x10        /* Video driver services */
#define TSD_COMM_SVC            0x14        /* Communication services */
#define TSD_SYS_SVC             0x15        /* System services */
#define TSD_KEYBD_SVC           0x16        /* Keyboard services */
#define TSD_PRINT_SVC           0x17        /* Parallel printer services */
#define TSD_SCAN_SVC            0x7A        /* Scanner services */
#define TSD_MCARD_SVC           0x80        /* Magcard services */
#define TSD_RTC_SVC             0x1A        /* Real time clock services */
#define TSD_BIOSCAN_SVC         0x54        /* Bios scanner services */
#define TSD_SRF_SVC             0x60
#define TSD_SRF_XPRX_SVC        0x61
#define TSD_SRF_TMR_SVC         0x62
#define TSD_SRF_POST_SVC        0x63



