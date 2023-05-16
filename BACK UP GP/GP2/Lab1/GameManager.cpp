#include "GameManager.h"
#include "MainCamera.h"
#include <iostream>
#include <string>
#define TICK_INTERVAL 30;

Transform transform;

GameManager::GameManager()
{
	// init variables
	offset = 0.0;
	scale = 1;

	counter = 0;
	gameState = GameState::PLAY;
	DisplayGame* gameDisplay = new DisplayGame(); //new display

	counter = 1.0f;
	collsionCounter = 1.0f;

	x = 0;
	y = 0;
	preX = 0;
	preY = 0;
}

GameManager::~GameManager()
{
}

// function called when game starts
void GameManager::run()
{
	SystemsStart();
	GameActive();
}

// initalizes shader, textures, meshes, sounds 
void GameManager::SystemsStart()
{
	gameDisplay.InitalizeDisplay();

	geoShader.initGeo();
    // audio 
	gameAudio.AddAudio("..\\res\\background.wav");
	gameAudio.AddSound("..\\res\\shoot.wav");
//	gameAudio.PlayAudio();

	// loads in models
	cube.ModelLoader("..\\res\\cube.obj");
	tree.ModelLoader("..\\res\\WoodenLog_obj.obj");
	apple.ModelLoader("..\\res\\Apple_obj.obj");

	mainCamera.InitializeCamera(glm::vec3(0, 0, -5), glm::radians(mainCamera.fov), (float)gameDisplay.getX()/gameDisplay.getY(), 0.01f, 1000.0f);

	// initalizes shaders
	shader.InitalizeShader("..\\res\\shader.vert", "..\\res\\shader.frag");
	cubemapShader.InitalizeShader("..\\res\\cubemapShader.vert", "..\\res\\cubemapShader.frag");
	reflectionShader.InitalizeShader("..\\res\\reflectionShader.vert", "..\\res\\reflectionShader.frag");
	refractionShader.InitalizeShader("..\\res\\refractionShader.vert", "..\\res\\refractionShader.frag");
	mixShader.InitalizeShader("..\\res\\mix.vert", "..\\res\\mix.frag");
	emapShader.InitalizeShader("..\\res\\environmentMap.vert", "..\\res\\environmentMap.frag");
	waterShader.InitalizeShader("..\\res\\water.vert", "..\\res\\water.frag");


	// initalizes cubemap/cube functions
	cubeMap.InitalizeCubeMap();
	cubeMap.CubeVertexArrayObject();

	// loads and initalizes textures
	tarmacTex.TextureLoader("..\\res\\TarmacDark_D.jpg"); //load texture
	treeTex.TextureLoader("..\\res\\WoodenLog_Diffuse_8K.jpg");
	appleTex.TextureLoader("..\\res\\apple l_Material.003_BaseColor.png");
	noise.TextureLoader("..\\res\\waternoise.png");
	texture1.TextureLoader("..\\res\\sky.jpg");
	water.TextureLoader("..\\res\\water1.jpg");
}


// used to regulate frame rate
static Uint32 next_time;

Uint32 time_left(void)
{
	Uint32 now;

	now = SDL_GetTicks();

	if (next_time <= now)
		return 0;
	else
		return next_time - now;
}

