#include <stdio.h>
#include <iostream>
#include "rc-switch/RCSwitch.h"
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {

    cout<<"Starting senddemo\n";
    cout<<"Make sure you have connected the 433Mhz sender data pin to WiringPi pin 0 (real pin 11)."<<endl;

    int PIN = 0;
    auto start = std::chrono::high_resolution_clock::now();
    unsigned long int ref = 0;


    wiringPiSetup();
    //pinMode(1, OUTPUT);         // configura pino 1 como saida
	pinMode(5, INPUT);          // configura pino 5 como entrada
	//pullUpDnControl(5, PUD_OFF); // configura resistor pull-up no pino 5 

    RCSwitch mySwitch = RCSwitch();

    // Transmitter is connected
    mySwitch.enableTransmit(PIN);

    // Optional set pulse length.
    //mySwitch.setPulseLength(1000);

    // Optional set protocol (default is 1, will work for most outlets)
    //	mySwitch.setProtocol(5);

    // Optional set number of transmission repetitions.
    // mySwitch.setRepeatTransmit(15);

    while(1){
        if(digitalRead(5)==HIGH){
            ref = (unsigned long int)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-start).count();
            mySwitch.setProtocol(4);
	    cout<<"Resetado!\n";
            mySwitch.send(123, 32);
        }else{
            mySwitch.setProtocol(1);
            cout<<"Sending something: ";
            auto finish = std::chrono::high_resolution_clock::now();
            unsigned long int aux = (unsigned long int)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count());
	    aux = (aux-ref)%4000000000;
            cout<<aux<<endl;
            mySwitch.send(aux, 32);
        }
        
        delay(1000);
    }
    printf("Done\n");

    return 0;
}
