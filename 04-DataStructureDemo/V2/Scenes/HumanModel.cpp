#include"HumanModel.h"

HumanModel::HumanModel()
{

}

HumanModel::~HumanModel()
{

}

void HumanModel::update(void)
{
	if (gbNamaskarAnimation)
	{
		if (gfLeftShoulderAngle >= 300.0f)
		{
			gfLeftShoulderAngle = gfLeftShoulderAngle - 1.0f;
		}

		if (gfRightShoulderAngle >= 300.0f)
		{
			gfRightShoulderAngle = gfRightShoulderAngle - 1.0f;
		}

		if (gfRightElbowAngle >= 240.0f)
		{
			gfRightElbowAngle = gfRightElbowAngle - 1.0f;
		}

		if (gfLeftElbowAngle >= 240.0f) // use this for both hand elbow
		{
			gfLeftElbowAngle = gfLeftElbowAngle - 1.0f;
		}
		else
		{
			gbStep1Done_forNamaskarAnimation = TRUE;
		}

		if (gbStep1Done_forNamaskarAnimation)
		{
			if (gfLeftWristAngle <= 45.0f)
			{
				gfLeftWristAngle = gfLeftWristAngle + 1.0f;
			}

			if (gfRightWristAngle <= 45.0f)
			{
				gfRightWristAngle = gfRightWristAngle + 1.0f;
			}

			if (gfLeftFingerAngle <= 360.0f)
			{
				gfLeftFingerAngle = 360.0f;
			}

			if (gfRightFingerAngle <= 360.0f)
			{
				gfRightFingerAngle = 360.0f;
			}

			if (gfNeckRotation_forNamaskarAnimation <= -70.0f)
			{
				gfNeckRotation_forNamaskarAnimation = gfNeckRotation_forNamaskarAnimation + 0.5f;
			}

			if (gfStep2Animation_LeftElbow >= 310.0f)
			{
				gfStep2Animation_LeftElbow = gfStep2Animation_LeftElbow - 1.0f;
			}
			else
			{
				//gbStep1Done_forNamaskarAnimation = FALSE;
			}

		}

	}
	else
	{
		if (gbStartWalkAnimation)
		{
			if (gbWalkAnimation == FALSE)
			{
				// for right hand
				if (gfRightShoulderAngle >= 250.0f)
				{
					gfRightShoulderAngle = gfRightShoulderAngle - 1.0f;
					if (gfRightElbowAngle >= 350.0f)
					{
						gfRightElbowAngle = gfRightElbowAngle - 1.0f;
					}
				}

				// for left hand
				if (gfLeftShoulderAngle <= 360.0f)
				{
					gfLeftShoulderAngle = gfLeftShoulderAngle + 1.0f;
					if (gfLeftElbowAngle <= 360.0f)
					{
						gfLeftElbowAngle = gfLeftElbowAngle + 1.0f;
					}
				}


				// for left leg
				if (gfLeftLegAngle >= 330.0f)
				{
					gfLeftLegAngle = gfLeftLegAngle - 1.0f;
					if (gfLeftKneeAngle >= 0.0f)
					{
						gfLeftKneeAngle = gfLeftKneeAngle - 1.0f;
					}
				}

				// for right leg
				if (gfRightLegAngle <= 360.0f)
				{
					gfRightLegAngle = gfRightLegAngle + 1.0f;

					if (gfRightKneeAngle <= 70.0f)
					{
						gfRightKneeAngle = gfRightKneeAngle + 1.0f;
					}
				}
				else
				{
					gbWalkAnimation = TRUE;
				}
			}
			else
			{
				// for right hand
				if (gfRightShoulderAngle <= 360.0f)
				{
					gfRightShoulderAngle = gfRightShoulderAngle + 1.0f;

					if (gfRightElbowAngle <= 360.0f)
					{
						gfRightElbowAngle = gfRightElbowAngle + 1.0f;
					}
				}

				// for left hand
				if (gfLeftShoulderAngle >= 250.0f)
				{
					gfLeftShoulderAngle = gfLeftShoulderAngle - 1.0f;
					if (gfLeftElbowAngle >= 350.0f)
					{
						gfLeftElbowAngle = gfLeftElbowAngle - 1.0f;
					}
				}

				// for left leg
				if (gfLeftLegAngle <= 360.0f)
				{
					gfLeftLegAngle = gfLeftLegAngle + 1.0f;
					if (gfLeftKneeAngle <= 70.0f)
					{
						gfLeftKneeAngle = gfLeftKneeAngle + 1.0f;
					}
				}

				// for right leg
				if (gfRightLegAngle >= 330.0f)
				{
					gfRightLegAngle = gfRightLegAngle - 1.0f;
					if (gfRightKneeAngle >= 0.0f)
					{
						gfRightKneeAngle = gfRightKneeAngle - 1.0f;
					}
				}
				else
				{
					gbWalkAnimation = FALSE;
				}
			}
		}
		
	}

}

