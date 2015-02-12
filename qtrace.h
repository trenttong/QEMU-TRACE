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
#include <stdbool.h>

/// ------------------------------------------------ ///
/// instrumentation IDs and flags. 
/// ------------------------------------------------ ///
#define QTRACE_REG_SHIFT(reg)                         (1<<reg)
#define QTRACE_ADD_INST_TYPE_FLAG(s,flg)              {s->qtrace_flags.qtrace_insnflags|=(flg);}
#define QTRACE_ADD_COND_INST_TYPE_FLAG(s,flg,c)       {if(c) QTRACE_ADD_INST_TYPE_FLAG(s, flg);}
#define QTRACE_ADD_INST_RR_FLAG(s,reg)                {s->qtrace_flags.qtrace_insnrr|=(1<<(reg));}
#define QTRACE_ADD_INST_RR_FLAG_NOSHIFT(s,reg)        {s->qtrace_flags.qtrace_insnrr|=reg;}
#define QTRACE_ADD_COND_INST_RR_FLAG(s,reg,c)         {if(c) QTRACE_ADD_INST_RR_FLAG(s, reg);}
#define QTRACE_ADD_COND_INST_RR_FLAG_NOSHIFT(s,reg,c) {if(c) QTRACE_ADD_INST_RR_FLAG_NOSHIFT(s, reg);} 
#define QTRACE_ADD_INST_WR_FLAG(s,reg)                {s->qtrace_flags.qtrace_insnwr|=(1<<(reg));}
#define QTRACE_ADD_INST_WR_FLAG_NOSHIFT(s,reg)        {s->qtrace_flags.qtrace_insnwr|=reg;}
#define QTRACE_ADD_COND_INST_WR_FLAG(s,reg,c)         {if(c) QTRACE_ADD_INST_WR_FLAG(s, reg);}
#define QTRACE_ADD_COND_INST_WR_FLAG_NOSHIFT(s,reg,c) {if(c) QTRACE_ADD_INST_WR_FLAG_NOSHIFT(s, reg);} 
#define QTRACE_ADD_INST_RR_ALL_FLAG(s)                {s->qtrace_flags.qtrace_insnrr|= ~0;  }
#define QTRACE_ADD_INST_WR_ALL_FLAG(s)                {s->qtrace_flags.qtrace_insnwr|= ~0;  }

#define QTRACE_ADD_INST_RR_ADDR_FLAG(s)                     \
do {                                                        \
    QTRACE_ADD_INST_RR_ALL_FLAG(s);                         \
} while(0);

#define QTRACE_RESET_INST_TYPE_FLAG(s)                      \
do {                                                        \
    s->qtrace_flags.qtrace_insnflags=0;                     \
    memset(s->qtrace_flags.qtrace_insnflags_str, 0, sizeof(s->qtrace_flags.qtrace_insnflags_str)); \
} while(0);
#define QTRACE_RESET_INST_RR_FLAG(s)                        \
do {                                                        \
    s->qtrace_flags.qtrace_insnrr=0;                        \
    memset(s->qtrace_flags.qtrace_fetch_gpr_str, 0, sizeof(s->qtrace_flags.qtrace_fetch_gpr_str));  \
} while(0);
#define QTRACE_RESET_INST_WR_FLAG(s)                        \
do {                                                        \
    s->qtrace_flags.qtrace_insnwr=0;                        \
    memset(s->qtrace_flags.qtrace_write_gpr_str, 0, sizeof(s->qtrace_flags.qtrace_write_gpr_str)); \
} while(0);

/// ------------------------------------------------ ///
/// reserve instrumentation space . 
/// ------------------------------------------------ ///
#define QTRACE_RESERVE_ICONTEXT_ARGUMENT(ictx)      {ictx->iargs[ictx->ciarg++] = 0;}
#define QTRACE_RESERVE_ICONTEXT_ARG(ictx)           QTRACE_RESERVE_ICONTEXT_ARGUMENT(ictx); 
#define QTRACE_RESERVE_ICONTEXT_ARG_OFFSET(ictx)    QTRACE_RESERVE_ICONTEXT_ARGUMENT(ictx); 
#define QTRACE_RESERVE_ICONTEXT_ARG_BASIZE(ictx)    QTRACE_RESERVE_ICONTEXT_ARGUMENT(ictx);

