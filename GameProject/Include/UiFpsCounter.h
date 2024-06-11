#pragma once
#include <Jett Engine/GameComponent.h>
#include<iostream>
#include <string>
#include <Jett Engine/FPSCounter.h>
#include <Kerosene Renderer/GUIText.h>
class UiFpsCounter : public DrawableGameComponent
{
	FPSCounter fpsCounter;

	GUIText* uiText = nullptr;


public:
	UiFpsCounter(GUIText* uiText);

	void Draw() override;

	GUIText* getuiText();
};

