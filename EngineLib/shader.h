#pragma once
#include "pch.h"

class Shader
{
public :
	Shader();
	~Shader();
	ID3DBlob* compileShader(wstring filename, D3D_SHADER_MACRO* defines, string entrypoint, string target);

private :

};