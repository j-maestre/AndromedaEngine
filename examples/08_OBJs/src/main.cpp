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

#include "Common/JobSystem.h"
#include "Common/Engine.h"
#include "Common/Window.h"
#include "Common/GraphicsContext.h"
#include "Common/Renderer.h"
#include "Common/Shader.h"
#include "Common/Triangle.h"
#include "Common/ObjLoader.h"
#include "Common/ObjGenerator.h"
#include "Common/ShaderGenerator.h"

#include "Common/Input.h"
#include "Common/ActionInput.h"
#include "Common/EntityComponentSystem.h"
<<<<<<< HEAD
#include "Common/Editor/Editor.h"
=======
#include "Common/ShaderTextEditor.h"
>>>>>>> ba24e31776529b0a89ed793749e4d6b18ea3fa32

#include "Common/JobSystem.h"
#include "Common/Log.h"

#include "Common/ResourceManager.h"

int main(int argc, char** argv){

  And::Engine e;

  And::JobSystem js{e};

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);

  And::ResourceManager r_manager{*window, js};
  r_manager.add_resource_generator<And::ObjGenerator>();

  And::Editor editor;
  

  // Show pc info
  g_context->create_info();


  // Creamos el shader
  //And::ShaderInfo s_info;
  //s_info.path_fragment = "fshader.fs";
  //s_info.path_vertex = "vshader.vs";

  //std::shared_ptr<And::Shader> g_shader = And::Shader::make("default/deafult_shader.shader");
  


  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);

  
  And::EntityComponentSystem entity_comp;
    
  entity_comp.add_component_class<And::resource<And::ObjLoader>>();
  entity_comp.add_component_class<And::resource<And::Shader>>();
  entity_comp.add_component_class<And::Transform>();  


  And::resource<And::Shader> g_shader = r_manager.new_resource<And::Shader>("default/default_shader_normals.shader");

  int num_obj = 10;
  float pos_x = 0.0f;
  float pos_y = -5.0f;

  for(int i = -5; i < (int)(num_obj / 2); i++){
    And::resource<And::ObjLoader> obj_teapot = r_manager.new_resource<And::ObjLoader>("teapot.obj");
    And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Entity obj_id = entity_comp.new_entity(obj_teapot, tran);
  }
  pos_y = 5.0f;
  for(int i = -5; i < (int)(num_obj / 2); i++){
    And::resource<And::ObjLoader> obj_teapot = r_manager.new_resource<And::ObjLoader>("teapot.obj");
    And::Transform tran = {{pos_x + (i*6.0f), pos_y, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    And::Entity obj_id = entity_comp.new_entity(obj_teapot, tran);
  }

  ShaderTextEditor editor_teapot("content/teapot_shader.ashader");

 
  while (window->is_open()){
    window->update();
    g_renderer.new_frame();

    editor.ShowWindows();


    std::function<void(And::Transform* trans, And::resource<And::ObjLoader>* resource)> obj_draw =  [&g_renderer, &g_shader] (And::Transform* trans, And::resource<And::ObjLoader>* resource){

      g_renderer.draw_obj(*(*resource), &(*g_shader), *trans);
    };

    entity_comp.execute_system(obj_draw);

    g_renderer.end_frame();
    window->swap_buffers();
  }

  
  return 0;
}