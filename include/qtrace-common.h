/*
 *  QTRACE asynchronous debug tool 
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

#ifndef QTRACE_COMMON_H
#define QTRACE_COMMON_H

#include <stdint.h> 
#include <stdlib.h>
#include <pthread.h>

/// ------------------------------------------------ ///
/// miscellaneous 
/// ------------------------------------------------ ///
#define QTRACE_EXIT             exit
#define QTRACE_CHANNEL_LOCK     pthread_mutex_lock
#define QTRACE_CHANNEL_UNLOCK   pthread_mutex_unlock
#define QTRACE_WAIT_COMMAND_HANDLED(X)  while(!X);
#define QTRACE_LOCAL_FUN        static
#define QTRACE_MAX_IARGS        256 
#define QTRACE_MAX_CALLBACK_NUM 256
#define QTRACE_DISASM_STR_LEN   256

#define ResetStatsNameString  "ResetStats"
#define PrintStatsNameString  "PrintStats"
#define SHARED_MEM_KEY        154519

#ifndef QTRACE_PRINT_OFF
#define QTRACE_ARG_PRINT(format, ...)   { printf(format, ##__VA_ARGS__); }
#define QTRACE_WARN(format, ...)        { printf("QEMU_WARN: ");  printf(format, ##__VA_ARGS__); } 
#define QTRACE_ERROR(format, ...)       { printf("QEMU_ERROR: "); printf(format, ##__VA_ARGS__); }
#else 
#define QTRACE_ARG_PRINT(format, ...)
#define QTRACE_WARN(format, ...)
#define QTRACE_ERROR(format, ...)
#endif

/// ------------------------------------------------ ///
/// instrumentation module metadata 
/// ------------------------------------------------ ///
typedef struct rc { 
    struct rc *next;      /* singly linkedlist */
    void *rtn;            /* pointer to the function */
    const char *fname;    /* name of the function */
    const char* mname;    /* name of the module the function belongs to */
} GenericRtnContainer;

/// ------------------------------------------------ ///
/// instrumentation context. 
/// ------------------------------------------------ ///
/// InstrumentContext - this structure represents a single instrumentation
/// context defined in the user defined plugins.
/// an instrumentation context holds various information regarding what
/// kind of instrumentations are needed, when and where to call the 
/// instrumentation function.
///
/// e.g. if (QTRACE_TEST_FETCH(type))
///      {
///         Module_INS_InsertCall(5,
///                               QTRACE_IPOINT_BEFORE,
///                               QTRACE_IFUN, CacheSim,
///                               QTRACE_MEMTRACE_VMA, 
///                               QTRACE_MEMTRACE_MSIZE);
///      }
/// creates an instrumentation context with QTRACE_MEMTRACE_VMA and 
/// QTRACE_MEMTRACE_MSIZE and call function CacheSim before the 
/// instruction executes.
typedef struct InstrumentContext  {
    uintptr_t ifun;     /* instrumentation function */
    uint64_t  ipoint;   /* insertion point */ 
    uint64_t  ciarg;    /* current iarg */
    uint64_t  iargs[QTRACE_MAX_IARGS]; /* instrumentation args */
    uint64_t  memfext;  /* this context has memory instrumentation */
    uint64_t  pcfext;   /* this context has program counter instrumentation */
    uint64_t  btarget;  /* this context has branch instrumentation */
	struct InstrumentContext *next;
} InstrumentContext;

/// ------------------------------------------------ ///
/// qtrace asynchronous debug channel. 
/// ------------------------------------------------ ///
typedef struct {
    pthread_mutex_t lock;     /* exclusive access to channel variables */
    int client_userd;         /* user defined function request */
    int client_reset;         /* reset stats function request */ 
    int client_print;         /* print stats function request */
    int client_reset_all;
    int client_print_all;
    int flushcc;
    char fname[128];
    char mname[128];
} DebugChannel;


/// ------------------------------------------------ ///
/// qtrace xlation support. 
/// ------------------------------------------------ ///
enum 
QTRACE_XLATE_FAULT
{
    QTRACE_XLATE_NOPRESENT_FAULT = 0x1,
    QTRACE_XLATE_PERM_FAULT      = 0x2,
};

enum 
QTRACE_INSTRUMENT_PAGE_PERM
{
    QTRACE_PAGE_FETCH=0,
    QTRACE_PAGE_WRITE,
    QTRACE_PAGE_EXEC,
};

/// ------------------------------------------------ ///
/// pagewalk context. 
/// ------------------------------------------------ ///
/// PageWalkContext - this structure contains information gathered in a
/// single page walk. 
typedef struct pagewalk {
    uint64_t vaddr;             /* virtual  address of the page walk */
    uint64_t paddr;             /* physcial address of the page walk */
    uint64_t xlation_fail;      /* reason of failure for the pagewalk */
} PageWalkContext;

/// ------------------------------------------------ ///
/// qtrace runtime provided functions to plugins. 
/// ------------------------------------------------ ///
enum 
QTRACE_INSTRUMENT_RUNTIME 
{
    QTRACE_PARSER_INDEX         = 0x0,
    QTRACE_PAGETABLE_INDEX      = 0x1,
    QTRACE_INSTRUMENT_RUNTIME_MAXF,
};

/// ------------------------------------------------ ///
/// qtrace global variables. 
/// ------------------------------------------------ ///
extern DebugChannel *channel;
extern GenericRtnContainer* resetstats_list;
extern GenericRtnContainer* printstats_list;
extern GenericRtnContainer* userdefine_list;
extern GenericRtnContainer* instruction_list;
extern GenericRtnContainer* ibasicblock_list;

typedef struct QTRACELogItem {
    uint64_t index;
    const char *name;
} QTRACELogItem;

typedef struct  {
    uint64_t qtrace_insnflags;
    uint16_t qtrace_insnrr;     /* read registers */
    uint16_t qtrace_insnwr;     /* write registers */
    char     qtrace_insnflags_str[QTRACE_DISASM_STR_LEN];
    char     qtrace_fetch_gpr_str[QTRACE_DISASM_STR_LEN];
    char     qtrace_write_gpr_str[QTRACE_DISASM_STR_LEN];
} QTraceFlags;


#endif // QTRACE_COMMON_H
