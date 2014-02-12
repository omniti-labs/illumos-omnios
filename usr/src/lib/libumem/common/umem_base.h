/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */
/*
 * Copyright (c) 2012, Joyent, Inc.  All rights reserved.
 */

#ifndef	_UMEM_BASE_H
#define	_UMEM_BASE_H

#include <umem_impl.h>

#ifdef	__cplusplus
extern "C" {
#endif

#include "misc.h"

extern size_t pagesize;
#undef PAGESIZE
#define	PAGESIZE	pagesize

/*
 * umem.c: non-tunables
 */
extern vmem_t		*umem_memalign_arena;

extern int umem_ready;
extern thread_t umem_init_thr;		/* the thread doing the init */

extern int umem_init(void);		/* do umem's initialization */
#pragma rarely_called(umem_init)

extern umem_log_header_t *umem_transaction_log;
extern umem_log_header_t *umem_content_log;
extern umem_log_header_t *umem_failure_log;
extern umem_log_header_t *umem_slab_log;

extern mutex_t umem_init_lock;

extern mutex_t umem_cache_lock;
extern umem_cache_t umem_null_cache;

extern mutex_t umem_flags_lock;

extern mutex_t umem_update_lock;
extern cond_t umem_update_cv;
extern volatile thread_t umem_st_update_thr;
extern thread_t umem_update_thr;
extern struct timeval	umem_update_next;

extern volatile hrtime_t umem_reap_next;
extern volatile uint32_t umem_reaping;
#define	UMEM_REAP_DONE		0x00000000	/* inactive */
#define	UMEM_REAP_ADDING	0x00000001	/* umem_reap() is active */
#define	UMEM_REAP_ACTIVE	0x00000002	/* update thread is reaping */

extern uintptr_t umem_tmem_off;

/*
 * umem.c: tunables
 */
extern uint32_t umem_max_ncpus;

extern uint32_t umem_stack_depth;
extern uint32_t umem_reap_interval;
extern uint32_t umem_update_interval;
extern uint32_t umem_depot_contention;
extern uint32_t umem_abort;
extern uint32_t umem_output;
extern uint32_t umem_logging;
extern uint32_t umem_mtbf;
extern size_t umem_transaction_log_size;
extern size_t umem_content_log_size;
extern size_t umem_failure_log_size;
extern size_t umem_slab_log_size;
extern size_t umem_content_maxsave;
extern size_t umem_lite_minsize;
extern size_t umem_lite_maxalign;
extern size_t umem_maxverify;
extern size_t umem_minfirewall;
extern size_t umem_ptc_size;

extern uint32_t umem_flags;

/*
 * umem.c: Internal aliases (to avoid PLTs)
 */
extern void *_umem_alloc(size_t size, int umflags);
extern void *_umem_zalloc(size_t size, int umflags);
extern void _umem_free(void *buf, size_t size);

extern void *_umem_cache_alloc(umem_cache_t *cache, int flags);
extern void _umem_cache_free(umem_cache_t *cache, void *buffer);

/*
 * umem.c: private interfaces
 */
extern void umem_type_init(caddr_t, size_t, size_t);
extern int umem_get_max_ncpus(void);
extern void umem_process_updates(void);
extern void umem_cache_applyall(void (*)(umem_cache_t *));
extern void umem_cache_update(umem_cache_t *);

extern void umem_alloc_sizes_add(size_t);
extern void umem_alloc_sizes_clear(void);
extern void umem_alloc_sizes_remove(size_t);

/*
 * umem_fork.c: private interfaces
 */
extern void umem_forkhandler_init(void);

/*
 * umem_update_thread.c
 */
extern int umem_create_update_thread(void);

/*
 * envvar.c:
 */
void umem_setup_envvars(int);
void umem_process_envvars(void);

/*
 * umem_genasm.c: private interfaces
 */
extern const int umem_genasm_supported;
extern int umem_genasm(int *, umem_cache_t **, int);

/*
 * malloc.c: traditional malloc/free interface for genasm
 */
extern void *umem_malloc(size_t);
extern void umem_malloc_free(void *);
extern void *_malloc(size_t);
extern void _free(void *);

#ifdef	__cplusplus
}
#endif

#endif	/* _UMEM_BASE_H */
