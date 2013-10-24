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

// Menu ->
//   main -------- config sensor 1 ------------ config sensor 2
//                       |                            |
//                     editar                       editar
Menu m;
Page p(&mainMenu,&main_execute,NULL,&mainRefresh);

Page config1(&page,&main_execute,NULL,NULL);
Page configSensors1(&enterConfigSensor,&configExecute,&configLeave,&configRefresh);
Page editSensor1(NULL,&editExecute,NULL,NULL);
parameters_sensor_page sensor1;

Page config2(&page,&main_execute,NULL,NULL);
Page configSensors2(&enterConfigSensor,&configExecute,&configLeave,&configRefresh);
Page editSensor2(NULL,&editExecute,NULL,NULL);
parameters_sensor_page sensor2;
//END-menu

void setup()
{
  lcd.init();
  lcd.backlight();
  carecteres_config();
  menu_config();
  attachInterrupt(1, selects, FALLING );
  attachInterrupt(0, buttom, CHANGE);
}

void loop()
{
  read_sensor();
  m.execute();
}
long counter_sensor;
void read_sensor(){
  if(millis() - counter_sensor < 300 *20)
    return;
  for(int i = 0 ; i < n_sensor ; i++){
    temp[i] = random(-40,30);
  }
  counter_sensor = millis();
}
inline void carecteres_config(){
#ifdef ico_ok
  uint8_t ok[8] = {
    0x0,0x1,0x3,0x16,0x1c,0x8,0x0  };
  lcd.createChar(ico_ok, ok);
#endif
#ifdef ico_err
  uint8_t err[8] = {
    0x0,0x1b,0xe,0x4,0xe,0x1b,0x0  };
  lcd.createChar(ico_err, err);
#endif
#ifdef ico_grau
  byte grau[8] = {
    B11100,
    B10100,
    B11100,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000     };
  lcd.createChar(ico_grau, grau);
#endif

}

inline void menu_config(){
  sensor1.m = &s_min[0];
  sensor1.mx = &s_max[0];  
  sensor2.m = &s_min[1];
  sensor2.mx = &s_max[1];
  sensor1.number = 1;
  sensor2.number = 2;
  sensor1.options = 0;
  sensor2.options = 0;  
  m.add(&p);
  m.add(&config1);
  m.add(&config2);
  config1.setArgs(&sensor1);
  config2.setArgs(&sensor2);
  configSensors1.setArgs(&sensor1);
  configSensors2.setArgs(&sensor2);
  editSensor1.setArgs(&sensor1);
  editSensor2.setArgs(&sensor2);  
  config1.addSubMenu(&configSensors1);  
  config2.addSubMenu(&configSensors2);    
  configSensors1.addSubMenu(&editSensor1);  
  configSensors2.addSubMenu(&editSensor2);    
  m.init(); 
}
//                       Buttons
int decoder;
long debounce;
void buttom()
{
  if(millis() - debounce > 100){
    if (digitalRead(4) == digitalRead(2))
      decoder = buttom_direita; //if encoder channels are the same, direction is CW
    else
      decoder = buttom_esquerda; //if they are not the same, direction is CCW
    debounce = millis(); 
  } 
}

void selects(void){ 
  if(millis() - debounce > 100){
    while(digitalRead(4)!=1);
    decoder = buttom_click;
    debounce = millis(); 
  }
}


//    Menu



