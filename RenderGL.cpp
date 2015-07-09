#include "RenderGL.h"


void Render()
{
	//Only proceed if the frame image is initialized
	if (new_frame_image.rows==0) 
	{
		cout<<"new frame rows=0"<<endl;
		return;
	}

	cv::imshow("Blob Data",BlobMat);
	//cv::imshow("DepthMapped",normalizedMappedDepthMat);
	//cv::imshow("Depth Data",DepthMat);
	//glClear needs to be made to clear the depth and color buffers
	//with the values passed to glClearDepth and glClearColor.	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//Draw the RGB Video stream on the opengl window
	
	// Change to the projection matrix and set our viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, new_frame_image.cols-0.5, new_frame_image.rows-0.5, -0.5, -1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0,0,new_frame_image.cols,new_frame_image.rows);


	//CREATE A TEXTURED QUAD FOR VIDEO STREAM
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		GLuint videoTex = matToTexture(new_frame_image);
		EnableRenderingBasedOnColor();
		glColor3f(1,1,1);
		//Video texture
		glBindTexture(GL_TEXTURE_2D, videoTex);
		glBegin(GL_QUADS);
		//VALE NORMALS PRIN APO KATHE VERTEX3I!!!!!
			glTexCoord2f(0.0, 0.0); 
			glNormal3f(0,0,1);
			glVertex3i(0, 0,-100);
			glTexCoord2f(0.0, 1.0); 
			glNormal3f(0,0,1);
			glVertex3i(0, new_frame_image.rows,-100);
			glTexCoord2f(1.0, 1.0); 
			glNormal3f(0,0,1);
			glVertex3i(new_frame_image.cols,new_frame_image.rows,-100);
			glTexCoord2f(1.0, 0.0); 
			glNormal3f(0,0,1);
			glVertex3i(new_frame_image.cols, 0,-100);
		glEnd();

		glDeleteTextures(1, &videoTex);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//DrawVideoStreamInOpenGLWindow();
	

	if(GameResult==GameWinner::USER)
		{
			//DRAW TEXT USER WINS
			glPushMatrix();
			EnableRenderingBasedOnColor();
			glColor3f(1,0.5,0);
			glTranslatef(new_frame_image.cols/2.0,new_frame_image.rows/2,0);
			glScalef(0.2,-0.2,-0.2);
			DrawString("YOU WIN!!!");
			glPopMatrix();
		}
			
		
		if(GameResult==GameWinner::ENEMY)
		{
			//DRAW TEXT ENEMY WINS
			
			glPushMatrix();
			EnableRenderingBasedOnColor();
			glColor3f(1,0.5,0);
			glTranslatef(new_frame_image.cols/2.0,new_frame_image.rows/2,0);
			glScalef(0.2,-0.2,-0.2);
			DrawString("YOU LOSE!!!");
			glPopMatrix();
		}

	//DRAW DEPTH DATA IN THE BUFFER FOR OCCLUSION EFFECTS
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glPixelZoom( 1, -1); //flip image vertically
	glRasterPos3f( 0, 0, -1.0 );
	glDrawPixels(640,480,GL_DEPTH_COMPONENT,GL_FLOAT,normalizedMappedDepthMat.ptr());
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );


	//AUGMENTED CONTENT
	//Get the Projection Matrix from Aruco and camera parameters
	glMatrixMode(GL_PROJECTION);
	double proj_matrix[16];
	TheCameraParameters.glGetProjectionMatrix(TheInputImage_Undistorted.size(),
		TheGlWindowSize,proj_matrix,nearValue,farValue);
	glLoadIdentity();
	//load the projection matrix acquired from aruco
	glLoadMatrixd(proj_matrix);

	double modelview_matrix[16];
	//WHEN THE BOARD IS DETECTED WITH A VALUE OVER A PROBABILITY 0.1
	if (draw_augmented_content==true) 
	{
		//Calculate the OpenGL modelview matrix from the detected board
		//in order to render everything w.r.t the chessboard center
		TheBoardDetector.getDetectedBoard().glGetModelViewMatrix(modelview_matrix);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//Setting this matrix,the reference corrdinate system is set in center of board
		glLoadMatrixd(modelview_matrix);
		
		//--------------------------------LIGHTING SETTINGS------------------------------//
		//Set up light source parameters
		//setup light position
		GLfloat light_position[] = { 0.45,0.15,0.4, 1.0 };

		//setup light color for ambient--diffuse--specular
		GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
		GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
		GLfloat specularLight[] = { 1.0, 1, 1, 0.1 };	//Ls of light 1
		//Now pass the light parameters to the system
		glLightfv( GL_LIGHT1, GL_AMBIENT, ambientLight );
		glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuseLight );
		glLightfv( GL_LIGHT1, GL_SPECULAR, specularLight);
		glLightfv( GL_LIGHT1, GL_POSITION, light_position);
	
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);
		
		//draw colored axis on the chessboard center
		axis(constants::BOARD_SIZE);

		if ( TheBoardDetector.isYPerpendicular() ) 
			glTranslatef(0,TheMarkerSize/2,0);
		else 
			glTranslatef(0,0,TheMarkerSize/2);

		DrawChessBoard();

		if(bRenderStaticBoardFlag==true)
			DrawStaticChessPieces(game);
		else if(bRenderMovingPawnFlag==true)
		{
			//cout<<pinch_point_wrt_chessboard_M.x<<" , "<<pinch_point_wrt_chessboard_M.y<<" , "<<pinch_point_wrt_chessboard_M.z<<endl;
			
			DrawProjectionOfPinchPoint3D();
			DrawBoardExceptForSelectedMovingPiece(game);
			DrawMovingPiece(game.mqGameBoard.p_PieceAtSquare[SelectedStartSquare.x][SelectedStartSquare.y]->GetPiece(),
				game.mqGameBoard.p_PieceAtSquare[SelectedStartSquare.x][SelectedStartSquare.y]->GetColor(),
				pinch_point_wrt_chessboard_M);
		}
		
		else if(bEnemyMovingPawn==true)
		{
			//drawallexcept the one of square EnemyStartSquareCoordinates
			DrawBoardExceptForSelectedENEMYMovingPiece(game);
			//Draw moving enemy pawn
			DrawEnemyMovingPiece(game.mqGameBoard.p_PieceAtSquare[EnemyStartSquareCoordinates.x][EnemyStartSquareCoordinates.y]->GetPiece(),
				game.mqGameBoard.p_PieceAtSquare[EnemyStartSquareCoordinates.x][EnemyStartSquareCoordinates.y]->GetColor());
		}
	}
	
	glutSwapBuffers();
}


