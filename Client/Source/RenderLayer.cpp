#include "RenderLayer.h"
#include "log/log.h"
RenderLayer::RenderLayer()
{
	RUNE_DEBUG("Created New Render-Layer!");
}
RenderLayer::~RenderLayer()
{
	RUNE_DEBUG("Destroyed Render-Layer!");
}
void RenderLayer::OnEvent(Rune::Event& event)
{
	//std::println("{}", event.ToString());
}
void RenderLayer::OnUpdate(float ts)
{

}
void RenderLayer::OnRender()
{

}
