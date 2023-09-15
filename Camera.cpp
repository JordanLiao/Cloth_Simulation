////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"
#include "glm/gtx/euler_angles.hpp"

////////////////////////////////////////////////////////////////////////////////

Camera::Camera() {
	Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Update() {
	// Compute camera world matrix
	glm::mat4 world(1.0f);
	world[3][2]=Distance;
	world=glm::eulerAngleY(glm::radians(-Azimuth)) * glm::eulerAngleX(glm::radians(-Incline)) * world;

	// Compute view matrix (inverse of world matrix)
	glm::mat4 view=glm::inverse(world);

	// Compute perspective projection matrix
	glm::mat4 project=glm::perspective(glm::radians(FOV),Aspect,NearClip,FarClip);

	// Compute final view-projection matrix
	ViewProjectMtx=project * view;
}

////////////////////////////////////////////////////////////////////////////////

void Camera::Reset() {
	FOV=45.0f;
	Aspect=1.33f;
	NearClip=0.1f;
	FarClip=100.0f;

	Distance=10.0f;
	Azimuth=0.0f;
	Incline=20.0f;
}

// Access functions
void Camera::SetAspect(float a) { Aspect = a; }
void Camera::SetDistance(float d) { Distance = d; }
void Camera::SetAzimuth(float a) { Azimuth = a; }
void Camera::SetIncline(float i) { Incline = i; }

float Camera::GetDistance() { return Distance; }
float Camera::GetAzimuth() { return Azimuth; }
float Camera::GetIncline() { return Incline; }

const glm::mat4& Camera::GetViewProjectMtx() { return ViewProjectMtx; }

////////////////////////////////////////////////////////////////////////////////
