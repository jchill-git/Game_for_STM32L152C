#include "main.h"

static volatile unsigned int TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

#define BASE     (0x40020000)

#define OFF             0
#define ON              1

struct gpio {
  int mode;             //0x00
  int outputType;       
  int outputSpeed;      //0x08
  int pullUpPullDown;   
  int input;            //0x10
  int output;           
  int setReset;         //0x18
  int lock;             
  int altLow;           //0x20
  int altHigh;          
};

#define RCC_BLOCK_A     0x1
#define RCC_BLOCK_B     0x2

#define MODER_OUTPUT_7  0x4000
#define MODER_OUTPUT_6  0x1000

#define GREEN_LED       0x80
#define BLUE_LED        0x40
#define BOTH_LED        0xC0

struct gpio* portA = (struct gpio*) (BASE + 0x0000);
struct gpio* portB = (struct gpio*) (BASE + 0x0400);

/* Private Function prototypes -----------------------------------------------*/
void  RCC_Configuration(void);
void  RTC_Configuration(void);
void  Init_GPIOs (void);
void  Init_Board(void);
void  moveNext(int);
void  movePrev(int);
void  moveUp(int);
void  moveDown(int);
void  sendAlert(void);
void  row0(void);
void  row1(void);
void  row2(void);
void  endGame(void);
void  resetGame(void);
/*******************************************************************************/
//global variables
int   symbolLoc;
//position of dragon
int   dragLoc;
//number of fly objects on screen
int   obCount;
//should another object be generated?
bool  needOb;
//is the botton pressed?
volatile bool  buttonPress;
//counter to give player time to respond to warning
int   responseTime;

int   points;

int   endCon;

int   objLine;

int   newSeed;
int main(void) {
  Init_Board();
  needOb=TRUE;
  //game active
  GAME:while(endCon==0){
  //fly up when pressed
    if(buttonPress==TRUE){
      moveUp(dragLoc);
    }
  //fall when not pressed
    else{
    moveDown(dragLoc);
    }
  //if no incoming object send new object
    if(needOb==TRUE){
    sendAlert();
    }
    if(needOb==FALSE){
    //allow enough time to react between objects
      if(responseTime<3){
        responseTime++;
      }
      else{
        needOb=TRUE;
        responseTime=0;
        if(objLine==0){
          row0();
        }
        if(objLine==1){
          row1();
        }
        if(objLine==2){
          row2();
        }
      }
    }
  }
  
  //end or play again?
  LCD_GLASS_DisplayString("HOLD");
  Delay(1000);
  LCD_GLASS_Clear();
  LCD_GLASS_DisplayString("BUTTON");
  Delay(1000);
  LCD_GLASS_Clear();
  LCD_GLASS_DisplayString("TO");
  Delay(1000);
  LCD_GLASS_Clear();
  LCD_GLASS_DisplayString("PLAY");
  Delay(1000);
  LCD_GLASS_Clear();
  LCD_GLASS_DisplayString("AGAIN");
  Delay(1000);
  LCD_GLASS_Clear();
  LCD_GLASS_DisplayString("3");
  Delay(1000);
  LCD_GLASS_DisplayString("2");
  Delay(1000);
  LCD_GLASS_DisplayString("1");
  Delay(1000);
  if(buttonPress==TRUE){
    resetGame();
    goto GAME;
  }
  LCD_GLASS_Clear();
}

void resetGame(){
  //reset game variables
  newSeed+=points;
  endCon=0;
  dragLoc=0;
  buttonPress=FALSE;
  needOb=TRUE;
  points=0;
}

void endGame(){
  //convert points to string
  char score[50];
  sprintf(score,"%d", points);
  
  //end game display sequence
  LCD_GLASS_Clear();
  Delay(100);
  LCD_GLASS_DisplayString("MISS");
  Delay(1000);
  LCD_GLASS_Clear();
  LCD_GLASS_DisplayString("SCORE");
  Delay(1000);
  LCD_GLASS_Clear();
  LCD_GLASS_DisplayString(score);
  Delay(3000);
  LCD_GLASS_Clear();
  endCon=1;
}

