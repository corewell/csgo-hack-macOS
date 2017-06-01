#include "Cheat.h"

void assignOffsets()
{
	Offsets["m_iGlowIndex"] = 0xAC10;
	Offsets["GlowInfoOffset"] = 0x1983690;
	Offsets["PlayerBase"] = 0x111bc38;
	Offsets["LocalPlayer"] = 0x110fa68;
	Offsets["m_iHealth"] = 0x134;
	Offsets["m_iTeamNum"] = 0x128;
	Offsets["inCross"] = 0xbbd8;
	Offsets["inAttack"] = 0x1A76D80;
	Offsets["m_iFlashDuration"] = 0xABF8;
	Offsets["inAlt1"] = 0x1988928;
}

int main(int argc, const char * argv[])
{
	
	assignOffsets();
	
	printf("- csgo-macOS-Cheat v1.0 -\n");
	
	
	if (mem->g_cProc->mainPid == -1) {
		printf("> Error! CSGO not running.\n");
		return 0;
	} else {
		printf("> Found CSGO! PID: %i\n", mem->g_cProc->mainPid);
	}
	
	off_t moduleLength = 0;
	mach_vm_address_t moduleStartAddress;
	mem->g_cProc->getModule(mem->g_cProc->mainTask, &moduleStartAddress, &moduleLength, "/client.dylib");
	
	if (mem->g_cProc->mainTask)
		printf("> Module found!\n");
	
	
	uint64_t glowObjectLoopStartAddress = mem->read<uint64_t>(moduleStartAddress + Offsets["GlowInfoOffset"]);
	
	while (mem->g_cProc->mainPid != -1 && mem->g_cProc->mainTask != 0)
	{
		uint64_t playerAddress = mem->read<uint64_t>(moduleStartAddress + Offsets["LocalPlayer"]);

		noFlash(moduleStartAddress, playerAddress);
		Glow(moduleStartAddress, glowObjectLoopStartAddress, playerAddress);
		
		int inTrigger = mem->read<int>(moduleStartAddress + Offsets["inAlt1"]);
		if (inTrigger == 5)
		{
			Trigger(moduleStartAddress, playerAddress);
		}
		
		
		usleep(1500);
	}
	return 0;
}
