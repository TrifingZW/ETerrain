//
// Created by TrifingZW on 24-11-22.
//

#include "game_panel_2d.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "e_terrain.h"
#include "core/core.h"
#include "game_shader.h"
#include "core/helpers/imgui_helper.h"
#include "core/icon/IconsFontAwesome6.h"
#include "core/math/math_funcs.h"

struct ToolButton
{
    std::string icon;
    std::string name;
};

std::vector<ToolButton> ToolButtons = {
    {ICON_FA_PEN, "Pen"},
    {ICON_FA_PEN_FANCY, "Pen Fancy"},
    {ICON_FA_ERASER, "Eraser"}
};

GamePanel2D::GamePanel2D()
{
    auto* camera = new Camera2D();
    camera2d = camera;
    AddChild(camera);
}

GamePanel2D::~GamePanel2D()
{
    delete shader;
    delete vertexInfo;
    delete hexManager;
    delete camera2d;
    delete renderTarget;
}

void GamePanel2D::Init()
{
    NewFramebuffer(width, height);
    ColorTexture = new Texture2D;
    shader = new Shader(GLSL::game_vsh, GLSL::game_fsh, Shader::ShaderSourceType::Code);
}

void GamePanel2D::Ready()
{
    binParser.Parse("world.bin");
    hexManager = new GridManager(binParser.GetWidth(), binParser.GetHeight(), 74.0f);

    ColorTexture->Generate(binParser.GetWidth(), binParser.GetHeight());

    landUnit = new LandUnit *[binParser.GetWidth()];
    for (int x = 0; x < binParser.GetWidth(); x++)
    {
        landUnit[x] = new LandUnit[binParser.GetHeight()];
        for (int y = 0; y < binParser.GetHeight(); y++)
        {
            Topography& topography = binParser.topographies[y * binParser.GetWidth() + x];
            landUnit[x][y].Topography = &topography;
            landUnit[x][y].Position = Vector2(hexManager->GridToPixel({x, y}));
            landUnit[x][y].GridPosition = Vector2I(x, y);
        }
    }

    UpdateColorTexture();
    vertexInfo = new HexVertexType();
    GridBufferManager = new BufferManager();
}

void GamePanel2D::Rendering(SpriteBatch& spriteBatch)
{
    Core::GetGraphicsDevice()->SetRenderTarget(renderTarget);
    Core::GetGraphicsDevice()->Clear();

    spriteBatch.Begin(Graphics::SpriteSortMode::Deferred, camera2d->GetProjectionMatrix());
    spriteBatch.Draw(
        Editor::loadResources->mapLand,
        Rect2(0, 0, hexManager->GetPixelWidth(), hexManager->GetPixelHeight()),
        Color(1.0f, 1.0f, 1.0f, 1.0f)
    );

    IterateLandUnit(
        [&spriteBatch](const LandUnit& landUnit)
        {
            DrawTerrain(
                spriteBatch,
                landUnit.Topography->装饰类型B,
                landUnit.Topography->装饰BID,
                (landUnit.Position + Vector2(landUnit.Topography->装饰BX, landUnit.Topography->装饰BY))
            );
        }
    );

    IterateLandUnit(
        [&spriteBatch](const LandUnit& landUnit)
        {
            DrawTerrain(
                spriteBatch,
                landUnit.Topography->装饰类型A,
                landUnit.Topography->装饰AID,
                landUnit.Position + Vector2(landUnit.Topography->装饰AX, landUnit.Topography->装饰AY)
            );
        }
    );

    IterateLandUnit(
        [&spriteBatch](const LandUnit& landUnit)
        {
            DrawTerrain(
                spriteBatch,
                landUnit.Topography->地块类型,
                landUnit.Topography->地块ID,
                landUnit.Position + Vector2(landUnit.Topography->地块X, landUnit.Topography->地块Y)
            );
        }
    );

    spriteBatch.End();

    shader->Apply();
    shader->SetInt("Image", 0);
    shader->SetInt("Color", 1);
    shader->SetMatrix4("uTransform", camera2d->GetProjectionMatrix());
    shader->SetVector2("texsize", {1024.0f, 1024.0f});
    Core::GetGraphicsDevice()->Textures[1] = ColorTexture;
    Core::GetGraphicsDevice()->SamplerStates[1] = SamplerState::PointWrap;
    spriteBatch.Begin(Graphics::SpriteSortMode::Deferred, camera2d->GetProjectionMatrix());
    spriteBatch.Draw(
        Editor::loadResources->mapSea,
        Rect2(0, 0, hexManager->GetPixelWidth(), hexManager->GetPixelHeight()),
        Color(1.0f, 1.0f, 1.0f, 1.0f)
    );
    spriteBatch.End();

    spriteBatch.Begin(Graphics::SpriteSortMode::Deferred, camera2d->GetProjectionMatrix());
    if (MouseSelect)
        spriteBatch.DrawCenter(Editor::loadResources->AnimStatus, MouseSelect->ToGLM(), Color(1.0f, 1.0f, 1.0f, 1.0f));
    spriteBatch.End();

    Core::GetGraphicsDevice()->ResetRenderTarget();
}

