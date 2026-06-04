#pragma once
#include "Layer.h"

class RenderLayer : public Rune::Layer
{
public:
	RenderLayer();
	virtual ~RenderLayer();
	virtual void OnEvent(Rune::Event& event) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;
private:
};
