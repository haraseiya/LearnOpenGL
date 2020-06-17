#pragma once
#include <glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// �v���O����ID
	unsigned int ID;

	// �R���X�g���N�^�[�ł̓V�F�\�_�[��ǂݎ��A�r���h����
	Shader(const char* vertexPath, const char* fragmentPath);

	// �V�F�[�_�[�̎g�p/�L�������s��
	void use();

	// �V�F�[�_�[��uniform�ϐ����Z�b�g���邽�߂̃��\�b�h
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};
