#include "Cafe.h"
#include "Utilities.h"
#include "Timer.h"
#include <random>

//gravity
Cafe::Cafe(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);	
}
//register stuff
void Cafe::InitScene(float windowWidth, float windowHeight)
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

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;
	//std::cout << "Hello \n";
	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		/*ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);*/

		//vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);//original
		vec4 temp = vec4(-70.f, 70.f, -70.f, 100.f);//alternate
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		/*ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));*/
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

		//Set up components
		std::string fileName = "spritesheets/barista.png";
		std::string animations = "cafeBoy.json";
		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 20, 30, &ECS::GetComponent<Sprite>(entity),
			&ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 6.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	//cafe reference layout
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		toggleLayout = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "layout.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 626/3, 576/3);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 20.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//cafe floor
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "floor.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 838 / 3.5, 671 / 3.5);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 15.f, 0.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	//left counter
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "leftcounter.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, (129 / 5) - 3, 561 / 5);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-92.f, 15.f, 3.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-92.f), float32(15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//front counter
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "frontcounter.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, (1208 / 6) + 5, 219 / 6);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -40.f, 3.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(-40.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//right counter
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "rightcounter.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, (137 / 5) - 3, 561 / 5);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(91.f, 15.f, 3.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(91.f), float32(15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//wall
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "wall.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, (1208 / 6)+5, 225 / 5);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 70.f, 1.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(80.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() / 4 - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}

	//cupboard
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "cupboard.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 324 / 6, 325 / 6);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-54.f, 65.f, 2.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	//blend station
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "blendstation.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 303/8, 144/8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-60.f, -28.f, 5.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	//milk station
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "milkstation.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128 / 8, 185 / 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-23.f, -25.f, 7.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	//sugar station
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "sugarstation.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 168 / 7, 129 / 7);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(10.f, -28.f, 7.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	//cup station
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "cupstation.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 293 / 7, 168 / 7);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(54.f, -25.f, 7.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	//cash register
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "cashregister.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 136 / 6, 149 / 6);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(89.f, 23.f, 7.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	//Setup blend station trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new PosCheck();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);  
		ECS::GetComponent<Trigger*>(entity)->SetFlag(0);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-10.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(10.f - shrinkX), float(10.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(-65, -20));
	}

	//Setup milk station trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new PosCheck();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetFlag(1);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(10.f - shrinkX), float(10.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(-20, -20));
	}
	
	//Setup sugar station trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new PosCheck();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetFlag(2);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(10.f - shrinkX), float(10.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(20, -20));
	}

	//Setup cup station trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new PosCheck();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetFlag(3);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(10.f - shrinkX), float(10.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(60, -20));
	}

	//Setup register trigger
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "boxSprite.jpg";
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new PosCheck();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(player);
		ECS::GetComponent<Trigger*>(entity)->SetFlag(4);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(10.f - shrinkX), float(10.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		tempPhsBody.SetPosition(b2Vec2(75, 20));
	}

	//Setup cup signs
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		smallCup = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "small.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(60.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		mediumCup = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "medium.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(60.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		largeCup = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "large.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(60.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//setup milk signs
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		m_none = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "none.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-22.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		m_aLittleBit = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "alittle.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-22.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		m_regular = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "regular.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-22.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		m_lots = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "lots.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-22.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//setup blend signs
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		blueMountainBlend = entity;

		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "bluemountain.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-62.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		kilimonjaroBlend = entity;

		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "kilimanjaro.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-62.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		mochaBlend = entity;

		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "mocha.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-62.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	//setup sugar signs
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		noneSpoon = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "none.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(13.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		oneSpoon = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "onespoon.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(13.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		twoSpoon = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "twospoons.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(13.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		threeSpoon = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "threespoons.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 10);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(13.f, -40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}

	//setup order log
	//blend
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		blendOrder = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "bluemountain.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 15);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-90.f, 40.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}
	//milk
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		milkOrder = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "none.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 15);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-90.f, 20.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}
	//sugar
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		sugarOrder = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "none.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 15);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-90.f, 0.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}
	//cups
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		cupOrder = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "small.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 15);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-90.f, -20.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	}

	//Time's Up Sign
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		timesUpSign = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "timesUp.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 50);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 20.f, 10.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
	}
	
	//Minutes
	{
		auto entity = ECS::CreateEntity();
		timerMinutes = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Digit1.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 40.f, 50.f));
	}

	//Colon
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Colon.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(56.f, 40.f, 50.f));
	}

	//Seconds (10s)
	{
		auto entity = ECS::CreateEntity();
		timerSecondsTens = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Digit3.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(62.f, 40.f, 50.f));
	}

	//Seconds (1s)
	{
		auto entity = ECS::CreateEntity();
		timerSecondsOnes = entity;

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Digit0.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(68.f, 40.f, 50.f));
	}

	//Points(ten)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		pointTens = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		
		std::string fileName = "Digit0.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(90.f, 80.f, 50.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		std::cout << fileName;
	}
	//Points(one)
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		pointOnes = entity;
		//set up components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Digit0.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 5, 8);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(98.f, 80.f, 50.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		std::cout << fileName;
	}

	/*ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));*/
}

