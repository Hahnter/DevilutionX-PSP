#ifdef USE_SDL3
#include <SDL3/SDL.h>
#else
#include <SDL.h>
#include <SDL_main.h>
#endif

#ifdef __SWITCH__
#include "platform/switch/network.h"
#include "platform/switch/random.hpp"
#include "platform/switch/romfs.hpp"
#endif
#ifdef __3DS__
#include "platform/ctr/system.h"
#endif
#ifdef __vita__
#include "platform/vita/network.h"
#include "platform/vita/random.hpp"
#endif
#ifdef NXDK
#include <nxdk/mount.h>
#endif
#ifdef PSP
#include <cstdio>
#include <pspdebug.h>
#include <pspiofilemgr.h>
#include <pspthreadman.h>
#endif

#ifdef GPERF_HEAP_MAIN
#include <gperftools/heap-profiler.h>
#endif

#include "diablo.h"

#if !defined(__APPLE__)
extern "C" const char *__asan_default_options() // NOLINT(bugprone-reserved-identifier, readability-identifier-naming)
{
	return "halt_on_error=0";
}
#endif

#ifdef PSP
namespace {
void PspExceptionHandler(PspDebugRegBlock *regs)
{
	// Use a fixed buffer and path: the C++ heap may be damaged at the fault.
	char report[256];
	const int length = std::snprintf(report, sizeof(report),
	    "DevilutionX PSP exception\nEPC=%08X RA=%08X BadVAddr=%08X Cause=%08X Thread=%08X\n",
	    static_cast<unsigned>(regs->epc), static_cast<unsigned>(regs->r[31]),
	    static_cast<unsigned>(regs->badvaddr), static_cast<unsigned>(regs->cause),
	    static_cast<unsigned>(sceKernelGetThreadId()));
	if (length > 0) {
		const SceUID file = sceIoOpen("ms0:/devx-crash.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		if (file >= 0) {
			const int bytes = length < static_cast<int>(sizeof(report)) ? length : static_cast<int>(sizeof(report)) - 1;
			sceIoWrite(file, report, bytes);
			sceIoClose(file);
		}
	}
	pspDebugScreenInit();
	pspDebugDumpException(regs);
	for (;;)
		sceKernelDelayThread(1000000);
}
} // namespace
#endif

extern "C" int main(int argc, char **argv)
{
#ifdef PSP
	pspDebugInstallErrorHandler(PspExceptionHandler);
#endif
#ifdef __SWITCH__
	switch_romfs_init();
	switch_enable_network();
#ifdef PACKET_ENCRYPTION
	randombytes_switchrandom_init();
#endif
#endif
#ifdef __3DS__
	ctr_sys_init();
#endif
#ifdef __vita__
	vita_enable_network();
#ifdef PACKET_ENCRYPTION
	randombytes_vitarandom_init();
#endif
#endif
#ifdef NXDK
	nxMountDrive('E', "\\Device\\Harddisk0\\Partition1\\");
#endif
#ifdef GPERF_HEAP_MAIN
	HeapProfilerStart("main");
#endif
	const int result = devilution::DiabloMain(argc, argv);
#ifdef GPERF_HEAP_MAIN
	HeapProfilerStop();
#endif
	return result;
}
