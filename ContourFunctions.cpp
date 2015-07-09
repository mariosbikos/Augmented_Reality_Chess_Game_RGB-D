#include "ContourFunctions.h"

void FindLeftMostAndRightMostPointsOfInnerContour(
	vector<PXCPointI32> &inner_contours_vec,
	cv::Point &leftMostPt,
	cv::Point &rightMostPt
)
{
	int leftMostX=1000;
	int leftMostY=-10;
	int rightMostX=-1;
	int rightMostY=-1;


	//For each inner contour Point
	for (int i = 0; i < inner_contours_vec.size(); ++i) 
	{
		int curX=inner_contours_vec[i].x;
		int curY=inner_contours_vec[i].y;
							
		//find left-most
		if(curX < leftMostX)
		{
			leftMostX=curX;
			leftMostY=curY;
		}
		//find right-most
		if(curX > rightMostX)
		{
			rightMostX=curX;
			rightMostY=curY;
		}
	}
	leftMostPt.x=leftMostX;
	leftMostPt.y=leftMostY;
	rightMostPt.x=rightMostX;
	rightMostPt.y=rightMostY;
}


void InitializeLeftandRightMostPoints(cv::Point &leftMostPt,cv::Point &rightMostPt)
{
	leftMostPt.x=0;
	leftMostPt.y=0;
	rightMostPt.x=0;
	rightMostPt.y=0;
}

void CreatePinchNeighborhoodPoints(cv::Point &leftMostPt,
	cv::Point &rightMostPt,vector<cv::Point> &target_points_neighborhood_vec,
	int sizeOfNeighborhood)
{
	//always min number of horizontals=4
	int sizeOfHorizontalPoints=sizeOfNeighborhood;
	//int sizeOfVerticalPoints=sizeOfNeighborhood-sizeOfHorizontalPoints;

	//get horizontal points
	if((leftMostPt.x-6-sizeOfHorizontalPoints)>=0 && leftMostPt.x<=640)
	{
		int xValueHorizontal=leftMostPt.x-6;
		for(int i=0;i<sizeOfHorizontalPoints;i++)
		{
			cv::Point p(xValueHorizontal,GetYofLineabGivenX(leftMostPt,rightMostPt,xValueHorizontal) );
			target_points_neighborhood_vec.push_back(p);
			xValueHorizontal--;
		}
	}
	else
	{
		cv::Point p(leftMostPt.x,leftMostPt.y );
		target_points_neighborhood_vec.push_back(p);
	}
	

	/*xValueHorizontal=leftMostPt.x-4;
	int xValueVertical=leftMostPt.x-4;
	int verticalStep=2;
	for(int i=0;i<sizeOfVerticalPoints;i++)
	{
		if(i%2!=0)
			verticalStep = -verticalStep;

		target_points_neighborhood_vec.push_back(cv::Point(xValueHorizontal, (GetYofLineabGivenX(leftMostPoint,rightMostPoint,xValueHorizontal)) +verticalStep ));
		xValueHorizontal--;
	}*/


}



float GetAverageZValueOfNeighborhoodPoints(
	vector<cv::Point> &target_points_neighborhood_v)
{
	float average_z_of_valid_target_points_mm=0.0;
	int counter_of_valid_target_points = 0;
	pxcU16 depth=0;
	
	for(int i=0;i<target_points_neighborhood_v.size();i++)
	{
		//Get the number of pixel if image was 1D
		int coords_in_1D=(target_points_neighborhood_v[i].y * 640) + target_points_neighborhood_v[i].x;
		//cout<<"coords_in_1D: "<<coords_in_1D<<endl;
						
		//Get the depth value(z) of the target point i and if it is valid add it to the sum to calculate average z
		//cout<<coords_in_1D<<endl;
		if(coords_in_1D<640*480)
		{
			depth=depthValues_mm[coords_in_1D];
			if (depth != depth_invalid_value && depth>0)
			{
				counter_of_valid_target_points++;
				average_z_of_valid_target_points_mm += depth;
			}
		}
	}
	//Calculate the average depth of the 5 target points- make it zero if none is valid
	if(counter_of_valid_target_points!=0)
		return average_z_of_valid_target_points_mm/(counter_of_valid_target_points*1.0);
	else
		return 0.0;
}



void GetAverageXandYofMappedPoints(
	int &avg_x_of_mapped_points,
	int &avg_y_of_mapped_points,
	vector<cv::Point> &target_points_neighborhood_v,
	PXCPointF32* uvmap)
{
	int numberOfcorrectlyMappedNeighborhoodPoints=0;
	for(int i=0; i<target_points_neighborhood_v.size(); i++)
	{
		cv::Point mappedPt(0,0);
		if((target_points_neighborhood_v[i].y*640 + target_points_neighborhood_v[i].x)<640*480 && 
			(target_points_neighborhood_v[i].y*640 + target_points_neighborhood_v[i].x>=0))
		//Get mapped point
		{
			mappedPt.x=(int)(uvmap[target_points_neighborhood_v[i].y*640 + target_points_neighborhood_v[i].x].x * 640);
			mappedPt.y=(int)(uvmap[target_points_neighborhood_v[i].y*640 + target_points_neighborhood_v[i].x].y * 480);
			/*cv::Point mappedPt(
			(int)(uvmap[target_points_neighborhood_v[i].y*640 + target_points_neighborhood_v[i].x].x * 640),
			(int)(uvmap[target_points_neighborhood_v[i].y*640 + target_points_neighborhood_v[i].x].y * 480));
		*/
		//Calculate the average x,y of the mapped points if they are valid >0
		
		}
		
			
		if(mappedPt.x>0 && mappedPt.y>0 && mappedPt.x<640 && mappedPt.y<480)
		{
			numberOfcorrectlyMappedNeighborhoodPoints++;
			avg_x_of_mapped_points += mappedPt.x;
			avg_y_of_mapped_points += mappedPt.y;
		}
	}

	if(numberOfcorrectlyMappedNeighborhoodPoints>0)
	{
		//pixels
		avg_x_of_mapped_points = avg_x_of_mapped_points/numberOfcorrectlyMappedNeighborhoodPoints;
		avg_y_of_mapped_points = avg_y_of_mapped_points/numberOfcorrectlyMappedNeighborhoodPoints;
	}
	else 
	{
		avg_x_of_mapped_points=-1;
		avg_y_of_mapped_points=-1;
	}
}
