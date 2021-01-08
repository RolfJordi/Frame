#include "Application.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Draw.h"

Application::Application(
	const std::shared_ptr<frame::WindowInterface>& window) :
	window_(window) {}

void Application::Startup()
{
	auto draw = std::make_shared<Draw>(window_->GetUniqueDevice());
	window_->SetDrawInterface(draw);
}

void Application::Run()
{
	window_->Run();
}