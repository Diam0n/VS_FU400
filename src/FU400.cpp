#include <Arduino.h>


#define DISABLE 0

#define ON LOW
#define OFF HIGH
#define STOP        !BT_STATE[0]      //BTN STOP NC!!


#define BOOT_TIME 2000
#define WORK_ALAMP_DELAY 1000
//#define 

//D13 - MORGANIE ZAZHIM INSTRUMENTSA


//######################### PINS ##############################

#define POWER_ON 2          //pusk knopka shkaf (a gde kontrol naprug i ostalnogo?) - -- delat stop razriv STOP MAIN
#define ERR_END 3           //na potom oshibka (stol end?)
//####KNOPKI STANINA
#define BTN_JOG_STOL  4     //tolchok stol      S57
#define BTN_JOG_SPIN  5     //tolchok spindel   S56
////#define BTN_CL        5     //zazhim razhim tool  S251


//####KNOPKI

#define END_BOX_SPIN  6   // koncevik korobki spindel

//perkluchatel auto

#define SW_FU_MODE_M    7    //manual
#define SW_FU_MODE_XY   8    //ramka XY
#define SW_FU_MODE_XZ   9    //ramka XZ
#define SW_FU_MODE _YZ  10   //ramka YZ
//###############################//DEBUG/REMOTE IF NO CHOICE





//#define END_STOP_W  11    // rabochaya zona stol
////#define END_CL      10    // tool zazhat  S252
///#define END_BOX_STOL  9   // koncevik korobki podach



#define END_STOL_W_X_PL  11 //stol x+ levo stop 
#define END_STOL_W_X_MI   12 //stol x- pravo stop 
#define END_STOL_W_Y_PL   16 //stol y+ ot koloni stop 
#define END_STOL_W_Y_MI   17 //stol y- k kolone stop 
#define END_STOL_W_Z_PL   18 //stol z+ stol vniz
#define END_STOL_W_Z_MI   19 //stol z- ztol up

#define END_X_AXIS  54    // ruchki axis 
#define END_Y_AXIS  55
#define END_Z_AXIS  56
#define END_KRON_UP 58    // ends konsoli
#define END_KRON_DOWN 57

//####LEVIY BLOK RELE   M - DVIGUN
#define M_SPIN_L  53  //Q1
#define M_SPIN_R  52  //Q2
#define M_MOV_L   51   //Q5
#define M_MOV_R   50   //Q6
#define M_ZZR     49   //Q7
#define M_WATER   48   //Q8
#define M_KRON_DOWN 46   //Q9
#define M_KRON_UP 47   //Q10
#define M_CLIN    45   //Q11
#define M_CLOUT   44   //Q12

#define LAMP_X    43
#define LAMP_Y    42
#define LAMP_Z    41
#define LAMP_PL   39
#define LAMP_MI   40
#define LAMP_SPD  38
#define LAMP_CL   13

//####LEVIY BLOK RELE

//####PRAVIY BLOK RELE    MU - MUFTA
#define MU_RUN    37   //Y1
#define MU_SPD    36   //Y2
#define MU_TX     35   //Y7
#define MU_TY     34   //Y8
#define MU_TZ     33   //Y9
#define MU_TS     32   //Y6

#define MU_X      30   //Q13
#define MU_Y      31   //Q14
#define MU_Z      29   //Q15
//####PRAVIY BLOK RELE


//####KNOPKI PULT
#define BTN_STOP    22
#define BTN_X       23
#define BTN_Y       24
#define BTN_Z       25
#define BTN_RUN_PL  27
#define BTN_RUN_MI  26
#define BTN_SPD     28

// + ili -
#define START_PLS      BT_STATE[4]         
#define START_MNS      BT_STATE[5]


//####PERKLYUCHATEL
#define SW_ST_JOG   69  //OFF?
#define SW_ST_SET   68
#define SW_ST_RUN   67
#define SW_ST_CLIN  66
#define SW_ST_CLOUT 65

#define SW_WATER 64

#define SW_SPIN_L   63
#define SW_SPIN_R   61
#define SW_SPIN_M   62

#define SW_ZZR      60
#define SW_KRON     59

#define ERR_X FU_STATE[4]
#define ERR_Y FU_STATE[5]
#define ERR_Z FU_STATE[6]

#define FU_SIZE 50      //razmer massiva parametrov FU_STATUS
#define BT_ARR_SIZE 6   //size of button matrix  
#define DEB_BT 50       //debounce time
#define DEB_SPD 200       //debounce SPD MU
#define DEB_ZZR 3000    //debounce ZZR
#define DEB_STOP 500    //debounce STOP to MU

//####PERKLYUCHATEL

//VARS

