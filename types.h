#ifndef __TYPES_H_
#define __TYPES_H_

struct Vector{
    float x;
    float y;
    float z;
};
struct Color {
    unsigned char _color[4];
};

struct Vector2D {
    float x, y;
};

struct QAngle {
    float x; // Pitch
    float y; // Yaw
    float z; // Roll
};

struct BoneMatrix {
    char __buff_0x00[0xC];//0x00
    float x;//0xC
    char __buff_0x10[0xC];//0x10
    float y;//0x1c
    char __buff_0x20[0xC];//0x20
    float z;//0x2c
};

struct Entity {
    char __buf_0x00[0x94]; // 0x0
    int ID;// 0x94
    char __buf_0x98[0x34]; // 0x98
    Vector m_vecAbsVelocity; // 0xCC
    Vector m_vecAbsOrigin; // 0xD8
    Vector m_vecOrigin; // 0xE4
    Vector m_vecAngVelocity; // 0xF0
    QAngle m_angAbsRotation; // 0xFC
    QAngle m_angRotation; // 0x108
    float m_flGravity; // 0x114
    float m_flProxyRandomValue; // 0x118
    int m_iEFlags; // 0x11C
    unsigned char m_nWaterType; // 0x120
    unsigned char m_bDormant; // 0x121
    char __buf_0x122[0x2]; // 0x122
    float m_fEffects; // 0x124
    int m_iTeamNum; // 0x128
    int m_iPendingTeamNum; // 0x12C
    char __buf_0x130[0x4]; // 0x130
    int m_iHealth; // 0x134
    int m_fFlags; // 0x138
    Vector m_vecViewOffset; // 0x13C
    Vector m_vecVelocity; // 0x148
    Vector m_vecBaseVelocity; // 0x154
    QAngle m_angNetworkAngles   ; // 0x160
    Vector m_vecNetworkOrigin; // 0x16C
};

enum WeaponIndex {
    WEAPON_NONE,
    WEAPON_DEAGLE_REVOLVER = 1,
    WEAPON_DUAL = 2,
    WEAPON_FIVE7 = 3,
    WEAPON_GLOCK = 4,
    WEAPON_AK47 = 7,
    WEAPON_AUG = 8,
    WEAPON_AWP = 9,
    WEAPON_FAMAS = 10,
    WEAPON_G3SG1 = 11,
    WEAPON_GALIL = 13,
    WEAPON_M249 = 14,
    WEAPON_M4A1 = 16,
    WEAPON_MAC10 = 17,
    WEAPON_P90 = 19,
    WEAPON_UMP = 24,
    WEAPON_XM1014 = 25,
    WEAPON_BIZON = 26,
    WEAPON_MAG7 = 27,
    WEAPON_NEGEV = 28,
    WEAPON_SAWEDOFF = 29,
    WEAPON_TEC9 = 30,
    WEAPON_TASER = 31,
    WEAPON_HKP2000 = 32,
    WEAPON_MP7 = 33,
    WEAPON_MP9 = 34,
    WEAPON_NOVA = 35,
    WEAPON_P250_CZ75 = 36,
    WEAPON_SCAR20 = 38,
    WEAPON_SG553 = 39,
    WEAPON_SSG08 = 40,
    WEAPON_KNIFEGG = 41,
    WEAPON_KNIFE = 42,
    WEAPON_FLASHBANG = 43,
    WEAPON_HEGRENADE = 44,
    WEAPON_SMOKE = 45,
    WEAPON_T_MOLOTOV = 46,
    WEAPON_DECOY = 47,
    WEAPON_CT_MOLOTOV = 48,
    WEAPON_C4 = 49,
    WEAPON_MAX
};

struct EntityInfo{
    Entity entity;
    void* entityPtr;
};

struct EntityToScreen{
    Vector head;
    Vector origin;
    EntityInfo entityInfo;
    //string name;
    int health;
    bool scoped;
    bool myTeam;
    bool defusing;
    float lby;
};

#endif
