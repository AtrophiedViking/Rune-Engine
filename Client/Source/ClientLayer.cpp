#include "ClientLayer.h"
#include "MenuLayer.h"
#include "../Platform/Client/Application-Client.h"
#include "log/log.h"
ClientLayer::ClientLayer()
{
	RUNE_DEBUG("Created New Client-Layer!");
	Rune::Application::Get().AssetManagerGet().GLBLoad("Kobold.glb");
}

ClientLayer::~ClientLayer()
{
	RUNE_DEBUG("Destroyed Client-Layer!");
}

void ClientLayer::OnEvent(Rune::Event& event)
{
	//std::println("{}", event.ToString());

	Rune::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Rune::MouseButtonPressedEvent>([this](Rune::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Rune::MouseMovedEvent>([this](Rune::MouseMovedEvent& e) { return OnMouseMoved(e); });
	dispatcher.Dispatch<Rune::KeyPressedEvent>([this](Rune::KeyPressedEvent& e) {return OnKeyPressed(e); });
	dispatcher.Dispatch<Rune::WindowClosedEvent>([this](Rune::WindowClosedEvent& e) { return OnWindowClosed(e); });
}

void ClientLayer::OnUpdate(float ts)
{
	m_Time += ts;
	
	auto primary = Rune::Application::Get().WindowGet();
	if (primary && primary->IsValid())
	{
		
	}
}

void ClientLayer::OnRender()
{

}

bool ClientLayer::OnMouseButtonPressed(Rune::MouseButtonPressedEvent& event)
{
	glm::vec2 framebufferSize = GetOwner()->GetFramebufferSize();
	float aspectRatio = framebufferSize.x / framebufferSize.y;
	glm::vec2 normalizedMousePos = (m_MousePosition / framebufferSize) * 2.0f - 1.0f;
	normalizedMousePos.x *= aspectRatio;
	normalizedMousePos.y *= -1.0f;
	normalizedMousePos.y += 0.7f;

	RUNE_TRACE("Mouse Button Pressed at: ({}, {})", normalizedMousePos.x, normalizedMousePos.y);

	return false;
}

bool ClientLayer::OnMouseMoved(Rune::MouseMovedEvent& event)
{
	m_MousePosition = { static_cast<float>(event.GetX()), static_cast<float>(event.GetY()) };

	return false;
}

bool ClientLayer::OnKeyPressed(Rune::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == GLFW_KEY_ESCAPE)
	{
		auto window = Rune::Application::Get().WindowCreate({ "Menu", 400, 600, false, false});

		window->PushLayer<MenuLayer>();

		//TransitionTo<MenuLayer>();
		return true; // handled
	}
	return false;
}

bool ClientLayer::OnWindowClosed(Rune::WindowClosedEvent& event)
{
	// If event gives the closed Window pointer, handle it here.
	if (event.WindowGet())
	{
		auto primary = Rune::Application::Get().WindowGet();
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