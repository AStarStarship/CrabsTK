/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2007             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************
 
 function: toplevel libogg include
 last mod: $Id: ogg.h 18044 2011-08-01 17:55:20Z gmaxwell $

 ********************************************************************/
#ifndef _OGG_H
#define _OGG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <ogg/os_types.h>

typedef struct {
  void *iov_base;
  size_t iov_len;
} ogg_iovec_t;

typedef struct {
  long endbyte;
  SI4  endbit;

  unsigned CH1 *buffer;
  unsigned CH1 *ptr;
  long storage;
} oggpack_buffer;

/* ogg_page is used to encapsulate the data in one Ogg bitstream page *****/

typedef struct {
  unsigned CH1 *header;
  long header_len;
  unsigned CH1 *body;
  long body_len;
} ogg_page;

/* ogg_stream_state contains the current encode/decode state of a logical
   Ogg bitstream **********************************************************/

typedef struct {
  unsigned CH1   *body_data;    /* bytes from packet bodies */
  long    body_storage;          /* storage elements allocated */
  long    body_fill;             /* elements stored; fill mark */
  long    body_returned;         /* elements of fill returned */


  SI4     *lacing_vals;      /* The values that will go to the segment table */
  ogg_int64_t *granule_vals; /* granulepos values for headers. Not compact
                                this way, but it is simple coupled to the
                                lacing fifo */
  long    lacing_storage;
  long    lacing_fill;
  long    lacing_packet;
  long    lacing_returned;

  unsigned CH1    header[282];      /* working space for header encode */
  SI4              header_fill;

  SI4     e_o_s;          /* set when we have buffered the last packet in the
                             logical bitstream */
  SI4     b_o_s;          /* set after we've written the initial page
                             of a logical bitstream */
  long    serialno;
  long    pageno;
  ogg_int64_t  packetno;  /* sequence number for decode; the framing
                             knows where there's a hole in the data,
                             but we need coupling so that the codec
                             (which is in a separate abstraction
                             layer) also knows about the gap */
  ogg_int64_t   granulepos;

} ogg_stream_state;

/* ogg_packet is used to encapsulate the data and metadata belonging
   to a single raw Ogg/Vorbis packet *************************************/

typedef struct {
  unsigned CH1 *packet;
  long  bytes;
  long  b_o_s;
  long  e_o_s;

  ogg_int64_t  granulepos;

  ogg_int64_t  packetno;     /* sequence number for decode; the framing
                                knows where there's a hole in the data,
                                but we need coupling so that the codec
                                (which is in a separate abstraction
                                layer) also knows about the gap */
} ogg_packet;

typedef struct {
  unsigned CH1 *data;
  SI4 storage;
  SI4 fill;
  SI4 returned;

  SI4 unsynced;
  SI4 headerbytes;
  SI4 bodybytes;
} ogg_sync_state;

/* Ogg BITSTREAM PRIMITIVES: bitstream ************************/

extern void  oggpack_writeinit(oggpack_buffer *b);
extern SI4   oggpack_writecheck(oggpack_buffer *b);
extern void  oggpack_writetrunc(oggpack_buffer *b,long bits);
extern void  oggpack_writealign(oggpack_buffer *b);
extern void  oggpack_writecopy(oggpack_buffer *b,void *source,long bits);
extern void  oggpack_reset(oggpack_buffer *b);
extern void  oggpack_writeclear(oggpack_buffer *b);
extern void  oggpack_readinit(oggpack_buffer *b,unsigned CH1 *buf,SI4 bytes);
extern void  oggpack_write(oggpack_buffer *b,unsigned long value,SI4 bits);
extern long  oggpack_look(oggpack_buffer *b,SI4 bits);
extern long  oggpack_look1(oggpack_buffer *b);
extern void  oggpack_adv(oggpack_buffer *b,SI4 bits);
extern void  oggpack_adv1(oggpack_buffer *b);
extern long  oggpack_read(oggpack_buffer *b,SI4 bits);
extern long  oggpack_read1(oggpack_buffer *b);
extern long  oggpack_bytes(oggpack_buffer *b);
extern long  oggpack_bits(oggpack_buffer *b);
extern unsigned CH1 *oggpack_get_buffer(oggpack_buffer *b);

