﻿#if _MSC_VER
#pragma warning(disable:4996)
#endif
#include <iostream>
#include <CoalaMOD.h>
#pragma comment(lib, "CoalaMOD.lib")
#define locateBlock_s(block, x, y, z) if (sx <= x && x <= ex && sy <= y && y <= ey && sz <= z && z <= ez) locateBlock(block, x, y, z)
#define locateBricks_s(block, x, y, z) if (sx <= x && x <= ex && sy <= y && y <= ey && sz <= z && z <= ez) locateBricks(block, x, y, z)
#define lnstallStairs_s(stairs, x, y, z, orientation) if (sx <= x && x <= ex && sy <= y && y <= ey && sz <= z && z <= ez) installStairs(stairs, x, y, z, orientation)
#define SIMPLE_ORIENTATION calculateOrientation(x, z, sx + offset, sz + offset, ex - offset, ez - offset)
#define MAKE_ODD(a, b) if((b - a) % 2 == 1) b -= 1
#define SWAP_IF_NEEDED(A,B) if(A > B) { int t = A; A = B; B = t; } 1 + 1


BlockOrientation calculateOrientation(int x, int z, int sx, int sz, int ex, int ez) {
	// x+ = east
	// z+ = south
	if (x == sx) {
		return FACING_EAST;
	}
	else if (x == ex) {
		return FACING_WEST;
	}
	else if (z == sz) {
		// westnorth
		return FACING_SOUTH;
	}
	else if (z == ez) {
		return FACING_NORTH;
	}
	throw (BlockOrientation)-1;
}

int main()
{
	// 좌표 입력
	int sx, sy, sz, ex, ey, ez;
	scanf("%d %d %d %d %d %d", &sx, &sy, &sz, &ex, &ey, &ez);

	// 정규화
	SWAP_IF_NEEDED(sx, ex);
	SWAP_IF_NEEDED(sy, ey);
	SWAP_IF_NEEDED(sz, ez);

	// 홀수만
	MAKE_ODD(sx, ex);
	MAKE_ODD(sz, ez);

	// 블럭 선언
	BricksID bricks = createBricks(BRICKS_STONE);
	BlockID air = createBlock(BLOCK_AIR);
	BlockID light = createBlock(BLOCK_GLOWSTONE);
	BlockID glass = createBlock(BLOCK_GLASS);
	StairsID stairs = createStairs(STAIRS_STONE_BRICK, false);

	// 밀기
	for (int x = sx; x <= ex; x++)
		for (int y = sy; y <= ey; y++)
			for (int z = sz; z <= ez; z++)
				locateBlock_s(air, x, y, z);

	// 토대
	const int baseHeight = 2;
	const int baseTop = sy + baseHeight - 1;
	for (int y = sy, offset = 0; y <= baseTop; y++, offset++)
		for (int x = sx + offset; x <= ex - offset; x++)
			for (int z = sz + offset; z <= ez - offset; z++) {
				locateBricks_s(bricks, x, y, z);
				if (x == sx + offset || x == ex - offset || z == sz + offset || z == ez - offset)
					lnstallStairs_s(stairs, x, y, z, SIMPLE_ORIENTATION);
			}

					

	// X축 기둥 건설
	for (int x = sx + 3; x <= ex - 3; x += 2) {
		for (int y = baseTop; y <= ey; y++) {
			locateBricks_s(bricks, x, y, sz + 3);
			locateBricks_s(bricks, x, y, ez - 3);
		}
	}

	// Z축 기둥 건설
	for (int z = sz + 3; z <= ez - 3; z += 2) {
		for (int y = baseTop; y <= ey; y++) {
			locateBricks_s(bricks, sx + 3, y, z);
			locateBricks_s(bricks, ex - 3, y, z);
		}
	}

	// 지붕
	for (int y = ey - 2, offset = 0; y <= ey; y++, offset++)
		for (int x = sx + offset; x <= ex - offset; x++)
			for (int z = sz + offset; z <= ez - offset; z++)
				if (x != sx + offset && x != ex - offset && z != sz + offset && z != ez - offset) {
					if (y != ey - 2)
						locateBricks_s(bricks, x, y, z);
				}
				else
					lnstallStairs_s(stairs, x, y, z, SIMPLE_ORIENTATION);

	// 전등
	for (int x = sx + 6; x <= ex - 6; x += 2)
		for (int z = sz + 6; z <= ez - 6; z += 2) {
			locateBlock_s(glass, x, ey - 1, z);
			locateBlock_s(light, x, ey, z);
		}
}
