/*
 * %CopyrightBegin%

 *
 * Copyright Ericsson AB 2016. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * %CopyrightEnd%
 */
/*
 * hipe_load.c
 *
 * HiPE atomic code loader
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "sys.h"
#include "global.h"
#include "erl_binary.h"
#include "hipe_load.h"

/*
 * This destructor function can safely be called multiple times.
 */
static void
hipe_loader_state_dtor(Binary* magic)
{
    HipeLoaderState* stp = ERTS_MAGIC_BIN_DATA(magic);
    if (stp->module == NIL) return;

    erts_fprintf(stderr, "Destroying HiPE loader state for module %T\n",
		 stp->module);

    // TODO: Needs to be freed separately. We'd like have a unified executable
    // code allocator, so postpone this for now.
    /* if (stp->text_segment) */
    /* 	erts_free(ERTS_ALC_T_HIPE, stp->text_segment); */
    stp->text_segment = NULL;
    stp->text_segment_size = 0;

    if (stp->data_segment)
	erts_free(ERTS_ALC_T_HIPE, stp->data_segment);
    stp->data_segment = NULL;
    stp->data_segment_size = 0;

    if (stp->beam_stub_magic
	&& erts_refc_dectest(&stp->beam_stub_magic->refc, 0) == 0)
	erts_bin_free(stp->beam_stub_magic);
    stp->beam_stub_magic = NULL;

    stp->module = NIL;
}

Binary *hipe_alloc_loader_state(Eterm module)
{
    HipeLoaderState *stp;
    Binary *magic;

    if (is_not_atom(module)) return NULL;

    erts_fprintf(stderr, "Creating HiPE loader state for module %T\n", module);

    magic = erts_create_magic_binary(sizeof(HipeLoaderState),
				     hipe_loader_state_dtor);
    erts_refc_inc(&magic->refc, 1);
    stp = ERTS_MAGIC_BIN_DATA(magic);

    stp->module = module;
    stp->text_segment = NULL;
    stp->text_segment_size = 0;
    stp->data_segment = NULL;
    stp->data_segment_size = 0;
    stp->beam_stub_magic = NULL;

    return magic;
}

void hipe_free_loader_state(Binary *magic)
{
    if (ERTS_MAGIC_BIN_DESTRUCTOR(magic) != hipe_loader_state_dtor)
	return;

    /* Why does BEAM do a refc_dec here? What is holding that reference? */
    hipe_loader_state_dtor(magic);
}

HipeLoaderState *
hipe_get_loader_state(Binary *magic)
{
    if (ERTS_MAGIC_BIN_DESTRUCTOR(magic) != hipe_loader_state_dtor)
	return NULL;

    return (HipeLoaderState*) ERTS_MAGIC_BIN_DATA(magic);
}

BIF_RETTYPE hipe_bifs_prepare_stub_module_3(BIF_ALIST_3)
{
    HipeLoaderState *stp;

    if (!ERTS_TERM_IS_MAGIC_BINARY(BIF_ARG_1)
	|| (stp = hipe_get_loader_state
	    (((ProcBin*)binary_val(BIF_ARG_1))->val)) == NULL
	|| is_not_atom(stp->module)) {
	BIF_ERROR(BIF_P, BADARG);
    }

    return erts_prepare_stub_module(BIF_P, BIF_ARG_1, BIF_ARG_2, BIF_ARG_3);
}