int STATUS; //-1 to STOP 0-STOP 1-RUN 2-VIBOR 
/*
-3 - ERROR ???
-2 - main POWER BOX not ready
-1 - to start (stop to start)
0 - ready for RUN
1 - RUN
*/


//BUTTONS INERTED NAOBOROT BLYAT!!!!!!!!!!!!!!!
const int BT_PIN[]={BTN_STOP, BTN_X, BTN_Y, BTN_Z, BTN_RUN_PL, BTN_RUN_MI};   // 6 buttons pins
bool BT_STATE [BT_ARR_SIZE];                    //sostoyanie knopok pult
bool BT_pSTATE [BT_ARR_SIZE];                    // prev state dlya debounce
unsigned long BT_debSTATE[BT_ARR_SIZE];                   // deb state ^^
//bool STOP_STATE;
//bool STOP_pSTATE;
int FU_STATE [FU_SIZE];                           //massiv sostoyanij

int BT_SPD;         //uskorenaya button status 
int BT_pSPD;        //uskorennya prev status
unsigned long BT_debSPD;      // timer press
unsigned long delay_toSPD;    // timer uskor enable
unsigned long delay_SPD;      // itogo debounce SPD  
unsigned long BT_debSTOP;     // timer STOP for MU

//int delay_toRUN;
unsigned long toSPD;
//int toRUN;
unsigned long  time_curloop;

unsigned long WORK_ALAMP_deb;
bool WORK_ALAMP_STATE;

int mode;            //vibor rezhima
int spin_mode;
int toJOGst;
int toJOGsp;
//ERRORS

int ERR;
int ERR_IS_WORK;        //ENABLE???
int ERR_AXIS;
int ERR_BOX_SP;
int ERR_BOX_ST;
//int ERR_X;
//int ERR_Y;
//int ERR_Z;
//int 
int i;

