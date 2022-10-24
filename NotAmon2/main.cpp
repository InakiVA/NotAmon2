#include "cMon.h"
#include "time.h"

bool isNumber(string s){
  bool b=true;
  for(int i=0;i<s.length();i++){
    if(!isdigit(s.at(i)))
      return false;
  }
  return true;
}

float makeR(){
  float r=80+rand()%41;
  r/=100;
  return r;
}

void chooseAttack(Mon* Attacker, Mon* Defender, float r) {
  if (Attacker->getAtk()-Defender->getDef()>Attacker->getSpa()-Defender->getSdf()){
    cout<<Attacker->getName()<<" attacks "<<Defender->getName()<<"!"<<endl;
    Attacker->Attack(Defender,r);
  }    
  else {
    cout<<Attacker->getName()<<" special attacks "<<Defender->getName()<<"!"<<endl;
    Attacker->SpAttack(Defender,r);
  }    
  if (Defender->getHp()>0)
    cout<<"\t"<<Defender->getName()<<" HP left: "<<Defender->getHp()<<endl;
}

void chooseBuff(Mon* Buffer, Mon* Compare) {
  cout<<Buffer->getName()<<" buffs: ";
  if (Buffer->getHp()<=Buffer->getOriginal(0)/3)
    Buffer->setBuff(1);
  else if (Buffer->getHp()<=Buffer->getOriginal(0)/2) {
    if (Buffer->getDef()-Compare->getAtk()>Buffer->getSdf()-Compare->getSpa())
      Buffer->setBuff(3);
    else if (Buffer->getDef()-Compare->getAtk()<=Buffer->getSdf()-Compare->getSpa())
      Buffer->setBuff(5);
  }
  else if (Buffer->getHp()<=Buffer->getOriginal(0)*2/3) {
    if (Buffer->getAtk()>Buffer->getSpa())
      Buffer->setBuff(2);
    else
      Buffer->setBuff(4);
  }
  else {
    if (Buffer->getSpe()<Compare->getSpe())
      Buffer->setBuff(6);
    else
      Buffer->setBuff(7);
  }  
  Buffer->display();
}

void chooseDebuff(Mon* Attacker, Mon* Victim){
  cout<<Attacker->getName()<<" debuffs "<<Victim->getName()<<": ";
  if(Attacker->getHp()<Attacker->getOriginal(0)/3){
    if(Attacker->getDef()-Victim->getAtk()>=Attacker->getSdf()-Victim->getSpa())
      Victim->setDebuff(1);
    else
      Victim->setDebuff(3);
  }
  else if(Attacker->getHp()<Attacker->getOriginal(0)/2){
    if(Attacker->getAtk()-Victim->getDef()>=Attacker->getSpa()-Victim->getSdf())
      Victim->setDebuff(2);
    else
      Victim->setDebuff(4);
    }
  else if(Victim->getCrit()<8)
    Victim->setDebuff(6);
  else if(Victim->getSpe()>Attacker->getSpe())
    Victim->setDebuff(5);
  else {
    int pos;
    float temp=0;
    bool changed=false;
    for(int i=1;i<6;i++){
      if(temp<Victim->getCurrent(i)/Victim->getOriginal(i)){
        temp=Victim->getCurrent(i)/Victim->getOriginal(i);
        pos=i;
        changed=true;
      }
    }
    if(temp<8/Victim->getCurrent(6)){//caso de crit, tienen que estar invertidos
        temp=8/Victim->getCurrent(6);//8 porque todos tienen mismo original
        pos=6;
        changed=true;
      }
    if(!changed){
      pos=1+rand()%6;
    }      
    Victim->setDebuff(pos);
    }
  Victim->display();
}

LinkedList<Mon*> sortSpeed(LinkedList<Mon*> list){
  LinkedList<Mon*> lspeed; 
  int k=0;
  for(int i=0; i<list.size();i++){
    if(k!=lspeed.size()){
      k=0;
      for(int j=0;j<lspeed.size();j++){
        if(list.get(i)->getSpe()<lspeed.get(j)->getSpe()) 
          k++; 
      } 
      if(k==lspeed.size())
        lspeed.addLast(list.get(i));
      else if (k==0)
        lspeed.addFirst(list.get(i));
      else
        lspeed.insert(list.get(i),k);
    } 
    else 
      lspeed.addLast(list.get(i));
  }  
  return lspeed;
}

