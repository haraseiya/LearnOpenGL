// 色情報のシェーダー
#version 330 core
out vec4 FragColor; // このフラグメントの出力

in vec3 ourColor;   // 頂点シェーダーからの頂点カラーを入力として受け取る
in vec2 TexCoord;   // 頂点シェーダーからの頂点座標を入力として受け取る

uniform sampler2D texture1;   // 2次元テクスチャ型
uniform sampler2D texture2; 

void main()
{
    // テクスチャカラーと頂点カラーを乗算合成する 
    FragColor=mix(texture(texture1,TexCoord),texture(texture2,TexCoord),0.6f);
}
