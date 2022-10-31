/*
 * This file convert raw CCD bitmaps to BMP. Used for thumbnails.
 * Code probably grafted from Largan Lmini SDK.
 */

#include <string.h>

#include "lmini_ccd.h"

typedef struct {
	char *data;

	long	out_index;
	long	count;
	int	in_bit;

	unsigned char BUFF11[14400];

	unsigned long in_string;
	int	pre_y, pre_cb, pre_cr;

	int y[7446];
} largan_ctx;

static void	fetchstr(largan_ctx *, int, int, int);
static void	dhuf(largan_ctx *, int);
static void	YCbCr2RGB(unsigned char *BUFF11, const int *YY, int Cb, int Cr, int w, int h);

static const int y_max[10]= {
	0x0,  0x0,  0x0,  0x6,
	0xe,  0x1e, 0x3e, 0x7e,
	0xfe, 0x1fe,
	};

static const int y_min[10]= {
	0x0,  0x0,  0x0,  0x2,
	0xe,  0x1e, 0x3e, 0x7e,
	0xfe, 0x1fe,
	};

static const int uv_max[12]= {
	0x0,  0x0,   0x2,   0x6,
	0xe,  0x1e,  0x3e,  0x7e,
	0xfe, 0x1fe, 0x3fe, 0x7fe
	};

static const int uv_min[12]= {
	0x0,  0x0,   0x0,   0x6,
	0xe,  0x1e,  0x3e,  0x7e,
	0xfe, 0x1fe, 0x3fe, 0x7fe
	};


/* --------------------------------------------------------------------------- */
void largan_ccd2dib(char *pData, char *pDib, long dwDibRowBytes, int nCcdFactor)
{
    int i, j, w, h;
    int YY[4], Cb, Cr;

/************************************************/

    Cb = 0;
    Cr = 0;

    // TODO: this is quite large, should we allocate it on the heap instead?
    largan_ctx ctx = {
	.data = pData,
	.in_string = (*pData << 8) | *(pData + 1),
	.in_bit = 16,
	.count = 2,
    };

/******************************************************/

    for (j = 0; j < 1200; j++)
{
	for (i = 0; i < 4; i++)
	    dhuf(&ctx, 0);

	dhuf(&ctx, 1);
	dhuf(&ctx, 2);
	}

    /* printf("Now process the DC term to R G B transfer !\n"); */

    for (h = 0; h < 30; h++) /* mini size height = 30 x 2 */
	{
	for (w = 0; w < 40; w++)
	    {
	    for (i = 0; i < 6; i++)
		{
		int	k = i + w * 6 + h * 240;

		if (i < 4)
		    YY[i] = ctx.y[k] * nCcdFactor;
		else if (i == 4)
		    Cb = ctx.y[k] * nCcdFactor;
		else if (i == 5)
		    Cr = ctx.y[k] * nCcdFactor;
		}

	    YCbCr2RGB(ctx.BUFF11, YY, Cb, Cr, w, h);
	    }
	}

    for (i = 0, j = 0; i < 60; i++, j += 240)
	{
	memcpy (pDib, &ctx.BUFF11[j], 240);
	pDib -= dwDibRowBytes;
	}
}

/* --------------------------------------------------------------------------- */
static void YCbCr2RGB(unsigned char *BUFF11, const int *YY, int Cb, int Cr, int w, int h)
{
    int     i;
    double  B,G,R;
    double  c2 = 1.7753, c3 = -0.0015;
    double  c5 = -0.3443, c6 = -0.7137;
    double  c8 = -0.0009, c9 = 1.4017;

    for (i = 0; i < 2; i++)
	{
	int	k = 3 * i + w * 6 + h * 480;

	B = ((double) YY[i] + 128.0 + c2 * Cb + c3 * Cr) + 0.5;
	if (B > 255)
	   B = 255;
	else if (B < 0)
	   B = 0;
	BUFF11[k] = (unsigned char) B;

	G = ((double) YY[i] + 128.0 + c5 * Cb + c6 * Cr) + 0.5;
	if (G > 255)
	   G = 255;
	else if (G < 0)
	   G = 0;
	BUFF11[k + 1] = (unsigned char) G;

	R = ((double) YY[i] + 128.0 + c8 * Cb + c9 * Cr) + 0.5;
	if (R > 255)
	   R = 255;
	else if (R < 0)
	   R = 0;
	BUFF11[k + 2] = (unsigned char) R;
	}

    for (i = 0; i < 2; i++)
	{
	int	k = 3 * i + w * 6 + h * 480 + 240;

	B = ((double) YY[i + 2] + 128.0 + c2 * Cb + c3 * Cr) + 0.5;
	if (B > 255)
	   B = 255;
	else if (B < 0)
	   B = 0;
	BUFF11[k] = (unsigned char) B;

	G = ((double) YY[i + 2] + 128.0 + c5 * Cb + c6 * Cr) + 0.5;
	if (G > 255)
	   G = 255;
	else if (G < 0)
	   G = 0;
	BUFF11[k + 1] = (unsigned char) G;

	R = ((double) YY[i + 2] + 128.0 + c8 * Cb + c9 * Cr) + 0.5;
	if (R > 255)
	   R = 255;
	else if (R < 0)
	   R = 0;
	BUFF11[k + 2] = (unsigned char) R;
	}
}

