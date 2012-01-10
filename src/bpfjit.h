/*-
 * Copyright (c) 2011 Alexander Nasonov.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _NET_BPFJIT_H_
#define _NET_BPFJIT_H_

#include <stddef.h>

#ifdef __linux
#include <stdint.h>
#include <sys/types.h>
#include <pcap-bpf.h>
#else
#include <net/bpf.h>
#endif

#include <sljitLir.h>

#if defined(__GNUC)
#define	bpfjit_unused	__attribute__((__unused__))
#else
#define	bpfjit_unused	/* delete */
#endif

void *bpfjit_generate_code(struct bpf_insn *insns, size_t insn_count);
void bpfjit_free_code(void *code);

static inline unsigned int bpfjit_unused
bpfjit_execute_code(const uint8_t *p, size_t wirelen,
    size_t buflen, const void *code)
{
	union {
		const void* code;
		sljit_uw (SLJIT_CALL *func)(const uint8_t *p,
		    sljit_uw wirelen, sljit_uw buflen);
	} func = { code };

	/* Explicit cast to discard high bits on 64bit arches */
	return (uint32_t)func.func(p, wirelen, buflen);
}

#endif /* !_NET_BPFJIT_H_ */