//update
void Cafe::Update()
{
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());

	
	//Scene::AdjustScrollOffset();
	player.Update();
	//std::cout << currentTime << "\n";
	if (currentTime > 90.f)
	{
		gameOver = true;
	}
	
	if (gameOver == true)
	{
		ECS::GetComponent<Sprite>(timesUpSign).SetTransparency(1.f);
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempBody = m_physicsWorld->CreateBody(&tempDef);
		ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetBody(tempBody);//sets the player's body to a static body
	}
	if (start == true)
	{
		CurrentTime();
	}
	UpdateSprites();
}



void Cafe::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	float speed = 1.f;
	vec3 vel = vec3(0.f, 0.f, 0.f);
	if (gameOver == false && start == true)
	{
		if (Input::GetKey(Key::Shift))
		{
			speed *= 10.f;
		}

		if (Input::GetKey(Key::A))//left
		{
			//player.GetBody()->ApplyForceToCenter(b2Vec2(-400.f * speed, 0.f), true);
			vel = vel + vec3(-50.f, 0.f, 0.f);
		}
		if (Input::GetKey(Key::D))//right
		{
			//player.GetBody()->ApplyForceToCenter(b2Vec2(400.f * speed, 0.f), true);
			vel = vel + vec3(50.f, 0.f, 0.f);
		}
		if (Input::GetKey(Key::W))//up
		{
			//player.GetBody()->ApplyForceToCenter(b2Vec2(0.f, 400.f * speed), true);
			vel = vel + vec3(0.f, 50.f, 0.f);
		}
		if (Input::GetKey(Key::S))//down
		{
			//player.GetBody()->ApplyForceToCenter(b2Vec2(0.f, -400.f * speed), true);
			vel = vel + vec3(0.f, -50.f, 0.f);
		}
		player.SetVelocity(vel * speed);
	}
}

