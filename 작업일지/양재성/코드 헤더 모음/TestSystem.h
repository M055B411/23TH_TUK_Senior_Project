// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ECS_Test/Database.h"
#include "ECS_Test/Query.h"
#include "ECS_Test/SystemBase.h"
#include "FSlimeCoreCharacter.h"
#include "FSlimeBubble.h"
#include "ESC_Test_Components.h"

class TestSystem : public SystemBase
{
	//float BubbleHealth;
public:
	virtual void Update(float deltaTime, Database* DB) override;
};
