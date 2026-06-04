#pragma once 
#include "imgui.h"

class Gui
{
public:
	Gui();
	~Gui();


	void init();
	void shutdown();
	void begin();
	void end();
	void render();


	ImGuiIO& io;
private:
};