void setup() {
  i=0;
  Serial.begin(9600);
  
//init ERROR
  ERR=1;
  ERR_IS_WORK=1;
  ERR_AXIS=1;
  
  ERR_X=1;
  ERR_Y=1;
  ERR_Z=1;

  STATUS=0;
 // STOP_STATE=0;
 // STOP_pSTATE=0;
//  memset(BT_STATE, 0, BT_ARR_SIZE);
//  memset(BT_pSTATE, 0, BT_ARR_SIZE);
// memset(BT_pSTATE, 0, BT_debSTATE);
for (i=0; i < BT_ARR_SIZE; i++){            //setup buttons
  BT_STATE[i]=0;
  BT_pSTATE[i]=0;

}

for (i=0; i < FU_SIZE; i++){            //FU_STATUS to zero
  FU_STATE[i]=0;
}

  time_curloop=0;
  mode=0;

  BT_SPD=0;
  BT_pSPD=0;
  BT_debSPD=0;
  delay_toSPD=0;
  toSPD=0;
  BT_debSTOP=0;
//  delay_toRUN=0;  //delay to spd-to-run
//  toRUN=1;  //delay to spd-to-run
  toJOGsp=0;
  toJOGst=0;

  pinMode(POWER_ON, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);
  pinMode(BTN_X, INPUT_PULLUP);
  pinMode(BTN_Y, INPUT_PULLUP);
  pinMode(BTN_Z, INPUT_PULLUP);
  pinMode(BTN_RUN_PL, INPUT_PULLUP);
  pinMode(BTN_RUN_MI, INPUT_PULLUP);
  pinMode(BTN_SPD, INPUT_PULLUP);
  pinMode(SW_ST_JOG, INPUT_PULLUP);
  pinMode(SW_ST_SET, INPUT_PULLUP);
  pinMode(SW_ST_RUN, INPUT_PULLUP);
  pinMode(SW_ST_CLIN, INPUT_PULLUP);
  pinMode(SW_ST_CLOUT, INPUT_PULLUP);
  pinMode(SW_WATER, INPUT_PULLUP);
  pinMode(SW_SPIN_L, INPUT_PULLUP);
  pinMode(SW_SPIN_R, INPUT_PULLUP);
  pinMode(SW_SPIN_M, INPUT_PULLUP);

  pinMode(SW_ZZR, INPUT_PULLUP);
  pinMode(SW_KRON, INPUT_PULLUP);
  pinMode(END_KRON_UP, INPUT_PULLUP);
  pinMode(END_KRON_DOWN, INPUT_PULLUP);
  pinMode(END_X_AXIS, INPUT_PULLUP);
  pinMode(END_Y_AXIS, INPUT_PULLUP);
  pinMode(END_Z_AXIS, INPUT_PULLUP);

  pinMode(END_STOL_W_X_PL, INPUT_PULLUP);
  pinMode(END_STOL_W_X_MI, INPUT_PULLUP);
  pinMode(END_STOL_W_Y_PL, INPUT_PULLUP);
  pinMode(END_STOL_W_Y_MI, INPUT_PULLUP);
  pinMode(END_STOL_W_Z_PL, INPUT_PULLUP);
  pinMode(END_STOL_W_Z_MI, INPUT_PULLUP);

  pinMode(END_BOX_SPIN, INPUT_PULLUP);
  pinMode(BTN_JOG_SPIN, INPUT_PULLUP);
  pinMode(BTN_JOG_STOL, INPUT_PULLUP);


  pinMode(M_SPIN_R, OUTPUT);
  pinMode(M_SPIN_L, OUTPUT);
  pinMode(M_MOV_L, OUTPUT);
  pinMode(M_MOV_R, OUTPUT);
  pinMode(M_ZZR, OUTPUT);
  pinMode(M_WATER, OUTPUT);
  pinMode(M_KRON_UP, OUTPUT);
  pinMode(M_KRON_DOWN, OUTPUT);
  pinMode(M_CLIN, OUTPUT);
  pinMode(M_CLOUT, OUTPUT);
  pinMode(LAMP_CL, OUTPUT);
  pinMode(LAMP_X, OUTPUT);
  pinMode(LAMP_Y, OUTPUT);
  pinMode(LAMP_Z, OUTPUT);
  pinMode(LAMP_PL, OUTPUT);
  pinMode(LAMP_MI, OUTPUT);
  pinMode(LAMP_SPD, OUTPUT);
  pinMode(MU_RUN, OUTPUT);
  pinMode(MU_SPD, OUTPUT);
  pinMode(MU_TX, OUTPUT);
  pinMode(MU_TY, OUTPUT);
  pinMode(MU_TZ, OUTPUT);
  pinMode(MU_TS, OUTPUT);
  pinMode(MU_X, OUTPUT);
  pinMode(MU_Y, OUTPUT);
  pinMode(MU_Z, OUTPUT);

  digitalWrite(M_SPIN_R, OFF);
  digitalWrite(M_SPIN_L, OFF);
  digitalWrite(M_MOV_L, OFF);
  digitalWrite(M_MOV_R, OFF);
  digitalWrite(M_ZZR, OFF);
  digitalWrite(M_WATER, OFF);
  digitalWrite(M_KRON_UP, OFF);
  digitalWrite(M_KRON_DOWN, OFF);
  digitalWrite(M_CLIN, OFF);
  digitalWrite(M_CLOUT, OFF);
  digitalWrite(LAMP_X, OFF);
  digitalWrite(LAMP_Y, OFF);
  digitalWrite(LAMP_Z, OFF);
  digitalWrite(LAMP_PL, OFF);
  digitalWrite(LAMP_MI, OFF);
  digitalWrite(LAMP_SPD, OFF);
  digitalWrite(MU_RUN, OFF);
  digitalWrite(MU_SPD, OFF);
  digitalWrite(MU_TX, OFF);
  digitalWrite(MU_TY, OFF);
  digitalWrite(MU_TZ, OFF);
  digitalWrite(MU_TS, OFF);
  digitalWrite(MU_X, OFF);
  digitalWrite(MU_Y, OFF);
  digitalWrite(MU_Z, OFF);
  
  Serial.println("HELLLOOO!!!!!!");
  // TODOOOOO POKLACAT LAMPAMI --- Done
  digitalWrite(LAMP_X, ON);
  digitalWrite(LAMP_SPD, ON);
  delay(200);
  digitalWrite(LAMP_X, OFF);
  digitalWrite(LAMP_SPD, OFF);
  digitalWrite(LAMP_Y, ON);
  digitalWrite(LAMP_MI, ON);
  delay(200);
  digitalWrite(LAMP_Y, OFF);
  digitalWrite(LAMP_MI, OFF);
  digitalWrite(LAMP_Z, ON);
  digitalWrite(LAMP_PL, ON);
  delay(200);
  digitalWrite(LAMP_Z, OFF);
  digitalWrite(LAMP_PL, OFF);
  delay(200);
  digitalWrite(LAMP_X, ON);
  digitalWrite(LAMP_Y, ON);
  digitalWrite(LAMP_Z, ON);
  digitalWrite(LAMP_PL, ON);
  digitalWrite(LAMP_MI, ON);
  digitalWrite(LAMP_SPD, ON);
  delay(200);
  digitalWrite(LAMP_X, OFF);
  digitalWrite(LAMP_Y, OFF);
  digitalWrite(LAMP_Z, OFF);
  digitalWrite(LAMP_PL, OFF);
  digitalWrite(LAMP_MI, OFF);
  digitalWrite(LAMP_SPD, OFF);
  //delay(BOOT_TIME);
}

