#ifndef COPY_STREAMS_TO_MATS_H
#define COPY_STREAMS_TO_MATS_H

//OpenCV Libraries
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\calib3d\calib3d.hpp"
#include "IncludeRSHeaders.h"
#include "Common.h"
#include <string.h>

void CopyImageStreamToColorMat(cv::Mat &CMat);
void CopyColorStreamToColorMat(PXCImage *image,PXCImage::ImageData &data,cv::Mat &ColorMat);
void CopyDepthStreamToDepthMatAndGetUVMap(PXCImage *image,PXCImage::ImageData &data,PXCPointF32* uvmap);
void CopyBlobToBlobMat(PXCBlobData::IBlob *pBlob, PXCImage *blobIm,PXCImage::ImageData &data);
void CopyDepthStreamToDepthMatAndGetUVMap(PXCImage *image,PXCImage::ImageData &data,PXCPointF32* uvmap,cv::Mat &DepthMat);
void CopyDepthImageMappedToColor_ToMat(PXCImage *ColorImage,PXCImage *DepthImage,PXCImage::ImageData &data,cv::Mat &MappedDepthMat);

#endif