void HumanModel::display(void)
{

}

void HumanModel::toggleWalkingAnimation(void)
{
	//code
	gfRightShoulderAngle = 340.0f;
	gfRightElbowAngle = 360.0f;
	gfRightWristAngle = 0.0f;

	gfRightFingerAngle = 360.0f;

	gfLeftShoulderAngle = 340.0f;
	gfLeftElbowAngle = 360.0f;
	gfLeftWristAngle = 0.0f;

	gfLeftFingerAngle = 0.0f;

	gfRightLegAngle = 350.0f;
	gfRightKneeAngle = 0.0f;
	gfRightAnkleAngle = 360.0f;

	gfLeftLegAngle = 350.0f;
	gfLeftKneeAngle = 0.0f;
	gfLeftAnkleAngle = 360.0f;
	gbStartWalkAnimation = !gbStartWalkAnimation;
}

void HumanModel::toggleNamaskarAnimation(void)
{
	// code
	gfRightShoulderAngle = 340.0f;
	gfRightElbowAngle = 360.0f;
	gfRightWristAngle = 0.0f;

	gfRightFingerAngle = 360.0f;

	gfLeftShoulderAngle = 340.0f;
	gfLeftElbowAngle = 360.0f;
	gfLeftWristAngle = 0.0f;

	gfLeftFingerAngle = 0.0f;

	gfRightLegAngle = 350.0f;
	gfRightKneeAngle = 0.0f;
	gfRightAnkleAngle = 360.0f;

	gfLeftLegAngle = 350.0f;
	gfLeftKneeAngle = 0.0f;
	gfLeftAnkleAngle = 360.0f;
	gbNamaskarAnimation = !gbNamaskarAnimation;
}


int HumanModel::initialize(void)
{
	//variable declaration
	BOOL bResult = FALSE;
	 
	// code
	// initialize quadric
	quadric = gluNewQuadric();

	// load texture
	bResult = loadGLTexture(&texture_sadFace, MAKEINTRESOURCE(MY_SAD_FACE_BITMAP));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of screen texture is Failed.\n");
		return(-1);
	}

	bResult = loadGLTexture(&texture_whiteCloth, MAKEINTRESOURCE(MY_WHITE_CLOTH_BITMAP));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of screen texture is Failed.\n");
		return(-2);
	}

	bResult = loadGLTexture(&texture_upperwear, MAKEINTRESOURCE(UPPER_WEAR_BITMAP));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of screen texture is Failed.\n");
		return(-3);
	}

	bResult = loadGLTexture(&texture_evilSmile, MAKEINTRESOURCE(MY_EVIL_SMILE_FACE_BITMAP));
	if (bResult == FALSE)
	{
		fprintf(gpFILE, "load of screen texture is Failed.\n");
		return(-4);
	}

	return(0);
}

void HumanModel::uninitialize(void)
{
	if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}

	if (texture_upperwear)
	{
		glDeleteTextures(1, &texture_upperwear);
		texture_upperwear = 0;
	}

	if (texture_whiteCloth)
	{
		glDeleteTextures(1, &texture_whiteCloth);
		texture_whiteCloth = 0;
	}

	if (texture_sadFace)
	{
		glDeleteTextures(1, &texture_sadFace);
		texture_sadFace = 0;
	}

	if (texture_evilSmile)
	{
		glDeleteTextures(1, &texture_evilSmile);
		texture_evilSmile = 0;
	}
}

BOOL HumanModel::isNamaskarAnimationEnable(void)
{
	return(gbNamaskarAnimation);
}

BOOL HumanModel::isHumanSwitch(void)
{
	return(gbHumanSwitch);
}

void HumanModel::toggleHumanSwitch(void)
{
	// code
	gbHumanSwitch = !gbHumanSwitch;
}

BOOL HumanModel::isWalkingAnimationEnable(void)
{
	return(gbStartWalkAnimation);
}

void HumanModel::drawBody(void)
{

	GLfloat lightAmbient[] = { 0.1f,0.1f,0.1f,1.0f };
	GLfloat lightDiffuse[] = { 0.5f,0.5f,0.5f,1.0f };
	//GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
	GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

	// light related initialization
	glLightfv(GL_LIGHT6, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, lightDiffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT6, GL_POSITION, lightPosition);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT6);

	drawTrunk(0.0f, -0.5f, 0.0f);

	glDisable(GL_LIGHT6);
	glDisable(GL_LIGHTING);

}

