//
// Created by TrifingZW on 24-11-22.
//

#include "game_panel_2d.h"

#include <imgui.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "core/core.h"
#include "game_shader.h"

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
    for (size_t x = 0; x < binParser.GetWidth(); x++)
    {
        landUnit[x] = new LandUnit[binParser.GetHeight()];
        for (size_t y = 0; y < binParser.GetHeight(); y++)
        {
            landUnit[x][y].topography = binParser.topographies[y * binParser.GetWidth() + x];
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

    spriteBatch.Begin(Graphics::SpriteSortMode::Deferred, SamplerState::LinearMirror);
    spriteBatch.Draw(
        Editor::loadResources->mapLand,
        Rect(0, 0, hexManager->GetPixelWidth(), hexManager->GetPixelHeight()),
        Color(1.0f, 1.0f, 1.0f, 1.0f)
    );
    spriteBatch.End();

    spriteBatch.Begin(Graphics::SpriteSortMode::Deferred, camera2d->GetProjectionMatrix());

    IterateLandUnit(
        [&spriteBatch](const LandUnit& landUnit, const glm::vec2& position)
        {
            DrawTerrain(
                spriteBatch,
                landUnit.topography.装饰类型B,
                landUnit.topography.装饰BID,
                position + glm::vec2(landUnit.topography.装饰BX, landUnit.topography.装饰BY)
            );
        }
    );

    IterateLandUnit(
        [&spriteBatch](const LandUnit& landUnit, const glm::vec2& position)
        {
            DrawTerrain(
                spriteBatch,
                landUnit.topography.装饰类型A,
                landUnit.topography.装饰AID,
                position + glm::vec2(landUnit.topography.装饰AX, landUnit.topography.装饰AY)
            );
        }
    );

    IterateLandUnit(
        [&spriteBatch](const LandUnit& landUnit, const glm::vec2& position)
        {
            DrawTerrain(
                spriteBatch,
                landUnit.topography.地块类型,
                landUnit.topography.地块ID,
                position + glm::vec2(landUnit.topography.地块X, landUnit.topography.地块Y)
            );
        }
    );
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); // 设置窗口内边距为 0
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // 去掉窗口边框

    ImGui::Begin("Game2D");

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
        ImageInput();

    ImGui::End();
    ImGui::PopStyleVar(2);


    static float fov = 0.1f;
    /*static float f1 = 0.0f;
    static float f2 = 0.0f;
    static float f3 = 0.0f;
    static float f4 = 0.0f;
    static float f5 = 0.0f;
    static float f6 = 0.0f;
    static float f7 = 0.0f;
    static float f8 = 0.0f;
    static float f9 = 0.0f;
    static auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);*/
    ImGui::Begin("Game Test");
    ImGui::SliderFloat("fov", &fov, 0.1f, 10.0f);
    /*ImGui::SliderFloat("f1", &f1, -10.0f, 10.0f);
    ImGui::SliderFloat("f2", &f2, -10.0f, 10.0f);
    ImGui::SliderFloat("f3", &f3, -10.0f, 10.0f);
    ImGui::SliderFloat("f4", &f4, -10.0f, 10.0f);
    ImGui::SliderFloat("f5", &f5, -10.0f, 10.0f);
    ImGui::SliderFloat("f6", &f6, -10.0f, 10.0f);
    ImGui::SliderFloat("f7", &f7, -10.0f, 10.0f);
    ImGui::SliderFloat("f8", &f8, -10.0f, 10.0f);
    ImGui::SliderFloat("f9", &f9, -10.0f, 10.0f);
    ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&clear_color));*/
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void GamePanel2D::ImageInput() const
{
    if (const float mouseWheel = ImGui::GetIO().MouseWheel; mouseWheel != 0.0f)
    {
        camera2d->SetZoom(camera2d->GetZoom() + mouseWheel * 0.1f);
    }

    if (const ImVec2 mouseDelta = ImGui::GetIO().MouseDelta; mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
    {
        camera2d->SetOffset(glm::vec2(mouseDelta.x, mouseDelta.y));
    }
}

void GamePanel2D::IterateLandUnit(const std::function<void(LandUnit&, glm::vec2)>& func) const
{
    for (size_t y = 0; y < binParser.GetHeight(); y++)
        for (size_t x = 0; x < binParser.GetWidth(); x++)
            func(landUnit[x][y], hexManager->HexToPixel(glm::ivec2(x, y)));
}

void GamePanel2D::DrawTerrain(SpriteBatch& spriteBatch, uint8_t type, uint8_t id, const glm::vec2& position)
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
                    position,
                    rectOpt.value(),
                    Color(1.0f, 1.0f, 1.0f, 1.0f)
                );
            if (const auto rectOpt = terrain_resource_texture.GetRect(tile_iterator->image))
                spriteBatch.DrawCenter(
                    &terrain_resource_texture.Texture2D,
                    position,
                    rectOpt.value(),
                    Color(1.0f, 1.0f, 1.0f, 1.0f)
                );
        }
    }
}

void GamePanel2D::NewFramebuffer(const int width, const int height)
{
    // 设置相机视口大小
    camera2d->SetViewportSize(glm::vec2(static_cast<float>(width), static_cast<float>(height)));
    camera2d->SetZoom(0.1f);

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
        const glm::vec2 position = hexManager->HexToPixel({x, 0});
        for (int index = 0; index < 4; index++)
        {
            glm::mat4 model(1.0f);
            model = translate(model, glm::vec3(position.x, 0.0f, 0.0));
            model = scale(model, glm::vec3(glm::vec2(HexMetrics::TileWidth / 2.0f), 1.0f));
            baseVertex.push_back(model * glm::vec4(HexMetrics::Corners[index], 1.0f));
        }
    }

    for (size_t y = 0; y < binParser.GetHeight() + 1; y++)
    {
        for (auto& vertex: baseVertex)
        {
            const glm::vec2 position = hexManager->HexToPixel({0, y});
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
    for (size_t y = 0; y < binParser.GetHeight() + 1; y++)
    {
        for (size_t x = 0; x < binParser.GetWidth(); x++)
        {
            const glm::vec2 position = hexManager->HexToPixel({x, y});
            offset.emplace_back(position.x, position.y, 0.0f);
        }
    }

    GridBufferManager->SetData(nullptr, HexMetrics::Corners, sizeof(HexMetrics::Corners));
    GridBufferManager->SetAttribute(nullptr, 0, 3, 3 * sizeof(float), 0);

    GridBufferManager->AddBuffer("GridOffset", GL_ARRAY_BUFFER);
    GridBufferManager->SetData("GridOffset", offset.data(), offset.size() * sizeof(glm::vec3));
    GridBufferManager->SetAttribute("GridOffset", 1, 3, 3 * sizeof(float), 0, 1);
}
