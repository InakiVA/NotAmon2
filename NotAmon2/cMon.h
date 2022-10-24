#include "lista.h"

class Mon {
  private:
    float hp, atk, def, spa, sdf, spe;
    int crit=8;
    string name;
    bool moved;
    vector<float> original;
  public:
    Mon() {
      name="";
      hp=atk=def=spa=sdf=spe=0;
      crit=8;
    }
    Mon(string name, vector<float> v) {
      this->name=name;
      this->hp=v[0];
      this->atk=v[1];
      this->def=v[2];
      this->spa=v[3];
      this->sdf=v[4];
      this->spe=v[5];
      original=v;      
      this->crit=8;
      original.push_back(8);
      this->moved=false;
    }
    float getCurrent(int i){
      switch(i){
        case 0:
          return this->hp;
        case 1:
          return this->atk;
        case 2:
          return this->def;
        case 3:
          return this->spa;
        case 4:
          return this->sdf;
        case 5:
          return this->spe;
        case 6:
          return this->crit;
      }
    }
    float getDef() {
      return this->def;
    }
    float getSdf() {
      return this->sdf;
    }
    float getHp() {
      return this->hp;
    }
    float getSpa() {
      return this->spa;
    }
    float getAtk() {
      return this->atk;
    }
    float getSpe() {
      return this->spe;
    }
    int getCrit() {
      return this->crit;
    }
    string getName() {
      return this->name;
    }
    float getOriginal(int i) {
      return this->original[i];      
    }
    bool getMoved(){
      return this->moved;
    }
    void setMoved(bool b){
      this->moved=b;
    }
    void setHp(float hp) {
      if (hp>original[0]) {
        this->hp=original[0];
        cout<<"It has already reached max hp"<<endl;
      }        
      else
        this->hp=hp;        
    }
    void setAtk(float atk) {
      if (this->atk<original[1]*2.5)
        this->atk=atk;
      else
        cout<<"It has already reached max attack"<<endl;
      if (this->atk<0) {
        this->atk=0;
        cout<<"It has already reached min attack"<<endl;
      }
    }
    void setDef(float def) {
      if (this->def<original[2]*2.5)
        this->def=def;
      else
        cout<<"It has already reached max defense"<<endl;
      if (this->def<0) {
        this->def=0;
        cout<<"It has already reached min defense"<<endl;
      }        
    }    
    void setSpa(float spa) {
      if (this->spa<original[3]*2.5)
        this->spa=spa;
      else
        cout<<"It has already reached max special attack"<<endl;
      if (this->def<0) {
        this->def=0;
        cout<<"It has already reached min special attack"<<endl;
      }
    }
    void setSdf(float sdf) {
      if (this->sdf<original[4]*2.5)
        this->sdf=sdf;
      else
        cout<<"It has already reached max special defense"<<endl;
      if (this->def<0) {
        this->def=0;
        cout<<"It has already reached min special defense"<<endl;
      }
    }
    void setSpe(float spe) {
      if (this->spe<original[5]*2.5)
        this->spe=spe;
      else
        cout<<"It has already reached max speed"<<endl;
      if (this->def<0) {
        this->def=0;
        cout<<"It has already reached min speed"<<endl;
      }
    }
    void setCrit(int i) {
      if (this->crit>2&&this->crit<16)
        this->crit=crit-i;
      else if(this->crit<=2)
        cout<<"It has already reached max critical rate"<<endl;
      else
        cout<<"It has already reached min critical rate"<<endl;
    }
    void setDebuff(int i){
      while (i<1||i>6) {
        cout<<"Invalid input. Enter again: ";
        cin>>i;
      }
      switch(i) {
        case 1:
          cout<<"Attack down!"<<endl;
          setAtk(getAtk()-original[i]/4);
          break;
        case 2:
          cout<<"Defense down!"<<endl;
          setDef(getDef()-original[i]/4);
          break;
        case 3:
          cout<<"Special Attack down!"<<endl;
          setSpa(getSpa()-original[i]/4);
          break;
        case 4:
          cout<<"Special Defense down!"<<endl;
          setSdf(getSdf()-original[i]/4);
          break;
        case 5:
          cout<<"Speed down!"<<endl;
          setSpe(getSpe()-original[i]/4);
          break;
        case 6:
          cout<<"Critical rate down!"<<endl;
          setCrit(-1);
          break;
        }
    }
    void setBuff(int i) {
      while (i<1||i>7) {
        cout<<"Invalid input. Enter again: ";
        cin>>i;
      }
      switch(i) {
        case 1:
          cout<<"HP up!"<<endl;
          setHp(getHp()+original[i-1]/4);
          break;
        case 2:
          cout<<"Attack up!"<<endl;
          setAtk(getAtk()+original[i-1]/4);
          break;
        case 3:
          cout<<"Defense up!"<<endl;
          setDef(getDef()+original[i-1]/4);
          break;
        case 4:
          cout<<"Special Attack up!"<<endl;
          setSpa(getSpa()+original[i-1]/4);
          break;
        case 5:
          cout<<"Special Defense up!"<<endl;
          setSdf(getSdf()+original[i-1]/4);
          break;
        case 6:
          cout<<"Speed up!"<<endl;
          setSpe(getSpe()+original[i-1]/4);
          break;
        case 7:
          cout<<"Critical rate up!"<<endl;
          setCrit(1);
          break;
        }
    }
    Mon Attack(Mon* m, float r) {
      int crt=this->crit;
      crt=rand()%crit;
      float atk2=this->atk;
      float damage;
      atk2*=r;
      if (crt==0) {
        cout<<"Critical hit!"<<endl;
        if(m->getOriginal(2)<m->getDef())
          damage=atk2-m->getOriginal(2);
        else
          damage=atk2-m->getDef();
      }        
      else
        damage=atk2-m->getDef();
      if (damage<1)
        damage=1;
      if (crt==0)
        damage*=2;
      cout<<"\tDamage: "<<damage<<endl;
      m->setHp(m->getHp()-damage); 
      if (m->getHp()<=0) {
        m->setHp(0);
        cout<<'\t'<<m->getName()<<" defeated!"<<endl;
        }      
      return *m;
    }
    Mon SpAttack(Mon* m, float r) {
      int crt=this->crit;
      crt=rand()%crt;
      float spa2=this->spa;
      float damage, orig;
      spa2*=r;
      if (crt==0) {
        cout<<"\tCritical hit!"<<endl;
        if(m->getOriginal(4)<m->getSdf())
          damage=spa2-m->getOriginal(4);
        else 
          damage =spa2-m->getSdf();
      } 
      else 
        damage =spa2-m->getSdf();
      if (damage<1)
        damage=1;
      if (crt==0)
        damage*=2;
      cout<<"\tDamage: "<<damage<<endl;
      m->setHp(m->getHp()-damage); 
      if (m->getHp()<=0){
        m->setHp(0);
        cout<<'\t'<<m->getName()<<" defeated!"<<endl;
        }
      return *m;
    }
    float damage(Mon* m) {
      float da, ds;
      da=this->atk-m->getDef();
      ds=this->spa-m->getSdf();
      return(da>=ds)?da:ds;
    }
    Mon* chooseVictimDebuff(LinkedList<Mon*> list){//checa qué stat es el más buffeado de todos los oponentes. Si no hay ninguno buffeado en lo absoluto, regresa un mon cualquiera
      vector<float> vGod={1000,1000,1000,1000,1000,1000};
      Mon* God = new Mon("God",vGod);
      float f;
      bool changed=false;
      for(int i=0;i<list.size();i++) {
        Mon* temp= list.get(i);
        if(this->name!=temp->getName()){
          for(int j=1;j<6;j++){
            if(temp->getCurrent(j)/temp->getOriginal(j)>God->getCurrent(j)/God->getOriginal(j)){
              changed=true;
              f=temp->getCurrent(j)/temp->getOriginal(j);
              God=temp;
            }
          }   
          if(temp->getOriginal(6)/temp->getCurrent(6)>God->getOriginal(6)/God->getCurrent(6)){//caso de crit, tienen que estar invertidos
              changed=true;
              f=temp->getOriginal(6)/temp->getCurrent(6);
              God=temp;
            }
        }
      }
      if(!changed){
        int r=rand()%list.size(), pos=0;
        God=list.get(r);
        while(God->getName()==this->name){
          r=rand()%list.size();
          God=list.get(r);
        }
      }
      return God;
    }
    Mon* chooseVictim(LinkedList<Mon*> list){      
      vector<float> vGod={1000,1000,1000,1000,1000,1000};
      Mon* God = new Mon("God",vGod);
      for(int i=0;i<list.size();i++) {
        Mon* temp= list.get(i);
        if(this->name==temp->getName())
          continue;
        else {
          if(damage(temp)>=damage(God)){
            if(damage(temp)==damage(God)){
              if(temp->getHp()<God->getHp())
                God=temp;
            }
            else
              God=temp;
          }            
        }
      }
      return God;
    }
    void display() {      
      float crit2=100/(float)getCrit();
      string scrit=to_string(crit2);
      cout<<"Name: "<<getName()<<endl<<"\tHP: "<<getHp()<<endl<<"\tAtk: "<<getAtk()<<endl<<"\tDef: "<<getDef()<<endl<<"\tSpAtk: "<<getSpa()<<endl<<"\tSpDef: "<<getSdf()<<endl<<"\tSpe: "<<getSpe()<<endl<<"\tCritical rate: "<<scrit<<"%"<<endl;
    }
};