void f_STOP(){ 
  FU_STATE[0]=0;
  
  //OFF ALL
  digitalWrite(MU_RUN, OFF);
  digitalWrite(MU_SPD, OFF);
  digitalWrite(MU_X, OFF);
  digitalWrite(MU_Y, OFF);
  digitalWrite(MU_Z, OFF);
  digitalWrite(M_SPIN_R, OFF);
  digitalWrite(M_SPIN_L, OFF);
  digitalWrite(M_MOV_L, OFF);
  digitalWrite(M_MOV_R, OFF);
  digitalWrite(M_ZZR, OFF);
  digitalWrite(M_WATER, OFF);
  digitalWrite(M_KRON_UP, OFF);
  digitalWrite(M_KRON_DOWN, OFF);
  digitalWrite(M_CLIN, OFF);
  digitalWrite(M_CLOUT, OFF);
  //except AXIS LAMP
  FU_STATE[12]=0;                      //viklyuchaem lampu SPD
  digitalWrite(LAMP_PL, OFF);
  digitalWrite(LAMP_MI, OFF);
  digitalWrite(LAMP_SPD, OFF);
  /*
  // if (!ERR && ERR_IS_WORK) digitalWrite(MU_TS, ON);  //NEMA POTUZHNOSTI tormoz shpindel????
  while (!digitalRead(BTN_STOP)){                      //tormoz spindel poka derzhim stop i visim
    digitalWrite(MU_TS, ON);
  }
  */
  //ili sdelat s zaderzhkoy????
  if (STOP && ((time_curloop - BT_debSTOP)>(DEB_STOP)/2)){ //poka nazhata to nachinaem tormozit spin cherez DEB_STOP/2 (~250ms) i stol tozhe
    digitalWrite(MU_TS, ON);
    digitalWrite(MU_TX, ON);    //vkl T_MU axis
    digitalWrite(MU_TY, ON);
    digitalWrite(MU_TZ, ON);
  }                             // esli stop otzhat posle DEB_STOP -> vikl T_MU 
    else if (!STOP && ((time_curloop - BT_debSTOP)>DEB_STOP)){
    digitalWrite(MU_TX, OFF);    
    digitalWrite(MU_TY, OFF);
    digitalWrite(MU_TZ, OFF);
    digitalWrite(MU_TS, OFF);
    STATUS=0;                                          // i perehod v rezhim STOP
  } 
                              // esli stop otzhat - stop stol do DEB_STOP
    else {
      STATUS=-1;
      digitalWrite(MU_TX, ON);    //vkl T_MU axis
      digitalWrite(MU_TY, ON);
      digitalWrite(MU_TZ, ON);
      digitalWrite(MU_TS, ON);
    }
      
  
}


//READ BTN
//BUTTONS INVERTED NAOBOROT BLYAT!!!!!!!!!!!!!!!
void f_BREAD () {                                 //read buttons NOT SWITCH

  bool b_read=0;
  for (i=0; i < BT_ARR_SIZE; i++){
   b_read=!digitalRead(BT_PIN[i]);
    
   if (b_read!=BT_pSTATE[i]) BT_debSTATE[i]=time_curloop;

   if ((time_curloop-BT_debSTATE[i]) > DEB_BT){
     if (b_read!=BT_STATE[i]){
      BT_STATE[i]=b_read;
     }
    }
   BT_pSTATE[i]=b_read;
  }
}


