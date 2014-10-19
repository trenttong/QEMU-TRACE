/// ----------------------------------------------------------- ///
/// this file implements a test case for register tracing       ///
/// ----------------------------------------------------------- ///

#include "qtrace-client-header.h"
#include <stdio.h>

#define BRANCH_PRINT 1 

#define target_ulong long unsigned int

void BranchSim(target_ulong prip, target_ulong beip)
{
    printf("BRANCHSIM: 0x%lx 0x%lx\n", prip, beip); 
}

void InstructionCallBack(unsigned type)
{
    if (QTRACE_TEST_JMP(type))
    {
      QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG,
                                    QTRACE_IPOINT_BEFORE, 
                                    QTRACE_IFUN, BranchSim, 
                                    QTRACE_REGTRACE_VALUE,
                                    QTRACE_X86_RIP, 
                                    QTRACE_BRANCHTRACE_TARGET,
                                    QTRACE_END_ARG);
    }
    return;
}

void StatsReset() { printf("BRANCHSIM: StatsReset\n"); }
void StatsPrint() { printf("BRANCHSIM: StatsPrint\n"); }

static int main(void) __attribute__((constructor));
int main(void)
{
   AddInstructionCallBack(InstructionCallBack);
   AddStatsResetCallBack(StatsReset);
   AddStatsPrintCallBack(StatsPrint);
   return 0;
}
