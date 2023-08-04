#include <Arduino.h>

#define D1 5  /*  Пины (катоды) для игроков  */
#define D2 4  //  Для белых/первого игрока
#define D3 3
#define D4 2

#define D5 A2 //  Для черных/второго игрока
#define D6 A3
#define D7 A4
#define D8 A5

#define E 6   /*  Сегменты (общие аноды) на индикаторах  */
#define D 7
#define DP 8
#define C 9
#define G 10
#define B 11
#define FF 12
#define A 13

#define whiteButton A1  //  Кнопка подтверждения хода белых
#define blackButton A0  //  Кнопка подтверждения хода черных
#define modeButton 1    //  Кнопка изменения режима игры

uint32_t curent = 0;          //  Отсчет времени игры
uint32_t modeChangeTime = 0;  //  Отсчет времени изменения времени

bool agreed;                //  "Согласие"
char turn = 'S';            //  "Фаза игры/Очередь" (S - подготовка)
int16_t blackSeconds = 30;  //  Начальное время для черных
int16_t whiteSeconds = 30;  //  Начальное время для белых

void setup(){
  for(int l=2; l<=13; l++){
    pinMode(l,OUTPUT);
    digitalWrite(l,LOW);
  }
  for(int i = A5; i>=A2; i--){
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
  pinMode(whiteButton,INPUT_PULLUP);
  pinMode(blackButton,INPUT_PULLUP);
}
    /*  Процедуры отображения цифр и символов  */
void zero(){
  digitalWrite(A,HIGH);
  digitalWrite(B,HIGH);
  digitalWrite(C,HIGH);
  digitalWrite(D,HIGH);
  digitalWrite(E,HIGH);
  digitalWrite(FF,HIGH);
  digitalWrite(G,LOW);
}

void one(){
  digitalWrite(A,LOW);
  digitalWrite(B,HIGH);
  digitalWrite(C,HIGH);
  digitalWrite(D,LOW);
  digitalWrite(E,LOW);
  digitalWrite(FF,LOW);
  digitalWrite(G,LOW);
}

void two(){
  digitalWrite(A,HIGH);
  digitalWrite(B,HIGH);
  digitalWrite(C,LOW);
  digitalWrite(D,HIGH);
  digitalWrite(E,HIGH);
  digitalWrite(FF,LOW);
  digitalWrite(G,HIGH);
}
  
void three(){
  digitalWrite(A,HIGH);
  digitalWrite(B,HIGH);
  digitalWrite(C,HIGH);
  digitalWrite(D,HIGH);
  digitalWrite(E,LOW);
  digitalWrite(FF,LOW);
  digitalWrite(G,HIGH);
}
  
void four(){
  digitalWrite(A,LOW);
  digitalWrite(B,HIGH);
  digitalWrite(C,HIGH);
  digitalWrite(D,LOW);
  digitalWrite(E,LOW);
  digitalWrite(FF,HIGH);
  digitalWrite(G,HIGH);
}
  
void five(){
  digitalWrite(A,HIGH);
  digitalWrite(B,LOW);
  digitalWrite(C,HIGH);
  digitalWrite(D,HIGH);
  digitalWrite(E,LOW);
  digitalWrite(FF,HIGH);
  digitalWrite(G,HIGH);
}
  
void six(){
  digitalWrite(A,HIGH);
  digitalWrite(B,LOW);
  digitalWrite(C,HIGH);
  digitalWrite(D,HIGH);
  digitalWrite(E,HIGH);
  digitalWrite(FF,HIGH);
  digitalWrite(G,HIGH);
}
  
void seven(){
  digitalWrite(A,HIGH);
  digitalWrite(B,HIGH);
  digitalWrite(C,HIGH);
  digitalWrite(D,LOW);
  digitalWrite(E,LOW);
  digitalWrite(FF,LOW);
  digitalWrite(G,LOW);
}
  
void eight(){
  digitalWrite(A,HIGH);
  digitalWrite(B,HIGH);
  digitalWrite(C,HIGH);
  digitalWrite(D,HIGH);
  digitalWrite(E,HIGH);
  digitalWrite(FF,HIGH);
  digitalWrite(G,HIGH);
}
  
void nine(){
  digitalWrite(A,HIGH);
  digitalWrite(B,HIGH);
  digitalWrite(C,HIGH);
  digitalWrite(D,HIGH);
  digitalWrite(E,LOW);
  digitalWrite(FF,HIGH);
  digitalWrite(G,HIGH);
}

void L(){
  digitalWrite(A,LOW);
  digitalWrite(B,LOW);
  digitalWrite(C,LOW);
  digitalWrite(D,HIGH);
  digitalWrite(E,HIGH);
  digitalWrite(FF,HIGH);
  digitalWrite(G,LOW);
}

void S(){
  digitalWrite(A,HIGH);
  digitalWrite(B,LOW);
  digitalWrite(C,HIGH);
  digitalWrite(D,HIGH);
  digitalWrite(E,LOW);
  digitalWrite(FF,HIGH);
  digitalWrite(G,HIGH);
}

void minus(){
  digitalWrite(A,LOW);
  digitalWrite(B,LOW);
  digitalWrite(C,LOW);
  digitalWrite(D,LOW);
  digitalWrite(E,LOW);
  digitalWrite(FF,LOW);
  digitalWrite(G,HIGH);
}

void writeNum(int cathode,int number){    /*  Динамическая индикация  */
  digitalWrite(cathode,LOW);
  switch (number){    //  Возможно добавление новых символов
    case -3: S(); break;
    case -2: L(); break;
    case -1: minus(); break;
    case 0: zero(); break;
    case 1: one(); break;
    case 2: two(); break;
    case 3: three(); break;
    case 4: four(); break;
    case 5: five(); break;
    case 6: six(); break;
    case 7: seven(); break;
    case 8: eight(); break;
    case 9: nine(); break;
  }
  delay(1);
  digitalWrite(cathode,HIGH);
}

void chezzTime(){   /*  Вывод времени белых и черных (в секундах)  */
  writeNum(D1,whiteSeconds/60/10);
  writeNum(D2,whiteSeconds/60%10);
  writeNum(D3,whiteSeconds%60/10);
  writeNum(D4,whiteSeconds%60%10);
 
  writeNum(D5,blackSeconds/60/10);
  writeNum(D6,blackSeconds/60%10);
  writeNum(D7,blackSeconds%60/10);
  writeNum(D8,blackSeconds%60%10);
}

void loop() {
  uint8_t mode = 0;   //  Наяальный режим игры
  
  while (turn == 'S'){    /*  Подготовка  */
    chezzTime();

    if(!digitalRead(modeButton)){   //  При нажатии кнопки "режима"...
      if(millis() - modeChangeTime > 250){mode++; modeChangeTime = millis();}   //  Плавная смена режима
      switch (mode){    //  Возможно изменение числа режимов и времени (это по желанию)
        case 0: blackSeconds = whiteSeconds = 30; break;
        case 1: blackSeconds = whiteSeconds = 60; break;
        case 2: blackSeconds = whiteSeconds = 60*2; break;
        case 3: blackSeconds = whiteSeconds = 60*3; break;
        case 4: blackSeconds = whiteSeconds = 60*5; break;
        case 5: blackSeconds = whiteSeconds = 60*10; break;
        default: mode = 0; break;
      }  
    }
    
    if (!(digitalRead(whiteButton) || digitalRead(blackButton))){   // Согласие игроков (при одновременном нажатии)
      curent = millis();    //  (curent можно убрать и поставить delay(1500))
      agreed = 1;
    }
    
    if(agreed && millis() - curent >= 1500){    // Когда прошло 1,5 секунды после согласия...   (если 228 строка изменена, curent можно убрать)
      if (!digitalRead(blackButton)){turn = 'w';}   //  После хода черных очередь переходит белым
      else if (!digitalRead(whiteButton)){turn = 'b';}    //  После хода белых - черным 
    }
  }
  while (turn == 'w'){    /*  Во время хода белых...  */
    chezzTime();
    if(millis() - curent >= 1000){
        whiteSeconds -= 1;
        if(whiteSeconds < 0){   //  Если время истекло... (00:00 - Не истечение времени)
          turn = 'B';   //  Фаза - победа черных
          whiteSeconds = 30;    //  Перезапуск времени
          blackSeconds = 30;
        }
      digitalWrite(DP,!digitalRead(DP));
      curent = millis();
    }
    if (!digitalRead(whiteButton)){turn = 'b';}   // После белых ход передается черным 
  }
  while (turn == 'b'){    /*  Во время хода черных... */
    chezzTime();
    if(millis() - curent >= 1000){
        blackSeconds -= 1;
        if(blackSeconds < 0){
          turn = 'W';   //  Фаза - победа белых
          whiteSeconds = 30;
          blackSeconds = 30;
        }
      digitalWrite(DP,!digitalRead(DP));    //Просто двоеточие/напоминание, что время идет  
      curent = millis();
    }
    if (!digitalRead(blackButton)){turn = 'w';}
  }
  while (turn == 'W'){    /*  При победе белых...  */
    writeNum(D1,-1);      //  -
    writeNum(D2,-2);      //  L
    writeNum(D3,0);       //  0
    writeNum(D4,-2);      //  L

    writeNum(D5,-3);      //  S
    writeNum(D6,0);       //  0
    writeNum(D7,-3);      //  S
    writeNum(D8,-1);      //  -

    if(!(digitalRead(whiteButton) || digitalRead(blackButton))){    //  При одновременном нажатии 
      turn = 'S';   //  Перезапуск игры
      agreed = 0;   //  Сборос "согласия"
      delay(1500);
    }
  }
  while (turn == 'B'){    /*  При победе черных... */
    writeNum(D1,-3);      //  S
    writeNum(D2,0);       //  0
    writeNum(D3,-3);      //  S
    writeNum(D4,-1);      //  -

    writeNum(D5,-1);      //  -
    writeNum(D6,-2);      //  L
    writeNum(D7,0);       //  0
    writeNum(D8,-2);      //  L

    if(!(digitalRead(whiteButton) || digitalRead(blackButton))){
      turn = 'S';
      delay(1000);
    }
  }
}
