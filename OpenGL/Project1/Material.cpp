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

Uint32 lastTime = 0;    // 最後のフレーム時刻 intミリ秒単位
float  deltaTime = 0.0f; // フレーム間の時刻 秒単位 ( 1.0 sec )

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
		100, 80, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!SDLWindow)
	{
		printf("Windowの作成に失敗: %s", SDL_GetError());
		return false;
	}

	// SDL Rendererの作成
	SDL_Renderer* renderer = SDL_CreateRenderer(SDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		printf("SDLRendererの作成に失敗 %s", SDL_GetError());
		return false;
	}

	SDL_GLContext context;
	context = SDL_GL_CreateContext(SDLWindow);

	// GLADの初期化
	const int version = gladLoadGL();
	if (version == 0)
	{
		printf("glad初期化失敗！\n");
	}

	glEnable(GL_DEPTH_TEST);

	// 画像をSDLサーフェスに読み込む
	SDL_Surface* surf = IMG_Load("images/nier.jpg");
	if (!surf)
	{
		printf("ファイル読み込みに失敗");
		return -1;
	}

	// 画像の幅・高さを取得
	int width = surf->w;
	int	height = surf->h;
	int channels = surf->format->BytesPerPixel;

	// 画像フォーマットの判別
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// SDLサーフェスからＧＬテクスチャ作成＆各種パラメータセット
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// テクスチャラッピング＆フィルタリング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// SDLデータをGLのテクスチャデータとしてコピーする
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
		GL_UNSIGNED_BYTE, surf->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	// SDLサーフェスは用済みなので解放
	SDL_FreeSurface(surf);

	////////////////////////////////////////////////////////////////////////
	// 頂点定義(ライトに照らされる側のキューブ) 頂点＋頂点法線
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

	// 頂点配列　頂点バッファ　エレメント配列バッファ
	unsigned int CubeVAO, CubeVBO;
	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &CubeVBO);

	// 頂点配列オブジェクト(VAO)に、VBOを関連付ける
	glBindVertexArray(CubeVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubevertices), cubevertices, GL_STATIC_DRAW);
	}
	// 位置属性 : 0 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 法線属性 : 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	///////////////////////////////////////////////////////////// キューブ頂点定義


	////////////////////////////////////////////////////////////////////////
	// 頂点定義(光源を表すキューブ)
	// 照らされる側の影響を受けたくないので重複しているが別で定義
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

	// 頂点配列　頂点バッファ　エレメント配列バッファ
	unsigned int lightVAO, lightVBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	// 頂点配列オブジェクト(VAO)に、VBOを関連付ける
	glBindVertexArray(lightVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	}

	// 位置属性 : 0 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 法線属性 : 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	///////////////////////////////////////////////////////////// 光源用頂点定義終わり

	// 照らされるキューブ用のシェーダー
	Shader cubeshader("PhoneLight.vert", "material.frag");

	// 光源用のシェーダー
	Shader lightCubeshader("lightCube.vert", "lightCube.frag");

	Vector3 cameraPos(0.0f, 1.0f, 5.0f);
	FlyCamera flyCamera(cameraPos);

	// 光源位置定義
	/*Vector3 lightPos(0.0f, 0.5f, 2.0f);*/

	Vector2 mouse;
	MOUSE_INSTANCE.SetRelativeMouseMode(true);

	float anim = 0;

	bool renderLoop = true;
	while (renderLoop)
	{
		// キー入力更新
		INPUT_INSTANCE.Update(); // レンダーループの初めに1回だけ呼ぶ
		MOUSE_INSTANCE.Update(); // レンダーループの初めに1回だけ呼ぶ

		deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;
		lastTime = SDL_GetTicks();

		// 終了イベントのキャッチ
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

		// Model行列
		Matrix4 model;

		// レンダリング
		// カラーバッファのクリア
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// テクスチャを利用
		glBindTexture(GL_TEXTURE_2D, textureID);

		// ライト公転
		anim += 0.001;
		/*float lightX = 2.0f * sin(anim);
		float lightY = 0.0f;
		float lightZ = 2.0f * cos(anim);*/
		float lightX = 0.0f;
		float lightY = 1.0f;
		float lightZ = 0.0f;
		Vector3 lightPos(lightX, lightY, lightZ);

		// ライト色
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

		// ライトセット
		Vector3 diffuseColor = lightColor * Vector3(0.5f, 0.5f, 0.5f);
		Vector3 ambientColor = diffuseColor * Vector3(0.2f, 0.2f, 0.2f);
		cubeshader.setVec3("light.ambient", ambientColor);
		cubeshader.setVec3("light.diffuse", diffuseColor);
		cubeshader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// マテリアルセット
		cubeshader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		cubeshader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		cubeshader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		cubeshader.setFloat("material.shininess",2.0f);

		glBindVertexArray(CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		Matrix4 lightModel;
		lightModel = Matrix4::CreateScale(0.2f) * Matrix4::CreateTranslation(lightPos);

		// lightcubeの設定
		lightCubeshader.use();
		lightCubeshader.setMatrix("model", lightModel.GetAsFloatPtr());
		lightCubeshader.setMatrix("view", flyCamera.GetViewMatrix().GetAsFloatPtr());
		lightCubeshader.setMatrix("projection", flyCamera.GetProjectionMatrix().GetAsFloatPtr());

		// lightcube描画
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		SDL_GL_SwapWindow(SDLWindow);
	}

	return 0;
}