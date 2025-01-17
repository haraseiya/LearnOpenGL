#include "Library.h"

Uint32 lastTime = 0;
float deltaTime = 0.0f;

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("SDL初期化失敗: %s\n", SDL_GetError());
        return false;
    }

    // OpenGL アトリビュートのセット
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // GL version 3.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // ダブルバッファリング
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // ハードウェアアクセラレーションを強制
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // Windowの作成
    SDL_Window* mWindow = SDL_CreateWindow("SDL & GL Window",
        100, 80,
        1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!mWindow)
    {
        printf("Windowの作成に失敗: %s", SDL_GetError());
        return false;
    }

    // SDL Rendererの作成
    SDL_Renderer* renderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        printf("SDLRendererの作成に失敗 %s", SDL_GetError());
        return false;
    }

    // OpenGLContextの作成
    SDL_GLContext mContext = SDL_GL_CreateContext(mWindow);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("GL 拡張読み込み失敗\n");
    }

    // 画像をSDLサーフェスに読み込む
    SDL_Texture* tex1 = nullptr;
    SDL_Surface* surf1 = IMG_Load("images/nier.jpg");
    if (!surf1)
    {
        printf("ファイル読み込みに失敗");
        return -1;
    }

    SDL_Surface* surf2 = IMG_Load("images/nier2.jpg");
    if (!surf2)
    {
        printf("ファイル読み込みに失敗");
        return -1;
    }

    // テクスチャIDを格納するための変数
    unsigned texture1;

    // テクスチャオブジェクトを生成、各種パターメーターセット
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(
        GL_TEXTURE_2D,          // テクスチャターゲットの指定
        GL_TEXTURE_WRAP_S,      // 設定したいオプションとテクスチャ軸
        GL_REPEAT      // 現在アクティブなテクスチャにテクスチャラッピングオプションを設定
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 縮小時はニアネストネイバー法
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 拡大時はリニアフィルタリング
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 画像の幅・高さを取得
    int width = surf1->w;
    int height = surf1->h;
    int channels = surf1->format->BytesPerPixel;

    // 画像フォーマットの判別
    int format = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
    }

    // SDLデータをGLのテクスチャデータとしてコピーする
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surf1->pixels);

    // SDLサーフェスは用済みなので開放
    SDL_FreeSurface(surf1);

    unsigned texture2;
    // テクスチャオブジェクトを生成、各種パターメーターセット
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(
        GL_TEXTURE_2D,          // テクスチャターゲットの指定
        GL_TEXTURE_WRAP_S,      // 設定したいオプションとテクスチャ軸
        GL_REPEAT      // 現在アクティブなテクスチャにテクスチャラッピングオプションを設定
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 縮小時はニアネストネイバー法
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 拡大時はリニアフィルタリング
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    width = surf2->w;
    height = surf2->h;
    channels = surf2->format->BytesPerPixel;

    // 画像フォーマットの判別
    format = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surf2->pixels);

    SDL_FreeSurface(surf2);

    // RGBA
    float borderColor[] = { 1.0f,1.0f,0.0f,1.0f };

    // float型のvector配列
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glGenerateMipmap(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] =
    {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO;   // 頂点バッファオブジェクト
    unsigned int VAO;   // 頂点配列オブジェクト
    unsigned int EBO;

    // 頂点配列オブジェクトのIDが入る
    glGenVertexArrays(1, &VAO);

    // 頂点バッファオブジェクトのIDが入る
    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    // VAOをバインド
    glBindVertexArray(VAO);

    // 取得したVBOのバッファタイプをGL_ARRAY_BUFFERとして設定
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // vertices配列の先頭アドレスを指定、先頭アドレスからサイズのデータ分、さっき作ったVBOに転送する関数
    glBufferData(
        GL_ARRAY_BUFFER,    // データのコピー先のバッファータイプ
        sizeof(vertices),   // バッファに渡すデータのサイズ指定
        vertices,           // 送信するデータが入った配列の先頭アドレス
        GL_STATIC_DRAW      // グラフィックスカードでデータを管理する方法
    );
    // 4番目の引数は
    // GL_STREAM_DRAW   データは一度だけ書き込まれGPUによって数回使用される
    // GL_STATIC_DRAW   データは一度だけ設定され、何度もGPUで使用される
    // GL_DYNAMIC_DRAW  データは何度も変更可能、GPUでも何度も使用される
    // 通常はSTATICを使う

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);*/
    /*glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    glVertexAttribPointer(
        0,                      // 構成する頂点属性layout(location=0)の0
        3,                      // 頂点属性のサイズを指定、vec3のため3値で構成
        GL_FLOAT,               // データのタイプを指定
        GL_FALSE,               // データを正規化するかどうか
        5 * sizeof(float),      // 
        (void*)0
    );

    // 頂点属性を指定する
    glEnableVertexAttribArray(0);

    /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));*/
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    printf("初期化成功\n");

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "最大頂点アトリビュート数: " << nrAttributes << std::endl;

    Shader ourShader("vertex.vert", "fragment.frag");

    static float anim = 0.1f;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // ビュー行列
    glm::mat4 view;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 cameraRightVec = glm::cross(cameraUp, cameraFront);

    // uniform変数を設定する前にシェーダーをアクティブにすることを忘れない
    ourShader.use();

    // テクスチャUint0番にセットする
    ourShader.setInt("texture1", 0);

    // テクスチャUint1番にセットする
    ourShader.setInt("texture2", 1);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

    //レンダリングループ
    bool renderLoop = true;

    Vector2 mouse;
    float yaw = -90.0f;
    float pitch = 0.0f;

    MOUSE_INSTANCE.SetRelativeMouseMode(true);

    while (renderLoop)
    {
        INPUT_INSTANCE.Update();
        MOUSE_INSTANCE.Update();

        // 現在時刻とlastTime間で何ミリ秒経過したかを計算
        // 秒単位に変換したいため1000で割る
        deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;

        // 次のフレームのために最終時刻を更新する
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

        // マウス処理
        mouse = MOUSE_INSTANCE.GetPosition();
        static float sensitivity = 0.5f;
        float xOffset = mouse.x * sensitivity * deltaTime;
        float yOffset = mouse.y * sensitivity * deltaTime;

        yaw += xOffset;
        pitch -= yOffset;

        if (pitch > Math::ToRadians(89.0f))
        {
            pitch = Math::ToRadians(89.0f);
        }
        if (pitch < Math::ToRadians(-89.0f))
        {
            pitch = Math::ToRadians(-89.0f);
        }

        glm::vec3 direction;
        direction.x = cosf(yaw) * cos(pitch);
        direction.y = sinf(pitch);
        direction.z = sinf(yaw) * cos(pitch);
        glm::normalize(direction);

        cameraFront = direction;

        float zoom = 0.0f;
        Vector2 wheel = MOUSE_INSTANCE.GetWheel();
        zoom += wheel.y * deltaTime;

        if (Math::ToDegrees(zoom) < 1.0f)
        {
            zoom = Math::ToRadians(1.0f);
        }
        if (Math::ToDegrees(zoom) > 90.0f)
        {
            zoom = Math::ToRadians(90.0f);
        }
        /*projection = glm::perspectiveFov(zoom, 1024.0f, 768.0f, 0.1f, 1000.0f);*/

        // カメラ移動処理
        // deltaTimeを使うことでマシン差に依存しない移動速度の完成
        const float cameraSpeed = 2.5f * deltaTime;

        if (INPUT_INSTANCE.GetInput(SDL_SCANCODE_W)==Input::KEY_STATE_PRESSED)
        {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (INPUT_INSTANCE.GetInput(SDL_SCANCODE_S) == Input::KEY_STATE_PRESSED)
        {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (INPUT_INSTANCE.GetInput(SDL_SCANCODE_A) == Input::KEY_STATE_PRESSED)
        {
            cameraPos += cameraSpeed * cameraRightVec;
        }
        if (INPUT_INSTANCE.GetInput(SDL_SCANCODE_D) == Input::KEY_STATE_PRESSED)
        {
            cameraPos -= cameraSpeed * cameraRightVec;
        }
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // レンダリング
        // カラーバッファのクリア
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();

        /*view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);*/

        unsigned int modelLocation = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLocation = glGetUniformLocation(ourShader.ID, "view");
        unsigned int projectionLocation = glGetUniformLocation(ourShader.ID, "projection");

        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        /*model = glm::rotate(model, anim * glm::radians(1.0f), glm::vec3(0.5f, 1.0f, 0.0f));*/

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 1.0f * i;
            model = glm::rotate(model, anim * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        anim += 0.1f;

        /*const float radius = 10.0f;
        float camX = sinf(SDL_GetTicks() / 1000.0f) * radius;
        float camZ = cosf(SDL_GetTicks() / 1000.0f) * radius;*/
        
        /*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
        /*glDrawArrays(GL_TRIANGLES, 0, 36);*/

        // 画面のスワップ
        SDL_GL_SwapWindow(mWindow);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // SDL & GLの後始末処理
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);

    return 0;
}
