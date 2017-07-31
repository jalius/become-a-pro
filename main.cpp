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

#include <hack.h>
hack h;

void glowLoop(){
    for(;;){
        h.glow();
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}
void aimLoop(){
    for(;;){
        h.aim();
        std::this_thread::sleep_for(std::chrono::microseconds(30));
    }
}
void bhopLoop(){
    for(;;){
        h.bhop();
        std::this_thread::sleep_for(std::chrono::microseconds(30));
    }
}
void multiQuickLoop(){
    for(;;){
        h.trigger();
        std::this_thread::sleep_for(std::chrono::microseconds(30));
    }
}
void multiSlowLoop(){
    while(true){
        h.noFlash();
        h.setFov();
        h.setHands();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}
void checkKeysLoop(){
    while(h.checkKeys()){
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
