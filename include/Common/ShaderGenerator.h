#pragma once
#include "Common/Resources/ResourceGenerator.h"
#include "Common/Shader.h"


namespace And{

class ShaderGenerator : public ResourceGenerator<Shader>{

  public:

  ShaderGenerator(){
    
    ShaderInfo s_info;
    s_info.path_fragment = "default/fshader_default.fs";
    s_info.path_vertex = "default/vshader_default.vs";

    std::shared_ptr<Shader> shader = Shader::make("default/deafult_shader.shader");
    m_Default = shader;
  }
  ShaderGenerator(const ShaderGenerator&);
  ShaderGenerator(ShaderGenerator&&);
  ~ShaderGenerator(){}

  virtual std::shared_ptr<Shader> operator()(const std::string& path) override{
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    std::shared_ptr<Shader> tmp = Shader::make(path);
    if(tmp)return tmp;

    return m_Default;
  }

  virtual uint64 GenerateId(const std::string& Path) override{
		return std::hash<std::string>{}(Path);
	}

	virtual std::shared_ptr<Shader> GetDefault() override{
		return m_Default;
	}

  private:

  std::shared_ptr<Shader> m_Default;
};

}