void Cafe::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& stuff = ECS::GetComponent<CollisionFlagClass>(MainEntities::MainPlayer());
	bool triggered = false;
	std::string smallFile = "cupSmall.jpg";
	std::string mediumFile = "cupMedium.jpg";
	std::string largeFile = "cupLarge.jpg";

	std::string milk1File = "none.jpg";
	std::string milk2File = "aLittleBit.jpg";
	std::string milk3File = "regular.jpg";
	std::string milk4File = "lots.jpg";

	std::string blend1 = "blueMountain.jpg";
	std::string blend2 = "kilimanjaro.jpg";
	std::string blend3 = "mocha.jpg";

	std::string sugar1 = "none.jpg";
	std::string sugar2 = "oneSpoon.jpg";
	std::string sugar3 = "twoSpoon.jpg";
	std::string sugar4 = "threeSpoon.jpg";

	if (Input::GetKeyDown(Key::X))
	{
		std::cout << Timer::StopWatch(timer) << "\n";
	}

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	/*if (Input::GetKey(Key::One))
	{
		ECS::GetComponent<Sprite>(toggleLayout).SetTransparency(0.f);
	}
	if (Input::GetKeyDown(Key::Two))
	{
		ECS::GetComponent<Sprite>(toggleLayout).SetTransparency(0.5f);
	}*/
	if (gameOver == false && start == true)
	{
		if (Input::GetKeyDown(Key::E) && stuff.GetCollisionBlend() == true)
		{
			{
				if (bBlend == true)
				{
					ECS::GetComponent<Sprite>(blueMountainBlend).SetTransparency(0.f);
					ECS::GetComponent<Sprite>(kilimonjaroBlend).SetTransparency(1.f);
					ECS::GetComponent<Sprite>(mochaBlend).SetTransparency(0.f);

					bBlend = false;
					kBlend = true;
					mBlend = false;
					beverage[0] = 2;
				}
				else if (kBlend == true)
				{
					ECS::GetComponent<Sprite>(blueMountainBlend).SetTransparency(0.f);
					ECS::GetComponent<Sprite>(kilimonjaroBlend).SetTransparency(0.f);
					ECS::GetComponent<Sprite>(mochaBlend).SetTransparency(1.f);

					bBlend = false;
					kBlend = false;
					mBlend = true;
					beverage[0] = 3;
				}
				else if (mBlend == true)
				{
					ECS::GetComponent<Sprite>(blueMountainBlend).SetTransparency(1.f);
					ECS::GetComponent<Sprite>(kilimonjaroBlend).SetTransparency(0.f);
					ECS::GetComponent<Sprite>(mochaBlend).SetTransparency(0.f);

					bBlend = true;
					kBlend = false;
					mBlend = false;
					beverage[0] = 1;
				}
			}
			for (int i = 0; i < 4; i++)
			{
				std::cout << beverage[i] << " ";
			}
			std::cout << "\n";
		}
		if (Input::GetKeyDown(Key::E) && stuff.GetCollisionMilk() == true)
		{
			if (milk1 == true)
			{
				ECS::GetComponent<Sprite>(m_none).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(m_aLittleBit).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(m_regular).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(m_lots).SetTransparency(0.f);

				milk1 = false;
				milk2 = true;
				milk3 = false;
				milk4 = false;
				beverage[1] = 2;

			}
			else if (milk2 == true)
			{
				ECS::GetComponent<Sprite>(m_none).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(m_aLittleBit).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(m_regular).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(m_lots).SetTransparency(0.f);

				milk1 = false;
				milk2 = false;
				milk3 = true;
				milk4 = false;
				beverage[1] = 3;

			}
			else if (milk3 == true)
			{
				ECS::GetComponent<Sprite>(m_none).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(m_aLittleBit).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(m_regular).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(m_lots).SetTransparency(1.f);

				milk1 = false;
				milk2 = false;
				milk3 = false;
				milk4 = true;
				beverage[1] = 4;

			}
			else if (milk4 == true)
			{
				ECS::GetComponent<Sprite>(m_none).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(m_aLittleBit).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(m_regular).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(m_lots).SetTransparency(0.f);

				milk1 = true;
				milk2 = false;
				milk3 = false;
				milk4 = false;
				beverage[1] = 1;
			}
			for (int i = 0; i < 4; i++)
			{
				std::cout << beverage[i] << " ";
			}
			std::cout << "\n";
		}
		if (Input::GetKeyDown(Key::E) && stuff.GetCollisionSugar() == true)
		{
			if (b_noneSpoon == true)
			{
				ECS::GetComponent<Sprite>(noneSpoon).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(oneSpoon).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(twoSpoon).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(threeSpoon).SetTransparency(0.f);

				b_noneSpoon = false;
				b_oneSpoon = true;
				b_twoSpoon = false;
				b_threeSpoon = false;
				beverage[2] = 2;
			}
			else if (b_oneSpoon == true)
			{
				ECS::GetComponent<Sprite>(noneSpoon).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(oneSpoon).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(twoSpoon).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(threeSpoon).SetTransparency(0.f);

				b_noneSpoon = false;
				b_oneSpoon = false;
				b_twoSpoon = true;
				b_threeSpoon = false;
				beverage[2] = 3;
			}
			else if (b_twoSpoon == true)
			{
				ECS::GetComponent<Sprite>(noneSpoon).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(oneSpoon).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(twoSpoon).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(threeSpoon).SetTransparency(1.f);

				b_noneSpoon = false;
				b_oneSpoon = false;
				b_twoSpoon = false;
				b_threeSpoon = true;
				beverage[2] = 4;
			}
			else if (b_threeSpoon == true)
			{
				ECS::GetComponent<Sprite>(noneSpoon).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(oneSpoon).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(twoSpoon).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(threeSpoon).SetTransparency(0.f);

				b_noneSpoon = true;
				b_oneSpoon = false;
				b_twoSpoon = false;
				b_threeSpoon = false;
				beverage[2] = 1;
			}
			for (int i = 0; i < 4; i++)
			{
				std::cout << beverage[i] << " ";
			}
			std::cout << "\n";
		}
		if (Input::GetKeyDown(Key::E) && stuff.GetCollisionCups() == true)
		{
			if (sC == true)
			{
				ECS::GetComponent<Sprite>(smallCup).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(mediumCup).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(largeCup).SetTransparency(0.f);
				sC = false;
				mC = true;
				lC = false;
				beverage[3] = 2;
			}
			else if (mC == true)
			{
				ECS::GetComponent<Sprite>(smallCup).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(mediumCup).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(largeCup).SetTransparency(1.f);
				sC = false;
				mC = false;
				lC = true;
				beverage[3] = 3;
			}
			else if (lC == true)
			{
				ECS::GetComponent<Sprite>(smallCup).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(mediumCup).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(largeCup).SetTransparency(0.f);
				sC = true;
				mC = false;
				lC = false;
				beverage[3] = 1;
			}
			for (int i = 0; i < 4; i++)
			{
				std::cout << beverage[i] << " ";
			}
			std::cout << "\n";
		}
		if (Input::GetKeyDown(Key::E) && stuff.GetCollisionRegister() == true)
		{
			std::string tempName;
			bool correctOrder = true;
			for (int i = 0; i < 4; i++)
			{
				if (beverage[i] != custOrder[i])
				{
					correctOrder = false;
				}
			}
			if (correctOrder == true)
			{
				std::cout << "The order is correct\n";

				//creates new order
				int orderIndex = 0;
				srand(time(0) + rand());

				custOrder[0] = rand() % 4;
				custOrder[1] = rand() % 5;
				custOrder[2] = rand() % 5;
				custOrder[3] = rand() % 4;

				while (custOrder[0] == 0)
				{
					custOrder[0] = rand() % 4;
				}
				while (custOrder[1] == 0)
				{
					custOrder[1] = rand() % 4;
				}
				while (custOrder[2] == 0)
				{
					custOrder[2] = rand() % 4;
				}
				while (custOrder[3] == 0)
				{
					custOrder[3] = rand() % 4;
				}
				tempName = blendNames[custOrder[0] - 1];
				ECS::GetComponent<Sprite>(blendOrder).LoadSprite(tempName, 30, 15);
				tempName = milkNames[custOrder[1] - 1];
				ECS::GetComponent<Sprite>(milkOrder).LoadSprite(tempName, 30, 15);
				tempName = sugarNames[custOrder[2] - 1];
				ECS::GetComponent<Sprite>(sugarOrder).LoadSprite(tempName, 30, 15);
				tempName = cupNames[custOrder[3] - 1];
				ECS::GetComponent<Sprite>(cupOrder).LoadSprite(tempName, 30, 15);

				std::cout << "New Order: " << custOrder[0] << custOrder[1] << custOrder[2] << custOrder[3];
				std::cout << "\n";

				ordersComplete += 1;//increases the number of orders the player has successfully completed by 1.
				one += 1;
				UpdatePoints();
			}
			else
			{
				std::cout << "The order is wrong\n";
			}
		}
	}
	if (gameOver == true)//exit mini-game
	{
		if (Input::GetKeyDown(Key::Space))
		{
			Scene::SetSceneChange(true, 5);
		}
	}
	if (gameOver == true && repeat == true)
	{
		std::cout << "You have completed: " << ordersComplete << " orders!\n";
		repeat = false;
	}
	if (Input::GetKeyDown(Key::Enter) && start == false)
	{
		start = true;
		if (timer == 0)
		{
			timer = Timer::time;


			SDL_LoadWAV("CafeBGM.wav", &wavSpec, &wavBuffer, &wavLength);
			deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
			int success = SDL_QueueAudio(deviceID, wavBuffer, wavLength);
			SDL_PauseAudioDevice(deviceID, 0);
		}
	}
	//manual point increase
	if (Input::GetKeyDown(Key::N))
	{
		ordersComplete += 1;//increases the number of orders the player has successfully completed by 1.
		one += 1;
		UpdatePoints();
	}
	//manual scene swicth
	if (Input::GetKeyDown(Key::Y))
	{
		SDL_CloseAudioDevice(deviceID);
		Scene::SetSceneChange(true, 4);
	}
}

