#pragma once

#include <Vroom/Scene/Scene.h>
#include <Vroom/Render/Camera/FirstPersonCamera.h>
#include <Vroom/Asset/StaticAsset/MeshAsset.h>

#include <glm/gtc/constants.hpp>

#include <vector>

#include "imgui.h"

#include "SceneGraph/SceneGraph.h"

class MyScene : public vrm::Scene
{
public:
	MyScene();
	~MyScene() = default;
 
protected:
	void onInit() override;

	void onEnd() override;

	void onUpdate(float dt) override;

	void onRender() override;

private:
	void updateMesh();

	void onImGui();

private:
    vrm::FirstPersonCamera m_Camera;
    float forwardValue = 0.f, rightValue = 0.f, upValue = 0.f;
	float turnRightValue = 0.f, lookUpValue = 0.f;
	float myCameraSpeed = 10.f, myCameraAngularSpeed = .08f * glm::two_pi<float>() / 360.f;
    bool m_MouseLock = false;

	ImFont* m_Font = nullptr;

	bool m_ControlsEnabled = true;

	bool m_RealTimeProcessing = true;
	int m_DiscretizationParameter = 70;
	glm::vec3 m_BoxStart = glm::vec3(-10.f), m_BoxEnd = glm::vec3(10.f);
	float m_LastProcessTimeSeconds = 0.f;

	SceneGraph m_SceneGraph;

	vrm::MeshAsset m_MeshAsset;
};