#define QTRACE_RESERVE_ICONTEXT_OP_UNARY(ictx)      \
do  {                                               \
    QTRACE_RESERVE_ICONTEXT_ARGUMENT(ictx);         \
    QTRACE_RESERVE_ICONTEXT_ARGUMENT(ictx);         \
    QTRACE_RESERVE_ICONTEXT_ARGUMENT(ictx);         \
} while(0);

#define QTRACE_RESERVE_ICONTEXT_OP_BINARY(ictx)     \
do  {                                               \
    QTRACE_RESERVE_ICONTEXT_OP_UNARY(ictx);         \
    QTRACE_RESERVE_ICONTEXT_OP_UNARY(ictx);         \
    QTRACE_RESERVE_ICONTEXT_OP_UNARY(ictx);         \
} while(0);

/* every QTRACE unary opcode has 1 opcode and 2 operands */
#define QTRACE_ADVANCE_ICONTEXT_OP_UNARY(x)         { x += 3; }
#define QTRACE_ADVANCE_ICONTEXT_OP_BINARY(x)        { x += 9; }

/// ------------------------------------------- 
/// general facility.
/// ------------------------------------------- 
#define QTRACE_MAX_ARGS                             (1024)
#define QTRACE_IFUN                                 (1UL << 0)
#define QTRACE_IPOINT_BEFORE                        (1UL << 1)
#define QTRACE_IPOINT_AFTER                         (1UL << 2)
#define QTRACE_PROCESS_UPID                         (1UL << 3)
#define QTRACE_BEGIN_ARG                            QTRACE_MAX_ARGS
#define QTRACE_END_ARG                              (1UL << 4)
#define QTRACE_ISPREINST_INSTRUMENT(ictx)                        (ictx->ipoint == QTRACE_IPOINT_BEFORE)
#define QTRACE_PSTINST(ictx)                        (ictx->ipoint == QTRACE_IPOINT_AFTER)

/// ------------------------------------------- 
/// memory tracing.
/// ------------------------------------------- 
#define QTRACE_MEMTRACE_FETCH_VMA                   (1UL << 5)
#define QTRACE_MEMTRACE_FETCH_PMA                   (1UL << 6) 
#define QTRACE_MEMTRACE_FETCH_MSIZE                 (1UL << 7)     
#define QTRACE_MEMTRACE_FETCH_VALUE                 (1UL << 8)     
#define QTRACE_MEMTRACE_FETCH_VPMA                  (QTRACE_MEMTRACE_FETCH_VMA | QTRACE_MEMTRACE_FETCH_PMA)
#define QTRACE_MEMTRACE_STORE_VMA                   (1UL << 9)
#define QTRACE_MEMTRACE_STORE_PMA                   (1UL << 10) 
#define QTRACE_MEMTRACE_STORE_MSIZE                 (1UL << 11)     
#define QTRACE_MEMTRACE_STORE_VALUE                 (1UL << 12)     
#define QTRACE_MEMTRACE_STORE_VPMA                  (QTRACE_MEMTRACE_STORE_VMA | QTRACE_MEMTRACE_STORE_PMA)
/* 4 internal values */
#define QTRACE_MEMTRACE_FETCH_PREOP_VALUE           (1UL << 13)
#define QTRACE_MEMTRACE_FETCH_PSTOP_VALUE           (1UL << 14)
#define QTRACE_MEMTRACE_STORE_PREOP_VALUE           (1UL << 15)
#define QTRACE_MEMTRACE_STORE_PSTOP_VALUE           (1UL << 16)

/* shift away from the last N bits. they are used by QEMU */
#define QTRACE_MEMTRACE_BITS                        ((int)(ceil(log2(NB_MMU_MODES)))) 
#define QTRACE_ADD_MEMTRACE(index,x)                (((index) | (x << QTRACE_MEMTRACE_BITS))) 
#define QTRACE_EXT_MEMTRACE(index)                  ((index >> QTRACE_MEMTRACE_BITS)) 
#define QTRACE_EXT_MEMINDEX(index)                  ((index & ((1UL << QTRACE_MEMTRACE_BITS)-1))) 

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
#define QTRACE_REGTRACE_VALUE                       (1UL << 17)
#define QTRACE_REGTRACE_VALUE_FETCH                 (1UL << 18)
#define QTRACE_REGTRACE_VALUE_STORE                 (1UL << 19)
#define QTRACE_REGTRACE_REGFETCH(x)                 (x&QTRACE_REGTRACE_VALUE_FETCH)
#define QTRACE_REGTRACE_REGSTORE(x)                 (x&QTRACE_REGTRACE_VALUE_STORE)

