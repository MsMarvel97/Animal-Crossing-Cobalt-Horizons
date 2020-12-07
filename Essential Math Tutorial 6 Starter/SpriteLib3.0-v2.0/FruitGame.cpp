#include "FruitGame.h"
#include "Utilities.h"
#include "Game.h"
#include "Scene.h"
#include <iostream>
#include <random>

FruitGame::FruitGame(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -20.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

void FruitGame::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;
	m_physicsWorld = new b2World(m_gravity);
	m_physicsWorld->SetContactListener(&listener);
	SetSceneChange(false, -1);
	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	timer = 0;

	SDL_Init(SDL_INIT_AUDIO);



	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Basket Boy entity
	{
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);
		player = entity;

		//Add components
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<PlayerPoints>(entity);
		ECS::AttachComponent<EndConditions>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Set up components
		std::string fileName = "spritesheets/BasketBoy.png";
		std::string animations = "HoldingBasket.json";
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 20, 30, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(-50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}


	{
		//Creates background middle
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "treeBackground.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 160);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -5.f, 0.f));
	}


	{

		//Creates background left
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "treeBackground.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 160);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-150.f, -5.f, 0.f));
	}

	{

		//Creates background right
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "treeBackground.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 160);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(150.f, -5.f, 0.f));
	}

	{
		//Creates the boundary wall on the right
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "Side1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 15);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, -1.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(135.f), float32(-50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(90.f);
	}

	{
		//Creates boundary wall on the left
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "Side1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 15);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, -1.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-135.f), float32(-50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		tempPhsBody.SetRotationAngleDeg(90.f);
	}

	{
		//Floor
		//Creates entity
		auto entity = ECS::CreateEntity();
		floor = entity;
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1000, 15);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, -1.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(-70.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	}



	//Stun Icon
	{
		auto entity = ECS::CreateEntity();
		stunIcon = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Kinematics>(entity);

		std::string fileName = "stun.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 15, 15);
		ECS::GetComponent<Kinematics>(entity).SetChild(entity);
		ECS::GetComponent<Kinematics>(entity).SetParent(player);
		ECS::GetComponent<Kinematics>(entity).SetOffset(0.f, 5.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f + 30, -50 + 30, 50.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

	}

	//Bonk Icon
	{
		auto entity = ECS::CreateEntity();
		bonk = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "bonk.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 40.f, 50.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);

	}

	//Time's Up Screen
	{
		auto entity = ECS::CreateEntity();
		timesUp = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "timeUp.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 80, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 20.f, 50.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);


	}

	//Timer Nameplate
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "timer.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(-87.f, 65.f, 1.f);
	}

	//Points Nameplate
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "points.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 20);
		ECS::GetComponent<Transform>(entity).SetPosition(83.f, 65.f, 1.f);
	}

	//Minutes
	{
		auto entity = ECS::CreateEntity();
		timerMinutes = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Digit1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-83.f, 65.f, 50.f));
	}

	//Colon
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Colon.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-75.f, 65.f, 50.f));
	}

	//Seconds (10s)
	{
		auto entity = ECS::CreateEntity();
		timerSecondsTens = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Digit3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-67.f, 65.f, 50.f));
	}

	//Seconds (1s)
	{
		auto entity = ECS::CreateEntity();
		timerSecondsOnes = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Digit0.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-59.f, 65.f, 50.f));
	}

	//Points (10s)
	{
		auto entity = ECS::CreateEntity();
		pointsTens = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Digit0.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(91.f, 65.f, 50.f));

	}

	//Points (1s)
	{
		auto entity = ECS::CreateEntity();
		pointsOnes = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Digit0.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(99.f, 65.f, 50.f));
	}

	//Setup MainCamera Entity
	{

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

		//Attaches Camera for Side Scrolling
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void FruitGame::Update()
{
	auto& stun = ECS::GetComponent<PlayerPoints>(MainEntities::MainPlayer());
	auto& stunKin = ECS::GetComponent<Kinematics>(stunIcon);
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());

	Scene::AdjustScrollOffset();

	NewFruits();
	UpdateSprites();
	UpdatePoints();
	UpdateCamera();

	if (stun.GetStun() == false)
	{
		player.Update();
	}

	stunKin.SetPosition();
}

void FruitGame::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& points = ECS::GetComponent<PlayerPoints>(MainEntities::MainPlayer());
	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift) && start == true && points.GetStun() == false)
	{
		speed *= 1.3f;
	}

	if (Input::GetKey(Key::A) && start == true && points.GetStun() == false)//left
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-350000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D) && start == true && points.GetStun() == false)//right
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(350000.f * speed, 0.f), true);
	}

	if (Input::GetKey(Key::O)) {
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
	}
}