void sendAlert(){
  needOb=FALSE;
  //select row randomly
  srand(dragLoc+points+newSeed);
  objLine=(rand()%(3));
  //top row alert
  if(objLine==2){
    portB->output = GREEN_LED;
    Delay(500);
    portB->output = 0;
  }
  //mid row alert
  if(objLine==1){
    portB->output = BOTH_LED;
    Delay(500);
    portB->output = 0;
  }
  //bottom row alert
  if(objLine==0){
    portB->output = BLUE_LED;
    Delay(500);
    portB->output = 0;
  }
}

void row0(){
  if(dragLoc==1){
    LCD_GLASS_DisplayString("*    _");
    Delay(200);
    LCD_GLASS_DisplayString("*   _ ");
    Delay(200);
    LCD_GLASS_DisplayString("*  _  ");
    Delay(200);
    LCD_GLASS_DisplayString("* _   ");
    Delay(200);
    LCD_GLASS_DisplayString("*_    ");
    Delay(200);
    LCD_GLASS_DisplayString("&     ");
    Delay(200);
    endGame();
    }
    else if(dragLoc==2){
    LCD_GLASS_DisplayString("%    _");
    Delay(200);
    LCD_GLASS_DisplayString("%   _ ");
    Delay(200);
    LCD_GLASS_DisplayString("%  _  ");
    Delay(200);
    LCD_GLASS_DisplayString("% _   ");
    Delay(200);
    LCD_GLASS_DisplayString("%_    ");
    Delay(200);
    LCD_GLASS_DisplayString("$     ");
    Delay(200);
    endGame();
    }
    else{
    LCD_GLASS_DisplayString("/    _");
    Delay(200);
    LCD_GLASS_DisplayString("/   _ ");
    Delay(200);
    LCD_GLASS_DisplayString("/  _  ");
    Delay(200);
    LCD_GLASS_DisplayString("/ _   ");
    Delay(200);
    LCD_GLASS_DisplayString("/_    ");
    Delay(200);
    LCD_GLASS_DisplayString("/     ");
    Delay(200);
    points++;
    }
}

void row1(){
  if(dragLoc==1){
    LCD_GLASS_DisplayString("*    -");
    Delay(200);
    LCD_GLASS_DisplayString("*   - ");
    Delay(200);
    LCD_GLASS_DisplayString("*  -  ");
    Delay(200);
    LCD_GLASS_DisplayString("* -   ");
    Delay(200);
    LCD_GLASS_DisplayString("*-    ");
    Delay(200);
    LCD_GLASS_DisplayString("*     ");
    Delay(200);
    points++;
    }
    else if(dragLoc==2){
    LCD_GLASS_DisplayString("%    -");
    Delay(200);
    LCD_GLASS_DisplayString("%   - ");
    Delay(200);
    LCD_GLASS_DisplayString("%  -  ");
    Delay(200);
    LCD_GLASS_DisplayString("% -   ");
    Delay(200);
    LCD_GLASS_DisplayString("%-    ");
    Delay(200);
    LCD_GLASS_DisplayString("#     ");
    Delay(200);
    endGame();
    }
    else{
    LCD_GLASS_DisplayString("/    -");
    Delay(200);
    LCD_GLASS_DisplayString("/   - ");
    Delay(200);
    LCD_GLASS_DisplayString("/  -  ");
    Delay(200);
    LCD_GLASS_DisplayString("/ -   ");
    Delay(200);
    LCD_GLASS_DisplayString("/-    ");
    Delay(200);
    LCD_GLASS_DisplayString(":     ");
    Delay(200);
    endGame();
    }
}

