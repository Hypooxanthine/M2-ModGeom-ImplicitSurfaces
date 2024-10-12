#include "MyScene.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Core/Window.h>

#include <Vroom/Scene/Components/MeshComponent.h>
#include <Vroom/Scene/Components/TransformComponent.h>
#include <Vroom/Scene/Components/PointLightComponent.h>

#include <Vroom/Asset/AssetManager.h>

#include <glm/gtx/string_cast.hpp>

#include "imgui.h"

#include "Implicits/Primitives/Sphere.h"
#include "Implicits/Operators/TransformOperator.h"
#include "Implicits/Operators/UnionOperator.h"
#include "Implicits/Operators/IntersectionOperator.h"
#include "Implicits/Operators/BlendOperator.h"
#include "Implicits/Operators/DifferenceOperator.h"

#include "SceneGraph/SceneGraph.h"

MyScene::MyScene()
    : vrm::Scene(), m_Camera(0.1f, 100.f, glm::radians(90.f), 600.f / 400.f, { 0.5f, 10.f, 20.f }, { glm::radians(45.f), 0.f, 0.f })
{
    auto& gameLayer = vrm::Application::Get().getGameLayer();

    // Bind triggers to the camera
    // This is a bit ugly. I might create some facilities that do this job in the future.
    // Maybe another event type, which will give a scalar depending on the input (moveForward in [-1, 1] for example, controlled with any input we want).
    gameLayer.getTrigger("MoveForward")
        .bindCallback([this](bool triggered) { forwardValue += triggered ? 1.f : -1.f; });
    gameLayer.getTrigger("MoveBackward")
        .bindCallback([this](bool triggered) { forwardValue -= triggered ? 1.f : -1.f; });
    gameLayer.getTrigger("MoveRight")
        .bindCallback([this](bool triggered) { rightValue += triggered ? 1.f : -1.f; });
    gameLayer.getTrigger("MoveLeft")
        .bindCallback([this](bool triggered) { rightValue -= triggered ? 1.f : -1.f; });
    gameLayer.getTrigger("MoveUp")
        .bindCallback([this](bool triggered) { upValue += triggered ? 1.f : -1.f; });
    gameLayer.getTrigger("MoveDown")
        .bindCallback([this](bool triggered) { upValue -= triggered ? 1.f : -1.f; });
    
    gameLayer.getTrigger("MouseLeft")
        .bindCallback([this](bool triggered) {
            if (!m_ControlsEnabled)
                return;
            m_MouseLock = triggered;
            vrm::Application::Get().getWindow().setCursorVisible(!triggered);
        });
    
    gameLayer.getCustomEvent("MouseMoved")
        .bindCallback([this](const vrm::Event& event) {
            turnRightValue += static_cast<float>(event.mouseDeltaX);
            lookUpValue -= static_cast<float>(event.mouseDeltaY);
        });
}

void MyScene::onInit()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    /* ImGui */

    ImGuiIO& io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("Resources/Fonts/Roboto/Roboto-Regular.ttf", 24.0f);
    VRM_ASSERT_MSG(m_Font, "Failed to load font.");

    setCamera(&m_Camera);

    /* Processing */

    

    /* Visualization */

    auto meshEntity = createEntity("Mesh");
    auto& meshComponent = meshEntity.addComponent<vrm::MeshComponent>(m_MeshAsset.createInstance());
    //auto sphere = std::make_unique<Sphere>(glm::vec3{ 0.f, 0.f, 0.f }, 1.5f);
    //auto tSphere = std::make_unique<TransformOperator>(glm::translate(glm::mat4(1.f), glm::vec3{ 1.f, 1.f, 1.f }), sphere.get());
    //auto un = std::make_unique<UnionOperator>(sphere.get(), tSphere.get());
    //auto inter = std::make_unique<IntersectionOperator>(sphere.get(), tSphere.get());
    //auto blend = std::make_unique<BlendOperator>(1.f, sphere.get(), tSphere.get());
    //auto diff = std::make_unique<DifferenceOperator>(sphere.get(), tSphere.get());

    auto lightEntity = createEntity("Light");
    auto& c =  lightEntity.addComponent<vrm::PointLightComponent>();
    c.color = { 1.f, 1.f, 1.f };
    c.intensity = 1000000.f;
    c.radius = 2000.f;
    lightEntity.getComponent<vrm::TransformComponent>().setPosition({ -5.f, 1000.f , -5.f });
}

void MyScene::onEnd()
{
}

void MyScene::onUpdate(float dt)
{
    /* Processing */

    if (m_RealTimeProcessing)
        updateMesh();

    /* Camera */
    if (m_ControlsEnabled && m_MouseLock)
    {
        m_Camera.move(forwardValue * myCameraSpeed * dt * m_Camera.getForwardVector());
        m_Camera.move(rightValue * myCameraSpeed * dt * m_Camera.getRightVector());
        m_Camera.move(upValue * myCameraSpeed * dt * glm::vec3{0.f, 1.f, 0.f});
        m_Camera.addYaw(turnRightValue * myCameraAngularSpeed);
        m_Camera.addPitch(lookUpValue * myCameraAngularSpeed);
    }

    lookUpValue = 0.f;
    turnRightValue = 0.f;
}

void MyScene::onRender()
{
    ImGui::PushFont(m_Font);

    onImGui();

    ImGui::PopFont();
}

void MyScene::updateMesh()
{
    auto start = std::chrono::high_resolution_clock::now();

    const auto* imp = m_SceneGraph.getRoot()->getImplicit();
    vrm::MeshData m;
    imp->Polygonize(m_DiscretizationParameter, m, Box(m_BoxStart, m_BoxEnd));
    m_MeshAsset.clear();
    m_MeshAsset.addSubmesh(std::move(m));

    auto end = std::chrono::high_resolution_clock::now();

    // Time in seconds
    m_LastProcessTimeSeconds = std::chrono::duration<float>(end - start).count();
}

void MyScene::onImGui()
{
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::Begin("Controls");
        ImGui::TextWrapped("While left clicking:");
        ImGui::TextWrapped("WASD to move the camera");
        ImGui::TextWrapped("Space to move up, Left Shift to move down");
        ImGui::Checkbox("Enable controls", &m_ControlsEnabled);
        ImGui::TextWrapped("Camera speed");
        ImGui::SliderFloat("##Camera speed", &myCameraSpeed, 0.f, 100.f);
        ImGui::TextWrapped("Camera angular speed");
        ImGui::SliderFloat("##Camera angular speed", &myCameraAngularSpeed, 0.f, 0.1f);
    ImGui::End();

    ImGui::Begin("Tweaks");
        ImGui::Checkbox("Real-time processing", &m_RealTimeProcessing);
        ImGui::TextWrapped("Discretization parameter");
        ImGui::SliderInt("##Discretization parameter", &m_DiscretizationParameter, 2, 200);
        ImGui::TextWrapped("Box start");
        ImGui::SliderFloat3("##Box start", &m_BoxStart.x, -20.f, 20.f);
        ImGui::TextWrapped("Box end");
        ImGui::SliderFloat3("##Box end", &m_BoxEnd.x, -20.f, 20.f);
        if (ImGui::Button("Manual update"))
            updateMesh();
    ImGui::End();

    m_SceneGraph.onImgui();

    ImGui::Begin("Stats");
        ImGui::TextWrapped("FPS: %.2f", ImGui::GetIO().Framerate);
        ImGui::TextWrapped("Last processing time: %.6f s", m_LastProcessTimeSeconds);
    ImGui::End();
}