void mainMenu(void* p)
{
  lcd.clear();
  lcd.print("       Status       ");
  lcd.setCursor(0,1);
  lcd.print(" 1: 0.0");
  lcd.write(ico_grau);
  lcd.setCursor(0,2);
  lcd.print(" 2: 0.0");
  lcd.write(ico_grau);
}
void main_execute(void* p){
  if(!decoder)
    return;
  switch(decoder){    
  case buttom_direita:
    m.next();
    break;
  case buttom_esquerda:
    m.prev();
    break;
  case buttom_click:
    m.enter();
    break;
  }
  decoder = 0;
}
void mainRefresh(void* p){

  lcd.setCursor(3,1);
  if(temp[0] >= 0)
    lcd.print(" ");      
  lcd.print((float)temp[0]/10,1);
  lcd.write(ico_grau);
  printStatus(0);
  lcd.setCursor(3,2);
  if(temp[1] >= 0)
    lcd.print(" ");      
  lcd.print((float) temp[1]/10,1);
  lcd.write(ico_grau);
  printStatus(1);

}
void printStatus(int i){
  if(s_min[i] > temp[i] || temp[i]  > s_max[i] ){
    lcd.print(" "); 
    lcd.write(ico_err);
  }
  else{      
    lcd.print(" "); 
    lcd.write(ico_ok);  }

}
void page(void* p)
{
  parameters_sensor_page s = *(parameters_sensor_page*)p;
  lcd.setCursor(0,0);
  lcd.print("<   Configuracao   >");
  lcd.setCursor(0,1);
  lcd.print("Sensor:");
  lcd.print(s.number,DEC);
  lcd.setCursor(8,1);
  lcd.print("           ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
}

void configLeave(void* p)
{

}

void configRefresh(void* p)
{


  //lcd.clear();
}


void enterConfigSensor(void* p)
{
  parameters_sensor_page s = *(parameters_sensor_page*)p;
  lcd.setCursor(13,1);  
  lcd.print("voltar");  
  lcd.setCursor(0,2);
  lcd.print(" min            max ");
  lcd.setCursor(0,3);

  if(*s.m > 0)
    lcd.print(" ");   


  lcd.print((float)*s.m/10,1);
  lcd.write(ico_grau);
  lcd.setCursor(15,3);
  if(*s.m > 0)
    lcd.print(" ");   

  lcd.print((float)*s.mx/10,1);
  lcd.write(ico_grau);
}

void configExecute(void* p)
{
  parameters_sensor_page * s = (parameters_sensor_page*)p;
  if(!decoder && s->options)
    return;
  switch(decoder){    
  case buttom_direita:

    s->options = ++s->options > 3? 1:s->options;

    break;
  case buttom_esquerda:

    s->options = --s->options <= 0 ? 3:s->options;

    break;
  case 3:
    decoder = 0;
    if(s->options == 3){
      m.back();  
      s->options =0 ;
    }
    else
      m.enter();
    return; 
    break;  
  case 4:
    m.back();
    return;    
    break;
  }
  decoder = 0;

  lcd.setCursor(12,1); 
  lcd.print(" ");
  lcd.setCursor(0,2); 
  lcd.print(" ");
  lcd.setCursor(15,2); 
  lcd.print(" ");

  switch(s->options){
  case 1:
    lcd.setCursor(0,2); 
    lcd.print(">");
    break;
  case 2  :
    lcd.setCursor(15,2); 
    lcd.print(">");     
    break;
  default:
    s->options = 3;
    lcd.setCursor(12,1); 
    lcd.print(">");

  }
}

void editExecute(void* p)
{
  parameters_sensor_page * s = (parameters_sensor_page*)p;
  if(!decoder)
    return;
  switch(decoder){    
  case buttom_direita:
    if(s->options == 1)
      *s->m += 1;      
    else
      *s->mx += 1;      
    break;
  case buttom_esquerda:
    if(s->options == 1)
      *s->m-=1;      
    else
      *s->mx-=1;      
    break;
  case 3:
    m.back();  
    decoder = 0;
    return;    
    break;
  }

  decoder = 0;

  if(s->options == 1){
    lcd.setCursor(0,3);
    if(*s->m > 0)
      lcd.print(" ");   
    lcd.print((float)*s->m/10,1);
  }  
  else{
    lcd.setCursor(15,3); 
    if(*s->mx > 0)
      lcd.print(" ");
    lcd.print((float)*s->mx/10,1);      
  }
  lcd.print(" ");
}








