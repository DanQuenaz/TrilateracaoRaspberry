#include <stdio.h>
#include "rc-switch/RCSwitch.h"
#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {

    printf("Starting receivedemo\n");
    printf("Make sure you have connected the 433Mhz receiver data pin to WiringPi pin 2 (real pin 13). See: https://pinout.xyz/pinout/pin13_gpio27\n");
    printf("\n");
    
    int PIN = 2; // this is pin 13, aka GPIO22 on the PI3, see https://www.element14.com/community/servlet/JiveServlet/previewBody/73950-102-10-339300/pi3_gpio.png
    unsigned long int lastValue;
    unsigned long int ref = 0;
    unsigned long int aux = 0;

    auto start = std::chrono::high_resolution_clock::now();


    if (wiringPiSetup () == -1) {
        printf("ERROR: WiringPi not installed. Make sure you have WiringPi installed.\n");
        printf("Quick tutorial:\n\n");
        printf("    sudo apt-get install git\n");
        printf("    cd ~/\n");
        printf("    git clone git://git.drogon.net/wiringPi\n");
        printf("    cd wiringPi\n");
        printf("    ./build\n\n");
        return 1;
    }

    // put the PIN into no-pull/up down state:
    // see https://github.com/ninjablocks/433Utils/issues/21
    pullUpDnControl(PIN, PUD_OFF);

    RCSwitch mySwitch = RCSwitch();

    mySwitch.enableReceive(PIN);

    printf("Listening:\n");

    while(true) {

        if (mySwitch.available()) {
            unsigned long int value = mySwitch.getReceivedValue();
            if (value != 0 && value != lastValue) {
                if(mySwitch.getReceivedProtocol() == 4){
                    ref = (unsigned long int)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-start).count();
                    cout<<"Resetado\n";
                }
		        
                aux = (unsigned long int)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-start).count();

                cout<<(aux-ref)-(unsigned long int)mySwitch.getReceivedValue()<<" - "<<mySwitch.getReceivedProtocol()<<endl;
		        
                lastValue = value;
            }
            mySwitch.resetAvailable();
        }
    }
}
