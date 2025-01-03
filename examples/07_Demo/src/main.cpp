#include <assert.h>

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <functional>
#include <algorithm>
#include <utility>

#include <optional>
#include <memory>

#include <string>
#include <vector>
#include <queue>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>
#include <iostream>
#include <random>
#include <cmath>
#include <cstdlib>
#include "Andromeda.h"

const float PI = 3.14159265f;

const float Global_Scale = 20.0f;

static And::Entity* point_tv;

static And::Entity* spot_estanteria0;
static And::Entity* spot_estanteria1;
static And::Entity* spot_estanteria2;

static And::Entity* spheres_disco[3];
static And::Entity* spot_cuadro[3];
static And::Entity* points_habitaculo[4];
static And::Entity* point_hexagonos;
static And::Entity* tubos_entity;

static And::Audio audio_tele;
static And::Audio audio_lapiz;
static And::Audio audio_teclado;

static std::vector<And::Entity*> billboards;
static std::vector<And::MaterialComponent> materiales;


void LoadMaterials(std::string file) {

    And::MaterialComponent mat_com;
    std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> tex = And::MakeTexture(file.c_str());
    std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/titanio/normals.png");
    std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/titanio/ao.png");
    std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/titanio/metallic.png");
    std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/titanio/ao.png");
    mat->SetColorTexture(tex);
    mat->SetNormalTexture(normals);
    mat->SetAmbientOclusionTexture(ao);
    mat->SetMetallicTexture(metallic);
    mat->SetRoughnessTexture(rou);
    mat_com.SetMaterial(mat);

    materiales.push_back(mat_com);
}

void AddBillBoardAtLocation(And::EntityComponentSystem& ecs, float x, float y, float z, And::MaterialComponent& mat_com) {

    std::shared_ptr<And::BillBoardComponent> billboard_comp = std::make_shared<And::BillBoardComponent>();

    std::shared_ptr<And::BillBoard> bill = std::make_shared<And::BillBoard>();
    bill->SetMaterial(std::make_shared<And::MaterialComponent>(mat_com));
    billboard_comp->SetBillBoard(bill);
    billboard_comp->bScale[0] = 4.0f;
    billboard_comp->bScale[1] = 4.0f;

    And::TransformComponent tr_com;
    tr_com.position[0] = x;
    tr_com.position[1] = y;
    tr_com.position[2] = z;
    billboards.push_back(ecs.new_entity(tr_com, *billboard_comp));

}

