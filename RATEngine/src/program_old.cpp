#include <raylib-cpp.hpp>
#include <stdio.h>

#include "constants.hpp"
#include "map.hpp"
#include "actor.hpp"
#include "glyphtype.hpp"
#include "mt_engine.hpp"

constexpr size_t minimumEnemies = 50;
constexpr size_t maximumEnemies = 100;

int main()
{
    MTEngine random;
    raylib::Window window(windowWidth, windowHeight, windowTitle);
    window.SetState(FLAG_WINDOW_UNDECORATED);

    SetTargetFPS(30);

    GlyphType* glyphType = new GlyphType("assets/fonts/font.png", 12, 12);

    Map* map = new Map();
    map->Generate();
    map->Smooth();

    map->Populate();

    Actor* player = new Actor("Player", { actorGlyph, GREEN }, map->FindOpen(1.0f), true);
    size_t totalEnemies = (size_t)random.Next(minimumEnemies, maximumEnemies);
    std::vector<Actor*> enemies(totalEnemies, nullptr);

    for (int i = 0; i < totalEnemies; i++)
        enemies[i] = new Actor("Ork", { actorGlyph, RED }, map->FindOpen(1.0f));

    bool centerOnPlayer = true;

    while (!window.ShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
            centerOnPlayer = !centerOnPlayer;

        if (!centerOnPlayer)
        {
            if (IsKeyDown(KEY_RIGHT))
                map->Move({ -1 * cameraSpeed, 0 * cameraSpeed });
            if (IsKeyDown(KEY_LEFT))
                map->Move({ 1 * cameraSpeed, 0 * cameraSpeed });
            if (IsKeyDown(KEY_DOWN))
                map->Move({ 0 * cameraSpeed, -1 * cameraSpeed });
            if (IsKeyDown(KEY_UP))
                map->Move({ 0 * cameraSpeed, 1 * cameraSpeed });
        }
        
        if (IsKeyPressed(KEY_F1))
        {
            map->Generate();
            map->Smooth();
        }

        int x = 0;
        int y = 0;

        if (IsKeyPressed(KEY_D))
        { x = 1; }
        if (IsKeyPressed(KEY_A))
        { x = -1; }
        if (IsKeyPressed(KEY_S))
        { y = 1; }
        if (IsKeyPressed(KEY_W))
        { y = -1; }

        
        if (IsKeyPressed(KEY_KP_6))
        { x = 1; }
        if (IsKeyPressed(KEY_KP_4))
        { x = -1; }
        if (IsKeyPressed(KEY_KP_2))
        { y = 1; }
        if (IsKeyPressed(KEY_KP_8))
        { y = -1; }

        if (IsKeyPressed(KEY_KP_3))
        { x = 1; y = 1; }
        if (IsKeyPressed(KEY_KP_9))
        { x = 1; y = -1; }
        if (IsKeyPressed(KEY_KP_1))
        { x = -1; y = 1; }
        if (IsKeyPressed(KEY_KP_7))
        { x = -1; y = -1; }

        if (x != 0 || y != 0)
            player->Act(x, y, *map);
        

        if (centerOnPlayer)
            map->CenterOn(player->GetPosition());

        BeginDrawing();

            ClearBackground(CHARCOAL);

            map->Draw(*glyphType);

            DrawRectangle(0, 0, windowWidth, windowHeight * 0.025f + 10, CHARCOAL);
            DrawText(windowTitle, 0, 0, 20, MARBLE);

            //DrawRectangle(windowWidth * 0.25f, windowHeight - windowHeight * 0.125f, windowWidth * 0.5f, windowHeight * 0.125f, CHARCOAL);
            //DrawText(lastMessage, windowWidth * 0.25f + 10, windowHeight - windowHeight * 0.125f + 10, 15, MARBLE);

        EndDrawing();
    }

    delete glyphType;
    delete map;
    delete player;

    for (auto& enemy : enemies)
        delete enemy;

    return 0;
}