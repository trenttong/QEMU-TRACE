/// ----------------------------------------------------------- ///
/// this file implements a test case for register tracing       ///
/// ----------------------------------------------------------- ///

#include "qtrace-client-header.h"
#include <stdio.h>

#define REG_PRINT 1 

const char *regsim8lname = "RegSimPrint8l";
const char *regsim8hname = "RegSimPrint8h";
const char *regsim16name = "RegSimPrint16";
const char *regsim32name = "RegSimPrint32";
const char *regsim64name = "RegSimPrint64";

#if REG_PRINT != 0
#define RegSimPrint(size, type, name)                             \
static void RegSimPrint##size(const char* name,                   \
                              type rax,                           \
                              type rcx,                           \
                              type rdx,                           \
                              type rbx,                           \
                              type rsp,                           \
                              type rbp,                           \
                              type rsi,                           \
                              type rdi,                           \
                              type r8,                            \
                              type r9,                            \
                              type r10,                           \
                              type r11,                           \
                              type r12,                           \
                              type r13,                           \
                              type r14,                           \
                              type r15,                           \
                              type rip)                           \
{                                                                 \
    printf("%s %8x %8x %8x %8x %8x %8x %8x %8x %8x %8x %8x %8x %8x %8x %8x %8x %8x\n",       \
           name, rax, rcx, rdx, rbx, rsp, rbp, rsi, rdi, r8, r9,  \
           r10, r11, r12, r13, r14,                               \
           r15, rip);                                             \
}

#else 

#define RegSimPrint(size, type, name)                             \
static void RegSimPrint##size(const char* name,                   \
                              type rax,                           \
                              type rcx,                           \
                              type rdx,                           \
                              type rbx,                           \
                              type rsp,                           \
                              type rbp,                           \
                              type rsi,                           \
                              type rdi,                           \
                              type r8,                            \
                              type r9,                            \
                              type r10,                           \
                              type r11,                           \
                              type r12,                           \
                              type r13,                           \
                              type r14,                           \
                              type r15,                           \
                              type rip)                           \
{                                                                 \
}
#endif

RegSimPrint(8,  uint8_t,  regsim8name);
RegSimPrint(16, uint16_t, regsim16name);
RegSimPrint(32, uint32_t, regsim32name);
RegSimPrint(64, uint64_t, regsim64name);
#undef RegSimPrint