void row2(){
  if(dragLoc==1){
    LCD_GLASS_DisplayString("*    @");
    Delay(200);
    LCD_GLASS_DisplayString("*   @ ");
    Delay(200);
    LCD_GLASS_DisplayString("*  @  ");
    Delay(200);
    LCD_GLASS_DisplayString("* @   ");
    Delay(200);
    LCD_GLASS_DisplayString("*@    ");
    Delay(200);
    LCD_GLASS_DisplayString("^     ");
    Delay(200);
    endGame();
    }
    else if(dragLoc==2){
    LCD_GLASS_DisplayString("%    @");
    Delay(200);
    LCD_GLASS_DisplayString("%   @ ");
    Delay(200);
    LCD_GLASS_DisplayString("%  @  ");
    Delay(200);
    LCD_GLASS_DisplayString("% @   ");
    Delay(200);
    LCD_GLASS_DisplayString("%@    ");
    Delay(200);
    LCD_GLASS_DisplayString("%     ");
    Delay(200);
    points++;
    }
    else{
    LCD_GLASS_DisplayString("/    @");
    Delay(200);
    LCD_GLASS_DisplayString("/   @ ");
    Delay(200);
    LCD_GLASS_DisplayString("/  @  ");
    Delay(200);
    LCD_GLASS_DisplayString("/ @   ");
    Delay(200);
    LCD_GLASS_DisplayString("/@    ");
    Delay(200);
    LCD_GLASS_DisplayString(";     ");
    Delay(200);
    endGame();
    }
}

void moveUp(int loc){
  if(loc==0){
    dragLoc=1;
    LCD_GLASS_DisplayString("*");
    Delay(1000);
   }
   if(loc==1){
    dragLoc=2;
    LCD_GLASS_DisplayString("%");
    Delay(1000);
   }
    if(loc==2){
    LCD_GLASS_DisplayString("%");
    Delay(1000);
   }
}

void moveDown(int loc){
  if(loc==2){
    dragLoc=1;
    LCD_GLASS_DisplayString("*");
    Delay(1000);
   }
   if(loc==1){
    dragLoc=0;
    LCD_GLASS_DisplayString("/");
    Delay(1000);
   }
    if(loc==0){
    dragLoc=0;
    LCD_GLASS_DisplayString("/");
    Delay(1000);
   }
}


//move to the next rightward spot based on present position of symbol
void moveNext(int loc){
//case 0
  if(loc==0){
    symbolLoc=1;
    LCD_GLASS_DisplayString("*     ");
    Delay(1000);
  }
//case 1
  if(loc==1){
    symbolLoc=2;
    LCD_GLASS_DisplayString(" *    ");
    Delay(1000);
  }
//case 2
  if(loc==2){
    symbolLoc=3;
    LCD_GLASS_DisplayString("  *   ");
    Delay(1000);
  }
//case 3
  if(loc==3){
    symbolLoc=4;
    LCD_GLASS_DisplayString("   *  ");
    Delay(1000);
  }
//case 4
  if(loc==4){
    symbolLoc=5;
    LCD_GLASS_DisplayString("    * ");
    Delay(1000);
  }
//case 5
  if(loc==5){
    symbolLoc=6;
    LCD_GLASS_DisplayString("     *");
    Delay(1000);
  }
//case 6
  if(loc==6){
    symbolLoc=0;
    LCD_GLASS_Clear();
    Delay(1000);
  }
}

//move to the next leftward spot based on present positon of symbol
void movePrev(int loc){
//case 0
  if(loc==0){
    symbolLoc=6;
    LCD_GLASS_DisplayString("     *");
    Delay(1000);
  }
//case 1
  if(loc==1){
    symbolLoc=0;
    LCD_GLASS_Clear();
    Delay(1000);
  }
//case 2
  if(loc==2){
    symbolLoc=1;
    LCD_GLASS_DisplayString("*     ");
    Delay(1000);
  }
//case 3
  if(loc==3){
    symbolLoc=2;
    LCD_GLASS_DisplayString(" *    ");
    Delay(1000);
  }
//case 4
  if(loc==4){
    symbolLoc=3;
    LCD_GLASS_DisplayString("  *   ");
    Delay(1000);
  }
//case 5
  if(loc==5){
    symbolLoc=4;
    LCD_GLASS_DisplayString("   *  ");
    Delay(1000);
  }
//case 6
  if(loc==6){
    symbolLoc=5;
    LCD_GLASS_DisplayString("    * ");
    Delay(1000);
  }
}

