#include "GameWorld.h"
#include "Utilities.h"
#include "Timer.h"
#include <random>
#include "Cafe.h"
#include "FruitGame.h"

//gravity
GameWorld::GameWorld(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);
}


void GameWorld::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;
	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetContactListener(&listener);

	SetSceneChange(false, -1);
	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//initializing audio
	SDL_Init(SDL_INIT_AUDIO);
	//loading wav file
	SDL_LoadWAV("overworldBGM.wav", &wavSpec, &wavBuffer, &wavLength);

	//setting output device
	deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

	//checking if audio is loaded properly and returning true if it is
	int success = SDL_QueueAudio(deviceID, wavBuffer, wavLength);
	//starting audio
	SDL_PauseAudioDevice(deviceID, 0);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//main player entity
	{
		auto entity = ECS::CreateEntity();
		player = entity;

		ECS::SetIsMainPlayer(entity, true);

		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<CollisionFlagClass>(entity);
		ECS::AttachComponent<DialogueClass>(entity);
		ECS::AttachComponent<EndConditions>(entity);

		//Set up components
		std::string fileName = "spritesheets/overworldSpritesheet.png";
		std::string animations = "villager.json";
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 20, 30, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(60.f, 60.f, 6.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 20.f;
		float shrinkY = 20.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(150.f), float32(-80.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	//Setup ground
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "ground.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 830, 700);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(170.f, 80.f, 1.f));
	}

	//Setup water
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "water.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 829, 70);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(110.f, -120.f, 2.f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(170.f), float32(-130.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup West Boundary
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "filler.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 600, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
		
		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-125.f), float32(60.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(90);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//Setup North Boundary
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "filler.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 600, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(170.f), float32(360.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//Setup East Boundary
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "filler.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 600, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(470.f), float32(60.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(90);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//Setup Trees
	for (int i = 0; i < 6; i++)
	{
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Sets up components
			std::string fileName = "tree.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 320 / 4, 320 / 4);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(treeX, treeY, 3.f));
		}
		treeY += 80;
	}
	treeY = 400.f;
	treeX += 100;
	for (int i = 0; i < 6; i++)
	{
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Sets up components
			std::string fileName = "tree.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 320 / 4, 320 / 4);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(treeX, treeY, 3.f));
		}
		treeX += 100;
	}
	treeY -= 80;

	for (int i = 0; i < 5; i++)
	{
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Sets up components
			std::string fileName = "tree.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 320 / 4, 320 / 4);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(treeX, treeY, 3.f));
		}
		treeY -= 80;
	}
	treeX = -180.f;
	treeY = -50.f;

	//Setup Docks
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "dock.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 512/6, 512/6);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(150.f, -120.f, 3.f));
	}

	//Setup Boat
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "boat.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 500/7, 500/7);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(100.f, -120.f, 3.f));
	}

	//Setup Mithunan
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "mithunan.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 372/17, 678/17);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(210.f), float32(-40.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup Mithunan Start Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		mStart = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "MStart.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(205.f, 35.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//Setup Mithunan End Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		mEnd = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "MFinish.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(205.f, 35.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}


	//Setup Kainat
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "kainat.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 483 / 20, 679 / 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 101.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(-60.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup Kainat Start Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		kaStart = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "KStart.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-5.f, 15.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	//Setup Kainat End Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		kaEnd = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "KFinish.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-5.f, 15.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}


	//Setup Stevie
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "stevie.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 485 / 20, 678 / 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(340.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup Stevie Start Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		sStart = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "SStart.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(335.f, 45.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	//Setup Stevie End Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		sEnd = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "SFinish.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(335.f, 45.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//Setup Kyra
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "kyra.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 398 / 16, 678 / 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-5.f), float32(190.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup Kyra Start Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		kyStart = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "KyStart.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, 265.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	//Setup Kyra End Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		kyEnd = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "KyFinish.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-10.f, 265.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//Setup Winston
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "winston.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 425 / 16, 597 / 16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(130.f), float32(210.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup Winston Start Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		wStart = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "WStart.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(125.f, 285.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	//Setup Winston End Dialogue
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		wEnd = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "WFinish.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 140, 120);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(125.f, 285.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//Setup Orchard Office
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "orchard.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1417/16, 1417/16);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 100.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(40.f), float32(-10.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - 30), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "orchard.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1417 / 16, 1417 / 24);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(40.f), float32(-45.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - 30), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup Library
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "library.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1417 / 12, 1417 / 12);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(290.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup Cobalt Cafe
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "cafe.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1417 / 15, 1417 / 15);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(50.f), float32(240.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup Photography Den
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "photoDen.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 512 / 5, 512 / 5);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 100.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 30.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(390.f), float32(200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "photoDen.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 512 / 5, 512 / 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 100.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 30.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(390.f), float32(155.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//Setup Orchard Trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "orchardMat.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 500/12, 300/12);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new TestTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(40, -65));
	}

	//Setup Book One
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		bookOne = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "book.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 512 / 19, 384 / 19);
		if (bookOneTriggered == false)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}
		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(5);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(400, 340));
	}

	//Setup Book Two
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		bookTwo = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "book.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 512 / 19, 384 / 19);
		if (bookTwoTriggered == false)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}
		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(6);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(300.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(380, 30));
	}

	//Setup Book Three
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		bookThree = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "book.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 512 / 19, 384 / 19);
		if (bookThreeTriggered == false)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}
		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(7);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(-30, 250));
	}

	//Setup Roll One
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		rollOne = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "roll.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 475 / 19, 420 / 19);
		if (rollOneTriggered == false)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}
		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(120.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(8);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-30.f), float32(-60.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(-80, -65));
	}

	//Setup Roll Two
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		rollTwo = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "roll.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 475 / 19, 420 / 19);
		if (rollTwoTriggered == false)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}
		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(120.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(9);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(35, 100));
	}

	//Setup Roll Three
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		rollThree = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "roll.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 475 / 19, 420 / 19);
		if (rollThreeTriggered == false)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}
		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(120.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(10);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(150, 340));
	}

	//Setup Instrument One (Record Disc)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		instrumentOne = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "record.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 512 / 19, 384 / 19);
		if (instrumentOneTriggered == false)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}
		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(11);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(450, 120));
	}

	//Setup Instrument Two (Drumsticks)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		instrumentTwo = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "drumSticks.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 475 / 18, 470 / 18);
		if (instrumentTwoTriggered == false)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}
		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(12);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(200, 110));
	}

	//Setup Instrument Three (Headphones)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		instrumentThree = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "headphones.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 512 / 19, 384 / 19);
		if (instrumentThreeTriggered == false)
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		}
		else
		{
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		}
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(13);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(300, 230));
	}

	//Setup Cafe Trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "coffeeMat.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 500 / 12, 300 / 12);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(14);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(50, 180));
	}

	//Setup Cafe Trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "orchardMat.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 500 / 12, 300 / 12);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 3.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(15);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(40, -65));
	}

	//Setup Mithunan Trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "filler.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 2.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(2);

		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(210, -50));
	}

	//Setup Stevie Trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "filler.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 2.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(3);

		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(340, -40));
	}

	//Setup Winston Trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "filler.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 2.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(4);

		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-40.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(130, 200));
	}

	//Setup Kainat Trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "filler.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 2.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(0);

		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(30.f), float32(-40.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(0, -70));
	}

	//Setup Kyra Trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "filler.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 50.f, 2.f));
		ECS::GetComponent<Trigger*>(entity) = new DialogueTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetDiaNum(1);

		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-5.f), float32(190.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(-5, 180));
	}	

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}
void GameWorld::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	auto& winCon = ECS::GetComponent<EndConditions>(MainEntities::MainPlayer());
	//Scene::AdjustScrollOffset();
	player.Update();
}

