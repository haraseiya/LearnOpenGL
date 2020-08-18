#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad.h>
#include "Math.h"
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

	void setVec3(const std::string& valiableName, float x, float y, float z);
	void setVec3(const std::string& valiableName, Vector3& vec);
	void setVec4(const std::string& valiableName, float x, float y, float z, float w);
	void setVec4(const std::string& valiableName, Vector3& vec, float w);

	void setMatrix(const std::string& valiableName, const float* matrix);

	unsigned int GetID() { return ID; }
};

#endif
