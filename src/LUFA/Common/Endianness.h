/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/**
 *  Endianness and Byte Ordering macros and functions.
 */

#pragma once

#define SWAPENDIAN_16(x) (uint16_t)((((x) & 0xFF00) >> 8) | (((x) & 0x00FF) << 8))
#define SWAPENDIAN_32(x) (uint32_t)((((x) & 0xFF000000UL) >> 24UL) | (((x) & 0x00FF0000UL) >> 8UL) | \
                                    (((x) & 0x0000FF00UL) << 8UL)  | (((x) & 0x000000FFUL) << 24UL))

// Run-time
#define le16_to_cpu(x) (x)
#define le32_to_cpu(x) (x)
#define be16_to_cpu(x) SwapEndian_16(x)
#define be32_to_cpu(x) SwapEndian_32(x)
#define cpu_to_le16(x) (x)
#define cpu_to_le32(x) (x)
#define cpu_to_be16(x) SwapEndian_16(x)
#define cpu_to_be32(x) SwapEndian_32(x)

// Compile-time
#define LE16_TO_CPU(x) (x)
#define LE32_TO_CPU(x) (x)
#define BE16_TO_CPU(x) SWAPENDIAN_16(x)
#define BE32_TO_CPU(x) SWAPENDIAN_32(x)
#define CPU_TO_LE16(x) (x)
#define CPU_TO_LE32(x) (x)
#define CPU_TO_BE16(x) SWAPENDIAN_16(x)
#define CPU_TO_BE32(x) SWAPENDIAN_32(x)


/* Inline Functions: */
/** Function to reverse the byte ordering of the individual bytes in a 16 bit value.
 *
 *  \ingroup Group_ByteSwapping
 *
 *  \param[in] Word  Word of data whose bytes are to be swapped.
 *
 *  \return Input data with the individual bytes reversed.
 */
static inline uint16_t SwapEndian_16(const uint16_t Word) ATTR_WARN_UNUSED_RESULT ATTR_CONST;
static inline uint16_t SwapEndian_16(const uint16_t Word)
{
    if (GCC_IS_COMPILE_CONST(Word))
      return SWAPENDIAN_16(Word);

    uint8_t Temp;

    union
    {
        uint16_t Word;
        uint8_t  Bytes[2];
    } Data;

    Data.Word = Word;

    Temp = Data.Bytes[0];
    Data.Bytes[0] = Data.Bytes[1];
    Data.Bytes[1] = Temp;

    return Data.Word;
}

/** Function to reverse the byte ordering of the individual bytes in a 32 bit value.
 *
 *  \ingroup Group_ByteSwapping
 *
 *  \param[in] DWord  Double word of data whose bytes are to be swapped.
 *
 *  \return Input data with the individual bytes reversed.
 */
static inline uint32_t SwapEndian_32(const uint32_t DWord) ATTR_WARN_UNUSED_RESULT ATTR_CONST;
static inline uint32_t SwapEndian_32(const uint32_t DWord)
{
    if (GCC_IS_COMPILE_CONST(DWord))
      return SWAPENDIAN_32(DWord);

    uint8_t Temp;

    union
    {
        uint32_t DWord;
        uint8_t  Bytes[4];
    } Data;

    Data.DWord = DWord;

    Temp = Data.Bytes[0];
    Data.Bytes[0] = Data.Bytes[3];
    Data.Bytes[3] = Temp;

    Temp = Data.Bytes[1];
    Data.Bytes[1] = Data.Bytes[2];
    Data.Bytes[2] = Temp;

    return Data.DWord;
}

/** Function to reverse the byte ordering of the individual bytes in a n byte value.
 *
 *  \ingroup Group_ByteSwapping
 *
 *  \param[in,out] Data    Pointer to a number containing an even number of bytes to be reversed.
 *  \param[in]     Length  Length of the data in bytes.
 *
 *  \return Input data with the individual bytes reversed.
 */
static inline void SwapEndian_n(void* const Data,
                                uint8_t Length) ATTR_NON_NULL_PTR_ARG(1);
static inline void SwapEndian_n(void* const Data,
                                uint8_t Length)
{
    uint8_t* CurrDataPos = (uint8_t*)Data;

    while (Length > 1)
    {
        uint8_t Temp = *CurrDataPos;
        *CurrDataPos = *(CurrDataPos + Length - 1);
        *(CurrDataPos + Length - 1) = Temp;

        CurrDataPos++;
        Length -= 2;
    }
}
