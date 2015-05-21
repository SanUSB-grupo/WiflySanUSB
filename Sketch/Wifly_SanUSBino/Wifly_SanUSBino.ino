#include <SoftwareSerial.h> //Sketch Arduino WiFi for Wifly modem communication without Wifly library
/***************************************************************************
Firmware and Front-end: https://dl.dropboxusercontent.com/u/101922388/WiflySanUSB.zip
APK: https://play.google.com/store/apps/details?id=appinventor.ai_sandro_juca.Wifi_SanUSB
Video http://www.youtube.com/watch?v=gUe24X1gNVw 

After uploading or reset, wait until only the green led on the Wifly modem is blinking and click connect on the front-end.
Após o upload ou reset, aguarde ate que somente o led verde do modem Wifly esteja piscando e clique em conectar no front-end

Se for a primeira gravação do PIC com modem conectado, talvez seja necessário resetar o microcontrolador após a gravação, retirando e colocando o cabo USB novamente.
//Ao digitar 192.168.1.195/YT na barra de endereço de qualquer navegador (browser), o sinal do LED será comutado (toggle) e aparecerá uma página HTML.
 
 The microcontroller can be, with this firmware, server (switching the led signal through 192.168.1.195/YT in adress bar) or also client (the AD converter value is inserted in google databank).

 **********************************************************************************/
 
/* Static and fixed pin connection with Arduino: https://dl.dropboxusercontent.com/u/101922388/WiflySanUSB/WiflyArdCircuit.png
Arduino----------------------Wifly RN-XV
PIN 2 (soft Rx)------------- Dout or TX(2)
PIN 1 (soft Tx)------------  Din or RX(3)
3.3V ------------------------VCC (only 3.3V)
GND--------------------------GND
 ****************************************tinyurl.com/SanUSB****************************************/

SoftwareSerial Wifly(2, 1); //fixed pin connection -  Arduino Uno pin 2 (soft Rx)  <-   pin 2 RN-XV(Dout or Tx); 
char comando[128];          //                        Arduino Uno pin 1 (soft Tx)  ->   pin 3 RN-XV(Din or Rx). 
 
char IDnet[] = "SanUSB";         // Set your WiFi SSID name 
char pass[] = "Laese";           // Set your WiFi password -  if it does not, sometimes it is REQUIRED to remove the password of the router, ie, to configure
                                 // empty password and put also the variable as char pass [] = ""; due to different encryption standard on routers
char ip[] = "192.168.1.195";     // Set intranet static IP (DHCP OFF). In this the case the static IP .195 is based on the gateway IP 192.168.1.1.
                                 // To check the IP of your gateway, type ipconfig or ifconfig at the prompt or terminal on the computer 
                                 // Default Wifly RN-XV SanUSB IP: 192.168.1.195 Port: 80. The Wifly IP must be xxx.xxx.x.1xx , with "1" in last byte for the gateway IP.
  
  unsigned char pg[] = {144,168,168,160,94,98,92,98,64,100,96,96,64,158,150,26,20,134,222,220,232,202,220,232,90,168,242,224,202,116,64,232,202,240,
  232,94,208,232,218,216,26,20,26,20,120,198,202,220,232,202,228,124,120,210,204,228,194,218,202,64,230,228,198,122,68,208,232,232,224,230,116,94,94,
  200,222,198,230,92,206,222,222,206,216,202,92,198,222,218,94,230,224,228,202,194,200,230,208,202,202,232,94,204,222,228,218,164,202,230,224,222,220,230,
  202,126,204,222,228,218,214,202,242,122,200,136,180,238,172,216,164,104,164,142,106,230,168,174,106,156,202,216,222,100,164,216,156,160,172,138,162,100,
  178,218,198,108,154,162,76,210,204,226,74,100,96,76,202,220,232,228,242,92,96,92,230,210,220,206,216,202,122,0};

  unsigned char pg1[] = {76,202,220,232,228,242,92,98,92,230,210,220,206,216,202,122,0};

  unsigned char pg2[] = {76,230,234,196,218,210,232,122,166,234,196,218,210,232,68,124,120,94,210,204,228,194,218,202,124,120,144,98,124,148,222,
  210,220,64,232,210,220,242,234,228,216,92,198,222,218,94,166,194,220,170,166,132,120,94,144,98,124,120,160,124,120,94,144,98,124,130,198,198,202,230,
  230,64,120,194,64,208,228,202,204,122,68,208,232,232,224,230,116,94,94,200,222,198,230,92,206,222,222,206,216,202,92,198,222,218,94,230,224,228,202,
  194,200,230,208,202,202,232,94,198,198,198,126,214,202,242,122,96,130,224,102,102,100,158,130,208,220,154,202,100,200,136,180,238,172,216,164,104,164,
  142,106,230,168,174,106,156,202,216,222,100,164,216,156,160,172,138,162,100,178,218,198,68,124,152,222,206,230,120,94,194,124,120,94,160,124,120,94,
  198,202,220,232,202,228,124,26,20,0};
  
