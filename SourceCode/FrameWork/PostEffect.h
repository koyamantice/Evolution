#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite {
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    PostEffect();
    /// <summary>
    /// ������
    /// </summary>
    void Initialize();
    /// <summary>
    /// �`��
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);
    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
    /// <summary>
    /// �V�[���`���n��
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);


    void CreateGraphicsPipelineState();
public:
    ComPtr<ID3D12Resource> texBuff;

    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    ComPtr<ID3D12Resource> depthBuff;
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
    // ���[�g�V�O�l�`��
    static ComPtr<ID3D12RootSignature> rootSignature;
    // �p�C�v���C���X�e�[�g�I�u�W�F�N�g
    static ComPtr<ID3D12PipelineState> pipelineState;

private:
    // Microsoft::WRL::���ȗ�
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
    //��ʃN���A�J���[
    static const float clearColor[4];


};