void Cafe::CurrentTime()
{
	currentTime = Timer::StopWatch(timer);
}

void Cafe::UpdateSprites()
{
	float currentTime = Timer::StopWatch(timer);
	auto& minutesSprite = ECS::GetComponent<Sprite>(timerMinutes);
	auto& secondsTens = ECS::GetComponent<Sprite>(timerSecondsTens);
	auto& secondsOnes = ECS::GetComponent<Sprite>(timerSecondsOnes);

	const float timing = 1 / 60.f;
	static float remainder = 0.f;
	static int counter = 0;

	if (timer == 0 || currentTime > 91){return;}

		remainder += Timer::deltaTime;
		while (remainder >= timing)
		{
			remainder -= timing;
			counter++;
		}

		if(counter >= 60)
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


//updates the points counter
void Cafe::UpdatePoints()
{
	auto& oneSprite = ECS::GetComponent<Sprite>(pointOnes);
	auto& tenSprite = ECS::GetComponent<Sprite>(pointTens);
	std::string digit = "";

	switch (one) {
	case 0:
		digit = TimerStrings("0");
		oneSprite.LoadSprite(digit, 5, 8);
		ten += 1;
		break;
	case 1:
		digit = TimerStrings("1");
		oneSprite.LoadSprite(digit, 5, 8);
		break;
	case 2:
		digit = TimerStrings("2");
		oneSprite.LoadSprite(digit, 5, 8);
		break;
	case 3:
		digit = TimerStrings("3");
		oneSprite.LoadSprite(digit, 5, 8);
		break;
	case 4:
		digit = TimerStrings("4");
		oneSprite.LoadSprite(digit, 5, 8);
		break;
	case 5:
		digit = TimerStrings("5");
		oneSprite.LoadSprite(digit, 5, 8);
		break;
	case 6:
		digit = TimerStrings("6");
		oneSprite.LoadSprite(digit, 5, 8);
		break;
	case 7:
		digit = TimerStrings("7");
		oneSprite.LoadSprite(digit, 5, 8);
		break;
	case 8:
		digit = TimerStrings("8");
		oneSprite.LoadSprite(digit, 5, 8);
		break;
	case 9:
		digit = TimerStrings("9");
		oneSprite.LoadSprite(digit, 5, 8);
		one = -1;
		
		break;
	}

	switch (ten) {
	case 0:
		digit = TimerStrings("0");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	case 1:
		digit = TimerStrings("1");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	case 2:
		digit = TimerStrings("2");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	case 3:
		digit = TimerStrings("3");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	case 4:
		digit = TimerStrings("4");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	case 5:
		digit = TimerStrings("5");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	case 6:
		digit = TimerStrings("6");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	case 7:
		digit = TimerStrings("7");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	case 8:
		digit = TimerStrings("8");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	case 9:
		digit = TimerStrings("9");
		tenSprite.LoadSprite(digit, 5, 8);
		break;
	}
}

std::string Cafe::TimerStrings(std::string digit)
{
	std::string base = "Digit";
	std::string ones = digit;
	std::string time = base + ones + ".png";
	return time;
}