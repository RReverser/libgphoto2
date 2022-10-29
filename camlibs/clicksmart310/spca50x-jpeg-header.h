/* spca50x-jpeg-header.h
 *
 * part of libgphoto2/camlibs/clicksmart310
 *
 * This file is essentially identical to the file of the same name,
 * found in libgphoto2/camlibs/spca50x.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#ifndef CAMLIBS_CLICKSMART310_SPCA50X_JPEG_HEADER_H
#define CAMLIBS_CLICKSMART310_SPCA50X_JPEG_HEADER_H

/*
 *  Start from 0 -> 588
 *    7 -  70 : Quantization Table[0]
 *   72 - 135 : Quantization Table[1]
 */
#define SPCA50X_JPG_DEFAULT_HEADER_PART1_LENGTH 136
#define SPCA50X_JPG_DEFAULT_HEADER_PART2_LENGTH 420
#define SPCA50X_JPG_DEFAULT_HEADER_PART3_LENGTH 33
#define SPCA50X_JPG_DEFAULT_HEADER_LENGTH	589

static const unsigned
char SPCA50xJPGDefaultHeaderPart1[SPCA50X_JPG_DEFAULT_HEADER_PART1_LENGTH] = {
	/* SOI(Start of Image) */
	0xFF,0xD8,
	/* DQT(Define Quantization Table) */
	0xFF,0xDB,
	0x00,0x84,
	0x00,
	0x05,0x03,0x04,0x04,0x04,0x03,0x05,0x04,0x04,0x04,0x06,0x05,0x05,0x06,0x08,0x0D,
	0x08,0x08,0x07,0x07,0x08,0x10,0x0C,0x0C,0x0A,0x0D,0x14,0x11,0x15,0x14,0x13,0x11,
	0x13,0x13,0x16,0x18,0x1F,0x1A,0x16,0x17,0x1E,0x17,0x13,0x13,0x1B,0x25,0x1C,0x1E,
	0x20,0x21,0x23,0x23,0x23,0x15,0x1A,0x27,0x29,0x26,0x22,0x29,0x1F,0x22,0x23,0X22,
	0X01,
	0x05,0x06,0x06,0x08,0x07,0x08,0x10,0x08,0x08,0x10,0x22,0x16,0x13,0x16,0x22,0x22,
	0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
	0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,
	0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22
};
static const unsigned
char SPCA50xJPGDefaultHeaderPart2[SPCA50X_JPG_DEFAULT_HEADER_PART2_LENGTH] = {
	/* DHT(Define Huffman Table) */
	0xFF,0xC4,
	0x01,0xA2,
	0x00,0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x01,0x00,0x03,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x10,0x00,0x02,0x01,0x03,0x03,
	0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D,0x01,0x02,0x03,0x00,0x04,
	0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,
	0x91,0xA1,0x08,0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,0x33,0x62,0x72,0x82,
	0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,0x29,0x2A,0x34,0x35,0x36,
	0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,
	0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,
	0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x92,0x93,0x94,0x95,
	0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,
	0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,
	0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,
	0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0x11,0x00,0x02,
	0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,0x77,0x00,0x01,
	0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,
	0x32,0x81,0x08,0x14,0x42,0x91,0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,0xF0,0x15,0x62,
	0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,0x26,0x27,0x28,
	0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,
	0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,
	0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
	0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,
	0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,
	0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE2,0xE3,
	0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA
};
/*
 *  5 - 6 : Image Height(H Byte,L Byte)
 *  7 - 8 : Image Width(H Byte,L Byte)
 */
static const unsigned
char  SPCA50xJPGDefaultHeaderPart3[SPCA50X_JPG_DEFAULT_HEADER_PART3_LENGTH] = {
	/* SOFn(Start of Frame) */
	0xFF,0xC0,
	0x00,0x11,
	0x08,0x00,0x78,0x00,0xA0,0x03,0x01,0x21,0x00,0x02,0x11,0x01,0x03,0x11,0x01,
	/* SOS(Start of Scan) */
	0xFF,0xDA,
	0x00,0x0C,
	0x03,0x01,0x00,0x02,0x11,0x03,0x11,0x00,0x3F,0x00
};