extern void  oggpackB_writeinit(oggpack_buffer *b);
extern SI4   oggpackB_writecheck(oggpack_buffer *b);
extern void  oggpackB_writetrunc(oggpack_buffer *b,long bits);
extern void  oggpackB_writealign(oggpack_buffer *b);
extern void  oggpackB_writecopy(oggpack_buffer *b,void *source,long bits);
extern void  oggpackB_reset(oggpack_buffer *b);
extern void  oggpackB_writeclear(oggpack_buffer *b);
extern void  oggpackB_readinit(oggpack_buffer *b,unsigned CH1 *buf,SI4 bytes);
extern void  oggpackB_write(oggpack_buffer *b,unsigned long value,SI4 bits);
extern long  oggpackB_look(oggpack_buffer *b,SI4 bits);
extern long  oggpackB_look1(oggpack_buffer *b);
extern void  oggpackB_adv(oggpack_buffer *b,SI4 bits);
extern void  oggpackB_adv1(oggpack_buffer *b);
extern long  oggpackB_read(oggpack_buffer *b,SI4 bits);
extern long  oggpackB_read1(oggpack_buffer *b);
extern long  oggpackB_bytes(oggpack_buffer *b);
extern long  oggpackB_bits(oggpack_buffer *b);
extern unsigned CH1 *oggpackB_get_buffer(oggpack_buffer *b);

/* Ogg BITSTREAM PRIMITIVES: encoding **************************/

extern SI4      ogg_stream_packetin(ogg_stream_state *os, ogg_packet *op);
extern SI4      ogg_stream_iovecin(ogg_stream_state *os, ogg_iovec_t *iov,
                                   SI4 count, long e_o_s, ogg_int64_t granulepos);
extern SI4      ogg_stream_pageout(ogg_stream_state *os, ogg_page *og);
extern SI4      ogg_stream_pageout_fill(ogg_stream_state *os, ogg_page *og, SI4 nfill);
extern SI4      ogg_stream_flush(ogg_stream_state *os, ogg_page *og);
extern SI4      ogg_stream_flush_fill(ogg_stream_state *os, ogg_page *og, SI4 nfill);

/* Ogg BITSTREAM PRIMITIVES: decoding **************************/

extern SI4      ogg_sync_init(ogg_sync_state *oy);
extern SI4      ogg_sync_clear(ogg_sync_state *oy);
extern SI4      ogg_sync_reset(ogg_sync_state *oy);
extern SI4      ogg_sync_destroy(ogg_sync_state *oy);
extern SI4      ogg_sync_check(ogg_sync_state *oy);

extern CH1    *ogg_sync_buffer(ogg_sync_state *oy, long size);
extern SI4      ogg_sync_wrote(ogg_sync_state *oy, long bytes);
extern long     ogg_sync_pageseek(ogg_sync_state *oy,ogg_page *og);
extern SI4      ogg_sync_pageout(ogg_sync_state *oy, ogg_page *og);
extern SI4      ogg_stream_pagein(ogg_stream_state *os, ogg_page *og);
extern SI4      ogg_stream_packetout(ogg_stream_state *os,ogg_packet *op);
extern SI4      ogg_stream_packetpeek(ogg_stream_state *os,ogg_packet *op);

/* Ogg BITSTREAM PRIMITIVES: general ***************************/

extern SI4      ogg_stream_init(ogg_stream_state *os,SI4 serialno);
extern SI4      ogg_stream_clear(ogg_stream_state *os);
extern SI4      ogg_stream_reset(ogg_stream_state *os);
extern SI4      ogg_stream_reset_serialno(ogg_stream_state *os,SI4 serialno);
extern SI4      ogg_stream_destroy(ogg_stream_state *os);
extern SI4      ogg_stream_check(ogg_stream_state *os);
extern SI4      ogg_stream_eos(ogg_stream_state *os);

extern void     ogg_page_checksum_set(ogg_page *og);

extern SI4      ogg_page_version(const ogg_page *og);
extern SI4      ogg_page_continued(const ogg_page *og);
extern SI4      ogg_page_bos(const ogg_page *og);
extern SI4      ogg_page_eos(const ogg_page *og);
extern ogg_int64_t  ogg_page_granulepos(const ogg_page *og);
extern SI4      ogg_page_serialno(const ogg_page *og);
extern long     ogg_page_pageno(const ogg_page *og);
extern SI4      ogg_page_packets(const ogg_page *og);

extern void     ogg_packet_clear(ogg_packet *op);


#ifdef __cplusplus
}
#endif

#endif  /* _OGG_H */