void FruitGame::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (Input::GetKeyDown(Key::Enter) && start == false) {
		start = true;
		if (timer == 0)
		{
			timer = Timer::time;

			SDL_LoadWAV("fruitgameBGM.wav", &wavSpec, &wavBuffer, &wavLength);

			deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

			int success = SDL_QueueAudio(deviceID, wavBuffer, wavLength);
			SDL_PauseAudioDevice(deviceID, 0);
		}
	}
	if (Input::GetKeyDown(Key::Enter) && gameOver == true)
	{
		gameOver = false;
		Scene::SetSceneChange(true, 6);
	}

}

void FruitGame::KeyboardUp()
{


}

void FruitGame::NewFruits()
{
	float currentTime = Timer::StopWatch(timer);
	auto& points = ECS::GetComponent<PlayerPoints>(MainEntities::MainPlayer());
	auto& winCon = ECS::GetComponent<EndConditions>(MainEntities::MainPlayer());
	auto& stun = ECS::GetComponent<Sprite>(stunIcon);
	auto& sign = ECS::GetComponent<Sprite>(bonk);
	auto& endGame = ECS::GetComponent<Sprite>(timesUp);

	if (points.GetStun() == true)
	{
		stunTracker++;
		stun.SetTransparency(1.f);
		sign.SetTransparency(1.f);
	}

	if (stunTracker == 180)
	{
		points.SetStun(false);
		stun.SetTransparency(0.f);
		sign.SetTransparency(0.f);
		stunTracker = 0;
	}

	if (timer != 0 && currentTime < 90.f)
	{
		if (frames == 0)
		{
			if (fruitCounter == 3)
			{
				auto entity = ECS::CreateEntity();
				//float randomXVal = ((float)rand()) / ((float)RAND_MAX / (150.f - 20.f));

				auto randomSeed = std::default_random_engine(std::random_device{}());
				std::uniform_real_distribution range{ -115.f, 115.f };
				//float randomXVal = rand() % 220 - 115;
				float randomXVal = range(randomSeed);

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);
				ECS::AttachComponent<Trigger*>(entity);

				//Sets up the components
				std::string fileName = "Coconut.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));
				ECS::GetComponent<Trigger*>(entity) = new CoconutTrigger();
				ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(entity);

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_dynamicBody;
				tempDef.position.Set(float32(randomXVal), float32(70.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), true, OBJECTS, PLAYER | TRIGGER, 0.3f);

				tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
				fruitCounter = 0;
			}

			else
			{
				auto entity = ECS::CreateEntity();
				auto randomSeed = std::default_random_engine(std::random_device{}());
				std::uniform_real_distribution range{ -115.f, 115.f };
				//float randomXVal = rand() % 220 - 115;
				float randomXVal = range(randomSeed);

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);
				ECS::AttachComponent<Trigger*>(entity);

				//Sets up the components
				std::string fileName = "Peach.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));
				ECS::GetComponent<Trigger*>(entity) = new PeachTrigger();
				ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(entity);

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_dynamicBody;
				tempDef.position.Set(float32(randomXVal), float32(70.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), true, OBJECTS, PLAYER | TRIGGER, 0.3f);

				tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
			}

			fruitCounter++;
			frames++;
		}

		else if (frames >= 1 && frames < 50)
		{
			frames++;
		}

		else if (frames == 50)
		{
			frames = 0;
		}
	}
	if (timer != 0 && currentTime > 91.f)
	{
		if (points.GetPeachPoints() >= 35)
		{
			SetFlag(true, 2);
		}
		endGame.SetTransparency(1.f);
		gameOver = true;
	}

}