//VIBOR PERED START
void f_CHOICE(){                                //from BUTTONS to FU_STATE
  /*
    * FU STATE
      0- 0-disable all (todo)
    * 1-x
    * 2-y
    * 3-z
    * 4 - disable X
    * 5 - disable Y
    * 6 - disable Z
    * 7 - MU podacha
    * 8 - MU Uskor
    */
//AXIS CHOOSE
    if (BT_STATE[1]){
      FU_STATE[1]=1;
      FU_STATE[2]=0;
      FU_STATE[3]=0;
      }
    if (BT_STATE[2]){
      FU_STATE[1]=0;
      FU_STATE[2]=1;
      FU_STATE[3]=0;
      }
    if (BT_STATE[3]){
      FU_STATE[1]=0;
      FU_STATE[2]=0;
      FU_STATE[3]=1;
      }

digitalWrite(LAMP_X, !FU_STATE[1]);
digitalWrite(LAMP_Y, !FU_STATE[2]);
digitalWrite(LAMP_Z, !FU_STATE[3]);
      


//REZHIM RABOTI

mode=0;
FU_STATE[11]=0;
FU_STATE[19]=0;
 if (!digitalRead(SW_ST_JOG)) mode=1;
 if (!digitalRead(SW_ST_SET)) mode=2;
 if (!digitalRead(SW_ST_RUN)) mode=3;
 if (!digitalRead(SW_ST_CLIN)) mode=4;
 if (!digitalRead(SW_ST_CLOUT)) mode=5;
 
 if (mode==2 || mode==3) FU_STATE[11]=1;
 /*
 switch (mode){                           //razreshenie SPINDEL                             
    case 2:
    FU_STATE[11]=1;
    break;
    case 3:
    FU_STATE[11]=1;
    break;
    }
 */   
if (mode) FU_STATE[20]=1;               //razreshenie M podach

FU_STATE[19]=mode;
//REZHIM SPINDEL
spin_mode=0;
if (!digitalRead(SW_SPIN_L)) spin_mode=1;     //spindel mode 1 levo pravo mayatnik
if (!digitalRead(SW_SPIN_R)) spin_mode=2;     //spindel mode 2 pravo 
if (!digitalRead(SW_SPIN_M)) spin_mode=3;     //spindel mode 3 mayatnik

if (spin_mode==0 || mode==0){  //ne vibran reshi, raboti ili rezhim spindelya - zapret spindel
  FU_STATE[11]=0;
  FU_STATE[20]=0;
  FU_STATE [19]=0;
}



if (!digitalRead(SW_ZZR) && FU_STATE[1]){ //ZAZOR ENABLE esli X i ustanovka debounce na SPD 
  FU_STATE[13]=1;
  delay_SPD=DEB_ZZR;
  }
  else{
    FU_STATE[13]=0;
    delay_SPD=DEB_SPD;  
  }


if (!digitalRead(SW_KRON) && (FU_STATE[1] || FU_STATE[2])){ //KRON ENABLE esli X or Y
    FU_STATE[15]=1;
    FU_STATE[16]=1;
    FU_STATE[17]=0;
  }
  else{
    FU_STATE[15]=0;
  }
   
/* check choised axis? error? 
*  
*/
return;
}


void f_RUN(){
  if(FU_STATE[19]==3 || FU_STATE[19]==1)                    //SPD tolko v mode WORK ili NALADKA
  {
    FU_STATE[12]=1;  //vklyuchaem lampu SPD
    bool b_read=0;
  
    b_read=!digitalRead(BTN_SPD);
    
    if (b_read!=BT_pSPD) BT_debSPD=time_curloop;

    if ((time_curloop-BT_debSPD) > 100){
      if (b_read!=BT_SPD){
        BT_SPD=b_read;
        //DUMP BUTTON STATE VSTAVIT
      }
    }
    BT_pSPD=b_read;
    
  }

///RUN OFF - SPD ON
  if (BT_SPD)
  { 
    FU_STATE[7]=0;
    if (FU_STATE[13])           //if ZZR enable to vikl M_ZZR
      FU_STATE[14]=0;
    if (FU_STATE[15]){          //esli KRON on to stol down
      FU_STATE[16]=0;           // stol up vikl
      FU_STATE[17]=1;           // stol down VKL
    }
  }  

  if (BT_SPD && !toSPD) {
    delay_toSPD=time_curloop;
    toSPD=1;

  }
  if (BT_SPD && (time_curloop-delay_toSPD)>delay_SPD){
    FU_STATE[8]=1;
  }

//SPD OFF _ RUN ON
  if (!BT_SPD && toSPD) {
    toSPD=0;
    FU_STATE[8]=0;
    //delay_toRUN=time_curloop; //delay to spd-to-run
    FU_STATE[7]=1;              //NO delay to spd-to-run
    if (FU_STATE[13])           //esli ZZR enable to enable ZZR_M
      FU_STATE[14]=1;
    if (FU_STATE[15]){           //esli KRON on to stol UP
       FU_STATE[17]=0;           // stol down VIK
       FU_STATE[16]=1;           // stol up VKL
     }
  } 

/*if (!BT_SPD && (time_curloop-delay_toRUN)>DEB_SPD)  //delay to spd-to-run
  {
    FU_STATE[7]=1;
  }
*/

if (!digitalRead(SW_WATER))
  FU_STATE[18]=1;
  else
    FU_STATE[18]=0;

// FU_STATE[18] to byte format!!!!!!!!!!!!!! (NE AUTO REZHIM)
/*for (i=0; i<6; i++){
  FU_STATE[31][i];
}

if (FU_STATE[31]) {
    //lyuboy iz W_koncevik - uskorennaya OFF
  FU_STATE[8]=0;
  FU_STATE[12]=0;
}
*/
return;
}



