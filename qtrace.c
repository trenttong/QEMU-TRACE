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

#include <dlfcn.h>
#include <stdio.h>
#include <stdarg.h>
#include "cpu.h"
#include "qtrace.h"
#include "config.h"

/// ------------------------------------------------ ///
/// frontend compiling for. 
/// ------------------------------------------------ ///
#if TARGET_X86_64 == 1
    #define qtrace_instrument_parser qtrace_instrument_x86_parser 
#else
    #error "unsupport platform"
#endif

/* maximum # of instructions per basicblock */
#define MAX_ICONTEXT_ROOT CF_COUNT_MASK 
#define CURRENT_ROOT     (rootcount-1)
static unsigned rootcount;
static InstrumentContext* rootarray[MAX_ICONTEXT_ROOT];

GenericRtnContainer* resetstats_list;
GenericRtnContainer* printstats_list;
GenericRtnContainer* userdefine_list;
GenericRtnContainer* instruction_list;
GenericRtnContainer* ibasicblock_list;

/* the current and last unique instruction id */
unsigned long long uiid = 0;
unsigned long long last_uiid = 0;

/// @ qtrace_invoke_instruction_callback - this function goes through the list 
/// @ of all instruction callbacks and call them one by one. the callbacks then
/// @ tells QTRACE what to instrument. 
#define qtrace_invoke_callback(unit, ulist)             \
void qtrace_invoke_##unit##_callback(unsigned arg)      \
{                                                       \
    GenericRtnContainer* b = ulist;                     \
    while(b)                                            \
    {                                                   \
        ((INSTRUMENTATION_CALLBACK)b->rtn)(arg);        \
        b = b->next;                                    \
    }                                                   \
    return;                                             \
}   
qtrace_invoke_callback(instruction, instruction_list);
qtrace_invoke_callback(ibasicblock, ibasicblock_list);
#undef qtrace_invoke_callback

/// @ qtrace_has_instrument - whether a given icontext list has a
/// @ specified type of instrumentation.
bool qtrace_has_instrument(InstrumentContext *ictx, unsigned position)
{
    bool instrument = false;
    while(ictx)
    {
        instrument |= (ictx->ipoint == position);
        ictx = ictx->next;
    }
    return instrument;
}

/// @ qtrace_invoke_client - call client defined reset/print functions.
void qtrace_invoke_client_from_list(const char *mn, const char* fn, 
                                    GenericRtnContainer *rtn)
{
    while (rtn)
    {
        if (!strcmp(fn, rtn->fname)) 
            if (!mn || (mn && !strcmp(mn, rtn->mname))) 
        {
            ((RESET_OR_PRINT_STATS)rtn->rtn)();
        }
        rtn = rtn->next;
    }
    /* done */
}

static inline void handle_unable_load_module(const char *optarg)
{
    QTRACE_ERROR("unable to load instrumentation module %s\n", optarg);
    QTRACE_EXIT(-1);
}

static void add_function_to_list(void *rtn, 
                                 const char *mname, 
                                 const char *fname, 
                                 GenericRtnContainer **list)
{
   /* it is possible that this module does not define a instruction callback */
   if (!rtn) return;

   GenericRtnContainer *head = *list;
   if (!*list) 
   {
      head = *list = (GenericRtnContainer*) malloc(sizeof(GenericRtnContainer));
   }
   else 
   {
      /* get to the end of the linkedlist */
      while(head->next) head = head->next;
      head->next = (GenericRtnContainer*) malloc(sizeof(GenericRtnContainer));
      head = head->next;
   }

   /* register the callback */
   head->rtn   = rtn;
   head->mname = mname;
   head->fname = fname;
   head->next  = NULL;
}

/// @ register_stats_reset - register a function to resetstats_list list.
static void register_function_to_list(void* function, 
                                      const char *mname, 
                                      const char* fname, 
                                      GenericRtnContainer **list)  
{ 
   add_function_to_list(function, mname, fname, list);
}

/// @ qtrace_increment_uiid - increment the uiid. one more instruction has passed.
void qtrace_increment_uiid(void) { uiid ++; }
/// @ qtrace_allocate_icontext_root - allocate a icontext root pointer.
void qtrace_allocate_icontext_root(void) 
{ 
   rootarray[rootcount++] = 0;  
   assert(rootcount < MAX_ICONTEXT_ROOT); 
}
/// @ qtrace_get_current_icontext_list - return the current icontext.
InstrumentContext* qtrace_get_current_icontext_list(void) 
{ 
    return rootcount ? rootarray[rootcount-1] : NULL; 
}

