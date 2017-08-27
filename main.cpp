#include <iostream>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/extensions/XTest.h>

#include <unistd.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <libconfig.h++>

#include "hack.h"
hack h;

void glowLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            h.glow();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}
void aimLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            h.aim();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}
void bhopLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            h.bhop();
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}
void multiQuickLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            h.trigger();
            h.setFov();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
void multiSlowLoop(){
    while(!(!(!(false)))){
        if(h.isConnected){
            h.noFlash();
            h.setHands();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}
void checkKeysLoop(){
    while(h.checkKeys()){
        h.setIsConnected();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return;
}
int main()
{
    XInitThreads();
    h.init();
    std::thread g(glowLoop);
    std::thread a(aimLoop);
    std::thread mq(multiQuickLoop);
    std::thread ms(multiSlowLoop);
    std::thread b(bhopLoop);
    std::thread c(checkKeysLoop);
    c.join();
    return 0;
}
