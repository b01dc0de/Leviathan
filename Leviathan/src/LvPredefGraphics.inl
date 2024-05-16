#if defined(LVPREDEFGRAPHICS_INL) || !defined(LVGRAPHICS_H)
	#error "Should only be included (once) in LvGraphics.cpp"
#else
#define LVPREDEFGRAPHICS_INL
namespace Leviathan
{
	inline namespace LvPredefGraphicsConstants
	{
		static constexpr D3D11_PRIMITIVE_TOPOLOGY LvDefault_Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		static constexpr DXGI_FORMAT LvDefault_IndexFormat = DXGI_FORMAT_R32_UINT;
		static constexpr int LvDefault_WorldCBufferIdx = 0;
		static constexpr int LvDefault_ViewProjCBufferIdx = 1;
		static constexpr float LvDefault_ClearColor[4] = { 0.1f, 0.2f, 0.3f, 1.0f };
		static constexpr D3D_FEATURE_LEVEL LvDefault_FeatureLevel = D3D_FEATURE_LEVEL_11_1;
		static constexpr DXGI_FORMAT LvDefault_RenderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		static constexpr D3D_FEATURE_LEVEL LvDefault_SupportedFeatureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		};
		static unsigned int LvDefault_NumSupportedFeatureLevels = LV_ARRAYSIZE(LvDefault_SupportedFeatureLevels);

		static constexpr DXGI_FORMAT LvDefault_TextureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

		static constexpr D3D11_RASTERIZER_DESC WireframeRasterDesc =
		{
			D3D11_FILL_WIREFRAME, //D3D11_FILL_MODE FillMode,
			D3D11_CULL_NONE, //D3D11_CULL_MODE CullMode,
			TRUE, //BOOL FrontCounterClockwise,
			0, //INT DepthBias,
			0.0f, //FLOAT DepthBiasClamp,
			0.0f,//FLOAT SlopeScaledDepthBias;
			TRUE,//BOOL DepthClipEnable,
			FALSE,//BOOL ScissorEnable,
			TRUE,//BOOL MultisampleEnable,
			TRUE//BOOL AntialiasedLineEnable
		};

		static constexpr D3D_SHADER_MACRO ColorShader_Defines[] =
		{
			"ENABLE_WVP_TRANSFORM", "1",
			"COMBINED_WVP_BUFFER", "0",
			"ENABLE_VERTEX_TEXTURE", "0",
			"ENABLE_VERTEX_COLOR", "1",
			NULL, NULL
		};
		static constexpr D3D_SHADER_MACRO TextureShader_Defines[] =
		{
			"ENABLE_WVP_TRANSFORM", "1",
			"COMBINED_WVP_BUFFER", "0",
			"ENABLE_VERTEX_TEXTURE", "1",
			"ENABLE_VERTEX_COLOR", "0",
			NULL, NULL
		};

		static const InputDescType VxColor_InputLayoutDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		static const int VxColor_NumInputElements = LV_ARRAYSIZE(VxColor_InputLayoutDesc);