static const unsigned char SPCA50xQTable[20][64]=
{
	/*  index 0, Q50 */
	{  16, 11, 12, 14, 12, 10, 16, 14, 13, 14, 18, 17, 16, 19, 24, 40,
       26, 24, 22, 22, 24, 49, 35, 37, 29, 40, 58, 51, 61, 60, 57, 51,
       56, 55, 64, 72, 92, 78, 64, 68, 87, 69, 55, 56, 80,109, 81, 87,
       95, 98,103,104,103, 62, 77,113,121,112,100,120, 92,101,103, 99 },
	{  17, 18, 18, 24, 21, 24, 47, 26, 26, 47, 99, 66, 56, 66, 99, 99,
       99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
	   99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
       99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },

	/*  index 1, Q70 */
	{  10,  7,  7,  8,  7,  6, 10,  8,  8,  8, 11, 10, 10, 11, 14, 24,
	   16, 14, 13, 13, 14, 29, 21, 22, 17, 24, 35, 31, 37, 36, 34, 31,
	   34, 33, 38, 43, 55, 47, 38, 41, 52, 41, 33, 34, 48, 65, 49, 52,
	   57, 59, 62, 62, 62, 37, 46, 68, 73, 67, 60, 72, 55, 61, 62, 59 },
	{  10, 11, 11, 14, 13, 14, 28, 16, 16, 28, 59, 40, 34, 40, 59, 59,
	   59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
	   59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
	   59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59 },

	/*  index 2, Q80 */
	{   6,  4,  5,  6,  5,  4,  6,  6,  5,  6,  7,  7,  6,  8, 10, 16,
	   10, 10,  9,  9, 10, 20, 14, 15, 12, 16, 23, 20, 24, 24, 23, 20,
	   22, 22, 26, 29, 37, 31, 26, 27, 35, 28, 22, 22, 32, 44, 32, 35,
	   38, 39, 41, 42, 41, 25, 31, 45, 48, 45, 40, 48, 37, 40, 41, 40 },
	{   7,  7,  7, 10,  8, 10, 19, 10, 10, 19, 40, 26, 22, 26, 40, 40,
	   40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
	   40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
	   40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },

	/*  index 3, Q85 */
	{   5,  3,  4,  4,  4,  3,  5,  4,  4,  4,  5,  5,  5,  6,  7, 12,
	    8,  7,  7,  7,  7, 15, 11, 11,  9, 12, 17, 15, 18, 18, 17, 15,
	   17, 17, 19, 22, 28, 23, 19, 20, 26, 21, 17, 17, 24, 33, 24, 26,
	   29, 29, 31, 31, 31, 19, 23, 34, 36, 34, 30, 36, 28, 30, 31, 30 },
	{   5,  5,  5,  7,  6,  7, 14,  8,  8, 14, 30, 20, 17, 20, 30, 30,
	   30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
	   30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
	   30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },

	/*  index 4, Q90 */
	{   3,  2,  2,  3,  2,  2,  3,  3,  3,  3,  4,  3,  3,  4,  5,  8,
	    5,  5,  4,  4,  5, 10,  7,  7,  6,  8, 12, 10, 12, 12, 11, 10,
	   11, 11, 13, 14, 18, 16, 13, 14, 17, 14, 11, 11, 16, 22, 16, 17,
	   19, 20, 21, 21, 21, 12, 15, 23, 24, 22, 20, 24, 18, 20, 21, 20 },
	{   3,  4,  4,  5,  4,  5,  9,  5,  5,  9, 20, 13, 11, 13, 20, 20,
	   20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	   20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
	   20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },

	/*  index 5, Q60 */
	{  13,  9, 10, 11, 10,  8, 13, 11, 10, 11, 14, 14, 13, 15, 19, 32,
	   21, 19, 18, 18, 19, 39, 28, 30, 23, 32, 46, 41, 49, 48, 46, 41,
	   45, 44, 51, 58, 74, 62, 51, 54, 70, 55, 44, 45, 64, 87, 65, 70,
	   76, 78, 82, 83, 82, 50, 62, 90, 97, 90, 80, 96, 74, 81, 82, 79 },
	{  14, 14, 14, 19, 17, 19, 38, 21, 21, 38, 79, 53, 45, 53, 79, 79,
	   79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79,
	   79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79,
	   79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79, 79 },

	/*  index 6, Q25 */
	{  32, 22, 24, 28, 24, 20, 32, 28, 26, 28, 36, 34, 32, 38, 48, 80,
	   52, 48, 44, 44, 48, 98, 70, 74, 58, 80,116,102,122,120,114,102,
	  112,110,128,144,184,156,128,136,174,138,110,112,160,218,162,174,
	  190,196,206,208,206,124,154,226,242,224,200,240,184,202,206,198 },
	{  34, 36, 36, 48, 42, 48, 94, 52, 52, 94,198,132,112,132,198,198,
	  198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,
	  198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,
	  198,198,198,198,198,198,198,198,198,198,198,198,198,198,198,198 },

	/*  index 7, Q95 */
	{   2,  1,  1,  1,  1,  1,  2,  1,  1,  1,  2,  2,  2,  2,  2,  4,
	    3,  2,  2,  2,  2,  5,  4,  4,  3,  4,  6,  5,  6,  6,  6,  5,
	    6,  6,  6,  7,  9,  8,  6,  7,  9,  7,  6,  6,  8, 11,  8,  9,
	   10, 10, 10, 10, 10,  6,  8, 11, 12, 11, 10, 12,  9, 10, 10, 10 },
	{   2,  2,  2,  2,  2,  2,  5,  3,  3,  5, 10,  7,  6,  7, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	   10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },

	/*  index 8, Q93 */
	{   2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  3,  2,  2,  3,  3,  6,
	    4,  3,  3,  3,  3,  7,  5,  5,  4,  6,  8,  7,  9,  8,  8,  7,
	    8,  8,  9, 10, 13, 11,  9, 10, 12, 10,  8,  8, 11, 15, 11, 12,
	   13, 14, 14, 15, 14,  9, 11, 16, 17, 16, 14, 17, 13, 14, 14, 14 },
	{   2,  3,  3,  3,  3,  3,  7,  4,  4,  7, 14,  9,  8,  9, 14, 14,
	   14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	   14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
	   14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14 },

	/*  index 9, Q40 */
	{  20, 14, 15, 18, 15, 13, 20, 18, 16, 18, 23, 21, 20, 24, 30, 50,
	   33, 30, 28, 28, 30, 61, 44, 46, 36, 50, 73, 64, 76, 75, 71, 64,
	   70, 69, 80, 90,115, 98, 80, 85,109, 86, 69, 70,100,136,101,109,
	  119,123,129,130,129, 78, 96,141,151,140,125,150,115,126,129,124 },
	{  21, 23, 23, 30, 26, 30, 59, 33, 33, 59,124, 83, 70, 83,124,124,
	  124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,
	  124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,
	  124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124 }
};

#endif /* !defined(CAMLIBS_CLICKSMART310_SPCA50X_JPEG_HEADER_H) */