void GamePanel2D::Gui()
{
    constexpr ImGuiWindowFlags dock_space_window_flags = ImGuiWindowFlags_NoDecoration // 无标题栏、不可改变大小、无滚动条、不可折叠
                                                         | ImGuiWindowFlags_NoMove // 不可移动
                                                         | ImGuiWindowFlags_NoBackground // 无背景（背景透明）
                                                         | ImGuiWindowFlags_MenuBar // 菜单栏
                                                         | ImGuiWindowFlags_NoDocking // 不可停靠
                                                         | ImGuiWindowFlags_NoBringToFrontOnFocus // 无法设置前台和聚焦
                                                         | ImGuiWindowFlags_NoNavFocus; // 无法通过键盘和手柄聚焦

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin(UI_DOCK_WINDOW, nullptr, dock_space_window_flags);
    ImGui::PopStyleVar(3);

    DockingSpaceUI();
    Menu();

    ImGui::End();

    GameView();
    GameTestView();
    PlacementSettings();
    EditorModelWindow();

    // ImGui::ShowDemoWindow();
}

void GamePanel2D::Process(const double delta)
{
    camera2d->SetZoom(Math::Lerp(camera2d->GetZoom(), TargetCameraZoom, static_cast<float>(delta) * 20.0f));
}

void GamePanel2D::ImageInput()
{
    const Vector2 mouse_position = ImGuiHelper::GetMousePositionInCamera2DWorld(*camera2d, ImGuiHelper::GetMousePositionInItem());

    delete MouseSelect;
    MouseSelect = new Vector2(hexManager->GetStandardPosition(mouse_position));

    const int grid_width = binParser.GetWidth();
    const int grid_height = binParser.GetHeight();
    const float pixel_width = hexManager->GetPixelWidth();
    const float pixel_height = hexManager->GetPixelHeight();

    if (const float mouseWheel = ImGui::GetIO().MouseWheel; mouseWheel != 0.0f)
        TargetCameraZoom += mouseWheel * 0.1f * camera2d->GetZoom();

    if (mouse_position.X >= pixel_width - 10.0f && mouse_position.X <= pixel_height + 10.0f
        && mouse_position.Y >= 10.0f && mouse_position.Y <= pixel_height + 10.0f)
    {

    }

    if (ImGui::GetIO().MouseDown[0])
    {
        const Vector2I position_grid = hexManager->PixelToGrid(mouse_position);

        if (position_grid.X >= 0 && position_grid.X < grid_width &&
            position_grid.Y >= 0 && position_grid.Y < grid_height)
        {
            Topography* topography = landUnit[position_grid.X][position_grid.Y].Topography;

            if (ActiveToolButton == "Pen")
                switch (PlacementType)
                {
                    case 0:
                        topography->地块类型 = SelectedTerrainG;
                        topography->地块ID = SelectedTileIdx;
                        topography->地块X = TerrainOffset[0];
                        topography->地块Y = TerrainOffset[1];
                        break;
                    case 1:
                        topography->装饰类型A = SelectedTerrainG;
                        topography->装饰AID = SelectedTileIdx;
                        topography->装饰AX = TerrainOffset[0];
                        topography->装饰AY = TerrainOffset[1];
                        break;
                    case 2:
                        topography->装饰类型B = SelectedTerrainG;
                        topography->装饰BID = SelectedTileIdx;
                        topography->装饰BX = TerrainOffset[0];
                        topography->装饰BY = TerrainOffset[1];
                        break;
                    default: break;
                }
            else if (ActiveToolButton == "Pen Fancy")
            {
                ImGui::OpenPopup("Delete?asd");
            }
            else if (ActiveToolButton == "Eraser")
            {
                topography->地块类型 = 1;
                topography->地块ID = 0;
                topography->地块X = 0;
                topography->地块Y = 0;
            }
        }

        UpdateColorTexture();
    }

    if (ImGui::GetIO().MouseDown[1])
        if (const ImVec2 mouseDelta = ImGui::GetIO().MouseDelta; mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
            camera2d->Transform2D.Position -= Vector2(mouseDelta.x, mouseDelta.y) / camera2d->GetZoom();
}

void GamePanel2D::IterateLandUnit(const std::function<void(LandUnit&)>& func) const
{
    for (int y = 0; y < binParser.GetHeight(); y++)
        for (int x = 0; x < binParser.GetWidth(); x++)
            func(landUnit[x][y]);
}

void GamePanel2D::DrawTerrain(SpriteBatch& spriteBatch, uint8_t type, uint8_t id, const Vector2& position)
{
    ResourceTextureParser& plant_resource_texture = Editor::loadResources->plantResourceTextureParser;
    ResourceTextureParser& terrain_resource_texture = Editor::loadResources->terrainResourceTextureParser;
    auto& [terrains] = Editor::loadResources->terrainConfigParser;


    auto find_terrain = [type](const Terrain& terrain) { return terrain.terrainG == type; };
    auto find_tile = [id](const Tile& tile) { return tile.idx == id; };

    // 查找 terrain
    const auto terrain_iterator = std::find_if(
        terrains.begin(),
        terrains.end(),
        find_terrain
    );

    if (terrain_iterator != terrains.end())
    {
        // 查找 tile
        const auto tile_iterator = std::find_if(
            terrain_iterator->tiles.begin(),
            terrain_iterator->tiles.end(),
            find_tile
        );

        if (tile_iterator != terrain_iterator->tiles.end())
        {
            if (const auto rectOpt = plant_resource_texture.GetRect(tile_iterator->image))
                spriteBatch.DrawCenter(
                    &plant_resource_texture.Texture2D,
                    position.ToGLM(),
                    rectOpt.value(),
                    Color(1.0f, 1.0f, 1.0f, 1.0f)
                );
            if (const auto rectOpt = terrain_resource_texture.GetRect(tile_iterator->image))
                spriteBatch.DrawCenter(
                    &terrain_resource_texture.Texture2D,
                    position.ToGLM(),
                    rectOpt.value(),
                    Color(1.0f, 1.0f, 1.0f, 1.0f)
                );
        }
    }
}

void GamePanel2D::NewFramebuffer(const int width, const int height)
{
    // 设置相机视口大小
    camera2d->SetViewportSize(Vector2(static_cast<float>(width), static_cast<float>(height)));
    camera2d->SetZoom(1.0f);

    if (renderTarget)
        renderTarget->Reinitialize(width, height);
    else
        renderTarget = new RenderTarget(width, height);
}

void GamePanel2D::UpdateColorTexture() const
{
    std::vector<unsigned char> colorData;
    IterateLandUnit(
        [this, &colorData](const LandUnit& landUnit)
        {
            unsigned char color[4];
            if (landUnit.Topography->地块类型 == 1)
                color[3] = 0;
            else color[3] = 255;
            colorData.push_back(color[0]);
            colorData.push_back(color[1]);
            colorData.push_back(color[2]);
            colorData.push_back(color[3]);
        }
    );
    ColorTexture->SetRangePixelColor(0, 0, binParser.GetWidth(), binParser.GetHeight(), colorData.data());
}

void GamePanel2D::DockingSpaceUI() const
{
    const ImGuiID dockspace_id = ImGui::GetID("##ui.dock_space");
    if (!ImGui::DockBuilderGetNode(dockspace_id))
    {
        // 删除创建根节点
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGuiID root = ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);

        // 设置根节点位置大小
        ImGui::DockBuilderSetNodePos(root, {0.0f, 0.0f});
        ImGui::DockBuilderSetNodeSize(root, ImGui::GetWindowSize());

        // 分割停靠空间
        const ImGuiID leftNode = ImGui::DockBuilderSplitNode(root, ImGuiDir_Left, 0.25f, nullptr, &root);
        ImGuiID rightNode = ImGui::DockBuilderSplitNode(root, ImGuiDir_Right, 0.25f / 0.75f, nullptr, &root);

        // 禁止其他窗口/节点分割根节点
        // ImGui::DockBuilderGetNode(dockspaceID)->LocalFlags |= ImGuiDockNodeFlags_NoDockingSplit;

        // 设置分割到最后的根节点隐藏标签栏
        ImGui::DockBuilderGetNode(root)->LocalFlags |= ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking;

        // 设置节点停靠窗口
        ImGui::DockBuilderDockWindow(UI_VIEW_BOX, root);
        ImGui::DockBuilderDockWindow(UI_VIEW_GAME_TEST, leftNode);

        ImGui::DockBuilderFinish(dockspace_id);
        ImGui::SetWindowFocus(UI_VIEW_BOX);
    }
    // 创建停靠空间
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void GamePanel2D::Menu()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("文件"))
        {
            if (ImGui::MenuItem("新建", "Ctrl+Alt+Insert")) {}
            if (ImGui::MenuItem("打开", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::MenuItem("fish_hat.c");
                ImGui::MenuItem("fish_hat.inl");
                ImGui::MenuItem("fish_hat.h");
                if (ImGui::BeginMenu("More.."))
                {
                    ImGui::MenuItem("Hello");
                    ImGui::MenuItem("Sailor");
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("保存", "Ctrl+S"))
            {
                binParser.Save("world_save.bin");
            }
            if (ImGui::MenuItem("保存至...")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Options"))
            {
                static bool enabled = true;
                ImGui::MenuItem("Enabled", "", &enabled);
                ImGui::BeginChild("child", ImVec2(0, 60), ImGuiChildFlags_Borders);
                for (int i = 0; i < 10; i++)
                    ImGui::Text("Scrolling Text %d", i);
                ImGui::EndChild();
                static float f = 0.5f;
                static int n = 0;
                ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
                ImGui::InputFloat("Input", &f, 0.1f);
                ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("退出", "Alt+F4")) {}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("编辑"))
        {
            ImGui::EndMenu();
        }

        //增加主题切换
        if (ImGui::BeginMenu("主题（Other）"))
        {
            if (ImGui::MenuItem("暗黑（Dark）")) { ImGui::StyleColorsDark(); }
            if (ImGui::MenuItem("明亮（Light）")) { ImGui::StyleColorsLight(); }
            if (ImGui::MenuItem("经典（Classic）")) { ImGui::StyleColorsClassic(); }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void GamePanel2D::GameView()
{
    constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
                                              | ImGuiWindowFlags_NoCollapse
                                              | ImGuiWindowFlags_NoMove;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 设置窗口内边距为 0
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // 去掉窗口边框
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, {0.5f, 0.5f});

    ImGui::Begin(UI_VIEW_BOX, nullptr, window_flags);

    const float BTN_SIZE = 30.0f * ETerrain::DpiScale;
    const int n = static_cast<int>(std::max(1.0f, ImGui::GetContentRegionAvail().x / (BTN_SIZE)));

    ImGui::BeginChild("Tool", {0, BTN_SIZE}, ImGuiChildFlags_NavFlattened, 0);
    ImGui::Columns(n, nullptr, false);
    for (const auto& [icon, name]: ToolButtons)
    {
        if (ImGui::Selectable(icon.c_str(), name == ActiveToolButton, 0, ImVec2(BTN_SIZE, BTN_SIZE)))
            ActiveToolButton = name;
        ImGui::NextColumn();
    }
    ImGui::EndChild();

    const ImVec2 panelSize = ImGui::GetContentRegionAvail();
    if (static_cast<int>(panelSize.x) != width || static_cast<int>(panelSize.y) != height)
    {
        width = static_cast<int>(CLAMP(panelSize.x, 1.0f, 10000.0f));
        height = static_cast<int>(CLAMP(panelSize.y, 1.0f, 10000.0f));
        NewFramebuffer(width, height);
    }

    ImGui::Image(
        static_cast<ImTextureID>(static_cast<intptr_t>(renderTarget->textureBuffer)),
        panelSize,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    if (ImGui::IsItemHovered())

        ImageInput();
    else
        MouseSelect = nullptr;

    ImGui::End();
    ImGui::PopStyleVar(4);
}

void GamePanel2D::GameTestView()
{
    ImGui::Begin(UI_VIEW_GAME_TEST);

    const ResourceTextureParser& plant_resource_texture = Editor::loadResources->plantResourceTextureParser;
    const ResourceTextureParser& terrain_resource_texture = Editor::loadResources->terrainResourceTextureParser;
    const std::vector<Terrain>& terrains = Editor::loadResources->terrainConfigParser.terrains;
    for (const auto& terrain: terrains)
    {
        if (ImGui::CollapsingHeader((terrain.name + std::to_string(terrain.terrainG)).c_str()))
        {
            const float window_visible_x2 = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;

            for (size_t i = 0; i < terrain.tiles.size(); ++i)
            {
                const auto& [idx, image] = terrain.tiles[i];

                const Texture2D* texture = &plant_resource_texture.Texture2D;
                std::optional<Rect2> rect = plant_resource_texture.GetRect(image);
                if (!rect)
                {
                    rect = terrain_resource_texture.GetRect(image);
                    texture = &terrain_resource_texture.Texture2D;
                }
                if (!rect) continue;

                const ImVec2 button_size = (rect->GetSize() / 2.0f).ToImVec2();
                const Vector2 texture_size = texture->GetSize();

                ImGui::SetNextItemAllowOverlap();
                if (ImGui::Selectable(("##ui.topography." + image).c_str(), image == ActiveTopography, 0, ImVec2(0, button_size.y)))
                {
                    SelectedTerrainG = terrain.terrainG;
                    SelectedTileIdx = idx;
                    ActiveTopography = image;
                }
                ImGui::SameLine();
                ImGui::Image(
                    texture->Id,
                    button_size,
                    (rect->GetPosition() / texture_size).ToImVec2(),
                    ((rect->GetPosition() + rect->GetSize()) / texture_size).ToImVec2()
                );

                /*if (const float next_button_x2 = ImGui::GetItemRectMax().x + button_size.x + ImGui::GetStyle().ItemSpacing.x;
                    i < terrain.tiles.size() - 1 && next_button_x2 < window_visible_x2)
                    ImGui::SameLine();*/
            }
        }
    }
    ImGui::End();
}

void GamePanel2D::PlacementSettings()
{
    ImGui::Begin(UI_VIEW_PLACEMENT_SETTINGS);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Combo("放置类型", &PlacementType, "地块类型\0装饰类型A\0装饰类型B");
    ImGui::InputInt2("地形偏移", TerrainOffset);
    ImGui::InputInt("水边缘", &WaterEdge);
    ImGui::InputInt("路边缘", &RoadEdge);

    ImGui::End();
}

void GamePanel2D::EditorModelWindow()
{
    // Always center this window when appearing
    const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Delete?asd", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!");
        ImGui::Separator();

        //static int unused_i = 0;
        //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

        static bool dont_ask_me_next_time = false;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        ImGui::PopStyleVar();

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
}

float GamePanel2D::GetMaxRectWidth(
    const Terrain& terrain,
    const ResourceTextureParser& plant_resource_texture,
    const ResourceTextureParser& terrain_resource_texture
)
{
    auto max_width = std::max_element(
        terrain.tiles.begin(),
        terrain.tiles.end(),
        [&plant_resource_texture, &terrain_resource_texture](const auto& tile1, const auto& tile2)
        {
            const auto& [idx1, image1] = tile1;
            const auto& [idx2, image2] = tile2;

            std::optional<Rect2> rect1 = plant_resource_texture.GetRect(image1);
            if (!rect1) rect1 = terrain_resource_texture.GetRect(image1);

            std::optional<Rect2> rect2 = plant_resource_texture.GetRect(image2);
            if (!rect2) rect2 = terrain_resource_texture.GetRect(image2);

            const float width1 = rect1 ? rect1->Width : 0;
            const float width2 = rect2 ? rect2->Width : 0;

            return width1 < width2;
        }
    );

    if (max_width != terrain.tiles.end())
    {
        const auto& [idx, image] = *max_width;
        std::optional<Rect2> rect = plant_resource_texture.GetRect(image);
        if (!rect) rect = terrain_resource_texture.GetRect(image);
        return rect ? rect->Width : 0;
    }

    return 0; // 默认值，表示没有有效的 rect
}