void FruitGame::UpdateSprites()
{
	float currentTime = Timer::StopWatch(timer);

	auto& minutesSprite = ECS::GetComponent<Sprite>(timerMinutes);
	auto& secondsTens = ECS::GetComponent<Sprite>(timerSecondsTens);
	auto& secondsOnes = ECS::GetComponent<Sprite>(timerSecondsOnes);

	const float timing = 1 / 60.f;
	static float remainder = 0.f;
	static int counter = 0;

	//checks if timer has started and if it hasn't or if game is over, returns the function without doing anything
	if (timer == 0 || currentTime > 91) { return; }

	//increasing remainder by delta time
	remainder += Timer::deltaTime;

	//checking if remainder is greater than or equal to the time of a frame
	while (remainder >= timing)
	{
		//subtracting the time of a frame from remainder
		remainder -= timing;
		//increasing frame count by one
		counter++;
	}

	if (counter >= 60)
	{
		std::string time = "";

		switch (onesCount)
		{
		case 0:
			time = TimerStrings("9");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 9;
			tens = true;
			break;

		case 9:
			time = TimerStrings("8");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 8;
			break;

		case 8:
			time = TimerStrings("7");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 7;
			break;

		case 7:
			time = TimerStrings("6");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 6;
			break;

		case 6:
			time = TimerStrings("5");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 5;
			break;

		case 5:
			time = TimerStrings("4");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 4;
			break;

		case 4:
			time = TimerStrings("3");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 3;
			break;

		case 3:
			time = TimerStrings("2");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 2;
			break;

		case 2:
			time = TimerStrings("1");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 1;
			break;

		case 1:
			time = TimerStrings("0");
			secondsOnes.LoadSprite(time, 5, 8);
			onesCount = 0;
			break;
		}

		if (tens == true)
		{
			switch (tensCount)
			{
			case 0:
				time = TimerStrings("5");
				secondsTens.LoadSprite(time, 5, 8);
				tensCount = 5;
				minutes = true;
				break;

			case 5:
				time = TimerStrings("4");
				secondsTens.LoadSprite(time, 5, 8);
				tensCount = 4;
				break;

			case 4:
				time = TimerStrings("3");
				secondsTens.LoadSprite(time, 5, 8);
				tensCount = 3;
				break;

			case 3:
				time = TimerStrings("2");
				secondsTens.LoadSprite(time, 5, 8);
				tensCount = 2;
				break;

			case 2:
				time = TimerStrings("1");
				secondsTens.LoadSprite(time, 5, 8);
				tensCount = 1;
				break;

			case 1:
				time = TimerStrings("0");
				secondsTens.LoadSprite(time, 5, 8);
				tensCount = 0;
				break;
			}
		}

		if (minutes == true)
		{
			switch (minutesCount)
			{
			case 0:
				time = TimerStrings("6");
				minutesSprite.LoadSprite(time, 5, 8);
				minutesCount = 6;
				break;

			case 6:
				time = TimerStrings("5");
				minutesSprite.LoadSprite(time, 5, 8);
				minutesCount = 5;
				break;

			case 5:
				time = TimerStrings("4");
				minutesSprite.LoadSprite(time, 5, 8);
				minutesCount = 4;
				break;

			case 4:
				time = TimerStrings("3");
				minutesSprite.LoadSprite(time, 5, 8);
				minutesCount = 3;
				break;

			case 3:
				time = TimerStrings("2");
				minutesSprite.LoadSprite(time, 5, 8);
				minutesCount = 2;
				break;

			case 2:
				time = TimerStrings("1");
				minutesSprite.LoadSprite(time, 5, 8);
				minutesCount = 1;
				break;

			case 1:
				time = TimerStrings("0");
				minutesSprite.LoadSprite(time, 5, 8);
				minutesCount = 0;
				break;
			}
		}
		counter -= 60;
		minutes = false;
		tens = false;
	}
}


void FruitGame::UpdatePoints()
{
	auto& playerPoints = ECS::GetComponent<PlayerPoints>(MainEntities::MainPlayer());
	auto& pTens = ECS::GetComponent<Sprite>(pointsTens);
	auto& pOnes = ECS::GetComponent<Sprite>(pointsOnes);

	if (playerPoints.GetPointGain() == true)
	{
		std::string points = "";
		switch (pointsOnesCount)
		{
		case 0:
			points = TimerStrings("1");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 1;
			break;

		case 1:
			points = TimerStrings("2");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 2;
			break;

		case 2:
			points = TimerStrings("3");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 3;
			break;

		case 3:
			points = TimerStrings("4");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 4;
			break;

		case 4:
			points = TimerStrings("5");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 5;
			break;

		case 5:
			points = TimerStrings("6");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 6;
			break;

		case 6:
			points = TimerStrings("7");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 7;
			break;

		case 7:
			points = TimerStrings("8");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 8;
			break;

		case 8:
			points = TimerStrings("9");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 9;
			break;

		case 9:
			points = TimerStrings("0");
			pOnes.LoadSprite(points, 5, 8);
			pointsOnesCount = 0;
			pointsTensFlag = true;
			break;
		}

		if (pointsTensFlag == true)
		{
			switch (pointsTensCount)
			{
			case 0:
				points = TimerStrings("1");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 1;
				break;

			case 1:
				points = TimerStrings("2");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 2;
				break;

			case 2:
				points = TimerStrings("3");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 3;
				break;

			case 3:
				points = TimerStrings("4");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 4;
				break;

			case 4:
				points = TimerStrings("5");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 5;
				break;

			case 5:
				points = TimerStrings("6");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 6;
				break;

			case 6:
				points = TimerStrings("7");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 7;
				break;

			case 7:
				points = TimerStrings("8");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 8;
				break;

			case 8:
				points = TimerStrings("9");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 9;
				break;

			case 9:
				points = TimerStrings("0");
				pTens.LoadSprite(points, 5, 8);
				pointsTensCount = 0;
				break;
			}
		}
		playerPoints.SetPointGain(false);
		pointsTensFlag = false;
	}
}

void FruitGame::UpdateCamera()
{
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

std::string FruitGame::TimerStrings(std::string digit)
{
	std::string base = "Digit";
	std::string ones = digit;
	std::string time = base + ones + ".png";
	return time;
}


