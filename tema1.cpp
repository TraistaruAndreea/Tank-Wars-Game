#include "lab_m1/tema1/tema1.h"
#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;

Tema1::Tema1() {
}

Tema1::~Tema1() {
}

std::vector<glm::vec2> terrain_points;
float freq = 0.02f;
float squareSide = 1;
float tank1Radius = 25.0f;
float tank2Radius = 25.0f;
float projectileRadius = 7.0f;
float tank1Life = 1.0f;
float tank2Life = 1.0f;
float elapsedTime = 0.0f;
float tank1DestroyTime = -1.0f;
float tank2DestroyTime = -1.0f;

struct Projectile {
    glm::vec2 position;
    glm::vec2 velocity;
    float angle;
};
std::vector<Projectile> activeProjectilesTank1;
std::vector<Projectile> activeProjectilesTank2;

float TerrainFunction(float t) {
    t *= freq;
    return 5 * sin(t) + 10 * sin(0.5f * t) + 0.5f * sin(0.5f * t);

}


void Tema1::Init() {
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    Mesh* square = object2D::CreateSquare("square", corner, squareSide, glm::vec3(0.2f, 0.7f, 0.3f), true);
    AddMeshToList(square);

    Mesh* baseTrapezoid1Tank1 = object2D::CreateTrapezoid("baseTrapezoid1Tank1", 110, 20, 100, glm::vec3(0.6f, 0.4f, 0.2f), true);
    AddMeshToList(baseTrapezoid1Tank1);

    Mesh* baseTrapezoid1Tank2 = object2D::CreateTrapezoid("baseTrapezoid1Tank2", 110, 20, 100, glm::vec3(1.0f, 0.4f, 0.7f), true);
    AddMeshToList(baseTrapezoid1Tank2);

    Mesh* baseTrapezoid2Tank1 = object2D::CreateTrapezoid("baseTrapezoid2Tank1", 80, 17, 100, glm::vec3(0.2f, 0.2f, 0.6f), true);
    AddMeshToList(baseTrapezoid2Tank1);
    
    Mesh* baseTrapezoid2Tank2 = object2D::CreateTrapezoid("baseTrapezoid2Tank2", 80, 17, 100, glm::vec3(0.2f, 0.4f, 0.6f), true);
    AddMeshToList(baseTrapezoid2Tank2);
    
    Mesh* circleTank1 = object2D::CreateCircle("circleTank1", corner, 21, glm::vec3(0.6f, 0.4f, 0.2f), true);
    AddMeshToList(circleTank1);

    Mesh* circleTank2 = object2D::CreateCircle("circleTank2", corner, 21, glm::vec3(0.6f, 0.4f, 0.4f), true);
    AddMeshToList(circleTank2);

    Mesh* rectangleTank1 = object2D::CreateRectangle("rectangleTank1", corner, 6.0f, 60.0f, glm::vec3(0.4f, 0.4f, 0.4f), true);
    AddMeshToList(rectangleTank1);

    Mesh* rectangleTank2 = object2D::CreateRectangle("rectangleTank2", corner, 6.0f, 60.0f, glm::vec3(0.4f, 0.4f, 0.4f), true);
    AddMeshToList(rectangleTank2);

	Mesh* projectileTank1 = object2D::CreateCircle("projectileTank1", corner, 7, glm::vec3(0.4f, 0.4f, 0.4f), true);
	AddMeshToList(projectileTank1);

    Mesh* projectileTank2 = object2D::CreateCircle("projectileTank2", corner, 7, glm::vec3(0.4f, 0.4f, 0.4f), true);
    AddMeshToList(projectileTank2);

	Mesh* trajectoryTank1 = object2D::CreateSquare("trajectoryTank1", corner, 1.5f, glm::vec3(0.8f, 0.8f, 0.8f), true);
	AddMeshToList(trajectoryTank1);

    Mesh* trajectoryTank2 = object2D::CreateSquare("trajectoryTank2", corner, 1.5f, glm::vec3(0.8f, 0.8f, 0.8f), true);
    AddMeshToList(trajectoryTank2);

    Mesh* barBehindTank1 = object2D::CreateRectangle("barBehindTank1", corner, 22.0f, 90.0f, glm::vec3(0.8f, 0.8f, 0.8f), false);
    AddMeshToList(barBehindTank1);

    Mesh* barBehindTank2 = object2D::CreateRectangle("barBehindTank2", corner, 22.0f, 90.0f, glm::vec3(0.8f, 0.8f, 0.8f), false);
    AddMeshToList(barBehindTank2);

    Mesh* barAboveTank1 = object2D::CreateRectangle("barAboveTank1", corner, 22.0f, 90.0f, glm::vec3(0.8f, 0.8f, 0.8f), true);
    AddMeshToList(barAboveTank1);

    Mesh* barAboveTank2 = object2D::CreateRectangle("barAboveTank2", corner, 22.0f, 90.0f, glm::vec3(0.8f, 0.8f, 0.8f), true);
    AddMeshToList(barAboveTank2);

    Mesh* dustCloud = object2D::CreateCircle("dustCloud", corner, 10, glm::vec3(0.5f, 0.5f, 0.5f), true);
	AddMeshToList(dustCloud);

    float elapsedTime = 0.0f;

    tank1X = 100;
    tank1Y = 100;
	turret1Angle = 0;

    tank2X = 1100;
    tank2Y = 100;
	turret2Angle = 0;

	projectileActive = false;
	gravity = 300.0f;

    int num_points = resolution.x;
    terrain_points.reserve(num_points);

    for (int i = 0; i < num_points; i++) {
        float x = i;
        float y = 8 * TerrainFunction(i) + 300;
        terrain_points.emplace_back(x, y);
    }
}


