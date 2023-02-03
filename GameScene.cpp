#include "GameScene.h"
#include <cassert>
#include "Collision.h"
#include <sstream>
#include <iomanip>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete model_;
	delete model2_;
	delete object3d_;
	delete object3d2_;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	Sprite::LoadTexture(2, L"Resources/texture.png");

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	model_ = Model::LoadFromOBJ("green");
	model2_ = Model::LoadFromOBJ("Plane");

	// 3Dオブジェクト生成
	object3d_ = Object3d::Create();
	object3d2_ = Object3d::Create();
	object3d_->SetModel(model_);
	object3d2_->SetModel(model2_);
	object3d2_->SetPosition({ 0,-7.5f,0 });
	object3d_->Update();
	object3d2_->Update();

	// 球の初期値を設定
	sphere.center = XMVectorSet(0, 2, 0, 1); // 中心点座標
	sphere.radius = 5.0f; // 半径

	// 平面の初期値を設定
	plane.normal = XMVectorSet(0, 1, 0, 0); // 法線ベクトル
	plane.distance = -7.0f; // 原点(0,0,0)からの距離
}

void GameScene::Update()
{
	// オブジェクト移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_A) || input->PushKey(DIK_D))
	{
		// 現在の座標を取得
		XMFLOAT3 position = object3d_->GetPosition();

		// 移動後の座標を計算
		if (input->PushKey(DIK_W)) { position.y += 0.1f; }
		else if (input->PushKey(DIK_S)) { position.y -= 0.1f; }
		if (input->PushKey(DIK_D)) { position.x += 0.1f; }
		else if (input->PushKey(DIK_A)) { position.x -= 0.1f; }

		// 座標の変更を反映
		object3d_->SetPosition(position);
	}

	// 球移動
	{
		XMVECTOR moveY = XMVectorSet(0, 0.1f, 0, 0);
		if (input->PushKey(DIK_W)) { sphere.center += moveY; }
		else if (input->PushKey(DIK_S)) { sphere.center -= moveY; }

		XMVECTOR moveX = XMVectorSet(0.1f, 0, 0,0);
		if (input->PushKey(DIK_A)) { sphere.center += moveX; }
		else if (input->PushKey(DIK_D)) { sphere.center -= moveX; }

		// 現在の座標を取得
		XMFLOAT3 position = object3d_->GetPosition();

	}

	object3d_->Update();


	// stringstreamで変数の値を埋め込んで整形する
	std::ostringstream spherestr;
	spherestr << "Sphere:("
		<< std::fixed << std::setprecision(2)	//	小数点以下2桁まで
		<< sphere.center.m128_f32[0] << ","		//	x
		<< sphere.center.m128_f32[1] << ","		//	y
		<< sphere.center.m128_f32[2] << ")";	//	z

	debugText.Print(spherestr.str(), 50, 180, 1.0f);

	// 球と平面の当たり判定
	bool hit = Collision::CheckSphere2Plane(sphere, plane);
	if (hit)
	{
		debugText.Print("HIT", 50, 200, 1.0f);
	}

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();
	//sprite1->Draw();
	//sprite2->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	object3d_->Draw();
	object3d2_->Draw();
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
