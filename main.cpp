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
        //std::cout<<"you're glowing :)"<<std::endl;
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
        //std::cout<<"ph00n 2.0?"<<std::endl;
        h.bhop();
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
}
void triggerLoop(){
    for(;;){
        //std::cout<<"that 0.001 ms reaction time tho"<<std::endl;
        h.trigger();
    }
}
void flashLoop(){
    while(true){
        h.noFlash();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}
void checkKeysLoop(){
    while(h.checkKeys()){
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    /*while (h.csgo.IsRunning()) {
		XQueryKeymap(h.display, h.keys);
		for (unsigned i = 0; i < sizeof(h.keys); ++i) {
			if (h.keys[i] != h.lastkeys[i]) {
				// check which key got changed
				for (unsigned j = 0, test = 1; j < 8; ++j, test *= 2) {
					// if the key was pressed, and it wasn't before, print this
					if ((h.keys[i] & test) &&
							((h.keys[i] & test) != (h.lastkeys[i] & test))) {
						const int code = i * 8 + j;
						if (code == h.keycodeGlow) {
							h.csgo.m_bShouldGlow = !h.csgo.m_bShouldGlow;
							std::cout<<"ESP: %b"<<h.csgo.m_bShouldGlow<<endl;
						}
						else if (code == h.keycodeTrigger){
							h.csgo.m_ShouldTrigger = !h.csgo.m_ShouldTrigger;
							std::cout<<"Trigger: "<<h.csgo.m_ShouldTrigger<<endl;
						}
						else if (code == h.keycodeBhop){
							h.csgo.ShouldBhop = !h.csgo.ShouldBhop;
							std::cout<<"Bhop: "<< h.csgo.ShouldBhop<<endl;
						}
					}
				}
			}

			h.lastkeys[i] = h.keys[i];
		}
	}*/
    return;
}
int main()
{
    //Display* display = XOpenDisplay(0);
    XInitThreads();
    h.init();
    std::thread g(glowLoop);
    std::thread a(aimLoop);
    std::thread b(bhopLoop);
    std::thread t(triggerLoop);
    std::thread f(flashLoop);
    std::thread c(checkKeysLoop);
    c.join();
    return 0;
}