void f_WORKMATRIX(){
/*
  OPISANIE FU_STATE

  FU_STATE 20 - razreshnenie vkl M podach 
  
  * FU STATE
      0- 0-disable all (todo)
    * 1 - X
    * 2 - Y
    * 3 - Z
    * 4 - disable X (ERR_X)
    * 5 - disable Y (ERR_Y)
    * 6 - disable Z (ERR_Z)
    * 7 - MU podacha
    * 8 - MU Uskor
    * 9 - levo (+)
    * 10 - prabo (-)
    * 11 - allow SPIN enable?
    * 
  
  */
  

  if (mode==3 || mode==1)                       // rezhim WORK (??ili NALADKA)
  {
    if (ERR_X) {                                //X esli END_X (ruchka v limbe) - MU_T OFF, AXIS OFF
      digitalWrite(MU_TX, OFF);
      digitalWrite(MU_X, OFF);
    }
    else{                                       //inache NORM
      digitalWrite(MU_TX, FU_STATE[1]);     
      digitalWrite(MU_X, !FU_STATE[1]);    
    }
    
    if (ERR_Y) {                                //Y 
      digitalWrite(MU_TY, OFF);
      digitalWrite(MU_Y, OFF);
    }
    else{                                       //inache NORM
      digitalWrite(MU_TY, FU_STATE[2]);     
      digitalWrite(MU_Y, !FU_STATE[2]);    
    }
    if (ERR_Z) {                                //Z
      digitalWrite(MU_TZ, OFF);
      digitalWrite(MU_Z, OFF);
    }
    else{                                       //inache NORM
      digitalWrite(MU_TZ, FU_STATE[3]);     
      digitalWrite(MU_Z, !FU_STATE[3]);    
    }    
    
    digitalWrite(MU_RUN, !FU_STATE[7]);  //PODACHA
    digitalWrite(MU_SPD, !FU_STATE[8]);  //USKORENAYA

    //WORK KONCEVIK otklyuchaet podachu v svoem napralenii
        //inverse for ON
      if (!digitalRead(END_STOL_W_X_PL) && FU_STATE[1] && FU_STATE[9]) FU_STATE[20]=0;    //esli koncevik X+ i axis X i napravlenie + : to HUY!
      if (!digitalRead(END_STOL_W_X_MI) && FU_STATE[1] && FU_STATE[10]) FU_STATE[20]=0;   //esli koncevik X- i axis X i napravlenie - : to HUY!
      if (!digitalRead(END_STOL_W_Y_PL) && FU_STATE[2] && FU_STATE[9]) FU_STATE[20]=0;    //esli koncevik Y+ i axis Y i napravlenie + : to HUY!
      if (!digitalRead(END_STOL_W_Y_MI) && FU_STATE[2] && FU_STATE[10]) FU_STATE[20]=0;   //esli koncevik Y- i axis Y i napravlenie - : to HUY!
        //reverse for Z
      if (!digitalRead(END_STOL_W_Z_PL) && FU_STATE[3] && FU_STATE[9]) FU_STATE[20]=0;    //esli koncevik Z+ i axis Z i napravlenie + : to HUY!
      if (!digitalRead(END_STOL_W_Z_MI) && FU_STATE[3] && FU_STATE[10]) FU_STATE[20]=0;   //esli koncevik Z- i axis Z i napravlenie - : to HUY!
    //VKLYUCHAEM M podachi dlya X/Y i revers dlya Z. Pri uslovii OK dlya podach
    if(!FU_STATE[3] && FU_STATE[20])                      //vkl M podacha LEVO         REVERS DLYA Z
      digitalWrite(M_MOV_L, !FU_STATE[9]);
    else if(!FU_STATE[3] && FU_STATE[20])                  //
      digitalWrite(M_MOV_R, !FU_STATE[9]);

    if(!FU_STATE[3] && FU_STATE[20])                      //vkl M podacha  PRAVO        REVERS DLYA Z
      digitalWrite(M_MOV_R, !FU_STATE[10]);
    else if(FU_STATE[3] && FU_STATE[20]) 
      digitalWrite(M_MOV_L, !FU_STATE[10]);
  };

  digitalWrite(LAMP_PL, !FU_STATE[9]);
  digitalWrite(LAMP_MI, !FU_STATE[10]);

  if (FU_STATE[11] && !ERR_BOX_SP)                       //RAZRESHNIE SHPINDEL
      switch (spin_mode){
        case 1:
        digitalWrite(M_SPIN_L, !FU_STATE[11]);
        break;
        case 2:
        digitalWrite(M_SPIN_R, !FU_STATE[11]);
        break;
        case 3:
        break;
      }
//FU_STATE 12 - RAZRESHENIE SPD vkl LAMP
//FU_STATE 13 - ENABLE ZZR
//FU_STATE 14 - vkl M_ZZR LAMP
if (FU_STATE[13])
  digitalWrite(M_ZZR, !FU_STATE[14]);

//FU_STATE 15 - ENABLE KRON-CONTROL
//FU_STATE 16 - KRON UP (normal)
//FU_STATE 17 - KRON DOWN (if uskor i !Z) для Z инверсия!
if (FU_STATE[15])
  {
    if (!digitalRead(END_KRON_UP) && FU_STATE[16])
      digitalWrite(M_KRON_UP, ON);
      else
        digitalWrite(M_KRON_UP, OFF);

    if (!digitalRead(END_KRON_DOWN) && FU_STATE[17])
      digitalWrite(M_KRON_DOWN, ON);
      else
        digitalWrite(M_KRON_DOWN, OFF);
  }
else{
  if(!digitalRead(END_KRON_UP)) digitalWrite(M_KRON_UP, ON);      //STOL VSEGDA VVERH
  if(digitalRead(END_KRON_UP)) digitalWrite(M_KRON_UP, OFF);
  digitalWrite(M_KRON_DOWN, OFF);
  Serial.print ("KROUP");
}  
// FU_STATE 18 - ohlazhdayka
if (FU_STATE[11])
  digitalWrite(M_WATER, !FU_STATE[18]);

//LAMP!
digitalWrite(LAMP_SPD, !FU_STATE[12]);

// FU_STATE 19 mode SET (rezhim raboti)
return;
}

