//
// Created by TrifingZW on 24-11-22.
//

#pragma once

#include <functional>

#include "core/graphics/render_target.h"
#include "core/graphics/sprite_batch.h"
#include "scene/node.h"
#include "scene/2d/camera_2d.h"
#include "scene/resources/shader.h"
#include "editor/parser/bin_parser.h"
#include "grid_manager.h"
#include "editor/parser/terrain_config_parser.h"
#include "editor/world/land_unit.h"

class ResourceTextureParser;

class GamePanel2D : public Node
{
public:
    int width = 1, height = 1;
    float TargetCameraZoom = 1.0f;

    Vector2* MouseSelect = nullptr;
    int SelectedTerrainG{}, SelectedTileIdx{};
    std::string ActiveToolButton = "Pen";
    std::string ActiveTopography{};
    int PlacementType{};
    int TerrainOffset[2]{};
    int WaterEdge{}, RoadEdge{};
    bool VerticalResize;
    bool HorizontalResize;
    bool DiagonalResize;

    const char* UI_DOCK_WINDOW = "##ui.dock_window";
    const char* UI_VIEW_BOX = "##ui.view";
    const char* UI_VIEW_GAME_TEST = "选取地形##ui.game.test";
    const char* UI_VIEW_PLACEMENT_SETTINGS = "放置设置##ui.game.placement.settings";

    Texture2D* ColorTexture = nullptr;
    Shader* shader = nullptr;
    HexVertexType* vertexInfo = nullptr;
    BufferManager* GridBufferManager = nullptr;
    LandUnit** landUnit = nullptr;

    Camera2D* camera2d = nullptr;
    RenderTarget* renderTarget = nullptr;

    BinParser binParser = {};
    GridManager* hexManager = nullptr;

    explicit GamePanel2D();
    ~GamePanel2D() override;

    void Init() override;
    void Ready() override;
    void Rendering(SpriteBatch& spriteBatch) override;
    void Gui() override;
    void Process(double delta) override;

    void ImageInput();
    void IterateLandUnit(const std::function<void(LandUnit&)>& func) const;
    void NewFramebuffer(int width, int height);
    void UpdateColorTexture() const;

    static void DrawTerrain(SpriteBatch& spriteBatch, uint8_t type, uint8_t id, const Vector2& position);

    void DockingSpaceUI() const;
    void Menu();
    void GameView();
    void GameTestView();
    void PlacementSettings();
    void EditorModelWindow();

    static float GetMaxRectWidth(
        const Terrain& terrain,
        const ResourceTextureParser& plant_resource_texture,
        const ResourceTextureParser& terrain_resource_texture
    );
};
