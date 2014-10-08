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

/// ------------------------------------------------ ///
/// miscellaneous 
/// ------------------------------------------------ ///
#define QTRACE_EXIT             exit
#define QTRACE_ERROR            printf
#define QTRACE_WAIT_COMMAND_HANDLED(X)  while(!X);
#define QTRACE_LOCAL_FUN        static
#define QTRACE_MAX_IARGS        256 
#define QTRACE_MAX_CALLBACK_NUM 256

#define ResetStatsNameString  "ResetStats"
#define PrintStatsNameString  "PrintStats"
#define SHARED_MEM_KEY        154519

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
    unsigned  ipoint;   /* insertion point */ 
    unsigned  ciarg;    /* current iarg */
    unsigned  iargs[QTRACE_MAX_IARGS]; /* instrumentation args */
    unsigned  memfext;  /* this context has memory instrumentation */
    unsigned  pcfext;   /* this context has program counter instrumentation */
    unsigned  btarget;  /* this context has branch instrumentation */
	struct InstrumentContext *next;
} InstrumentContext;


/// ------------------------------------------------ ///
/// qtrace asynchronous debug channel. 
/// ------------------------------------------------ ///
typedef struct {
    int client_userd;     /* user defined function request */
    int client_reset;     /* reset stats function request */ 
    int client_print;     /* print stats function request */
    int client_reset_all;
    int client_print_all;
    int flushcc;
    char fname[128];
    char mname[128];
} DebugChannel;

/// ------------------------------------------------ ///
/// qtrace global variables. 
/// ------------------------------------------------ ///
extern DebugChannel *channel;
extern GenericRtnContainer* resetstats_list;
extern GenericRtnContainer* printstats_list;
extern GenericRtnContainer* userdefine_list;
extern GenericRtnContainer* instruction_list;
extern GenericRtnContainer* ibasicblock_list;

#endif // QTRACE_COMMON_H
