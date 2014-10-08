/// ----------------------------------------------------------- ///
/// this file implements a test case for register tracing       ///
/// ----------------------------------------------------------- ///

#include "qtrace-client-header.h"
#include <stdio.h>

#define target_ulong long unsigned int

void RegSim(target_ulong rax,
            target_ulong rcx,
            target_ulong rdx,
            target_ulong rbx,
            target_ulong rsp,
            target_ulong rbp,
            target_ulong rsi,
            target_ulong rdi,
            target_ulong r8,
            target_ulong r9,
            target_ulong r10,
            target_ulong r11,
            target_ulong r12,
            target_ulong r13,
            target_ulong r14,
            target_ulong r15,
            target_ulong rip)
{
#if 1 
    printf("0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n", 
            rax, 
            rcx, 
            rdx, 
            rbx, 
            rsp, 
            rbp,
            rsi, 
            rdi, 
            r8, 
            r9, 
            r10, 
            r11, 
            r12, 
            r13, 
            r14, 
            r15, 
            rip);
#endif
    return;
}

void InstructionCallBack(unsigned type)
{
    QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG,
                                  QTRACE_IPOINT_BEFORE, 
                                  QTRACE_IFUN, RegSim, 
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_RAX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_RCX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_RDX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_RBX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_RSP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_RBP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_RSI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_RDI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R8,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R9,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R10,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R11,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R12,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R13,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R14,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R15,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_RIP,
                                  QTRACE_END_ARG);
    return;
}

void StatsReset() { printf("REGSIM: StatsReset\n"); }
void StatsPrint() { printf("REGSIM: StatsPrint\n"); }

static int main(void) __attribute__((constructor));
int main(void)
{
   AddInstructionCallBack(InstructionCallBack);
   AddStatsResetCallBack(StatsReset);
   AddStatsPrintCallBack(StatsPrint);
   return 0;
}
