#include "IdleGL.h"

void Idle()
{
	flag_found_pinch_data=false;
	KeyboardChanges();
	std::unique_ptr<PXCPointF32[]> uvmap(new PXCPointF32[640*480]);
	//+ uvmap.get() as argument passing
	//PXCPointF32 *uvmap=uvmap=
	//	new PXCPointF32[constants::COLOR_WIDTH*constants::COLOR_HEIGHT];
	//std::vector<PXCPointF32> uvmap(640*480);
	
	
	//-------CAPTURE NEW IMAGE FRAME--------------------//
		PXCImage::ImageData data;
		pxcStatus sts=psm->AcquireFrame(true);
		if (sts==PXC_STATUS_NO_ERROR) 
		{	
			PXCCapture::Sample *sample = psm->QuerySample();
			if (sample)
			{
				PXCImage *colorIm=0,*depthIm=0,*blobIm=0;
				colorIm = sample->color;
				depthIm = sample->depth;
				if(colorIm && depthIm)
				{
					CopyColorStreamToColorMat(colorIm,data,ColorMat);
					CopyDepthStreamToDepthMatAndGetUVMap(depthIm,data,uvmap.get(),DepthMat);
					CopyDepthImageMappedToColor_ToMat(colorIm,depthIm,data,MappedDepthMat);
					//After this function I have a UVMap available!

					//************PINCH GESTURE DETECTION ALGORITHM*****************************************//

					// Get extracted blobs
					blobData->Update(); // update to the current blob data
					int iBlobsNum = blobData->QueryNumberOfBlobs();
					PXCBlobData::IBlob *pBlob=nullptr;
					//Get the nearest blob-only 1
					if( (blobData->QueryBlobByAccessOrder(0, PXCBlobData::ACCESS_ORDER_NEAR_TO_FAR, pBlob))
						==PXC_STATUS_NO_ERROR)
					{
						CopyBlobToBlobMat(pBlob,blobIm,data);
						//Edo exo to BlobMat based on depth data only
						
						int nContours = pBlob->QueryNumberOfContours();
						//if there is a hole
						if(nContours==2)
						{
							vector<PXCPointI32> outerContourPoints;
							vector<PXCPointI32> innerContourPoints;
							//for each contour
							int outerContourSize = pBlob->QueryContourSize(0);
							int innerContourSize = pBlob->QueryContourSize(1);
							outerContourPoints.resize(outerContourSize);
							innerContourPoints.resize(innerContourSize);
							//cout<<innerContourSize<<endl;
							if( innerContourSize >= constants::MIN_INNER_CONTOUR_POINTS_TO_VALID_PINCH
								&& outerContourSize > 1 )
							{
								//fill vectors with the points of each contour
								pBlob->QueryContourPoints(0,outerContourSize,&outerContourPoints[0]);
								pBlob->QueryContourPoints(1,innerContourSize,&innerContourPoints[0]);
								
								//FIND THE LEFT-MOST AND RIGHT-MOST INNER CONTOUR POINTS
								//given the contour points of inner contour
								
								FindLeftMostAndRightMostPointsOfInnerContour(
								innerContourPoints,
								leftMostPoint,
								rightMostPoint);

								//Create 3 neighbor points left to the left-most point of inner contour
								//-6,-10,-14 distance in pixels from left-most point
								//Create also 2 neighbor points above and underneath the middle previous neighbor
								//MAKE A CLASS WHICH WILL HAVE 5 POINTS AS PROPERTIES
								
								vector<cv::Point> target_points_neighborhood_vec;
								//pass target_points_neighborhood_vec to fill it with neighborhood points
								//10 means 8 for horizontal and 10-8=2 for verticals
								CreatePinchNeighborhoodPoints(leftMostPoint,rightMostPoint,target_points_neighborhood_vec,8);
							    //target_points_neighborhood_vec is filled with Points
								if(target_points_neighborhood_vec.size()>0)
								{
									float average_z_of_valid_target_points_mm=GetAverageZValueOfNeighborhoodPoints(target_points_neighborhood_vec);
									//FIND THE AVERAGE X AND Y OF TARGET POINTS IN COLOR IMAGE MAPPED TO DEPTH
									int average_x_of_valid_target_points_pixels=0;
									int average_y_of_valid_target_points_pixels=0;
								
									//pass average_x and average_y as reference and also vector as reference
									GetAverageXandYofMappedPoints(average_x_of_valid_target_points_pixels,average_y_of_valid_target_points_pixels,
										target_points_neighborhood_vec,uvmap.get());

									//if(average_x_of_valid_target_points_pixels>=0 && average_y_of_valid_target_points_pixels>=0 && average_x_of_valid_target_points_pixels<640 && average_x_of_valid_target_points_pixels<480)
									if(average_x_of_valid_target_points_pixels>=0 && average_y_of_valid_target_points_pixels>=0)
									{
										PXCPoint3DF32 pinch_data_color_PIXELS;
										pinch_data_color_PIXELS.x=average_x_of_valid_target_points_pixels;
										pinch_data_color_PIXELS.y=average_y_of_valid_target_points_pixels;
										pinch_data_color_PIXELS.z=average_z_of_valid_target_points_mm;
										if(projection->ProjectColorToCamera(
											1,&pinch_data_color_PIXELS,&pinch_data_world_MM)
											== PXC_STATUS_NO_ERROR)
										{
											flag_found_pinch_data=true;
											cout.precision(2);
											//pinch_data_world_MM is the 3D Point of the target pinch in camera world coordinates
											//cout<<fixed<<"World X(cm): "<<pinch_data_world_MM.x/10<<" World Y(cm): "<<pinch_data_world_MM.y/10<<" World.Z(cm)"<<pinch_data_world_MM.z/10<<endl;
											cv::Point avgpt(average_x_of_valid_target_points_pixels,average_y_of_valid_target_points_pixels);
											MyFilledCircle(ColorMat,avgpt,cv::Scalar( 0,0,255 ),4);
										}
									}
								}
							}						
						}
						else
						{
							InitializeLeftandRightMostPoints(leftMostPoint,rightMostPoint);
						}
					}
				}
			}
		}

		cv::undistort(ColorMat,TheInputImage_Undistorted,
			TheDistCameraParameters.CameraMatrix, TheDistCameraParameters.Distorsion);
	
	    //Method for detecting Black/White Chessboard
		probDetect=TheBoardDetector.detect(TheInputImage_Undistorted);
		frameCnt++;
		if(probDetect>0.06) 
		{
			//blobConfig->SetMaxDistance(TheBoardDetector.getDetectedBoard().Tvec.at<float>(0,2)*1000);
			// … call more configuration methods

			// Apply the new configuration values
			//blobConfig->ApplyChanges();
			draw_augmented_content=true;


			//********************CHANGE THE STATE OF SYSTEM BASED ON PINCH DETECTION********************//
			//Based on whether or not a pinch gesture has been detected in the current frame,
			//we change the current state of our system
			if(flag_found_pinch_data)
			{
				//pinch+chessboard detected

				//An egine detect pinch eno itan free, pigaine se pinch-in state
				if(game.getPinchState() == PinchState::FREE )
					game.setPinchState(PinchState::PINCH_IN);
				//an itan se pinch-in kai egine detect pali pinch, pigaine se pinch continuous
				else if(game.getPinchState()==PinchState::PINCH_IN)
					game.setPinchState(PinchState::PINCH_CONTINUOUS);
				else if(game.getPinchState()==PinchState::PINCH_CONTINUOUS)
					game.setPinchState(PinchState::PINCH_CONTINUOUS);
				else if(game.getPinchState()==PinchState::PINCH_OUT )
				{
					if(bValidEngineMove)
						game.setPinchState(PinchState::ENEMY_MOVE);
					else
						game.setPinchState(PinchState::FREE);
				}
				else if(game.getPinchState()==PinchState::ENEMY_MOVE)
				{
					if(bEndOfEnemyMoveFlag==true)
						game.setPinchState(PinchState::FREE);
					else
						game.setPinchState(PinchState::ENEMY_MOVE);
				}
			}
			else{

				if(game.getPinchState()==PinchState::PINCH_IN)
					game.setPinchState(PinchState::PINCH_OUT);
				else if(game.getPinchState()==PinchState::PINCH_OUT)
				{
					if(bValidEngineMove==true)
						game.setPinchState(PinchState::ENEMY_MOVE);
					else
						game.setPinchState(PinchState::FREE);
				}
				else if(game.getPinchState()==PinchState::ENEMY_MOVE)
				{
					if(bEndOfEnemyMoveFlag==true )
						game.setPinchState(PinchState::FREE);
					else
						game.setPinchState(PinchState::ENEMY_MOVE);
				}
				else if(game.getPinchState()==PinchState::PINCH_CONTINUOUS)
				{
					boolVec[frameCnt%constants::numFrames]=1;
					cout<<"VECTOR FULL: "<<std::accumulate(boolVec.begin(), boolVec.end(), 0)<<" / "<<boolVec.size()<<endl;
					if(std::accumulate(boolVec.begin(), boolVec.end(), 0)==boolVec.size())
					{
						game.setPinchState(PinchState::PINCH_OUT);
						frameCnt=0;
						for(int i=0;i<boolVec.size();i++)
						{
							boolVec[i]=0;
						}
					}
				}
				else if(game.getPinchState()==PinchState::FREE)
					game.setPinchState(PinchState::FREE);
			}

			//***************************************************************************************//
		}
		else
			draw_augmented_content=false;
		

		//*****************************ACTIONS BASED ON THE CURRENT STATE OF THE SYSTEM************************//
		//After setting the new State of our system, we can now proceed and make use of algorithms for 
		//the detection of the 3D position of pinch and the correct rendering of objects.
		if(game.getPinchState()==PinchState::FREE)
		{
			bRenderStaticBoardFlag=true;
			bRenderMovingPawnFlag=false;
		}
		else if(game.getPinchState()==PinchState::PINCH_IN)
			//bRenderStaticBoardFlag=false;
		{
			pinch_point_wrt_chessboard_M=GetPinchPointwrtChessboard(pinch_data_world_MM);
		    float dist=
				CalculateDistanceBetweenPinchPointAndCentersOfSquaresWithWhitePiece
				(pinch_point_wrt_chessboard_M,SelectedStartSquare);
			//cout<<"Distance: "<<dist<<endl;
			//cout<<"Closest square: "<<SelectedStartSquare.x<<" , "<<SelectedStartSquare.y<<endl;
			if(dist>constants::DISTANCE_THRESHOLD_FOR_PIECE_SELECTION)
			{
				//Way too far away for selecting a piece
				cout<<"PINCH TOO FAR AWAY FROM A PIECE"<<endl;
				SelectedStartSquare.x=-1;
				SelectedStartSquare.y=-1;
				bRenderStaticBoardFlag=true;
				bRenderMovingPawnFlag=false;
			}
			else
			{
				bRenderStaticBoardFlag=false;
				bRenderMovingPawnFlag=true;
			}
		}
		else if(game.getPinchState()==PinchState::PINCH_CONTINUOUS)
		{
			if(SelectedStartSquare.x!=-1 && SelectedStartSquare.y!=-1)
			{
				pinch_point_wrt_chessboard_M=GetPinchPointwrtChessboard(pinch_data_world_MM);
				//A piece was selected during pinch-in
				bRenderStaticBoardFlag=false;
				bRenderMovingPawnFlag=true;
			}
			else
			{
				cout<<"No piece has been selected!"<<endl;
				cout<<"Please PINCH-IN AGAIN"<<endl;
				bRenderStaticBoardFlag=true;
				bRenderMovingPawnFlag=false;
			}
		}
		else if(game.getPinchState()==PinchState::PINCH_OUT)
		{
			if(SelectedStartSquare.x==-1 && SelectedStartSquare.y==-1)
			{
				cout<<"No piece has been selected!"<<endl;
				cout<<"Please PINCH-IN AGAIN"<<endl;
				bValidEngineMove=false;
				bRenderStaticBoardFlag=true;
				bRenderMovingPawnFlag=false;
				//valid engine move=false
			}
			else
			{
				float dist2=CalculateDistanceBetweenPinchPointAndPossibleEndSquare
				(pinch_point_wrt_chessboard_M,SelectedEndSquare);
				bValidEngineMove=
					game.CallPlayerMove(SelectedStartSquare,SelectedEndSquare);
				
				bRenderStaticBoardFlag=true;
				bRenderMovingPawnFlag=false;
			}
		}
		else if(game.getPinchState()==PinchState::ENEMY_MOVE)
		{
			
			if(GameResult!=GameWinner::ENEMY && GameResult!=GameWinner::USER)
			{

				if(bEnemyMovingPawn==false)
				{
					//pare apo to AI tin Move (arxiko-teliko)
					game.CallEnemyMove();
					bEnemyMovingPawn=true;
					changingStartPoint=Start3DPositionOfEnemyPiece;
					diffvec.x=End3DPositionOfEnemyPiece.x-Start3DPositionOfEnemyPiece.x;
					diffvec.y=End3DPositionOfEnemyPiece.y-Start3DPositionOfEnemyPiece.y;
					diffvec.z=End3DPositionOfEnemyPiece.z-Start3DPositionOfEnemyPiece.z;
					diffvec.x/=Calculate3DDistanceOf2Points(Start3DPositionOfEnemyPiece,End3DPositionOfEnemyPiece);
					diffvec.y/=Calculate3DDistanceOf2Points(Start3DPositionOfEnemyPiece,End3DPositionOfEnemyPiece);
					
					bRenderStaticBoardFlag=false;
					bRenderMovingPawnFlag=false;
					bEndOfEnemyMoveFlag=false;
				}

				else
				{
					bFlagTimer=true;
				}
			}	
		}
		//*****************************************************************************************************//

		//cout<<game.getPinchState()<<endl;

		//Change the inpur BGR frame to RGB for OpenGL
		cv::cvtColor(TheInputImage_Undistorted,TheInputImage_RGB,CV_BGR2RGB);
		//TheInputImage_RGB becomes new_frame_image
		cv::resize(TheInputImage_RGB,new_frame_image,TheGlWindowSize);
		
		psm->ReleaseFrame();

		glutPostRedisplay();
}