void Tema1::RenderTank(float x, float y, float rotationAngle, float turretAngle, int tank) {
    glm::mat3 modelMatrix;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x, y);
    modelMatrix *= transform2D::Rotate(rotationAngle);

    if (tank == 1) {
        glm::mat3 baseMatrix = modelMatrix;
        baseMatrix *= transform2D::Translate(0, 20);
        RenderMesh2D(meshes["baseTrapezoid1Tank1"], shaders["VertexColor"], baseMatrix);

        baseMatrix = modelMatrix;
        baseMatrix *= transform2D::Translate(0, 6);
        baseMatrix *= transform2D::Scale(-1, -1);
        RenderMesh2D(meshes["baseTrapezoid2Tank1"], shaders["VertexColor"], baseMatrix);

        glm::mat3 turretMatrix = modelMatrix;
        turretMatrix *= transform2D::Translate(0, 28.0f);
        RenderMesh2D(meshes["circleTank1"], shaders["VertexColor"], turretMatrix);

        glm::mat3 barrelMatrix = modelMatrix;
        barrelMatrix *= transform2D::Translate(0, 33.0f);
        float barrelInclineAngle = glm::radians(-45.0f);
        barrelMatrix *= transform2D::Rotate(barrelInclineAngle);
        barrelMatrix *= transform2D::Rotate(turretAngle);
        RenderMesh2D(meshes["rectangleTank1"], shaders["VertexColor"], barrelMatrix);
        glm::vec3 tipPosition = barrelMatrix * glm::vec3(1, 1, 1);
        turretTipX = tipPosition.x;
        turretTipY = tipPosition.y;
    }
    else {
        glm::mat3 baseMatrix = modelMatrix;
        baseMatrix *= transform2D::Translate(0, 20);
        RenderMesh2D(meshes["baseTrapezoid1Tank2"], shaders["VertexColor"], baseMatrix);

        baseMatrix = modelMatrix;
        baseMatrix *= transform2D::Translate(0, 6);
        baseMatrix *= transform2D::Scale(-1, -1);
        RenderMesh2D(meshes["baseTrapezoid2Tank2"], shaders["VertexColor"], baseMatrix);

        glm::mat3 turretMatrix = modelMatrix;
        turretMatrix *= transform2D::Translate(0, 28.0f);
        RenderMesh2D(meshes["circleTank2"], shaders["VertexColor"], turretMatrix);

        glm::mat3 barrelMatrix = modelMatrix;
        barrelMatrix *= transform2D::Translate(0, 33.0f);
        float barrelInclineAngle = glm::radians(-45.0f);
        barrelMatrix *= transform2D::Rotate(barrelInclineAngle);
        barrelMatrix *= transform2D::Rotate(turretAngle);
        RenderMesh2D(meshes["rectangleTank2"], shaders["VertexColor"], barrelMatrix);
        glm::vec3 tipPosition = barrelMatrix * glm::vec3(1, 1, 1);
        turretTipX = tipPosition.x;
        turretTipY = tipPosition.y;
    }

}


