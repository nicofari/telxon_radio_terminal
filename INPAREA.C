#include <stdio.h>
#include <string.h>
#include "telxon.h"  
#include "costanti.h"

extern void  CopiaStr(char* sDst, const char* sSrc, const int start, const int stop);
extern void Cls();
void evalstatus(unsigned int status, const int bVerbose);

unsigned int InputArea(char* sCodArea)
{
  int scrl_speed, w_delay;
  unsigned int status;

  DFT_STRUCT accept_dft ;
  DFT_STRUCT *screen;

  static char fieldstr[5];       // store data as character strs 
//    unsigned int  fieldinteger;       // store data into integer      
///    unsigned long fieldlong;        // store data into long     

  static char *prompt1= "AREA: ";
//    static char *prompt2= "ACCEPT LEFT TO RT";
//    static char *prompt3= "ACCEPT RT TO LEFT";

  static char *mask1=   "____";
//    static char *mask2=   "__________";
//    static char *mask3=   "__________";

  Cls();
// Resetta il campo (spero)
  memset(fieldstr, ' ', 8);  
  
  w_delay = WBEEPDEFAULT;       // set wand beep delay       
  scrl_speed = 10;          // set the scroll speed for edit 
  setdelayvals(scrl_speed, w_delay);      // set delay values once       

// #1 PROMPT 
  screen = &accept_dft; 
  screen->df1 = WANDABLE;   // Inverse video prompt, wand   
  screen->df2 = ALPNUM_DATA | EDIT_DATA; // Alpha/Numeric, edit     

  screen->prompt_str = prompt1;     // setup the data format table  
  screen->prompt_len = strlen(prompt1);   // get the length of prompt1    

  screen->mask_str = mask1;
  screen->mask_str_len = strlen(mask1);  // get the length of mask1      

  screen->prompt_col = 0;       // col 1, row 2, zero relative  
  screen->prompt_row = 1;
  screen->mask_col = 8;
//    screen->mask_row = 2;       // col 9, row 3, zero relative   
  screen->mask_row = 1;       
  screen->default_str_len = 0;      // no default data        

  screen->min_input = 1;        // min/max range        
  screen->max_input = CODAREA_LEN;
            // define the storage type      
  screen->field.fieldchar = fieldstr;     // pointer to character string  
  screen->field_len = CODAREA_LEN+1;       // maximum length of the field  

  screen->clear_start_line = 0;     // line num zero relative     
  screen->clear_end_line = 20;
  screen->status_line = 20;       // status line num zero relative

  status = acceptscreen(screen);      // accept the data        
//    evalstatus(status,FALSE);         // check return status      
  if (status == GOOD_DATA) {
#ifdef _DEBUG  
    printf("%s\n", fieldstr);
#endif
    CopiaStr(sCodArea,fieldstr,0,CODAREA_LEN);
  }

/**********************************************************************    
// #2 PROMPT 
    screen->df1 = WANDABLE;       // Wandable prompt        

    screen->df2 = NUMERIC_DATA | EDIT_DATA; // Numeric, edit        

    screen->prompt_str = prompt2;     // setup the data format table  
    screen->prompt_len = 0x00;        // use 0 for null terminated
                 prompt strings       
    screen->mask_str = mask2;
    screen->mask_str_len = strlen(mask2);   // setup length of mask2      
              // use the same col, row
                 default_str_len, min/max
                 field pointer, clr lines
                 and status line info     

    screen->field.fieldint = &fieldinteger; // pointer to integer field     
    screen->field_len = 1;        // store into 1 integer     

    status = acceptscreen(screen);      // accept the data        
    evalstatus(status);         // check return status      
    if (status = GOOD_DATA)
      printf("%s\n", fieldstr);

// #3 PROMPT 
    screen->df1 = WANDABLE | INV_PROMPT | INV_ACCEPTSCR;
    screen->df2 = ALPNUM_DATA | EDIT_DATA;

    screen->prompt_str = prompt3;     
    screen->prompt_len = 0x00;        // use 0 for null terminated    

    screen->mask_str = mask3;
    screen->mask_str_len = strlen(mask3);   // setup length of mask2      
              // use the same col, row
                 default_str_len, min/max
                 field pointer, clr lines
                 and status line info     

    screen->field.fieldint = &fieldinteger; // pointer to integer field     
    screen->field_len = 1;        // store into 1 integer     

    status = acceptscreen(screen);      // go accept the data    
    evalstatus(status);
    if (status = GOOD_DATA)
      printf("%s\n", fieldstr);
***********************************************************/
  return status;
}

