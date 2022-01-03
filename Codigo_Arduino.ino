#include <Servo.h>

//inputs de fase thetas 1 e 2 em formatos de strings
String inp_fase,inp_t1,inp_t2;
int fase=0,theta1=90,theta2=90;
float incremento;
Servo motor1,motor2;

void setup() {
  Serial.begin(9600);
  motor1.attach(5);
  if(motor1.read()!=90){
    for(int i=0;i<=90;i++){
      motor1.write(i);
      delay(15);
    }
  }
  motor2.attach(6);
  if(motor2.read()!=90){
    for(int i=0;i<=90;i++){
      motor2.write(i);
      delay(15);
    }
  }
}

void loop() {
  if(Serial.available()){
    inp_fase = Serial.readStringUntil(' ');
    inp_t1 = Serial.readStringUntil(' ');
    inp_t2 = Serial.readStringUntil('\n');
    fase = inp_fase.toInt();
    theta1 = inp_t1.toInt();
    theta2 = inp_t2.toInt();
    int x = abs((motor1.read()-theta1)) - abs((motor2.read()-theta2));
    switch(fase){
      case 1://Mover separadamente, comeca com t1 e após t2, angulos iguais nao fazem nada
          if(motor1.read()<theta1){
            for(int pos=motor1.read();pos<=theta1;pos+=1){
              motor1.write(pos);
              delay(15);
            }
          }else if(motor1.read()>theta1){
            for(int pos=motor1.read();pos>=theta1;pos-=1){
              motor1.write(pos);
              delay(15);
            } 
          }
          if(motor2.read()<theta2){
            for(int pos=motor2.read();pos<=theta2;pos+=1){
              motor2.write(pos);
              delay(15);
            }
          }else if(motor2.read()>theta2){
            for(int pos=motor2.read();pos>=theta2;pos-=1){
              motor2.write(pos);
              delay(15);
            } 
          }
      break;
      case 2://mesma velocidade
      //primeiro checa-se qual a maior variacao
        if(abs(x)>=0){//motor1 varia + ou o mesmo que o motor2
          if(motor1.read()>theta1){
              if(motor2.read()>theta2){
                  int pos2 = motor2.read();
                  for(int pos = motor1.read();pos>=theta1;pos--){
                      motor1.write(pos);
                      if(pos2>=theta2){
                          motor2.write(pos2);
                      }
                      delay(15);
                      pos2--;
                  }
              }else{//m2<t2
                  int pos2 = motor2.read();
                  for(int pos = motor1.read();pos>=theta1;pos--){
                      motor1.write(pos);
                      if(pos<=theta2){
                          motor2.write(pos2);
                      }
                      delay(15);
                      pos2++;
                  }
              }
          }else{
              if(motor2.read()<theta2){
                  int pos2=motor2.read();
                  for(int pos = motor1.read();pos<=theta1;pos++){
                      motor1.write(pos);
                      if(pos2<=theta2){
                          motor2.write(pos2);
                      }
                      delay(15);
                      pos2++;
                  }
              }else{
                  int pos2 = motor2.read();
                  for(int pos=motor1.read();pos<=theta1;pos++){
                      motor1.write(pos);
                      if(pos2>=theta2){
                          motor2.write(pos2);
                      }
                      delay(15);
                      pos2--;
                  }
              }
          }
          
        }else{
            if(motor2.read()>theta2){
              if(motor1.read()>theta1){
                  int pos2 = motor1.read();
                  for(int pos = motor2.read();pos>=theta2;pos--){
                      motor2.write(pos);
                      if(pos2>=theta1){
                          motor1.write(pos2);
                      }
                      delay(15);
                      pos2--;
                  }
              }else{//caso m2 aumente e m1 diminua
                  int pos2 = motor1.read();
                  for(int pos = motor2.read();pos>=theta2;pos--){
                      motor2.write(pos);
                      if(pos2<=theta1){
                          motor1.write(pos2);
                      }
                      delay(15);
                      pos2++;
                  }
              }
          }else{
              if(motor1.read()<theta1){
                  int pos2=motor1.read();
                  for(int pos = motor2.read();pos<=theta2;pos++){
                      motor2.write(pos);
                      if(pos2<=theta1){
                          motor1.write(pos2);
                      }
                      delay(15);
                      pos2++;
                  }
              }else{
                  int pos2 = motor1.read();
                  for(int pos=motor2.read();pos<=theta2;pos++){
                      motor2.write(pos);
                      if(pos2>=theta1){
                          motor1.write(pos2);
                      }
                      delay(15);
                      pos2--;
                  }
              }
          }
          
        }
        if(theta1==motor1.read()){//caso algum theta nao varie, foi usado o codigo do caso 1
          if(motor2.read()<theta2){
            for(int pos=motor2.read();pos<=theta2;pos+=1){
              motor2.write(pos);
              delay(15);
            }
          }else if(motor2.read()>theta2){
            for(int pos=motor2.read();pos>=theta2;pos-=1){
              motor2.write(pos);
              delay(15);
            } 
          }
        }else if(theta2 == motor2.read()){
          if(motor1.read()<theta1){
            for(int pos=motor1.read();pos<=theta1;pos+=1){
              motor1.write(pos);
              delay(15);
            }
          }else if(motor1.read()>theta1){
            for(int pos=motor1.read();pos>=theta1;pos-=1){
              motor1.write(pos);
              delay(15);
            } 
          }
        }
      break;
      case 3://param juntas
       float aux = 0;//variavel auxiliar para variacao angular
       if(abs(x)>=0){//variacao de m1 >= variacao de m2
        aux = theta1/theta2;
        int pos2 = motor2.read();
        if(motor1.read()<theta1){
            if(motor2.read()<theta2){
                for(int pos=motor1.read();pos<=theta1;pos++){
                    motor1.write(pos);
                    motor2.write(pos2);
                    delay(15);     
                    pos2+=aux;
                    if((pos2)>theta2){
                      pos2=theta2;
                    }
                }
            }else{//m2>t2
                for(int pos=motor1.read();pos<=theta1;pos++){
                    motor1.write(pos);
                    motor2.write(pos2);
                    delay(15);
                    pos2-=aux;
                    if((pos2)<theta2){
                      pos2=theta2;
                    }
                }
            }
        }else{//m1>t1
            if(motor2.read()<theta2){
                for(int pos=motor1.read();pos<=theta1;pos--){
                    motor1.write(pos);
                    motor2.write(pos2);
                    delay(15);
                    pos2+=aux;
                    if((pos2)>theta2){
                      pos2=theta2;
                    }
                }
            }else{//m2>t2
                for(int pos=motor1.read();pos<=theta1;pos--){
                    motor1.write(pos);
                    motor2.write(pos2);
                    delay(15);
                    pos2-=aux;
                    if((pos2)<theta2){
                      pos2=theta2;
                    }
                }
            }
        }
      }else{//abs(x)<0
      //Sendo a mesma logica, so foi trocado os "1"s e "2"s
        aux = theta2/theta1;
        int pos2 = motor1.read();
        if(motor2.read()<theta2){
            if(motor1.read()<theta1){
                for(int pos=motor2.read();pos<=theta2;pos++){
                    motor2.write(pos);
                    motor1.write(pos2);
                    delay(15);
                    pos2+=aux;
                    if((pos2)>theta1){
                      pos2=theta1;
                    }
                }
            }else{
                for(int pos=motor2.read();pos<=theta2;pos++){
                    motor2.write(pos);
                    motor1.write(pos2);
                    delay(15);
                    pos2-=aux;
                    if((pos2)<theta1){
                      pos2=theta1;
                    }
                }
            }
        }else{
            if(motor1.read()<theta1){
                for(int pos=motor2.read();pos<=theta2;pos--){
                    motor2.write(pos);
                    motor1.write(pos2);
                    delay(15);
                    pos2+=aux;
                    if((pos2)>theta1){
                      pos2=theta1;
                    }
                }
            }else{
                for(int pos=motor2.read();pos<=theta2;pos--){
                    motor2.write(pos);
                    motor1.write(pos2);
                    delay(15);
                    pos2-=aux;
                    if((pos2)<theta1){
                      pos2=theta1;
                    }
                }
            }
        }
      }   
      break;
    }
  }

}
