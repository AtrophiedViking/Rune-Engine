#pragma once
#include "../Platform/Client/Application-Client.h"
#include "Layer.h"
#include "WindowEvents.h"
#include "InputEvents.h"


class LauncherLayer : public Rune::Layer
{
public:
	LauncherLayer();
	~LauncherLayer();

	void OnEvent(Rune::Event& event) override;
	void OnUpdate(float ts) override;
private:
	bool OnMouseMoved(Rune::MouseMovedEvent& event);
	bool OnMouseButtonPressed(Rune::MouseButtonPressedEvent& event);
	bool OnKeyPressed(Rune::KeyPressedEvent& event);
	bool OnWindowClosed(Rune::WindowClosedEvent& event);
};