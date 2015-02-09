/// ----------------------------------------------------------- ///
/// this file implements a test case for memory tracing         ///
/// ----------------------------------------------------------- ///

#include "qtrace-client-header.h"
#include <stdio.h>

static unsigned long long kerncount, usercount, totalcount;

static void KernCount(void)
{
    ++ kerncount; ++totalcount;
    printf("executed %lld instructions, %lld in ring 0\n", totalcount, kerncount);
}

static void UserCount(void)
{
    ++ usercount; ++totalcount;
    printf("executed %lld instructions, %lld in ring 3\n", totalcount, usercount);
}

void InstructionCallBack(unsigned type)
{
    if (TEST_QTRACE_IS_X86_KERN(type))
    {
          QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG, 
                                        QTRACE_IPOINT_BEFORE, 
                                        QTRACE_IFUN, KernCount,
                                        QTRACE_END_ARG); 
    }

    if (TEST_QTRACE_IS_X86_USER(type))
    {
          QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG, 
                                        QTRACE_IPOINT_BEFORE, 
                                        QTRACE_IFUN, UserCount,
                                        QTRACE_END_ARG); 
    }

    return;
}

void StatsReset() { printf("ICOUNT: StatsReset\n"); }
void StatsPrint() { printf("ICOUNT: StatsPrint\n"); }

static int main(void) __attribute__((constructor));
int main(void)
{
   AddInstructionCallBack(InstructionCallBack);
   AddStatsResetCallBack(StatsReset);
   AddStatsPrintCallBack(StatsPrint);
   return 0;
}
