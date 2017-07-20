#ifndef __TYPES_H_
#define __TYPES_H_

struct Vector {
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
    QAngle m_angNetworkAngles; // 0x160
    Vector m_vecNetworkOrigin; // 0x16C
};

#endif
