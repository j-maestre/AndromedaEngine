#include "Andromeda/Graphics/Light.h"
#include "Andromeda/Graphics/Shader.h"

namespace And {



    LightManager::LightManager() {
        m_ambient_shader = Shader::make_default("lights/ambient.shader", "UniformAmbient");
        m_directional_shader = Shader::make_default("lights/ambient.shader", "UniformAmbient");
        m_point_shader = Shader::make_default("lights/ambient.shader", "UniformAmbient");
        m_spot_shader = Shader::make_default("lights/ambient.shader", "UniformAmbient");
    }

    LightManager::~LightManager() {

    }


    void LightManager::add_light(std::shared_ptr<DirectionalLight> l) {

        /*LightBuffer default_light;

        default_light.light.directional = l;
        default_light.light.type = LightType::Directional;
        //m_ambient_shader.set_light(l.get());

        m_Lights.push_back(default_light);*/
    }

    void LightManager::add_light(std::shared_ptr<AmbientLight> l) {
        Light default_light;

        default_light.ambient = l;
        default_light.type = LightType::Ambient;

        m_Lights.push_back(default_light);
    }

    void LightManager::add_light(std::shared_ptr<PointLight> l) {

        /*Light default_light;

        default_light.point = l;
        default_light.type = LightType::Point;

        m_Lights.push_back(default_light);*/
    }

    void LightManager::add_light(std::shared_ptr<SpotLight> l) {
        /*Light default_light;

        default_light.spot = l;
        default_light.type = LightType::Spot;

        m_Lights.push_back(default_light);*/
    }


    Shader* LightManager::bind_light(Light light){
        
        switch (light.type){
        case LightType::Ambient: 
            m_ambient_shader->set_light(light.ambient.get());
            return m_ambient_shader.get();
            break;
        //case LightType::Directional:m_directional_shader.set_light(light.directional.get());break;
        case LightType::Point:
            m_point_shader->set_light(light.point.get());
            return m_point_shader.get();
            break;
       // case LightType::Spot:m_spot_shader.set_light(light.spot.get());break;
        
        default:
            break;
        }

        return nullptr;
    }

}