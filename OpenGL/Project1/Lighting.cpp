#include "Library.h"

Uint32 lastTime = 0;
float deltaTime = 0.0f;


int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("SDL���������s: %s\n", SDL_GetError());
        return false;
    }

    // OpenGL �A�g���r���[�g�̃Z�b�g
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // GL version 3.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // �_�u���o�b�t�@�����O
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // �n�[�h�E�F�A�A�N�Z�����[�V����������
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // Window�̍쐬
    SDL_Window* mWindow = SDL_CreateWindow("SDL & GL Window",
        100, 80,
        1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!mWindow)
    {
        printf("Window�̍쐬�Ɏ��s: %s", SDL_GetError());
        return false;
    }

    // SDL Renderer�̍쐬
    SDL_Renderer* renderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        printf("SDLRenderer�̍쐬�Ɏ��s %s", SDL_GetError());
        return false;
    }

    // OpenGLContext�̍쐬
    SDL_GLContext mContext = SDL_GL_CreateContext(mWindow);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("GL �g���ǂݍ��ݎ��s\n");
    }

    // �摜��SDL�T�[�t�F�X�ɓǂݍ���
    SDL_Texture* tex1 = nullptr;
    SDL_Surface* surf1 = IMG_Load("images/nier.jpg");
    if (!surf1)
    {
        printf("�t�@�C���ǂݍ��݂Ɏ��s");
        return -1;
    }

    SDL_Surface* surf2 = IMG_Load("images/nier2.jpg");
    if (!surf2)
    {
        printf("�t�@�C���ǂݍ��݂Ɏ��s");
        return -1;
    }

    // �e�N�X�`��ID���i�[���邽�߂̕ϐ�
    unsigned texture1;

    // �e�N�X�`���I�u�W�F�N�g�𐶐��A�e��p�^�[���[�^�[�Z�b�g
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(
        GL_TEXTURE_2D,          // �e�N�X�`���^�[�Q�b�g�̎w��
        GL_TEXTURE_WRAP_S,      // �ݒ肵�����I�v�V�����ƃe�N�X�`����
        GL_REPEAT      // ���݃A�N�e�B�u�ȃe�N�X�`���Ƀe�N�X�`�����b�s���O�I�v�V������ݒ�
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // �k�����̓j�A�l�X�g�l�C�o�[�@
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // �g�厞�̓��j�A�t�B���^�����O
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // �摜�̕��E�������擾
    int width = surf1->w;
    int height = surf1->h;
    int channels = surf1->format->BytesPerPixel;

    // �摜�t�H�[�}�b�g�̔���
    int format = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
    }

    // SDL�f�[�^��GL�̃e�N�X�`���f�[�^�Ƃ��ăR�s�[����
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surf1->pixels);

    // SDL�T�[�t�F�X�͗p�ς݂Ȃ̂ŊJ��
    SDL_FreeSurface(surf1);

    unsigned texture2;
    // �e�N�X�`���I�u�W�F�N�g�𐶐��A�e��p�^�[���[�^�[�Z�b�g
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(
        GL_TEXTURE_2D,          // �e�N�X�`���^�[�Q�b�g�̎w��
        GL_TEXTURE_WRAP_S,      // �ݒ肵�����I�v�V�����ƃe�N�X�`����
        GL_REPEAT      // ���݃A�N�e�B�u�ȃe�N�X�`���Ƀe�N�X�`�����b�s���O�I�v�V������ݒ�
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // �k�����̓j�A�l�X�g�l�C�o�[�@
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // �g�厞�̓��j�A�t�B���^�����O
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    width = surf2->w;
    height = surf2->h;
    channels = surf2->format->BytesPerPixel;

    // �摜�t�H�[�}�b�g�̔���
    format = GL_RGB;
    if (channels == 4)
    {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, surf2->pixels);

    SDL_FreeSurface(surf2);

    // RGBA
    float borderColor[] = { 1.0f,1.0f,0.0f,1.0f };

    // float�^��vector�z��
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glGenerateMipmap(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);

    float vertices[] =
    {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
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

    unsigned int VBO;   // ���_�o�b�t�@�I�u�W�F�N�g
    unsigned int cubeVAO;   // ���_�z��I�u�W�F�N�g

    // ���_�z��I�u�W�F�N�g��ID������
    glGenVertexArrays(1, &cubeVAO);

    // ���_�o�b�t�@�I�u�W�F�N�g��ID������
    glGenBuffers(1, &VBO);

    // �擾����VBO�̃o�b�t�@�^�C�v��GL_ARRAY_BUFFER�Ƃ��Đݒ�
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // vertices�z��̐擪�A�h���X���w��A�擪�A�h���X����T�C�Y�̃f�[�^���A�����������VBO�ɓ]������֐�
    glBufferData(
        GL_ARRAY_BUFFER,    // �f�[�^�̃R�s�[��̃o�b�t�@�[�^�C�v
        sizeof(vertices),   // �o�b�t�@�ɓn���f�[�^�̃T�C�Y�w��
        vertices,           // ���M����f�[�^���������z��̐擪�A�h���X
        GL_STATIC_DRAW      // �O���t�B�b�N�X�J�[�h�Ńf�[�^���Ǘ�������@
    );
    // 4�Ԗڂ̈�����
    // GL_STREAM_DRAW   �f�[�^�͈�x�����������܂�GPU�ɂ���Đ���g�p�����
    // GL_STATIC_DRAW   �f�[�^�͈�x�����ݒ肳��A���x��GPU�Ŏg�p�����
    // GL_DYNAMIC_DRAW  �f�[�^�͉��x���ύX�\�AGPU�ł����x���g�p�����
    // �ʏ��STATIC���g��

    // ���_�������w�肷��
    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "�ő咸�_�A�g���r���[�g��: " << nrAttributes << std::endl;

    Shader lightingShader("vertex.vert", "lighting.frag");
    Shader lightCubeShader("lighting.vert", "lightCube.frag");

    static float anim = 0.1f;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // �r���[�s��
    glm::mat4 view;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 cameraRightVec = glm::cross(cameraUp, cameraFront);

    // uniform�ϐ���ݒ肷��O�ɃV�F�[�_�[���A�N�e�B�u�ɂ��邱�Ƃ�Y��Ȃ�
    lightingShader.use();

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 1000.0f);

    //�����_�����O���[�v
    bool renderLoop = true;

    Vector2 mouse;
    float yaw = 180.0f;
    float pitch = 0.0f;

    MOUSE_INSTANCE.SetRelativeMouseMode(true);

    while (renderLoop)
    {
        INPUT_INSTANCE.Update();
        MOUSE_INSTANCE.Update();

        // ���ݎ�����lastTime�Ԃŉ��~���b�o�߂��������v�Z
        // �b�P�ʂɕϊ�����������1000�Ŋ���
        deltaTime = (SDL_GetTicks() - lastTime) / 1000.0f;

        // ���̃t���[���̂��߂ɍŏI�������X�V����
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

        // �}�E�X����
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

        // �J�����ړ�����
        // deltaTime���g�����ƂŃ}�V�����Ɉˑ����Ȃ��ړ����x�̊���
        const float cameraSpeed = 2.5f * deltaTime;

        if (INPUT_INSTANCE.GetInput(SDL_SCANCODE_W) == Input::KEY_STATE_PRESSED)
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


        // �����_�����O
        // �J���[�o�b�t�@�̃N���A
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ��ʂ̃X���b�v
        SDL_GL_SwapWindow(mWindow);
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    // SDL & GL�̌�n������
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);

    return 0;
}
