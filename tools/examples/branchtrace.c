/// ----------------------------------------------------------- ///
/// this file implements a test case for register tracing       ///
/// ----------------------------------------------------------- ///

#define QTRACE_PRINT_OFF 1 

#include "qtrace-client-header.h"
#include <stdio.h>

#define QTRACE_BRANCHTRACE_PCTRACE 1 
#define QTRACE_BRANCHTRACE_NVTRACE 1 

PageWalkContext ptw;

void PCTrace(void *rip)
{
    QTRACE_TRANSLATE_VIRTUAL_ADDR(rip, QTRACE_PAGE_EXEC, &ptw);
    printf("BRANCHTRACE-using-pc: 0x%lx\n", (unsigned long) rip);
    printf("BRANCHTRACE-using-xlate: 0x%lx\n", (unsigned long) ptw.paddr);
}

void BranchTrace(void* rip)
{
    printf("BRANCHTRACE-native: 0x%lx\n", (unsigned long) rip);
}

void InstructionCallBack(unsigned type)
{
    /// -------------------------------------------------------- ///
    /// instrumenting virtual program counter and brancg target  ///
    /// -------------------------------------------------------- ///
    if (QTRACE_TEST_X86_JMP(type))
    {
#if QTRACE_BRANCHTRACE_PCTRACE
        QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG,
                                      QTRACE_IPOINT_AFTER, 
                                      QTRACE_IFUN, PCTrace, 
                                      QTRACE_REGTRACE_VALUE,
                                      QTRACE_X86_RIP, 
                                      QTRACE_END_ARG);
#endif

#if QTRACE_BRANCHTRACE_NVTRACE
        QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG,
                                      QTRACE_IPOINT_AFTER, 
                                      QTRACE_IFUN, BranchTrace, 
                                      QTRACE_BRANCHTRACE_TARGET,
                                      QTRACE_END_ARG);
#endif
    }
    return;
}

void StatsReset() { printf("BRANCHTRACE: StatsReset\n"); }
void StatsPrint() { printf("BRANCHTRACE: StatsPrint\n"); }

int main(void)
{
   AddInstructionCallBack(InstructionCallBack);
   AddStatsResetCallBack(StatsReset);
   AddStatsPrintCallBack(StatsPrint);
   return 0;
}
