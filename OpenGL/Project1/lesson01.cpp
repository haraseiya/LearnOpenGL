#include <stdio.h>
#include <glad.h>
#include <sdl.h>
#include <iostream>

const char* vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char* fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

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
		100, 80,
		1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!SDLWindow)
	{
		printf("Window�̍쐬�Ɏ��s: %s", SDL_GetError());
		return false;
	}
	SDL_GLContext context;
	context = SDL_GL_CreateContext(SDLWindow);

	// GLAD�̏�����
	const int version = gladLoadGL((GLADloadfunc)(SDL_GL_GetProcAddress));
	if (version == 0)
	{
		printf("glad���������s�I\n");
	}

	// ���_�V�F�[�_�[�v���O�����̃R���p�C���ƃr���h
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	// ���_�V�F�[�_�[�v���O�����̃R���p�C���G���[�̃`�F�b�N
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::�R���p�C���G���[\n" << infoLog << std::endl;
	}

	// �t���O�����g�V�F�[�_�[
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::�R���p�C���G���[\n" << infoLog << std::endl;
	}
	// ���_�V�F�[�_�[�ƃt���O�����g�V�F�[�_�[�̃����N
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// �����N�G���[�̃`�F�b�N
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::�����N���s\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ���_�f�[�^�ƃo�b�t�@�̃Z�b�g�A�b�v�ƒ��_�����̐ݒ�
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// �ŏ��ɒ��_�z��I�u�W�F�N�g���o�C���h���Ă���A���_�o�b�t�@���o�C���h���Đݒ肵�܂��B�Ō�ɒ��_������ݒ肵�܂��B
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ����͋�����Ă��邱�Ƃɒ��ӂ��Ă��������B
	// glVertexAttribPointer�֐��ɂ���āAVBO�𒸓_�����̃o�C���h���ꂽ
	// ���_�o�b�t�@�[�I�u�W�F�N�g�Ƃ��ēo�^�������߁A���S�ɃA���o�C���h�ł��܂��B
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAO����Ńo�C���h�������āA����VAO�Ăяo��������VAO������ĕύX���Ȃ��悤�ɂ��邱�Ƃ��ł��܂����A����͂߂����ɋN����܂���B
	// ����VAO��ύX����ɂ́A�Ƃɂ���glBindVertexArray���Ăяo���K�v�����邽�߁A�ʏ�͒��ڕK�v�łȂ��ꍇ��VAO�iVBO���j�̃o�C���h���������܂���B
	glBindVertexArray(0);

	//�R�����g����������ƁA���C���t���[���ŕ`�悵�܂��B
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	printf("����������\n");

	//�����_�����O���[�v
	bool renderLoop = true;
	while (renderLoop)
	{
		// �I���C�x���g�̃L���b�`
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				renderLoop = false;
				break;
			}
		}

		// �`�揈��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// �O�p�`��`��
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// ��ʂ̃X���b�v
		SDL_GL_SwapWindow(SDLWindow);
	}

	// �I�v�V�����F���ׂẴ��\�[�X�̖������I������A���ׂẴ��\�[�X�̊��蓖�Ă��������܂��B
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// SDL & GL�̌�n������
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(SDLWindow);

	return 0;
}

