#include "ResizeGL.h"

void vResize( GLsizei iWidth, GLsizei iHeight )
{
	//input= TheGlWindowSize global
    TheGlWindowSize=cv::Size(iWidth,iHeight);
    //not all sizes are allowed. OpenCv images have padding at the end of each line in these that are not aligned to 4 bytes
    if (iWidth*3%4!=0) 
	{
        iWidth+=iWidth*3%4;//resize to avoid padding
        vResize(iWidth,TheGlWindowSize.height);
    }
    else 
	{
        //resize the image to the size of the GL window
        if (TheInputImage_Undistorted.rows!=0)
		{
            cv::resize(TheInputImage_Undistorted,new_frame_image,TheGlWindowSize);
		}
    }
}