/// @ qtrace_allocate_new_icontext - allocate a new instrumentation context
/// @ pointed by root.
void qtrace_allocate_new_icontext(InstrumentContext **root)
{   
    InstrumentContext* newhead = malloc(sizeof(InstrumentContext));
    memset(newhead, 0x0, sizeof(InstrumentContext));
    newhead->next = *root;
    *root = newhead;
}

/// @ qtrace_free_all_icontexts - free all allocated instrumentation context
/// @ pointed by root.
void qtrace_free_icontext_root(InstrumentContext *root) 
{
   InstrumentContext *next = NULL;
   while(root) 
   {
      next = root->next;  root->next = NULL; free(root);
      root = next;
   }
   return;
}

/// @ qtrace_free_all_icontexts - free all the allocated instrumentation context.
void qtrace_free_all_icontexts(void)
{
   unsigned idx;
   for(idx = 0; idx < rootcount; ++idx)
   {
      qtrace_free_icontext_root(rootarray[idx]);
      rootarray[idx] = NULL;
   }   
   rootcount = 0;
   return;
}

/// @ qtrace_icontext_sanity_check - make sure the icontext makes sense before 
/// @ generating instrumetation.
void qtrace_icontext_sanity_check(InstrumentContext *ictx)
{
   if (!ictx->ifun) QTRACE_ERROR("icontext ifun is NULL\n"); 
}


/// @ qtrace calls instrumentation function after the instruction is emulated.
/// @ as a result, qtrace shadows (backs up) any architectural states that are
/// @ modified as a result of the currently instrumented instruction.
/// @ this simplifies the modifications to the frontend, as now they only need 
/// @ to be changed minimally, i.e. to create a TCG IR node to backup architectural
/// @ states which are about to be modified.
///
/// @ for every instruction, there is a pre-inst instrumentation callback and
/// @ a post-inst instrumentation callback. the instrumentation callbacks have
/// @ 2 parts. in the first part, it allows qtrace to create any

