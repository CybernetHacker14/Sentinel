#include "stpch.h"

#ifdef ST_RENDERER_DX11
    #include "Sentinel/Graphics/Material/ShaderAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/Windows/DirectX11/Graphics/Core/DX11Common.h"

    #include "Sentinel/Common/Core/Assert.h"
    #include "Sentinel/Common/Core/Malloc.h"
    #include "Sentinel/Filesystem/Filesystem.h"

    #include <sparse_map.h>

namespace Sentinel {
    namespace DX11ShaderAPIUtils {
        static tsl::sparse_map<std::string, Sentinel::ShaderType> s_ShaderStringTypeMap = {
            {"vertex", ShaderType::VERTEX}, {"pixel", ShaderType::PIXEL}, {"compute", ShaderType::COMPUTE}};

        static CChar* s_ShaderTypeString[3] = {"vertex", "pixel", "compute"};
        static CChar* s_ShaderTypeProfile[3] = {"vs_5_0", "ps_5_0", "cs_5_0"};
        static CChar* s_ShaderTypeEntryPoint[3] = {"VShader", "PShader", "CShader"};

        static void PreprocessSource(CChar* source, Char** sources) {
            std::string sourceString(source);

            CChar* typeToken = "#type";
            Size_t typeTokenLength = strlen(typeToken);
            Size_t pos = sourceString.find(typeToken, 0);  // Start of shader type declaration file;

            while (pos != std::string::npos) {
                Size_t eol = sourceString.find_first_of("\r\n", pos);  // End of shader type declaration line
                ST_BREAKPOINT_ASSERT(eol != std::string::npos, "Syntax error");
                Size_t begin = pos + typeTokenLength + 1;  // Start of shader type name (after "#type" keyword)
                const std::string& type = sourceString.substr(begin, eol - begin);

                Size_t nextLinePos = sourceString.find_first_not_of(
                    "\r\n", eol);  // Start of shader code after shader type declaration line
                ST_BREAKPOINT_ASSERT(nextLinePos != std::string::npos, "Syntax error");
                pos = sourceString.find(typeToken, nextLinePos);  // Start of next shader type declaration line

                std::string subShaderSource =
                    ((pos == std::string::npos) ? sourceString.substr(nextLinePos)
                                                : sourceString.substr(nextLinePos, pos - nextLinePos));

                // TODO : Data is not being copied properly
                sources[(UInt8)s_ShaderStringTypeMap[type]] = (Char*)Malloc(subShaderSource.size() + 1);
                sources[(UInt8)s_ShaderStringTypeMap[type]][subShaderSource.size()] = '\0';
                MemFunctions::Memcpy(
                    sources[(UInt8)s_ShaderStringTypeMap[type]], subShaderSource.c_str(), subShaderSource.size());

                /*sources[(UInt8)s_ShaderStringTypeMap[type]] =
                    ((pos == std::string::npos) ? sourceString.substr(nextLinePos)
                                                : sourceString.substr(nextLinePos, pos - nextLinePos))
                        .data();*/
            }
        }

        // TODO : Maybe rethink about this, whether the count
        static void CompileFromSource(Char** sources, ID3DBlob** binaries) {
            HRESULT result;
            ID3DBlob* errorMessages;

            UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

    #ifdef ST_DEBUG
            flags |= D3DCOMPILE_DEBUG;
    #endif  // ST_DEBUG

            for (UInt32 i = (UInt32)ShaderType::VERTEX; i <= (UInt32)ShaderType::COMPUTE; i++) {
                if (!sources[i]) continue;

                Size_t shaderSourceSize = strlen(sources[i]);
                if (shaderSourceSize == 0) continue;

                CChar* ep = s_ShaderTypeEntryPoint[i];
                CChar* profile = s_ShaderTypeProfile[i];

                result = D3DCompile(
                    sources[i],
                    shaderSourceSize,
                    nullptr,
                    nullptr,
                    D3D_COMPILE_STANDARD_FILE_INCLUDE,
                    ep,
                    profile,
                    flags,
                    0,
                    &binaries[i],
                    &errorMessages);
            }

    #if ST_DEBUG
            if (FAILED(result)) {
                Char* error = (Char*)errorMessages->GetBufferPointer();
                error[strnlen_s(error, 128) - 1] = '\0';

                ST_TERMINAL_ERROR("%s", error);
                errorMessages->Release();
            }
    #endif

            if (errorMessages) errorMessages->Release();
        }
    }  // namespace DX11ShaderAPIUtils

    ShaderData* Sentinel::ShaderAPI::CreateShaderData(
        FixedSlabAllocator<ShaderData>& allocator, ContextData* context, CChar* filepath, CChar* name) {
        ShaderData* shaderObject = allocator.New();  // Remove the <T> from New calls in other classes
        shaderObject->m_Filepath = filepath;
        shaderObject->m_ShaderName = name;
        shaderObject->Context = context;

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
            if (binary) {
                binary->Release();
                binary = NULL;
            }
        }

        // dataObject->m_ShaderName.clear();
        // dataObject->m_Filepath.clear();
    }

    void ShaderAPI::Unbind(ShaderData* dataObject) {
        for (auto& binary: dataObject->m_Binaries) {
            if (binary != NULL) {
                binary->Release();
                binary = nullptr;
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

    void ShaderAPI::Load(ShaderData* dataObject) {
        Path filePath(dataObject->m_Filepath);
        Int64 shaderSourceSize = Filesystem::GetFileSize(filePath);

        if (shaderSourceSize == 0) {
            ST_TERMINAL_ERROR("Shader source at path : %s is empty", dataObject->m_Filepath);
            return;
        }

        Char* buffer = (Char*)Malloc(shaderSourceSize + 1);
        buffer[shaderSourceSize] = '\0';
        if (!Filesystem::ReadTextFileAtPath(dataObject->m_Filepath, buffer, shaderSourceSize)) {
            ST_TERMINAL_ERROR("Error reading shader file at path : %s", dataObject->m_Filepath);
            return;
        }

        DX11ShaderAPIUtils::PreprocessSource(buffer, dataObject->m_Sources);
        DX11ShaderAPIUtils::CompileFromSource(dataObject->m_Sources, dataObject->m_Binaries);

        ID3D11Device* device = ContextAPI::GetDevice(dataObject->Context);

        if (dataObject->m_Binaries[(UInt8)ShaderType::VERTEX]) {
            device->CreateVertexShader(
                dataObject->m_Binaries[(UInt8)ShaderType::VERTEX]->GetBufferPointer(),
                dataObject->m_Binaries[(UInt8)ShaderType::VERTEX]->GetBufferSize(),
                nullptr,
                &(dataObject->m_NativeVS));
        }

        if (dataObject->m_Binaries[(UInt8)ShaderType::PIXEL]) {
            device->CreatePixelShader(
                dataObject->m_Binaries[(UInt8)ShaderType::PIXEL]->GetBufferPointer(),
                dataObject->m_Binaries[(UInt8)ShaderType::PIXEL]->GetBufferSize(),
                nullptr,
                &(dataObject->m_NativePS));
        }

        if (dataObject->m_Binaries[(UInt8)ShaderType::COMPUTE]) {
            device->CreateComputeShader(
                dataObject->m_Binaries[(UInt8)ShaderType::COMPUTE]->GetBufferPointer(),
                dataObject->m_Binaries[(UInt8)ShaderType::COMPUTE]->GetBufferSize(),
                nullptr,
                &dataObject->m_NativeCS);
        }
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11