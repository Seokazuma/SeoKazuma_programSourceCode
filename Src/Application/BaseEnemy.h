#pragma once

class GameScene;
class Enemy;

class BaseEnemy
{
public:
	virtual BaseEnemy* Action(GameScene* _gameScene, Enemy* _enemy) = 0;
};