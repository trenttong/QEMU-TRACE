/// ----------------------------------------------------------- ///
/// this file implements a test case for memory tracing         ///
/// ----------------------------------------------------------- ///

#include "qtrace-client-header.h"
#include <stdio.h>

#define target_ulong unsigned long long

static void CacheSim(void *vma, unsigned msize)
{
    printf("msize is %d vma is 0x%lx\n", msize, (unsigned long)vma);
}

void InstructionCallBack(unsigned type)
{
    if (QTRACE_TEST_FETCH(type))
    {
          QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG, 
                                        QTRACE_IPOINT_AFTER, 
                                        QTRACE_IFUN, CacheSim,
                                        QTRACE_MEMTRACE_FETCH_VMA,
                                        QTRACE_MEMTRACE_FETCH_MSIZE,
                                        QTRACE_END_ARG); 
    }
    return;
}

void StatsReset() { printf("MEMSIM: StatsReset\n"); }
void StatsPrint() { printf("MEMSIM: StatsPrint\n"); }

static int main(void) __attribute__((constructor));
int main(void)
{
   AddInstructionCallBack(InstructionCallBack);
   AddStatsResetCallBack(StatsReset);
   AddStatsPrintCallBack(StatsPrint);
   return 0;
}
