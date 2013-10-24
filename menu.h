#ifndef MENU_ARDUINO
#define MENU_ARDUINO

#include "Arduino.h"

typedef void (* Event)(void*); //function pointer prototype to a function which takes an 'int' an returns 'void'


struct Events {   
  Event *enter;      
  Event *execute;      
  Event *leave;      
  Event *refresh;      
}; 


class Page{
private:
  Event onEnter;
  Event onExecute;
  Event onLeave;
  Event onRefresh;
public:
  int option;
  Page(Event e, Event a, Event b,Event c){
    this->onEnter = e;
    this->onExecute = a;
    this->onLeave = b;
    this->onRefresh = c;      
  };
  void enter(void){
    if(this->onEnter)this->onEnter(args);
  };

  void leave(void){
    if(this->onLeave)this->onLeave(args);
  };  
  void execute(void){
    if(this->onExecute)this->onExecute(args);
  };      
  void refresh(void){
    if(this->onRefresh)this->onRefresh(args);
  };  
  void setArgs(void * args){

    this->args = args;
  };    
  void add(Page * p){
    if(!this->right){
      this->right = p;
      p->left = this;
    }
    else
      this->right->add(p);
  };
  void addSubMenu(Page * p){
    p->parent = this;
    if(!this->child)
      this->child = p;
    else
      this->child->add(p);
  };
  void * args;
  Page * right;
  Page * left;
  Page * parent;
  Page * child;    

}; 
class Menu{
  Page * atual;

  long time_refresh;
public:
  Page * root;
  void add(Page * p){
    if(!this->root){
      this->root = p;
      this->atual = p;
      return;
    }
    this->atual->add(p);
  } 
  void addSubMenu(Page * p){
    this->atual->addSubMenu(p);
  } 


  void execute()
  {

    this->atual->execute();
    if(millis() - time_refresh < 500)
      return;
    this->atual->refresh();
    time_refresh = millis();
  }
  void init(void)
  {
    this->atual = this->root;
    this->atual->enter();
  }

  void enter(void){
    if(!this->atual->child)
      return;
    this->atual = this->atual->child;
    this->atual->enter();       

  }
  void back(void){
    if(!this->atual->parent)
      return;
    this->atual->leave();      
    this->atual = this->atual->parent;
    this->atual->enter();

  }
  void next(void){
    if(!this->atual->right)
      return;
    this->atual = this->atual->right;
    this->atual->enter();       
  }

  void prev(void){
    if(!this->atual->left)
      return;
    this->atual = this->atual->left;
    this->atual->enter();


  }
};


#endif
