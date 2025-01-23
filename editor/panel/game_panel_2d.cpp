//
// Created by TrifingZW on 24-11-22.
//

#include "game_panel_2d.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "core/core.h"
#include "game_shader.h"
#include "core/helpers/imgui_helper.h"
#include "core/math/math_funcs.h"

struct ImGuiDockNode;

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
    shader = new Shader(GLSL::game_vsh, GLSL::game_fsh, Shader::ShaderSourceType::Code);
}

void GamePanel2D::Ready()
{
    binParser.Parse("world.bin");
    hexManager = new HexManager(binParser.GetWidth(), binParser.GetHeight(), 74.0f);

    landUnit = new LandUnit *[binParser.GetWidth()];
    for (int x = 0; x < binParser.GetWidth(); x++)
    {
        landUnit[x] = new LandUnit[binParser.GetHeight()];
        for (int y = 0; y < binParser.GetHeight(); y++)
        {
            landUnit[x][y].Topography = binParser.topographies[y * binParser.GetWidth() + x];
            landUnit[x][y].Position = Vector2(hexManager->GridToPixel({x, y}));
            landUnit[x][y].GridPosition = Vector2I(x, y);
        }
    }


    /*vertexInfo = new HexVertexType();
    GridBufferManager = new BufferManager();
    GenerateOceanVertex();
    GenerateBuffer();*/
}

void GamePanel2D::Rendering(SpriteBatch& spriteBatch)
{
    Core::GetGraphicsDevice()->SetRenderTarget(renderTarget);
    Core::GetGraphicsDevice()->Clear();

    spriteBatch.Begin(Graphics::SpriteSortMode::Deferred, camera2d->GetProjectionMatrix());
    spriteBatch.Draw(
        Editor::loadResources->mapLand,
        Rect(0, 0, hexManager->GetPixelWidth(), hexManager->GetPixelHeight()),
        Color(1.0f, 1.0f, 1.0f, 1.0f)
    );

    IterateLandUnit(
        [&spriteBatch](const LandUnit& landUnit, const Vector2& position)
        {
            DrawTerrain(
                spriteBatch,
                landUnit.Topography.装饰类型B,
                landUnit.Topography.装饰BID,
                (position + Vector2(landUnit.Topography.装饰BX, landUnit.Topography.装饰BY))
            );
        }
    );

    IterateLandUnit(
        [&spriteBatch](const LandUnit& landUnit, const Vector2& position)
        {
            DrawTerrain(
                spriteBatch,
                landUnit.Topography.装饰类型A,
                landUnit.Topography.装饰AID,
                position + Vector2(landUnit.Topography.装饰AX, landUnit.Topography.装饰AY)
            );
        }
    );

    IterateLandUnit(
        [&spriteBatch](const LandUnit& landUnit, const Vector2& position)
        {
            DrawTerrain(
                spriteBatch,
                landUnit.Topography.地块类型,
                landUnit.Topography.地块ID,
                position + Vector2(landUnit.Topography.地块X, landUnit.Topography.地块Y)
            );
        }
    );

    if (MouseSelect)
        spriteBatch.DrawCenter(Editor::loadResources->AnimStatus, MouseSelect->ToGLM(), Color(1.0f, 1.0f, 1.0f, 1.0f));

    spriteBatch.End();


    /*glm::ma0t4 model(1.0f);
    model = scale(model, glm::vec3(glm::vec2(HexMetrics::TileWidth / 2.0f), 1.0f));
    shader->Apply();
    shader->SetMatrix4("model", model);
    shader->SetMatrix4("uTransform", camera2d->GetProjectionMatrix());
    Core::GetGraphicsDevice()->textures[0] = Editor::loadResources->mapLand;
    Core::GetGraphicsDevice()->DrawUserPrimitivesIndexed(GL_LINE_LOOP, GridBufferManager, 6, binParser.GetSize());*/


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

    DockingSpace();
    Menu();

    ImGui::End();

    GameView();
    GameTestView();
}

void GamePanel2D::Process(const double delta)
{
    camera2d->SetZoom(Math::Lerp(camera2d->GetZoom(), TargetCameraZoom, static_cast<float>(delta) * 20.0f));
}

