/*
 * stdint.h
 *
 *  Created on: 01.03.2016
 *      Author: andi
 */

#ifndef SPLINT_INCLUDES_STDINT_H_
#define SPLINT_INCLUDES_STDINT_H_

#ifndef _SYS__STDINT_H
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

typedef unsigned char       uint_least8_t;
typedef unsigned short      uint_least16_t;
typedef unsigned int        uint_least32_t;
typedef unsigned long long  uint_least64_t;

typedef unsigned int        uint_fast8_t;
typedef unsigned int        uint_fast16_t;
typedef unsigned int        uint_fast32_t;
typedef unsigned long long  uint_fast64_t;

typedef char                int8_t;
typedef short               int16_t;
typedef int                 int32_t;
typedef long long           int64_t;

typedef char                int_least8_t;
typedef short               int_least16_t;
typedef int                 int_least32_t;
typedef long long           int_least64_t;

typedef int                 int_fast8_t;
typedef int                 int_fast16_t;
typedef int                 int_fast32_t;
typedef long long           int_fast64_t;
#endif

#ifndef _MACHINE__DEFAULT_TYPES_H
typedef uint32_t __uint32_t;

typedef int32_t  __int32_t;
typedef int64_t  __int64_t;
#endif

#endif /* SPLINT_INCLUDES_STDINT_H_ */
