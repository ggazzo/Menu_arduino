#include <menu.h>

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