// Function turn a cv::Mat into a texture, and return the texture ID as a GLuint for use
GLuint matToTexture(cv::Mat &mat)
{
	// Generate a number for our textureID's unique handle
	GLuint tex;
	glGenTextures(1, &tex);

	// Bind to our texture handle
	glBindTexture(GL_TEXTURE_2D, tex);

	// Set texture interpolation methods for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set incoming texture format to:
	// GL_BGR       for CV_CAP_OPENNI_BGR_IMAGE,
	// GL_LUMINANCE for CV_CAP_OPENNI_DISPARITY_MAP,
	// Work out other mappings as required ( there's a list in comments in main() )
	GLenum inputColourFormat = GL_RGB;
	GLenum matDataType = GL_UNSIGNED_BYTE;
	if (mat.channels() == 1)
	{
		inputColourFormat = GL_LUMINANCE;
		matDataType=GL_UNSIGNED_SHORT;
	}

	// Create the texture
	glTexImage2D(GL_TEXTURE_2D,     // Type of texture
	             0,                 // Pyramid level (for mip-mapping) - 0 is the top level
	             GL_RGB,            // Internal colour format to convert to
	             mat.cols,          // Image width  i.e. 640 for Kinect in standard mode
	             mat.rows,          // Image height i.e. 480 for Kinect in standard mode
	             0,                 // Border width in pixels (can either be 1 or 0)
	             inputColourFormat, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
				 matDataType,  // Image data type
	             mat.ptr());        // The actual image data itself

	return tex;
}


