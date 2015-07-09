#include "CopyStreamsToMats.h"

using namespace cv;



void CopyImageStreamToColorMat(cv::Mat &CMat)
{
	pxcStatus sts=psm->AcquireFrame(true);
	if (sts<PXC_STATUS_NO_ERROR) 
	{
		cout<<"Couldn't acquire frame ! "<<endl;
		system("pause");
		exit(1);
	}

	PXCImage *colorIm=0;
	PXCImage::ImageData first_data;
	PXCCapture::Sample *sample=psm->QuerySample();
	do
	{
		colorIm = sample->color;
		if(colorIm)
		{
			sts=colorIm->AcquireAccess(PXCImage::ACCESS_READ,PXCImage::PIXEL_FORMAT_RGB24,&first_data);
			if(PXC_STATUS_NO_ERROR == sts) 
			{
				//Copy Color stream data to an openc ColorMat
				memcpy(CMat.data,first_data.planes[0],sizeof(unsigned char) * 3 * 640 * 480);
				colorIm->ReleaseAccess(&first_data);
			}
			else
				cout<<"CAN'T ACQUIRE COLOR IMAGE DATA"<<endl;
		}
	}
	while(sample==0 || colorIm==0);
}

void CopyColorStreamToColorMat(PXCImage *image,PXCImage::ImageData &data,cv::Mat &ColorMat)
{
	pxcStatus sts=image->AcquireAccess(PXCImage::ACCESS_READ,PXCImage::PIXEL_FORMAT_RGB24,&data);

	if(sts==PXC_STATUS_NO_ERROR) 
	{
		//Copy Color stream data to an openc ColorMat
		memcpy(ColorMat.data,data.planes[0],sizeof(unsigned char) * 3 * 640 * 480);
		image->ReleaseAccess(&data);
	}
	else{
		cout<<"CAN'T ACQUIRE COLOR IMAGE DATA"<<endl;
		system("pause");
		exit(0);
	}
}



void CopyDepthImageMappedToColor_ToMat(PXCImage *ColorImage,PXCImage *DepthImage,PXCImage::ImageData &data,cv::Mat &MappedDepthMat)
{
	PXCImage* mapped=projection->CreateDepthImageMappedToColor(DepthImage,ColorImage);
	pxcStatus sts=mapped->AcquireAccess(PXCImage::ACCESS_READ,&data);
	if(sts==PXC_STATUS_NO_ERROR )
	{
		memcpy(MappedDepthMat.data,data.planes[0],sizeof(unsigned short) * 640 * 480);
		//MappedDepthMat=MappedDepthMat/255;

		MappedDepthMat.convertTo(normalizedMappedDepthMat,CV_32F);

		//normalizedMappedDepthMat has float depth values in mm
		normalizedMappedDepthMat=-normalizedMappedDepthMat/1000.0f;
		//normalizedMappedDepthMat has float depth values in m
		double A=(farValue*nearValue)/(farValue-nearValue);
		double B=farValue/(farValue-nearValue);
		for(int i=0;i<normalizedMappedDepthMat.rows;i++)
		{
			for(int j=0;j<normalizedMappedDepthMat.cols;j++)
			{
				normalizedMappedDepthMat.at<float>(i,j)=(A/normalizedMappedDepthMat.at<float>(i,j))+B;
			}
		}
		mapped->ReleaseAccess(&data);
	}
}

void CopyDepthStreamToDepthMatAndGetUVMap(PXCImage *image,PXCImage::ImageData &data,PXCPointF32* uvmap,cv::Mat &DepthMat)
{
	pxcStatus sts=image->AcquireAccess(PXCImage::ACCESS_READ,&data);
	if(sts==PXC_STATUS_NO_ERROR )
	{
		//Pass depth data to depthValues pointer to array
		depthValues_mm=(pxcU16*)data.planes[0];
		//memcpy(depthValues_mm,data.planes[0], sizeof(unsigned short)*640*480);
		//cout<<sizeof(unsigned short)<<endl;
		memcpy(DepthMat.data,data.planes[0],sizeof(unsigned short) * 640 * 480);
		projection->QueryUVMap(image, uvmap);
		//Range of Mat 0-255(opencv way to draw depth) instead of 0-1 initially
		DepthMat=DepthMat*255;
		image->ReleaseAccess(&data);

	}
	else
	{
		cout<<"CAN'T ACQUIRE Depth IMAGE DATA"<<endl;

	}
}




void CopyBlobToBlobMat(PXCBlobData::IBlob *pBlob, PXCImage *blobIm,PXCImage::ImageData &data)
{
	pBlob->QuerySegmentationImage(blobIm);
	pxcStatus sts=blobIm->AcquireAccess(PXCImage::ACCESS_READ,&data);
	if(PXC_STATUS_NO_ERROR == sts)
	{
		memcpy(BlobMat.data,data.planes[0], sizeof(char) * 640 * 480);
		blobIm->ReleaseAccess(&data);
	}
	else
	{
		//BlobMat.setTo(Scalar(0)); 
		blobIm->ReleaseAccess(&data);
	}

}