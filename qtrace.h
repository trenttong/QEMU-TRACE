/*
 *  QTRACE header - mostly macros. 
 *
 *  Copyright (c) 2003-2005 Fabrice Bellard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QTRACE_H
#define QTRACE_H

#include "qtrace-common.h"
#include <math.h>

/// ------------------------------------------------ ///
/// instruction types. 
/// ------------------------------------------------ ///
#define QTRACE_IS_USER            (1<<0)
#define QTRACE_IS_KERN            (1<<1)
#define QTRACE_IS_FETCH           (1<<2)
#define QTRACE_IS_STORE           (1<<3)
#define QTRACE_IS_JMP             (1<<4)
#define QTRACE_IS_CALL            (1<<5)
#define QTRACE_IS_RETURN          (1<<6)
#define QTRACE_IS_SYSENTER        (1<<7)
#define QTRACE_IS_SYSEXIT         (1<<8)
#define QTRACE_IS_SYSCALL         (1<<9)
#define QTRACE_IS_SYSRET          (1<<10)
#define QTRACE_IS_CPUID           (1<<11)
#define QTRACE_IS_HLT             (1<<12)
#define QTRACE_IS_INVLPGA         (1<<13)
#define QTRACE_IS_MONITOR         (1<<14)
#define QTRACE_IS_MWAIT           (1<<15)
#define QTRACE_IS_INVD            (1<<16)
#define QTRACE_IS_PREFETCH        (1<<17)
#define QTRACE_IS_NOP             (1<<18)
#define QTRACE_IS_MEMFENCE        (1<<19)
#define QTRACE_IS_CLFLUSH         (1<<20)
#define QTRACE_IS_RDTSC           (1<<21)
#define QTRACE_IS_SSE             (1<<22)
#define QTRACE_IS_FLOAT           (1<<23)
#define QTRACE_IS_COND            (1<<24)
#define QTRACE_IS_ARITHLOGIC      (1<<25)
#define QTRACE_IS_INDIRECT        (1<<26)

#define QTRACE_TEST_USER(X)       ((X&QTRACE_IS_USER))
#define QTRACE_TEST_KERN(X)       ((X&QTRACE_IS_KERN))
#define QTRACE_TEST_FETCH(X)      ((X&QTRACE_IS_FETCH))
#define QTRACE_TEST_STORE(X)      ((X&QTRACE_IS_STORE))
#define QTRACE_TEST_JMP(X)        ((X&QTRACE_IS_JMP))
#define QTRACE_TEST_CALL(X)       ((X&QTRACE_IS_CALL))
#define QTRACE_TEST_RETURN(X)     ((X&QTRACE_IS_RETURN)) 
#define QTRACE_TEST_SYSENTER(X)   ((X&QTRACE_IS_SYSENTER))
#define QTRACE_TEST_SYSEXIT(X)    ((X&QTRACE_IS_SYSEXIT))
#define QTRACE_TEST_SYSCALL(X)    ((X&QTRACE_IS_SYSCALL))
#define QTRACE_TEST_SYSRET(X)     ((X&QTRACE_IS_SYSRET))
#define QTRACE_TEST_CPUID(X)      ((X&QTRACE_IS_CPUID))
#define QTRACE_TEST_HLT(X)        ((X&QTRACE_IS_HLT))
#define QTRACE_TEST_INVLPGA(X)    ((X&QTRACE_IS_INVLPGA))
#define QTRACE_TEST_MONITOR(X)    ((X&QTRACE_IS_MONITOR))
#define QTRACE_TEST_MWAIT(X)      ((X&QTRACE_IS_MWAIT))
#define QTRACE_TEST_INVD(X)       ((X&QTRACE_IS_INVD))
#define QTRACE_TEST_PREFETCH(X)   ((X&QTRACE_IS_PREFETCH))
#define QTRACE_TEST_NOP(X)        ((X&QTRACE_IS_NOP))
#define QTRACE_TEST_MEMFENCE(X)   ((X&QTRACE_IS_MEMFENCE))
#define QTRACE_TEST_CLFLUSH(X)    ((X&QTRACE_IS_CLFLUSH))
#define QTRACE_TEST_RDTSC(X)      ((X&QTRACE_IS_RDTSC)) 
#define QTRACE_TEST_SSE(X)        ((X&QTRACE_IS_SSE))
#define QTRACE_TEST_COND(X)       ((X&QTRACE_IS_COND))
#define QTRACE_TEST_ARITHLOGIC(X) ((X&QTRACE_IS_ARITHLOGIC)) 
#define QTRACE_TEST_INDIRECT(X)   ((X&QTRACE_IS_INDIRECT)) 

/// ------------------------------------------------ ///
/// instrumentation IDs and flags. 
/// ------------------------------------------------ ///
#define QTRACE_RESET_INST_TYPE_FLAG(s)              (s->qtrace_insnflags=0)
#define QTRACE_ADD_INST_TYPE_FLAG(s,flg)            (s->qtrace_insnflags|=(flg))
#define QTRACE_SUB_INST_TYPE_FLAG(s,flg)            (s->qtrace_insnflags&=~(flg))
#define QTRACE_ADD_COND_INST_TYPE_FLAG(s,flg,c)     {if(c) QTRACE_ADD_INST_TYPE_FLAG(s, flg);}
#define QTRACE_SUB_COND_INST_TYPE_FLAG(s,flg,c)     {if(c) QTRACE_SUB_INST_TYPE_FLAG(s, flg);}
#define RESERVE_INSTRUMENT_CONTEXT_ARGUMENT(ictx)   {ictx->iargs[ictx->ciarg++] = 0;}

/// ------------------------------------------- 
/// general facility.
/// ------------------------------------------- 
#define QTRACE_MAX_ARGS                             (128)
#define QTRACE_IFUN                                 (1<<0)
#define QTRACE_IPOINT_BEFORE                        (1<<1)
#define QTRACE_IPOINT_AFTER                         (1<<2)
#define QTRACE_PROCESS_UPID                         (1<<3)
#define QTRACE_BEGIN_ARG                            QTRACE_MAX_ARGS
#define QTRACE_END_ARG                              (1<<4)
#define QTRACE_PREINST(ictx)                        (ictx->ipoint == QTRACE_IPOINT_BEFORE)
#define QTRACE_PSTINST(ictx)                        (ictx->ipoint == QTRACE_IPOINT_AFTER)

/// ------------------------------------------- 
/// memory tracing.
/// ------------------------------------------- 
#define QTRACE_MEMTRACE_FETCH_VMA                   (1<<5)
#define QTRACE_MEMTRACE_FETCH_PMA                   (1<<6) 
#define QTRACE_MEMTRACE_FETCH_MSIZE                 (1<<7)     
#define QTRACE_MEMTRACE_FETCH_VALUE                 (1<<8)     
#define QTRACE_MEMTRACE_FETCH_VPMA                  (QTRACE_MEMTRACE_FETCH_VMA | QTRACE_MEMTRACE_FETCH_PMA)
#define QTRACE_MEMTRACE_STORE_VMA                   (1<<9)
#define QTRACE_MEMTRACE_STORE_PMA                   (1<<10) 
#define QTRACE_MEMTRACE_STORE_MSIZE                 (1<<11)     
#define QTRACE_MEMTRACE_STORE_VALUE                 (1<<12)     
#define QTRACE_MEMTRACE_STORE_VPMA                  (QTRACE_MEMTRACE_STORE_VMA | QTRACE_MEMTRACE_STORE_PMA)
/* 4 internal values */
#define QTRACE_MEMTRACE_FETCH_PREOP_VALUE           (1<<13)
#define QTRACE_MEMTRACE_FETCH_PSTOP_VALUE           (1<<14)
#define QTRACE_MEMTRACE_STORE_PREOP_VALUE           (1<<15)
#define QTRACE_MEMTRACE_STORE_PSTOP_VALUE           (1<<16)

