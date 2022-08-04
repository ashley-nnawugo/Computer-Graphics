#pragma once
#include <glm/glm/glm.hpp>
#include "camera.h"

void MoveCamera(SCamera& in, SCamera::Camera_Movement direction)
{

	float speed = 0.1f;

	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
	in.Up = glm::normalize(glm::cross(in.Right, in.Front));
	
	if(direction == SCamera::FORWARD)
		in.Position = in.Position + (in.Front * speed);
	if(direction == SCamera::BACKWARD)
		in.Position = in.Position - (in.Front * speed);
	if(direction == SCamera::RIGHT)
		in.Position = in.Position + (in.Right * speed);
	if(direction == SCamera::LEFT)
		in.Position = in.Position - (in.Right * speed);
	
}

void OrientCamera(SCamera& in, float xoffset, float yoffset)
{

	in.Yaw += xoffset;
	in.Pitch -= yoffset;

	if (in.Pitch > 89.0f)
	{
		in.Pitch = 89.0f;
	}
	if (in.Pitch < -89.0f)
	{
		in.Pitch = -89.0f;
	}
	float theta = in.Yaw * (3.14159265358979323846f / 180.f);
	float alpha = in.Pitch * (3.14159265358979323846f / 180.f);

	in.Front = glm::normalize(glm::vec3{ cos(theta) * cos(alpha), sin(alpha), sin(theta) * cos(alpha) });
	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
	in.Up = glm::normalize(glm::cross(in.Right, in.Front));
}