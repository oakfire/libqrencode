#pragma once
#include "../../qrencode.h"

class GQrcode
{
private:
	GQrcode();
	static QRcode *encode(const  char *intext, int length);

public:
	static HBITMAP qrencode(const  char *intext, int length);
	static const int PIXEL;
	static const DWORD FORE_COLOR;
	static const DWORD BACK_COLOR;
	static const int BMP_SIZE;
};

