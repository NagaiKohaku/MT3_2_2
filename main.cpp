#include <Novice.h>
#include <imgui.h>
#include "Vector3Math.h"
#include "MatrixMath.h"
#include "MakeMatrix.h"
#include "Draw.h"

const char kWindowTitle[] = "LE2B_17_ナガイ_コハク_3次元の衝突判定(平面と球)";

bool IsCollision(const Sphere& sphere, const Plane& plane);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//ウィンドウサイズ
	float kWindowWidth = 1280.0f;
	float kWindowHeight = 720.0f;

	Sphere sphere{ Vector3(0.0f,0.0f,0.0f),0.5f };

	Plane plane{ Vector3(0.0f,1.0f,0.0f),1.0f };

	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };

	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Draw draw;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		ImGui::Begin("Debug");
		ImGui::DragFloat3("Sphere.Center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("Sphere.Radius", &sphere.radius, 0.01f);
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Plane.Distance", &plane.distance, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::End();

		plane.normal = Vector3Math::Normalize(plane.normal);

		draw.Pipeline(cameraTranslate, cameraRotate, kWindowWidth, kWindowHeight);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		draw.DrawGrid();

		if (IsCollision(sphere,plane)) {

			draw.DrawSphere(sphere, RED);

		} else {

			draw.DrawSphere(sphere, WHITE);

		}

		draw.DrawPlane(plane, WHITE);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

bool IsCollision(const Sphere& sphere, const Plane& plane) {

	float distance = fabsf(Vector3Math::Dot(plane.normal, sphere.center) - plane.distance);

	if (distance <= sphere.radius) {
		return true;
	}

	return false;
}