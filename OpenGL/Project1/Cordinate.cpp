#include "Library.h"

// �J����
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float cameraSpeed = 2.5 * deltaTime;

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

    //float vertices[] = {
    //    // ���_�ʒu            // ���_�J���[       // �e�N�X�`�����W
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // �E��
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // �E��
    //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // ����
    //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // ���� 
    //};
    glEnable(GL_DEPTH_TEST);

    float vertices[] = 
    {
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

    unsigned int VBO;   // ���_�o�b�t�@�I�u�W�F�N�g
    unsigned int VAO;   // ���_�z��I�u�W�F�N�g
    unsigned int EBO;

    // ���_�z��I�u�W�F�N�g��ID������
    glGenVertexArrays(1, &VAO);

    // ���_�o�b�t�@�I�u�W�F�N�g��ID������
    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    // VAO���o�C���h
    glBindVertexArray(VAO);

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

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);*/
    /*glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    glVertexAttribPointer(
        0,                      // �\�����钸�_����layout(location=0)��0
        3,                      // ���_�����̃T�C�Y���w��Avec3�̂���3�l�ō\��
        GL_FLOAT,               // �f�[�^�̃^�C�v���w��
        GL_FALSE,               // �f�[�^�𐳋K�����邩�ǂ���
        5 * sizeof(float),      // 
        (void*)0
    );

    // ���_�������w�肷��
    glEnableVertexAttribArray(0);

    /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));*/
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    printf("����������\n");

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "�ő咸�_�A�g���r���[�g��: " << nrAttributes << std::endl;

    Shader ourShader("vertex.vert", "fragment.frag");

    static float anim = 0.1f;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    glm::mat4 view = glm::mat4(1.0f);

    // uniform�ϐ���ݒ肷��O�ɃV�F�[�_�[���A�N�e�B�u�ɂ��邱�Ƃ�Y��Ȃ�
    ourShader.use();

    // �e�N�X�`��Uint0�ԂɃZ�b�g����
    ourShader.setInt("texture1", 0);

    // �e�N�X�`��Uint1�ԂɃZ�b�g����
    ourShader.setInt("texture2", 1);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f),1024.0f/768.0f,0.1f,1000.0f);

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

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT)
        {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT)
        {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP)
        {
            cameraPos += cameraSpeed * cameraFront;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN)
        {
            cameraPos -= cameraSpeed * cameraFront;
        }

        // �����_�����O
        // �J���[�o�b�t�@�̃N���A
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

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
            model = glm::rotate(model, anim*glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        anim += 0.1f;

        /*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
        /*glDrawArrays(GL_TRIANGLES, 0, 36);*/

        // ��ʂ̃X���b�v
        SDL_GL_SwapWindow(mWindow);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // SDL & GL�̌�n������
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);

    return 0;
}