/* shift away from the last N bits. they are used by QEMU */
#define QTRACE_MEMTRACE_BITS                        ((int)(ceil(log2(NB_MMU_MODES)))) 
#define QTRACE_ADD_MEMTRACE(index,x)                (((index) | (x << QTRACE_MEMTRACE_BITS))) 
#define QTRACE_EXT_MEMTRACE(index)                  ((index >> QTRACE_MEMTRACE_BITS)) 
#define QTRACE_EXT_MEMINDEX(index)                  ((index & ((1<<QTRACE_MEMTRACE_BITS)-1))) 

#define QTRACE_RESET_MEMTRACE_FETCH_ADDRS(x)        (x &= (~QTRACE_MEMTRACE_FETCH_VPMA)) 
#define QTRACE_RESET_MEMTRACE_FETCH_MSIZE(x)        (x &= (~QTRACE_MEMTRACE_FETCH_MSIZE))
#define QTRACE_RESET_MEMTRACE_FETCH_PREOP_VALUE(x)  (x &= (~QTRACE_MEMTRACE_FETCH_PREOP_VALUE))
#define QTRACE_RESET_MEMTRACE_FETCH_PSTOP_VALUE(x)  (x &= (~QTRACE_MEMTRACE_FETCH_PSTOP_VALUE))
#define QTRACE_RESET_MEMTRACE_STORE_ADDRS(x)        (x &= (~QTRACE_MEMTRACE_STORE_VPMA)) 
#define QTRACE_RESET_MEMTRACE_STORE_MSIZE(x)        (x &= (~QTRACE_MEMTRACE_STORE_MSIZE))
#define QTRACE_RESET_MEMTRACE_STORE_PREOP_VALUE(x)  (x &= (~QTRACE_MEMTRACE_STORE_PREOP_VALUE))
#define QTRACE_RESET_MEMTRACE_STORE_PSTOP_VALUE(x)  (x &= (~QTRACE_MEMTRACE_STORE_PSTOP_VALUE))