void GameWorld::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	float speed = 4.f;
	vec3 vel = vec3(0.f, 0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 10.f;
	}

	if (Input::GetKey(Key::A))//left
	{
		//player.GetBody()->ApplyForceToCenter(b2Vec2(-400.f * speed, 0.f), true);
		vel = vel + vec3(-10.f, 0.f, 0.f);
	}
	if (Input::GetKey(Key::D))//right
	{
		//player.GetBody()->ApplyForceToCenter(b2Vec2(400.f * speed, 0.f), true);
		vel = vel + vec3(10.f, 0.f, 0.f);
	}
	if (Input::GetKey(Key::W))//up
	{
		//player.GetBody()->ApplyForceToCenter(b2Vec2(0.f, 400.f * speed), true);
		vel = vel + vec3(0.f, 10.f, 0.f);
	}
	if (Input::GetKey(Key::S))//down
	{
		//player.GetBody()->ApplyForceToCenter(b2Vec2(0.f, -400.f * speed), true);
		vel = vel + vec3(0.f, -10.f, 0.f);
	}
	player.SetVelocity(vel * speed);

	//Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
	}
}

//controls the overworld keyboard input
void GameWorld::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& talk = ECS::GetComponent<DialogueClass>(MainEntities::MainPlayer());
	auto& winCon = ECS::GetComponent<EndConditions>(MainEntities::MainPlayer());
	auto& mS = ECS::GetComponent<Sprite>(mStart);
	auto& mF = ECS::GetComponent<Sprite>(mEnd);
	auto& kaS = ECS::GetComponent<Sprite>(kaStart);
	auto& kaF = ECS::GetComponent<Sprite>(kaEnd);
	auto& kyS = ECS::GetComponent<Sprite>(kyStart);
	auto& kyF = ECS::GetComponent<Sprite>(kyEnd);
	auto& wS = ECS::GetComponent<Sprite>(wStart);
	auto& wF = ECS::GetComponent<Sprite>(wEnd);
	auto& sS = ECS::GetComponent<Sprite>(sStart);
	auto& sF = ECS::GetComponent<Sprite>(sEnd);


	//draws physics bodies for debugging purposes
	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}

	//talking to Mithunan
	if (Input::GetKeyDown(Key::E) && talk.GetMithunan() == true)
	{
		if (rollsFound == 3)
		{
			mS.SetTransparency(0.f);
			mF.SetTransparency(1.f);
			SetFlag(true, 5);
		}
		else
		{
			mS.SetTransparency(1.f);
		}
	}

	//talking to Stevie
	if (Input::GetKeyDown(Key::E) && talk.GetStevie() == true)
	{
		if (booksFound == 3)
		{
			sS.SetTransparency(0.f);
			sF.SetTransparency(1.f);
			SetFlag(true, 3);
		}
		else
		{
			sS.SetTransparency(1.f);
		}
		
	}

	//talking to Winston
	if (Input::GetKeyDown(Key::E) && talk.GetWinston() == true)
	{
		if (instrumentsFound == 3)
		{
			wS.SetTransparency(0.f);
			wF.SetTransparency(1.f);
			SetFlag(true, 4);
		}
		else
		{
			wS.SetTransparency(1.f);
		}
	}

	//talking to Kainat
	if (Input::GetKeyDown(Key::E) && talk.GetKainat() == true)
	{
		kaS.SetTransparency(1.f);
	}

	//talking to Kyra
	if (Input::GetKeyDown(Key::E) && talk.GetKyra() == true)
	{
		kyS.SetTransparency(1.f);
	}

	//collecting quest items\\

	if (Input::GetKeyDown(Key::E) && talk.GetBookOne() == true && bookOneTriggered == false)
	{;
		booksFound += 1;
		ECS::GetComponent<Sprite>(bookOne).SetTransparency(0.f);
		bookOneTriggered = true;
	}
	else if (Input::GetKeyDown(Key::E) && talk.GetBookTwo() == true && bookTwoTriggered == false)
	{
		booksFound += 1;
		ECS::GetComponent<Sprite>(bookTwo).SetTransparency(0.f);
		bookTwoTriggered = true;
	}
	else if (Input::GetKeyDown(Key::E) && talk.GetBookThree() == true && bookThreeTriggered == false)
	{
		booksFound += 1;
		ECS::GetComponent<Sprite>(bookThree).SetTransparency(0.f);
		bookThreeTriggered = true;
	}

	if (Input::GetKeyDown(Key::E) && talk.GetRollOne() == true && rollOneTriggered == false)
	{
		rollsFound += 1;
		ECS::GetComponent<Sprite>(rollOne).SetTransparency(0.f);
		rollOneTriggered = true;
	}
	else if (Input::GetKeyDown(Key::E) && talk.GetRollTwo() == true && rollTwoTriggered == false)
	{
		rollsFound += 1;
		ECS::GetComponent<Sprite>(rollTwo).SetTransparency(0.f);
		rollTwoTriggered = true;
	}
	else if (Input::GetKeyDown(Key::E) && talk.GetRollThree() == true && rollThreeTriggered == false)
	{
		rollsFound += 1;
		ECS::GetComponent<Sprite>(rollThree).SetTransparency(0.f);
		rollThreeTriggered = true;
	}
	
	if (Input::GetKeyDown(Key::E) && talk.GetInstrumentOne() == true && instrumentOneTriggered == false)
	{
		instrumentsFound += 1;
		ECS::GetComponent<Sprite>(instrumentOne).SetTransparency(0.f);
		instrumentOneTriggered = true;
	}
	else if (Input::GetKeyDown(Key::E) && talk.GetInstrumentTwo() == true && instrumentTwoTriggered == false)
	{
		instrumentsFound += 1;
		ECS::GetComponent<Sprite>(instrumentTwo).SetTransparency(0.f);
		instrumentTwoTriggered = true;
	}
	else if (Input::GetKeyDown(Key::E) && talk.GetInstrumentThree() == true && instrumentThreeTriggered == false)
	{
		instrumentsFound += 1;
		ECS::GetComponent<Sprite>(instrumentThree).SetTransparency(0.f);
		instrumentThreeTriggered = true;
	}

	//entering cafe chaos and closing audio
	if (Input::GetKeyDown(Key::E) && talk.GetCafe() == true)
	{
		SDL_CloseAudioDevice(deviceID);
		Scene::SetSceneChange(true, 3);		
	}

	//entering fruit shakedown and closing audio
	if (Input::GetKeyDown(Key::E) && talk.GetOrchard() == true)
	{
		SDL_CloseAudioDevice(deviceID);
		Scene::SetSceneChange(true, 4);
	}
	
	//hiding dialogue bubbles
	if (Input::GetKeyDown(Key::R))
	{
		mS.SetTransparency(0.f);
		mF.SetTransparency(0.f);
		sS.SetTransparency(0.f);
		sF.SetTransparency(0.f);
		wS.SetTransparency(0.f);
		wF.SetTransparency(0.f);
		kaS.SetTransparency(0.f);
		kaF.SetTransparency(0.f);
		kyS.SetTransparency(0.f);
		kyF.SetTransparency(0.f);
	}
}

