// �F���̃V�F�[�_�[
#version 330 core
out vec4 FragColor; // ���̃t���O�����g�̏o��

in vec3 ourColor;   // ���_�V�F�[�_�[����̒��_�J���[����͂Ƃ��Ď󂯎��
in vec2 TexCoord;   // ���_�V�F�[�_�[����̒��_���W����͂Ƃ��Ď󂯎��

uniform sampler2D texture1;   // 2�����e�N�X�`���^
uniform sampler2D texture2; 

void main()
{
    // �e�N�X�`���J���[�ƒ��_�J���[����Z�������� 
    FragColor=mix(texture(texture1,TexCoord),texture(texture2,TexCoord),0.6f);
}
