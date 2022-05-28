#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Components/RenderResources/Materials/DX11ShaderData.h"
#include "Platform/DirectX11/Graphics/Components/RenderResources/Materials/DX11ShaderAPI.h"

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

    DX11ShaderAPI::_init DX11ShaderAPI::_initializer;

    void DX11ShaderAPI::Bind(ShaderData* dataObject) {
        ID3D11DeviceContext* context = DX11Common::GetContext();
        DX11ShaderData* shader = ShaderAPI::Cast<DX11ShaderData>(dataObject);

        if (shader->m_VertexShader) context->VSSetShader(shader->m_VertexShader, nullptr, 0);
        if (shader->m_PixelShader) context->PSSetShader(shader->m_PixelShader, nullptr, 0);
        if (shader->m_ComputeShader) context->CSSetShader(shader->m_ComputeShader, nullptr, 0);
    }

    void DX11ShaderAPI::Reload(ShaderData* dataObject) {
        ID3D11DeviceContext* context = DX11Common::GetContext();
        DX11ShaderData* shader = ShaderAPI::Cast<DX11ShaderData>(dataObject);

        DX11ShaderAPI::Load(shader);
    }

    void DX11ShaderAPI::Clean(ShaderData* dataObject) {
        ID3D11DeviceContext* context = DX11Common::GetContext();
        DX11ShaderData* shader = ShaderAPI::Cast<DX11ShaderData>(dataObject);

        for (auto& binary: shader->m_BinaryMap) {
            if (binary.second) binary.second->Release();
        }

        if (shader->m_VertexShader) shader->m_VertexShader->Release();
        if (shader->m_PixelShader) shader->m_PixelShader->Release();
        if (shader->m_ComputeShader) shader->m_ComputeShader->Release();

        shader->m_BinaryMap.clear();
        shader->m_ShaderSources.clear();

        shader->m_ShaderName.clear();
        shader->m_Filepath.clear();
    }

    STL::unordered_map<ShaderType, STL::string> DX11ShaderAPI::PreprocessSource(const STL::string& source) {
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

            shaderSources[Utils::s_ShaderStringTypeMap.at(type)] =
                (pos == STL::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void DX11ShaderAPI::CompileFromSource(DX11ShaderData* dataObject) {
        HRESULT result;
        ID3DBlob* errorMessages;

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef ST_DEBUG
        flags |= D3DCOMPILE_DEBUG;
#endif  // ST_DEBUG

        for (auto& tuple: dataObject->m_ShaderSources) {
            ShaderType type = tuple.first;
            const STL::string& shader = tuple.second;

            result = D3DCompile(
                shader.c_str(),
                shader.size(),
                nullptr,
                nullptr,
                D3D_COMPILE_STANDARD_FILE_INCLUDE,
                Utils::s_ShaderTypeEntryPointMap.at(type),
                Utils::s_ShaderTypeProfileMap.at(type),
                flags,
                0,
                &dataObject->m_BinaryMap.at(type),
                &errorMessages);

#if ST_DEBUG
            if (FAILED(result)) {
                char* error = (char*)errorMessages->GetBufferPointer();
                error[strnlen_s(error, 128) - 1] = '\0';

                ST_ENGINE_ERROR("{0}", error);
                errorMessages->Release();
                ST_ENGINE_ERROR(
                    "{0}::{1} Shader Compilation error",
                    dataObject->m_ShaderName.c_str(),
                    Utils::s_ShaderTypeStringMap.at(type).c_str());
                ST_ENGINE_ASSERT(false, "");
            }
#endif

            if (errorMessages) errorMessages->Release();
        }
    }

    void DX11ShaderAPI::Load(DX11ShaderData* dataObject) {
        DX11ShaderAPI::Reset(dataObject);
        STL::string& source = Filesystem::ReadTextFileAtPath(dataObject->m_Filepath);

        if (source.empty()) {
            ST_ENGINE_ASSERT("Shader source at path {0} is empty", dataObject->m_Filepath.c_str());
            return;
        }

        dataObject->m_ShaderSources = DX11ShaderAPI::PreprocessSource(source);
        DX11ShaderAPI::CompileFromSource(dataObject);

        if (dataObject->m_BinaryMap.at(ShaderType::VERTEX)) {
            DX11Common::GetDevice()->CreateVertexShader(
                dataObject->m_BinaryMap.at(ShaderType::VERTEX)->GetBufferPointer(),
                dataObject->m_BinaryMap.at(ShaderType::VERTEX)->GetBufferSize(),
                nullptr,
                &dataObject->m_VertexShader);
        }

        if (dataObject->m_BinaryMap.at(ShaderType::PIXEL)) {
            DX11Common::GetDevice()->CreatePixelShader(
                dataObject->m_BinaryMap.at(ShaderType::PIXEL)->GetBufferPointer(),
                dataObject->m_BinaryMap.at(ShaderType::PIXEL)->GetBufferSize(),
                nullptr,
                &dataObject->m_PixelShader);
        }

        if (dataObject->m_BinaryMap.at(ShaderType::COMPUTE)) {
            DX11Common::GetDevice()->CreateComputeShader(
                dataObject->m_BinaryMap.at(ShaderType::COMPUTE)->GetBufferPointer(),
                dataObject->m_BinaryMap.at(ShaderType::COMPUTE)->GetBufferSize(),
                nullptr,
                &dataObject->m_ComputeShader);
        }
    }

    void DX11ShaderAPI::Reset(DX11ShaderData* dataObject) {
        for (auto& binary: dataObject->m_BinaryMap) {
            if (binary.second != nullptr) {
                binary.second->Release();
                binary.second = nullptr;
            }
        }

        dataObject->m_BinaryMap.clear();
        dataObject->m_ShaderSources.clear();

        if (dataObject->m_VertexShader != nullptr) {
            dataObject->m_VertexShader->Release();
            dataObject->m_VertexShader = nullptr;
        }

        if (dataObject->m_PixelShader != nullptr) {
            dataObject->m_PixelShader->Release();
            dataObject->m_PixelShader = nullptr;
        }

        if (dataObject->m_ComputeShader != nullptr) {
            dataObject->m_ComputeShader->Release();
            dataObject->m_ComputeShader = nullptr;
        }
    }
}  // namespace Sentinel