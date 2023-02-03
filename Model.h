#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <vector>
#include <DirectXMath.h>


using namespace DirectX;

class Model
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �ÓI�����o�֐�

	// ONBJ�t�@�C������3D���f����ǂݍ���
	static Model* LoadFromOBJ(const std::string& modelName);

	// setter
	static void SetDevice(ID3D12Device* device) { 
		Model::device = device; }

public: // �T�u�N���X

	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//	�A���r�G���g�W��
		float pad1;			//	�p�f�B���O
		XMFLOAT3 diffuse;	//	�f�B�t���[�Y�W��
		float pad2;			//	�p�f�B���O
		XMFLOAT3 specular;	//	�X�y�L�����[�W��
		float alpha;		//	�A���t�@
	};

	// �}�e���A��
	struct Material
	{
		std::string name;	//	�}�e���A��
		XMFLOAT3 ambient;	//	�A���r�G���g�e���x
		XMFLOAT3 diffuse;	//	�f�B�t���[�Y�e���x
		XMFLOAT3 specular;	//	�X�y�L�����[�e���x
		float alpha;	//	�A���t�@
		std::string textureFilename;	//	�e�N�X�`���t�@�C����
		// �R���X�g���N�^
		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

public: // �����o�֐�

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�`��R�}���h���X�g</param>
	/// <param name="rootParamIndexMaterial">�}�e���A���p���[�g�p�����[�^�ԍ�</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);


	// �e��o�b�t�@�쐬
	void CreateBuffers();

private: // �ÓI�����o�ϐ�
	
	// �f�o�C�X
	static ID3D12Device* device;

	
private: // �����o�ϐ�
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;
	// �R�}���h���X�g
	ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �r���[�s��
	XMMATRIX matView;
	// �ˉe�s��
	XMMATRIX matProjection;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	// �}�e���A��
	Material material;

	// ���_���C���f�b�N�z��
	std::vector<unsigned short> indices;

	std::vector<VertexPosNormalUv> vertices;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1; 

private: // ����J�̃����o�֐�

	// OBJ�t�@�C������3D���f����ǂݍ���(����J)
	void LoadFromOBJInternal(const std::string& modelName);
};