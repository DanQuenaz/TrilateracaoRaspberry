#include <stdio.h>
#include "rc-switch/RCSwitch.h"
#include <iostream>
#include <chrono>
#include <pthread.h>

using namespace std;

int PIN_tx = 0;
int PIN_rx = 2;

int cicle = 1;

RCSwitch myTx;
RCSwitch myRx;

void *sendMessages(void *threadid) {
    while(1){
        myTx.setProtocol(1);
        auto time1 = std::chrono::high_resolution_clock::now();
        myTx.send(cicle%1024, 32);

        cicle++;
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pthread_t thread1;
    int rc;
    int i=1;

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
    pullUpDnControl(PIN_rx, PUD_OFF);

    myTx = RCSwitch();
    myRx = RCSwitch();

    // Transmitter is connected
    myTx.enableTransmit(PIN_tx);
    myRx.enableReceive(PIN_rx);

    cout << "main() : creating thread, " << i << endl;
    rc = pthread_create(&thread1, NULL, sendMessages, (void *)i);

    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    printf("Listening:\n");

    while(true) {

        if (myRx.available() && myRx.getReceivedValue() > 2000) {
            long double xxTTIME = (long double)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-time1).count();
            cout<<myRx.getReceivedValue()<<" - "<<myRx.getReceivedProtocol()<<": "<<xxTTIME<<endl;
        }

        delay(1000);

    }
}
