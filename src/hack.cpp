#include "hack.h"
Config cfg;
string getConfigValue(string property) {
	try {
		string name = cfg.lookup(property);
		return name;
	} catch (const SettingNotFoundException &nfex) {
		stringstream ss;
		ss << "Cannot find property: '" << property << "' in settings.cfg file";
		std::cout<<ss.str();
	}
	return NULL;
}
void hack::aim(){
    int isConnected;
    csgo.Read((void*)client.start+0x1D089C0,&isConnected,sizeof(int));
    if(isConnected!=1){
        return;
    }
    unsigned int serverDetail;
    unsigned long one;
    unsigned long two;
    unsigned int shotsFired;
    BoneMatrix theirBones;
    unsigned long localPlayer = 0;
    shouldShoot = false;
    char myLifeXD = LIFE_DEAD;
    int AltTwo;
    csgo.Read((void*)m_addressOfAlt2,&AltTwo,sizeof(long));
    if(localPlayer!=0){
        csgo.Read((void*)(unsigned long)localPlayer,&myLifeXD,sizeof(char));
    }
    if(myLifeXD!=LIFE_ALIVE){
        //return;
    }
    //cout<<myLifeXD<<endl;
    csgo.Read((void*) m_addressOfLocalPlayer, &localPlayer, sizeof(long));
    csgo.Read((void*)hack::addressServerDetail,&serverDetail, sizeof(int));
    csgo.Read((void*)hack::basePointerOfViewAngle,&one,sizeof(long));
    csgo.Read((void*)one+serverDetail,&two,sizeof(long));
    csgo.Read((void*)two+0x4308,&(hack::addressOfViewAngle),sizeof(long));
    hack::addressOfViewAngle+=0x8e20;

    csgo.Read((void*)(unsigned long)localPlayer,&myEnt,sizeof(Entity));

    csgo.Read((void*)(localPlayer+0x3764),&punch,sizeof(QAngle));

    csgo.Read((void*)(localPlayer+0xABB0), &shotsFired,sizeof(int));

    csgo.Read((void*)(unsigned long)addressOfViewAngle, &(hack::viewAngle),sizeof(QAngle));

    char enemyLifeState=LIFE_DEAD;

    csgo.Read((void*)bonePtrs[idclosestEnt]-0x2c70+0x293,&enemyLifeState,sizeof(char));

	hack::CGlowObjectManager manager;
    punchDelta.x = punch.x-hack::oldPunch.x;
    punchDelta.y = punch.y-hack::oldPunch.y;
	if(isAiming&&!rage){
        myPos.x = myEnt.m_vecNetworkOrigin.x;
        myPos.y = myEnt.m_vecNetworkOrigin.y;
        myPos.z = myEnt.m_vecNetworkOrigin.z + myEnt.m_vecViewOffset.z;
        newAngle = viewAngle;
        if(enemyLifeState!=LIFE_ALIVE){
            foundTarget=false;
        }
	}
	else{
    struct hack::GlowObjectDefinition_t g_glow[1024];
    bzero(g_glow, sizeof(g_glow));
    bzero(entities, sizeof(entities));
    bzero(bonePtrs, sizeof(bonePtrs));
    lowestDistance = -1.0;
    closestFootDistance=-1.0;
	if (!csgo.Read((void*) m_addressOfGlowPointer, &manager, sizeof(hack::CGlowObjectManager))) {
		cout<<("Failed to read glowClassAddress")<<endl;
		throw 1;
		return;
	}

	size_t count = manager.m_GlowObjectDefinitions.Count;

	void* data_ptr = (void*) manager.m_GlowObjectDefinitions.DataPtr;

	if (!csgo.Read(data_ptr, g_glow, sizeof(hack::GlowObjectDefinition_t) * count)) {
		cout<<("Failed to read m_GlowObjectDefinitions")<<endl;
		throw 1;
		return;
	}
    for (unsigned int i = 0; i < count; i++) {
		if (g_glow[i].m_pEntity != NULL) {
			Entity ent;
			if (csgo.Read(g_glow[i].m_pEntity, &ent, sizeof(Entity)))
            if (ent.m_iTeamNum == 2 || ent.m_iTeamNum == 3) {
                if(ent.m_iHealth>0&&ent.ID<=64&&ent.ID>0){
                    entities[ent.ID]=ent;
                    bonePtrs[ent.ID]=g_glow[i].m_pEntity+0x2c70;
                }
			}
        }
    }

    myPos.x = myEnt.m_vecNetworkOrigin.x;
    myPos.y = myEnt.m_vecNetworkOrigin.y;
    myPos.z = myEnt.m_vecNetworkOrigin.z + myEnt.m_vecViewOffset.z;

    newAngle = viewAngle;

    idclosestEnt=0;

    //aimbot block
    for(int i = 1;i<64;i++){
        char lifeState = LIFE_DEAD;
        csgo.Read((void*)(bonePtrs[i]-0x2c70+0x293),&lifeState,sizeof(char));//(entities[i].m_iTeamNum != myEnt.m_iTeamNum)&&
        //cout<<lifeState<<endl;
        if(((entities[i].m_iTeamNum ==3 && myEnt.m_iTeamNum==2)||(entities[i].m_iTeamNum ==2 && myEnt.m_iTeamNum==3))&&(entities[i].m_bDormant==0)&&(entities[i].m_iHealth>0)&&(entities[i].ID!=myEnt.ID)&&(lifeState==LIFE_ALIVE)){
            void* m_pStudioBones;
            //`printf("lifestate: %d",lifeState);
            csgo.Read(bonePtrs[i],&m_pStudioBones,sizeof(int));
            csgo.Read(m_pStudioBones+bone*sizeof(BoneMatrix),&theirBones,sizeof(BoneMatrix));
            theirPos.x = theirBones.x;
            theirPos.y = theirBones.y;
            theirPos.z = theirBones.z;
            if(theirPos.y==0&&theirPos.x==0&&theirPos.z==0){
                //cout<<"0 0 0 found"<<endl;
                continue;

            }
            aimDelta=calcAngle(&myPos,&theirPos);
            aimDelta.x -=viewAngle.x+(punch.x*2);
            aimDelta.y -=viewAngle.y+(punch.y*2);
            clampAngle(&aimDelta);
            float xhairDistance = sqrt((aimDelta.x*aimDelta.x)+(aimDelta.y*aimDelta.y));
            //cout<< "aimDelta.x, y = "<<aimDelta.x<<", "<<aimDelta.y<<endl;
            if(xhairDistance<=lowestDistance||lowestDistance==-1.0){//(||rage)
                //cout<<std::dec<<entities[i].ID<<" crosshair distance = "<<sqrt(xhairDistance)<<endl;
                //cout<<std::dec<<entities[i].ID<<" crosshair distance = "<<xhairDistance<<endl;
                //float checkClosestFootDistance = sqrt((theirPos.x-myPos.x)*(theirPos.x-myPos.x)+(theirPos.y-myPos.y)*(theirPos.y-myPos.y)+(theirPos.z-myPos.z)*(theirPos.z-myPos.z));
                //cout << "checkClosestFootDistance: " <<checkClosestFootDistance<< "closestFootDistance: "<<closestFootDistance<<endl;
                /*if(checkClosestFootDistance<closestFootDistance||closestFootDistance==-1.0){
                    //cout<<"setting new: "<<checkClosestFootDistance<<endl;
                    lowestDistance=xhairDistance;
                    closestEnt=&entities[i];
                    idclosestEnt = entities[i].ID;
                    closestFootDistance = checkClosestFootDistance;
                    foundTarget = true;
                    //cout<<"new: " <<closestFootDistance<<endl;
                }*/
                lowestDistance=xhairDistance;
                //cout<<"Lowest distance: "<<lowestDistance<<endl;
                closestEnt=&entities[i];
                idclosestEnt = entities[i].ID;
                foundTarget = true;
                //cout<<"set true ft"<<foundTarget<<endl;
                //cout<<"closest ent = "<<entities[i].ID<<endl;
            }
        }
    }
	}//else if (!isAiming)

    //cout<<std::dec<<"\nidclosestEnt: "<<idclosestEnt<<endl;
    if(!foundTarget&&isAiming||idclosestEnt==0){
        if(AltTwo==5){
            csgo.Write((void*)m_addressOfForceAttack,&toggleOn,sizeof(int));
        }
        if(AltTwo==4){
            csgo.Write((void*)m_addressOfForceAttack,&toggleOff,sizeof(int));
            /*newAngle.x-=punch.x;
            newAngle.y-=punch.y;*/
            isAiming=false;
        }
    }
    else{
        void* m_pStudioBones;
        csgo.Read(bonePtrs[idclosestEnt],&m_pStudioBones,sizeof(int));
        csgo.Read(m_pStudioBones+bone*sizeof(BoneMatrix),&theirBones,sizeof(BoneMatrix));
        theirPos.x = theirBones.x;
        theirPos.y = theirBones.y;
        theirPos.z = theirBones.z;
        if(enemyLifeState!=LIFE_ALIVE||shotsFired!=0){
            acquiring=false;
            //cout<<shotsFired<<endl;
        }
        //cout<<"not ignorning"<<endl;
        if(AltTwo==5||acquiring){
            //cout<<"1fov: "<<fov<<" lowestDistance: "<<lowestDistance<<endl;
            aimDelta=calcAngle(&myPos,&theirPos);
            aimDelta.x -=viewAngle.x+(punch.x*2);
            aimDelta.y -=viewAngle.y+(punch.y*2);
            clampAngle(&aimDelta);
            float xhairDistance = sqrt((aimDelta.x*aimDelta.x)+(aimDelta.y*aimDelta.y));
            lowestDistance=xhairDistance;
            cout<<"2fov: "<<fov<<" lowestDistance: "<<lowestDistance<<endl;
            if(lowestDistance<=fov&&lowestDistance!=-1.0){
                acquiring = true;
            //cout<<std::dec<<"targeting closestEnt: "<<idclosestEnt<<endl;
                if(lowestDistance<.1){
                if(lowestDistance<.05){
                    shouldShoot=true;
                }
                newAngle=calcAngle(&myPos,&theirPos);
                newAngle.x-=punch.x*2;
                newAngle.y-=punch.y*2;
                Smoothing(&viewAngle,&newAngle);
                isAiming =true;
                //cout<<"newAngle.x, "<<"newAngle.y :"<<newAngle.x<<", "<<newAngle.y<<endl;
            }
            else{
                shouldShoot=true;
            }
        }
        if(AltTwo==4){
            isAiming=false;
        }
    }
    /*if(rage){
        if(lowestDistance>-1.0&&lowestDistance<=.5){
            csgo.Write((void*)m_addressOfForceAttack,&toggleOn,sizeof(int));
            isAiming =true;
        }
        else if(idclosestEnt==0){
            csgo.Write((void*)m_addressOfForceAttack,&toggleOff,sizeof(int));
        }
        if(idclosestEnt!=0){
            newAngle=calcAngle(&myPos,&theirPos);
            newAngle.x-=punch.x*2;
            newAngle.y-=punch.y*2;
        }
    }*/

    //rcs block
    if(!isAiming&&alwaysRCS){
    if(alwaysRCS){
        if((punchDelta.y!=0.0||punchDelta.x!=0.0)){//&&shotsFired>0
                /*if(shotsFired>10){
                    newAngle.x -=punchDelta.x*1.7;//if shots fired greater than 10 then we will reduce the coefficient (trying to make ourselves seem legit)
                    newAngle.y -=punchDelta.y*1.5;
                    newAngle.z = 0;
                }
                else{*/
                    newAngle.x -=punchDelta.x*2;//new camera angle is the old camera angle minus the change in view punch angle *2 (because it works)
                    newAngle.y -=punchDelta.y*2;
                    newAngle.z = 0;
                //}
        }
    }
    hack::oldPunch.y = punch.y;
    hack::oldPunch.x = punch.x;
    hack::oldPunch.z = 0;
    }
    if (newAngle.x!=viewAngle.x||newAngle.y!=viewAngle.y){
        setVAng(&newAngle);
        //cout<<"set vang {" <<newAngle.x<< ", "<<newAngle.y<<" } ent id: "<<idclosestEnt<<endl;
    }
    if(foundTarget&&idclosestEnt!=0){
    if(AltTwo==5){
        if(shouldShoot){
            csgo.Write((void*)m_addressOfForceAttack,&toggleOn,sizeof(int));
            usleep(10000);
        }
    }
    else{
    if(!shouldShoot){
        csgo.Write((void*)m_addressOfForceAttack,&toggleOff,sizeof(int));
    }
    if(acquiring&&shouldShoot){
        csgo.Write((void*)m_addressOfForceAttack,&toggleOn,sizeof(int));
        usleep(40000);
        csgo.Write((void*)m_addressOfForceAttack,&toggleOff,sizeof(int));
        usleep(10000);
        acquiring=false;
    }
    }
    }
}
void hack::Smoothing(QAngle* source, QAngle* target){
    QAngle delta;
    delta.x = target->x - source->x;
    delta.y = target->y - source->y;
    //cout<< "Delta.x, y = "<<delta.x<<", "<<delta.y<<endl;
    clampAngle(&delta);
    float sqDistance = sqrt((delta.x*delta.x)+(delta.y*delta.y));
    float coefficient = percentSmoothing*(1/(sqDistance));
    if(coefficient>1){
        coefficient = 1;
    }
    delta.x*=coefficient;
    delta.y*=coefficient;
    clampAngle(&delta);
    //cout<< "After Delta.x, y = "<<delta.x<<", "<<delta.y<<endl;
    target->x=source->x+delta.x;
    target->y=source->y+delta.y;
    usleep(200);

}
QAngle hack::calcAngle(Vector* source, Vector* target){
    QAngle angle;
    Vector delta = {
        (source->x-target->x), (source->y-target->y), (source->z-target->z)
    };
	double hyp = sqrt(delta.x*delta.x + delta.y*delta.y);
	angle.x = (float) (atanf(delta.z/hyp) * 57.295779513082f);
	angle.y = (float) (atanf(delta.y/delta.x) * 57.295779513082f);
	angle.z = 0.0f;
	if(delta.x>=0){
        angle.y+=180;
    }
    clampAngle(&angle);
	return angle;
}
void hack::setVAng(QAngle *newAngle){
    while (newAngle->y>180){
        newAngle->y-=360;
    }
    while (newAngle->y<-180){
        newAngle->y+=360;
    }
    if(newAngle->x>89){
        newAngle->x=89;
    }
    else if(newAngle->x<-89){
        newAngle->x=-89;
    }
    if(newAngle->y!=newAngle->y){
        //cout<<"problem y "<<endl;
        newAngle->y=0;
    }
    if(newAngle->x!=newAngle->x){
        //cout<<"problem x "<<endl;
        newAngle->x=0;
    }
    newAngle->z=0;
    csgo.Write((void*)(unsigned long)(hack::addressOfViewAngle),&(*newAngle),sizeof(QAngle));
}
void hack::clampAngle(QAngle *angle){
    while (angle->y>180){
        angle->y-=360;
    }
    while (angle->y<-180){
        angle->y+=360;
    }
    if(angle->x>89){
        angle->x=89;
    }
    else if(angle->x<-89){
        angle->x=-89;
    }
    if(angle->y!=angle->y){
        //cout<<"problem y "<<endl;
        angle->y=0;
    }
    if(angle->x!=angle->x){
        //cout<<"problem x "<<endl;
        angle->x=0;
    }
}
void hack::bhop(){
    unsigned int iAlt1Status = 0 ;
    int onGround = 0;
    unsigned long localPlayer = 0;
    csgo.Read((void*) m_addressOfLocalPlayer, &localPlayer, sizeof(long));

    csgo.Read((void*)(m_addressOfAlt1), &iAlt1Status, sizeof(int));
    csgo.Read((void*)(localPlayer+0x138), &onGround, sizeof(unsigned char));
    if(ShouldBhop==true){
        //cout<<"\nalt1: "<<iAlt1Status<<" \nonGround: "<<onGround<<endl;
        if(iAlt1Status==5&&onGround>0){
            //cout<<"jumping\n:)"<<endl;
            csgo.Write((void*)((unsigned long)m_addressOfJump),&toggleOn,sizeof(int));
            this_thread::sleep_for(chrono::microseconds(500));
            csgo.Write((void*)((unsigned long)m_addressOfJump),&toggleOff, sizeof(int));
            /*XFlush(display);
            XTestFakeKeyEvent(display, 65, true, 0);
            this_thread::sleep_for(chrono::milliseconds(1));
            XTestFakeKeyEvent(display, 65, false, 0);*/
        }
    }
    else{
        if(iAlt1Status==5){
            //cout<<"jumping\n:)"<<endl;
            csgo.Write((void*)((unsigned long)m_addressOfJump),&toggleOn,sizeof(int));
            this_thread::sleep_for(chrono::microseconds(500));
        }
        else{
            csgo.Write((void*)((unsigned long)m_addressOfJump),&toggleOff, sizeof(int));
            this_thread::sleep_for(chrono::microseconds(500));
        }
    }
}
void hack::noFlash(){
    if(NoFlash){
        unsigned long localPlayer = 0;
        csgo.Read((void*) m_addressOfLocalPlayer, &localPlayer, sizeof(long));
        csgo.Write((void*)(localPlayer+0xabf4), &flashMax, sizeof(float));
    }
}
bool hack::glow(){
    int isConnected;
    csgo.Read((void*)client.start+0x1D089C0,&isConnected,sizeof(int));
    if(isConnected!=1){
        return false;
    }
    struct iovec g_remote[1024], g_local[1024];
    struct hack::GlowObjectDefinition_t g_glow[1024];
	// Reset
	bzero(g_remote, sizeof(g_remote));
	bzero(g_local, sizeof(g_local));
	bzero(g_glow, sizeof(g_glow));

    unsigned long localPlayer = 0;
	csgo.Read((void*) m_addressOfLocalPlayer, &localPlayer, sizeof(long));

    char myLifeXD = LIFE_DEAD;
    if (localPlayer != 0){
        csgo.Read((void*)(unsigned long)localPlayer,&myLifeXD,sizeof(char));
        if(myLifeXD!=LIFE_ALIVE){
            //return false;
        }
    }
	hack::CGlowObjectManager manager;

	if (!csgo.Read((void*) m_addressOfGlowPointer, &manager, sizeof(hack::CGlowObjectManager))) {
		cout<<("Failed to read glowClassAddress")<<endl;
		throw 1;
		return false;
	}

	size_t count = manager.m_GlowObjectDefinitions.Count;

	void* data_ptr = (void*) manager.m_GlowObjectDefinitions.DataPtr;

	if (!csgo.Read(data_ptr, g_glow, sizeof(hack::GlowObjectDefinition_t) * count)) {
		cout<<("Failed to read m_GlowObjectDefinitions")<<endl;
		throw 1;
		return false;
	}

	size_t writeCount = 0;
	unsigned int teamNumber = 0;

	if (localPlayer != 0){
		csgo.Read((void*) (localPlayer+0x128), &teamNumber, sizeof(int));
        csgo.Read((void*) (localPlayer+0x94), &(hack::myEntId), sizeof(int));
	}
	for (unsigned int i = 0; i < count; i++) {
		if (g_glow[i].m_pEntity != NULL) {
			Entity ent;

			if (csgo.Read(g_glow[i].m_pEntity, &ent, sizeof(Entity))) {
				if (ent.m_iTeamNum != 2 && ent.m_iTeamNum != 3) {
					g_glow[i].m_bRenderWhenOccluded = 1;
					g_glow[i].m_bRenderWhenUnoccluded = 0;
					continue;
				}
                entityInCrossHair = false;

				if (localPlayer != 0) {
					                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          if (ent.m_iTeamNum != teamNumber) {
						unsigned int crossHairId = 0;
						unsigned int entityId = 0;
						csgo.Read((void*) (localPlayer+0xBBD8), &crossHairId, sizeof(int)); // 0xB390 => 0xB3A0 => 0xB398 => 0xBBD8// +0x7C avec m_bhasdefuser
						csgo.Read((void*) (g_glow[i].m_pEntity + 0x94), &entityId, sizeof(int));

						if (crossHairId == entityId) {
							entityInCrossHair = true;
							}
					}
				}
				csgo.Write((void*) ((unsigned long) g_glow[i].m_pEntity + 0xECD), &spotted, sizeof(unsigned char));

				if (g_glow[i].m_bRenderWhenOccluded == 1)
					continue;

				g_glow[i].m_bRenderWhenOccluded = 1;
				g_glow[i].m_bRenderWhenUnoccluded = 0;
                int spottedByMask = 0;
                csgo.Read((void*)g_glow[i].m_pEntity+0xf10,&spottedByMask, sizeof(int));
                bool spottedByMe = (spottedByMask>>myEntId-1)&1;
				if (ent.m_iTeamNum == 2 || ent.m_iTeamNum == 3) {
					if (teamNumber == ent.m_iTeamNum) {
						g_glow[i].m_flGlowRed = colors[8];
						g_glow[i].m_flGlowGreen = colors[9];
						g_glow[i].m_flGlowBlue = colors[10];
						g_glow[i].m_flGlowAlpha = colors[11];
					} else {
						g_glow[i].m_flGlowRed = spottedByMe ? colors[4] : colors[0];
						g_glow[i].m_flGlowGreen = spottedByMe ? colors[5] : colors[1];
						g_glow[i].m_flGlowBlue = spottedByMe  ? colors[6] : colors[2];
						g_glow[i].m_flGlowAlpha = spottedByMe  ? colors[7] : colors[3];
					}
				}
			}
		}

		if (ShouldGlow) {
			size_t bytesToCutOffEnd = sizeof(hack::GlowObjectDefinition_t) - g_glow[i].writeEnd();
			size_t bytesToCutOffBegin = (size_t) g_glow[i].writeStart();
			size_t totalWriteSize = (sizeof(hack::GlowObjectDefinition_t) - (bytesToCutOffBegin + bytesToCutOffEnd));

			g_remote[writeCount].iov_base =
				((uint8_t*) data_ptr + (sizeof(hack::GlowObjectDefinition_t) * i)) + bytesToCutOffBegin;
			g_local[writeCount].iov_base = ((uint8_t*) &g_glow[i]) + bytesToCutOffBegin;
			g_remote[writeCount].iov_len = g_local[writeCount].iov_len = totalWriteSize;

			writeCount++;
		}
	}
    process_vm_writev(csgo.GetPid(), g_local, writeCount, g_remote, writeCount, 0);
}
void hack::trigger(){
    if(ShouldTrigger&&entityInCrossHair){
		XFlush(display);
        XTestFakeButtonEvent(display, Button1, true, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
        XTestFakeButtonEvent(display, Button1, false, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
        /*unsigned int attack = 0x5;
        unsigned int release = 0x4;
		unsigned int shooting;
        csgo.Read((void*) (csgo.m_addressOfForceAttack), &shooting, sizeof(int));
        if(shooting!=attack){
            cout<<"we should be shooting"<<endl;
            csgo.Write((void*) (csgo.m_addressOfForceAttack), &attack, sizeof(int));
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            csgo.Write((void*) (csgo.m_addressOfForceAttack), &release, sizeof(int));
        }*/
    }
}
bool hack::checkKeys(){
    if (csgo.IsRunning()) {
		XQueryKeymap(display, keys);
		for (unsigned i = 0; i < sizeof(keys); ++i) {
			if (keys[i] != lastkeys[i]) {
				// check which key got changed
				for (unsigned j = 0, test = 1; j < 8; ++j, test *= 2) {
					// if the key was pressed, and it wasn't before, print this
					if ((keys[i] & test) &&
							((keys[i] & test) != (lastkeys[i] & test))) {
						const int code = i * 8 + j;
						if (code == keycodeGlow) {
							ShouldGlow = !ShouldGlow;
							std::cout<<"ESP: "<<ShouldGlow<<endl;
						}
						else if (code == keycodeTrigger){
							ShouldTrigger = !ShouldTrigger;
							std::cout<<"Trigger: "<<ShouldTrigger<<endl;
						}
						else if (code == keycodeBhop){
							ShouldBhop = !ShouldBhop;
							std::cout<<"Bhop: "<< ShouldBhop<<endl;
						}
						else if (code==keycodeNoFlash){
                            NoFlash = !NoFlash;
                            std::cout<<"NoFlash: "<<NoFlash<<endl;
						}
						else if (code==keycodeRCS){
                            alwaysRCS=!alwaysRCS;
                            cout<<"AlwaysRCS: "<<alwaysRCS<<endl;
						}
						else if (code==keycodeRage){
                            rage=!rage;
                            cout<<"RAGE: "<<rage<<endl;
						}
					}
				}
			}

			lastkeys[i] = keys[i];
		}
		return true;
	}
	else
    {
        return false;
    }
}
void hack::init(){
    if (getuid() != 0) {
        cout<<"Cannot start as NON ROOT user.";
        return;
    }
    hack::display = XOpenDisplay(0);
    //XInitThreads();
    //read our cfg file
    try {
		cfg.readFile("settings.cfg");
	} catch (const FileIOException &fioex) {
		cout<<"I/O error while reading settings.cfg.";
	} catch (const ParseException &pex) {
		stringstream ss;
		ss << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError();
		cout<<ss.str()<<endl;
	}
	//configure our custom settings from settings.cfg
	keycodeGlow = XKeysymToKeycode(hack::display, XStringToKeysym(getConfigValue("ToggleGlow").c_str()));
	cout << "ESP Toggle = " << getConfigValue("ToggleGlow").c_str() << " Keycode: " << keycodeGlow << endl;
	keycodeTrigger = XKeysymToKeycode(hack::display, XStringToKeysym(getConfigValue("ToggleTrigger").c_str()));
	cout << "Trigger Toggle = " << getConfigValue("ToggleTrigger").c_str() << " Keycode: "<<keycodeTrigger<<endl;
	keycodeBhop = XKeysymToKeycode(hack::display, XStringToKeysym(getConfigValue("ToggleBhop").c_str()));
	cout << "Bhop Toggle = " << getConfigValue("ToggleBhop").c_str() <<" Keycode: " <<keycodeBhop<< endl;
	keycodeNoFlash = XKeysymToKeycode(hack::display, XStringToKeysym(getConfigValue("ToggleNoFlash").c_str()));
	cout << "NoFlash Toggle = " << getConfigValue("ToggleNoFlash").c_str() <<" Keycode: " <<keycodeNoFlash<< endl;
    keycodeRCS = XKeysymToKeycode(hack::display, XStringToKeysym(getConfigValue("ToggleRCS").c_str()));
	cout << "RCS Toggle = " << getConfigValue("ToggleRCS").c_str() <<" Keycode: " <<keycodeNoFlash<< endl;
    keycodeRage = XKeysymToKeycode(hack::display, XStringToKeysym(getConfigValue("ToggleRage").c_str()));
	cout << "Rage Toggle = " << getConfigValue("ToggleRage").c_str() <<" Keycode: " <<keycodeNoFlash<< endl;

	double enemyRed = ::atof(getConfigValue("enemyRed").c_str());
	double enemyGreen = ::atof(getConfigValue("enemyGreen").c_str());
	double enemyBlue = ::atof(getConfigValue("enemyBlue").c_str());
	double enemyAlpha = ::atof(getConfigValue("enemyAlpha").c_str());

	double enemyInCrosshairRed = ::atof(getConfigValue("enemyInCrosshairRed").c_str());
	double enemyInCrosshairGreen = ::atof(getConfigValue("enemyInCrosshairGreen").c_str());
	double enemyInCrosshairBlue = ::atof(getConfigValue("enemyInCrosshairBlue").c_str());
	double enemyInCrosshairAlpha = ::atof(getConfigValue("enemyInCrosshairAlpha").c_str());

	double allyRed = ::atof(getConfigValue("allyRed").c_str());
	double allyGreen = ::atof(getConfigValue("allyGreen").c_str());
	double allyBlue = ::atof(getConfigValue("allyBlue").c_str());
	double allyAlpha = ::atof(getConfigValue("allyAlpha").c_str());

	colors = new double[12] {
		enemyRed, enemyGreen, enemyBlue, enemyAlpha,
		enemyInCrosshairRed, enemyInCrosshairGreen, enemyInCrosshairBlue, enemyInCrosshairAlpha,
		allyRed, allyGreen, allyBlue, allyAlpha
	};

	while (true) {
        if (remote::FindProcessByName("csgo_linux64", &csgo))
            break;
        usleep(1000);
	}
	cout<<"Discovered CSGO with PID: "<<csgo.GetPid()<<endl;

	//we will search for the client address in memory
	client.start = 0;
	engine.start = 0;

	while (client.start == 0) {
		if (!csgo.IsRunning()) {
			cout<<"The game was closed before I could find the client and engine libraries inside of csgo";
			return;
		}

		csgo.ParseMaps();

		for (auto region : csgo.regions) {
			if (region.filename.compare("client_client.so") == 0 && region.executable) {
				client = region;
			}
			if (region.filename.compare("engine_client.so") == 0 && region.executable) {
				engine = region;
			}
		}

		usleep(500);
	}

	client.client_start = client.start;
	engine.client_start = engine.start;

	//find our pointers to memory
	void* foundGlowPointerCall = client.find(csgo,
		"\xE8\x00\x00\x00\x00\x48\x8b\x3d\x00\x00\x00\x00\xBE\x01\x00\x00\x00\xC7", // 2017-10-06
		"x????xxx????xxxxxx");
	unsigned long call = csgo.GetCallAddress(foundGlowPointerCall+1);

    m_addressOfGlowPointer = csgo.GetCallAddress((void*)(call+0x10));

    unsigned long foundLocalPlayerLea = (long)client.find(csgo,
		"\x48\x89\xe5\x74\x0e\x48\x8d\x05\x00\x00\x00\x00", //27/06/16
		"xxxxxxxx????");

	m_addressOfLocalPlayer = csgo.GetCallAddress((void*)(foundLocalPlayerLea+8));

	unsigned long foundAttackMov = (long)client.find(csgo,
		"\x44\x89\xe8\x83\xe0\x01\xf7\xd8\x83\xe8\x03\x45\x84\xe4\x74\x00\x21\xd0", //10/07/16
		"xxxxxxxxxxxxxxx?xx");

	m_addressOfForceAttack = csgo.GetCallAddress((void*)(foundAttackMov+20));

	unsigned long foundAlt1Mov = (long)client.find(csgo,
		"\x44\x89\xe8\xc1\xe0\x11\xc1\xf8\x1f\x83\xe8\x03\x45\x84\xe4\x74\x00\x21\xd0", //10/07/16
		"xxxxxxxxxxxxxxxx?xx");

	m_addressOfAlt1 = csgo.GetCallAddress((void*)(foundAlt1Mov+21));
    m_addressOfJump = m_addressOfAlt1-0x18;
	m_addressOfAlt2 = m_addressOfAlt1+0xC;

    unsigned long foundVangMov = (long)engine.find(csgo,
        "\x00\x00\x00\x00\x4a\x8b\x1c\x20\x48\x8b\x03\x48\x89\xdf\xff\x90\x00\x00\x00\x00\x41\x39\xc5", //Jul-12-17
         "????xxxxxxxxxxxx????xxx");

    basePointerOfViewAngle = csgo.GetCallAddress((void*)foundVangMov);

    unsigned long foundServerDetailOffsetMov = (long)engine.find(csgo,
        "\x44\x89\xe7\x45\x89\xa5\x00\x00\x00\x00\xe8",
        "xxxxxx????x");

    int serverDetailOffset=0x294;
    csgo.Read((void*)foundServerDetailOffsetMov+21,&serverDetailOffset,sizeof(int));
    //csgo.Read((void*)serverDetailOffsetAddress,&serverDetailOffset,sizeof(int));

    unsigned long foundServerDetailBaseMove = (long)engine.find(csgo,"\x90\x0f\x1f\x84\x00\x00\x00\x00\x00\x55\x48\x89\xe5\x48\x83\xec\x00\x48\x8b\x05", "xxxx????xxxxxxxx?xxx");

    unsigned long serverDetailBase = csgo.GetCallAddress((void*)foundServerDetailBaseMove+20);

    csgo.Read((void*)serverDetailBase,&addressServerDetail,sizeof(long));

    addressServerDetail+=serverDetailOffset;

    cout<<"client.start: "<<std::hex<<client.start<<endl;
    cout<<"engine.start: "<<engine.start<<endl;
	cout<<"GlowPointer address: "<<m_addressOfGlowPointer<<endl;
	cout<<"LocalPlayer pointer address: "<< m_addressOfLocalPlayer<<endl;
	cout<<"View angle base pointer address: " << basePointerOfViewAngle<<endl;
	cout<<"server detail offset: "<<serverDetailOffset<<endl;
	cout<<"addressServerDetail: "<<addressServerDetail<<endl;
    cout<<"jump address: "<< m_addressOfJump<<endl;
	cout<<"ForceAttack address: "<< m_addressOfForceAttack<<endl;
	cout<<"Alt1 address: "<< m_addressOfAlt1<<endl;
	cout<<"Alt2 address: "<< m_addressOfAlt2<<endl;

    //settings
	ShouldGlow = true;
	ShouldTrigger = false;
	ShouldBhop = true;
	NoFlash = true;
	alwaysRCS = false;
	rage=false;
    bone = ::atof(getConfigValue("bone").c_str());
	fov = ::atof(getConfigValue("fov").c_str());
	flashMax = ::atof(getConfigValue("flash_max").c_str());
	percentSmoothing = ::atof(getConfigValue("aim_smooth_percent").c_str());
	if(flashMax<0||flashMax>255)
    {
        flashMax=70;
    }

	spotted = 1;
	entityInCrossHair = false;
	isAiming=false;
}