#define QTRACE_MEMTRACE_FETCH_EXT_ADDRS(x)                                                         \
({                                                                                                 \
    int i = (x & QTRACE_MEMTRACE_FETCH_VPMA); QTRACE_RESET_MEMTRACE_FETCH_ADDRS(x); i;             \
})
#define QTRACE_MEMTRACE_FETCH_EXT_MSIZE(x)                                                         \
({                                                                                                 \
    int i = (x & QTRACE_MEMTRACE_FETCH_MSIZE);QTRACE_RESET_MEMTRACE_FETCH_MSIZE(x); i;             \
})
#define QTRACE_MEMTRACE_FETCH_EXT_PREOP_VALUE(x)                                                   \
({                                                                                                 \
    int i = (x & QTRACE_MEMTRACE_FETCH_PREOP_VALUE);QTRACE_RESET_MEMTRACE_FETCH_PREOP_VALUE(x); i; \
})
#define QTRACE_MEMTRACE_FETCH_EXT_PSTOP_VALUE(x)                                                   \
({                                                                                                 \
    int i = (x & QTRACE_MEMTRACE_FETCH_PSTOP_VALUE); QTRACE_RESET_MEMTRACE_FETCH_PSTOP_VALUE(x);i; \
})

#define QTRACE_MEMTRACE_STORE_EXT_ADDRS(x)                                                         \
({                                                                                                 \
    int i = (x & QTRACE_MEMTRACE_STORE_VPMA); QTRACE_RESET_MEMTRACE_STORE_ADDRS(x); i;             \
})
#define QTRACE_MEMTRACE_STORE_EXT_MSIZE(x)                                                         \
({                                                                                                 \
    int i = (x & QTRACE_MEMTRACE_STORE_MSIZE);QTRACE_RESET_MEMTRACE_STORE_MSIZE(x); i;             \
})
#define QTRACE_MEMTRACE_STORE_EXT_PREOP_VALUE(x)                                                   \
({                                                                                                 \
    int i = (x & QTRACE_MEMTRACE_STORE_PREOP_VALUE);QTRACE_RESET_MEMTRACE_STORE_PREOP_VALUE(x); i; \
})
#define QTRACE_MEMTRACE_STORE_EXT_PSTOP_VALUE(x)                                                   \
({                                                                                                 \
    int i = (x & QTRACE_MEMTRACE_STORE_PSTOP_VALUE); QTRACE_RESET_MEMTRACE_STORE_PSTOP_VALUE(x);i; \
})