void DrawString(char *string)
{
	
	char *c;
	 glRasterPos3f(320,240,0);
	for (c=string; *c != '\0'; c++) 
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
}

void DrawPinchPoint3D()
{
	EnableRenderingBasedOnColor();
	glPushMatrix();
			glColor3f (0,1,1 );
			glPointSize(5);
			//glScalef(1.055,1.055,1.055);
			glBegin(GL_POINTS);
				glVertex3f(0.0f, 0.0f, 0.0f); // chessboard origin
				glVertex3f(pinch_point_wrt_chessboard_M.x,pinch_point_wrt_chessboard_M.y, pinch_point_wrt_chessboard_M.z); // ending point of the line
			glEnd( );
		glPopMatrix();
}
void DrawProjectionOfPinchPoint3D()
{
	EnableRenderingBasedOnColor();
	glPushMatrix();
		glLineWidth(2.0);
		glColor3f (0,1,0 );
		//glScalef(1.055,1.055,1.055);
		glBegin(GL_LINES);
		glVertex3f(pinch_point_wrt_chessboard_M.x, pinch_point_wrt_chessboard_M.y, pinch_point_wrt_chessboard_M.z); // chessboard origin
		glVertex3f(pinch_point_wrt_chessboard_M.x, pinch_point_wrt_chessboard_M.y, 0.0f); // ending point of the line
		glEnd( );
	glPopMatrix();
	glLineWidth(1.0);
}

void axis(float size)
{
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable(GL_COLOR_MATERIAL);

	//x
	glColor3f (1,0,0 );
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	glVertex3f(size,0.0f, 0.0f); // ending point of the line
	glEnd( );
	//y
	glColor3f ( 0,1,0 );
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	glVertex3f( 0.0f,size, 0.0f); // ending point of the line
	glEnd( );
	//z
	glColor3f (0,0,1 );
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); // origin of the line
	glVertex3f(0.0f, 0.0f, size); // ending point of the line
	glEnd( );

}


void DrawVideoStreamInOpenGLWindow()
{
	/*
	//With the following 5 commands, we specify modelview and projection matrices
	//for 2D rendering(and z=-1 -> 1)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, TheGlWindowSize.width, 0, TheGlWindowSize.height, -1.0, 1.0);
	//----------------------2D Rendering----------------------------------------//
	glViewport(0, 0, TheGlWindowSize.width , TheGlWindowSize.height);
	glDisable(GL_TEXTURE_2D);
	

	//Specify raster position in screen coordinates
	//raster position is the origin where the next image is to be drawn
	glPixelZoom( 1, -1); //flip image vertically
	glRasterPos3f( 0, TheGlWindowSize.height  - 0.5, -1.0 );
	//After the above 2 commands--> top left corner of image to be drawn is at (0,height)

	//writes a rectangular array of pixels from data kept in processor memory 
	//into the framebuffer at the current raster position specified by glRasterPos*().
	glDrawPixels ( TheGlWindowSize.width , TheGlWindowSize.height , GL_RGB, GL_UNSIGNED_BYTE , new_frame_image.ptr(0) );
	*/
}


