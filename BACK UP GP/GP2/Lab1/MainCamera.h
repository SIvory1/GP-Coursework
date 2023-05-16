#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct MainCamera
{
public:

	float fov = 70;

	// reads in values for camera 
	void InitializeCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		this->pos = pos;
		// forward vector
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		// up vector
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		// projection matrix
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);	 
	}

	// combines view and projection matrix 
	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	// returns just view matrix 
	inline glm::mat4 GetView() const
	{
		return glm::lookAt(pos, pos + forward, up);
	}

	// returns just projeciton view 
	inline glm::mat4 GetProjection() const
	{
		return projection;
	}

	// returns pos of camera
	inline glm::vec3 getPosition() const
	{
		return pos;
	}
	
	// dont want to use inline, as it is called loads of times and that woudfl cause big memeoray footprint???
	// reads in new values to adjust camera fov 
	void ChangeFOV(float fov, float aspect, float nearClip, float farClip)
	{
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	}

	// used to adjust fov value
	void Zoom(int zoomForce)
	{
		fov += zoomForce;
	}
	
	// used to move camera forward
    void MoveForward(float cameraSPD)
    {
		// mutiples the pos by a forward vector and spd variables
	     pos += forward * cameraSPD;
    }

	// used to move camera horizontally
	void MoveHoriz(float cameraSPD)
	{
		// uses cross product of up and forward vector
		pos += glm::cross(up, forward) * -cameraSPD;
	}

	// used to move camera vertically
	void MoveVertical(float cameraSPD)
	{
		// mutiples the pos by an up vector and spd variables
		pos += up * cameraSPD;
	}
	
	// used to change where the camera is looking on the y 
	void RotateY(float angle)
    {
		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
		up = glm::normalize(glm::cross(forward, right));
	}

	// used to change where the camera is looking on the x
	void RotateX(float angle)
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
		up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
	}
private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
};