/// @ qtrace_instrument - this function is called by the instrumentatiom module.
/// @ it parses what is requested by the instrumentation module into a icontext
/// @ structure.
void qtrace_instrument_x86_parser(unsigned pos, ...) 
{
   va_list arguments;                  
   va_start(arguments, pos);         

   /* every instruction will call qtrace_instrument_parser only once.
      and 1 icontext list will be allocated */
   if (last_uiid != uiid)
   {
      qtrace_allocate_icontext_root();
   }

   /* allocate a icontext in the current rootarray */
   qtrace_allocate_new_icontext(&rootarray[CURRENT_ROOT]);

   InstrumentContext *ictxhead = rootarray[CURRENT_ROOT];
   
   /* 
      stage 1.
      setup necessary instrumentation types as well as their frontend agonistic 
      parameters and slots. this setup is then interpreted and modified by 
      different frontend before invoking the TCG on them.
      
      stage 2.
      e.g. when encountering QTRACE_REGTRACE_VALUE. QTRACE_REGTRACE_VALUE. becomes
      the first parameter in the instrumentation context argument. then this 
      function reserves 2 more slots - 1 for the name of the register and
      another for the size of the instrumentation.

      stage 3.
      frontend looks at the name of the register and figure out its instrumentation
      offset as well as the instrumentation size. this information is then recorded
      in the same place in the icontext structure and passed to the backend. 
   */

   unsigned idx=0;
   for (idx=0;idx<pos;++idx) 
   {
      uint64_t arg = va_arg(arguments, unsigned);
      switch (arg)
      {
      case QTRACE_IPOINT_BEFORE:
      case QTRACE_IPOINT_AFTER:
           ictxhead->ipoint = arg; 
           break;
      /// ---------------------------------- ///
      /// instrumentation function address 
      /// ---------------------------------- ///
      case QTRACE_IFUN:
           ictxhead->ifun = va_arg(arguments, uintptr_t);
           ++idx;
           break;
      /// ---------------------------------- ///
      //// register trace. */
      /// ---------------------------------- ///
      case QTRACE_REGTRACE_VALUE:
           /* record the instrumentation type */
           ictxhead->iargs[ictxhead->ciarg++] = QTRACE_REGTRACE_VALUE_FETCH;
           /* this will be modified into offset by frontends */
           ictxhead->iargs[ictxhead->ciarg++] = va_arg(arguments, unsigned);
           /* reserve for instrumentation size */
           RESERVE_INSTRUMENT_CONTEXT_ARG_BASIZE(ictxhead);
           break;
      case QTRACE_PCTRACE_VMA:
           /* record the instrumentation type */
           ictxhead->iargs[ictxhead->ciarg++] = arg;
           /* computed eip + cs_base */
           RESERVE_INSTRUMENT_CONTEXT_OP_UNARY(ictxhead);
           /* eip */
           RESERVE_INSTRUMENT_CONTEXT_OP_UNARY(ictxhead);
           /* cs_base */
           RESERVE_INSTRUMENT_CONTEXT_OP_UNARY(ictxhead);
           break;
      case QTRACE_PCTRACE_PMA:
           /* record the instrumentation type */
           ictxhead->iargs[ictxhead->ciarg++] = arg;
           /* computed eip + cs_base */
           RESERVE_INSTRUMENT_CONTEXT_OP_UNARY(ictxhead);
           /* eip */
           RESERVE_INSTRUMENT_CONTEXT_OP_UNARY(ictxhead);
           /* cs_base */
           RESERVE_INSTRUMENT_CONTEXT_OP_UNARY(ictxhead);
           /* xlate node */
           RESERVE_INSTRUMENT_CONTEXT_OP_UNARY(ictxhead);
           break;
      /// ---------------------------------- ///
      //// miscellaneous register trace. */
      /// ---------------------------------- ///
      case QTRACE_PROCESS_UPID:
      case QTRACE_BRANCHTRACE_TARGET:
           /* shall be converted to regtrace in the frontend */
           ictxhead->iargs[ictxhead->ciarg++] = arg;
           RESERVE_INSTRUMENT_CONTEXT_ARG_OFFSET(ictxhead);
           RESERVE_INSTRUMENT_CONTEXT_ARG_BASIZE(ictxhead);
           break;
      /// ---------------------------------- ///
      /// memory address instrumentation */
      /// ---------------------------------- ///
      case QTRACE_MEMTRACE_FETCH_VMA:
      case QTRACE_MEMTRACE_FETCH_PMA:
      case QTRACE_MEMTRACE_FETCH_VPMA:
      case QTRACE_MEMTRACE_FETCH_MSIZE:
      case QTRACE_MEMTRACE_STORE_VMA:
      case QTRACE_MEMTRACE_STORE_PMA:
      case QTRACE_MEMTRACE_STORE_VPMA:
      case QTRACE_MEMTRACE_STORE_MSIZE:
           ictxhead->memfext |= arg;
           ictxhead->iargs[ictxhead->ciarg++] = arg;
           RESERVE_INSTRUMENT_CONTEXT_ARG_OFFSET(ictxhead);
           break;
      /// ---------------------------------- ///
      ////memory value instrumentation */
      /// ---------------------------------- ///
      case QTRACE_MEMTRACE_FETCH_VALUE:
           arg = (QTRACE_IPOINT_BEFORE == ictxhead->ipoint) ?
                  QTRACE_MEMTRACE_FETCH_PREOP_VALUE :
                  QTRACE_MEMTRACE_FETCH_PSTOP_VALUE ;
           ictxhead->iargs[ictxhead->ciarg++]  = arg;
           RESERVE_INSTRUMENT_CONTEXT_ARGUMENT(ictxhead);
           ictxhead->memfext |= arg;
           break;
      case QTRACE_MEMTRACE_STORE_VALUE:
           arg = (QTRACE_IPOINT_BEFORE == ictxhead->ipoint) ?
                  QTRACE_MEMTRACE_STORE_PREOP_VALUE :
                  QTRACE_MEMTRACE_STORE_PSTOP_VALUE ;
           ictxhead->iargs[ictxhead->ciarg++]  = arg;
           RESERVE_INSTRUMENT_CONTEXT_ARGUMENT(ictxhead);
           ictxhead->memfext |= arg;
           break;
      case QTRACE_END_ARG:
           goto label_end_arg;
      default:
           ictxhead->iargs[ictxhead->ciarg++] = arg;
           break;
      }
   }

label_end_arg:
   va_end (arguments);          

   /* update last uiid */
   last_uiid = uiid;

   /* briefly verify the validity of the instrumentations */
   qtrace_icontext_sanity_check(ictxhead);
}

