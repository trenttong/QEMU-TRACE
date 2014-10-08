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

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "qemu-adebug.h"
#include <stdbool.h>

/// --------------------------------------------
/* connect/disconnect the shared memory */
/// --------------------------------------------
static inline DebugChannel* connect(void) 
{
   int shared_id = shmget(SHARED_MEM_KEY, 
                           sizeof(DebugChannel), 
                               S_IRUSR | S_IWUSR);

     return (DebugChannel*)shmat(shared_id, NULL, 0);
}

static inline void disconnect(DebugChannel *channel) { shmdt(channel); }

/// --------------------------------------------
/* handle commands */ 
/// --------------------------------------------
static const char* flushcc          =   "--flushcc";
static const char* client_userd     =   "--client-userd";
static const char* client_reset     =   "--client-reset";
static const char* client_print     =   "--client-print";
static const char* client_reset_all =   "--client-reset-all";
static const char* client_print_all =   "--client-print-all";

#define QTRACE_HANDLE_CONNECTION(cmd, var)                          \
static inline void handle_connection_##cmd(DebugChannel *channel,   \
                                           const char *fname,       \
                                           const char *mname,       \
                                           bool *valid)             \
{                                                                   \
   if (fname)                                                       \
        memcpy(channel->fname, fname, strlen(fname));               \
   if (mname)                                                       \
        memcpy(channel->mname, mname, strlen(mname));               \
   channel->var = 1;                                                \
   QTRACE_WAIT_COMMAND_HANDLED(channel->var);                       \
   *valid = true;                                                   \
} 
QTRACE_HANDLE_CONNECTION(flushcc, flushcc);
QTRACE_HANDLE_CONNECTION(client_userd, client_userd);
QTRACE_HANDLE_CONNECTION(client_reset, client_reset);
QTRACE_HANDLE_CONNECTION(client_print, client_print);
QTRACE_HANDLE_CONNECTION(client_reset_all, client_reset_all);
QTRACE_HANDLE_CONNECTION(client_print_all, client_print_all);
#undef QTRACE_HANDLE_CONNECTION

static inline void handle_connection_success(int argc, char** argv, DebugChannel *channel) 
{
     int i;
     for (i=1;i<argc;++i)
     {
        bool valid_command = false;
        if (!strcmp(argv[i], flushcc)) handle_connection_flushcc(channel, 0, 0, &valid_command);
        if (!strcmp(argv[i], client_userd)) handle_connection_client_userd(channel, 
                                                                           argv[++i], 
                                                                           argv[++i], 
                                                                           &valid_command);
        if (!strcmp(argv[i], client_reset)) handle_connection_client_reset(channel, 
                                                                           0, 
                                                                           argv[++i], 
                                                                           &valid_command);
        if (!strcmp(argv[i], client_print)) handle_connection_client_print(channel, 
                                                                           0, 
                                                                           argv[++i], 
                                                                           &valid_command);
        if (!strcmp(argv[i], client_reset_all)) handle_connection_client_reset_all(channel, 
                                                                                   0, 0, 
                                                                                   &valid_command);
        if (!strcmp(argv[i], client_print_all)) handle_connection_client_print_all(channel, 
                                                                                   0, 0, 
                                                                                   &valid_command);
      if (!valid_command) 
      {
            QTRACE_ERROR("invalid command %s\n", argv[i]);
            QTRACE_EXIT(-1);
      }
     }

   return;
}

static void handle_connection_error(void) 
{
   QTRACE_ERROR("unable to connect to shm %d\n", SHARED_MEM_KEY);
     QTRACE_EXIT(-1);
}

static void print_help(void)
{
   printf("qemu-adebug      asynchronous debugging tool for QTRACE\n");
   printf("--flushcc        flush code cache\n");
   printf("--client-reset      reset client instrumentation plugin\n");
   printf("--client-print      print client instrumentation plugin\n");
   printf("--client-reset-all   reset client instrumentation plugin\n");
   printf("--client-print-all   print client instrumentation plugin\n");
}

int main(int argc, char **argv)
{
   /* calling help ? */
   if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) 
   {
      print_help();
      exit(0);
   }

      /* connect to the shared memory */
      DebugChannel *channel=connect();
   
      if (!channel) handle_connection_error();
      else handle_connection_success(argc, argv, channel); 

      /* disconnect */
      disconnect(channel);

   /* done */
      return 0;
}

