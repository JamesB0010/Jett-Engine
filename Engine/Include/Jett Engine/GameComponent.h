#pragma once

class GameObject;
class GameComponent
{
protected:
	GameComponent() {}


	virtual void Awake();

	virtual void Start();

	virtual void Update();

	virtual void Destroy();

	friend class GameObject;
};

class DrawableGameComponent : public GameComponent {
private:
	virtual void Draw();

	friend class GameObject;
};

