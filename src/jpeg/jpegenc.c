
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jpeglib.h"

#include "jpeg/jpegenc.h"

#define logd(format, args...) 	printf(format"\r\n", ##args)
#define logv(format, args...) 	printf(format"\r\n", ##args)
#define loge(format, args...) 	printf(format"\r\n", ##args)

#define THUMB_BUF_SIZE  (1024 * 1024)

static const unsigned char huf_tbl_wr[] = {
	0xff, 0xc4, 0x00, 0x1f, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0a, 0x0b, 0xff, 0xc4, 0x00, 0xb5, 0x10, 0x00, 0x02,
	0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04,
	0x00, 0x00, 0x01, 0x7d, 0x01, 0x02, 0x03, 0x00, 0x04, 0x11,
	0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
	0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42,
	0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72,
	0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26,
	0x27, 0x28, 0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
	0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53,
	0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65,
	0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77,
	0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a,
	0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2,
	0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
	0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3, 0xe4,
	0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4,
	0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xff, 0xc4, 0x00, 0x1f,
	0x01, 0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
	0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0xff,
	0xc4, 0x00, 0xb5, 0x11, 0x00, 0x02, 0x01, 0x02, 0x04, 0x04,
	0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,
	0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06,
	0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22, 0x32, 0x81,
	0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33,
	0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
	0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44,
	0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54, 0x55, 0x56,
	0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
	0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92,
	0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3,
	0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
	0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
	0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
	0xd7, 0xd8, 0xd9, 0xda, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
	0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa
};

int JpegSetParameter(void *handle, int indexType, void *param)
{
	int result = 0;
	JpegCtx *jpegCtx = (JpegCtx *)handle;

	switch (indexType) {
	case VENC_IndexParamJpegQuality:
		jpegCtx->quality = *((int *)param);
		if (jpegCtx->quality >= 96)
			jpegCtx->quality = 99;
		else if (jpegCtx->quality >= 90)
			jpegCtx->quality = 95;
		break;
	case VENC_IndexParamJpegEncMode:
		jpegCtx->JpgColorFormat = *((unsigned int *)param);
		break;
	case VENC_IndexParamSetVsize:
		jpegCtx->image_height = *((unsigned int *)param);
		break;
	case VENC_IndexParamSetHsize:
		jpegCtx->image_width = *((int *)param);
		break;
	default:
		loge("jpeg do not support this %d indexType", indexType);

		result = -1;
		break;
	}

	return result;
}

void JPEG_Write_Header(void *handle)
{
	int i;

	JpegCtx *jpegCtx = (JpegCtx *)handle;
	if (!jpegCtx) {
		loge("invalid handle");
		return;
	}

	emit_marker(jpegCtx, M_SOI);

	//-------------------JFIF marker: 16 bytes
	emit_marker(jpegCtx, M_APP0);
	emit_2bytes(jpegCtx, 2 + 4 + 1 + 2 + 1 + 2 + 2 + 1 + 1); /* length */
	emit_byte(jpegCtx, 0x4A);	/* Identifier: ASCII "JFIF" */
	emit_byte(jpegCtx, 0x46);
	emit_byte(jpegCtx, 0x49);
	emit_byte(jpegCtx, 0x46);
	emit_byte(jpegCtx, 0);
	emit_byte(jpegCtx, 1); /* Version fields */
	emit_byte(jpegCtx, 1);
	emit_byte(jpegCtx, 0);
	emit_2bytes(jpegCtx,1);/* Pixel size information */
	emit_2bytes(jpegCtx,1);
	emit_byte(jpegCtx, 0); /* No thumbnail image */
	emit_byte(jpegCtx, 0);

	//-------------------DQT
	emit_dqt(jpegCtx,0);
	emit_dqt(jpegCtx,1);
	//-------------------SOF
	emit_sof(jpegCtx);
	//-------------------DHT
	for(i=0;i<sizeof(huf_tbl_wr);i++)
		emit_byte(jpegCtx,huf_tbl_wr[i]);
	//-------------------SOS
	emit_sos(jpegCtx);

}

static struct jpeg_ctl_ops encoder_jpeg = {
	.writeHeader  = JPEG_Write_Header,
	.setParameter = JpegSetParameter,
	.setQuantTbl  = jpeg_set_quant_tbl,
};

JpegCtx *JpegEncCreate()
{
	JpegCtx *jpegCtx = (JpegCtx *)malloc(sizeof(JpegCtx));

	if (!jpegCtx) {

		loge("Create jpegContext error.");
	}
	memset(jpegCtx, 0, sizeof(JpegCtx));

	jpegCtx->ctl_ops = &encoder_jpeg;

	return jpegCtx;
}

void JpegEncDestory(void *handle)
{
	JpegCtx *jpegCtx = (JpegCtx *)handle;
	if (!jpegCtx) {
		loge("invalid handle");
		return;
	}

	free(jpegCtx);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