void Tema1::RenderTankHealthBars(float x, float y, float rotationAngle, int tank) {
    glm::mat3 modelMatrix;

    float lifePercentage;
	if (tank == 1) {
		lifePercentage = tank1Life;
	}
	else {
		lifePercentage = tank2Life;
	}
    float barWidth = 90.0f * lifePercentage;

    if (tank == 1) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        modelMatrix *= transform2D::Rotate(rotationAngle);
        modelMatrix *= transform2D::Translate(40.0f, 60.0f);
        modelMatrix *= transform2D::Rotate(glm::radians(90.0f));
        RenderMesh2D(meshes["barBehindTank1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        modelMatrix *= transform2D::Rotate(rotationAngle);
        modelMatrix *= transform2D::Translate(40.0f, 60.0f);
        modelMatrix *= transform2D::Rotate(glm::radians(90.0f));
        modelMatrix *= transform2D::Scale(1.0f, lifePercentage);
        RenderMesh2D(meshes["barAboveTank1"], shaders["VertexColor"], modelMatrix);
    }
    else {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        modelMatrix *= transform2D::Rotate(rotationAngle);
        modelMatrix *= transform2D::Translate(40.0f, 60.0f);
        modelMatrix *= transform2D::Rotate(glm::radians(90.0f));
        RenderMesh2D(meshes["barBehindTank2"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(x, y);
        modelMatrix *= transform2D::Rotate(rotationAngle);
        modelMatrix *= transform2D::Translate(40.0f, 60.0f);
        modelMatrix *= transform2D::Rotate(glm::radians(90.0f));
        modelMatrix *= transform2D::Scale(1.0f, lifePercentage);
        RenderMesh2D(meshes["barAboveTank2"], shaders["VertexColor"], modelMatrix);
    }
}


bool CheckProjectileCollision(const glm::vec2& projectilePos, const std::vector<glm::vec2>& terrain, float collisionThreshold) {
    for (int i = 1; i < terrain.size(); ++i) {
        glm::vec2 P1 = terrain[i - 1];
        glm::vec2 P2 = terrain[i];

        if (projectilePos.x >= P1.x && projectilePos.x <= P2.x) {
            float t = (projectilePos.x - P1.x) / (P2.x - P1.x);
            float terrainY = P1.y * (1 - t) + P2.y * t;

            if (abs(projectilePos.y - terrainY) <= collisionThreshold) {
                return true;
            }
        }
    }
    return false;
}


std::vector<glm::vec2> CalculateProjectilePath(glm::vec2 startPosition, float angle, float initialSpeed, float gravity, int numSegments) {
    std::vector<glm::vec2> trajectory;
    float timeStep = 0.1f;

    for (int i = 0; i < numSegments; i++) {
        float t = i * timeStep;
        float x = startPosition.x + initialSpeed * cos(angle) * t;
        float y = startPosition.y + initialSpeed * sin(angle) * t - 0.5f * gravity * t * t;
        trajectory.push_back(glm::vec2(x, y));
        if (y < 0) {
            break;
        }

    }

    return trajectory;
}


void Tema1::RenderTrajectory(glm::vec2 startPosition, float angle, float initialSpeed, float gravity, float tank) {
    int numSegments = 60;
    std::vector<glm::vec2> trajectory = CalculateProjectilePath(startPosition, angle, initialSpeed, gravity, numSegments);

    for (size_t i = 1; i < trajectory.size(); ++i) {
        glm::vec2 p1 = trajectory[i - 1];
        glm::vec2 p2 = trajectory[i];

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(p1.x, p1.y);

        float segmentLength = glm::length(p2 - p1);
        float segmentAngle = atan2(p2.y - p1.y, p2.x - p1.x);

        modelMatrix *= transform2D::Rotate(segmentAngle);
        modelMatrix *= transform2D::Scale(segmentLength, 1.0f);

        if (tank == 1) {
            RenderMesh2D(meshes["trajectoryTank1"], shaders["VertexColor"], modelMatrix);
        }
        else {
            RenderMesh2D(meshes["trajectoryTank2"], shaders["VertexColor"], modelMatrix);
        }
    }
}


bool CheckTankCollision(const glm::vec2& pos1, float radius1, const glm::vec2& pos2, float radius2) {
    float distance = glm::distance(pos1, pos2);
    return distance < (radius1 + radius2);
}


void DeformTerrain(std::vector<glm::vec2>& terrain, const glm::vec2& collisionPoint, float radius, float depth) {
    for (auto& point : terrain) {
        float distance = glm::distance(point, collisionPoint);
        if (distance < radius) {
            point.y -= depth * (1.0f - (distance / radius) * (distance / radius));
        }
    }
}


void Tema1::RenderProjectile(float tank, float deltaTimeSeconds) {

    if (tank == 1) {
        for (auto projectile = activeProjectilesTank1.begin(); projectile != activeProjectilesTank1.end();) {
            projectile->position += projectile->velocity * deltaTimeSeconds;
            projectile->velocity.y -= gravity * deltaTimeSeconds;
            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(projectile->position.x, projectile->position.y);
            modelMatrix *= transform2D::Rotate(projectile->angle);
            if (tank1Life > 0) {
                RenderMesh2D(meshes["projectileTank1"], shaders["VertexColor"], modelMatrix);
            }

            if (CheckTankCollision(projectile->position, projectileRadius, glm::vec2(tank1X, tank1Y), tank1Radius)) {
                ReduceTankLife(1);
                projectile = activeProjectilesTank1.erase(projectile);
                continue;
            }

            if (CheckTankCollision(projectile->position, projectileRadius, glm::vec2(tank2X, tank2Y), tank2Radius)) {
                ReduceTankLife(2);
                projectile = activeProjectilesTank1.erase(projectile);
                continue;
            }

            if (CheckProjectileCollision(projectile->position, terrain_points, 30.0f)) {
                DeformTerrain(terrain_points, projectile->position, 100.0f, 50.0f);
                projectile = activeProjectilesTank1.erase(projectile);
            }
            else {
                projectile++;
            }
        }
    }
    else {
        for (auto projectile = activeProjectilesTank2.begin(); projectile != activeProjectilesTank2.end();) {
            projectile->position += projectile->velocity * deltaTimeSeconds;
            projectile->velocity.y -= gravity * deltaTimeSeconds;
            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(projectile->position.x, projectile->position.y);
            modelMatrix *= transform2D::Rotate(projectile->angle);

            if (tank2Life > 0) {
                RenderMesh2D(meshes["projectileTank1"], shaders["VertexColor"], modelMatrix);
            }

            if (CheckTankCollision(projectile->position, projectileRadius, glm::vec2(tank1X, tank1Y), tank1Radius)) {
                ReduceTankLife(1);
                projectile = activeProjectilesTank2.erase(projectile);
                continue;
            }

            if (CheckTankCollision(projectile->position, projectileRadius, glm::vec2(tank2X, tank2Y), tank2Radius)) {
                ReduceTankLife(2);
                projectile = activeProjectilesTank2.erase(projectile);
                continue;
            }

            if (CheckProjectileCollision(projectile->position, terrain_points, 30.0f)) {
                DeformTerrain(terrain_points, projectile->position, 100.0f, 50.0f);
                projectile = activeProjectilesTank2.erase(projectile);
            }
            else {
                projectile++;
            }
        }
    }
}


float CalculateSlopeAngle(glm::vec2 A, glm::vec2 B) {
    glm::vec2 V = B - A;
    return atan2(V.y, V.x);
}


float CalculateTankY(float tankX, glm::vec2 A, glm::vec2 B) {
    float t = (tankX - A.x) / (B.x - A.x);
    return A.y + t * (B.y - A.y);
}


void Tema1::ReduceTankLife(int tank) {
    if (tank == 1 && tank1Life > 0) {
        tank1Life -= 0.2f;
    }
    else if (tank == 2 && tank2Life > 0) {
        tank2Life -= 0.2f;
    }
}


void ApplyTerrainSlide(std::vector<glm::vec2>& terrain, float heightThreshold, float slideAmount, float deltaTime) {
    for (size_t i = 1; i < terrain.size(); ++i) {
        float d = abs(terrain[i].y - terrain[i - 1].y);
        while (d > heightThreshold) {
            if (terrain[i].y > terrain[i - 1].y) {
                terrain[i].y -= slideAmount * deltaTime;
                terrain[i - 1].y += slideAmount * deltaTime;
            }
            else {
                terrain[i].y += slideAmount * deltaTime;
                terrain[i - 1].y -= slideAmount * deltaTime;
            }
			d = abs(terrain[i].y - terrain[i - 1].y);
        }
    }
}

void Tema1::RenderDustCloud(float centerX, float centerY, float duration, float timeElapsed) {
    int numCircles = 15;
    float maxScale = 2.0f;

    for (int i = 0; i < numCircles; ++i) {
        float offsetX = ((rand() % 100) / 50.0f - 1.0f) * 30.0f;
        float offsetY = ((rand() % 100) / 50.0f - 1.0f) * 30.0f;

        float scale = maxScale * (1.0f - timeElapsed / duration);
        if (scale < 0.0f) scale = 0.0f;

        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(centerX + offsetX, centerY + offsetY);
        modelMatrix *= transform2D::Scale(scale, scale);

		RenderMesh2D(meshes["dustCloud"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::Update(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();
    elapsedTime += deltaTimeSeconds;

    if (tank1DestroyTime >= 0.0f) tank1DestroyTime += deltaTimeSeconds;
    if (tank2DestroyTime >= 0.0f) tank2DestroyTime += deltaTimeSeconds;
    for (int i = 1; i < terrain_points.size(); i++) {
        glm::vec2 prev = terrain_points[i - 1];
        glm::vec2 curr = terrain_points[i];

        modelMatrix = glm::mat3(1);
        float width = curr.x - prev.x;
        float height = std::max(curr.y, prev.y);

        modelMatrix *= transform2D::Translate(prev.x, prev.y);
        float sheerFactor = (curr.y - prev.y) / (curr.x - prev.x);
        modelMatrix *= transform2D::Sheer(sheerFactor);
        modelMatrix *= transform2D::Scale(width, -height);

        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
    }

    for (int i = 1; i < terrain_points.size(); i++) {
        glm::vec2 prev = terrain_points[i - 1];
        glm::vec2 curr = terrain_points[i];

        if (tank1X > prev.x && tank1X <= curr.x) {
            glm::vec2 A = prev;
            glm::vec2 B = curr;
            tank1Y = CalculateTankY(tank1X, A, B);
            rotationAngle1 = CalculateSlopeAngle(A, B);
            RenderTank(tank1X, tank1Y, rotationAngle1, turret1Angle, 1);
            turretTip1X = turretTipX;
            turretTip1Y = turretTipY;

            float angle = turret1Angle + rotationAngle1 - glm::radians(-45.0f);
            RenderTrajectory(glm::vec2(turretTip1X, turretTip1Y), angle, 400.0f, gravity, 1);
            RenderProjectile(1, deltaTimeSeconds);
        }
    }
    for (int i = 1; i < terrain_points.size(); i++) {
        glm::vec2 prev = terrain_points[i - 1];
        glm::vec2 curr = terrain_points[i];

        if (tank2X >= prev.x && tank2X < curr.x) {
            glm::vec2 A = prev;
            glm::vec2 B = curr;
            tank2Y = CalculateTankY(tank2X, A, B);
            rotationAngle2 = CalculateSlopeAngle(A, B);
            RenderTank(tank2X, tank2Y, rotationAngle2, turret2Angle, 2);
            turretTip2X = turretTipX;
            turretTip2Y = turretTipY;

            float angle = turret2Angle + rotationAngle2 - glm::radians(-45.0f);
            RenderTrajectory(glm::vec2(turretTip2X, turretTip2Y), angle, 400.0f, gravity, 2);
            RenderProjectile(2, deltaTimeSeconds);
        }
    }

    if (tank1Life > 0) {
        RenderTankHealthBars(tank1X, tank1Y, rotationAngle1, 1);
    }
    if (tank2Life > 0) {
        RenderTankHealthBars(tank2X, tank2Y, rotationAngle2, 2);
    }

	ApplyTerrainSlide(terrain_points, 2.0f, 30.0f, deltaTimeSeconds);

    if (tank1Life <= 0.2f && tank1DestroyTime < 0.0f) {

        meshes.erase("baseTrapezoid1Tank1");
        meshes.erase("baseTrapezoid2Tank1");
        meshes.erase("circleTank1");
        meshes.erase("rectangleTank1");
        meshes.erase("projectileTank1");
        meshes.erase("trajectoryTank1");
        meshes.erase("barBehindTank1");
        meshes.erase("barAboveTank1");
        activeProjectilesTank1.clear();
		tank1DestroyTime = 0.0f;
    }

    if (tank2Life <= 0.2f && tank2DestroyTime < 0.0f) {
        meshes.erase("baseTrapezoid1Tank2");
        meshes.erase("baseTrapezoid2Tank2");
        meshes.erase("circleTank2");
        meshes.erase("rectangleTank2");
        meshes.erase("projectileTank2");
        meshes.erase("trajectoryTank2");
        meshes.erase("barBehindTank2");
        meshes.erase("barAboveTank2");
        activeProjectilesTank2.clear();
		tank2DestroyTime = 0.0f;
    }

    if (tank1DestroyTime >= 0.0f && tank1DestroyTime <= 2.0f) {
        RenderDustCloud(tank1X, tank1Y, 2.0f, tank1DestroyTime);
    }

	if (tank2DestroyTime >= 0.0f && tank2DestroyTime <= 2.0f) {
		RenderDustCloud(tank2X, tank2Y, 2.0f, tank2DestroyTime);
	}
}


void Tema1::FrameStart()
{
    glClearColor(0.52f, 0.81f, 0.92f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::FrameEnd()
{
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_A)) {
        tank1X -= 200 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        tank1X += 200 * deltaTime;
    }

	if (window->KeyHold(GLFW_KEY_LEFT)) {
		tank2X -= 200 * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_RIGHT)) {
		tank2X += 200 * deltaTime;
	}

    if (window->KeyHold(GLFW_KEY_W) && turret1Angle <= glm::pi<float>() / 1.5) {
        turret1Angle += 1.5f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S) && turret1Angle > -0.5) {
        turret1Angle -= 1.5f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_UP) && turret2Angle < glm::pi<float>() / 1.5) {
        turret2Angle += 1.5f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_DOWN) && turret2Angle > -0.5) {
        turret2Angle -= 1.5f * deltaTime;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (window->KeyHold(GLFW_KEY_SPACE)) {
        Projectile newProjectile;
        float angle = turret1Angle + rotationAngle1 - glm::radians(-45.0f);
        newProjectile.position = glm::vec2(turretTip1X, turretTip1Y);
        newProjectile.velocity = glm::vec2(cos(angle), sin(angle)) * 400.0f;
        newProjectile.angle = angle;
        activeProjectilesTank1.push_back(newProjectile);
    }

    if (window->KeyHold(GLFW_KEY_ENTER)) {
		Projectile newProjectile;
        float angle = turret2Angle + rotationAngle2 - glm::radians(-45.0f);
        newProjectile.position = glm::vec2(turretTip2X, turretTip2Y);
		newProjectile.velocity = glm::vec2(cos(angle), sin(angle)) * 400.0f;
		newProjectile.angle = angle;
		activeProjectilesTank2.push_back(newProjectile);
    }

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}