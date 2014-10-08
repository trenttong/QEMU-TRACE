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

#ifndef QTRACE_CLIENT_HEADER_H
#define QTRACE_CLIENT_HEADER_H

#include "qtrace.h"
#include "qtrace-common.h"
#include "qtrace-target.h"

/* functions to ask QTRACE to insert instrumentation */
static QTRACE_INSERT_INSTRUMENT QTRACE_INSTRUCTION_INSTRUMENT;
static QTRACE_INSERT_INSTRUMENT QTRACE_BASICBLOCK_INSTRUMENT;

unsigned InstructionCallBackNum = 0;
unsigned BasicBlockCallBackNum  = 0;
unsigned UserDefineCallBackNum  = 0;
unsigned ResetStatsCallBackNum  = 0;
unsigned PrintStatsCallBackNum  = 0;

GenericRtnContainer* ResetStatsCallBackArray[QTRACE_MAX_CALLBACK_NUM] = {0};
GenericRtnContainer* PrintStatsCallBackArray[QTRACE_MAX_CALLBACK_NUM] = {0};
GenericRtnContainer* InstructionCallBackArray[QTRACE_MAX_CALLBACK_NUM]= {0};
GenericRtnContainer* BasicBlockCallBackArray[QTRACE_MAX_CALLBACK_NUM] = {0};
GenericRtnContainer* UserDefineCallBackArray[QTRACE_MAX_CALLBACK_NUM] = {0};

/* hook up the QTRACE function to insert instrumentation */
void InitializeInstructionInstrument(QTRACE_INSERT_INSTRUMENT rtn)
{
    QTRACE_INSTRUCTION_INSTRUMENT = rtn;
}

/* hook up the QTRACE function to insert instrumentation */
void InitializeBasicBlockInstrument(QTRACE_INSERT_INSTRUMENT rtn)
{
    QTRACE_BASICBLOCK_INSTRUMENT = rtn;
}

/* AddStatsReset/AddStatsPrint - register stats reset/print */
static void AddFunction(void *rtn, 
                        char *fname, 
                        GenericRtnContainer **list, 
                        unsigned *position)
{
    GenericRtnContainer *c = (GenericRtnContainer*)malloc(sizeof(GenericRtnContainer));
    c->next = NULL;
    c->rtn  = rtn;
    c->fname= fname;
    c->mname= NULL;
    list[*position] =  c;
    *position = *position + 1;
}

void AddStatsResetCallBack(void *rtn) 
{ 
    AddFunction(rtn, ResetStatsNameString, ResetStatsCallBackArray, &ResetStatsCallBackNum);
}
void AddStatsPrintCallBack(void *rtn) 
{ 
    AddFunction(rtn, PrintStatsNameString, PrintStatsCallBackArray, &PrintStatsCallBackNum);
}
void AddInstructionCallBack(void *rtn)
{
    AddFunction(rtn, "InstructionCallBack", InstructionCallBackArray, &InstructionCallBackNum);
}
void AddBasicBlockCallBack(void *rtn)
{
    AddFunction(rtn, "BasicBlockCallBack", BasicBlockCallBackArray, &BasicBlockCallBackNum);
}
void AddUserDefineCallBack(void *rtn, char *fname) 
{ 
    AddFunction(rtn, fname, PrintStatsCallBackArray, &PrintStatsCallBackNum);
}

#endif /* QTRACE_CLIENT_HEADER_H */
