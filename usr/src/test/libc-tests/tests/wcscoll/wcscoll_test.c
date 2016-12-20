/*-
 * Copyright (c) 2016 Baptiste Daroussin <bapt@FreeBSD.org>
 * Copyright (c) 2016 Lauri Tirkkonen <lotheac@iki.fi>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include "test_common.h"

static int
cmp(const void *a, const void *b)
{
	const wchar_t wa[2] = { *(const wchar_t *)a, 0 };
	const wchar_t wb[2] = { *(const wchar_t *)b, 0 };

	return (wcscoll(wa, wb));
}

void
test_russian_collation(void)
{
	test_t t = test_start("russian_collation");
	wchar_t c[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzЁАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюяё";
	wchar_t res[] = L"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZаАбБвВгГдДеЕёЁжЖзЗиИйЙкКлЛмМнНоОпПрРсСтТуУфФхХцЦчЧшШщЩъЪыЫьЬэЭюЮяЯ";

	if (!setlocale(LC_COLLATE, "ru_RU.UTF-8") != NULL)
		test_failed(t, "setlocale LC_COLLATE=ru_RU.UTF-8 failed");
	qsort(c, wcslen(c), sizeof(wchar_t), cmp);
	if (wcscmp(c, res) != 0)
		test_failed(t, "expected collation: '%ls' got '%ls'", res, c);
	test_passed(t);
}

int
main(void)
{
	test_russian_collation();
	return (0);
}
