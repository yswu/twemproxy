/*
 * twemproxy - A fast and lightweight proxy for memcached protocol.
 * Copyright (C) 2011 Twitter, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _NC_MBUF_H_
#define _NC_MBUF_H_

#include <nc_core.h>

#define MCOPY_CODEC(ACTION)         \
    ACTION( MCOPY_GET,  "get "  )   \
    ACTION( MCOPY_GETS, "gets " )   \
    ACTION( MCOPY_CRLF, "\r\n"  )   \
    ACTION( MCOPY_NIL,  ""      )   \

#define DEFINE_ACTION(_type, _str) _type,
typedef enum mcopy_type {
    MCOPY_CODEC( DEFINE_ACTION )
    MCOPY_SENTINEL
} mcopy_type_t;
#undef DEFINE_ACTION

struct mbuf {
    uint32_t           magic;   /* mbuf magic (const) */
    STAILQ_ENTRY(mbuf) next;    /* next mbuf */
    uint8_t            *pos;    /* read marker */
    uint8_t            *last;   /* write marker */
    uint8_t            *start;  /* start of buffer (const) */
    uint8_t            *end;    /* end of buffer (const) */
};

STAILQ_HEAD(mhdr, mbuf);

#define MBUF_MAGIC      0xdeadbeef
#define MBUF_MIN_SIZE   512
#define MBUF_MAX_SIZE   65536
#define MBUF_SIZE       16384
#define MBUF_HSIZE      sizeof(struct mbuf)

static inline bool
mbuf_empty(struct mbuf *mbuf)
{
    return mbuf->pos == mbuf->last ? true : false;
}

static inline bool
mbuf_full(struct mbuf *mbuf)
{
    return mbuf->last == mbuf->end ? true : false;
}

void mbuf_init(struct instance *nci);
void mbuf_deinit(void);
struct mbuf *mbuf_get(void);
void mbuf_put(struct mbuf *mbuf);
uint32_t mbuf_length(struct mbuf *mbuf);
uint32_t mbuf_size(struct mbuf *mbuf);
void mbuf_insert(struct mhdr *mhdr, struct mbuf *mbuf);
void mbuf_remove(struct mhdr *mhdr, struct mbuf *mbuf);
void mbuf_copy(struct mbuf *mbuf, uint8_t *pos, size_t n);
struct mbuf *mbuf_split(struct mhdr *h, uint8_t *pos, mcopy_type_t headcopy, mcopy_type_t tailcopy);

#endif
