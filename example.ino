#include <menu.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20,20,4);  // set the LCD address to 0x20 for a 16 chars and 2 line display

#define buttom_esquerda 1
#define buttom_direita  2
#define buttom_click    3

#define ico_ok 1
#define ico_err 2
#define ico_grau 3


#define n_sensor 2
int temp[n_sensor]={
  0,0}; // Valores reais minimos e maximos dos sensores
int s_min[n_sensor]={
  90,10};
int s_max[n_sensor]={
  100,120};


struct parameters_sensor_page {
  int * m;
  int * mx;
  int number;
  int options;
};
char command;
Menu m;
Page pagina1(&enter_pagina1,&paginas_execute,NULL,NULL);
Page pagina2(&enter_pagina2,&paginas_execute,NULL,NULL);
inline void menu_config(){
  m.add(&pagina1);
  m.add(&pagina2);
  m.init(); 
}


void enter_pagina1 (void *p){
  Serial.println("Pagina 1);
}
void enter_pagina2 (void *p){
  Serial.println("Pagina 2);
}

void paginas_execute(void *p){
  if(!command)
    return;
    
  switch(command){
    case 'd':
      m.next();
      break;
    case 'a':
      m.prev();
      break;
  
  }
  command=0;
}

void setup()
{
   menu_config();
}

void loop()
{
  
  if (Serial.available() > 0) {
      command =Serial.read();
   }
   m.execute();
}