void f_LAMP(){

  }

void f_DEBUG(){
  if (STATUS==-1){
    Serial.println("IN STOP OR ERROR");
    };
  if (STATUS==1){
/*     digitalWrite(10, OFF);
    digitalWrite(11, ON);
    digitalWrite(12, OFF);
    digitalWrite(13, OFF);
 */
    Serial.println("RUN");
    };
  if (STATUS==0){
    Serial.println("STOPPED CHOISE JOG? NO ERROR");
/*     
    digitalWrite(10, OFF);
    digitalWrite(11, OFF);
    digitalWrite(12, ON);
    digitalWrite(13, OFF);
 */
    };

/*  if (BT_STATE[0]){
     digitalWrite(13, ON);
    }
    else
    digitalWrite(13, OFF);
    */
   Serial.print(FU_STATE[1]);
   Serial.print(FU_STATE[2]);
   Serial.print(FU_STATE[3]);
   Serial.println(",> ");
//  */
}


//BTN JOG epta
void f_JOG (){  
if (FU_STATE[19]==1)  {
  if (!digitalRead(BTN_JOG_STOL)){
      digitalWrite(MU_RUN,ON);
      digitalWrite(MU_SPD,OFF);
      digitalWrite(M_MOV_R,ON);
      toJOGst=1;
    }  
  else if (toJOGst){
      digitalWrite(MU_RUN,OFF);
      digitalWrite(MU_SPD,OFF);
      digitalWrite(M_MOV_R,OFF);
      toJOGst=0;
    }
}

if (FU_STATE[19]==2)  {
  if (!digitalRead(BTN_JOG_SPIN) && digitalRead(END_BOX_SPIN)){
      digitalWrite(M_SPIN_L,ON);
      digitalWrite(M_SPIN_R,OFF);
    }  
  if (digitalRead(BTN_JOG_SPIN) || !digitalRead(END_BOX_SPIN)){
      digitalWrite(M_SPIN_R,OFF);
      digitalWrite(M_SPIN_L,OFF);
    }
  }


return;
}

