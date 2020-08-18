#include<stdio.h>
#include<glad.h>
#include<sdl.h>
#include<sdl_image.h>
#include<iostream>
#include "Shader.h"
#include "Math.h"
#include "Input.h"
#include "mouse.h"
#include "FlyCamera.h"

Uint32 lastTime = 0;    // �Ō�̃t���[������ int�~���b�P��
float  deltaTime = 0.0f; // �t���[���Ԃ̎��� �b�P�� ( 1.0 sec )

int main(int argc, char** argv)
{
	// SDL�̏�����
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("SDL���������s : %s\n", SDL_GetError());
		return -1;
	}

	// OpenGL �A�g���r���[�g�̃Z�b�g
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// GL version 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// 8Bit RGBA �`�����l��
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// �_�u���o�b�t�@�����O
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// �n�[�h�E�F�A�A�N�Z�����[�V����������
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Window�̍쐬
	SDL_Window* SDLWindow = SDL_CreateWindow("SDL & GL Window",
		100, 80, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!SDLWindow)
	{
		printf("Window�̍쐬�Ɏ��s: %s", SDL_GetError());
		return false;
	}

	// SDL Renderer�̍쐬
	SDL_Renderer* renderer = SDL_CreateRenderer(SDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		printf("SDLRenderer�̍쐬�Ɏ��s %s", SDL_GetError());
		return false;
	}

	SDL_GLContext context;
	context = SDL_GL_CreateContext(SDLWindow);

	// GLAD�̏�����
	const int version = gladLoadGL();
	if (version == 0)
	{
		printf("glad���������s�I\n");
	}

	glEnable(GL_DEPTH_TEST);

	// �摜��SDL�T�[�t�F�X�ɓǂݍ���
	SDL_Surface* surf = IMG_Load("images/nier.jpg");
	if (!surf)
	{
		printf("�t�@�C���ǂݍ��݂Ɏ��s");
		return -1;
	}

	// �摜�̕��E�������擾
	int width = surf->w;
	int	height = surf->h;
	int channels = surf->format->BytesPerPixel;

	// �摜�t�H�[�}�b�g�̔���
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// SDL�T�[�t�F�X����f�k�e�N�X�`���쐬���e��p�����[�^�Z�b�g
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// �e�N�X�`�����b�s���O���t�B���^�����O�ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// SDL�f�[�^��GL�̃e�N�X�`���f�[�^�Ƃ��ăR�s�[����
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
		GL_UNSIGNED_BYTE, surf->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	// SDL�T�[�t�F�X�͗p�ς݂Ȃ̂ŉ��
	SDL_FreeSurface(surf);

	////////////////////////////////////////////////////////////////////////
	// ���_��`(���C�g�ɏƂ炳��鑤�̃L���[�u) ���_�{���_�@��
	////////////////////////////////////////////////////////////////////////
	float cubevertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// ���_�z��@���_�o�b�t�@�@�G�������g�z��o�b�t�@
	unsigned int CubeVAO, CubeVBO;
	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &CubeVBO);

	// ���_�z��I�u�W�F�N�g(VAO)�ɁAVBO���֘A�t����
	glBindVertexArray(CubeVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubevertices), cubevertices, GL_STATIC_DRAW);
	}
	// �ʒu���� : 0 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// �@������ : 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	///////////////////////////////////////////////////////////// �L���[�u���_��`


	////////////////////////////////////////////////////////////////////////
	// ���_��`(������\���L���[�u)
	// �Ƃ炳��鑤�̉e�����󂯂����Ȃ��̂ŏd�����Ă��邪�ʂŒ�`
	////////////////////////////////////////////////////////////////////////
	float cubeVertices[] = {
		//x ,   y ,   z       Nx,    Ny,   Nz
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// ���_�z��@���_�o�b�t�@�@�G�������g�z��o�b�t�@
	unsigned int lightVAO, lightVBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	// ���_�z��I�u�W�F�N�g(VAO)�ɁAVBO���֘A�t����
	glBindVertexArray(lightVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	}

	// �ʒu���� : 0 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// �@������ : 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	///////////////////////////////////////////////////////////// �����p���_��`�I���

	// �Ƃ炳���L���[�u�p�̃V�F�[�_�[
	Shader cubeshader("PhoneLight.vert", "material.frag");

	// �����p�̃V�F�[�_�[
	Shader lightCubeshader("lightCube.vert", "lightCube.frag");

	Vector3 cameraPos(0.0f, 1.0f, 5.0f);
	FlyCamera flyCamera(cameraPos);

	// �����ʒu��`
	/*Vector3 lightPos(0.0f, 0.5f, 2.0f);*/

	Vector2 mouse;
	MOUSE_INSTANCE.SetRelativeMouseMode(true);

	float anim = 0;

	bool renderLoop = true;
	while (renderLoop)
	{
		// �L�[���͍X�V
		INPUT_INSTANCE.Update(); // �����_�[���[�v�̏��߂�1�񂾂��Ă�
		MOUSE_INSTANCE.Update(); // �����_�[���[�v�̏��߂�1�񂾂��Ă�

		deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;
		lastTime = SDL_GetTicks();

		// �I���C�x���g�̃L���b�`
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_MOUSEWHEEL:
				MOUSE_INSTANCE.OnMouseWheelEvent(event);
				break;

			case SDL_QUIT:
				renderLoop = false;
				break;
			}
		}
		flyCamera.UpdateCamera(deltaTime);

		// Model�s��
		Matrix4 model;

		// �����_�����O
		// �J���[�o�b�t�@�̃N���A
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// �e�N�X�`���𗘗p
		glBindTexture(GL_TEXTURE_2D, textureID);

		// ���C�g���]
		anim += 0.001;
		/*float lightX = 2.0f * sin(anim);
		float lightY = 0.0f;
		float lightZ = 2.0f * cos(anim);*/
		float lightX = 0.0f;
		float lightY = 1.0f;
		float lightZ = 0.0f;
		Vector3 lightPos(lightX, lightY, lightZ);

		// ���C�g�F
		float lightColorR = sin(anim);
		float lightColorG = cos(anim);
		float lightColorB = sin(anim);
		Vector3 lightColor(lightColorR, lightColorG, lightColorB);

		// cube
		cubeshader.use();
		cubeshader.setVec3("light.position", lightPos);
		cubeshader.setVec3("viewPos", cameraPos);

		cubeshader.setMatrix("model", model.GetAsFloatPtr());
		cubeshader.setMatrix("view", flyCamera.GetViewMatrix().GetAsFloatPtr());
		cubeshader.setMatrix("projection", flyCamera.GetProjectionMatrix().GetAsFloatPtr());

		// ���C�g�Z�b�g
		Vector3 diffuseColor = lightColor * Vector3(0.5f, 0.5f, 0.5f);
		Vector3 ambientColor = diffuseColor * Vector3(0.2f, 0.2f, 0.2f);
		cubeshader.setVec3("light.ambient", ambientColor);
		cubeshader.setVec3("light.diffuse", diffuseColor);
		cubeshader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// �}�e���A���Z�b�g
		cubeshader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		cubeshader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		cubeshader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		cubeshader.setFloat("material.shininess",2.0f);

		glBindVertexArray(CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		Matrix4 lightModel;
		lightModel = Matrix4::CreateScale(0.2f) * Matrix4::CreateTranslation(lightPos);

		// lightcube�̐ݒ�
		lightCubeshader.use();
		lightCubeshader.setMatrix("model", lightModel.GetAsFloatPtr());
		lightCubeshader.setMatrix("view", flyCamera.GetViewMatrix().GetAsFloatPtr());
		lightCubeshader.setMatrix("projection", flyCamera.GetProjectionMatrix().GetAsFloatPtr());

		// lightcube�`��
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		SDL_GL_SwapWindow(SDLWindow);
	}

	return 0;
}