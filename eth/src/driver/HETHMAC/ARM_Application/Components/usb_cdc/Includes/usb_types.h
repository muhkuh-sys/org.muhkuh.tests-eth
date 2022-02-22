/**
 * @file    stdtypes.h
 *
 * @brief   Standard type definitions
 *
 * @author  Stefan Schulze <stefan.schulze@emsys.de> +49(3677)668 257\n
 *          emsys GmbH, D-98693 Ilmenau, Ehrenbergstr. 11
 *
 * $Date: 26.07.06 11:29 $
 *
 * $Id: stdtypes.h,v 1.2 2004/05/17 12:29:23 proto Exp $

 *
 ****************************************************************************/

#ifndef _USB_STDTYPES_H_FIRST_
#define _USB_STDTYPES_H_FIRST_

#define UNUSED_PARAM(p)  ((void)(p))

/** @name Processor Word Types
 *
 * This data types are defined in an processor independent way.
 */
/*@{*/
typedef unsigned char       byte;    /**< A single byte (8-bit).*/
typedef unsigned short int  word16;  /**< A 16-bit word.*/
typedef unsigned long int   word32;  /**< A 32-bit word.*/
/*@}*/

/* define minimum and maximum decissions */
#define MIN_OF(a,b)		a > b ? b : a
#define MAX_OF(a,b)		a > b ? a : b


/* defining a buffer */
typedef struct _DATA_BUFFER
{
	word16  size;
	byte*		data_ptr;
} DATA_BUF; 	



/** @name Bit Positions
 */
/*@{*/
#define bmBit0  0x0001            /**< Bit 0 */
#define bmBit1  0x0002            /**< Bit 1 */
#define bmBit2  0x0004            /**< Bit 2 */
#define bmBit3  0x0008            /**< Bit 3 */
#define bmBit4  0x0010            /**< Bit 4 */
#define bmBit5  0x0020            /**< Bit 5 */
#define bmBit6  0x0040            /**< Bit 6 */
#define bmBit7  0x0080            /**< Bit 7 */
#define bmBit8  0x0100            /**< Bit 8 */
#define bmBit9  0x0200            /**< Bit 9 */
#define bmBit10 0x0400            /**< Bit 10 */
#define bmBit11 0x0800            /**< Bit 11 */
#define bmBit12 0x1000            /**< Bit 12 */
#define bmBit13 0x2000            /**< Bit 13 */
#define bmBit14 0x4000            /**< Bit 14 */
#define bmBit15 0x8000            /**< Bit 15 */

#define bmBit16 0x00010000        /**< Bit 16 */
#define bmBit17 0x00020000        /**< Bit 17 */
#define bmBit18 0x00040000        /**< Bit 18 */
#define bmBit19 0x00080000        /**< Bit 19 */
#define bmBit20 0x00100000        /**< Bit 20 */
#define bmBit21 0x00200000        /**< Bit 21 */
#define bmBit22 0x00400000        /**< Bit 22 */
#define bmBit23 0x00800000        /**< Bit 23 */
#define bmBit24 0x01000000        /**< Bit 24 */
#define bmBit25 0x02000000        /**< Bit 25 */
#define bmBit26 0x04000000        /**< Bit 26 */
#define bmBit27 0x08000000        /**< Bit 27 */
#define bmBit28 0x10000000        /**< Bit 28 */
#define bmBit29 0x20000000        /**< Bit 29 */
#define bmBit30 0x40000000        /**< Bit 30 */
#define bmBit31 0x80000000        /**< Bit 31 */
/*@}*/

#endif			

/*************************** EOF **************************************/