void f_ERR (){
  //read ENDs to ERR etc

  /*  ERR  - obshaya oshibka
    ERR_END - oshibka koncevik
    ERR_X  - koncevik ruchki X
    ERR_Y - koncevik ruchki Y
    ERR_Z - koncevik ruchki Z
    ERR_SPD_SW  - korobka podach
  */
  /*
  * FU_STATE
  * 4 - disable X
  * 5 - disable Y
  * 6 - disable Z
  */
  ERR=0;
  if (!digitalRead(POWER_ON))
    ERR_IS_WORK=1;
    else
      ERR_IS_WORK=0;    //esli halt END to bida

  ERR_BOX_SP=digitalRead(END_BOX_SPIN);
  
  //NO AXIS ENDS?? (razmikanie pri vstavlenoyruchke)
  ERR_X=digitalRead(END_X_AXIS);
  ERR_Y=digitalRead(END_Y_AXIS);
  ERR_Z=digitalRead(END_Z_AXIS);

  FU_STATE[25]=!digitalRead(END_STOL_W_X_PL);   //stol x+ levo stop  ERR_X_MOV_PL
  FU_STATE[26]=!digitalRead(END_STOL_W_X_MI);   //stol x- pravo stop 
  FU_STATE[27]=!digitalRead(END_STOL_W_Y_PL);   //stol y+ ot koloni stop 
  FU_STATE[28]=!digitalRead(END_STOL_W_Y_MI);   //stol y- k kolone stop 
  FU_STATE[29]=!digitalRead(END_STOL_W_Z_PL);   //stol z+ stol vniz
  FU_STATE[30]=!digitalRead(END_STOL_W_Z_MI);   //stol z- stol vverh
  FU_STATE[31]=FU_STATE[25]+(FU_STATE[26]*10)+(FU_STATE[27]*100)+(FU_STATE[28]*1000)+(FU_STATE[29]*10000)+(FU_STATE[30]*100000);
  if (FU_STATE[31] && !FU_STATE[0] && !FU_STATE[32])      //TOLKO POSLE STOP
  {
    FU_STATE[32]=1;
    }
  if (!FU_STATE[31])
    FU_STATE[32]=0;
  ERR=!ERR_IS_WORK+ERR;        //REMAKE

  //oshibok poka net !!!!!!!!!!!!!!!!! UDALIT!!!!!!!
 
  if (ERR) {           //esli err to bida a nado li? proverka v loop
    FU_STATE[0]=0;
    STATUS=-1;
  } 

  return; 
}


void loop() {
  time_curloop=millis();                                      // current time for loop
  f_BREAD();                                                  // BUTTON update
  // moved to f_MATRIX f_LAMP();                                                   // LAMP update   ---   too f_matrix??
  f_ERR();                                                    // ERROR work -- err to fail
  
  if ((STOP || ERR) && STATUS>0){             //zapisivaem nachalo stop ili err i stavim status toSTOP
    FU_STATE[0]=0;
    STATUS=-1; //strashno                       // if STOP or ERR i !toSTOP -> toSTOP SUKABLYAT!
    BT_debSTOP=time_curloop;                    //last time press stop ili voznikla oshibkla
  } 
  // toSTOP make STOP
  if (STATUS<0) f_STOP();                       //stop in progress - -1, stop otrabotal - 0 stopped
 
  //if (!ERR || !DISABLE || STATUS==-1)  //sdelat - esli net oshibok ili stanok pusk ERR- main oshibka DISABLE - net puska (vlogit v ERR??) 
  switch (STATUS){
    case -1:
      f_STOP();                   //po idee poka stop ne otrabotal -> f_stop
      break;

      case 0:
      if (!ERR) f_JOG();          // eboshim JOG tolko v rezhime NO_RUN
      if (!ERR) f_CHOICE();
      break;

      case 1:
      if (!ERR) f_RUN();
      
      break;

      /*case 2:
      if (!ERR) f_CHOICE();
      break;
      */
  } 

//  if (!ERR && (!START_PLS || !START_MNS)) STATUS=1;//---------        // STARTUEM?
///POKA   if (STATUS==0 && !ERR) STATUS=2;
//  if ((STATUS==2 && !ERR) && (START_PLS || START_MNS) && (FU_STATE[1] || FU_STATE[2] || FU_STATE[3])) {  ///STARTUEM!!!! s viborom AXIS
if ((STATUS==0 && !ERR) && (START_PLS || START_MNS)) {  ///STARTUEM!!!! BEZ vibor AXIS
    FU_STATE[0]=1;                //go work
    STATUS=1;                     //STATUS RUN
    FU_STATE[9]=START_PLS;        //start left or
    FU_STATE[10]=START_MNS;       //start right
    FU_STATE[7]=1;                //mu_RUN ON?
    if (FU_STATE[13])             //esli ZZR en to M_ZZR EN stol UP
      FU_STATE[14]=1;
    }

// ispolnayem esli FU_STATE[0] = 0

if (FU_STATE[0]>0) 
  f_WORKMATRIX();                                             //FU_STATE to real
//f_DEBUG();
Serial.println();
Serial.print("ERR=");
Serial.print (ERR);
Serial.print(" STATUS=");
Serial.print (STATUS);
Serial.print (" FUSTATE_O=");
Serial.print (FU_STATE[0]);
Serial.print (" PON");
Serial.print (!digitalRead(POWER_ON));

Serial.print ( "   krup=");
Serial.print (digitalRead(END_KRON_UP));
Serial.print ( " krdn=");
Serial.print (digitalRead(END_KRON_DOWN));


if ((time_curloop-WORK_ALAMP_deb)>WORK_ALAMP_DELAY)
  { 
    WORK_ALAMP_STATE=!WORK_ALAMP_STATE;
    digitalWrite(LAMP_CL, WORK_ALAMP_STATE);
    WORK_ALAMP_deb=time_curloop; 
  }
}