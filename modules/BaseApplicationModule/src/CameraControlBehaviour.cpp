#include "CameraControlBehaviour.h"

#include "Application.h"
#include "Timing.h"
#include "Transform.h"

void CameraControlBehaviour::OnLoad(entt::handle entity) {
	_initial = entity.get<Transform>().GetLocalRotationQuat();
}

void CameraControlBehaviour::Update(entt::handle entity)
{
	float dt = Timing::Instance().DeltaTime;
	GLFWwindow* window = Application::Instance().Window;
	std::shared_ptr<GameScene> curScene = Application::Instance().ActiveScene;
	double mx, my;
	glfwGetCursorPos(window, &mx, &my);
	Transform& transform = entity.get<Transform>();


	if (true) {
		if (!_isPressed) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			_isPressed = true;
			_prevMouseX = mx;
			_prevMouseY = my;
		}


		

		_rotationX += static_cast<float>(mx - _prevMouseX) * 0.5f;
		_rotationY += static_cast<float>(my - _prevMouseY) * 0.3f;
		glm::quat rotX = glm::angleAxis(glm::radians(-_rotationX), glm::vec3(0, 0, 1));
		glm::quat rotY = glm::angleAxis(glm::radians(-_rotationY), glm::vec3(1, 0, 0));
		transform.SetLocalRotation(rotX * rotY);
		
		_prevMouseX = mx;
		_prevMouseY = my;




	
		

	}


}
