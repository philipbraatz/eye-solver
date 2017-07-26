#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

class Veiwer
{
public:
	Veiwer(Rect area) {
		if (area.width != 0)
		{
			hdcSys = GetDC(NULL); // Get DC of the target capture..
			hdcMem = CreateCompatibleDC(hdcSys); // Create compatible DC 

			size = area;

			// Initialize DCs
			// Create hBitmap with Pointer to the pixels of the Bitmap
			ZeroMemory(&bi, sizeof(BITMAPINFO));
			bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bi.bmiHeader.biWidth = size.width;
			bi.bmiHeader.biHeight = -size.height;  //negative so (0,0) is at top left
			bi.bmiHeader.biPlanes = 1;
			bi.bmiHeader.biBitCount = 32;
			hdc = GetDC(NULL);

			hBitmap = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &ptrBitmapPixels, NULL, 0);
			// ^^ The output: hBitmap & ptrBitmapPixels
			// Set hBitmap in the hdcMem 
			SelectObject(hdcMem, hBitmap);
			// Set matBitmap to point to the pixels of the hBitmap
			matBitmap = Mat(size.height, size.width, CV_8UC4, ptrBitmapPixels, 0);
			//              ^^ note: first it is y, then it is x. very confusing
			// * SETUP DONE *
		}
		else
		{
			cout << "Veiwer not loader" << endl;
		}
		
	}

	//call to get new image
	Mat Capture(float WidthScale=1, float HeightScale=1) {
		// Now update the pixels using BitBlt
		BitBlt(hdcMem, 0, 0, size.width, size.height, hdcSys, size.x, size.y, SRCCOPY);
		StretchBlt(hdcMem, 0, 0, WidthScale, HeightScale, hdcMem, 0, 0, size.width, size.height, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !

		return matBitmap;
	}

	void SelectHWND(float scale) {


	}

	~Veiwer() {
		DeleteObject(hBitmap);
		DeleteDC(hdcSys);
		DeleteDC(hdcMem);
	}

private:
	HBITMAP hBitmap; // <-- The image represented by hBitmap
	Mat matBitmap;

	HDC hdcSys = GetDC(NULL); // Get DC of the target capture..
	HDC hdcMem = CreateCompatibleDC(hdcSys); // Create compatible DC 
	void *ptrBitmapPixels; // <-- Pointer variable that will contain the potinter for the pixels

	Rect size;

	BITMAPINFO bi; HDC hdc;
};

Mat hwnd2mat(HWND hwnd) {

	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = windowsize.bottom / 2;  //change this to whatever size you want to resize to
	width = windowsize.right / 2;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

																									   // avoid memory leak
	DeleteObject(hbwindow); DeleteDC(hwindowCompatibleDC); ReleaseDC(hwnd, hwindowDC);

	return src;
}