#include <stdio.h>
#include <iostream>
#include "rc-switch/RCSwitch.h"
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {

    cout<<"Starting senddemo\n";
    cout<<"Make sure you have connected the 433Mhz sender data pin to WiringPi pin 0 (real pin 11)."<<endl;

    int PIN_tx = 0;
    int PIN_rx = 2;
    int last_value = -1;
    //auto start = std::chrono::high_resolution_clock::now();
    //unsigned long int ref = 0;


    wiringPiSetup();
    //pinMode(1, OUTPUT);         // configura pino 1 como saida
	pinMode(5, INPUT);          // configura pino 5 como entrada
	//pullUpDnControl(5, PUD_OFF); // configura resistor pull-up no pino 5 

    RCSwitch myTx = RCSwitch();
    RCSwitch myRx = RCSwitch();

    // Transmitter is connected
    myTx.enableTransmit(PIN_tx);
    myRx.enableReceive(PIN_rx);

    // Optional set pulse length.
    //mySwitch.setPulseLength(1000);

    // Optional set protocol (default is 1, will work for most outlets)
    myTx.setProtocol(1);

    // Optional set number of transmission repetitions.
    // mySwitch.setRepeatTransmit(15);

    while(1){
        if(digitalRead(5)==HIGH){
           
        }else{
            if (myRx.available()) {
                if(myRx.getReceivedProtocol() == 1){
                    if(myRx.getReceivedValue() != last_value){
                        myTx.send(myRx.getReceivedValue(), 32);
                        cout<<"Atena 1 respondeu"<<endl;
                        last_value = myRx.getReceivedValue();    
                    }
                     
                }
            }
            
            //auto finish = std::chrono::high_resolution_clock::now();
            //unsigned long int aux = (unsigned long int)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count());
           
        }
    }
    printf("Done\n");

    return 0;
}