/// @ qtrace_instrument_parse - this function takes the name of the module and setup
/// @ the module. it loads the module first and then passes the module the function to 
/// @ call to request instrumentation. 
void qtrace_instrument_setup(const char *module)
{
    int idx = 0;
    void* handle = NULL;

    const char *ResetStatsCBArrayName  = "ResetStatsCallBackArray";
    const char *ResetStatsCBNumName    = "ResetStatsCallBackNum";
    const char *PrintStatsCBArrayName  = "PrintStatsCallBackArray";
    const char *PrintStatsCBNumName    = "PrintStatsCallBackNum";
    const char *InstructionCBArrayName = "InstructionCallBackArray";
    const char *InstructionCBNumName   = "InstructionCallBackNum";
    const char *BasicBlockCBArrayName  = "BasicBlockCallBackArray";
    const char *BasicBlockCBNumName    = "BasicBlockCallBackNum";
    const char *UserDefineCBArrayName  = "UserDefineCallBackArray";
    const char *UserDefineCBNumName    = "UserDefineCallBackNum";
    const char *InitInsInstrumentName  = "InitializeInstructionInstrument";

    /* load the instrumentation module */
    if (!(handle=dlopen(module, RTLD_LAZY))) 
    {
        handle_unable_load_module(module);
    }

    /* register all instruction callbacks */
    GenericRtnContainer **InstructionCallBackArray = (GenericRtnContainer **)
                                                      dlsym(handle, 
                                                      InstructionCBArrayName); 
    int InstructionCallBackNum = *(int*) dlsym(handle, InstructionCBNumName);
    for(idx=0; idx<InstructionCallBackNum; ++idx) 
    {
        register_function_to_list(InstructionCallBackArray[idx]->rtn,
                                  module,
                                  InstructionCallBackArray[idx]->fname,
                                  &instruction_list);
    }

    /* register all basicblock callbacks */
    GenericRtnContainer **BasicBlockCallBackArray = (GenericRtnContainer **)
                                                    dlsym(handle, 
                                                    BasicBlockCBArrayName); 
    int BasicBlockCallBackNum = *(int*) dlsym(handle, BasicBlockCBNumName);
    for(idx=0; idx<BasicBlockCallBackNum; ++idx) 
    {
         register_function_to_list(BasicBlockCallBackArray[idx]->rtn,
                                  module,
                                  BasicBlockCallBackArray[idx]->fname,
                                  &ibasicblock_list);
    }

    /* register all user-define callbacks */
    GenericRtnContainer **UserDefineCallBackArray = (GenericRtnContainer **)
                                                     dlsym(handle, 
                                                     UserDefineCBArrayName);
    int UserDefineCallBackNum = *(int*) dlsym(handle, UserDefineCBNumName);
    for(idx=0; idx<UserDefineCallBackNum; ++idx) 
    {
        register_function_to_list(UserDefineCallBackArray[idx]->rtn,
                                  module, 
                                  UserDefineCallBackArray[idx]->fname,
                                  &userdefine_list);
    }

    /* register module reset stats */
    int ResetStatsCallBackNum = *(int*) dlsym(handle, ResetStatsCBNumName);
    if (ResetStatsCallBackNum)
    {
        register_function_to_list(((GenericRtnContainer**) 
                                  dlsym(handle, ResetStatsCBArrayName))[0]->rtn, 
                                  module, 
                                  ResetStatsNameString, 
                                  &resetstats_list); 
    }

    /* register module print stats */
    int PrintStatsCallBackNum = *(int*) dlsym(handle, PrintStatsCBNumName);
    if (PrintStatsCallBackNum)
    {
        register_function_to_list(((GenericRtnContainer**) 
                                  dlsym(handle, PrintStatsCBArrayName))[0]->rtn, 
                                  module, 
                                  PrintStatsNameString, 
                                  &printstats_list); 
    }

    /* register instrumentation parsing functions with the instrumentation module */
    QTRACE_MODULE_FINIT module_finit = (QTRACE_MODULE_FINIT) 
                                       dlsym(handle, InitInsInstrumentName);
    assert(module_finit);
    module_finit(qtrace_instrument_parser);

    /* done */
    return;
}


