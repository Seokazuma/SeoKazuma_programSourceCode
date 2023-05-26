#pragma once
#include "main.h"
#include "define.h"

bool ModelIntersects(const KdModel& _model, const Math::Matrix& _mat, const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec);

#include"System.h"
#include"SceneManager.h"
#include"BaseScene.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"PauseScene.h"
#include"ScoreScene.h"

#include"Wall.h"
#include"Config.h"
#include"Treasure.h"
#include"TreasureManager.h"
#include"Diamond.h"
#include"Potion.h"

#include"Player.h"
#include"PotionEffect.h"
#include"PotionEffectManager.h"
#include"BaseEnemy.h"
#include"EnemyWalk.h"
#include"EnemyTrack.h"
#include"EnemyAttack.h"
#include"Enemy.h"
#include"EnemyManager.h"

#include"Score.h"
#include"ModelManager.h"
#include"TextureManager.h"