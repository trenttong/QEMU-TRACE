/// ----------------------------------------------------------- ///
/// this file implements a test case for memory tracing         ///
/// ----------------------------------------------------------- ///

#define QTRACE_PRINT_OFF 1 

#include "qtrace-client-header.h"
#include <stdio.h>

#define QTRACE_PCTRACE_VPCTRACE 1 
#define QTRACE_PCTRACE_PPCTRACE 1 

static void VPCTracePreInst(void *pc)
{
    QTRACE_ARG_PRINT("VPCTracePreInst: pc is 0x%lx\n", (unsigned long)pc);
}

static void VPCTracePstInst(void *pc)
{
    QTRACE_ARG_PRINT("VPCTracePstInst: pc is 0x%lx\n", (unsigned long)pc);
}

static void PPCTracePreInst(void *pc)
{
    QTRACE_ARG_PRINT("PPCTracePreInst: pc is 0x%lx\n", (unsigned long)pc);
}

static void PPCTracePstInst(void *pc)
{
    QTRACE_ARG_PRINT("PPCTracePstInst: pc is 0x%lx\n", (unsigned long)pc);
}

void InstructionCallBack(unsigned type)
{
    /// -------------------------------------- ///
    /// instrumenting virtual program counter  ///
    /// -------------------------------------- ///
#if QTRACE_PCTRACE_VPCTRACE
    QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG, 
                                  QTRACE_IPOINT_BEFORE, 
                                  QTRACE_IFUN, VPCTracePreInst,
                                  QTRACE_PCTRACE_VMA,
                                  QTRACE_END_ARG); 

    QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG, 
                                  QTRACE_IPOINT_AFTER, 
                                  QTRACE_IFUN, VPCTracePstInst,
                                  QTRACE_PCTRACE_VMA,
                                  QTRACE_END_ARG); 
#endif

    /// -------------------------------------- ///
    /// instrumenting physical program counter ///
    /// -------------------------------------- ///
#if QTRACE_PCTRACE_PPCTRACE
    QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG, 
                                  QTRACE_IPOINT_BEFORE, 
                                  QTRACE_IFUN, PPCTracePreInst,
                                  QTRACE_PCTRACE_PMA,
                                  QTRACE_END_ARG); 

    QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG, 
                                  QTRACE_IPOINT_AFTER, 
                                  QTRACE_IFUN, PPCTracePstInst,
                                  QTRACE_PCTRACE_PMA,
                                  QTRACE_END_ARG); 
#endif

    /* done */
    return;
}

void StatsReset() { QTRACE_ARG_PRINT("PCTRACE: StatsReset\n"); }
void StatsPrint() { QTRACE_ARG_PRINT("PCTRACE: StatsPrint\n"); }

int main(void)
{
   AddInstructionCallBack(InstructionCallBack);
   AddStatsResetCallBack(StatsReset);
   AddStatsPrintCallBack(StatsPrint);
   return 0;
}