void HumanModel::drawFace(float xAxis, float yAxis, float zAxis)
{

	// face back side sphere
	gluQuadricTexture(quadric, GL_TRUE);

	// ambient material
	materialAmbient[0] = 0.1f; // r
	materialAmbient[1] = 0.1f; // g
	materialAmbient[2] = 0.1f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	// diffuse material
	materialDiffuse[0] = 255.0f / 255.0f; // r
	materialDiffuse[1] = 178.0f / 255.0f; // g
	materialDiffuse[2] = 106.0f / 255.0f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glColor3f(1.0f, 178.0f / 255.0f, 106.0f / 255.0f);
	// draw neck
	glPushMatrix();
	{

		glTranslatef(0.0f, -0.32f, -0.15f); // after rotate x = 'X' Axis, y = 'Z' Axis, z = 'Y' Axis

		gluSphere(quadric, 0.1f, 30, 30);
		if (gbStep1Done_forNamaskarAnimation)
		{
			glRotatef(gfNeckRotation_forNamaskarAnimation, 1.0f, 0.0f, 0.0f); // by default inside glu Library x axis is 'Z' axis and y axis is 'Y' axis
		}
		else
		{
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // by default inside glu Library x axis is 'Z' axis and y axis is 'Y' axis
		}


		glPushMatrix();
		{
			glScalef(1.2f, 1.0f, 1.0f);

			gluCylinder(quadric, 0.1f, 0.1f, 0.2f, 30, 1);

		}
		glPopMatrix();

		// face partW	
		glPushMatrix();
		{

			if (gbHumanSwitch)
			{
				glBindTexture(GL_TEXTURE_2D, texture_evilSmile);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, texture_sadFace);
			}

			// diffuse material
			materialDiffuse[0] = 255.0f / 255.0f; // r
			materialDiffuse[1] = 255.0f / 255.0f; // g
			materialDiffuse[2] = 255.0f / 255.0f; // b
			materialDiffuse[3] = 1.0f; // a
			glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

			glColor3f(1.0f, 1.0f, 1.0f);

			glTranslatef(0.0f, 0.0f, 0.45f);
			glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(12.0f, 0.0f, 0.0f, 1.0f);
			glScalef(0.7f, 0.7f, 0.8f);
			gluSphere(quadric, 0.4f, 25, 25);

			glBindTexture(GL_TEXTURE_2D, 0);
			// pheta part
			glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);
			glPushMatrix();
			{

				if (gbHumanSwitch)
				{
					// diffuse material
					materialDiffuse[0] = 200.0f / 255.0f; // r
					materialDiffuse[1] = 200.0f / 255.0f; // g
					materialDiffuse[2] = 200.0f / 255.0f; // b
					materialDiffuse[3] = 1.0f; // a
					glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

					glColor3f(243.0f / 255.0f, 211.0f / 255.0f, 82.0f / 255.0f);
				}
				else
				{
					// diffuse material
					materialDiffuse[0] = 229.0f / 255.0f; // r
					materialDiffuse[1] = 82.0f / 255.0f; // g
					materialDiffuse[2] = 22.0f / 255.0f; // b
					materialDiffuse[3] = 1.0f; // a
					glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

					glColor3f(243.0f / 255.0f, 211.0f / 255.0f, 82.0f / 255.0f);
				}


				glTranslatef(0.0f, 0.0f, 0.2f);
				glScalef(0.95f, 1.0f, 1.0f);
				/*glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);*/

				// pheta tura
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.54f, 0.2f);
					glRotatef(75.0f, 1.0f, 0.0f, 0.0f);
					gluPartialDisk(quadric, 0.0f, 0.25f, 20, 20, 0.0f, 60.0f);

					glTranslatef(0.05f, 0.01f, 0.0f);
					gluPartialDisk(quadric, 0.0f, 0.25f, 20, 20, 0.0f, 70.0f);
				}
				glPopMatrix();

				glPushMatrix();
				{
					// right side
					// pheta main base
					glTranslatef(0.02f, 0.0f, 0.0f);
					glRotatef(12.0f, 0.0f, 1.0f, 0.0f);
					gluCylinder(quadric, 0.4f, 0.55f, 0.2f, 20, 1);

					// pheta top part
					glTranslatef(0.0f, 0.0f, 0.2f);
					glScalef(1.0f, 1.0f, 0.4f);
					gluSphere(quadric, 0.55f, 20, 20);
				}
				glPopMatrix();

				// left part of pheta
				glPushMatrix();
				{
					// pheta base part
					glTranslatef(-0.06f, 0.0f, 0.0f);
					glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);
					gluCylinder(quadric, 0.4f, 0.55f, 0.2f, 10, 1);

					// pheta base sphere
					glPushMatrix();
					{
						glScalef(1.0f, 1.0f, 0.1f);
						gluSphere(quadric, 0.4f, 10, 10);
					}
					glPopMatrix();

					// pheta top sphere
					glTranslatef(0.0f, 0.0f, 0.2f);
					glScalef(1.0f, 1.0f, 0.5f);
					gluSphere(quadric, 0.55f, 10, 10);
				}
				glPopMatrix();

				//// pheta left side part
				//glPushMatrix();
				//{
				//	glTranslatef(-0.37f, -0.15f, 0.15f);

				//	glRotatef(200.0f, 1.0f, 0.0f, 0.0f);
				//	glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);
				//	glScalef(0.5f, 1.0f, 1.0f);
				//	gluCylinder(quadric, 0.1f, 0.1f, 0.5f, 15, 1);

				//	glTranslatef(0.0f, 0.0f, 0.5f);
				//	gluSphere(quadric, 0.1f, 15, 15);
				//}
				//glPopMatrix();

			}
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glPopMatrix();

	}
	glPopMatrix();





}

