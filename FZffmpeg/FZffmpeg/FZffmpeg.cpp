// FZffmpeg.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "./libavcodec/avcodec.h"

int _tmain(int argc, _TCHAR* argv[])
{
	avcodec_init();
    AVCodecContext *avctx_opts = NULL;

	avctx_opts = avcodec_alloc_context();

	return 0;
}

