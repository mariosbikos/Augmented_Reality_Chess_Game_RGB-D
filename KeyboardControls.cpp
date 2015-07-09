#include "KeyboardControls.h"

void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 27: // Escape key
        projection->Release();
		blobConfig->Release();
		blobData->Release();
		//handData->Release();
		//handConfig->Release();
		psm->Release();
		session->Release();
        exit (0);
        break;
	//Press d to disable/enable depth buffer
	case 'd' : 
		{
			if (glIsEnabled(GL_DEPTH_TEST))
				glDisable(GL_DEPTH_TEST);
			else
				glEnable(GL_DEPTH_TEST);
		}
		break;
	//Press c to enable/disable culling
	case 'c' :
		culling = !culling;
		break;
	//Press f to render cw or ccw
	case 'f' :
		ccw = !ccw;
		break;
	//Press w to render wireframe or solid
	case 'w' :
		wireframe=!wireframe;
		break;
	
	default : 
		break;
	}
	glutPostRedisplay();

}

void KeyboardChanges()
{
	if(wireframe==false)
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//In a scene of OPAQUE closed surfaces, back-facing polygons are never visible. 
	//Eliminating these invisible polygons -->speeding up the rendering of the image
	if (culling)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);
	if(ccw==true)
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);
}