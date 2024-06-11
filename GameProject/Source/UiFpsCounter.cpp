#include "../Include/UiFpsCounter.h"

UiFpsCounter::UiFpsCounter(GUIText* uiText) {
	this->uiText = uiText;
}

void UiFpsCounter::Draw()
{
	this->fpsCounter.FrameRendered();

	std::string fpsString = std::to_string((int)this->fpsCounter.getFps());

	this->uiText->setText("Fps: " + fpsString);
}

GUIText* UiFpsCounter::getuiText()
{
	return this->uiText;
}
