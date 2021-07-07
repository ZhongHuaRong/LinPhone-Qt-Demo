/*****************************************************************************/
/* BroadVoice(R)16 (BV16) Floating-Point ANSI-C Source Code                  */
/* Revision Date: October 5, 2012                                            */
/* Version 1.2                                                               */
/*****************************************************************************/

/*****************************************************************************/
/* Copyright 2000-2012 Broadcom Corporation                                  */
/*                                                                           */
/* This software is provided under the GNU Lesser General Public License,    */
/* version 2.1, as published by the Free Software Foundation ("LGPL").       */
/* This program is distributed in the hope that it will be useful, but       */
/* WITHOUT ANY SUPPORT OR WARRANTY; without even the implied warranty of     */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the LGPL for     */
/* more details.  A copy of the LGPL is available at                         */
/* http://www.broadcom.com/licenses/LGPLv2.1.php,                            */
/* or by writing to the Free Software Foundation, Inc.,                      */
/* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.                 */
/*****************************************************************************/


/*****************************************************************************
  bv16strct.h : BV16 data structures

  $Log$
******************************************************************************/

#ifndef  BV16STRCT_H
#define  BV16STRCT_H

struct BV16_Decoder_State {
Float	stsym[LPCO];
Float	ltsym[LTMOFF];
Float	xq[XQOFF];
Float	lsppm[LPCO*LSPPORDER];
Float	lgpm[LGPORDER];
Float	lsplast[LPCO];
Float	prevlg[2];
Float lmax;           /* level-adaptation */
Float lmin;
Float lmean;
Float x1;
Float level;
short pp_last;
short	cfecount;
short	ngfae;
Float	bq_last[3];
short nggalgc;
Float estl_alpha_min;
UWord32 idum;
Float per;
Float E;
Float atplc[LPCO+1];
Float ma_a;
Float b_prv[2];
int pp_prv;
};

struct BV16_Encoder_State {
Float	prevlg[2];
Float	lmax;			/* level-adaptation */
Float	lmin;
Float	lmean;
Float	x1;
Float	level;
Float	x[XOFF];		/* 8kHz down-sampled signal memory */
Float	xwd[XDOFF];		/* memory of DECF:1 decimated version of xw() */
Float	dq[XOFF];		/* quantized short-term pred error */
Float	dfm[DFO];		/* decimated xwd() filter memory */
Float	stpem[LPCO];		/* ST Pred. Error filter memory */
Float	stwpm[LPCO];		/* ST Weighting all-Pole Memory */
Float	stsym[LPCO];		/* ST SYnthesis filter Memory	*/
Float	stnfz[NSTORDER];
Float	stnfp[NSTORDER];
Float	ltsym[MAXPP1+FRSZ];	/* long-term synthesis filter memory */
Float	ltnfm[MAXPP1+FRSZ];	/* long-term noise feedback filter memory */
Float	lsplast[LPCO];
Float	lsppm[LPCO*LSPPORDER];	/* LSP Predictor Memory */
Float	lgpm[LGPORDER];
int		cpplast;		/* pitch period pf the previous frame */
Float	hpfzm[HPO];
Float	hpfpm[HPO];
Float	old_A[LPCO+1];
};

struct BV16_Bit_Stream {
short	lspidx[2];
short	ppidx;
short	bqidx;
short	gidx;
short	qvidx[FRSZ/VDIM];
};

#endif /* BV16STRCT_H */
