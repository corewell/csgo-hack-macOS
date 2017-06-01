#include <string>
#include <iostream>
#include <unistd.h>
#include <ApplicationServices/ApplicationServices.h>
#include <thread>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <mach/mach_traps.h>
#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach.h>
#include <mach-o/dyld_images.h>
#include <mach-o/loader.h>
#include <libproc.h>
#include <sys/stat.h>

std::map<std::string, uint64_t> Offsets;

struct Color {
	float red;
	float green;
	float blue;
	float alpha;
};

pid_t   mainPid;
task_t  mainTask;
