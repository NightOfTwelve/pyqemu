#ifndef _FLX_INSTRUMENT_
#define _FLX_INSTRUMENT_

#include "cpu-all.h"

#include <inttypes.h>


#define FLX_BLACKLIST_SIZE 256*256*256

#define userspace(x) (!(x&0x80000000))
#define kernelspace(x) (x&0x80000000)

#define FLX_SLOT_EMPTY  0
#define FLX_SLOT_ISCALL 1
#define FLX_SLOT_ISJMP  2

extern CPUState *current_environment;

typedef struct {
	// global flags
	uint8_t global_active;
	uint8_t python_active;

	// feature flags
	uint8_t bbltranslate_active;
	uint8_t bbltrace_active;
	uint8_t memtrace_active;
	uint8_t filter_active;
	uint8_t calltrace_active;
	uint8_t functiontrace_active;

	// specific opcode flags
	uint8_t syscall_active;
	uint8_t jmp_active;
	uint8_t call_active;
	uint8_t ret_active;

	// heuristics
	uint8_t caballero_active;
	uint8_t arithwindow_active;
	uint8_t functionentropy_active;
	uint8_t constsearch_active;
} FLX_STATE;

extern FLX_STATE flx_state;

void flxinstrument_init(void);
int flxinstrument_update_cr3(uint32_t old_cr3, uint32_t new_cr3);
int flxinstrument_call_event(uint32_t call_origin, uint32_t call_destination, uint32_t next_eip, uint32_t esp);
int flxinstrument_jmp_event(uint32_t jmp_source, uint32_t jmp_destination);
int flxinstrument_syscall_event(uint32_t eax);
int flxinstrument_ret_event(uint32_t eip, uint32_t new_eip);
int flxinstrument_breakpoint_event(uint32_t eip);
int flxinstrument_memtrace_event(uint32_t address, uint32_t value, uint8_t size, uint8_t iswrite);
int flxinstrument_bbltrace_event(uint32_t eip, uint32_t esp);
int flxinstrument_caballero_event(uint32_t eip, uint32_t icount, uint32_t arithcount);
int flxinstrument_arithwindow_event(uint32_t eip);
int flxinstrument_functiontrace_event(uint32_t eip, uint8_t type);
int flxinstrument_functionentropy_event(float entropychange, uint32_t eip);
int flxinstrument_constsearch_event(uint32_t eip, uint8_t* pattern, uint32_t len);

int flxinstrument_shutdown_event(void);

#endif