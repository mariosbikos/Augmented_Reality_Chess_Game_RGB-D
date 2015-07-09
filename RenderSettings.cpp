#include "RenderSettings.h"

void EnableMaterialSettings(PieceMaterial mat)
{
	GLfloat specref[4];
	glDisable(GL_COLOR_MATERIAL);

	 switch (mat) 
	 {
		 case PieceMaterial::BLACK_PLASTIC: 
			specref[0] = 0.0f; specref[1] =0.0f; specref[2] = 0.00f; specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_AMBIENT,specref);
			specref[0] = 0.01f; specref[1] = 0.01f; specref[2] = 0.01f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_DIFFUSE,specref);
			specref[0] = 0.5f; specref[1] = 0.5f; specref[2] =0.5f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_SPECULAR,specref);
			glMaterialf(GL_FRONT,GL_SHININESS,128*0.25);
			break;
		case PieceMaterial::RED: 
			specref[0] = 0.0f; specref[1] =0.0f; specref[2] = 0.00f; specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_AMBIENT,specref);
			specref[0] = 0.5f; specref[1] = 0.0f; specref[2] = 0.0f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_DIFFUSE,specref);
			specref[0] = 0.7f; specref[1] = 0.6f; specref[2] =0.6f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_SPECULAR,specref);
			glMaterialf(GL_FRONT,GL_SHININESS,128*0.25);
			break;
		 case PieceMaterial::WHITE:
			 //RUBY
			specref[0] = 0.0f; specref[1] =0.0f; specref[2] = 0.0f; 
			specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_AMBIENT,specref);
			specref[0] = 0.55f; specref[1] = 0.55f; specref[2] = 0.55f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_DIFFUSE,specref);
			specref[0] = 0.70f; specref[1] = 0.7f; specref[2] = 0.7f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_SPECULAR,specref);
			glMaterialf(GL_FRONT,GL_SHININESS,128*.25);
			break;
		 case PieceMaterial::RUBY:
			 //RUBY
			specref[0] = 0.1745f; specref[1] =0.01175f; specref[2] = 0.01175f; 
			specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_AMBIENT,specref);
			specref[0] = 0.61424f; specref[1] = 0.04136f; specref[2] = 0.04136f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_DIFFUSE,specref);
			specref[0] = 0.727811f; specref[1] = 0.626959f; specref[2] = 0.626959f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_SPECULAR,specref);
			glMaterialf(GL_FRONT,GL_SHININESS,128*.078);
			break;
		 case PieceMaterial::GOLD:
			 specref[0] = 0.24725f; specref[1] =0.1995f; specref[2] = 0.1995f; specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_AMBIENT,specref);
			specref[0] =0.75164f; specref[1] = 0.60648f; specref[2] = 0.22648f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_DIFFUSE,specref);
			specref[0] = 0.628281f; specref[1] = 0.555802f; specref[2] = 0.366065f; 
			//specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_SPECULAR,specref);
			glMaterialf(GL_FRONT,GL_SHININESS,128*0.4);
		    break;
		 default:
			 //default is black plastic
			specref[0] = 0.0f; specref[1] =0.0f; specref[2] = 0.0f; specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_AMBIENT,specref);
			specref[0] = 0.01f; specref[1] = 0.01f; specref[2] = 0.01f; specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_DIFFUSE,specref);
			specref[0] = 0.5f; specref[1] = 0.5f; specref[2] = 0.5f; specref[3] = 1.0;
			glMaterialfv(GL_FRONT,GL_SPECULAR,specref);
			glMaterialf(GL_FRONT,GL_SHININESS,128*0.25);
			break;
	}
	
}

void EnableRenderingBasedOnColor()
{
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable(GL_COLOR_MATERIAL);
}