// 頂点情報のシェーダー
#version 330 core
layout(location = 0)in vec3 aPos;       // 頂点位置
layout(location = 1)in vec3 aColor;     // 頂点カラー
layout(location = 2)in vec2 aTexCoord;  // テクスチャ座標

out vec3 ourColor;
out vec2 TexCoord;  // フラグメントにテクスチャ座標を送るための変数

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
//    gl_Position = vec4(aPos, 1.0);
    gl_Position=projection*view*model*vec4(aPos,1.0);
//    ourColor=aColor; 
    TexCoord=aTexCoord;
}