#define RegSimInst(position, misc)                                \
static void RegSim##position##Inst(uint64_t rax,                  \
                                   uint64_t rcx,                  \
                                   uint64_t rdx,                  \
                                   uint64_t rbx,                  \
                                   uint64_t rsp,                  \
                                   uint64_t rbp,                  \
                                   uint64_t rsi,                  \
                                   uint64_t rdi,                  \
                                   uint64_t r8,                   \
                                   uint64_t r9,                   \
                                   uint64_t r10,                  \
                                   uint64_t r11,                  \
                                   uint64_t r12,                  \
                                   uint64_t r13,                  \
                                   uint64_t r14,                  \
                                   uint64_t r15,                  \
                                   uint64_t rip,                  \
                                   uint32_t eax,                  \
                                   uint32_t ecx,                  \
                                   uint32_t edx,                  \
                                   uint32_t ebx,                  \
                                   uint32_t esp,                  \
                                   uint32_t ebp,                  \
                                   uint32_t esi,                  \
                                   uint32_t edi,                  \
                                   uint32_t r8d,                  \
                                   uint32_t r9d,                  \
                                   uint32_t r10d,                 \
                                   uint32_t r11d,                 \
                                   uint32_t r12d,                 \
                                   uint32_t r13d,                 \
                                   uint32_t r14d,                 \
                                   uint32_t r15d,                 \
                                   uint32_t eip,                  \
                                   uint16_t ax,                   \
                                   uint16_t cx,                   \
                                   uint16_t dx,                   \
                                   uint16_t bx,                   \
                                   uint16_t sp,                   \
                                   uint16_t bp,                   \
                                   uint16_t si,                   \
                                   uint16_t di,                   \
                                   uint16_t r8w,                  \
                                   uint16_t r9w,                  \
                                   uint16_t r10w,                 \
                                   uint16_t r11w,                 \
                                   uint16_t r12w,                 \
                                   uint16_t r13w,                 \
                                   uint16_t r14w,                 \
                                   uint16_t r15w,                 \
                                   uint16_t ip,                   \
                                   uint8_t  al,                   \
                                   uint8_t  cl,                   \
                                   uint8_t  dl,                   \
                                   uint8_t  bl,                   \
                                   uint8_t  spl,                  \
                                   uint8_t  bpl,                  \
                                   uint8_t  sil,                  \
                                   uint8_t  dil,                  \
                                   uint8_t  r8b,                  \
                                   uint8_t  r9b,                  \
                                   uint8_t  r10b,                 \
                                   uint8_t  r11b,                 \
                                   uint8_t  r12b,                 \
                                   uint8_t  r13b,                 \
                                   uint8_t  r14b,                 \
                                   uint8_t  r15b,                 \
                                   uint8_t  ah,                   \
                                   uint8_t  ch,                   \
                                   uint8_t  dh,                   \
                                   uint8_t  bh)                   \
                                                                  \
{                                                                 \
   printf("---- REGSIM: %s Instrumentation ---- \n", misc);       \
   RegSimPrint64(regsim64name, rax, rcx, rdx, rbx, rsp, rbp, rsi, \
                 rdi, r8, r9, r10, r11, r12, r13, r14, r15, rip); \
   RegSimPrint32(regsim32name, eax, ecx, edx, ebx, esp, ebp, esi, \
                 edi, r8d, r9d, r10d, r11d, r12d, r13d, r14d,     \
                 r15d, eip);                                      \
   RegSimPrint16(regsim16name, ax, cx, dx, bx, sp, bp, si, di,    \
                 r8w, r9w, r10w, r11w, r12w, r13w, r14w, r15w,    \
                 ip);                                             \
   RegSimPrint8(regsim8lname, al, cl, dl, bl, spl, bpl, sil, dil, \
                r8b, r9b, r10b, r11b, r12b, r13b, r14b, r15b, 0); \
   RegSimPrint8(regsim8hname, ah, ch, dh, bh,0, 0, 0, 0, 0, 0,    \
                0, 0, 0, 0, 0, 0, 0);                             \
   printf("**** REGSIM: %s Instrumentation ****\n\n", misc);      \
}                          

RegSimInst(Pre, "preinst")
RegSimInst(Pst, "pstinst")
#undef RegSimInst

void InstructionCallBack(unsigned type)
{
    QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG,
                                  QTRACE_IPOINT_BEFORE, 
                                  QTRACE_IFUN, RegSimPreInst, 
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
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EAX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_ECX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EDX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EBX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_ESP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EBP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_ESI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EDI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R8D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R9D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R10D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R11D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R12D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R13D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R14D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R15D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EIP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_AX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_CX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_SP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_SI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R8W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R9W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R10W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R11W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R12W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R13W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R14W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R15W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_IP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_AL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_CL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_SPL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BPL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_SIL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DIL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R8B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R9B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R10B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R11B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R12B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R13B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R14B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R15B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_AH,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_CH,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DH,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BH,
                                  QTRACE_END_ARG);

    QTRACE_INSTRUCTION_INSTRUMENT(QTRACE_BEGIN_ARG,
                                  QTRACE_IPOINT_AFTER, 
                                  QTRACE_IFUN, RegSimPstInst, 
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
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EAX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_ECX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EDX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EBX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_ESP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EBP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_ESI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EDI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R8D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R9D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R10D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R11D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R12D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R13D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R14D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R15D,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_EIP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_AX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_CX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BX,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_SP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_SI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DI,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R8W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R9W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R10W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R11W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R12W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R13W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R14W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R15W,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_IP,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_AL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_CL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_SPL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BPL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_SIL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DIL,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R8B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R9B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R10B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R11B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R12B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R13B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R14B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_R15B,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_AH,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_CH,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_DH,
                                  QTRACE_REGTRACE_VALUE,
                                  QTRACE_X86_BH,
                                  QTRACE_END_ARG);

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
