#include "stdafx.h"
#include "GQrcode.h"

GQrcode::GQrcode()
{
}


QRcode * GQrcode::encode(const char *intext, int length)
{
	QRcode *code = NULL;
	int version = 0;
	QRecLevel level = QR_ECLEVEL_L;
	code = QRcode_encodeData(length, (const unsigned char*)intext, version, level);
		
	return code;
}

const int GQrcode::PIXEL = 4;
const DWORD GQrcode::FORE_COLOR = RGB(0, 0, 0);
const DWORD GQrcode::BACK_COLOR = RGB(255, 255, 255);
const INT GQrcode::BMP_SIZE = 200;
HBITMAP GQrcode::qrencode(const char *intext, int length)
{

	QRcode *code = encode(intext, length);
	int width = code->width*PIXEL;
	int height = width;

	HDC hdc = ::GetDC(NULL);
	HDC mdc = ::CreateCompatibleDC(hdc);


	HBITMAP bmp = ::CreateCompatibleBitmap(mdc, width, height);
	HGDIOBJ original = ::SelectObject(mdc, bmp);
	for (int y = 0; y < height; ++y){
		for (int x = 0; x < width; x++){
			unsigned char value = code->data[(y / PIXEL)*(code->width) + x / PIXEL];
			::SetPixel(mdc, x, y, value & 0x1 ? FORE_COLOR : BACK_COLOR);
		}
	}

	HDC sdc = ::CreateCompatibleDC(hdc);
	HBITMAP sbmp = ::CreateCompatibleBitmap(sdc, BMP_SIZE, BMP_SIZE);
	HGDIOBJ original_s = ::SelectObject(sdc, sbmp);

	::StretchBlt(sdc, 0, 0, BMP_SIZE, BMP_SIZE, mdc, 0, 0, width, height, SRCCOPY);

	::SelectObject(sdc, original_s);
	::DeleteDC(sdc);

	::SelectObject(mdc, original);	
	::DeleteObject(bmp);
	::DeleteDC(mdc);

	::ReleaseDC(NULL, hdc);
	return sbmp;
}