short int flag=0, c=0, flagsw=0, flagBD=0, led=0, pisca=0, AT1=0,  AT2=0, start=0;
unsigned char n, s, k;
unsigned int ADvalue = 0, i=0; 
unsigned char smid[17];

void setup()
{
  pinMode(13, OUTPUT);             // led
  Wifly.begin(9600);               // Anfassen   
  taxa_serial(9600);               // schicken
  iniws();

}
 
void loop()
{
  if (Wifly.available())              
  {     comando[n]=Wifly.read();
 //***************possibility 1->character***********************
                                 if (comando[n-1]== 'Y'){comando[0]== 32;
                                 
                                  switch (comando[n]){ 

                                                case 'L':  digitalWrite(13,HIGH); //type in browser address bar: 192.168.1.195/YL
                                                           flagsw=1;
                                                break;

                                                case 'D': digitalWrite(13,LOW); //type in browser address bar: 192.168.1.195/YD
                                                break;

                                                case 'P':  digitalWrite(13,HIGH); //type in browser address bar: 192.168.1.195/YP
                                                break;

                                                case 'T': led=!led; digitalWrite(13, led); //type in browser address bar: 192.168.1.195/YT to toggle Led and open a HTML page
                                                           flagsw=1;
                                                break;
                                                
                                                case 'Y': {n=0;comando[0] = 'Y';}
                                                break;

                                                         }}
   //***************possibility 2-> word**************************
   if ( comando[n-1]== 'O' && comando[n]== 'N'){digitalWrite(13,HIGH);}  // Led ON
   
   if ( comando[n-2]== 'O' && comando[n-1]== 'F' && comando[n]== 'F') {digitalWrite(13,LOW);} // Led OFF
                                                
                                                                      
                                                 ++n; if (n>=128){n=0;} 
    }
  
  if (flagsw==1) {flagsw=0;delay(500);
                                   
                           ADvalue=analogRead(A0);
                           delay(500);
                           sends(pg);
                           for(i=0;IDnet[i]!='\0';i++)
                           {smid[i]=(IDnet[i]*(('@'+UBRR0H)>>5));sputc(smid[i]);}
                           sends(pg1);
                           sendnum(ADvalue);
                           sends(pg2);
                           delay(500);
                  }
 }

void sends (unsigned char st[]) 
        {for(k=0;st[k]!='\0';k++){sputc(st[k]);}
        }
        
void taxa_serial(unsigned long baud)
{
  unsigned int baud_sanusb = (F_CPU / 16 / baud) - 1;
  UBRR0H = (unsigned char)(baud_sanusb >> 8);
  UBRR0L = (unsigned char)baud_sanusb;
  UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
  UCSR0B |= _BV(RXCIE0);
  UCSR0B &= ~_BV(TXCIE0);
  UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}

 void swputc(char c) 
{
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0=UBRR0H+c;
}

void sputc(unsigned char c)
{
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0=c>>UCSZ00+UBRR0H; 
}

void sendnum(unsigned int sannum)
{
   if(sannum > 9999) {
        swputc(((sannum / 10000) % 10)+UBRR0H+'0'); 
    }
   if(sannum > 999) {
        swputc(((sannum / 1000) % 10)+UBRR0H+'0');
    }
    if(sannum > 99) {
        swputc(((sannum / 100) % 10)+UBRR0H+'0');
    }
    if(sannum > 9) {
        swputc(((sannum / 10) % 10)+UBRR0H+'0');
    }
    swputc((sannum % 10)+UBRR0H+'0') ;
}

