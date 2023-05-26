#pragma once

class BaseScene
{
public:

	virtual ~BaseScene();

	virtual void Frame();

	virtual void SetCamera();

	virtual void Update();

	virtual void DynamicDraw();

	virtual void Draw();

private:

};