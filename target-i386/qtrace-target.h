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
#ifndef QTRACE_TARGET_H
#define QTRACE_TARGET_H

/// ------------------------------
/// register 
/// ------------------------------

/* general purpose registers. */
// 64 bit.
#define QTRACE_X86_RAX  0
#define QTRACE_X86_RCX  1
#define QTRACE_X86_RDX  2
#define QTRACE_X86_RBX  3 
#define QTRACE_X86_RSP  4
#define QTRACE_X86_RBP  5 
#define QTRACE_X86_RSI  6 
#define QTRACE_X86_RDI  7 
#define QTRACE_X86_R8   8 
#define QTRACE_X86_R9   9
#define QTRACE_X86_R10  10
#define QTRACE_X86_R11  11
#define QTRACE_X86_R12  12
#define QTRACE_X86_R13  13
#define QTRACE_X86_R14  14
#define QTRACE_X86_R15  15
#define QTRACE_X86_RIP  16 
// 32 bit.
#define QTRACE_X86_EAX  17 
#define QTRACE_X86_ECX  18 
#define QTRACE_X86_EDX  19 
#define QTRACE_X86_EBX  20 
#define QTRACE_X86_ESP  21 
#define QTRACE_X86_EBP  22 
#define QTRACE_X86_ESI  23 
#define QTRACE_X86_EDI  24 
#define QTRACE_X86_R8D  25 
#define QTRACE_X86_R9D  26 
#define QTRACE_X86_R10D 27 
#define QTRACE_X86_R11D 28 
#define QTRACE_X86_R12D 29 
#define QTRACE_X86_R13D 30 
#define QTRACE_X86_R14D 31 
#define QTRACE_X86_R15D 32 
#define QTRACE_X86_EIP  33 
// 16 bit.
#define QTRACE_X86_AX   34 
#define QTRACE_X86_CX   35 
#define QTRACE_X86_DX   36
#define QTRACE_X86_BX   37
#define QTRACE_X86_SP   38
#define QTRACE_X86_BP   39
#define QTRACE_X86_SI   40
#define QTRACE_X86_DI   41 
#define QTRACE_X86_R8W  42 
#define QTRACE_X86_R9W  43 
#define QTRACE_X86_R10W 44 
#define QTRACE_X86_R11W 45 
#define QTRACE_X86_R12W 46 
#define QTRACE_X86_R13W 47 
#define QTRACE_X86_R14W 48 
#define QTRACE_X86_R15W 49 
#define QTRACE_X86_IP   50 
// 8 bit - lower half.
#define QTRACE_X86_AL   51
#define QTRACE_X86_CL   52
#define QTRACE_X86_DL   53
#define QTRACE_X86_BL   54
#define QTRACE_X86_SPL  55
#define QTRACE_X86_BPL  56
#define QTRACE_X86_SIL  57
#define QTRACE_X86_DIL  58
#define QTRACE_X86_R8B  59
#define QTRACE_X86_R9B  60
#define QTRACE_X86_R10B 61
#define QTRACE_X86_R11B 62
#define QTRACE_X86_R12B 63
#define QTRACE_X86_R13B 64
#define QTRACE_X86_R14B 65
#define QTRACE_X86_R15B 66
// 8 bit - higher half.
#define QTRACE_X86_AH   67 
#define QTRACE_X86_CH   68
#define QTRACE_X86_DH   69
#define QTRACE_X86_BH   70

#define QTRACE_X86_CS   71

/* arcitecturally invisible register */
#define R_LRIP          72 
#define R_PRIP          73 
#define R_BRIP          74

/* control register. */
#define R_CR0 109 
#define R_CR1 103
#define R_CR2 104
#define R_CR3 105
#define R_CR4 106

#define QTRACE_X86_CR0 R_CR0
#define QTRACE_X86_CR1 R_CR1
#define QTRACE_X86_CR2 R_CR2
#define QTRACE_X86_CR3 R_CR3
#define QTRACE_X86_CR4 R_CR4
#define QTRACE_X86_PROCESS_UPID QTRACE_X86_CR3

/* timestamp counter register */
#define R_TSC 107 
#define QTRACE_X86_TSC R_TSC

/// ------------------------------
/// memory - mmu 
/// ------------------------------
#define NB_MMU_MODES 3

#endif /// QTRACE_TARGET_H
