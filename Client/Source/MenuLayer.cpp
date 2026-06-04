#include "../Platform/Client/Application-Client.h"
#include "../Platform/Client/Window.h"
#include "MenuLayer.h"
#include "ClientLayer.h"
#include "log/log.h"

MenuLayer::MenuLayer()
{
	RUNE_DEBUG("Created New Menu-Layer!");
}

MenuLayer::~MenuLayer()
{
	RUNE_DEBUG("Destroyed Menu-Layer!");
}

void MenuLayer::OnEvent(Rune::Event& event)
{
	RUNE_TRACE("{}", event.ToString());

	Rune::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Rune::MouseButtonPressedEvent>([this](Rune::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Rune::MouseMovedEvent>([this](Rune::MouseMovedEvent& e) { return OnMouseMoved(e); });
	dispatcher.Dispatch<Rune::KeyPressedEvent>([this](Rune::KeyPressedEvent& e) {return OnKeyPressed(e); });
	dispatcher.Dispatch<Rune::WindowClosedEvent>([this](Rune::WindowClosedEvent& e) { return OnWindowClosed(e); });
}

void MenuLayer::OnUpdate(float ts)
{
	auto primary = Rune::Application::Get().GetWindow();
	if (primary && primary->IsValid())
	{
		
	}
}

void MenuLayer::OnRender()
{

}

bool MenuLayer::IsPlayHovered() const
{
	return false;
}

bool MenuLayer::IsQuitHovered() const
{
	return false;
}

bool MenuLayer::OnMouseMoved(Rune::MouseMovedEvent& event)
{
	return false;
}

bool MenuLayer::OnMouseButtonPressed(Rune::MouseButtonPressedEvent& event)
{
	if (IsPlayHovered())
	{
		TransitionTo<ClientLayer>();
	}
	else if (IsQuitHovered())
	{
		Rune::Application::Get().Stop();
	}
	return false;
}

bool MenuLayer::OnKeyPressed(Rune::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == GLFW_KEY_ESCAPE)
	{
		if (auto owner = GetOwner())
		{
			// Request deferred close for only this window
			Rune::Application::Get().WindowRequestClose(owner);
			return true; // handled
		}
		return true; // handled
	}
	return false;
}

bool MenuLayer::OnWindowClosed(Rune::WindowClosedEvent& event)
{
	// If event gives the closed Window pointer, handle it here.
	if (event.WindowGet())
	{
		auto primary = Rune::Application::Get().GetWindow();
		// if primary window closed -> stop app (will trigger cleanup / destructors)
		if (primary && event.WindowGet() == primary.get())
		{
			Rune::Application::Get().Stop();
		}
		else
		{
			// secondary window: request deferred destruction (safe outside callbacks)
			Rune::Application::Get().WindowRequestClose(event.WindowGet());
		}
	}
	else
	{
		// Backwards compatible: no window pointer provided -> stop app
		Rune::Application::Get().Stop();
	}
	return false;
}