void DrawChessBoard()
{
	
	for(int x=0;x<8;x++)
	{
		for(int y=0;y<8;y++)
		{
			glPushMatrix();
			//glTranslatef(0,0,0.01);
			//Go to (0,0)
			glTranslatef(Offset_X_M,Offset_Y_M,0);
			//move according to given parameters
			glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0,0);
			if( (x+y)%2==0 )
				//black
				//glColor3f(0.88f,0.2f,0.0f);
			    EnableMaterialSettings(PieceMaterial::RED);
				//glColor3f(0.0f,0.0f,0.0f);
			else
				//white
				//glColor3f(1.0f,1.0f,1.0f);
				EnableMaterialSettings(PieceMaterial::WHITE);
			//glTranslatef(0,0,TheMarkerSize/2+constants::SPACE_BETWEEN_CELLS/2);
			
			glTranslatef(0,0,-0.015);
			glScalef(1.0,1.0,0.03/TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glutSolidCube(TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			//glutSolidCube(TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glPopMatrix();

		}
	}
	
	EnableRenderingBasedOnColor();
	glPushMatrix();
		glTranslatef(Offset_X_M,Offset_Y_M,0);
		glTranslatef(-1 * Offset_Pieces_M * 1.0, -1 * Offset_Pieces_M * 1.0,0);
		//draw
		for(int x=0;x<10;x++)
		{
			glPushMatrix();
			glTranslatef(x * Offset_Pieces_M * 1.0,0,0);
			glColor3f(0,0,0);
				glTranslatef(0,0,-0.015);
			glScalef(1.0,1.0,0.03/TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glutSolidCube(TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glPopMatrix();
			//move right
		}
		glPushMatrix();
		glTranslatef(0, 9 * Offset_Pieces_M * 1.0,0);
		for(int x=0;x<10;x++)
		{
			glPushMatrix();
			glTranslatef(x * Offset_Pieces_M * 1.0,0,0);
			glColor3f(0,0,0);
				glTranslatef(0,0,-0.015);
			glScalef(1.0,1.0,0.03/TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glutSolidCube(TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glPopMatrix();
			//move right
		}
		glPopMatrix();

		for(int x=1;x<9;x++)
		{
			glPushMatrix();
			glTranslatef(0,x * Offset_Pieces_M * 1.0,0);
			glColor3f(0,0,0);
				glTranslatef(0,0,-0.015);
			glScalef(1.0,1.0,0.03/TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glutSolidCube(TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glPopMatrix();
			//move right
		}
		glTranslatef(9 * Offset_Pieces_M * 1.0,0,0);
		for(int x=1;x<9;x++)
		{
			glPushMatrix();
			glTranslatef(0,x * Offset_Pieces_M * 1.0,0);
			glColor3f(0,0,0);
				glTranslatef(0,0,-0.015);
			glScalef(1.0,1.0,0.03/TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glutSolidCube(TheMarkerSize+constants::SPACE_BETWEEN_CELLS);
			glPopMatrix();
			//move right
		}
	glPopMatrix();
}

// **********************************************************************************
/*
* Summary:      Draws a chess piece specified of a given PieceType char 
*				and a given color W or B char, on the center of a square
*               specified by x,y coordinates(0,0-7,7)
//            
* Parameters:   char chessPieceType : Type of Piece(queen,king,etc)
*				char colorOfPiece : Color of the piece(W or B)
*				int x: x place of square in 8x8 grid
*				int y: y place of square in 8x8 grid
* Return:       Void(Draws in OpenGL)
*/
void DrawPieceofColorAndTypeInPosition(char chessPieceType,char colorOfPiece,
	int x, int y)
{
	if(colorOfPiece=='B')
		EnableMaterialSettings(PieceMaterial::RED);
	else if (colorOfPiece=='W')
		EnableMaterialSettings(PieceMaterial::WHITE);
	
	
	
	switch (chessPieceType) 
	{
		//pawn
	    case 'P': 
		{
			glPushMatrix();
				//Go to (0,0)
				glTranslatef(Offset_X_M,Offset_Y_M,0);
				//move according to given parameters
				glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, constants::ABOVE_GROUND_M);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_pawn.size();i++)
				{
					glNormal3f(normals_pawn[i].x,normals_pawn[i].y,normals_pawn[i].z);
					glVertex3f(vertices_pawn[i].x,vertices_pawn[i].y,vertices_pawn[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//knight horse
	    case 'N': 
		{
			glPushMatrix();
				//Go to (0,0)
				glTranslatef(Offset_X_M,Offset_Y_M,0);
				//move according to given parameters
				glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, constants::ABOVE_GROUND_M);
				//rotate because horse doesn't look straight(others are symmetrical)

				//if horse is enemy's make it look the other way around
				if(colorOfPiece=='B')
					glRotatef(-180,0.0,0.0,1.0);

				glRotatef(90,1.0,0.0,0.0);
				
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_knight.size();i++)
				{
					glNormal3f(normals_knight[i].x,normals_knight[i].y,normals_knight[i].z);
					glVertex3f(vertices_knight[i].x,vertices_knight[i].y,vertices_knight[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//bishop
		case 'B': 
		{
			glPushMatrix();
				//Go to (0,0)
				glTranslatef(Offset_X_M,Offset_Y_M,0);
				//move according to given parameters
				glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, constants::ABOVE_GROUND_M);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_bishop.size();i++)
				{
					glNormal3f(normals_bishop[i].x,normals_bishop[i].y,normals_bishop[i].z);
					glVertex3f(vertices_bishop[i].x,vertices_bishop[i].y,vertices_bishop[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//rook tower
		case 'R': 
		{
			glPushMatrix();
				//Go to (0,0)
				glTranslatef(Offset_X_M,Offset_Y_M,0);
				//move according to given parameters
				glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, constants::ABOVE_GROUND_M);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_tower.size();i++)
				{
					glNormal3f(normals_tower[i].x,normals_tower[i].y,normals_tower[i].z);
					glVertex3f(vertices_tower[i].x,vertices_tower[i].y,vertices_tower[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//queen
		case 'Q': 
		{
			glPushMatrix();
				//Go to (0,0)
				glTranslatef(Offset_X_M,Offset_Y_M,0);
				//move according to given parameters
				glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, constants::ABOVE_GROUND_M);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_queen.size();i++)
				{
					glNormal3f(normals_queen[i].x,normals_queen[i].y,normals_queen[i].z);
					glVertex3f(vertices_queen[i].x,vertices_queen[i].y,vertices_queen[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//king
		case 'K': 
		{
			glPushMatrix();
				//Go to (0,0)
				glTranslatef(Offset_X_M,Offset_Y_M,0);
				//move according to given parameters
				glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, constants::ABOVE_GROUND_M);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_king.size();i++)
				{
					glNormal3f(normals_king[i].x,normals_king[i].y,normals_king[i].z);
					glVertex3f(vertices_king[i].x,vertices_king[i].y,vertices_king[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		default:
		break;
	}
}


void DrawEnemyMovingPiece(char chessPieceType,char colorOfPiece)
{
	EnableMaterialSettings(PieceMaterial::GOLD);

	switch (chessPieceType) 
	{
		//pawn
	case 'P': 
		{
			glPushMatrix();
				
				glTranslatef(changingStartPoint.x,changingStartPoint.y,changingStartPoint.z+constants::MOVING_PAWN_OFFSET+constants::ANIMATION_Z_OFFSET );
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_pawn.size();i++)
				{
					glNormal3f(normals_pawn[i].x,normals_pawn[i].y,normals_pawn[i].z);
					glVertex3f(vertices_pawn[i].x,vertices_pawn[i].y,vertices_pawn[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//knight horse
	case 'N': 
		{
			glPushMatrix();
				glTranslatef(changingStartPoint.x,changingStartPoint.y,changingStartPoint.z+constants::MOVING_PAWN_OFFSET+constants::ANIMATION_Z_OFFSET );	
				//move according to given parameters
				//if horse is enemy's make it look the other way around
				if(colorOfPiece=='B')
					glRotatef(-180,0.0,0.0,1.0);

				glRotatef(90,1.0,0.0,0.0);
				
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_knight.size();i++)
				{
					glNormal3f(normals_knight[i].x,normals_knight[i].y,normals_knight[i].z);
					glVertex3f(vertices_knight[i].x,vertices_knight[i].y,vertices_knight[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//bishop
	case 'B': 
		{
			glPushMatrix();			
				glTranslatef(changingStartPoint.x,changingStartPoint.y,changingStartPoint.z+constants::MOVING_PAWN_OFFSET+constants::ANIMATION_Z_OFFSET );
				 //glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET);
				//move according to given parameters
				//glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, 0);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_bishop.size();i++)
				{
					glNormal3f(normals_bishop[i].x,normals_bishop[i].y,normals_bishop[i].z);
					glVertex3f(vertices_bishop[i].x,vertices_bishop[i].y,vertices_bishop[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//rook tower
	case 'R': 
		{
			glPushMatrix();
			
			glTranslatef(changingStartPoint.x,changingStartPoint.y,changingStartPoint.z+constants::MOVING_PAWN_OFFSET+constants::ANIMATION_Z_OFFSET );
				 //glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET);
				//move according to given parameters
				//glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, 0);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_tower.size();i++)
				{
					glNormal3f(normals_tower[i].x,normals_tower[i].y,normals_tower[i].z);
					glVertex3f(vertices_tower[i].x,vertices_tower[i].y,vertices_tower[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//queen
	case 'Q': 
		{
			glPushMatrix();
				
			
				glTranslatef(changingStartPoint.x,changingStartPoint.y,changingStartPoint.z+constants::MOVING_PAWN_OFFSET+constants::ANIMATION_Z_OFFSET );
				 //glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET);
				//move according to given parameters
				//glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, 0);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_queen.size();i++)
				{
					glNormal3f(normals_queen[i].x,normals_queen[i].y,normals_queen[i].z);
					glVertex3f(vertices_queen[i].x,vertices_queen[i].y,vertices_queen[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//king
	case 'K': 
		{
			glPushMatrix();
				//Go to (0,0)
				glTranslatef(changingStartPoint.x,changingStartPoint.y,changingStartPoint.z+constants::MOVING_PAWN_OFFSET+constants::ANIMATION_Z_OFFSET );
				// glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET);
				//move according to given parameters
				//glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, 0);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_king.size();i++)
				{
					glNormal3f(normals_king[i].x,normals_king[i].y,normals_king[i].z);
					glVertex3f(vertices_king[i].x,vertices_king[i].y,vertices_king[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
	default:
		break;

	}

}

void DrawMovingPiece(char chessPieceType,char colorOfPiece,PXCPoint3DF32 pinchPoint)
{
	
	EnableMaterialSettings(PieceMaterial::GOLD);
	
	switch (chessPieceType) 
	{
		//pawn
	case 'P': 
		{
			glPushMatrix();
			    glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET );
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_pawn.size();i++)
				{
					glNormal3f(normals_pawn[i].x,normals_pawn[i].y,normals_pawn[i].z);
					glVertex3f(vertices_pawn[i].x,vertices_pawn[i].y,vertices_pawn[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//knight horse
	case 'N': 
		{
			glPushMatrix();
				//Go to (0,0)
				 glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET);
				//move according to given parameters
				//if horse is enemy's make it look the other way around
				if(colorOfPiece=='B')
					glRotatef(-180,0.0,0.0,1.0);

				glRotatef(90,1.0,0.0,0.0);
				
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_knight.size();i++)
				{
					glNormal3f(normals_knight[i].x,normals_knight[i].y,normals_knight[i].z);
					glVertex3f(vertices_knight[i].x,vertices_knight[i].y,vertices_knight[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//bishop
	case 'B': 
		{
			glPushMatrix();
				//Go to (0,0)
				 glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET);
				//move according to given parameters
				//glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, 0);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_bishop.size();i++)
				{
					glNormal3f(normals_bishop[i].x,normals_bishop[i].y,normals_bishop[i].z);
					glVertex3f(vertices_bishop[i].x,vertices_bishop[i].y,vertices_bishop[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//rook tower
	case 'R': 
		{
			glPushMatrix();
				//Go to (0,0)
				 glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET);
				//move according to given parameters
				//glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, 0);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_tower.size();i++)
				{
					glNormal3f(normals_tower[i].x,normals_tower[i].y,normals_tower[i].z);
					glVertex3f(vertices_tower[i].x,vertices_tower[i].y,vertices_tower[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//queen
	case 'Q': 
		{
			glPushMatrix();
				//Go to (0,0)
				 glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET);
				//move according to given parameters
				//glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, 0);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_queen.size();i++)
				{
					glNormal3f(normals_queen[i].x,normals_queen[i].y,normals_queen[i].z);
					glVertex3f(vertices_queen[i].x,vertices_queen[i].y,vertices_queen[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
		//king
	case 'K': 
		{
			glPushMatrix();
				//Go to (0,0)
				 glTranslatef(pinchPoint.x,pinchPoint.y,pinchPoint.z+constants::MOVING_PAWN_OFFSET);
				//move according to given parameters
				//glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, 0);
				//rotate because horse doesn't look straight(others are symmetrical)
				glRotatef(90,1.0,0.0,0.0);
				glBegin(GL_TRIANGLES);
				for(unsigned int i=0;i<vertices_king.size();i++)
				{
					glNormal3f(normals_king[i].x,normals_king[i].y,normals_king[i].z);
					glVertex3f(vertices_king[i].x,vertices_king[i].y,vertices_king[i].z);
				}
				glEnd();
			glPopMatrix();
		}
		break;
	default:
		break;

	}
}


void DrawStaticChessPieces(CChess &gameref)
{
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(gameref.mqGameBoard.p_PieceAtSquare[i][j]!=0)
			{
				DrawPieceofColorAndTypeInPosition(gameref.mqGameBoard.p_PieceAtSquare[i][j]->GetPiece(),
					gameref.mqGameBoard.p_PieceAtSquare[i][j]->GetColor(),i,j);
			}
		}
	}

}

void DrawBoardExceptForSelectedENEMYMovingPiece(CChess &gameref)
{
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(gameref.mqGameBoard.p_PieceAtSquare[i][j]!=0)
			{
				if(i!=EnemyStartSquareCoordinates.x || j!=EnemyStartSquareCoordinates.y)
				{
					DrawPieceofColorAndTypeInPosition(gameref.mqGameBoard.p_PieceAtSquare[i][j]->GetPiece(),
					gameref.mqGameBoard.p_PieceAtSquare[i][j]->GetColor(),i,j);
				}
			}
			
		}
	}
}

void DrawBoardExceptForSelectedMovingPiece(CChess &gameref)
{
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			if(gameref.mqGameBoard.p_PieceAtSquare[i][j]!=0)
			{
				if(i!=SelectedStartSquare.x || j!=SelectedStartSquare.y)
				{
					DrawPieceofColorAndTypeInPosition(gameref.mqGameBoard.p_PieceAtSquare[i][j]->GetPiece(),
					gameref.mqGameBoard.p_PieceAtSquare[i][j]->GetColor(),i,j);
				}
			}
			
		}
	}
}




// **********************************************************************************
/*
* Summary:      Draws a chess piece specified by the vertices and normals w.r.t 
*				chessboard center on the center of a square
*               specified by x,y coordinates(0,0-7,7)
//            
* Parameters:   vector < glm::vec3 > &vertices : Mat image where the filledcircle will be drawn
*				vector < glm::vec3 > &normals : Center Point of the circle(pixels)
*				unsigned int x: x place of square in 8x8 grid
*				unsigned int y: y place of square in 8x8 grid
* Return:       Void(Draws in OpenGL)
*/
//void DrawPieceIn(std::vector < glm::vec3 > &vertices,
//	std::vector< glm::vec3 > &normals,
//	int x, int y)
//{
//	glPushMatrix();
//	//Go to (0,0)
//	glTranslatef(Offset_X_M,Offset_Y_M,0);
//	//move according to given parameters
//	glTranslatef(x * Offset_Pieces_M * 1.0, y * Offset_Pieces_M * 1.0, 0);
//	//rotate because horse doesn't look straight(others are symmetrical)
//	glRotatef(90,1.0,0.0,0.0);
//	glBegin(GL_TRIANGLES);
//	for(unsigned int i=0;i<vertices.size();i++)
//	{
//		glNormal3f(normals[i].x,normals[i].y,normals[i].z);
//		glVertex3f(vertices[i].x,vertices[i].y,vertices[i].z);
//	}
//	glEnd();
//	glPopMatrix();
//}


