#include "graphics.h"
#include <cmath>
#include <algorithm>

// 화면 채우는 함수
void FillScreen(std::vector<uint32_t>& buffer, uint32_t color) {
	std::fill(buffer.begin(), buffer.end(), color);
}

// 화면에 픽셀 찍는 함수
void PutPixel(std::vector<uint32_t>& buffer, int x, int y, uint32_t color) {
	// Todo 1: 범위 검사 작성
	// x나 y가 화면 범위보다 밖이라면, 함수 종료
	if (x >= WINDOW_WIDTH || x < 0 || y >= WINDOW_HEIGHT || y < 0) {
		return;
	}

	// Todo 2: 좌표 변환 공식 작성
	// buffer: 픽셀 데이터가 담긴 1차원 배열
	// 2차원 좌표를 1차원 배열 인덱스로 바꿔줘야 함
	// -> (y좌표값 * 화면 너비) + x좌표값
	int index = y * WINDOW_WIDTH + x;
	buffer[index] = color;
}

// 선 그리는 함수 (y = mx + b)
void DrawLine(std::vector<uint32_t>& buffer, int x1, int y1, int x2, int y2, uint32_t color) {
	// Todo 1:  변화량 절댓값 구하기
	int dx = x2 - x1;
	int dy = y2 - y1;

	// Todo 2: 기울기 경사 비교
	// std::abs 함수는 cmath 헤더에 있음

	// 1) 기울기가 완만한 경우
	if (std::abs(dx) >= std::abs(dy)) {
		// 기울기(m) 구하기
		float m = static_cast<float>(dy) / static_cast<float>(dx);

		// 점 찍기 (x값을 x1부터 x2까지 1씩 증가시키면서 점을 찍어줌)
		if (x2 > x1) {
			for (int x = x1; x <= x2; x++) {
				int y = y1 + m * (x - x1);
				PutPixel(buffer, x, y, color);
			}
		}
		else {
			for (int x = x2; x <= x1; x++) {
				int y = y2 + m * (x - x2);
				PutPixel(buffer, x, y, color);
			}
		}
	}
	// 2) 기울기가 가파른 경우
	else {
		// 기울기 역수(invm) 구하기
		float invm = static_cast<float>(dx) / static_cast<float>(dy);

		// 점 찍기 (y값을 y1부터 y2까지 1씩 증가시키면서 점을 찍어줌)
		if (y2 > y1) {
			for (int y = y1; y <= y2; y++) {
				int x = x1 + invm * (y - y1);
				PutPixel(buffer, x, y, color);
			}
		}
		else {
			for (int y = y2; y <= y1; y++) {
				int x = x2 + invm * (y - y2);
				PutPixel(buffer, x, y, color);
			}
		}
	}
}

// 삼각형 그리는 함수
void DrawAndFilledTriangle(std::vector<uint32_t>& buffer, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	// Todo 1: 삼각형의 각 꼭짓점 좌표 정렬

	// 1) y0 > y1 이면 두 점의 위치를 바꾼다. (아래로 갈 수록 값은 커짐)
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}
	// 2) y1 > y2 이면 두 점의 위치를 바꾼다.
	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}
	// 3) 위에서 y0과 y1의 위치가 바뀌었을 수 있음
	//    따라서 y0, y1의 바뀐 위치를 한 번 더 비교
	if (y0 > y1) {
		std::swap(y0, y1);
		std::swap(x0, x1);
	}

	// Todo 2: 삼각형 쪼개기
	// y2 좌표에서 삼각형 내부를 가로지르는 선 하나를 일직선으로 그어 높이가 똑같은 점 M을 구한다.
	int My = y1;
	int Mx = 0;

	if (y2 - y0 == 0) { // 삼각형 높이가 0인 경우 예외처리
		Mx = x1;
	}
	else { // 선형 보간 공식 사용
		Mx = x0 + static_cast<int>(static_cast<float>(x2 - x0) * (y1 - y0) / (y2 - y0));
	}

	// Todo 3: 위쪽 삼각형 채우기

	// 1) 역기울기 구하기
	float invSlope1 = 0;
	float invSlope2 = 0;

	if (y1 - y0 != 0) {
		invSlope1 = static_cast<float>(x1 - x0) / (y1 - y0);
	}
	if (y2 - y0 != 0) {
		invSlope2 = static_cast<float>(x2 - x0) / (y2 - y0);
	}

	// 2) 시작점 설정
	float curX1 = static_cast<float>(x0);
	float curX2 = static_cast<float>(x0);

	// 3) 한 줄씩 그리기
	for (int y = y0; y < y1; y++) {
		int startX = static_cast<int>(std::min(curX1, curX2));
		int endX = static_cast<int>(std::max(curX1, curX2));

		DrawLine(buffer, startX, y, endX, y, color);

		curX1 += invSlope1;
		curX2 += invSlope2;
	}

	// Todo 4: 아래쪽 삼각형 채우기

	// 1) y2 좌표에서 꺾이며 생기는 새 기울기 구하기
	float invSlope3 = 0;

	if (y2 - y1 != 0) {
		invSlope3 = static_cast<float>(x2 - x1) / (y2 - y1);
	}

	// 2) cur_x1, cur_x2 위치 다시 조정 
	curX1 = static_cast<float>(x1);
	curX2 = static_cast<float>(Mx);

	// 3) 한 줄씩 그리기
	for (int y = y1; y <= y2; y++) {
		int startX = static_cast<int>(std::min(curX1, curX2));
		int endX = static_cast<int>(std::max(curX1, curX2));

		DrawLine(buffer, startX, y, endX, y, color);

		curX1 += invSlope3;
		curX2 += invSlope2;
	}
}

