#pragma once
#include <glm/glm.hpp>
#include <stdint.h>
#include "Layer.h"
#include "InputEvents.h"
#include "WindowEvents.h"

class MenuLayer : public Rune::Layer
{
public:
	MenuLayer();
	virtual ~MenuLayer();

	virtual void OnEvent(Rune::Event& event) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;

private:
	bool OnMouseMoved(Rune::MouseMovedEvent& event);
	bool OnMouseButtonPressed(Rune::MouseButtonPressedEvent& event);
	bool OnKeyPressed(Rune::KeyPressedEvent& event);
	bool OnWindowClosed(Rune::WindowClosedEvent& event);
	bool IsPlayHovered() const;
	bool IsQuitHovered() const;

private:
	bool m_PlayButtonHovered = false;
	bool m_QuitButtonHovered = false;
};