void HumanModel::drawTrunk(float xAxis, float yAxis, float zAxis)
{
	// code

	// skin color
	// ambient material
	materialAmbient[0] = 0.05f; // r
	materialAmbient[1] = 0.0f; // g
	materialAmbient[2] = 0.0f; // b
	materialAmbient[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);

	if (gbHumanSwitch)
	{
		// diffuse material
		materialDiffuse[0] = 255.0f / 255.0f; // r
		materialDiffuse[1] = 193.0f / 255.0f; // g
		materialDiffuse[2] = 68.0f / 255.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		glColor3f(243.0f / 255.0f, 211.0f / 255.0f, 82.0f / 255.0f);
	}
	else
	{
		// diffuse material
		materialDiffuse[0] = 255.0f / 255.0f; // r
		materialDiffuse[1] = 255.0f / 255.0f; // g
		materialDiffuse[2] = 255.0f / 255.0f; // b
		materialDiffuse[3] = 1.0f; // a
		glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

		glColor3f(1.0f, 1.0f, 1.0f);
	}



	// shininess
	materialShininess = 0.078125f * 128;
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);


	glPushMatrix();
	{
		glTranslatef(xAxis, yAxis - 1.4f, zAxis - 0.15);
		glPushMatrix();
		{
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

			glPushMatrix(); // Waist base sphere
			{
				glTranslatef(0.0f, 0.0f, 0.0f);
				glScalef(1.2f, 0.6f, 0.4f);
				gluSphere(quadric, 0.4f, 20, 20);
			}
			glPopMatrix();


			glRotatef(gfWaistAngle, 1.0f, 0.0f, 0.0f); // Waist rotation

			glPushMatrix(); // Waist base sphere trunk start
			{
				glScalef(1.2f, 0.6f, 0.4f);
				gluSphere(quadric, 0.4f, 20, 20);
			}
			glPopMatrix();

			glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);
			glPushMatrix(); // trunk 
			{
				glScalef(1.2f, 0.6f, 1.0f);
				gluCylinder(quadric, 0.4f, 0.5f, 1.4f, 25, 1);
			}
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);

			glBindTexture(GL_TEXTURE_2D, texture_upperwear);
			glPushMatrix(); // top trunk part(sholder) sphere
			{
				glTranslatef(0.0f, 0.0f, zAxis + 1.4f);
				glScalef(1.2f, 0.6f, 0.4f);
				glRotatef(-70.0f, 1.0f, 0.0f, 0.0f);
				gluSphere(quadric, 0.5f, 25, 25);
			}
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);

			// diffuse material
			materialDiffuse[0] = 255.0f / 255.0f; // r
			materialDiffuse[1] = 178.0f / 255.0f; // g
			materialDiffuse[2] = 106.0f / 255.0f; // b
			materialDiffuse[3] = 1.0f; // a
			glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

			// for face
			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, yAxis + 2.4f, zAxis + 0.15f);

				drawFace(0.0f, 0.0f, 0.0f);
			}
			glPopMatrix();
		}
		glPopMatrix();

		// for hands
		glPushMatrix();
		{
			glRotatef(gfWaistAngle, 1.0f, 0.0f, 0.0f); // waist rotation required to move upper body part accordingly

			glPushMatrix();
			{
				glTranslatef(xAxis + 0.6f, yAxis + 1.8f, zAxis);
				drawRightHand(0.0f, 0.0f, 0.0f);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(xAxis - 0.6f, yAxis + 1.8f, zAxis);
				drawLeftHand(0.0f, 0.0f, 0.0f);
			}
			glPopMatrix();
		}
		glPopMatrix();

	}
	glPopMatrix();



	drawLegs(xAxis, yAxis - 1.55f, zAxis - 0.15f);

}