// 3D 게임 오브젝트를 그리는 함수
void DrawGameObject(std::vector<uint32_t>& buffer,const GameObject& obj) {
	Mat4x4 matScale = Mat4x4::MakeScale(obj.scale.x, obj.scale.y, obj.scale.z);
	Mat4x4 matRotY = Mat4x4::MakeRotationY(obj.rotation.y);
	Mat4x4 matTrans = Mat4x4::MakeTranslation(obj.position.x, obj.position.y, obj.position.z);

	// 큐브의 모든 삼각형 순회
	for (auto tri : obj.mesh.tris) {
		Triangle translatedTri;

		for (int i = 0; i < 3; i++) {
			Vec3 v = tri.p[i];
			// 큐브가 원점을 중심으로 돌아가도록 큐브의 중심을 원점으로 조정
			v.x -= 0.5;
			v.y -= 0.5;
			v.z -= 0.5;

			// 인자로 받은 변환 행렬 적용 [S -> R -> T]순서
			v = matScale.MultiplyVector(v);
			v = matRotY.MultiplyVector(v);
			v = matTrans.MultiplyVector(v);

			// 변환할 위치 저장
			translatedTri.p[i] = v;
		}

		// 벡터의 뺄셈: 삼각형의 한 꼭짓점에서 출발하는 두 개의 선(벡터) 구하기
		Vec3 triLine1 = translatedTri.p[1] - translatedTri.p[0];
		Vec3 triLine2 = translatedTri.p[2] - translatedTri.p[0];

		// 외적: 두 벡터의 외적 계산을 통해 두 벡터에 모두 수직인 벡터 구하기 (->면이 바라보는 방향)
		Vec3 normalVec = triLine1.Cross(triLine2);

		// 카메라의 시선 벡터 구하기 (카메라 위치가 (0,0,0)이라면, 점의 위치가 카메라에서 뻗어나간 시선 벡터)
		Vec3 cameraVec = translatedTri.p[0];

		// 내적: 카메라의 시선 방향과 면의 시선 방향 비교 (법선 벡터와 카메라 시선 벡터의 내적 < 0이면, 앞면)
		if (normalVec.Dot(cameraVec) < 0) {
			Triangle projectedTri;

			for (int i = 0; i < 3; i++) {
				// 2D 화면으로 투영 (원근법 적용)
				projectedTri.p[i].x = translatedTri.p[i].x / translatedTri.p[i].z;
				projectedTri.p[i].y = translatedTri.p[i].y / translatedTri.p[i].z;

				// 화면 중앙으로 좌표 옮기기 & 크기 키우기 (투영된 값이 매우 작기 때문에)
				projectedTri.p[i].x = (projectedTri.p[i].x + 1) * 0.5 * WINDOW_WIDTH;
				projectedTri.p[i].y = (projectedTri.p[i].y + 1) * 0.5 * WINDOW_HEIGHT;
			}

			// 그리기
			DrawAndFilledTriangle(buffer, projectedTri.p[0].x, projectedTri.p[0].y, projectedTri.p[1].x, projectedTri.p[1].y, projectedTri.p[2].x, projectedTri.p[2].y, obj.color.red);
		}
	}
}