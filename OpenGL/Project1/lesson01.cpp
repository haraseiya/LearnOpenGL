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
	// SDLの初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("SDL初期化失敗 : %s\n", SDL_GetError());
		return -1;
	}

	// OpenGL アトリビュートのセット
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// GL version 3.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// 8Bit RGBA チャンネル
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// ダブルバッファリング
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// ハードウェアアクセラレーションを強制
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Windowの作成
	SDL_Window* SDLWindow = SDL_CreateWindow("SDL & GL Window",
		100, 80,
		1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!SDLWindow)
	{
		printf("Windowの作成に失敗: %s", SDL_GetError());
		return false;
	}
	SDL_GLContext context;
	context = SDL_GL_CreateContext(SDLWindow);

	// GLADの初期化
	const int version = gladLoadGL((GLADloadfunc)(SDL_GL_GetProcAddress));
	if (version == 0)
	{
		printf("glad初期化失敗！\n");
	}

	// 頂点シェーダープログラムのコンパイルとビルド
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	// 頂点シェーダープログラムのコンパイルエラーのチェック
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::コンパイルエラー\n" << infoLog << std::endl;
	}

	// フラグメントシェーダー
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::コンパイルエラー\n" << infoLog << std::endl;
	}
	// 頂点シェーダーとフラグメントシェーダーのリンク
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// リンクエラーのチェック
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::リンク失敗\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 頂点データとバッファのセットアップと頂点属性の設定
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 最初に頂点配列オブジェクトをバインドしてから、頂点バッファをバインドして設定します。最後に頂点属性を設定します。
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// これは許可されていることに注意してください。
	// glVertexAttribPointer関数によって、VBOを頂点属性のバインドされた
	// 頂点バッファーオブジェクトとして登録したため、安全にアンバインドできます。
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// VAOを後でバインド解除して、他のVAO呼び出しがこのVAOを誤って変更しないようにすることができますが、これはめったに起こりません。
	// 他のVAOを変更するには、とにかくglBindVertexArrayを呼び出す必要があるため、通常は直接必要でない場合にVAO（VBOも）のバインドを解除しません。
	glBindVertexArray(0);

	//コメントを解除すると、ワイヤフレームで描画します。
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	printf("初期化成功\n");

	//レンダリングループ
	bool renderLoop = true;
	while (renderLoop)
	{
		// 終了イベントのキャッチ
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

		// 描画処理
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 三角形を描画
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 画面のスワップ
		SDL_GL_SwapWindow(SDLWindow);
	}

	// オプション：すべてのリソースの役割を終えたら、すべてのリソースの割り当てを解除します。
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// SDL & GLの後始末処理
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(SDLWindow);

	return 0;
}

