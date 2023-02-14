#include "Camera.h"

Camera::Camera(int width,int height)
{
	
	mPosition = { 2.0f,0.0f,-2.0f };
	mUp = { 0.0f,1.0f,0.0f };
	mLookAt = { 0.0f,0.0f,0.0f };
	
	mNear = 1.0f;
	mFar = 1000.0f;
	mAngle = XMConvertToRadians(100.0f);
	mRatio = static_cast<float>(width) / height;
}

void Camera::Resize(int width, int height)
{
	mRatio = static_cast<float>(width) / height;
}