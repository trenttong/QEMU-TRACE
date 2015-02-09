/// ----------------------------------------------------------- ///
/// this file implements a test case for memory tracing         ///
/// ----------------------------------------------------------- ///

#include "qtrace-client-header.h"
#include <stdio.h>

static void MemFetchTrace(void *vma, unsigned msize)
{
    printf("msize is %d vma is 0x%lx\n", msize, (unsigned long)vma);
}

void InstructionCallBack(unsigned type)
{
    if (QTRACE_TEST_X86_FETCH(type))
    {
          QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG, 
                                        QTRACE_IPOINT_AFTER, 
                                        QTRACE_IFUN, MemFetchTrace,
                                        QTRACE_MEMTRACE_FETCH_VMA,
                                        QTRACE_MEMTRACE_FETCH_MSIZE,
                                        QTRACE_END_ARG); 
    }
    return;
}

void StatsReset() { printf("MEMTRACE: StatsReset\n"); }
void StatsPrint() { printf("MEMTRACE: StatsPrint\n"); }

int main(void)
{
   AddInstructionCallBack(InstructionCallBack);
   AddStatsResetCallBack(StatsReset);
   AddStatsPrintCallBack(StatsPrint);
   return 0;
}
