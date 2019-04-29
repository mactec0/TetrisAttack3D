#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"

#include <algorithm>

namespace g3d{
	class Camera{
		float fov;
		glm::vec3 vecUP, vecRight;
		glm::vec3 look;
		glm::vec3 position, targetPos;
		glm::vec3 vecViewAngles;
		glm::mat4 viewMatrix;


		float moveAnimationPercentage, anglesAnimationPercentage;
		float moveAnimationLength, viewAnglesAnimationLength;
		glm::vec3 targetAnimationAngles, targetAnimationPosition;
		glm::vec3 positionAnimationStart, anglesAnimationStart;

		bool inAnglesAnimation, inMoveAnimation;

		void update();

		void normalizeAngles(glm::vec3 &angles);

	public:
		Camera();

		glm::mat4 getViewMatrix();
		glm::vec3 getUp();
		glm::vec3 getRight();
		glm::vec3 getLook();
		glm::vec3 getPosition();

		float getFov();
		void setFov(float fov);

		void setPosition(const glm::vec3& position);
		void rotate(const glm::vec3 &viewAngles);
		void move(const glm::vec3 &offset);

		glm::vec3 getViewAngles();
		void setViewAngles(const glm::vec3 &viewAngles);

		bool isInAnimation();

		void setMoveAnimationLength(float length);
		void setViewAnglesAnimationLength(float length);

		void movePositionAnimation(const glm::vec3 &finalPos);
		void moveAnglesAnimation(const glm::vec3 &finalAngles);
		void updateAnimation(float deltaTime);

	};

	extern Camera camera;
};
