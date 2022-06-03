#pragma once

#include "Frame/LevelInterface.h"
#include "Frame/UniformInterface.h"

namespace frame {

class UniformWrapper : public UniformInterface {
   public:
    UniformWrapper(Camera* camera) { camera_ = camera; }

   public:
    void SetModel(glm::mat4 model) { model_ = model; }
    void SetProjection(glm::mat4 projection) { projection_ = projection; }
    void SetView(glm::mat4 view) { view_ = view; }
    void SetTime(double time) { time_ = time; }

   public:
    glm::vec3 GetCameraPosition() const override;
    glm::vec3 GetCameraFront() const override;
    glm::vec3 GetCameraRight() const override;
    glm::vec3 GetCameraUp() const override;
    glm::mat4 GetProjection() const override;
    glm::mat4 GetView() const override;
    glm::mat4 GetModel() const override;
    double GetDeltaTime() const override;

   private:
    Camera* camera_ = nullptr;
    glm::mat4 model_         = glm::mat4(1.0f);
    glm::mat4 projection_    = glm::mat4(1.0f);
    glm::mat4 view_          = glm::mat4(1.0f);
    double time_             = 0.0;
};

}  // End namespace frame.
