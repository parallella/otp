/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 2012-2015. All Rights Reserved.
 *
 * The contents of this file are subject to the Erlang Public License,
 * Version 1.1, (the "License"); you may not use this file except in
 * compliance with the License. You should have received a copy of the
 * Erlang Public License along with this software. If not, it can be
 * retrieved online at http://www.erlang.org/.
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * %CopyrightEnd%
 */

/* Description:
 *	The slave_ix module makes sure that the slave emulator code_ix stays in
 *	lock-step with the master code_ix.
 */

#ifndef __SLAVE_IX_H__
#define __SLAVE_IX_H__

#include "code_ix.h"

/* Called once at emulator initialization.
 */
void slave_code_ix_init(void);

/* Called by code_ix as it updates the code indices.
 */
void slave_code_ix_update(void);

#endif /* !__SLAVE_IX_H__ */