void GamePanel2D::ImageInput()
{
    if (const float mouseWheel = ImGui::GetIO().MouseWheel; mouseWheel != 0.0f)
        TargetCameraZoom += mouseWheel * 0.1f * camera2d->GetZoom();

    if (ImGui::GetIO().MouseDown[0])
    {
        const Vector2I position_grid = hexManager->PixelToGrid(
            ImGuiHelper::GetMousePositionInCamera2DWorld(*camera2d, ImGuiHelper::GetMousePositionInItem())
        );
        landUnit[position_grid.x][position_grid.y].Topography.装饰类型A = SelectedTerrainG;
        landUnit[position_grid.x][position_grid.y].Topography.装饰AID = SelectedTileIdx;
    }

    if (ImGui::GetIO().MouseDown[1])
        if (const ImVec2 mouseDelta = ImGui::GetIO().MouseDelta; mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
            camera2d->Transform2D.Position -= Vector2(mouseDelta.x, mouseDelta.y) / camera2d->GetZoom();
}

void GamePanel2D::IterateLandUnit(const std::function<void(LandUnit&, Vector2)>& func) const
{
    for (int y = 0; y < binParser.GetHeight(); y++)
        for (int x = 0; x < binParser.GetWidth(); x++)
            func(landUnit[x][y], hexManager->GridToPixel(Vector2I(x, y)));
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

void GamePanel2D::GenerateOceanVertex() const
{
    // const size_t baseVertexCount = binParser.GetWidth() / 2 * 4 + (binParser.GetWidth() % 2 == 0) ? 1 : 4;
    std::vector<glm::vec4> baseVertex;
    for (int x = 0; x < binParser.GetWidth(); x++)
    {
        if (x % 2 == 1)continue;
        const Vector2 position = hexManager->GridToPixel({x, 0});
        for (int index = 0; index < 4; index++)
        {
            glm::mat4 model(1.0f);
            model = translate(model, glm::vec3(position.x, 0.0f, 0.0));
            model = scale(model, glm::vec3(glm::vec2(HexMetrics::TileWidth / 2.0f), 1.0f));
            baseVertex.push_back(model * glm::vec4(HexMetrics::Corners[index], 1.0f));
        }
    }

    for (int y = 0; y < binParser.GetHeight() + 1; y++)
    {
        for (auto& vertex: baseVertex)
        {
            const Vector2 position = hexManager->GridToPixel({0, y});
            glm::mat4 additionalTransform(1.0f);
            additionalTransform = translate(additionalTransform, glm::vec3(0.0f, position.y, 1.0f));

            vertexInfo->PHexVertexData.push_back(
                {
                    additionalTransform * vertex
                }
            );
        }

        for (size_t x = 0; x < binParser.GetWidth(); x++)
        {
            if (x % 2 == 0)
            {
                vertexInfo->PLineHexVertexIndices.push_back(static_cast<short>(x * 2 + 0));
                vertexInfo->PLineHexVertexIndices.push_back(static_cast<short>(x * 2 + 1));
                vertexInfo->PLineHexVertexIndices.push_back(static_cast<short>(x * 2 + 2));
                vertexInfo->PLineHexVertexIndices.push_back(static_cast<short>(x * 2 + 3));
                vertexInfo->PLineHexVertexIndices.push_back(static_cast<short>((y + 1) * baseVertex.size() + x * 4 + 2));
                vertexInfo->PLineHexVertexIndices.push_back(static_cast<short>((y + 1) * baseVertex.size() + x * 4 + 1));
            }


            /*/*if (binParser.topographies[y * binParser.GetWidth() + x].地块类型 != 1)
                continue;#1#
            const size_t index = (y * binParser.GetWidth() + x) * 6;
            const glm::vec2 position = hexManager->HexToPixel({x, y});
            glm::mat4 model(1.0f);
            model = translate(model, glm::vec3(position.x, position.y, 0.0));
            model = scale(model, glm::vec3(glm::vec2(HexMetrics::TileWidth / 2.0f), 1.0f));
            for (auto Corner: HexMetrics::Corners)
                vertexInfo->PHexVertexData.push_back(
                    {
                        model * glm::vec4(Corner, 1.0f)
                    }
                );*/
        }
    }

    for (int i = 0; i < 6; ++i)
    {
        std::cout << to_string(vertexInfo->PHexVertexData[i].Position) << std::endl;
    }
}

void GamePanel2D::GenerateBuffer() const
{
    std::vector<glm::vec3> offset;
    for (int y = 0; y < binParser.GetHeight() + 1; y++)
    {
        for (int x = 0; x < binParser.GetWidth(); x++)
        {
            const Vector2 position = hexManager->GridToPixel({x, y});
            offset.emplace_back(position.x, position.y, 0.0f);
        }
    }

    GridBufferManager->SetData(nullptr, HexMetrics::Corners, sizeof(HexMetrics::Corners));
    GridBufferManager->SetAttribute(nullptr, 0, 3, 3 * sizeof(float), 0);

    GridBufferManager->AddBuffer("GridOffset", GL_ARRAY_BUFFER);
    GridBufferManager->SetData("GridOffset", offset.data(), offset.size() * sizeof(glm::vec3));
    GridBufferManager->SetAttribute("GridOffset", 1, 3, 3 * sizeof(float), 0, 1);
}

void GamePanel2D::DockingSpace() const
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
        ImGuiID leftNode = ImGui::DockBuilderSplitNode(root, ImGuiDir_Left, 0.25f, nullptr, &root);
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // 去掉窗口边框

    ImGui::Begin(UI_VIEW_BOX, nullptr, window_flags);

    const ImVec2 panelSize = ImGui::GetContentRegionAvail();
    if (static_cast<int>(panelSize.x) != width || static_cast<int>(panelSize.y) != height)
    {
        width = static_cast<int>(panelSize.x);
        height = static_cast<int>(panelSize.y);
        NewFramebuffer(width, height);
    }

    ImGui::Image(
        static_cast<ImTextureID>(static_cast<intptr_t>(renderTarget->textureBuffer)),
        panelSize,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    if (ImGui::IsItemHovered())
    {
        delete MouseSelect;

        MouseSelect = new Vector2(
            hexManager->GetStandardPosition(ImGuiHelper::GetMousePositionInCamera2DWorld(*camera2d, ImGuiHelper::GetMousePositionInItem()))
        );
        ImageInput();
    }
    else
    {
        MouseSelect = nullptr;
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
}

void GamePanel2D::GameTestView()
{
    constexpr ImGuiTabBarFlags flags = ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_DrawSelectedOverline |
                                       ImGuiTabBarFlags_TabListPopupButton;

    ImGui::Begin(UI_VIEW_GAME_TEST);

    ResourceTextureParser& plant_resource_texture = Editor::loadResources->plantResourceTextureParser;
    ResourceTextureParser& terrain_resource_texture = Editor::loadResources->terrainResourceTextureParser;
    const std::vector<Terrain>& terrains = Editor::loadResources->terrainConfigParser.terrains;
    ImGui::BeginTabBar("terrain", flags);
    for (const auto& terrain: terrains)
    {
        if (ImGui::BeginTabItem((terrain.name + std::to_string(terrain.terrainG)).c_str()))
        {
            const float window_visible_x2 = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;

            for (const auto& [idx, image]: terrain.tiles)
            {
                const Texture2D* texture = &plant_resource_texture.Texture2D;
                std::optional<Rect> rect = plant_resource_texture.GetRect(image);
                if (!rect)
                {
                    rect = terrain_resource_texture.GetRect(image);
                    texture = &terrain_resource_texture.Texture2D;
                }
                if (!rect) continue;

                const ImVec2 button_size = rect->GetSize().ToImVec2();
                const Vector2 texture_size = texture->GetSize();

                if (ImGui::ImageButton(
                    image.c_str(),
                    texture->Id,
                    rect->GetSize().ToImVec2(),
                    (rect->GetPosition() / texture_size).ToImVec2(),
                    ((rect->GetPosition() + rect->GetSize()) / texture_size).ToImVec2()
                ))
                {
                    SelectedTerrainG = terrain.terrainG;
                    SelectedTileIdx = idx;
                }

                if (const float next_button_x2 = ImGui::GetItemRectMax().x + button_size.x + ImGui::GetStyle().ItemSpacing.x;
                    next_button_x2 < window_visible_x2)
                    ImGui::SameLine();
            }
            ImGui::EndTabItem();
        }
    }
    ImGui::EndTabBar();

    ImGui::End();
}
