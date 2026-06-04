#include "Gui.h"
#include "log/log.h"
void Gui::init()
{
	ImGui::CreateContext();
	io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	RUNE_DEBUG("ImGui Context Created!");
}