/* --------------------------------------------------------------------------- */
static void dhuf(largan_ctx *ctx, int flag)
{
    int     code_leng, val_leng;
    int    temp_s;
    int    temp;
    int     temp1;

    int in_string = (int) ctx->in_string;

    code_leng = 2;
    val_leng = 0;
    temp_s = in_string;
    temp_s >>= 14;

    if (flag == 0)
	{
	while (((int)temp_s > y_max[code_leng]) || ((int)temp_s < y_min[code_leng]))
	    {
	    code_leng++;
	    temp_s = in_string;
	    temp_s >>= (16 - code_leng);
	    }
	}
    else
	{
	while (((int)temp_s > uv_max[code_leng]) || ((int)temp_s < uv_min[code_leng]))
	    {
	    code_leng++;
	    temp_s = in_string;
	    temp_s >>= (16 - code_leng);
	    }
	}

    temp = in_string;
    temp >>= (16 - code_leng);
    temp1 = (int) temp;

    fetchstr(ctx, code_leng, 0, flag);

    if (flag == 0)
	{
	if (code_leng == 3)
	    {
	    switch (temp1)
		{
		case 2 : val_leng = 1; break;
		case 3 : val_leng = 2; break;
		case 4 : val_leng = 3; break;
		case 5 : val_leng = 4; break;
		case 6 : val_leng = 5; break;
		default: break;
		}
	    }
	else if (code_leng == 2)
	    val_leng = 0;
	else
	    val_leng = code_leng + 2;
	}
    else
	{
	if (code_leng == 2)
	    {
	    switch (temp1)
		{
		case 0 : val_leng = 0; break;
		case 1 : val_leng = 1; break;
		case 2 : val_leng = 2; break;
		default: break;
		}
	    }
	else
	    val_leng = code_leng;
	}

    fetchstr(ctx, val_leng, 1, flag);
}


/* --------------------------------------------------------------------------- */
static void fetchstr(largan_ctx *ctx, int shift_bit, int val_flag, int flag)
{
    int    temp_val;
    int     value1, value2;
    int     temp = 0;

    temp_val = (int) ctx->in_string;
    temp_val >>= (16 - shift_bit);
    ctx->in_string <<= shift_bit;
    ctx->in_bit -= shift_bit;

    if ((val_flag == 1) && (shift_bit == 0))
	{
	if (flag == 0)
	    temp = ctx->pre_y;
	else if (flag == 1)
	    temp = ctx->pre_cb;
	else if (flag == 2)
	    temp = ctx->pre_cr;

	ctx->y[ctx->out_index] = temp;
	ctx->out_index++;
	}

    if ((val_flag == 1) && (shift_bit != 0))
	{
	value1 = (int) temp_val;
	value2 = value1;
	value2 &= (1 << (shift_bit - 1));

	if (value2 != 0)
	    {
	    if (flag == 0)
		{
		ctx->pre_y += value1;
		temp = ctx->pre_y;
		}
	    else if (flag == 1)
		{
		ctx->pre_cb += value1;
		temp = ctx->pre_cb;
		}
	    else if (flag == 2)
		{
		ctx->pre_cr += value1;
		temp=ctx->pre_cr;
		}

	    ctx->y[ctx->out_index] = temp;
	    ctx->out_index++;
	    }
	else
	    {
	    value1++;
	    value1 = -value1;

	    value1 &= ((1 << shift_bit) - 1);
	    value1 = -value1;

	    if (flag == 0)
		{
		ctx->pre_y += value1;
		temp = ctx->pre_y;
		}
	    else if (flag == 1)
		{
		ctx->pre_cb += value1;
		temp = ctx->pre_cb;
		}
	    else if (flag == 2)
		{
		ctx->pre_cr +=value1;
		temp = ctx->pre_cr;
		}

	    ctx->y[ctx->out_index] = temp;
	    ctx->out_index++;
	    }
	}

    while (ctx->in_bit <= 8)
	{
	ctx->in_string = ctx->in_string | (*(ctx->data + ctx->count) << (8 - ctx->in_bit));
	ctx->in_bit += 8;
	ctx->count++;
	}
}
