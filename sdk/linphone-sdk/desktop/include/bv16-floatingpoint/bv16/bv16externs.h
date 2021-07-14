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
  bv16externs.c : BV16 Fixed-Point externs

  $Log$
******************************************************************************/

/* POINTERS */
extern Float  winl[WINSZ];
extern Float  sstwin[1+LPCO];
extern	Float	gfsz[];
extern	Float	gfsp[];
extern int idxord[];
extern	Float hpfa[];
extern	Float hpfb[];
extern	Float	adf[];
extern	Float	bdf[];
extern	Float	x[];
extern	Float	x2[];
extern	Float	MPTH[];

/* LSP Quantization */
extern Float  lspecb1[LSPECBSZ1*LPCO];
extern Float  lspecb2[LSPECBSZ2*LPCO];
extern Float  lspmean[LPCO];
extern Float  lspp[LSPPORDER*LPCO];

/* Pitch Predictor Codebook */
extern Float  pp9cb[PPCBSZ*9];

/* Log-Gain Quantization */
extern Float  lgpecb[LGPECBSZ];
extern Float  lgp[LGPORDER];
extern Float  lgmean;

/* Log-Gain Limitation */
extern Float   lgclimit[];

/* Excitation Codebook */
extern Float  cccb[CBSZ*VDIM];

extern Float lgpecb_nh[];


/* Function Prototypes */
Float	estlevel(
Float	lg,
Float	*level,
Float	*lmax,
Float	*lmin,
Float	*lmean,
Float	*x1,
short   ngfae,
short   nggalgc,
Float   *estl_alpha_min);

extern void excdec_w_LT_synth(
	Float   *ltsym, /* long-term synthesis filter memory at decoder*/
	short   *idx,   /* excitation codebook index array for current subframe */
	Float   gainq,  /* quantized linear gains for sub-subframes */
	Float   *b,     /* coefficient of 3-tap pitch predictor */
	short   pp,     /* pitch period */
	Float   *cb,    /* scalar quantizer codebook */
   Float   *EE); 

Float gaindec(
	      Float   *lgq,
	      short   gidx,	
	      Float   *lgpm,
	      Float   *prevlg,		/* previous log gains (last two frames) */
	      Float   level,
	      short   *nggalgc,
	      Float   *lg_el
	      );

void gainplc(Float E,
	     Float *lgeqm,
	     Float *lgqm);

extern void lspdec(
    Float   *lspq,  
    short   *lspidx,  
    Float   *lsppm,
	 Float	*lspq_last); 

extern void lspplc(
    Float   *lspq,
	 Float   *lsppm);

extern int coarsepitch(
Float	*xw,
Float	*xwd,
Float	*dfm,
int	cpplast);

extern int refinepitch(
Float	*x,
int		cpp,
Float	*ppt);

extern int pitchtapquan(
Float	*x,
int	pp,
Float	*b,
Float	*re);

extern void excquan(
   short   *idx,   /* quantizer codebook index for uq[] vector */
	Float   *s,     /* input speech signal vector */
	Float   *aq,    /* short-term predictor coefficient array */
	Float   *fsz,   /* short-term noise feedback filter - numerator */
	Float   *fsp,   /* short-term noise feedback filter - denominator */
	Float   *b,     /* coefficient of 3-tap pitch predictor */
	Float   beta,   /* coefficient of 1-tap LT noise feedback filter */
	Float   *stsym,  /* filter memory before filtering of current vector */
	Float   *ltsym, /* long-term synthesis filter memory */
	Float   *ltnfm, /* long-term noise feedback filter memory */
	Float   *stnfz,
	Float   *stnfp,
	Float   *cb,    /* scalar quantizer codebook */
	int     pp);    /* pitch period (# of 8 kHz samples) */

extern int gainquan(
    Float   *gainq,  
    Float   lg,     
    Float   *lgpm,
    Float   *prevlg,
    Float	level);

extern void lspquan(
    Float   *lspq,  
    short   *lspidx,  
    Float   *lsp,     
    Float   *lsppm);
