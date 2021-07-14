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
  bv16.h : 

  $Log$
******************************************************************************/

extern void Reset_BV16_Encoder(
struct BV16_Encoder_State *cs);

extern void BV16_Encode(
struct BV16_Bit_Stream *bs,
struct BV16_Encoder_State *cs,
short  *inx);

extern void Reset_BV16_Decoder(
struct BV16_Decoder_State *ds);

extern void BV16_Decode(
struct BV16_Bit_Stream     *bs,
struct BV16_Decoder_State  *ds,
short	*xq);

extern void BV16_PLC(
struct  BV16_Decoder_State   *ds,
short	*x);

