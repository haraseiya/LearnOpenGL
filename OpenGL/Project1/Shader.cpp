#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	// 1.filePath���璸�_/�t���O�����g�̃\�[�X�R�[�h���擾
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ifstream�I�u�W�F�N�g����O�X���[�ł��邱�Ƃ��m�F���܂�
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// �t�@�C�����J��
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// �t�@�C���̃o�b�t�@�̓��e���X�g���[���ɓǂݍ���
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// �t�@�C���n���h�������
		vShaderFile.close();
		fShaderFile.close();

		// �X�g���[������string�ɕϊ�����
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::�t�@�C���ǂݍ��݂Ɏ��s" << std::endl;
	}

	// �X�g�����O����C������ɕϊ�
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// �V�F�[�_�[���R���p�C��
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// ���_�V�F�[�_�[
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// �G���[���o���ꍇ�̓G���[��\������
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::���_�V�F�[�_�[�R���p�C�����s\n" << infoLog << std::endl;
	}

	// �t���O�����g�V�F�[�_�[
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// �G���[���o���ꍇ�̓G���[��\������
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::���_�V�F�[�_�[�R���p�C�����s\n" << infoLog << std::endl;
	}

	// �V�F�[�_�[�v���O����
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// �����N�G���[���o����G���[��\��
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::�V�F�[�_�[�����N�G���[\n" << infoLog << std::endl;
	}

	// �V�F�[�_�[�͌��݃v���O����ID�Ƀ����N����A�s�v�ɂȂ����V�F�[�_�[���폜����
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()),value);
}