/// ------------------------------------------- 
/// register tracing.
/// ------------------------------------------- 
#define QTRACE_REGTRACE_VALUE                       (1<<17)

/// ------------------------------------------- 
/// branch tracing.
/// ------------------------------------------- 
#define QTRACE_BRANCHTRACE_TARGET                   (1<<18)   
#define QTRACE_BRANCHTRACE_TAKEN                    (1<<19)   
#define QTRACE_BRANCHTRACE_NOTTAKEN                 (1<<20)   

/// ------------------------------------------- 
/// program counter tracing.
/// ------------------------------------------- 
#define QTRACE_PCTRACE_VMA                          (1<<21)

/// ------------------------------------------------ ///
/// instrumentation prototypes. 
/// ------------------------------------------------ ///
typedef 
void (*RESET_OR_PRINT_STATS)(void);

typedef 
void (*INSTRUMENTATION_CALLBACK)(unsigned);

typedef 
void (*INSTRUCTION_CALLBACK)(unsigned);

typedef 
void (*IBASICBLOCK_CALLBACK)(unsigned);

typedef 
void (*QTRACE_INSERT_INSTRUMENT)(unsigned, ...);

typedef 
void (*QTRACE_MODULE_FINIT)(QTRACE_INSERT_INSTRUMENT);

/// ------------------------------------------------ ///
/// instrumentation function list
/// ------------------------------------------------ ///
void qtrace_instrument_setup(const char*);
/// qtrace_invoke_instruction_callback - invoke all the instruction level callbacks.
void qtrace_invoke_instruction_callback(unsigned arg);
/// qtrace_invoke_ibasicblock_callback - invoke all the basic-block level callbacks.
void qtrace_invoke_ibasicblock_callback(unsigned arg);
/// invoke plugin-defined functions.
void qtrace_invoke_client_from_list(const char *mn, const char* fn, GenericRtnContainer *list);
/// qtrace_instrument_parser - called by every instruction to parse the 
/// instrumentation requirements and construct the instrumentation 
/// context list.
void qtrace_instrument_parser(unsigned pos, ...); 

/// QEMU dissasmbles instructions a basicblock at a time. For every instruction 
/// in the basicblock, QTRACE creates a list (singly linkedlist) of instrumentation
/// context.
///
/// qtrace_allocate_icontext_root/qtrace_free_icontext_root allocates/free the root
/// of the icontext list.
/// 
/// qtrace_allocate_new_icontext - allocates a new icontext for the given root.
/// its called when a new Instrumentation Context is needed for the current 
/// instruction.
void qtrace_allocate_icontext_root(void);
void qtrace_free_icontext_root(InstrumentContext *root); 
void qtrace_allocate_new_icontext(InstrumentContext **root);
/// qtrace_free_all_icontexts - frees all icontexts. called when the instrumentation
/// of this basicblock is done.
void qtrace_free_all_icontexts(void);
/// qtrace_increment_uiid - one more instruction instrumented.
void qtrace_increment_uiid(void);
/// qtrace_icontext_sanity_check - do a brief check on the validity of given icontext.
void qtrace_icontext_sanity_check(InstrumentContext *);
/// qtrace_get_current_icontext_list - return the root of the current icontext.
InstrumentContext* qtrace_get_current_icontext_list(void);

#endif /* QTRACE_H */