		static const InputDescType VxUV_InputLayoutDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		static const int VxUV_NumInputElements = LV_ARRAYSIZE(VxUV_InputLayoutDesc);
	}

	// **INFOMERCIAL ANNOUNCER VOICE** THERE MUST BE A BETTER WAY
	const InputDescType* VxTypeDef<VertexColor>::InputDescDef = VxColor_InputLayoutDesc;
	const int VxTypeDef<VertexColor>::NumInputElements = VxColor_NumInputElements;

	const D3D11_INPUT_ELEMENT_DESC* VxTypeDef<VertexUV>::InputDescDef = VxUV_InputLayoutDesc;
	const int VxTypeDef<VertexUV>::NumInputElements = VxUV_NumInputElements;

	struct LvPredefGraphics
	{
		ID3D11Texture2D* FallbackTexture = nullptr;
		ID3D11ShaderResourceView* FallbackTexture_SRV = nullptr;
		ID3D11SamplerState* FallbackSamplerState = nullptr;

		void PvInitFallbackTextureState(ID3D11Device* DXDevice);

		void InitState(ID3D11Device* DXDevice)
		{
			Assert(DXDevice);
			PvInitFallbackTextureState(DXDevice);
		}

		void TermState()
		{
			DXSAFERELEASE(FallbackTexture);
			DXSAFERELEASE(FallbackTexture_SRV);
			DXSAFERELEASE(FallbackSamplerState);
		}
	};

	void LvPredefGraphics::PvInitFallbackTextureState(ID3D11Device* DXDevice)
	{
		struct fRGBAColor
		{
			float R;
			float G;
			float B;
			float A;
		};
		constexpr fRGBAColor Pink{ 1.0f, 73.0f / 255.0f, 173.0f / 255.0f, 1.0f};
		constexpr fRGBAColor Black{ 0.0f, 0.0f, 0.0f, 1.0f};
		constexpr fRGBAColor Red{ 1.0f, 0.0f, 0.0f, 1.0f };
		constexpr fRGBAColor Green{ 0.0f, 1.0f, 0.0f, 1.0f };
		constexpr fRGBAColor Blue{ 0.0f, 0.0f, 1.0f, 1.0f };
		constexpr fRGBAColor White{ 1.0f, 1.0f, 1.0f, 1.0f };

		constexpr int Size = 512;
		constexpr int NumCells = 16;
		constexpr int CellSize = Size / NumCells;
		constexpr int MaxCell = (Size - 1) / CellSize;
		fRGBAColor* DefaultTextureData = new fRGBAColor[Size*Size];
		{
			for (int RowIdx = 0; RowIdx < Size; RowIdx++)
			{
				int CellRow = RowIdx / CellSize;
				for (int ColIdx = 0; ColIdx < Size; ColIdx++)
				{
					int CellCol = ColIdx / CellSize;
					bool bEvenCell = ((CellRow + CellCol) % 2 == 0);

					if (CellRow == 0 && CellCol == 0)
					{
						DefaultTextureData[(RowIdx * Size) + ColIdx] = Red;
					}
					else if (CellRow == 0 && CellCol == MaxCell)
					{
						DefaultTextureData[(RowIdx * Size) + ColIdx] = Green;
					}
					else if (CellRow == MaxCell && CellCol == 0)
					{
						DefaultTextureData[(RowIdx * Size) + ColIdx] = Blue;
					}
					else if (CellRow == MaxCell && CellCol == MaxCell)
					{
						DefaultTextureData[(RowIdx * Size) + ColIdx] = White;
					}
					else
					{
						DefaultTextureData[(RowIdx * Size) + ColIdx] = bEvenCell ? Pink : Black;
					}
				}
			}
		}
		D3D11_SUBRESOURCE_DATA TexDataDesc[] = { {} };
		TexDataDesc[0].pSysMem = DefaultTextureData;
		TexDataDesc[0].SysMemPitch = sizeof(fRGBAColor) * Size;
		TexDataDesc[0].SysMemSlicePitch = sizeof(fRGBAColor) * Size * Size;

		D3D11_TEXTURE2D_DESC DefaultTextureDesc = {};
		DefaultTextureDesc.Width = Size;
		DefaultTextureDesc.Height = Size;
		DefaultTextureDesc.MipLevels = 1;
		DefaultTextureDesc.ArraySize = 1;
		DefaultTextureDesc.Format = LvDefault_TextureFormat;
		DefaultTextureDesc.SampleDesc.Count = 1;
		DefaultTextureDesc.SampleDesc.Quality = 0;
		DefaultTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		DefaultTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		DefaultTextureDesc.CPUAccessFlags = 0;
		DefaultTextureDesc.MiscFlags = 0;
		DXCHECK(DXDevice->CreateTexture2D(&DefaultTextureDesc, &TexDataDesc[0], &FallbackTexture));

		delete[] DefaultTextureData;

		DXCHECK(DXDevice->CreateShaderResourceView(FallbackTexture, nullptr, &FallbackTexture_SRV));

		D3D11_SAMPLER_DESC DefaultTextureSamplerDesc = {};
		DefaultTextureSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		DefaultTextureSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultTextureSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultTextureSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultTextureSamplerDesc.MipLODBias = 0.0f;
		DefaultTextureSamplerDesc.MaxAnisotropy = 0; 
		DefaultTextureSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		DefaultTextureSamplerDesc.MinLOD = 0;
		DefaultTextureSamplerDesc.MaxLOD = 0;
		DXCHECK(DXDevice->CreateSamplerState(&DefaultTextureSamplerDesc, &FallbackSamplerState));
	}

	template <typename VxType>
	struct LvDrawState
	{
		ID3D11InputLayout* InputDataFormat = nullptr;
		ID3D11VertexShader* VertexShader = nullptr;
		ID3D11PixelShader* PixelShader = nullptr;
		ID3D11Buffer* VertexBuffer = nullptr;
		ID3D11Buffer* IndexBuffer = nullptr;
		ID3D11Buffer* WorldBuffer = nullptr;
		ID3D11Buffer* ViewProjBuffer = nullptr;
		ID3D11ShaderResourceView* OptFallbackTextureSRV = nullptr;
		ID3D11SamplerState* OptFallbackSampler = nullptr;


		void Draw(ID3D11DeviceContext* InDeviceContext, int NumPrims, const Matrix* pWorld = nullptr, const Matrix* pViewProj = nullptr)
		{
			Assert(InDeviceContext);
			UINT Stride = sizeof(VxType);
			UINT Offset = 0;
			InDeviceContext->IASetPrimitiveTopology(LvDefault_Topology);
			InDeviceContext->IASetInputLayout(InputDataFormat);
			InDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
			InDeviceContext->IASetIndexBuffer(IndexBuffer, LvDefault_IndexFormat, 0);

			InDeviceContext->VSSetShader(VertexShader, nullptr, 0);
			InDeviceContext->PSSetShader(PixelShader, nullptr, 0);
			if (OptFallbackTextureSRV) { InDeviceContext->PSSetShaderResources(0, 1, &OptFallbackTextureSRV); }
			if (OptFallbackSampler) { InDeviceContext->PSSetSamplers(0, 1, &OptFallbackSampler); }

			InDeviceContext->VSSetConstantBuffers(LvDefault_WorldCBufferIdx, 1, &WorldBuffer);
			InDeviceContext->VSSetConstantBuffers(LvDefault_ViewProjCBufferIdx, 1, &ViewProjBuffer);

			if (pWorld) { InDeviceContext->UpdateSubresource(WorldBuffer, 0, nullptr, pWorld, sizeof(Matrix), 0); }
			if (pViewProj) { InDeviceContext->UpdateSubresource(ViewProjBuffer, 0, nullptr, pViewProj, sizeof(Matrix) * 2, 0); }

			InDeviceContext->DrawIndexed(NumPrims * 3, 0u, 0u);
		}

		void Init
		(
			ID3D11InputLayout* InIAFmt,
			ID3D11VertexShader* InVS,
			ID3D11PixelShader* InPS,
			ID3D11Buffer* InVxBuffer,
			ID3D11Buffer* InIxBuffer,
			ID3D11Buffer* InWCbuffer,
			ID3D11Buffer* InVPCbuffer,
			ID3D11ShaderResourceView* InOptFallbackTextureSRV = nullptr,
			ID3D11SamplerState* InOptFallbackSampler = nullptr
		)
		{
			InputDataFormat = InIAFmt;
			VertexShader = InVS;
			PixelShader = InPS;
			VertexBuffer = InVxBuffer;
			IndexBuffer = InIxBuffer;
			WorldBuffer = InWCbuffer;
			ViewProjBuffer = InVPCbuffer;
			OptFallbackTextureSRV = InOptFallbackTextureSRV;
			OptFallbackSampler = InOptFallbackSampler;
		}
	};

}
#endif // LVPREDEFGRAPHICS_INL
