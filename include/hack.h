#ifndef __HACK_H_
#define __HACK_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include <X11/extensions/XTest.h>

#include <chrono>
#include <thread>
#include <stddef.h>
#include <libconfig.h++>
#include <iostream>
#include <atomic>
#include <cmath>

#include "remote.h"
#include "types.h"

using namespace std;
using namespace libconfig;

#define TEAM_SPECTATOR			1

#define LIFE_ALIVE			0 // alive
#define LIFE_DYING			1 // playing death animation or still falling off of a ledge waiting to hit ground
#define LIFE_DEAD			2 // dead. lying still.
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

#define ON_GROUND (1<<0)

#define MAX_TRAIL_LENGTH		30
#define MAX_PLAYER_NAME_LENGTH		128

class hack {

	template<class T> class CUtlVector {
	public:
		T* DataPtr; // 0000 (054612C0)
		unsigned int Max; // 0004 (054612C4)
		unsigned int unk02; // 0008 (054612C8)
		unsigned int Count; // 000C (054612CC)
		unsigned int DataPtrBack; // 0010 (054612D0)
	};
    public:
	struct GlowObjectDefinition_t {
		bool ShouldDraw( int nSlot ) const {
			return m_pEntity && ( m_nSplitScreenSlot == -1 || m_nSplitScreenSlot == nSlot ) && ( m_bRenderWhenOccluded || m_bRenderWhenUnoccluded );
		}

		bool IsUnused() const {
			return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE;
		}

		long writeStart() {
			return (long(&(this)->m_flGlowRed) - long(this));
		}

		long writeEnd() {
			return (long(&(this)->unk2) - long(this));
		}

		void* m_pEntity;
		float m_flGlowRed;
		float m_flGlowGreen;
		float m_flGlowBlue;
		float m_flGlowAlpha;
		uint8_t unk0[16];
		bool m_bRenderWhenOccluded : 8;
		bool m_bRenderWhenUnoccluded : 8;
		bool m_bFullBloomRender : 8;
		unsigned char unk1;
		int m_nFullBloomStencilTestValue;
		int m_nSplitScreenSlot;
		int m_nNextFreeSlot;
		unsigned char unk2[4];

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	}; // sizeof() == 0x34

	class CGlowObjectManager
	{
	public:
		CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions; // 0000
		int m_nFirstFreeSlot; // 0014 (054612D4)
		unsigned int unk1; // 0018 (054612D8)
		unsigned int unk2; // 001C (054612DC)
		unsigned int unk3; // 0020 (054612E0)
		unsigned int unk4; // 0024 (054612E4)
		unsigned int unk5; // 0028 (054612E8)
	};

public:
    void init();
    bool checkKeys();
    void bhop();
    void noFlash();
    bool glow();
    void trigger();
    void aim();
    void setFov();
    void setHands();

    void clampAngle(QAngle *angle);
    void setVAng(QAngle *newAngle);
    QAngle calcAngle(Vector *source, Vector *target);
    BoneMatrix getBones(void* bonePtr, int bone);
    void Smoothing(QAngle* source,QAngle* target);

    Entity entities[64];
    void* bonePtrs[64];

    remote::Handle csgo;
    Display* display;
    char keys[32];
    char lastkeys[32];
    remote::MapModuleMemoryRegion client;
    remote::MapModuleMemoryRegion engine;

    double* colors;
	int keycodeBhop;
    int keycodeTrigger;
	int keycodeGlow;
	int keycodeNoFlash;
	int keycodeRage;
	int keycodeRCS;
    unsigned char spotted;
    int toggleOn = 5;
    int toggleOff = 4;

    //
    std::atomic<bool> entityInCrossHair;
    int myEntId;
    QAngle viewAngle;//my view angle
    QAngle oldPunch;
    QAngle bulletViewAngle;
    QAngle newAngle;
    QAngle punch;
    QAngle aimDelta;
    QAngle punchDelta;
    Vector theirPos;
    Vector myPos;
    Entity myEnt;

    bool foundTarget;
    bool shouldShoot;
    bool isAiming;
    bool acquiring;

    Entity *closestEnt;
    Entity *targetedEnt;
    float lowestDistance;
    float closestFootDistance;
    unsigned int idclosestEnt;


    //static addresses
    unsigned long m_addressOfGlowPointer;
    unsigned long m_addressOfLocalPlayer;
    unsigned long m_addressOfForceAttack;
    unsigned long m_addressOfAlt1;
    unsigned long m_addressOfAlt2;
    unsigned long m_addressOfJump;
    unsigned long basePointerOfViewAngle;
    unsigned long addressServerDetail;

    //dynamic addresses
    unsigned long addressOfViewAngle;
    //std::atomic<unsigned long> localPlayer;

    //settings
    bool ShouldGlow;
    bool ShouldTrigger;
    bool ShouldBhop;
    bool NoFlash;
    bool alwaysRCS;
    int bone;
    float flashMax;
    float fov;
    bool rage;
    float percentSmoothing;
    int viewFov;
    bool noHands;
};

#endif
