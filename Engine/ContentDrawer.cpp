#include "ContentDrawer.h"
#include "Include/Jett Engine/GameEngine.h"
#include "Include/Jett Engine/GlobalLerpProcessor.h"
#include "FloatlerpPackage.h"

ContentDrawer::ContentDrawer() {
	MemberFuncKeyListener<ContentDrawer>* SpaceKeyListener = new MemberFuncKeyListener<ContentDrawer>(this, &ContentDrawer::SpaceBarUp);
	GE::GameEngine::AddKeyUpEventListener(SDL_SCANCODE_SPACE, SpaceKeyListener);

	MemberFuncButtonListener<ContentDrawer>* leftMouseUpListener = new MemberFuncButtonListener<ContentDrawer>(this, &ContentDrawer::MouseLeftUp);
	GE::GameEngine::AddMouseUpEventListener(SDL_BUTTON_LEFT, leftMouseUpListener);
}

void ContentDrawer::SpaceBarUp()
{
	if (this->contentDrawerMoving == true)
		return;

	ContentDrawer* contentDrawer = this;

	for (GUIImage* contentDrawerImage : GE::GameEngine::getContentDrawerImages()) {
		const float constantX = contentDrawerImage->getPosition().x;
		if (this->drawerOpen) {
			//close draw
			this->contentDrawerMoving = true;
			float startY = contentDrawerImage->getPosition().y;
			float targetY = contentDrawerImage->getPosition().y + 150;
			GlobalLerpProcessor::AddLerpPackage(
				new FloatLerpPackage(
					startY,
					targetY,
					[](float value) {},
					[contentDrawerImage, constantX](float value) {
						contentDrawerImage->setPosition(glm::vec2(constantX, value));
					},

					[contentDrawer](LerpPackage* pkg) {
						contentDrawer->setDrawerOpen(false);
						contentDrawer->setDrawerMoving(false);

						SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), GE::GameEngine::getWindowDimentions().x / 2, GE::GameEngine::getWindowDimentions().y / 2);
						GE::GameEngine::SetFpsCamMouseOwnership(true);
						GE::GameEngine::getFpsCamera()->setEnabled(true);
						GE::GameEngine::getObjectPicker().lock()->setEnabled(true);
					},
					0.2f,
					true
				)
			);
		}
		else {
			//open draw
			GE::GameEngine::getFpsCamera()->setEnabled(false);
			GE::GameEngine::getObjectPicker().lock()->setEnabled(false);
			this->contentDrawerMoving = true;
			float startY = contentDrawerImage->getPosition().y;
			float targetY = contentDrawerImage->getPosition().y - 150;
			GlobalLerpProcessor::AddLerpPackage(
				new FloatLerpPackage(
					startY,
					targetY,
					[](float value) {},
					[contentDrawerImage, constantX](float value) {
						contentDrawerImage->setPosition(glm::vec2(constantX, value));
					},
					[contentDrawer](LerpPackage* pkg) {
						contentDrawer->setDrawerOpen(true);
						contentDrawer->setDrawerMoving(false);

						GE::GameEngine::SetFpsCamMouseOwnership(false);
					},
					0.1f,
					true
				)
			);
		}
	}
}

void ContentDrawer::MouseLeftUp()
{
	if (this->contentDrawerMoving == true || this->drawerOpen == false)
		return;

	//check if an image has been pressed
	std::vector<GUIImage*> images = GE::GameEngine::getContentDrawerImages();

	for (int i = 1; i < images.size(); ++i) {
		int mouseX;
		int mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		if (images[i]->isInside(mouseX, mouseY)) {
			std::string ModelPath = this->indexToCachedModelPath.find(i)->second;
			GameObject* object = new GameObject(ModelPath);

			VisualMesh* visualMesh = VisualMesh::CreateHeapVisualMeshFromDaeWithIncludedTextureReference("../../Assets/" + ModelPath, GE::GameEngine::camera);
			visualMesh->setPos(glm::vec3(0, 0, 0));
			object->AddComponent<VisualMesh>(visualMesh);

			GE::GameEngine::StaticAddGameObject(object);

			GE::GameEngine::getObjectPicker().lock().get()->SetSelectedGameObject(object);
		}
	}

	GE::Model::ClearCachedModels();

	moveDrawerDown();
}

void ContentDrawer::moveDrawerDown()
{
	this->contentDrawerMoving = true;

	ContentDrawer* contentDrawer = this;


	for (GUIImage* contentDrawerImage : GE::GameEngine::getContentDrawerImages()) {
		float startY = contentDrawerImage->getPosition().y;
		float targetY = contentDrawerImage->getPosition().y + 150;
		const float constantX = contentDrawerImage->getPosition().x;
		GlobalLerpProcessor::AddLerpPackage(
			new FloatLerpPackage(
				startY,
				targetY,
				[](float value) {},
				[contentDrawerImage, constantX](float value) {
					contentDrawerImage->setPosition(glm::vec2(constantX, value));
				},

				[contentDrawer](LerpPackage* pkg) {
					contentDrawer->setDrawerOpen(false);
					contentDrawer->setDrawerMoving(false);


					SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), GE::GameEngine::getWindowDimentions().x / 2, GE::GameEngine::getWindowDimentions().y / 2);
					GE::GameEngine::SetFpsCamMouseOwnership(true);
					GE::GameEngine::getFpsCamera()->setEnabled(true);
					GE::GameEngine::getObjectPicker().lock()->setEnabled(true);
				},
				0.2f,
				true
			)
		);
	}
}


