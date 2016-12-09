// File    :   traffic_light.c
// Abstract:   A simple code to realize traffic light using event-driven programming
//			   approach.
// Hardware:   Raspberry Pi with Pibrella Board
// Language:   C
// Author  :   Swarna Kamakshi Jayaraman
// Project :   Internal  
// Subject :   ES6152: Embedded Software Development
// Version :   <1.0> Initial Version
//----------------------------------------------------------------------------------//
//-----Headers-----
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
//-----MACROS-----
#define PIBRELLA_RED 7
#define PIBRELLA_YELLOW 0
#define PIBRELLA_GREEN 2
#define PUSH_BUTTON 14
#define HW_PWM_PIN 1
#define HIGH 1
#define LOW 0
//-----Structures-----
struct BlinkLEDArgs
{
 int pin_num;
 int duration;
 int freq;
};

struct BuzzerArg
{
 int duration;
 int frequency;
};

struct GlowLEDArgs
{
 int pin_num;
 int duration;
};

typedef struct BuzzerArg BArg;
typedef struct BlinkLEDArgs BLArgs;
typedef struct GlowLEDArgs GLArgs;
//---Prototype Declaration---
void pibrellaSetup();
void *blink_t(void*);
void *startBuzzer(void*);
void *ringBuzzer(void*);
void *glowLED(void*);

//-----Main Function-----
void main(void)
{
 wiringPiSetup();
 pibrellaSetup();
 //-----Initializing thread function arguments------
 pthread_t tr, ty, tg, tb; //Threads for LED Blink and buzzer
 pthread_t g1, g2, b1, b2; //Threads for Green LED Blink and buzzer ringing
 pthread_t y1, b3; //Threads for glowing yellow LED and starting buzzer
 BLArgs l1, l2, l3; // LED Args
 int tr_ret,ty_ret, tg_ret,tb_ret; // Thread return values
 int g1_ret, g2_ret, b1_ret, b2_ret;
 int y1_ret, b3_ret;
 //Initializing args to thread functions
 l1.pin_num = PIBRELLA_RED;
 l1.duration = 1;
 l1.freq = 1;

 l2.pin_num = PIBRELLA_YELLOW;
 l2.duration = 1;
 l2.freq = 1;

 l3.pin_num = PIBRELLA_GREEN;
 l3.duration = 1;
 l3.freq = 1;

 int ring_duration = 1;
 //-----Spawn 3 threads for blinking LEDs once
 tr_ret = pthread_create(&tr,NULL,blink_t,(void *)&l1);
 if(tr_ret)
   printf("Thread tr creation failed\n");

 ty_ret = pthread_create(&ty,NULL,blink_t,(void *)&l2);
 if(ty_ret)
   printf("Thread ty creation failed\n");

 tg_ret = pthread_create(&tg,NULL,blink_t,(void *)&l3);
 if(tg_ret)
   printf("Thread tg creation failed\n");

 tb_ret = pthread_create(&tb,NULL, startBuzzer, (void *)ring_duration);
 if(tb_ret)
   printf("Thread tb creation failed\n");

 pthread_join(tr,NULL);
 pthread_join(ty,NULL);
 pthread_join(tg,NULL);
 pthread_join(tb,NULL);
 printf("Init Thread Exec complete ! \n");
 //-----Thread Execution Complete-----------
 
 l1: digitalWrite(PIBRELLA_RED, HIGH);  //RED LED On
 for(;;) //--Monitor push button status
 {
  if(digitalRead(PUSH_BUTTON)==HIGH) //When push button is pressed
  {
    printf("button pressed\n");
    delay(2000);
    digitalWrite(PIBRELLA_RED, LOW);
    digitalWrite(PIBRELLA_GREEN, HIGH);
    delay(5000);
    digitalWrite(PIBRELLA_GREEN,LOW);
   // delay(200);

    BLArgs g_1, g_2; //Objects of BlinkLEDArgs structure
    BArg b_1, b_2; //Objects of structure holding buzzer parameters

    g_1.pin_num = PIBRELLA_GREEN;
    g_1.duration = 5;
    g_1.freq = 1;

    b_1.duration = 5;
    b_1.frequency = 1;

    g1_ret = pthread_create(&g1, NULL, blink_t, (void *)&g_1);
    if(g1_ret)
     printf("Thread for green LED Blink @ 1Hz unsuccessful !\n");

    b1_ret = pthread_create(&b1,NULL, ringBuzzer, (void *)&b_1);
    if(b1_ret)
      printf("Thread for ringing of buzzer @1Hz unsuccessful !\n");

    pthread_join(g1, NULL);
    pthread_join(b1, NULL);

    g_2.pin_num = PIBRELLA_GREEN;
    g_2.freq = 2;
    g_2.duration = 5;

    b_2.duration = 5;
    b_2.frequency = 2;

    g2_ret = pthread_create(&g2,NULL,blink_t, (void *)&g_2);
    if(g2_ret)
      printf("Thread for blinking green LED @2Hz unsuccessful!\n");

    b2_ret = pthread_create(&b2,NULL, ringBuzzer, (void *)&b_2);
    if(b2_ret)
     printf("Thread for ringing buzzer @ 2Hz unsuccessful !\n");

    pthread_join(g2, NULL);
    pthread_join(b2, NULL);

    GLArgs y_glow; //Object of GlowLEDArgs structure
    y_glow.pin_num = PIBRELLA_YELLOW;
    y_glow.duration = 2;
    ring_duration = 2;
    y1_ret = pthread_create(&y1, NULL,  glowLED, (void *)&y_glow);
    if(y1_ret)
      printf("Thread for glowing yellow LED unsuccessful !\n");
    b3_ret = pthread_create(&b3, NULL, startBuzzer, (void *)ring_duration);
    if(b3_ret)
     printf("Thread for ringing buzzer for 2s failed !\n");

    pthread_join(y1, NULL);
    pthread_join(b3, NULL);

    goto l1;
   }
  }
} /*--End-of-Main()--*/

