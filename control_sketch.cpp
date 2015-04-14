/** Управление сервоприводами для робо-руки */

#include <Servo.h>

int const COUNT_SERVO = 1; //кол-во сервоприводов
int const pause = 30;
int const delta = 2;
int const MIN_WIDTH = 700;
int const MAX_WIDTH = 2400;
Servo servo[COUNT_SERVO]; //массив сервоприводов
int pos[COUNT_SERVO];
int current[COUNT_SERVO];
int first_servo_pin = 3; //серво №1 на этом пине, следующий на first_servo_pin + 1 и т д
// не стравить на RX TX !

int sgn(int x);

void setup() {
    Serial.begin(9600); //выставляем скорость com порта 9600 бод
    for (int i = 0; i < COUNT_SERVO; i++) {
        servo[i].attach(first_servo_pin + i, MIN_WIDTH, MAX_WIDTH); //аттачим все сервы на свои пины
        pos[i] = current[i] = 90;
        servo[i].write(current[i]);
    }
}

void loop() {
    if (Serial.available() > 0) {// если в буфере входного потока что-то есть
        int num = Serial.parseInt();
        int deg = Serial.parseInt();
        pos[num - 1] = deg;
        
        
    }
    
    for (int i = 0; i < COUNT_SERVO; i++) {
        if (abs(current[i] - pos[i]) > delta) {
            current[i] += delta * sgn(pos[i] - current[i]);
            servo[i].write(current[i]);
        }
        else {
            current[i] = pos[i];
            servo[i].write(pos[i]);
        }
    }
    delay(pause);
}


int sgn(int x) {
    return (x>=0)?1:(-1);
}
