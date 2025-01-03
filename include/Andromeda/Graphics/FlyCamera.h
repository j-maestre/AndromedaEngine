#include "Andromeda/Graphics/Camera.h"
#include "Andromeda/HAL/Input.h"

namespace And
{
  class FlyCamera : public Camera
  {
  public:
    FlyCamera(Window& w);

    ~FlyCamera();

    void ProcessInput();
    void SetFixed(bool value);
    bool GetFixed();

    void SetSpeed(const float speed);

  private:
    Input m_Input;
    Window& m_Window;
    float m_Speed;
    float m_Sensitivity;
    float m_Pitch;
    float m_Yaw;
    bool m_FirstMouse;
    float m_LastMouseX;
    float m_LastMouseY;

    bool m_can_move;
  };
}
