#include "SetupGL.h"


void Setup()
{
	
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowPosition( 0, 0);
	glutInitWindowSize(constants::COLOR_WIDTH,constants::COLOR_HEIGHT);

	glutCreateWindow( "AR_CHESS" );
	//clear buffers
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClearDepth( 1.0 );
	//To make sure normal vectors are normalized
	glEnable(GL_NORMALIZE);
	//Parameter handling
	glShadeModel (GL_SMOOTH);
	//glShadeModel (GL_FLAT);

	//------------------------DEPTH BUFFER SETTINGS----------------------------------//
	//FOR HAND OCCLUSION I MAY HAVE TO DISABLE THIS
	////Enable the depth buffer
	glEnable(GL_DEPTH_TEST);
	//renders a fragment if its z value is less or equal(closer to the user)
	//of the stored value
	glDepthFunc(GL_LEQUAL);
	//Initialize values in the depth buffer.This allows any primitive to appear at first
	glClearDepth(1);
	//-------------------------------------------------------------------------------//
	
	////--------------------------------LIGHTING SETTINGS------------------------------//
	//Set up light source parameters
	//setup light position
	GLfloat light_position[] = { 30.0, 20.0, 50.0, 1.0 };
	//setup light color for ambient--diffuse--specular
	GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1, 1, 0.1 };	//Ls of light 1
	//Now pass the light parameters to the system
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//--------------------------------------------------------------------------//
	//smooth points
	//glEnable(GL_POLYGON_SMOOTH);

	//---------------------TRANSPARENCY SETTINGS--------------------------------//
	////Enable the OpenGL Blending functionality  
	glEnable(GL_BLEND);
    // Set the blend mode to blend our current RGBA with what is already in the buffer  
					//incoming //  // stored //
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//--------------------------------------------------------------------------//
	glViewport(0, 0, (GLsizei)640, (GLsizei)480);
	
	//Load Objects from file
	//Read 3D Models from path file
	bool res = loadOBJ("3D_Models/blender_min_center2.obj", 
		vertices_king,   uvs_king,   normals_king,
		vertices_queen,  uvs_queen,  normals_queen,
		vertices_pawn,   uvs_pawn,   normals_pawn,
		vertices_tower,  uvs_tower,  normals_tower,
		vertices_knight, uvs_knight, normals_knight,
		vertices_bishop, uvs_bishop, normals_bishop,
		vertices_board,  uvs_board,  normals_board
		);

	if(res==false )
	{
		cout<<"Can't load 3d model- file"<<endl;
		system("pause");
		exit(1);
	}
}


