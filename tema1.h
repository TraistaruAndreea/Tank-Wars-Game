#pragma once

#include "components/simple_scene.h"
#include "utils/glm_utils.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void Tema1::RenderTank(float x, float y, float rotationAngle, float turretAngle, int tank);
        void Tema1::RenderTrajectory(glm::vec2 startPosition, float angle, float initialSpeed, float gravity, float tank);
        void Tema1::ReduceTankLife(int tank);
        void Tema1::RenderTankHealthBars(float x, float y, float rotationAngle, int tank);
        void Tema1::RenderProjectile(float tank, float deltaTimeSeconds);
        void Tema1::RenderDustCloud(float centerX, float centerY, float duration, float timeElapsed);


     protected:
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        
        float numSquares;
        std::vector<float> heightMap;

		float tank1X, tank1Y;
		float tank2X, tank2Y;

		float turret1Angle, turret2Angle;

		bool projectileActive;
        glm::vec2 projectilePosition;
        glm::vec2 projectileVelocity;
        float gravity;

		float turretTipX, turretTipY;
		float turretTip1X, turretTip1Y;
		float turretTip2X, turretTip2Y;
        glm::vec3 tipPosition;
		float rotationAngle1;
		float rotationAngle2;
        float elapsedTime;
    };
}   // namespace m1