/// ------------------------------------------- 
/// branch tracing.
/// ------------------------------------------- 
#define QTRACE_BRANCHTRACE_TARGET                   (1UL << 20)   
#define QTRACE_BRANCHTRACE_TAKEN                    (1UL << 21)   
#define QTRACE_BRANCHTRACE_NOTTAKEN                 (1UL << 22)   

/// ------------------------------------------- 
/// program counter tracing.
/// ------------------------------------------- 
#define QTRACE_PCTRACE_VMA                          (1UL << 23)
#define QTRACE_PCTRACE_PMA                          (1UL << 24)

/// ------------------------------------------- 
/// qtrace icontext operators.
/// ------------------------------------------- 
#define QTRACE_ICONTEXT_OPERATOR_BINARYSUM          (1UL << 25)
#define QTRACE_ICONTEXT_OPERATOR_BINARYSUB          (1UL << 26)
#define QTRACE_ICONTEXT_OPERATOR_BINARYMUL          (1UL << 27)
#define QTRACE_ICONTEXT_OPERATOR_BINARYDIV          (1UL << 28)
#define QTRACE_ICONTEXT_OPERATOR_UNARYFETCH         (1UL << 29)
#define QTRACE_ICONTEXT_OPERATOR_UNARYSTORE         (1UL << 30)
#define QTRACE_ICONTEXT_OPERATOR_UNARYXLATE         (1UL << 31)
#define QTRACE_ICONTEXT_OPERATOR_BINARYSUM_NOARG    (1UL << 32)
#define QTRACE_ICONTEXT_OPERATOR_NOARG(x)           (x&QTRACE_ICONTEXT_OPERATOR_BINARYSUM_NOARG)

#define QTRACE_ASSEMBLE_MULTIBYTE_VALUE(idx, address)         \
({                                                            \
    int i;                                                    \
    target_ulong val;                                         \
    for (i = 0; i < idx; ++i)                                 \
    {                                                         \
        val <<= 8; val |= address[i];                         \
    }                                                         \
    val;                                                      \
})

#define QTRACE_FIND_DIFFERENT_PAGE(currpage, address)         \
({                                                            \
    hwaddr nextpage;                                          \
    for (idx = 0; idx < env->multipage_fetch_count; ++idx)    \
    {                                                         \
        nextpage = address[idx];                              \
        if ((currpage & TARGET_PAGE_MASK) !=                  \
            (nextpage &TARGET_PAGE_MASK))                     \
            break;                                            \
    }                                                         \
    nextpage;                                                 \
})

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
void (*QTRACE_MODULE_INIT)(void**);

typedef 
void (*QTRACE_WALK_PAGETABLE)(int, bool, PageWalkContext*);

/// ------------------------------------------------ ///
/// instrumentation function list
/// ------------------------------------------------ ///
void qtrace_instrument_setup(const char*);
/// qtrace_invoke_instruction_callback - invoke all the instruction level callbacks.
void qtrace_invoke_instruction_callback(uint64_t);
/// qtrace_invoke_ibasicblock_callback - invoke all the basic-block level callbacks.
void qtrace_invoke_ibasicblock_callback(uint64_t);
/// invoke plugin-defined functions.
void qtrace_invoke_client_from_list(const char *mn, const char* fn, GenericRtnContainer *list);
/// qtrace_instrument_parser - called by every instruction to parse the 
/// instrumentation requirements and construct the instrumentation 
/// context list.
void qtrace_instrument_x86_parser(unsigned pos, ...);
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
/// @ qtrace_has_instrument - whether a given icontext list has a
/// @ specified type of instrumentation.
bool qtrace_has_instrument(InstrumentContext *ictx, unsigned position);

#endif /* QTRACE_H */