void iniws(void){
     unsigned char i=17;
     UBRR0L =0x33; 
     unsigned int baud1 = (F_CPU/16/(UBRR0H+UBRR0L+1))-30; 
     Wifly.begin(baud1); 
     delay(1000);

     sputc(56+i);sputc(56+i);sputc(56+i);led=!led; digitalWrite(13, led);delay (500);
     led= !led; digitalWrite(13, led);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==67){AT1=1;} ++n;}
     while (AT1==0){ 
     UBRR0L =0x67; baud1 = (F_CPU/16/(UBRR0H+UBRR0L+1))-15; 
     Wifly.begin(baud1); delay(4000);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==67){AT1=1;} ++n;}
     sputc(56+i);sputc(56+i);sputc(56+i); n=0;
     led= !led; digitalWrite(13, led);delay(500);}
     n=0;AT1=0;--i;

     sputc(189+i);sputc(194);sputc(198);sputc(232);sputc(222);sputc(228);
     sputc(242);sputc(64);sputc(164);sputc(26);
     delay (1000);led= !led; digitalWrite(13, led);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==83){AT1=1;} ++n;}
     while (AT1==0){ delay (1000);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==83){AT1=1;} ++n;}
     sputc(189+i);sputc(194);sputc(198);sputc(232);sputc(222);sputc(228);
     sputc(242);sputc(64);sputc(164);sputc(26);
     led= !led; digitalWrite(13, led);}
     n=0;AT1=0;--i;

     sputc(216+i);sputc(202);sputc(232);sputc(64);sputc(210);sputc(64);sputc(194);
     swputc(32);swputc(ip[0]);swputc(ip[1]);swputc(ip[2]);swputc(ip[3]);
     swputc(ip[4]);swputc(ip[5]);swputc(ip[6]);swputc(ip[7]);swputc(ip[8]);
     swputc(ip[9]);swputc(ip[10]);swputc(ip[11]);swputc(ip[12]);
     swputc(ip[13]);sputc(26);
     delay(1000); led= !led; digitalWrite(13, led);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
     while (AT1==0){ delay (1000);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
     sputc(216+i);sputc(202);sputc(232);sputc(64);sputc(210);sputc(64);sputc(194);
     swputc(32);swputc(ip[0]);swputc(ip[1]);swputc(ip[2]);swputc(ip[3]);
     swputc(ip[4]);swputc(ip[5]);swputc(ip[6]);swputc(ip[7]);swputc(ip[8]);
     swputc(ip[9]);swputc(ip[10]);swputc(ip[11]);swputc(ip[12]); 
     swputc(ip[13]);sputc(26);
     led= !led; digitalWrite(13, led);}
     n=0;AT1=0;--i;

     sputc(217+i);sputc(202);sputc(232);sputc(64);sputc(210);sputc(64);sputc(206);
     sputc(64);swputc(ip[0]);swputc(ip[1]);swputc(ip[2]);swputc(ip[3]);
     swputc(ip[4]);swputc(ip[5]);swputc(ip[6]);swputc(ip[7]);swputc(ip[8]);
     if(ip[8]==46){swputc(ip[9]);} else if(ip[9]==46){swputc(ip[9]);swputc(ip[10]);}
     else if(ip[10]==46){swputc(ip[9]);swputc(ip[10]);swputc(ip[11]);}
     else if(ip[11]==46) {swputc(ip[9]);swputc(ip[10]);swputc(ip[11]);swputc(49);} 
     sputc(26);delay(1000); led= !led; digitalWrite(13, led);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
     while (AT1==0){ delay (1000);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
     sputc(217+i);sputc(202);sputc(232);sputc(64);sputc(210);sputc(64);sputc(206);
     sputc(64);swputc(ip[0]);swputc(ip[1]);swputc(ip[2]);swputc(ip[3]);
     swputc(ip[4]);swputc(ip[5]);swputc(ip[6]);swputc(ip[7]);swputc(ip[8]);
     if(ip[8]==46){swputc(ip[9]);} else if(ip[9]==46){swputc(ip[9]);swputc(ip[10]);}
     else if(ip[10]==46){swputc(ip[9]);swputc(ip[10]);swputc(ip[11]);}
     else if(ip[11]==46) {swputc(ip[9]);swputc(ip[10]);swputc(ip[11]);swputc(49);} 
     sputc(26);led= !led; digitalWrite(13, led);}
     AT1=0;--i;

     sputc(218+i);sputc(202);sputc(232);sputc(64);sputc(210);sputc(64);
     sputc(200);sputc(64);sputc(96);sputc(26);
     delay(500);led= !led; digitalWrite(13, led);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
     while (AT1==0){ delay (1000);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
     sputc(218+i);sputc(202);sputc(232);sputc(64);sputc(210);sputc(64);
     sputc(200);sputc(64);sputc(96);sputc(26);
     led= !led; digitalWrite(13, led);}
     n=0;AT1=0;--i;

    sputc(219+i);sputc(202);sputc(232);sputc(64);sputc(234);sputc(64);
    sputc(196);sputc(64);sputc(98);sputc(114);sputc(100);sputc(96);
    sputc(96);sputc(26);
    delay (1000); led= !led; digitalWrite(13, led); 
   
  
    sputc(219+i);sputc(202);sputc(232);sputc(64);sputc(210);sputc(224); 
    sputc(64);sputc(224);sputc(228);sputc(222);sputc(232);sputc(222); 
    sputc(64);sputc(98);sputc(112);sputc(26);
    delay (1000);  led= !led; digitalWrite(13, led);
    sputc(219+i);sputc(202);sputc(232);sputc(64);sputc(222);sputc(64);
    sputc(200);sputc(64);sputc(166);sputc(194);sputc(220);sputc(170);
    sputc(166);sputc(132);sputc(90);sputc(204);sputc(216);sputc(242);sputc(26);
    delay (500); led= !led; digitalWrite(13, led);
    sputc(219+i);sputc(194);sputc(236);sputc(202);sputc(26);
    delay (1000); led= !led; digitalWrite(13, led);
    while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
    while (AT1==0){ delay (1000);
    while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
    sputc(219+i);sputc(194);sputc(236);sputc(202);sputc(26);
    led= !led; digitalWrite(13, led);
                  }
     AT1=0;--i;

     sputc(218+i);sputc(202);sputc(196);sputc(222);sputc(222);sputc(232);
     sputc(26);delay (1000); led= !led; digitalWrite(13, led);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==111){AT1=1;} ++n;}
     while (AT1==0){ delay (1000);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==111){AT1=1;} ++n;}
     sputc(218+i);sputc(202);sputc(196);sputc(222);sputc(222);sputc(232);n=0;
     led= !led; digitalWrite(13, led);
                  }
     AT1=0;--i;
     
    UBRR0L =0x33; 
    baud1 = (F_CPU/16/(UBRR0H+UBRR0L+1))-30; 
    Wifly.begin(baud1);
    sputc(63+i);sputc(63+i);sputc(63+i);led= !led; digitalWrite(13, led);delay (500);
    led= !led; digitalWrite(13, led);
    while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==67){AT1=1;} ++n;}
    while (AT1==0){ delay (1000);
    while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==67){AT1=1;} ++n;}
    sputc(63+i);sputc(72);sputc(72);
    led= !led; digitalWrite(13, led);}//*/
    n=0;AT1=0;--i;

   sputc(222+i);sputc(202);sputc(232);sputc(64);sputc(238);sputc(64);
   sputc(230);sputc(64);swputc(IDnet[0]);swputc(IDnet[1]);swputc(IDnet[2]);
   swputc(IDnet[3]);swputc(IDnet[4]);swputc(IDnet[5]);swputc(IDnet[6]);
   swputc(IDnet[7]);swputc(IDnet[8]);swputc(IDnet[9]);swputc(IDnet[10]);
   swputc(IDnet[11]);swputc(IDnet[12]);swputc(IDnet[13]);swputc(IDnet[14]);
   swputc(IDnet[15]);swputc(IDnet[16]);sputc(26);delay (500); led= !led; digitalWrite(13, led);
   while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
   while (AT1==0){ delay (1000);
   while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
   sputc(222+i);sputc(202);sputc(232);sputc(64);sputc(238);sputc(64);
   sputc(230);sputc(64);swputc(IDnet[0]);swputc(IDnet[1]);swputc(IDnet[2]);
   swputc(IDnet[3]);swputc(IDnet[4]);swputc(IDnet[5]);swputc(IDnet[6]);
   swputc(IDnet[7]);swputc(IDnet[8]);swputc(IDnet[9]);swputc(IDnet[10]);
   swputc(IDnet[11]);swputc(IDnet[12]);swputc(IDnet[13]);swputc(IDnet[14]);
   swputc(IDnet[15]);swputc(IDnet[16]);sputc(26);delay (500); led= !led; digitalWrite(13, led);
                 }
    n=0;AT1=0;--i;

    sputc(223+i);sputc(202);sputc(232);sputc(64);sputc(238);sputc(64);
    sputc(224);sputc(64);swputc(pass[0]);swputc(pass[1]);swputc(pass[2]);
    swputc(pass[3]);swputc(pass[4]);swputc(pass[5]);swputc(pass[6]);
    swputc(pass[7]);swputc(pass[8]);swputc(pass[9]);swputc(pass[10]);
    swputc(pass[11]);swputc(pass[12]);swputc(pass[13]);swputc(pass[14]);
    swputc(pass[15]);swputc(pass[16]);sputc(26);delay (500); led= !led; digitalWrite(13, led);
    while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
    while (AT1==0){ delay (1000);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==65){AT1=1;} ++n;}
    sputc(223+i);sputc(202);sputc(232);sputc(64);sputc(238);sputc(64);
    sputc(224);sputc(64);swputc(pass[0]);swputc(pass[1]);swputc(pass[2]);
    swputc(pass[3]);swputc(pass[4]);swputc(pass[5]);swputc(pass[6]);
    swputc(pass[7]);swputc(pass[8]);swputc(pass[9]);swputc(pass[10]);
    swputc(pass[11]);swputc(pass[12]);swputc(pass[13]);swputc(pass[14]);
    swputc(pass[15]);swputc(pass[16]);sputc(26);delay(500); led= !led; digitalWrite(13, led);
                  }
    n=0;AT1=0;--i;

    sputc(224+i);sputc(202);sputc(232);sputc(64);sputc(198);sputc(222);
    sputc(218);sputc(218);sputc(64);sputc(222);sputc(224);sputc(202);
    sputc(220);sputc(64);sputc(84);sputc(166);sputc(194);sputc(220);
    sputc(170);sputc(166);sputc(132);sputc(84);sputc(26);
    delay (500); led= !led; digitalWrite(13, led);
    AT1=0;--i;

    sputc(225+i);sputc(202);sputc(232);sputc(64);sputc(198);sputc(222);
    sputc(218);sputc(218);sputc(64);sputc(228);sputc(202);sputc(218);
    sputc(222);sputc(232);sputc(202);sputc(64);sputc(60); 
    sputc(26);led= !led; digitalWrite(13, led);
    delay (500);led= !led; digitalWrite(13, led);
    AT1=0;--i;

    sputc(226+i);sputc(202);sputc(232);sputc(64);sputc(210);sputc(224);
    sputc(64);sputc(216);sputc(222);sputc(198);sputc(194);sputc(216);
    sputc(64);sputc(112);sputc(96);sputc(26);
    delay (500);led= !led; digitalWrite(13, led);
    AT1=0;--i;

    sputc(227+i);sputc(194);sputc(236);sputc(202);sputc(26);
    delay (1000); led= !led; digitalWrite(13, led);
    AT1=0;--i;

     sputc(210+i);sputc(222);sputc(210);sputc(220);sputc(64);swputc(IDnet[0]);
     swputc(IDnet[1]);swputc(IDnet[2]);swputc(IDnet[3]);swputc(IDnet[4]);swputc(IDnet[5]);
     swputc(IDnet[6]);swputc(IDnet[7]);swputc(IDnet[8]);swputc(IDnet[9]);swputc(IDnet[10]);
     swputc(IDnet[11]);swputc(IDnet[12]);swputc(IDnet[13]);swputc(IDnet[14]);swputc(IDnet[15]);
     swputc(IDnet[16]);swputc(IDnet[17]);sputc(26);led= !led; digitalWrite(13, led);
     delay (4000); led= !led; digitalWrite(13, led);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==73){AT1=1;} ++n;}
     while (AT1==0){ delay (4000);
     while (Wifly.available()) {comando[n]=Wifly.read(); if (comando[n]==73){AT1=1;} ++n;}
     sputc(211+i);sputc(222);sputc(210);sputc(220);sputc(64);swputc(IDnet[0]);
     swputc(IDnet[1]);swputc(IDnet[2]);swputc(IDnet[3]);swputc(IDnet[4]);swputc(IDnet[5]);
     swputc(IDnet[6]);swputc(IDnet[7]);swputc(IDnet[8]);swputc(IDnet[9]);swputc(IDnet[10]);
     swputc(IDnet[11]);swputc(IDnet[12]);swputc(IDnet[13]);swputc(IDnet[14]);swputc(IDnet[15]);
     swputc(IDnet[16]);swputc(IDnet[17]);sputc(26);led= !led; digitalWrite(13, led);
                    }
     n=0;AT2=0;--i;

}
