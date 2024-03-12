#include "pch.h"
#include "shader.h"

Shader::Shader(){}

Shader::~Shader(){}

ID3DBlob* Shader::compileShader(wstring filename, D3D_SHADER_MACRO* defines, string entrypoint, string target)
	{
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
			compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;

	ID3DBlob* byteCode = nullptr;
	ID3DBlob* errors;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	assert(SUCCEEDED(hr));

	return byteCode;
}