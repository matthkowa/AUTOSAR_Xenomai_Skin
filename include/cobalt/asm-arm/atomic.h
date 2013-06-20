/*
 * Copyright (C) 2003,2004 Philippe Gerum <rpm@xenomai.org>.
 *
 * ARM port
 *   Copyright (C) 2005 Stelian Pop
 *
 * Xenomai is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * Xenomai is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Xenomai; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef _COBALT_ASM_ARM_ATOMIC_H
#define _COBALT_ASM_ARM_ATOMIC_H

#ifdef __KERNEL__
#include <linux/version.h>
#include <asm/irqflags.h>

#if __LINUX_ARM_ARCH__ >= 6
static inline void atomic_set_mask(unsigned long mask, unsigned long *addr)
{
    unsigned long tmp, tmp2;

    __asm__ __volatile__("@ atomic_set_mask\n"
			 "1:    ldrex   %0, [%3]\n\t"
			 "      orr     %0, %0, %4\n\t"
			 "      strex   %1, %0, [%3]\n"
			 "      teq     %1, #0\n"
			 "      bne     1b"
			 : "=&r" (tmp), "=&r" (tmp2), "+Qo" (*addr)
			 : "r" (addr), "Ir" (mask)
			 : "cc");
}
#else /* arm <= armv5 */
#define atomic_set_mask(mask, addr)					\
	({								\
		unsigned long flags;					\
									\
		flags = hard_local_irq_save();				\
		*(unsigned long *)(addr) |= (unsigned long)(mask);	\
		hard_local_irq_restore(flags);				\
	})
#endif /* arm <= armv5 */

#else /* !__KERNEL__ */
#include <asm/xenomai/features.h>

#if __LINUX_ARM_ARCH__ <= 5 || !defined(CONFIG_SMP)
#define xnarch_memory_barrier() \
       __asm__ __volatile__ ("": /* */ : /* */ :"memory")
#endif /* arm <= armv5 || !CONFIG_SMP */

#endif /* !__KERNEL__ */

#include <asm-generic/xenomai/atomic.h>

#endif /* !_COBALT_ASM_ARM_ATOMIC_H */