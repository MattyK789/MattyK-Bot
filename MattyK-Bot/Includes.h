#ifndef INCLUDES_H
#define INCLUDES_H

#include <string>
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <thread>

using namespace std;

struct CGlow
{
	DWORD pEntity;
	float r;
	float g;
	float b;
	float a;
	uint8_t unk1[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
	int GlowStyle;
	uint8_t unk2[10];
};

#endif