#include "raylib.h"
#include <cmath>

const int screenWidth = 1280;
const int screenHeight = 960;
const int mapWidth = 20;
const int mapHeight = 20;
const float moveSpeed = 0.02;
const float rotationSpeed = 0.02;

int worldMap[mapWidth][mapHeight] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

class Raycasting {
public:
    Raycasting() {
        playerX = 5.0;
        playerY = 5.0;
        playerDirX = 1.0;
        playerDirY = 0.0;
        playerPlaneX = 0.0;
        playerPlaneY = 0.66;
    }

    void Initialize() {
        InitWindow(screenWidth, screenHeight, "pseudo-3D raycasting");
        SetTargetFPS(144);
    }

    void Update() {
        while (!WindowShouldClose()) {
            HandleInput();
            BeginDrawing();
            ClearBackground(BLACK);
            drawMap();
            EndDrawing();
        }

        CloseWindow();
    }

private:
    float playerX;
    float playerY;
    float playerDirX;
    float playerDirY;
    float playerPlaneX;
    float playerPlaneY;

    void HandleInput() {
        if (IsKeyDown(KEY_W)) {
            MovePlayer(playerX + playerDirX * moveSpeed, playerY + playerDirY * moveSpeed);
        }

        if (IsKeyDown(KEY_S)) {
            MovePlayer(playerX - playerDirX * moveSpeed, playerY - playerDirY * moveSpeed);
        }

        if (IsKeyDown(KEY_A)) {
            RotatePlayer(-rotationSpeed);
        }

        if (IsKeyDown(KEY_D)) {
            RotatePlayer(rotationSpeed);
        }
    }

    void MovePlayer(float newX, float newY) {
        if (!isColliding(newX, newY)) {
            playerX = newX;
            playerY = newY;
        }
    }

    void RotatePlayer(float angle) {
        float oldDirX = playerDirX;
        playerDirX = playerDirX * cos(angle) - playerDirY * sin(angle);
        playerDirY = oldDirX * sin(angle) + playerDirY * cos(angle);
        float oldPlaneX = playerPlaneX;
        playerPlaneX = playerPlaneX * cos(angle) - playerPlaneY * sin(angle);
        playerPlaneY = oldPlaneX * sin(angle) + playerPlaneY * cos(angle);
    }

    bool isColliding(float x, float y) {
        int mapX = static_cast<int>(x);
        int mapY = static_cast<int>(y);
        return (worldMap[mapX][mapY] == 1);
    }

    void drawMap() {
        Color floorColor = { 105, 85, 50, 255 };
        DrawRectangle(0, 0, screenWidth, screenHeight, floorColor);

        for (int x = 0; x < screenWidth; x++) {
            float cameraX = 2 * x / (float)screenWidth - 1;
            float rayDirX = playerDirX + playerPlaneX * cameraX;
            float rayDirY = playerDirY + playerPlaneY * cameraX;

            int mapX = static_cast<int>(playerX);
            int mapY = static_cast<int>(playerY);

            float sideDistX;
            float sideDistY;

            float deltaDistX = std::abs(1 / rayDirX);
            float deltaDistY = std::abs(1 / rayDirY);
            float perpWallDist;

            int stepX;
            int stepY;

            int hit = 0;
            int side; 

            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (playerX - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
            }

            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (playerY - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
            }

            while (hit == 0) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                if (worldMap[mapX][mapY] > 0) {
                    hit = 1;
                }
            }

            if (side == 0) {
                perpWallDist = (mapX - playerX + (1 - stepX) / 2) / rayDirX;
            } else {
                perpWallDist = (mapY - playerY + (1 - stepY) / 2) / rayDirY;
            }

            int lineHeight = static_cast<int>(screenHeight / perpWallDist);

            int drawStart = -lineHeight / 2 + screenHeight / 2;
            if (drawStart < 0) {
                drawStart = 0;
            }

            int drawEnd = lineHeight / 2 + screenHeight / 2;
            if (drawEnd >= screenHeight) {
                drawEnd = screenHeight - 1;
            }

            Color wallColor = { 100, 100, 50, 255 };

            if (isColliding(playerX, playerY)) {
                playerX = std::round(playerX);
                playerY = std::round(playerY);
            }

            DrawRectangle(x, drawStart, 1, drawEnd - drawStart, wallColor);
        }
    }
};

int main() {
    Raycasting test;
    test.Initialize();
    test.Update();
    return 0;
}