void HumanModel::drawRightHand(float xAxis, float yAxis, float zAxis)
{
	// code

	// diffuse material
	materialDiffuse[0] = 255.0f / 255.0f; // r
	materialDiffuse[1] = 178.0f / 255.0f; // g
	materialDiffuse[2] = 106.0f / 255.0f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	glColor3f(1.0f, 178.0f / 255.0f, 106.0f / 255.0f);

	glPushMatrix();
	{
		glRotatef(20.0f, 1.0f, 0.0f, 0.0f); // adjustment made for walk animation 
		glPushMatrix();
		{
			glTranslatef(xAxis, yAxis, zAxis);
			gluSphere(quadric, 0.15f, 30, 30); // sholder start sphere

			glRotatef(gfRightShoulderAngle, 1.0f, 0.0f, 0.0f); // sholder angle handle on 'R'/'r' key press

			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.15f, 0.1f, 0.7f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -0.7f, 0.0f);
			gluSphere(quadric, 0.1f, 30, 30); // elbow start sphere
			if (gbNamaskarAnimation)
			{
				glRotatef(gfRightElbowAngle, 1.0f, 0.0f, 0.0f); // elbow angle handle on 'T'/'t' key press
				if (gbStep1Done_forNamaskarAnimation)
				{
					// same variable use for right hand animation
					glRotatef(gfStep2Animation_LeftElbow, 0.0f, 0.0f, 1.0f); // elbow angle handle on 'T'/'t' key press
				}
			}
			else
			{
				glRotatef(gfRightElbowAngle, 1.0f, 0.0f, 0.0f); // elbow angle handle on 'T'/'t' key press
			}


			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.1f, 0.07f, 0.7f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -0.7f, 0.0f); // wrist start here
			if (gbStep1Done_forNamaskarAnimation)
			{
				glRotatef(gfRightWristAngle, 0.0f, 0.0f, 1.0f);
			}

			glPushMatrix(); // palm start here
			{

				gluSphere(quadric, 0.07f, 15, 15); // wrist sphere

				glScalef(0.5f, 0.9f, 1.0f);
				glTranslatef(-0.01f, -0.1f, 0.01f);
				gluSphere(quadric, 0.1f, 15, 15); // palm sphere

			}
			glPopMatrix();

			glTranslatef(0.0f, -0.04f, 0.0f);
			glPushMatrix(); // finger start here
			{
				// first finger 
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.12f, 0.07f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 1 start sphere base part
					glRotatef(gfRightFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 1 start sphere middle part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.055f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// second finger
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.12f, 0.03f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 2 start sphere base part
					glRotatef(gfRightFingerAngle - 0.0, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.06f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.06f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 2 start sphere middle part
					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.06f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.06f, 0.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Third finger
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.12f, -0.01f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 3 start sphere base part
					glRotatef(gfRightFingerAngle - 0.0, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 3 start sphere middle part
					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.055f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Forth finger
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.12f, -0.05f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 4 start sphere base part
					glRotatef(gfRightFingerAngle - 0.0, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 4 start sphere middle part
					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.04f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.04f, 0.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Thumb
				glPushMatrix();
				{
					glTranslatef(0.01f, -0.06f, 0.09f);
					glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
					gluSphere(quadric, 0.03f, 15, 15); // Thumb start sphere base part
					glRotatef(gfRightFingerAngle - 5.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.03f, 0.024f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					gluSphere(quadric, 0.024f, 15, 15); // Thumb start sphere middle part
					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 12.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.024f, 0.018f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.018f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfRightFingerAngle - 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfRightFingerAngle - 16.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.018f, 0.014f, 0.04f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.04f, 0.0f);
					gluSphere(quadric, 0.014f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void HumanModel::drawLeftHand(float xAxis, float yAxis, float zAxis)
{
	// code

	// diffuse material
	materialDiffuse[0] = 255.0f / 255.0f; // r
	materialDiffuse[1] = 178.0f / 255.0f; // g
	materialDiffuse[2] = 106.0f / 255.0f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	glColor3f(1.0f, 178.0f / 255.0f, 106.0f / 255.0f);

	glPushMatrix();
	{
		glRotatef(20.0f, 1.0f, 0.0f, 0.0f); // adjustment made for walk animation 
		glPushMatrix();
		{
			glTranslatef(xAxis, yAxis, zAxis);
			gluSphere(quadric, 0.15f, 30, 30); // sholder start sphere

			glRotatef(gfLeftShoulderAngle, 1.0f, 0.0f, 0.0f); // sholder angle handle on 'R'/'r' key press

			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.15f, 0.1f, 0.7f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -0.7f, 0.0f);
			gluSphere(quadric, 0.1f, 30, 30); // elbow start sphere
			if (gbNamaskarAnimation)
			{

				glRotatef(gfLeftElbowAngle, 1.0f, 0.0f, 0.0f); // elbow angle handle on 'T'/'t' key press
				if (gbStep1Done_forNamaskarAnimation)
				{
					glRotatef(-gfStep2Animation_LeftElbow, 0.0f, 0.0f, 1.0f); // elbow angle handle on 'T'/'t' key press
				}

			}
			else
			{
				glRotatef(gfLeftElbowAngle, 1.0f, 0.0f, 0.0f); // elbow angle handle on 'T'/'t' key press
			}


			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.1f, 0.07f, 0.7f, 30, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -0.7f, 0.0f); // wrist start here
			if (gbStep1Done_forNamaskarAnimation)
			{
				glRotatef(-gfLeftWristAngle, 0.0f, 0.0f, 1.0f);
			}
			glPushMatrix(); // palm start here
			{

				gluSphere(quadric, 0.07f, 15, 15); // wrist sphere

				glScalef(0.5f, 0.9f, 1.0f);
				glTranslatef(0.01f, -0.1f, 0.01f);
				gluSphere(quadric, 0.1f, 15, 15); // palm sphere

			}
			glPopMatrix();

			glTranslatef(0.0f, -0.05f, 0.0f);
			glPushMatrix(); // finger start here
			{
				// first finger 
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.12f, 0.07f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 1 start sphere base part
					glRotatef(gfLeftFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 1 start sphere middle part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.055f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// second finger
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.12f, 0.03f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 2 start sphere base part
					glRotatef(gfLeftFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.06f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.06f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 2 start sphere middle part
					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.06f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.06f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Third finger
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.12f, -0.01f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 3 start sphere base part
					glRotatef(gfLeftFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 3 start sphere middle part
					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.055f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.055f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Forth finger
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.12f, -0.05f);
					gluSphere(quadric, 0.02f, 15, 15); // finger 4 start sphere base part
					glRotatef(gfLeftFingerAngle, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.02f, 0.015f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					gluSphere(quadric, 0.015f, 15, 15); // finger 4 start sphere middle part
					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 9.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.015f, 0.012f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.012f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 18.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.012f, 0.01f, 0.04f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.04f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.01f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();

				// Thumb
				glPushMatrix();
				{
					glTranslatef(0.0f, -0.06f, 0.09f);
					glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
					gluSphere(quadric, 0.03f, 15, 15); // Thumb start sphere base part
					glRotatef(gfLeftFingerAngle + 5.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press

					glPushMatrix();
					//glColor3f(1.0f, 0.5f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.03f, 0.024f, 0.05f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.05f, 0.0f);
					//glRotatef(30.0f, 1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.024f, 15, 15); // Thumb start sphere middle part
					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 12.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}


					glPushMatrix();
					//glColor3f(1.0f, 0.0f, 1.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.024f, 0.018f, 0.055f, 15, 1);
					glPopMatrix();

					glTranslatef(0.0f, -0.055f, 0.0f);
					gluSphere(quadric, 0.018f, 15, 15); // finger 1 start sphere tip/end part

					if (gbStep1Done_forNamaskarAnimation)
					{
						glRotatef(gfLeftFingerAngle + 5.5f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}
					else
					{
						glRotatef(gfLeftFingerAngle + 16.0f, 0.0f, 0.0f, 1.0f); // finger angle handle on 'Y'/'y' key press
					}

					glPushMatrix();
					//glColor3f(1.0f, 1.0f, 0.0f);
					glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
					gluCylinder(quadric, 0.018f, 0.014f, 0.04f, 15, 1);
					glPopMatrix();

					glPushMatrix();
					glTranslatef(0.0f, -0.04f, 0.0f);
					//glColor3f(1.0f, 0.0f, 1.0f);
					gluSphere(quadric, 0.014f, 15, 15);
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void HumanModel::drawLegs(float xAxis, float yAxis, float zAxis)
{

	// code
		// diffuse material
	materialDiffuse[0] = 255.0f / 255.0f; // r
	materialDiffuse[1] = 255.0f / 255.0f; // g
	materialDiffuse[2] = 255.0f / 255.0f; // b
	materialDiffuse[3] = 1.0f; // a

	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	glColor3f(1.0f, 1.0f, 1.0f);

	// waist cloth part
	glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);
	glPushMatrix();
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glTranslatef(xAxis, yAxis + 0.1f, zAxis);
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.1f, 0.0f);

			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glScalef(1.2f, 0.6f, 1.0f);

			gluCylinder(quadric, 0.39f, 0.45f, 0.05f, 20, 1);

			glTranslatef(0.0f, 0.0f, 0.05f);
			gluCylinder(quadric, 0.45f, 0.5f, 0.35f, 10, 1);

		}
		glPopMatrix();

	}
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

	glPushMatrix();
	{
		glTranslatef(xAxis + 0.225f, yAxis, zAxis);
		drawRightLeg(0.0f, 0.0f, 0.0f);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(xAxis - 0.225f, yAxis, zAxis);
		drawLeftLeg(0.0f, 0.0f, 0.0f);
	}
	glPopMatrix();
}

void HumanModel::drawRightLeg(float xAxis, float yAxis, float zAxis)
{
	// code
	// diffuse material
	materialDiffuse[0] = 255.0f / 255.0f; // r
	materialDiffuse[1] = 255.0f / 255.0f; // g
	materialDiffuse[2] = 255.0f / 255.0f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	glColor3f(1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);

	glPushMatrix();
	{
		glRotatef(10.0f, 1.0f, 0.0f, 0.0f);// adjustment made for walk animation 
		glPushMatrix();
		{
			glTranslatef(xAxis, yAxis - 0.1f, zAxis);
			gluSphere(quadric, 0.22f, 30, 30); // hip start sphere

			glRotatef(gfRightLegAngle, 1.0f, 0.0f, 0.0f); // leg angle handle on 'P'/'p' key press

			glPushMatrix();
			{ // thigh part
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glScalef(0.68f, 0.68f, 1.0f);
				gluCylinder(quadric, 0.4f, 0.4f, 1.1f, 5, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -1.1f, 0.0f);

			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glScalef(0.68f, 0.68f, 1.0f);
				//gluSphere(quadric, 0.4f, 25, 25); // knee start sphere		
				gluCylinder(quadric, 0.39f, 0.38f, 0.1f, 5, 1); // shin part
			}
			glPopMatrix();

			glRotatef(gfRightKneeAngle, 1.0f, 0.0f, 0.0f); // knee angle handle on 'O'/'o' key press

			glPushMatrix();
			{
				//glColor3f(1.0f, 0.0f, 0.0f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glPushMatrix();
				{
					// cloth
					glScalef(0.68f, 0.68f, 1.0f);
					gluCylinder(quadric, 0.4f, 0.3f, 0.7f, 5, 1); // shin part
				}
				glPopMatrix();

				glBindTexture(GL_TEXTURE_2D, 0);

				// body part 
				// diffuse material
				materialDiffuse[0] = 255.0f / 255.0f; // r
				materialDiffuse[1] = 178.0f / 255.0f; // g
				materialDiffuse[2] = 106.0f / 255.0f; // b
				materialDiffuse[3] = 1.0f; // a
				glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

				glColor3f(1.0f, 178.0f / 255.0f, 106.0f / 255.0f);

				gluCylinder(quadric, 0.15f, 0.12f, 1.0f, 30, 1); // shin part
			}
			glPopMatrix();

			glPushMatrix();// ankle start here
			{
				glTranslatef(0.0f, -1.0f, 0.0f);
				glPushMatrix();
				{
					glScalef(1.0f, 0.6f, 1.0f);
					gluSphere(quadric, 0.12f, 15, 15); // ankle start sphere
				}
				glPopMatrix();

				glRotatef(gfRightAnkleAngle, 1.0f, 0.0f, 0.0f); // not handle yet use if required implemented relative changes in wndproc()

				glPushMatrix(); // ankle start here
				{
					glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
					glScalef(1.0f, 0.6f, 1.0f);
					gluCylinder(quadric, 0.12f, 0.1f, 0.3f, 10, 1); // foot


				}
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, 0.3f);// foot ends(fingers start sphere)

				glPushMatrix();
				{
					glPushMatrix();
					{
						glScalef(1.0f, 0.6f, 1.0f);
						gluSphere(quadric, 0.1f, 10, 10); // (fingers start sphere)
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(0.0f, -0.015f, 0.0f);
						glPushMatrix(); // foot thumb
						{
							glTranslatef(-0.0625f, 0.0f, 0.06f);
							gluCylinder(quadric, 0.02f, 0.022f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.022f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot index finger
						{
							glTranslatef(-0.025f, 0.0f, 0.09f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot middle finger
						{
							glTranslatef(0.005f, 0.0f, 0.085f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot forth finger
						{
							glTranslatef(0.035f, 0.0f, 0.075f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.07f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.07f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot little finger
						{
							glTranslatef(0.065f, 0.0f, 0.06f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.06f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.06f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();
					}
					glPopMatrix();

				}
				glPopMatrix();
			}
			glPopMatrix();


		}
		glPopMatrix();
	}
	glPopMatrix();
}

void HumanModel::drawLeftLeg(float xAxis, float yAxis, float zAxis)
{
	// code
	// diffuse material
	materialDiffuse[0] = 255.0f / 255.0f; // r
	materialDiffuse[1] = 255.0f / 255.0f; // g
	materialDiffuse[2] = 255.0f / 255.0f; // b
	materialDiffuse[3] = 1.0f; // a
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, texture_whiteCloth);

		glRotatef(10.0f, 1.0f, 0.0f, 0.0f);// adjustment made for walk animation 
		glPushMatrix();
		{
			glTranslatef(xAxis, yAxis - 0.1f, zAxis);
			gluSphere(quadric, 0.22f, 30, 30); // hip start sphere

			glRotatef(gfLeftLegAngle, 1.0f, 0.0f, 0.0f); // leg angle handle on 'P'/'p' key press

			glPushMatrix();
			{ // thigh part
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glScalef(0.68f, 0.68f, 1.0f);
				gluCylinder(quadric, 0.4f, 0.4f, 1.1f, 5, 1);
			}
			glPopMatrix();

			glTranslatef(0.0f, -1.1f, 0.0f);

			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glScalef(0.68f, 0.68f, 1.0f);
				//gluSphere(quadric, 0.4f, 25, 25); // knee start sphere		
				gluCylinder(quadric, 0.39f, 0.38f, 0.1f, 5, 1); // shin part
			}
			glPopMatrix();

			glRotatef(gfLeftKneeAngle, 1.0f, 0.0f, 0.0f); // knee angle handle on 'O'/'o' key press

			glPushMatrix();
			{
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glPushMatrix();
				{
					// cloth
					glScalef(0.68f, 0.68f, 1.0f);
					gluCylinder(quadric, 0.4f, 0.3f, 0.7f, 5, 1); // shin part
				}
				glPopMatrix();

				glBindTexture(GL_TEXTURE_2D, 0);

				// body part 
				// diffuse material
				materialDiffuse[0] = 255.0f / 255.0f; // r
				materialDiffuse[1] = 178.0f / 255.0f; // g
				materialDiffuse[2] = 106.0f / 255.0f; // b
				materialDiffuse[3] = 1.0f; // a
				glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);

				glColor3f(1.0f, 178.0f / 255.0f, 106.0f / 255.0f);

				gluCylinder(quadric, 0.15f, 0.12f, 1.0f, 30, 1); // shin part
			}
			glPopMatrix();

			glPushMatrix();// ankle start here
			{
				glTranslatef(0.0f, -1.0f, 0.0f);
				glPushMatrix();
				{
					glScalef(1.0f, 0.6f, 1.0f);
					gluSphere(quadric, 0.12f, 15, 15); // ankle start sphere
				}
				glPopMatrix();

				glRotatef(gfLeftAnkleAngle, 1.0f, 0.0f, 0.0f); // not handle yet use if required implemented relative changes in wndproc()

				glPushMatrix(); // ankle start here
				{
					glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
					glScalef(1.0f, 0.6f, 1.0f);
					gluCylinder(quadric, 0.12f, 0.1f, 0.3f, 10, 1); // foot


				}
				glPopMatrix();

				glTranslatef(0.0f, 0.0f, 0.3f);// foot ends(fingers start sphere)

				glPushMatrix();
				{
					glPushMatrix();
					{
						glScalef(1.0f, 0.6f, 1.0f);
						gluSphere(quadric, 0.1f, 10, 10); // (fingers start sphere)
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(0.0f, -0.015f, 0.0f);
						glPushMatrix(); // foot thumb
						{
							glTranslatef(0.0625f, 0.0f, 0.06f);
							gluCylinder(quadric, 0.02f, 0.022f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.022f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot index finger
						{
							glTranslatef(0.025f, 0.0f, 0.09f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot middle finger
						{
							glTranslatef(-0.005f, 0.0f, 0.085f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.08f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.08f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot forth finger
						{
							glTranslatef(-0.035f, 0.0f, 0.075f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.07f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.07f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();

						glPushMatrix(); // foot little finger
						{
							glTranslatef(-0.065f, 0.0f, 0.06f);
							gluCylinder(quadric, 0.015f, 0.012f, 0.06f, 10, 1); // finger 

							glTranslatef(0.0f, 0.0f, 0.06f);
							gluSphere(quadric, 0.012f, 10, 10); // finger end sphere

						}
						glPopMatrix();
					}
					glPopMatrix();

				}
				glPopMatrix();
			}
			glPopMatrix();


		}
		glPopMatrix();
	}
	glPopMatrix();
}