void resetMove(LinkedList<Mon*> list, bool b){
  for(int i=0;i<list.size();i++)
    list.get(i)->setMoved(b);
}

bool allMoved(LinkedList<Mon*> list){
  bool all=true;
  for(int i=0;i<list.size();i++)
    all*=list.get(i)->getMoved();
  return all;
}

int main() {
  srand(time(NULL));
  
  LinkedList<Mon*> characters, lspeed, podium;
  vector<float> vB, vC, vS, vP, vJ;
  vB={100,40,25,50,25,20};//hp, atk, def, spa, sdf, spe
  vC={80,55,20,60,15,30};//260 de distribución
  vS={90,60,35,40,10,25};
  vP={90,50,10,60,10,40};
  vJ={110,30,30,30,30,30};
  
  Mon* Bulb = new Mon("Bulbosaur",vB);
  Mon* Char = new Mon("Charmando",vC);
  Mon* Squr = new Mon("Squirel",vS);
  Mon* Pika = new Mon("Pitachu",vP);  
  Mon* Jigy = new Mon("Pigglypuff",vJ);
  
  characters.addLast(Bulb);
  characters.addLast(Char);
  characters.addLast(Squr);  
  characters.addLast(Pika);  
  characters.addLast(Jigy);
  
  lspeed=sortSpeed(characters);

  string play="", sturn="";
  cout<<"This game is a battle royale, where every member takes turns choosing what to do (attack, buff, etc.). The first characters to move will be those with the highest speed stat. Last one standing wins."<<endl;
  while(play!="1"&&play!="2"){
    cout<<"\t1) Play\n\t2) Autoplay\n\t";
    cin>>play;
  }
  int iplay=stoi(play);
  int iline=0;
  if(iplay==1){
    while(sturn!="1"&&sturn!="2"&&sturn!="3"){
      cout<<"Show:\n\t1) Line by line\n\t2) End of movement\n\t3) End of turn\n\t";
      cin>>sturn;
    }
    iline=stoi(sturn);
  }  
  Mon* Good=new Mon();
  Mon* Bad = new Mon();
  Mon* Victim = new Mon();
  int turn=0, order=0;
  while(lspeed.size()>1){
    order=0;
    turn ++;    
    cout<<"\nTurn "<<turn<<endl;
    if (iplay==1)
      cin.ignore();
    resetMove(characters, false);
    lspeed=sortSpeed(characters);
    string sinput, sbad, sgood, smove, sname;
    int imove, irand=rand()%2;
    if(iplay==1){
      cout<<"\nChoose character: \n\t";
      for(int i=0;i<characters.size();i++)
        cout<<i+1<<") "<<characters.get(i)->getName()<<"\n\t";
      cin>>sgood;
      while(!isNumber(sgood)|| stoi(sgood)<1 ||stoi(sgood)>lspeed.size()) {
        cout<<"Invalid input"<<endl;
        cin>>sgood;
      }   
      Mon* Good=characters.get(stoi(sgood)-1);
      Good->display();
      sname=Good->getName();
      if (iplay==1&&iline==1)
        cin.ignore();
    }    
    for(int i=0;!allMoved(lspeed);i++){       
      if(!lspeed.get(i)->getMoved()){  
        order++;
        cout<<"\nSurvivors:\n";
        for(int j=0;j<characters.size();j++)
          cout<<"\t"<<j+1<<": "<<characters.get(j)->getName()<<endl; 
        if (iplay==1&&(iline==1||iline==2))
            cin.ignore();
        cout<<order<<"° character: "<<lspeed.get(i)->getName()<<" moves!"<<endl;
        if (iplay==1&&iline==1)
            cin.ignore();
        if(lspeed.get(i)->getName()==sname&&iplay==1){
          Good=lspeed.get(i);
          cout<<"\t1) Attack\n\t2) Special Attack\n\t3) Buff\n\t4) Debuff\n\t";
          cin>>smove;
          while(!isNumber(smove)|| stoi(smove)<1 ||stoi(smove)>4){
            cout<<"Invalid input"<<endl;
            cin>>smove;
          }
          imove=stoi(smove);
          if(imove!=3&&characters.size()>2){
            cout<<"Choose opponent:\n\t";
            cin>>sbad;
            while(!isNumber(sbad)|| stoi(sbad)<1 ||stoi(sbad)>lspeed.size()||sbad==sgood){
              if (sbad==sgood)
                cout<<"Cannot fight oneself. ";
              else
                cout<<"Invalid input. ";
              cout<<"Enter again: ";
              cin>>sbad;
            }
            Bad=characters.get(stoi(sbad)-1);
            Bad->display();
            if(iplay==1&&iline==1)
            cin.ignore();
          }
          else {
            int r=rand()%2;
            while(characters.get(r)==Good)
              r=rand()%2;
            Bad=characters.get(r);
          }
          if (iplay==1&&iline==1)
            cin.ignore();
          switch (imove) {
            case 1:
              Good->Attack(Bad, makeR());
              if(Bad->getHp()>0)
                cout<<"\t"<<Bad->getName()<<" HP left: "<<Bad->getHp()<<endl;
              break;
            case 2:
              Good->SpAttack(Bad, makeR());
              if(Bad->getHp()>0)
                cout<<"\t"<<Bad->getName()<<" HP left: "<<Bad->getHp()<<endl;
              break;
            case 3:
              Good->display();
              cout<<"\nIncrease:\n\t1) Hp\n\t2) Attack\n\t3) Defense\n\t4) Special Attack\n\t5) Special Defense\n\t6) Speed\n\t7) Critical rate\n\t";
              cin>>smove;
              while(!isNumber(smove)|| stoi(smove)<1 ||stoi(smove)>7){
                cout<<"Invalid input"<<endl;
                cin>>smove;
              }
              cout<<Good->getName()<<" buffs!"<<endl;
              Good->setBuff(stoi(smove));
              Good->display();
              break;
            case 4:
              cout<<"Decrease:\n\t1) Attack\n\t2) Defense\n\t3) Special Attack\n\t4) Special Defense\n\t5) Speed\n\t6) Critical rate\n\t";
              cin>>smove;
              while(!isNumber(smove)|| stoi(smove)<1 ||stoi(smove)>6){
                cout<<"Invalid input"<<endl;
                cin>>smove;
              }
              cout<<Good->getName()<<" debuffs "<<Bad->getName()<<"!"<<endl;
              Bad->setDebuff(stoi(smove));
              if (iplay==1&&iline==1)
                cin.ignore();
              Bad->display();
              break;
          }
          Good->setMoved(true);
          if (iplay==1&&iline==1)
            cin.ignore();
          int posvic=characters.getPos(Bad);
          if(Bad->getHp()<=0){
            podium.addFirst(Bad);
            characters.remove(posvic);
            i=0;
            if(characters.size()==1){
              resetMove(characters, true);
            }              
          }  
        }
        else {
          Bad=lspeed.get(i); 
          irand=rand()%2;
          if(irand==0){
            Victim=Bad->chooseVictim(lspeed);
            int posvic=characters.getPos(Victim);
            chooseAttack(Bad, Victim, makeR());
            if (iplay==1&&iline==1)
              cin.ignore();
            if(Victim->getHp()<=0)  {
              podium.addFirst(Victim);
              characters.remove(posvic);
              i=0;
              if(characters.size()==1)
                resetMove(characters, true);
            }             
          }          
          else {
            Victim=Bad->chooseVictimDebuff(lspeed);
            irand=rand()%2;
            if(irand==0)
              chooseBuff(Bad, Victim);
            else
              chooseDebuff(Bad, Victim);
            if (iplay==1&&iline==1)
                cin.ignore();
          }   
          Bad->setMoved(true);
        }
        lspeed=sortSpeed(characters);
      }
      if(!allMoved(lspeed)&&i>=lspeed.size()-1)
        i=0;      
    }    
  } 
  podium.addFirst(lspeed.getFirst());
  cout<<endl<<turn<<" turns total:\nThe winner is: "<<lspeed.getFirst()->getName()<<"!"<<endl;
  if (iplay==1)        
    cin.ignore();
  cout<<"\nFinal stats:"<<endl;
  lspeed.getFirst()->display();  
  cout<<"Podium:"<<endl;
  for(int i=0;i<podium.size();i++)
    cout<<'\t'<<i+1<<" place: "<<podium.get(i)->getName()<<endl;
  
}