void CreateHabitaculo(And::EntityComponentSystem& ecs, And::Entity* parent) {

    const float scale_suelo = 75.0f;
    
    And::MaterialComponent mat_com;
    std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/suelo_3/albedo.png");
    std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/suelo_3/normals.png");
    std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/suelo_3/ao.png");
    std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/suelo_3/metallic.png");
    std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/suelo_3/roughness.png");
    mat->SetColorTexture(tex);
    mat->SetNormalTexture(normals);
    mat->SetAmbientOclusionTexture(ao);
    mat->SetMetallicTexture(metallic);
    mat->SetRoughnessTexture(rou);
    mat_com.SetMaterial(mat);

    And::MeshComponent MC;
    MC.MeshOBJ = And::Geometry::load("cube.obj");
    And::RawMesh raw_mesh_vela_tmp(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
    std::shared_ptr<And::Mesh> mesh_vela_tmp = std::make_shared<And::Mesh>(raw_mesh_vela_tmp);
    MC.SetMesh(mesh_vela_tmp);

    And::TransformComponent tr;
    tr.SetPosition(0.0f, 0.0f, 0.0f);
    tr.SetRotation(0.0f, 0.0f, 0.0f);
    tr.SetScale(scale_suelo, 1.0f, scale_suelo * 1.5f);
    tr.HasRigidBody(false);
    //tr.SetParent(parent->get_component<And::TransformComponent>());
    ecs.new_entity(mat_com, MC, tr);


    And::MaterialComponent mat_com_pared;
    std::shared_ptr<And::Material> mat_pared = std::make_shared<And::Material>();
    std::shared_ptr<And::Texture> tex_pared = And::MakeTexture("demo/textures/suelo_7/albedo.png");
    std::shared_ptr<And::Texture> normals_pared = And::MakeTexture("demo/textures/suelo_7/normals.png");
    std::shared_ptr<And::Texture> ao_pared = And::MakeTexture("demo/textures/suelo_7/ao.png");
    std::shared_ptr<And::Texture> metallic_pared = And::MakeTexture("demo/textures/suelo_7/metallic.png");
    std::shared_ptr<And::Texture> rou_pared = And::MakeTexture("demo/textures/suelo_7/roughness.png");
    mat_pared->SetColorTexture(tex_pared);
    mat_pared->SetNormalTexture(normals_pared);
    mat_pared->SetAmbientOclusionTexture(ao_pared);
    mat_pared->SetMetallicTexture(metallic_pared);
    mat_pared->SetRoughnessTexture(rou_pared);
    mat_com_pared.SetMaterial(mat_pared);

    
    tr.SetScale(1.0f, 30.0f, scale_suelo * 1.5f);
    tr.SetPosition(scale_suelo * -0.5f, 15.0f, 0.0f);
    //tr.SetParent(parent->get_component<And::TransformComponent>());
    ecs.new_entity(mat_com_pared, MC, tr);
    
    tr.SetScale(1.0f, 30.0f, scale_suelo * 1.5f);
    tr.SetPosition(scale_suelo * 0.5f, 15.0f, 0.0f);
    //tr.SetParent(parent->get_component<And::TransformComponent>());
    ecs.new_entity(mat_com_pared, MC, tr);
    
    tr.SetScale(scale_suelo, 30.0f, 1.0f);
    tr.SetPosition(0.0f, 15.0f, scale_suelo * 0.75f);
    //tr.SetParent(parent->get_component<And::TransformComponent>());
    ecs.new_entity(mat_com_pared, MC, tr);



}

void CreateLighting(And::EntityComponentSystem& ecs) {

    And::AmbientLight ambient;
    ambient.SetDiffuseColor(1.0f, 1.0f, 1.0f);
    ambient.SetAmbientStrenght(0.001f);
    //ecs.new_entity(ambient);

    const float esquina_x = 30.0f;
    const float esquina_z = 49.0f;
    

    // Point esquina
    {
        const float intensity = 100.0f;
        And::PointLight point;
        point.SetPosition(esquina_x, 26.0f, esquina_z);
        AddBillBoardAtLocation(ecs, esquina_x, 26.0f, esquina_z, materiales[0]);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        points_habitaculo[0] = ecs.new_entity(point);
    }
    
    // Point esquina2
    {
        const float intensity = 100.0f;
        And::PointLight point;
        point.SetPosition(-esquina_x, 26.0f, -esquina_z);
        AddBillBoardAtLocation(ecs, -esquina_x, 26.0f, -esquina_z, materiales[0]);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        points_habitaculo[1] = ecs.new_entity(point);
    }
    
    // Point esquina3
    {
        const float intensity = 100.0f;
        And::PointLight point;
        AddBillBoardAtLocation(ecs, esquina_x, 26.0f, -esquina_z, materiales[0]);
        point.SetPosition(esquina_x, 26.0f, -esquina_z);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        points_habitaculo[2] = ecs.new_entity(point);
    }
    
    // Point esquina4
    {
        const float intensity = 100.0f;
        And::PointLight point;
        point.SetPosition(-esquina_x, 26.0f, esquina_z);
        //AddBillBoardAtLocation(ecs, -esquina_x, 26.0f, esquina_z, materiales[0]);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        //ecs.new_entity(point);
    }
    
    // Point medio
    {
        const float intensity = 100.0f;
        And::PointLight point;
        point.SetPosition(10.0f, 26.0f, -15.0f);
        AddBillBoardAtLocation(ecs, 10.0f, 26.0f, -15.0f, materiales[0]);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(32.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.09f);
        point.SetQuadraticAtt(0.032f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        points_habitaculo[3] = ecs.new_entity(point);
    }

    // Point detras tele
    {
        const float intensity = 200.0f;
        And::PointLight point;
        point.SetPosition(34.5f,9.0f, -15.0f);
        AddBillBoardAtLocation(ecs, 34.5f, 9.0f, -15.0f, materiales[0]);
        point.SetSpecularColor(0.0f, 0.0f, 0.0f);
        point.SetSpecularShininess(2.0f);
        point.SetSpecularStrength(0.0f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.0f);
        point.SetQuadraticAtt(0.0f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetIntensity(intensity);
        point.SetDiffuseColor(1.0f, 0.01f, 0.01f);
        point_tv = ecs.new_entity(point);
    }

    // Directional
    {
        And::DirectionalLight directional;
        directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        directional.SetDirection(0.5f, -0.5f, 0.5f);
        directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
        directional.SetSpecularShininess(32.0f);
        directional.SetSpecularStrength(0.003f);
        directional.SetEnabled(true);
        directional.SetCastShadows(true);
        directional.SetIntensity(1.0f);
        // ecs.new_entity(directional);
    }
    
    const float distance_cuadros = 20.0f;
    // Spot cuadro 1
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(-35.5f, 32.0f, distance_cuadros);
        AddBillBoardAtLocation(ecs, -35.5f, 32.0f, distance_cuadros, materiales[1]);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        //spot.SetDiffuseColor(0.976f, 0.518f, 0.012f);
        spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(17.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_cuadro[0] = ecs.new_entity(spot);
    }
    
    // Spot cuadro 2
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(-35.5f, 32.0f, 0.0f);
        AddBillBoardAtLocation(ecs, -35.5f, 32.0f, 0.0f, materiales[1]);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        //spot.SetDiffuseColor(0.976f, 0.518f, 0.012f);
        spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(17.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_cuadro[1] = ecs.new_entity(spot);
    }
    
    // Spot cuadro 3
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(-35.5f, 32.0f, -distance_cuadros);
        AddBillBoardAtLocation(ecs, -35.5f, 32.0f, -distance_cuadros, materiales[1]);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        //spot.SetDiffuseColor(0.976f, 0.518f, 0.012f);
        spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(17.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_cuadro[2] = ecs.new_entity(spot);
    }
    
    // Spot lamp
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(-9.5f, 12.5f, 52.0f);
        AddBillBoardAtLocation(ecs, -9.5f, 12.5f, 52.0f, materiales[1]);
        spot.SetDirection(-1.0f, -0.5f, 0.0f);
        spot.SetDiffuseColor(0.976f, 0.518f, 0.3f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        ecs.new_entity(spot);
    }
    
    // Spot estanteria
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        spot.SetPosition(35.0f, 31.0f, 43.0f);
        AddBillBoardAtLocation(ecs, 35.0f, 31.0f, 43.0f, materiales[1]);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        spot.SetDiffuseColor(0.976f, 0.518f, 0.3f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_estanteria0 = ecs.new_entity(spot);
    }

    // Spot estanteria 2
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        //43.0f - (scale_z * 1.5f)
        spot.SetPosition(35.0f, 31.0f,43.0f - (8.0f * 1.5f));
        AddBillBoardAtLocation(ecs, 35.0f, 31.0f, 43.0f - (8.0f * 1.5f), materiales[1]);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        spot.SetDiffuseColor(0.976f, 0.518f, 0.3f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_estanteria1 = ecs.new_entity(spot);
    }

    // Spot estanteria 3
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        //43.0f - (scale_z * 1.5f)
        spot.SetPosition(35.0f, 31.0f, 43.0f - (8.0f * 3.0f));
        AddBillBoardAtLocation(ecs, 35.0f, 31.0f, 43.0f - (8.0f * 3.0f), materiales[1]);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        spot.SetDiffuseColor(0.976f, 0.518f, 0.3f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        spot_estanteria2 = ecs.new_entity(spot);
    }
    
    // Spot hexagon tiles
    {
        And::SpotLight spot{};
        //spot.SetPosition(33.0f, 20.0f, 0.0f);
        //43.0f - (scale_z * 1.5f)
        spot.SetPosition(35.0f, 31.0f, -39.0f );
        //AddBillBoardAtLocation(ecs, 35.0f, 31.0f, 43.0f - (8.0f * 3.0f), materiales[1]);
        spot.SetDirection(0.0f, -1.0f, 0.0f);
        spot.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularColor(1.0f, 1.0f, 1.0f);
        spot.SetSpecularStrength(0.003f);
        spot.SetSpecularShininess(32.0f);
        spot.SetConstantAtt(1.0f);
        spot.SetLinearAtt(0.7f);
        spot.SetQuadraticAtt(1.8f);
        spot.SetCuttOff(2.5f);
        spot.SetOuterCuttOff(30.0f);
        spot.SetCastShadows(true);
        spot.SetEnabled(true);
        spot.SetIntensity(300.0f);
        //spot_hexagonos = ecs.new_entity(spot);
    }


   
    // Point hexagonos
    {
        const float intensity = 200.0f;
        And::PointLight point;
        point.SetPosition(25.0f, 17.0f, -39.0f);
        AddBillBoardAtLocation(ecs, 25.0f, 17.0f, -39.0f, materiales[0]);
        point.SetSpecularColor(1.0f, 1.0f, 1.0f);
        point.SetSpecularShininess(8.0f);
        point.SetSpecularStrength(0.003f);
        point.SetConstantAtt(1.0f);
        point.SetLinearAtt(0.0f);
        point.SetQuadraticAtt(0.0f);
        point.SetEnabled(true);
        point.SetCastShadows(true);
        point.SetDiffuseColor(1.0f, 1.0f, 1.0f);
        point.SetIntensity(intensity);
        point_hexagonos = ecs.new_entity(point);
    }

}

void CreateFurnitures(And::EntityComponentSystem& ecs, And::Entity* parent, And::TaskSystem& ts){
    
  And::Future<std::shared_ptr<And::Geometry>> office_desk = ts.AddTask(&And::Geometry::load, "demo/obj/office_desk.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> guitar = ts.AddTask(&And::Geometry::load, "demo/obj/guitar.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> cube = ts.AddTask(&And::Geometry::load, "cube.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> cube2 = ts.AddTask(&And::Geometry::load, "cube.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> cube3 = ts.AddTask(&And::Geometry::load, "cube.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> tv = ts.AddTask(&And::Geometry::load, "demo/obj/tv.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> sofa = ts.AddTask(&And::Geometry::load, "demo/obj/furniture/sofa.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> display_table = ts.AddTask(&And::Geometry::load, "demo/obj/furniture/display_table.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> desk = ts.AddTask(&And::Geometry::load, "demo/obj/office/desk.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> chair = ts.AddTask(&And::Geometry::load, "demo/obj/office/chair.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> lamp = ts.AddTask(&And::Geometry::load, "demo/obj/office/lamp.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> calendar = ts.AddTask(&And::Geometry::load, "demo/obj/office/calendar.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> tubos = ts.AddTask(&And::Geometry::load, "demo/obj/office/tubos.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> pencil = ts.AddTask(&And::Geometry::load, "demo/obj/office/pencil.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> keyboard = ts.AddTask(&And::Geometry::load, "demo/obj/keyboard.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> mouse = ts.AddTask(&And::Geometry::load, "demo/obj/mouse.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> taza = ts.AddTask(&And::Geometry::load, "demo/obj/taza.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> sphere = ts.AddTask(&And::Geometry::load, "sphere.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> cube4 = ts.AddTask(&And::Geometry::load, "cube.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> sphere2 = ts.AddTask(&And::Geometry::load, "sphere.obj", "");
  And::Future<std::shared_ptr<And::Geometry>> hexagon_tile = ts.AddTask(&And::Geometry::load, "demo/obj/furniture/hexagon_tile.obj", "");

    // Mesa 
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bambu/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bambu/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bambu/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bambu/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bambu/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        office_desk.Wait();
        MC.MeshOBJ = office_desk.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(33.0f, 1.0f, -15.0f);
        tr.SetRotation(0.0f,PI / 2.0f, 0.0f);
        tr.SetScale(15.0f, 5.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
    }

    // Guitarra 
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/suelo_5/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/suelo_5/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/suelo_5/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/suelo_5/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/suelo_5/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        guitar.Wait();
        MC.MeshOBJ = guitar.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-34.0f, 1.0f, 50.5f);
        tr.SetRotation(PI / -8.0f, PI, 0.0f);
        tr.SetScale(12.5f, 12.5f, 12.5f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    const float distance_cuadros = 20.0f;
    // Poster 1
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/poster/albedo_esat.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/poster/normals_raw.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/poster/ao_raw.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/poster/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/poster/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        cube.Wait();
        MC.MeshOBJ = cube.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-36.5f, 15.0f, distance_cuadros);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(0.3f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    // Poster 2
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/poster/albedo_ochy_raw.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/poster/normals_raw.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/poster/ao_raw.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/poster/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/poster/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        cube2.Wait();
        MC.MeshOBJ = cube2.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-36.5f, 15.0f, 0.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(0.3f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    // Poster 3
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/poster/albedo_xema_raw.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/poster/normals_raw.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/poster/ao_raw.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/poster/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/poster/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        cube3.Wait();
        MC.MeshOBJ = cube3.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-36.5f, 15.0f, -distance_cuadros);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(0.3f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // TV
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/titanio/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/titanio/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/titanio/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/titanio/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/titanio/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        tv.Wait();
        MC.MeshOBJ = tv.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(33.0f, 4.75f, -15.0f);
        tr.SetRotation(0.0f, PI * -0.5f, 0.0f);
        tr.SetScale(25.0f, 20.0f, 20.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Sofa 1
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/leather/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/leather/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/leather/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/leather/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/leather/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        sofa.Wait();
        MC.MeshOBJ = sofa.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-10.0f, 0.0f, -15.0f);
        tr.SetRotation(0.0f,PI * -0.5f, 0.0f);
        tr.SetScale(0.8f, 0.8f, 0.8f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    // Display table
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bambu/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bambu/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bambu/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bambu/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bambu/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        display_table.Wait();
        MC.MeshOBJ = display_table.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(10.0f, 0.0f, -15.0f);
        tr.SetRotation(0.0f,PI * -0.5f, 0.0f);
        tr.SetScale(12.0f, 6.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office desk
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/wood/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/wood/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/wood/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/wood/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/wood/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        desk.Wait();
        MC.MeshOBJ = desk.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-17.0f, 0.0f, 50.5f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(10.0f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office chair
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/grip/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/grip/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/grip/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/grip/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/grip/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        chair.Wait();
        MC.MeshOBJ = chair.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-17.0f, 0.0f, 45.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(12.0f, 12.0f, 12.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office lamp
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/aluminio/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/aluminio/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/aluminio/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/aluminio/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/aluminio/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        lamp.Wait();
        MC.MeshOBJ = lamp.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-9.5f, 9.1f, 52.0f);
        tr.SetRotation(0.0f, PI * -0.5f, 0.0f);
        tr.SetScale(12.0f, 12.0f, 12.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office calendar
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/paper/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/paper/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/paper/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/paper/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/paper/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        calendar.Wait();
        MC.MeshOBJ = calendar.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-25.0f, 9.1f, 52.0f);
        tr.SetRotation(0.0f, PI * 0.75f, 0.0f);
        tr.SetScale(12.0f, 12.0f, 12.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office tubos
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/metal_dented/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/metal_dented/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/metal_dented/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/metal_dented/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/metal_dented/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        tubos.Wait();
        MC.MeshOBJ = tubos.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(-21.0f, 9.1f, 51.0f);
        tr.SetRotation(0.0f, PI * 0.75f, 0.0f);
        tr.SetScale(14.0f, 14.0f, 14.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        tubos_entity = ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // office pencil
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/wood/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/wood/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/wood/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/wood/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/wood/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        pencil.Wait();
        MC.MeshOBJ = pencil.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(0.175f, 0.04f, 0.0f);
        tr.SetRotation(PI * 0.5f, 0.0f, PI * 0.25f);
        tr.SetScale(1.0f, 1.0f, 1.0f);
        tr.HasRigidBody(false);
        tr.SetParent(tubos_entity);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Living room keyboard
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/worn_metal/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/worn_metal/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/worn_metal/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/worn_metal/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/worn_metal/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        keyboard.Wait();
        MC.MeshOBJ = keyboard.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(9.0f, 5.0f, -16.0f);
        tr.SetRotation(0.0f, PI * 1.70f, 0.0f);
        tr.SetScale(16.0f, 16.0f, 16.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }
    
    // Living room mouse 
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/aluminio/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/aluminio/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/aluminio/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/aluminio/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/aluminio/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        mouse.Wait();
        MC.MeshOBJ = mouse.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(9.0f, 5.0f, -10.0f);
        tr.SetRotation(0.0f, PI * -0.5f, 0.0f);
        tr.SetScale(16.0f, 16.0f, 16.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Living room taza
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/iron/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/iron/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/iron/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/iron/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/iron/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        taza.Wait();
        MC.MeshOBJ = taza.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(7.0f, 5.0f, -10.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(16.0f, 16.0f, 16.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Living room cafe
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/shiny_metal/albedo_white.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/shiny_metal/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/shiny_metal/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/shiny_metal/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/shiny_metal/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        sphere.Wait();
        MC.MeshOBJ = sphere.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(7.0f, 6.2f, -10.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(0.7f, 0.2f, 0.7f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());
    }

    // Estanteria pared
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/bambu/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/bambu/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/bambu/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/bambu/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/bambu/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        cube4.Wait();
        MC.MeshOBJ = cube4.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        const float scale_x = 4.0f;
        const float scale_z = 8.0f;
        const float pos_y = 20.0f;
        const float diference_y = 3.5f;

        And::TransformComponent tr;
        tr.SetPosition(35.0f, pos_y, 43.0f);
        tr.SetRotation(0.0f, 0.0f, 0.0f);
        tr.SetScale(scale_x, 0.25f, scale_z);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        ecs.new_entity(mat_com, MC, tr);
        //ecs.new_entity(mat_com, MC, tr)->get_component<And::TransformComponent>()->SetParent(parent->get_component<And::TransformComponent>());

        tr.SetPosition(35.0f, pos_y - diference_y, 43.0f - (scale_z * 1.5f));
        ecs.new_entity(mat_com, MC, tr);
        
        tr.SetPosition(35.0f, pos_y - (diference_y * 2.0f), 43.0f - (scale_z * 3.0f));
        ecs.new_entity(mat_com, MC, tr);
    }

    // Estanteria sphere
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/metal_circulos/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/metal_circulos/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/metal_circulos/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/metal_circulos/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/metal_circulos/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        sphere2.Wait();
        MC.MeshOBJ = sphere2.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        const float scale_x = 4.0f;
        const float scale_z = 8.0f;
        const float pos_y = 22.0f;
        const float diference_y = 3.5f;

        And::TransformComponent tr;
        tr.SetPosition(35.0f, pos_y, 43.0f);
        tr.SetRotation(0.0f, PI * -0.5f, 0.0f);
        tr.SetScale(2.0f, 2.0f, 2.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        spheres_disco[0] = ecs.new_entity(mat_com, MC, tr);

        tr.SetPosition(35.0f, pos_y - diference_y, 43.0f - (8.0f * 1.5f));
        spheres_disco[1] = ecs.new_entity(mat_com, MC, tr);
        
        tr.SetPosition(35.0f, pos_y - (diference_y * 2.0f), 43.0f - (8.0f * 3.0f));
        spheres_disco[2] = ecs.new_entity(mat_com, MC, tr);


    }

    // Living room hexagon tiles
    {
        And::MaterialComponent mat_com;
        std::shared_ptr<And::Material> mat = std::make_shared<And::Material>();
        std::shared_ptr<And::Texture> tex = And::MakeTexture("demo/textures/fibra_carbono/albedo.png");
        std::shared_ptr<And::Texture> normals = And::MakeTexture("demo/textures/fibra_carbono/normals.png");
        std::shared_ptr<And::Texture> ao = And::MakeTexture("demo/textures/fibra_carbono/ao.png");
        std::shared_ptr<And::Texture> metallic = And::MakeTexture("demo/textures/fibra_carbono/metallic.png");
        std::shared_ptr<And::Texture> rou = And::MakeTexture("demo/textures/fibra_carbono/roughness.png");
        mat->SetColorTexture(tex);
        mat->SetNormalTexture(normals);
        mat->SetAmbientOclusionTexture(ao);
        mat->SetMetallicTexture(metallic);
        mat->SetRoughnessTexture(rou);
        mat_com.SetMaterial(mat);

        And::MeshComponent MC;
        hexagon_tile.Wait();
        MC.MeshOBJ = hexagon_tile.Get();
        And::RawMesh raw_mesh_mesa(MC.MeshOBJ->get_vertices(), MC.MeshOBJ->get_indices());
        std::shared_ptr<And::Mesh> mesh = std::make_shared<And::Mesh>(raw_mesh_mesa);
        MC.SetMesh(mesh);

        And::TransformComponent tr;
        tr.SetPosition(37.0f, 19.0f, -38.0f);
        tr.SetRotation(0.0f, PI * 0.5f, PI * 0.5f);
        tr.SetScale(10.0f, 10.0f, 10.0f);
        tr.HasRigidBody(false);
        //tr.SetParent(parent->get_component<And::TransformComponent>());
        //ecs.new_entity(mat_com, MC, tr);

        const int tile_num = 7;
        const int tile_files = 2;

        float offset_y = 1.3f;
        float offset_z = 0.0f;

        float tmp = 0.0f;

        float index = 0.2f;

        for (int j = -tile_files; j <= tile_files; j++) {
            for (int i = 0; i < tile_num; i++) {
                tr.SetPosition(37.0f, (14.0f - (2.1f * j)) + offset_y, ( -48.0f + (2.4f * i)) + std::abs(j) + (index * std::abs(j)));
                ecs.new_entity(mat_com, MC, tr);

                tmp += 0.1f;
                //index += 0.2f;
            }
            //index = 0.0f;
            tmp = 0.0f;
            offset_y += 0.1f;
            offset_z += 0.9f;
        }
    }
}

void CreateAudios(And::EntityComponentSystem& ecs) {

    audio_tele.load("demo/audio/jazz_estereo.wav");
    audio_tele.SetPosition(33.0f, 5.0f, -15.0f);
    AddBillBoardAtLocation(ecs, 33.0f, 5.0f, -15.0f, materiales[2]);
    audio_tele.SetGain(1.0f);
    audio_tele.SetDoppler(true);
    audio_tele.SetDopplerFactor(10.0f);
    audio_tele.SetLooping(true);
    audio_tele.SetPitch(1.0f);
    audio_tele.SetMaxDistance(30.0f);
    

    audio_lapiz.load("demo/audio/lapiz.wav");
    audio_lapiz.SetPosition(-16.0f, 10.0f, 50.0f);
    AddBillBoardAtLocation(ecs, -16.0f, 10.0f, 50.0f, materiales[2]);
    audio_lapiz.SetGain(1.0f);
    audio_lapiz.SetDoppler(true);
    audio_lapiz.SetDopplerFactor(5.0f);
    audio_lapiz.SetLooping(true);
    audio_lapiz.SetPitch(1.0f);
    audio_lapiz.SetMaxDistance(20.0f);
    
    audio_teclado.load("demo/audio/teclado.wav");
    audio_teclado.SetPosition(9.0f, 5.0f, -16.0f);
    AddBillBoardAtLocation(ecs, 9.0f, 5.0f, -16.0f, materiales[2]);
    audio_teclado.SetGain(1.0f);
    audio_teclado.SetDoppler(true);
    audio_teclado.SetDopplerFactor(10.0f);
    audio_teclado.SetLooping(true);
    audio_teclado.SetPitch(1.0f);
    audio_teclado.SetMaxDistance(15.0f);
}

int main(int argc, char** argv){

    And::Engine e;
    And::TaskSystem ts;

    {
      And::WorkerCreationInfo workerCreationInfo;
      workerCreationInfo.Name = "Worker 1";
      workerCreationInfo.Function = And::GetGenericWorkerFunction();
      workerCreationInfo.UserData = nullptr;

      ts.AddWorker(workerCreationInfo);
    }

    {
      And::WorkerCreationInfo workerCreationInfo;
      workerCreationInfo.Name = "Worker 2";
      workerCreationInfo.Function = And::GetGenericWorkerFunction();
      workerCreationInfo.UserData = nullptr;

      ts.AddWorker(workerCreationInfo);
    }

    {
      And::WorkerCreationInfo workerCreationInfo;
      workerCreationInfo.Name = "Worker 3";
      workerCreationInfo.Function = And::GetGenericWorkerFunction();
      workerCreationInfo.UserData = nullptr;

      ts.AddWorker(workerCreationInfo);
    }

    {
      And::WorkerCreationInfo workerCreationInfo;
      workerCreationInfo.Name = "Worker 4";
      workerCreationInfo.Function = And::GetGenericWorkerFunction();
      workerCreationInfo.UserData = nullptr;

      ts.AddWorker(workerCreationInfo);
    }

    {
      And::WorkerCreationInfo workerCreationInfo;
      workerCreationInfo.Name = "Worker 5";
      workerCreationInfo.Function = And::GetGenericWorkerFunction();
      workerCreationInfo.UserData = nullptr;

      ts.AddWorker(workerCreationInfo);
    }

    {
      And::WorkerCreationInfo workerCreationInfo;
      workerCreationInfo.Name = "Worker 6";
      workerCreationInfo.Function = And::GetGenericWorkerFunction();
      workerCreationInfo.UserData = nullptr;

      ts.AddWorker(workerCreationInfo);
    }

    {
      And::WorkerCreationInfo workerCreationInfo;
      workerCreationInfo.Name = "Worker 7";
      workerCreationInfo.Function = And::GetGenericWorkerFunction();
      workerCreationInfo.UserData = nullptr;

      ts.AddWorker(workerCreationInfo);
    }

    {
      And::WorkerCreationInfo workerCreationInfo;
      workerCreationInfo.Name = "Worker 8";
      workerCreationInfo.Function = And::GetGenericWorkerFunction();
      workerCreationInfo.UserData = nullptr;

      ts.AddWorker(workerCreationInfo);
    }

    std::shared_ptr<And::Window> window = And::Window::make(e, 1920, 1080, "Andromeda Engine", And::EGraphicsApiType::OpenGL);
    //window->set_vsync(true);
    std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
    std::shared_ptr<And::Renderer> g_renderer = And::Renderer::CreateShared(*window);

    std::vector<std::string> paths = { {"demo/skybox/lago/right.jpg"}, {"demo/skybox/lago/left.jpg"}, {"demo/skybox/lago/top.jpg"}, {"demo/skybox/lago/bottom.jpg"}, {"demo/skybox/lago/front.jpg"}, {"demo/skybox/lago/back.jpg"} };
    std::shared_ptr<And::SkyboxTexture> sky_box = And::MakeSkyboxTexture(paths);
    g_renderer->set_skybox_texture(sky_box);
    g_renderer->enable_skybox(true);

    And::FlyCamera fly_cam{*window};
    
    {
        fly_cam.SetPosition(-5.0f, 10.0f, 10.0f);
        fly_cam.SetSize(1920.0f, 1080.0f);
        fly_cam.SetFar(1000.0f);
        fly_cam.SetNear(0.1f);
        fly_cam.SetPosition(0.0f, 5.0f, 10.0f);
        fly_cam.SetFov(90.0f);
        fly_cam.SetDirection(0.0f, 0.0f, -1.0f);
        fly_cam.SetSpeed(10.0f);
        g_renderer->set_camera(&fly_cam);
    }
  
    And::Editor editor{*window};

    editor.AddWindow(ts.GetEditorWindow());

    // Show pc info
    g_context->create_info();
    float clear_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    g_renderer->set_clear_color(clear_color);
    And::EntityComponentSystem entity_comp;
    And::AddBasicComponents(entity_comp);

    //std::shared_ptr<And::PhysicsEngine> physics_engine = And::PhysicsEngine::Init(false, 8192);

  {

      And::TransformComponent scene;
      scene.SetPosition(0.0f, 0.0f, 0.0f);
      scene.SetScale(1.0f, 1.0f, 1.0f);
      scene.SetRotation(0.0f, 0.0f, 0.0f);

      And::Entity* scene_entity = entity_comp.new_entity(scene);
      
      LoadMaterials("billboard_bulb.png");
      LoadMaterials("billboard_spot.png");
      LoadMaterials("billboard_audio.png");

      CreateHabitaculo(entity_comp, scene_entity);
      CreateLighting(entity_comp);
      CreateFurnitures(entity_comp, scene_entity, ts);
    
  }

  And::AudioManager audio_manager;
  

  CreateAudios(entity_comp);

  audio_manager.play(audio_lapiz);
  audio_manager.play(audio_teclado);


  And::Input input{ *window };
  And::ActionInput light_tv{ "LightTV", And::KeyState::Press, { And::KeyCode::T} };
  And::ActionInput billboards_input{ "BillBoard", And::KeyState::Press, { And::KeyCode::B} };
  
  bool is_light_tv = false;
  bool is_bilboard_input= false;

  bool change_light = false;
  bool active_bilboards = true;

  float time = 0.0f;
  float fps_count = 0.0f;
  while (window->is_open()){

    //auto start_time = std::chrono::steady_clock::now();
    
    window->update();
    g_renderer->new_frame();
    editor.ShowWindows();
  
    fly_cam.ProcessInput();
    fly_cam.ShowValues();
    
    // Code Here


    if (input.check_action(light_tv)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_light_tv) {
            change_light = !change_light;
        }
        is_light_tv = true;
    }
    else {
        is_light_tv = false;
    }
    
    if (input.check_action(billboards_input)) {

        //static void LaunchBall(const float* pos, const float* dir, float force, And::EntityComponentSystem & ecs, And::PhysicsEngine & engine, And::MeshComponent & mesh, And::MaterialComponent & material_comp)
        if (!is_bilboard_input) {
            active_bilboards = !active_bilboards;
        }
        is_bilboard_input = true;
    }else {
        is_bilboard_input = false;
    }

    for (And::Entity* e : billboards) {
        e->get_component<And::BillBoardComponent>()->ActiveBillBoard(active_bilboards);
    }



    if (change_light) {

        audio_manager.play(audio_tele);
        audio_manager.stop(audio_teclado);
        And::PointLight* p = point_tv->get_component<And::PointLight>();
        const float speed = 0.2f;
        //float r = std::abs(sinf(time * speed));
        float r = 1.0f;
        float g = (cosf(time * speed) * 0.5f) + 0.5f;
        float b = 0.1f;
        //float b = std::abs(cosf(time * 0.7f * speed));
        
        p->SetDiffuseColor(r, g, b);

        //printf("R %f G %f B %f\n", r, g, b);
        p->SetIntensity(300.0f);

        for (int i = 0; i < 4; i++) {
            points_habitaculo[i]->get_component<And::PointLight>()->SetEnabled(false);
        }
        And::SpotLight* spot = spot_estanteria0->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 0.0f, 0.0f);
        spot->SetIntensity(300.0f);

        spot = spot_estanteria1->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 0.0f, 0.0f);
        spot->SetIntensity(300.0f);

        spot = spot_estanteria2->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 0.0f, 0.0f);
        spot->SetIntensity(300.0f);

        for (int i = 0; i < 3; i++) {
            spheres_disco[i]->get_component<And::TransformComponent>()->SetRotation(0.0f, time, 0.0f);
            spot_cuadro[i]->get_component<And::SpotLight>()->SetEnabled(true);
        }

        point_hexagonos->get_component<And::PointLight>()->SetDiffuseColor(1.0f, 0.0f, 0.0f);
        point_hexagonos->get_component<And::PointLight>()->SetIntensity(100.0f);

        
    } else {
        audio_manager.stop(audio_tele);
        audio_manager.play(audio_teclado);

        And::PointLight* p = point_tv->get_component<And::PointLight>();
        //p->SetIntensity(0.0f);
        //p->SetIntensity(1.0f);
        p->SetDiffuseColor(1.0f, 0.01f, 0.01f);
        p->SetIntensity(400.0f);
        for (int i = 0; i < 4; i++) {
            points_habitaculo[i]->get_component<And::PointLight>()->SetEnabled(true);
        }

        And::SpotLight* spot = spot_estanteria0->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot->SetIntensity(300.0f);

        spot = spot_estanteria1->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot->SetIntensity(300.0f);
        
        spot = spot_estanteria2->get_component<And::SpotLight>();
        spot->SetDiffuseColor(1.0f, 1.0f, 1.0f);
        spot->SetIntensity(300.0f);

        for (int i = 0; i < 3; i++) {
            spot_cuadro[i]->get_component<And::SpotLight>()->SetEnabled(false);
        }

        point_hexagonos->get_component<And::PointLight>()->SetDiffuseColor(1.0f, 1.0f, 1.0f);
        point_hexagonos->get_component<And::PointLight>()->SetIntensity(300.0f);
        

    }


    tubos_entity->get_component<And::TransformComponent>()->SetRotation(0.0f, PI + sinf(time), 0.0f);
    const float* src_pos = fly_cam.GetPosition();
    const float* src_dir = fly_cam.GetDirection();

    audio_tele.UpdateListenerPosition(src_pos);
    audio_teclado.UpdateListenerPosition(src_pos);
    audio_lapiz.UpdateListenerPosition(src_pos);

    audio_tele.UpdateListenerDirection(src_dir);
    audio_teclado.UpdateListenerDirection(src_dir);
    audio_lapiz.UpdateListenerDirection(src_dir);
    
    audio_tele.ApplyEffects();
    audio_teclado.ApplyEffects();
    audio_lapiz.ApplyEffects();

    audio_manager.Update();
  

    //physics_engine->Simulate(window->get_delta_time() > 1.0f ? 1.0f / 30.0f : window->get_delta_time());
    //physics_engine->Apply(entity_comp);
    
    //g_renderer->draw_forward(entity_comp);
    //g_renderer->draw_deferred(entity_comp);
    g_renderer->draw_pbr(entity_comp);
    
    time += window->get_delta_time();
    fps_count += 0.5f * window->get_delta_time();
    g_renderer->end_frame();
    window->swap_buffers();





    //auto end = std::chrono::steady_clock::now();
    //auto diff = end - start_time;
    //double tiempo_en_milisegundos = std::chrono::duration<double, std::milli>(diff).count();
    //std::cout << "TOTAL" << tiempo_en_milisegundos << " milisegundos" << std::endl;
  }

  //physics_engine->Release(entity_comp);

  return 0;
}