//set button variable based on hardware interupt           
void UserButtonDown(void)
{
    buttonPress=TRUE;
}

void UserButtonUp(void)
{
    buttonPress=FALSE;
}

void Delay(unsigned int nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0);
  
}

void Decrement_TimingDelay(void)
{
  if (TimingDelay != 0x00) { 
    TimingDelay--;
  }
}                                  

void Init_Board(void)
{ 
  /* Configure RTC Clocks */
  RTC_Configuration();

  /* Set internal voltage regulator to 1.8V */
  PWR_VoltageScalingConfig(PWR_VoltageScaling_Range1);

  /* Wait Until the Voltage Regulator is ready */
  while (PWR_GetFlagStatus(PWR_FLAG_VOS) != RESET) ;

  /* Enable debug features in low power modes (Sleep, STOP and STANDBY) */
#ifdef  DEBUG_SWD_PIN
  DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY, ENABLE);
#endif
  
  /* Configure SysTick IRQ and SysTick Timer to generate interrupts */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  
  /* Initializes the LCD glass */
  LCD_GLASS_Configure_GPIO();
  LCD_GLASS_Init();

  /* Init I/O ports */
  Init_GPIOs();
}
		
//Configures the different system clocks.
void RCC_Configuration(void)
{  
  
  /* Enable HSI Clock */
  RCC_HSICmd(ENABLE);
  
  /*!< Wait till HSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
  {}

  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  
  RCC_MSIRangeConfig(RCC_MSIRange_6);

  RCC_HSEConfig(RCC_HSE_OFF);  
  if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET )
  {
    while(1);
  }
 
  /* Enable  comparator clock LCD and PWR mngt */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_LCD | RCC_APB1Periph_PWR, ENABLE);
    
  /* Enable ADC clock & SYSCFG */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_SYSCFG, ENABLE);

}


void RTC_Configuration(void)
{
  
/* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  /* LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  
  RCC_RTCCLKCmd(ENABLE);
   
  /* LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable comparator clock LCD */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_LCD, ENABLE);

}

//To initialize the I/O ports
void conf_analog_all_GPIOS(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOs clock */ 	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | 
                        RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD | 
                        RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  
  /* Disable GPIOs clock */ 	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB |
                        RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD | 
                        RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH, DISABLE);
}

void  Init_GPIOs (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable GPIOs clock */ 	
  RCC_AHBPeriphClockCmd(LD_GPIO_PORT_CLK | USERBUTTON_GPIO_CLK, ENABLE);
 
  /* Configure User Button pin as input */
  GPIO_InitStructure.GPIO_Pin = USERBUTTON_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_Init(USERBUTTON_GPIO_PORT, &GPIO_InitStructure);

  /* Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

  /* Configure User Button and IDD_WakeUP EXTI line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0 ;  // PA0 for User button AND IDD_WakeUP
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set User Button and IDD_WakeUP EXTI Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure); 

/* Configure the GPIO_LED pins  LD3 & LD4*/
  GPIO_InitStructure.GPIO_Pin = LD_GREEN_GPIO_PIN | LD_BLUE_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(LD_GPIO_PORT, &GPIO_InitStructure);
  GPIO_LOW(LD_GPIO_PORT, LD_GREEN_GPIO_PIN);	
  GPIO_LOW(LD_GPIO_PORT, LD_BLUE_GPIO_PIN);

/* Disable all GPIOs clock */ 	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | 
                        RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD | 
                        RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOH, DISABLE);

  RCC_AHBPeriphClockCmd(LD_GPIO_PORT_CLK | USERBUTTON_GPIO_CLK, ENABLE);
}  