//------Pibrella Setup-----
void pibrellaSetup()
{
 //LED Config
 pinMode(PUSH_BUTTON,INPUT);
 pinMode(PIBRELLA_RED, OUTPUT);
 pinMode(PIBRELLA_YELLOW,OUTPUT);
 pinMode(PIBRELLA_GREEN, OUTPUT);
 //Pull down input pin
 pullUpDnControl(PUSH_BUTTON, PUD_DOWN);
 //Set Output pins to low
 digitalWrite(PIBRELLA_RED,LOW);
 digitalWrite(PIBRELLA_YELLOW,LOW);
 digitalWrite(PIBRELLA_GREEN,LOW);

 //Setup PWM
 pinMode(HW_PWM_PIN, PWM_OUTPUT);
 pwmSetMode(PWM_MODE_MS); //To run PWM Generator in Mark:Space Mode
 pwmSetRange(100); //Sets range register in PWM Generator
 pwmWrite(HW_PWM_PIN,LOW);
}

//-----Thread Routine-----
void *blink_t(void *args)
{
 int pin_num, duration, freq;
 int T = 0;
 BLArgs *Arg = (BLArgs *)args;
 if(Arg != NULL)
 {
  pin_num = (int)Arg->pin_num;
  duration = (int)Arg->duration;
  freq = (int)Arg->freq;
 }
 while(T < duration*freq)
 {
  digitalWrite(pin_num, HIGH);
  delay(1000/(2*freq));
  digitalWrite(pin_num, LOW);
  delay(1000/(2*freq));
  T++;
 }
 pthread_exit(NULL);
}
//-----Start Buzzer-------
void* startBuzzer(void* time_delay)
{
 int duration = (int)time_delay;
 pwmWrite(HW_PWM_PIN, 50);
 delay(duration*1000);
 pwmWrite(HW_PWM_PIN,0);
 pthread_exit(NULL);
}
//---Ring Buzzer with certain frequency---
void* ringBuzzer(void *arg)
{
 BArg *temp = (BArg *)arg;
 if(NULL != temp)
 {
  int freq = (int)(temp->frequency);
  int duration = (int)(temp->duration);
  int clock = 1000/(2*freq);
  int T = 0;
  while(T<duration*freq)
  {
   pwmWrite(HW_PWM_PIN,50);
   delay(clock);
   pwmWrite(HW_PWM_PIN, LOW);
   delay(clock);
   T++;
  }
 }
 pthread_exit(NULL);
}
//---------Glow LED----------------
void* glowLED(void *arg)
{
 GLArgs *temp = (GLArgs *)arg;
 int pin_num, duration;
 if(temp != NULL)
 {
  pin_num = (int)temp->pin_num;
  duration = (int)temp->duration;
  digitalWrite(pin_num, HIGH);
  delay(duration*1000);
  digitalWrite(pin_num,LOW);
 }
 pthread_exit(NULL);
}
