#pragma once

#include "Object2D.h"
#include <unordered_map>

static const XMMATRIX KMatrixIdentity{ XMMatrixIdentity() };

static bool operator==(const XMVECTOR& A, const XMVECTOR& B)
{
	return XMVector3Equal(A, B);
}

static string ConvertXMVECTORToString(const XMVECTOR& Vector)
{
	float X{ XMVectorGetX(Vector) };
	float Y{ XMVectorGetY(Vector) };
	float Z{ XMVectorGetZ(Vector) };
	float W{ XMVectorGetW(Vector) };

	string Result{ to_string(X) + "#" + to_string(Y) + "#" + to_string(Z) + "#" + to_string(W) };
	return Result;
}

static SObject2DData Generate2DRectangle(const XMFLOAT2& RectangleSize)
{
	SObject2DData Result{};

	float HalfWidth{ RectangleSize.x / 2 };
	float HalfHeight{ RectangleSize.y / 2 };

	Result.vVertices.emplace_back(SVertex2D(XMVectorSet(-HalfWidth, +HalfHeight, 0, 1), XMVectorSet(1, 1, 1, 1), XMVectorSet(0.0f, 0.0f, 0, 0)));
	Result.vVertices.emplace_back(SVertex2D(XMVectorSet(+HalfWidth, +HalfHeight, 0, 1), XMVectorSet(1, 1, 1, 1), XMVectorSet(1.0f, 0.0f, 0, 0)));
	Result.vVertices.emplace_back(SVertex2D(XMVectorSet(-HalfWidth, -HalfHeight, 0, 1), XMVectorSet(1, 1, 1, 1), XMVectorSet(0.0f, 1.0f, 0, 0)));
	Result.vVertices.emplace_back(SVertex2D(XMVectorSet(+HalfWidth, -HalfHeight, 0, 1), XMVectorSet(1, 1, 1, 1), XMVectorSet(1.0f, 1.0f, 0, 0)));

	Result.vTriangles.emplace_back(STriangle(0, 1, 2));
	Result.vTriangles.emplace_back(STriangle(1, 3, 2));

	return Result;
}