//******************************************************
//    evalstatus - evaluates the status returned from
//     acceptscreen library function 
//*******************************************************    
void evalstatus(unsigned int status, const int bVerbose)
{
unsigned char type;
char dir;
int mode, cols, page, attr, len=0;
    static char *goodmsg=   "GOOD DATA  ";
    static char *lenmsg=    "LENGTH ERR ";
    static char *inputmsg=  "INPUT TYPE ERR";
    static char *barmsg=    "BAR KEY    ";
    static char *farmsg=    "FAR KEY    ";
    static char *darmsg=    "DAR KEY    ";
    static char *uarmsg=    "UAR KEY    ";
    static char *clrmsg=    "CLR KEY    ";
    static char *bskeymsg=  "BS KEY     ";
    static char *delmsg=    "DELETE KEY ";
    static char *ltormsg=   "L to R SCAN ";
    static char *rtolmsg=   "R to L SCAN ";
    static char *unknown=   "UNKNOWN RETVAL";

    attr = NORMAL_VID;
    getvidmode(&mode, &cols, &page);
    setcurpos(page, 4, 2);
    switch (status)
    {
  case GOOD_DATA :
       if (bVerbose) writestr (page, goodmsg, len, attr);
       break;
  case LENGTH_ERR :
       if (bVerbose) writestr (page, lenmsg, len, attr);
       break;
  case INPUT_TYPE_ERR :
       if (bVerbose) writestr (page, inputmsg, len, attr);
       break;
  case WND :
       dir = getlabeltype(&type);
       if ( dir == L_TO_R)
    if (bVerbose) writestr(page, ltormsg, len, attr);
       else
    if (bVerbose) writestr(page, rtolmsg, len, attr);
       switch (type)
       {
    case PLESSEY:
        if (bVerbose) writestr(page, "Plessey", len, attr);
        break;
    case ALPHA_PLESS:
        if (bVerbose) writestr(page, "Alpha Plessey", len, attr);
        break;
    case ISBN_PLESS:
        if (bVerbose) writestr(page, "Isbn Plessey", len, attr);
        break;
    case PURE_PLESS:
        if (bVerbose) writestr(page, "Pure Plessey", len, attr);
        break;
    case SAIN_PLESS:
        if (bVerbose) writestr(page, "Sainesbury Plessey", len, attr);
        break;
    case UPC:
        if (bVerbose) writestr(page, "UPC", len, attr);
        break;
    case EAN:
        if (bVerbose) writestr(page, "Ean", len, attr);
        break;
    case UPC_EAN:
        if (bVerbose) writestr(page, "Upc Ean", len, attr);
        break;
    case CODABAR:
        if (bVerbose) writestr(page, "Codabar", len, attr);
        break;
    case CODE_3_9:
        if (bVerbose) writestr(page, "Code 3 of 9", len, attr);
        break;
    case CODE_2_5:
        if (bVerbose) writestr(page, "Code 2 of 5", len, attr);
        break;
    case DISCR_2_5:
        if (bVerbose) writestr(page, "Discr 2 of 5", len, attr);
        break;
    case INTERL_2_5:
        if (bVerbose) writestr(page, "Interl 2 of 5", len, attr);
        break;
    case INDUST_2_5:
        if (bVerbose) writestr(page, "Indust 2 of 5", len, attr);
        break;
    case CODE_11:
        if (bVerbose) writestr(page, "Code 11", len, attr);
        break;
    case CODE_128:
        if (bVerbose) writestr(page, "Code 128", len, attr);
        break;
       }
       break;

      case BAR :
       if (bVerbose) writestr (page, barmsg, len, attr);
       break;
  case FAR :
       if (bVerbose) writestr (page, farmsg, len, attr);
       break;
  case DAR :
       if (bVerbose) writestr (page, darmsg, len, attr);
       break;
  case UAR :
       if (bVerbose) writestr (page, uarmsg, len, attr);
       break;
  case CLR :
       if (bVerbose) writestr (page, clrmsg, len, attr);
       break;
  case BSKEY :
           if (bVerbose) writestr (page, bskeymsg, len, attr);
       break;
  case DEL :
       if (bVerbose) writestr (page, delmsg, len, attr);
       break;
  default : if (bVerbose) write(page, unknown, len, attr);
       break;
    }
    sysdelay(SECOND);
}

