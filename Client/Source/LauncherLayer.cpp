#include "LauncherLayer.h"
#include "Log/log.h"
#include "imgui.h"

LauncherLayer::LauncherLayer()
{
	RUNE_DEBUG("Created New Launcher-Layer!");

}

LauncherLayer::~LauncherLayer()
{
	RUNE_DEBUG("Destroyed Launcher-Layer!");
}

void LauncherLayer::OnEvent(Rune::Event& event)
{
	Rune::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Rune::MouseMovedEvent>([this](Rune::MouseMovedEvent& e) { return OnMouseMoved(e); });
	dispatcher.Dispatch<Rune::MouseButtonPressedEvent>([this](Rune::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Rune::KeyPressedEvent>([this](Rune::KeyPressedEvent& e) {return OnKeyPressed(e); });
	dispatcher.Dispatch<Rune::WindowClosedEvent>([this](Rune::WindowClosedEvent& e) { return OnWindowClosed(e); });
}

void LauncherLayer::OnUpdate(float ts)
{

}

bool LauncherLayer::OnMouseMoved(Rune::MouseMovedEvent& event)
{
	return false;
}

bool LauncherLayer::OnMouseButtonPressed(Rune::MouseButtonPressedEvent& event)
{
	return false;
}

bool LauncherLayer::OnKeyPressed(Rune::KeyPressedEvent& event)
{
	return false;
}

bool LauncherLayer::OnWindowClosed(Rune::WindowClosedEvent& event)
{
	if (event.WindowGet())
	{
		auto primary = Rune::Application::Get().GetWindow();
		
		if (primary && event.WindowGet() == primary.get())
		{
			Rune::Application::Get().Stop();
		}
		else
		{
			Rune::Application::Get().WindowRequestClose(event.WindowGet());
		}
	}
	else
	{
		Rune::Application::Get().Stop();
	}
	return false;
}

