// Router placements, Hash Code 2017 finals
// https://storage.googleapis.com/coding-competitions.appspot.com/HC/2017/hashcode2017_final_task.pdf
// https://github.com/Errichto/youtube/blob/master/hashcode/pro.cpp

#include <bits/stdc++.h>
using namespace std;

#define imie(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "

int H, W, R;
int cable_cost, router_cost, B;
pair<int,int> start;

const int MAX_N = 1005;
char grid[MAX_N][MAX_N];
int covered[MAX_N][MAX_N];
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};
int cost;
vector<pair<int,int>> cables, routers;

bool inside(int row, int col) {
	return 0 <= row && row < H && 0 <= col && col < W;
}

int main() {
	scanf("%d%d%d", &H, &W, &R);
	scanf("%d%d%d", &cable_cost, &router_cost, &B);
	scanf("%d%d", &start.first, &start.second);
	for(int row = 0; row < H; ++row) {
		scanf("%s", grid[row]);
	}
	
	//code
	
	printf("%d\n", (int) cables.size());
	for(pair<int,int> cell : cables) {
		printf("%d %d\n", cell.first, cell.second);
	}
	printf("%d\n", (int) routers.size());
	for(pair<int,int> cell : routers) {
		printf("%d %d\n", cell.first, cell.second);
	}
	
}
