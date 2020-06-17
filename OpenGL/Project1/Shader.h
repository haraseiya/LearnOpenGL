#pragma once
#include <glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// プログラムID
	unsigned int ID;

	// コンストラクターではシェ―ダーを読み取り、ビルドする
	Shader(const char* vertexPath, const char* fragmentPath);

	// シェーダーの使用/有効化を行う
	void use();

	// シェーダーのuniform変数をセットするためのメソッド
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};
