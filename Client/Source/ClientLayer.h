#pragma once
#include <glm/glm.hpp>
#include <stdint.h>
#include "Layer.h"
#include "InputEvents.h"
#include "WindowEvents.h"

class ClientLayer : public Rune::Layer
{
public:
	ClientLayer();
	virtual ~ClientLayer();

	virtual void OnEvent(Rune::Event& event) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;
private:
	bool OnMouseButtonPressed(Rune::MouseButtonPressedEvent& event);
	bool OnMouseMoved(Rune::MouseMovedEvent& event);
	bool OnKeyPressed(Rune::KeyPressedEvent& event);
	bool OnWindowClosed(Rune::WindowClosedEvent& event);
	

private:
	float m_Time = 0.0f;
	glm::vec2 m_MousePosition{ 0.0f };
};