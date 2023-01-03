#include "stpch.h"

#ifdef ST_RENDERER_DX11
    #include "Sentinel/Graphics/Material/ShaderAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel {

    namespace Utils {
        static STL::unordered_map<STL::string, Sentinel::ShaderType> s_ShaderStringTypeMap = {
            {"vertex", ShaderType::VERTEX}, {"pixel", ShaderType::PIXEL}, {"compute", ShaderType::COMPUTE}};

        static STL::unordered_map<Sentinel::ShaderType, const STL::string> s_ShaderTypeStringMap = {
            {ShaderType::VERTEX, "vertex"}, {ShaderType::PIXEL, "pixel"}, {ShaderType::COMPUTE, "compute"}};

        static STL::unordered_map<Sentinel::ShaderType, const char*> s_ShaderTypeProfileMap = {
            {ShaderType::VERTEX, "vs_5_0"}, {ShaderType::PIXEL, "ps_5_0"}, {ShaderType::COMPUTE, "cs_5_0"}};

        static STL::unordered_map<Sentinel::ShaderType, const char*> s_ShaderTypeEntryPointMap = {
            {ShaderType::VERTEX, "VShader"}, {ShaderType::PIXEL, "PShader"}, {ShaderType::COMPUTE, "CShader"}};

    }  // namespace Utils

    ShaderData* Sentinel::ShaderAPI::CreateShaderData(
        PoolAllocator<ShaderData>& allocator,
        ContextData* context,
        const STL::string& filepath,
        const STL::string& name) {
        ShaderData* shaderObject = allocator.New();  // Remove the <T> from New calls in other classes
        shaderObject->m_Filepath = filepath;
        shaderObject->m_ShaderName = name;
        shaderObject->Context = context;

        shaderObject->m_Sources[0].type = shaderObject->m_Binaries[0].type = ShaderType::NONE;
        shaderObject->m_Sources[1].type = shaderObject->m_Binaries[1].type = ShaderType::VERTEX;
        shaderObject->m_Sources[2].type = shaderObject->m_Binaries[2].type = ShaderType::PIXEL;
        shaderObject->m_Sources[3].type = shaderObject->m_Binaries[3].type = ShaderType::COMPUTE;

        Unbind(shaderObject);
        Load(shaderObject);
        return shaderObject;
    }

    void ShaderAPI::Bind(ShaderData* dataObject) {
        ID3D11DeviceContext* context = ContextAPI::GetNativeContext(dataObject->Context);

        if (dataObject->m_NativeVS) context->VSSetShader(dataObject->m_NativeVS, nullptr, 0);
        if (dataObject->m_NativePS) context->PSSetShader(dataObject->m_NativePS, nullptr, 0);
        if (dataObject->m_NativeCS) context->CSSetShader(dataObject->m_NativeCS, nullptr, 0);
    }

    void ShaderAPI::Reload(ShaderData* dataObject) {
        ID3D11DeviceContext* context = ContextAPI::GetNativeContext(dataObject->Context);
        Unbind(dataObject);
        Load(dataObject);
    }

    void ShaderAPI::Clean(ShaderData* dataObject) {
        if (dataObject->m_NativeVS) {
            dataObject->m_NativeVS->Release();
            dataObject->m_NativeVS = 0;
        }

        if (dataObject->m_NativePS) {
            dataObject->m_NativePS->Release();
            dataObject->m_NativePS = 0;
        }

        if (dataObject->m_NativeCS) {
            dataObject->m_NativeCS->Release();
            dataObject->m_NativeCS = 0;
        }

        for (auto& binary: dataObject->m_Binaries) {
            if (binary.binary) {
                binary.binary->Release();
                binary.binary = NULL;
            }
        }

        // dataObject->m_ShaderName.clear();
        // dataObject->m_Filepath.clear();
    }

    void ShaderAPI::Unbind(ShaderData* dataObject) {
        for (auto& binary: dataObject->m_Binaries) {
            if (binary.binary != NULL) {
                binary.binary->Release();
                binary.binary = nullptr;
            }
        }

        if (dataObject->m_NativeVS != nullptr) {
            dataObject->m_NativeVS->Release();
            dataObject->m_NativeVS = nullptr;
        }

        if (dataObject->m_NativePS != nullptr) {
            dataObject->m_NativePS->Release();
            dataObject->m_NativePS = nullptr;
        }

        if (dataObject->m_NativeCS != nullptr) {
            dataObject->m_NativeCS->Release();
            dataObject->m_NativeCS = nullptr;
        }
    }

    void ShaderAPI::PreprocessSource(const STL::string& source, ShaderSource* sources) {
        STL::unordered_map<ShaderType, STL::string> shaderSources;

        const char* typeToken = "#type";
        Size_t typeTokenLength = strlen(typeToken);
        Size_t pos = source.find(typeToken, 0);  // Start of shader type declaration file;

        while (pos != STL::string::npos) {
            Size_t eol = source.find_first_of("\r\n", pos);  // End of shader type declaration line
            ST_ENGINE_ASSERT(eol != STL::string::npos, "Syntax error");
            Size_t begin = pos + typeTokenLength + 1;  // Start of shader type name (after "#type" keyword)
            const STL::string& type = source.substr(begin, eol - begin);
            ST_ENGINE_ASSERT((Int32)Utils::s_ShaderStringTypeMap.at(type), "Invalid shader type specified");

            Size_t nextLinePos =
                source.find_first_not_of("\r\n", eol);  // Start of shader code after shader type declaration line
            ST_ENGINE_ASSERT(nextLinePos != STL::string::npos, "Syntax error");
            pos = source.find(typeToken, nextLinePos);  // Start of next shader type declaration line

            sources[(UInt8)(Utils::s_ShaderStringTypeMap.at(type))].source =
                (pos == STL::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }
    }

    void ShaderAPI::CompileFromSource(ShaderData* dataObject) {
        HRESULT result;
        ID3DBlob* errorMessages;

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

    #ifdef ST_DEBUG
        flags |= D3DCOMPILE_DEBUG;
    #endif  // ST_DEBUG

        for (auto& tuple: dataObject->m_Sources) {
            if (tuple.type == ShaderType::NONE || tuple.source.empty()) continue;

            const STL::string& shader = tuple.source;

            const char* ep = Utils::s_ShaderTypeEntryPointMap[tuple.type];
            const char* profile = Utils::s_ShaderTypeProfileMap[tuple.type];

            ID3DBlob* blob;
            result = D3DCompile(
                shader.c_str(),
                shader.size(),
                nullptr,
                nullptr,
                D3D_COMPILE_STANDARD_FILE_INCLUDE,
                ep,
                profile,
                flags,
                0,
                &blob,
                &errorMessages);

            dataObject->m_Binaries[(UInt8)tuple.type].binary = blob;

    #if ST_DEBUG
            if (FAILED(result)) {
                char* error = (char*)errorMessages->GetBufferPointer();
                error[strnlen_s(error, 128) - 1] = '\0';

                ST_ENGINE_ERROR("{0}", error);
                errorMessages->Release();
                ST_ENGINE_ERROR(
                    "{0}::{1} Shader Compilation error",
                    dataObject->m_ShaderName.c_str(),
                    Utils::s_ShaderTypeStringMap.at(tuple.type).c_str());
                ST_ENGINE_ASSERT(false, "");
            }
    #endif

            if (errorMessages) errorMessages->Release();
        }
    }

    void ShaderAPI::Load(ShaderData* dataObject) {
        STL::string& source = Filesystem::ReadTextFileAtPath(dataObject->m_Filepath);

        if (source.empty()) {
            ST_ENGINE_ASSERT("Shader source at path {0} is empty", dataObject->m_Filepath.c_str());
            return;
        }

        PreprocessSource(source, dataObject->m_Sources);
        CompileFromSource(dataObject);

        ID3D11Device* device = ContextAPI::GetDevice(dataObject->Context);

        if (dataObject->m_Binaries[(UInt8)ShaderType::VERTEX].binary) {
            device->CreateVertexShader(
                dataObject->m_Binaries[(UInt8)ShaderType::VERTEX].binary->GetBufferPointer(),
                dataObject->m_Binaries[(UInt8)ShaderType::VERTEX].binary->GetBufferSize(),
                nullptr,
                &(dataObject->m_NativeVS));
        }

        if (dataObject->m_Binaries[(UInt8)ShaderType::PIXEL].binary) {
            device->CreatePixelShader(
                dataObject->m_Binaries[(UInt8)ShaderType::PIXEL].binary->GetBufferPointer(),
                dataObject->m_Binaries[(UInt8)ShaderType::PIXEL].binary->GetBufferSize(),
                nullptr,
                &(dataObject->m_NativePS));
        }

        if (dataObject->m_Binaries[(UInt8)ShaderType::COMPUTE].binary) {
            device->CreateComputeShader(
                dataObject->m_Binaries[(UInt8)ShaderType::COMPUTE].binary->GetBufferPointer(),
                dataObject->m_Binaries[(UInt8)ShaderType::COMPUTE].binary->GetBufferSize(),
                nullptr,
                &dataObject->m_NativeCS);
        }
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11