void GameManager::GameActive()
{
	next_time = SDL_GetTicks() + TICK_INTERVAL;
	// if game isnt closed then constantly function calls fucntiosn that need to be updated regulary
	while (gameState != GameState::EXIT)
	{
		ProcessInputs();
		DrawGame();
		IsColliding(apple, tree);

		// used for regulating framerate
		SDL_Delay(time_left());
		next_time += TICK_INTERVAL;

		// locks mouse in window and hide it 
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}

void GameManager::ProcessInputs()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) //get and process events
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gameState = GameState::EXIT;
			break;
		case SDL_MOUSEWHEEL:
		{
			// when player used scroll wheel
			if (event.wheel.y > 0 && mainCamera.fov != 0)			
			{			
				// increases fov
				mainCamera.Zoom(-1);
				// updates fov 
				mainCamera.ChangeFOV(glm::radians(mainCamera.fov), (float)gameDisplay.getX() / gameDisplay.getY(), 0.01f, 1000.0f);
			}
		    if (event.wheel.y < 0)
			{
				// decreases fov
				mainCamera.Zoom(1);
				// updates fov 
				mainCamera.ChangeFOV(glm::radians(mainCamera.fov), (float)gameDisplay.getX() / gameDisplay.getY(), 0.01f, 1000.0f);
			}
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			// when player inputs mouse buttons objects in scene moves 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				offset += 1;
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				offset -= 1;
			}
			break;
		}
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				// players wasd, shift, space movement 
			case SDLK_w:
				mainCamera.MoveForward(1);
				break;
			case SDLK_s:
				mainCamera.MoveForward(-1);
				break;
			case SDLK_d:
				mainCamera.MoveHoriz(1);
				break;
			case SDLK_a:
				mainCamera.MoveHoriz(-1);
				break;
			case SDLK_SPACE:
				mainCamera.MoveVertical(1);
				break;
			case SDLK_LSHIFT:
				mainCamera.MoveVertical(-1);
				break;
				// can adjust object size
			case SDLK_e:
				scale += .1;
				break;
			case SDLK_q:
				if (scale > 0)
				{
					scale -= .1;
				}
				break;
				// reset camera pos 
			case SDLK_v:
				mainCamera.InitializeCamera(glm::vec3(0, 0, -5), glm::radians(mainCamera.fov), (float)gameDisplay.getX() / gameDisplay.getY(), 0.01f, 1000.0f);
				break;
			}
		case SDL_MOUSEMOTION:
		{
			// stores the x and y position of screen space
			x = event.motion.x;
			y = event.motion.y;
			// checks to see if the x value has increased or decreases from last pos
			if (x > preX || x >= (gameDisplay.getX()) - 1)
			{
				mainCamera.RotateX(-0.01);
			}
			else if (x < preX || x == 0)
			{
				mainCamera.RotateX(0.01);
			}
			// checks to see if the y value has increased or decreases from last pos
			if (y > preY || preY >= (gameDisplay.getY()) - 1)
			{
				mainCamera.RotateY(0.01);
			}
			if (y < preY || y == 0)
			{
				mainCamera.RotateY(-0.01);
			}
			// gives current pos to variables
			preX = x;
			preY = y;
		}
		}
    }
}

// reads in 2 meshes and checks if they are colliding
bool GameManager::IsColliding(MeshManager& mesh, MeshManager& mesh1)
{

	float distance = 
		((mesh1.getSpherePos().x - mesh.getSpherePos().x) * (mesh1.getSpherePos().x - mesh.getSpherePos().x) 
		+ (mesh1.getSpherePos().y - mesh.getSpherePos().y) * (mesh1.getSpherePos().y - mesh.getSpherePos().y) 
		+ (mesh1.getSpherePos().z - mesh.getSpherePos().z) * (mesh1.getSpherePos().z - mesh.getSpherePos().z));
	if (distance * distance < (mesh.getSphereRad() + mesh1.getSphereRad()))
	{	
    	collsionCounter *= 0;
		gameAudio.PlaySound(0);
		return true;
	}
		return false;
}


void GameManager::Spinning()
{
	// sets pos, rotation, scale
	transform.SetPos(glm::vec3(-sinf(collsionCounter), 0, -sinf(collsionCounter) * 5));
	transform.SetRot(glm::vec3(0.0, 0.0, 0/*counter * 5*/));
	transform.SetScale(glm::vec3(scale, scale, scale));

	// binds texture
	appleTex.BindTexture(0);

	// bind shader and updates 
	shader.Bind();
	shader.UpdateShader(transform, mainCamera);

	// updates col
	apple.UpdateColData(*transform.GetPos(), scale);
	
	// draws object 
	apple.Draw();
}

void GameManager::Ground()
{
	transform.SetPos(glm::vec3(10, -4, -15));
	transform.SetScale(glm::vec3(50, 1 ,50));

	tarmacTex.BindTexture(0);

	shader.Bind();
	shader.UpdateShader(transform, mainCamera);

	cube.UpdateColData(*transform.GetPos(), 1);

	cube.Draw();
}

void GameManager::Tree()
{
		transform.SetPos(glm::vec3(5 + offset, 0.0,0));
		transform.SetRot(glm::vec3(0.0, counter * 2, 0));
     	transform.SetScale(glm::vec3(scale, scale, scale));

		treeTex.BindTexture(0);

		geoShader.Bind();

		geoShader.UpdateShader(transform, mainCamera);

		geoShader.setFloat("time", counter / 2);

		tree.UpdateColData(*transform.GetPos(), scale);

		tree.Draw();
}

