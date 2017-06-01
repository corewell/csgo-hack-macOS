
#include "Memory.cpp"

std::map<std::string, uint64_t> Offsets;



struct Color {
	float red;
	float green;
	float blue;
	float alpha;
};



void Glow(mach_vm_address_t moduleStart, mach_vm_address_t glowStartAddress, mach_vm_address_t playerAddress)
{
	int iTeamNum = mem->read<int>(playerAddress + Offsets["m_iTeamNum"]);
	for (int i = 0; i < 60; i++) {
		uint64_t memoryAddress  = mem->read<uint64_t>(moduleStart + Offsets["PlayerBase"] + 0x20 * i);
		
		if (memoryAddress <= 0x0){
			continue;
		}

		if (true)
		{
			int glowIndex		   = mem->read<int>(memoryAddress + Offsets["m_iGlowIndex"]);
			int health			  = mem->read<int>(memoryAddress + Offsets["m_iHealth"]);
			int playerTeamNum	   = mem->read<int>(memoryAddress + Offsets["m_iTeamNum"]);
			
			if (playerTeamNum == 0) {
				continue;
			}
			
			if (health == 0) {
				health = 100;
			}
			
			Color color = {float((100 - health) / 100.0), float((health) / 100.0), 0.0f, 0.8f};
			Color white = {255.0f,255.0f, 255.0f, 0.5f};
			
			
			uint64_t glowBase = glowStartAddress + (0x40 * glowIndex);
			
			mem->write<bool>(glowBase + 0x28, 5);
			if (playerTeamNum == iTeamNum) {
				mem->write<Color>(glowBase + 0x8, white);
			} else
			{
				mem->write<Color>(glowBase + 0x8, color);
			}
		}
		
		
	}
}


void Trigger(mach_vm_address_t moduleStart, mach_vm_address_t playerAddress){
	
	int CrossHairID = mem->read<int>(playerAddress + Offsets["inCross"]);
	int iTeamNum = mem->read<int>(playerAddress + Offsets["m_iTeamNum"]);


	// Dont shoot at nothing or weapons
	if (CrossHairID == 0 || CrossHairID > 60) {
		return;
	}
	
	uint64_t enemyAddress  = mem->read<uint64_t>(moduleStart + Offsets["PlayerBase"] + 0x20 * (CrossHairID - 1));
	
	// Sanity check
	if (enemyAddress <= 0x0){
		return;
	}
	
	int playerTeam = mem->read<int>(enemyAddress + Offsets["m_iTeamNum"]);
	
	if (playerTeam == iTeamNum || playerTeam == 0) {
		return;
	}
	
	while(CrossHairID != 0 && playerTeam != iTeamNum && playerTeam != 0){
		
		mem->write(moduleStart + Offsets["inAttack"], 5);
		usleep(1000);
		mem->write(moduleStart + Offsets["inAttack"], 4);
		
		CrossHairID = mem->read<int>(playerAddress + Offsets["inCross"]);
		
		if (CrossHairID == 0) {
			break;
		}
		
		enemyAddress	= mem->read<uint64_t>(moduleStart + Offsets["PlayerBase"] + 0x20 * (CrossHairID - 1));
		playerTeam	  = mem->read<int>(enemyAddress + 0x128);
	}
}

void noFlash(mach_vm_address_t moduleStart, mach_vm_address_t playerAddress)
{
	int flashed = mem->read<int>(playerAddress + Offsets["m_iFlashDuration"]);
	if(flashed > 0) {
		mem->write<int>(playerAddress + Offsets["m_iFlashDuration"], 0);
	}
		
}