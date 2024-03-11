#pragma once
#include"./Common.h"

class HumanModel
{
	public:
	HumanModel();

	~HumanModel();

	void drawBody(void);

	void drawTrunk(float xAxis, float yAxis, float zAxis);

	// 1st param x coordinate of top of right hand
	// 2nd param y coordinate of top of right hand
	// 3rd param z coordinate of top of right hand
	void drawRightHand(float, float, float);

	// 1st param x coordinate of top of left hand
	// 2nd param y coordinate of top of left hand
	// 3rd param z coordinate of top of left hand
	void drawLeftHand(float, float, float);

	// 1st param x coordinate of top of legs 
	// 2nd param y coordinate of top of legs 
	// 3rd param z coordinate of top of legs 
	void drawLegs(float, float, float);

	// 1st param x coordinate of top of right legs 
	// 2nd param y coordinate of top of right legs 
	// 3rd param z coordinate of top of right legs 
	void drawRightLeg(float, float, float);

	// 1st param x coordinate of top of left legs 
	// 2nd param y coordinate of top of left legs 
	// 3rd param z coordinate of top of left legs
	void drawLeftLeg(float, float, float);


	// 1st param x coordinate of center of nose
	// 2nd param y coordinate of center of nose
	// 3rd param z coordinate of center of nose
	void drawFace(float, float, float);

	void update(void);

	void display(void);

	int initialize(void);

	void uninitialize(void);
	
	BOOL isNamaskarAnimationEnable(void);

	BOOL isWalkingAnimationEnable(void);

	BOOL isHumanSwitch(void);

	void toggleWalkingAnimation(void);

	void toggleNamaskarAnimation(void);

	void toggleHumanSwitch(void);

	private:
	GLfloat gfRightShoulderAngle = 340.0f;
	GLfloat gfRightElbowAngle = 360.0f;
	GLfloat gfRightWristAngle = 0.0f;

	GLfloat gfRightFingerAngle = 360.0f;

	GLfloat gfLeftShoulderAngle = 340.0f;
	GLfloat gfLeftElbowAngle = 360.0f;
	GLfloat gfLeftWristAngle = 0.0f;

	GLfloat gfLeftFingerAngle = 0.0f;

	GLfloat gfRightLegAngle = 350.0f;
	GLfloat gfRightKneeAngle = 0.0f;
	GLfloat gfRightAnkleAngle = 360.0f;

	GLfloat gfLeftLegAngle = 350.0f;
	GLfloat gfLeftKneeAngle = 0.0f;
	GLfloat gfLeftAnkleAngle = 360.0f;

	GLfloat gfWaistAngle = 0.0f;

	BOOL gbWalkAnimation = FALSE;
	BOOL gbStartWalkAnimation = FALSE;
	BOOL gbNamaskarAnimation = FALSE;
	BOOL gbStep1Done_forNamaskarAnimation = FALSE;

	GLfloat gfStep2Animation_LeftElbow = 360.0f;
	GLfloat gfNeckRotation_forNamaskarAnimation = -90.0f;

	BOOL gbHumanSwitch = FALSE;

	GLuint texture_sadFace = 0;
	GLuint texture_whiteCloth = 0;
	GLuint texture_upperwear = 0;
	GLuint texture_evilSmile = 0;

	GLUquadric *quadric;
};