void GameManager::DrawSkyBox()
{
	transform.SetPos(glm::vec3(0.0, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(100, 100, 100));

	cubemapShader.Bind();
	cubemapShader.UpdateCubemap(mainCamera);
	cubeMap.DrawCubemap();
}

void GameManager::DrawRefraction()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	transform.SetPos(glm::vec3(2, 0.0, 2));
	transform.SetRot(glm::vec3(0.0, 0, 0));
	transform.SetScale(glm::vec3(1, 1, 1));

	refractionShader.Bind();

	refractionShader.setMat4("view", mainCamera.GetView());
	refractionShader.setMat4("projection", mainCamera.GetProjection());
	refractionShader.setMat4("model", transform.GetModel());
	refractionShader.setVec3("cameraPos", mainCamera.getPosition());

	refractionShader.UpdateShader(transform, mainCamera);

	cubeMap.DrawCube();
}

void GameManager::DrawReflection()
{
	transform.SetPos(glm::vec3(4, 0.0, 2));
	transform.SetRot(glm::vec3(0.0, counter , 0));
	transform.SetScale(glm::vec3(1, 1, 1));

	reflectionShader.Bind();

	reflectionShader.setMat4("view", mainCamera.GetView());
	reflectionShader.setMat4("projection", mainCamera.GetProjection());
	reflectionShader.setMat4("model", transform.GetModel());
	reflectionShader.setVec3("cameraPos", mainCamera.getPosition());

	reflectionShader.UpdateShader(transform, mainCamera);
	cubeMap.DrawCube();
}

void GameManager::DrawMix()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	transform.SetPos(glm::vec3(6, 0.0, 2));
	transform.SetRot(glm::vec3(0.0, counter, 0));
	transform.SetScale(glm::vec3(1, 1, 1));

	mixShader.Bind();

	mixShader.setMat4("view", mainCamera.GetView());
	mixShader.setMat4("projection", mainCamera.GetProjection());
	mixShader.setMat4("model", transform.GetModel());
	mixShader.setVec3("cameraPos", mainCamera.getPosition());
	mixShader.setVec3("waterColor", glm::vec3(0.5,0.5,0.5));

	mixShader.UpdateShader(transform, mainCamera);

	cubeMap.DrawCube();

}

void GameManager::DrawEMap()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	transform.SetPos(glm::vec3(-2, 0.0, 0));
	transform.SetRot(glm::vec3(0.0, 0, 0));
	transform.SetScale(glm::vec3(scale, scale, scale));

	emapShader.Bind();
	emapShader.setMat4("model", transform.GetModel());
	emapShader.setVec3("cameraPos", mainCamera.getPosition());

	GLuint blur = glGetUniformLocation(emapShader.getID(), "diffuse"); 

	//set textures
	glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, tarmacTex.getID());
	glUniform1i(blur, 1);

	emapShader.UpdateShader(transform, mainCamera);

	cube.Draw();
}

void GameManager::DrawWater()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	transform.SetPos(glm::vec3(1, 0, 0));
	transform.SetRot(glm::vec3(0.0, 0, 0));
	transform.SetScale(glm::vec3(scale, scale, scale));

	waterShader.Bind();
	waterShader.setMat4("model", transform.GetModel());
	waterShader.setVec3("cameraPos", mainCamera.getPosition());

	waterShader.setFloat("time", counter / 20);

	GLuint noiseTex = glGetUniformLocation(waterShader.getID(), "noiseTex");
	GLuint waterTex = glGetUniformLocation(waterShader.getID(), "water");

	//set textures
	glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, noise.getID());
	glUniform1i(noiseTex, 1);

	glActiveTexture(GL_TEXTURE2); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, water.getID());
	glUniform1i(waterTex, 2);

	waterShader.UpdateShader(transform, mainCamera);

	cube.Draw();
}


void GameManager::DrawGame()
{
	gameDisplay.ClearDisplay(0.5, 0.5, 0.5, 1.0);

   // Spinning();
	//Ground();
	Tree();
	//DrawRefraction();
	//DrawReflection();
	DrawSkyBox();
	//DrawMix();
	DrawEMap();
	DrawWater();

	// counter is adjusted 
	counter = counter + 0.05f;
	collsionCounter = collsionCounter + 0.05f;


	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	gameDisplay.ChangeBuffer();
} 