/*
 * i386 virtual CPU header
 *
 *  Copyright (c) 2003 Fabrice Bellard
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
#ifndef QTRACE_I386_H
#define QTRACE_I386_H

/// ------------------------------
/// register 
/// ------------------------------

/* general purpose registers. */
#define R_EAX 0
#define R_ECX 1
#define R_EDX 2
#define R_EBX 3
#define R_ESP 4
#define R_EBP 5
#define R_ESI 6
#define R_EDI 7
#define R_R8  8
#define R_R9  9 
#define R_R10 10
#define R_R11 11
#define R_R12 12
#define R_R13 13
#define R_R14 14
#define R_R15 15
#define R_RIP 16

#define QTRACE_X86_RAX R_EAX
#define QTRACE_X86_RCX R_ECX
#define QTRACE_X86_RDX R_EDX
#define QTRACE_X86_RBX R_EBX
#define QTRACE_X86_RSP R_ESP
#define QTRACE_X86_RBP R_EBP
#define QTRACE_X86_RSI R_ESI
#define QTRACE_X86_RDI R_EDI
#define QTRACE_X86_R8  R_R8
#define QTRACE_X86_R9  R_R9
#define QTRACE_X86_R10 R_R10
#define QTRACE_X86_R11 R_R11
#define QTRACE_X86_R12 R_R12
#define QTRACE_X86_R13 R_R13
#define QTRACE_X86_R14 R_R14
#define QTRACE_X86_R15 R_R15
#define QTRACE_X86_RIP R_RIP

/* control register. */
#define R_CR0 17
#define R_CR1 18
#define R_CR2 19
#define R_CR3 20 
#define R_CR4 21

#define QTRACE_X86_CR0 R_CR0
#define QTRACE_X86_CR1 R_CR1
#define QTRACE_X86_CR2 R_CR2
#define QTRACE_X86_CR3 R_CR3
#define QTRACE_X86_CR4 R_CR4

/* timestamp counter register */
#define R_TSC 22
#define QTRACE_X86_TSC R_TSC

/// ------------------------------
/// memory - mmu 
/// ------------------------------
#define NB_MMU_MODES 3

#endif /// QTRACE_I386_H
