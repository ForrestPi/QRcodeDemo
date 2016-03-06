#include <iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <zbar.h>
#include <locale.h>
#include <stdlib.h> 
#define STR(s) #s

using namespace std;
using namespace zbar;

int main (int argc, char **argv)
{
	
	// create a reader
	ImageScanner scanner;

	// configure the reader
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	///***********************************opencv**************************************************************/

	CvCapture* capture;   
	capture =cvCaptureFromCAM(0) ;
	IplImage* frame;
	IplImage* frame_gray;
	frame = cvQueryFrame( capture );
	frame_gray=cvCreateImage(cvGetSize(frame),8,1);
	cvNamedWindow("Webcam",0);
	//cvResizeWindow("video",750,750);
	string str;
	while (1)
	{
		frame = cvQueryFrame( capture );
		if (!frame) break;
		//frame_gray=cvCreateImage(cvGetSize(frame),8,1);
		cvCvtColor(frame,frame_gray,CV_RGB2GRAY);
		const void *raw = frame_gray->imageData;
		// wrap image data
		Image image(cvGetSize(frame).width, cvGetSize(frame).height, "Y800", raw, cvGetSize(frame).width * cvGetSize(frame).height);

		// scan the image for barcodes
		int n = scanner.scan(image);
		if(n>0)
		{		// extract results
				for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end();  ++symbol) {
					// do something useful with results
					if(str.compare(symbol->get_data())!=0){
						cout << "decoded " << symbol->get_type_name()
						<< " symbol \"" << symbol->get_data() << '"' << endl;
						str=symbol->get_data();
					}
				}
		}
		cvShowImage("Webcam",frame_gray);
		if(cvWaitKey(10)>=0){ 
			break;  
			//image.set_data(NULL, 0);
		}
		
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Webcam");
	
	return(0);
}
