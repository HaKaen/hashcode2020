#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <bitset>
#include <assert.h> 
#include <limits.h>

using namespace std;

struct Lib {
	int n, signup, perday;
	vector<int> books;
	vector<long> tbs; // total book score (cumulative)
	void read(const vector<int>& book_scores) {
		scanf("%d%d%d", &n, &signup, &perday);
		perday = min(n, perday); // useless to have more threads than books
		for (int i = 0; i < n; ++i) {
			int x;
			scanf("%d", &x);
			books.push_back(x);
			tbs.push_back(book_scores[x]);
		}
		sort(tbs.rbegin(), tbs.rend()); // decreasingly
		for (int i = 1; i < (int)tbs.size(); ++i) {
			tbs[i] += tbs[i - 1]; 
		}
	}
};

/*
B : lib->score = 2 * lib->score - mean * ((long double)D - lib->signup - mean_signup);			5 822 900
C :
D :
E :
F : lib->score + mean * ((long double)D - lib->signup - mean_signup);							5 089 564

lib->score = lib->tbs / lib->nb * lib->perday * min(lib->dtr, D);		//4M122 sur B, 4M837 sur D
lib->score = lib->tbs * min(lib->dtr, D);								//4M122 sur B, 4M837 sur D
lib->score = (long double)lib->tbs / lib->perday;						//2M032 sur F
lib->score = lib->tbs;													//4M122 sur B, 4M837 sur D
lib->score = min(lib->dtr, D);											//4M122 sur B, 4M837 sur D
lib->score = lib->tbs * lib->dtr / D;									//GARBAGE
lib->score = (long double)lib->dtr / max(lib->dtr, D) * lib->tbs;		//4M122 sur B, 4M837 sur D
lib->score = (long double)1/(D - min(lib->dtr, D));						//4M122 sur B, 4M837 sur D
*/

int main() {
	/*READ INPUT*/
	int B, L, D;
	scanf("%d%d%d", &B, &L, &D);
	vector<int> book_scores(B);
	vector<Lib> libs(L);
	for (int b = 0; b < B; ++b) {
		scanf("%d", &book_scores[b]);
	}
	for (int l = 0; l < L; ++l) {
		libs[l].read(book_scores);
	}

	/*EVAL SCHEDULE - fast but over-estimate*/
	auto estimate = [&](vector<int> order) {
		int score = 0;
		int day = 0;
		for (int l : order) {
			day += libs[l].signup;
			if (day >= D) {
				break;
			}
			long long reading = (long long)libs[l].perday * (D - day);
			int lastbook = min(1000LL * 1000 * 1000, reading);
			lastbook = min(lastbook, (int)libs[l].books.size());
			score += libs[l].tbs[lastbook - 1];
		}
		return score;
	};

	vector< pair< int, vector<int>> > R; // best schedule
	vector<bool> is_scanned(B), in_standby(L); // status on books scanned and library going into signup
	vector<int> order;
	long long current_score = 0;
	for (int rep = 0; rep < 800000; ++rep) {
		vector<int> new_order = order;
		int type = rand() % 4;
		if (type == 0) {
			// insert
			int i = rand() % L;
			if (in_standby[i]) {
				continue;
			}
			new_order.insert(new_order.begin() + rand() % (1 + new_order.size()), i);
		}
		else if (type == 1) {
			// replace
			int i = rand() % L;
			if (in_standby[i] || order.empty()) {
				continue;
			}
			new_order[rand() % order.size()] = i;
		}
		else if (type == 2) {
			if (order.empty()) {
				continue;
			}
			int i = rand() % order.size();
			int j = rand() % order.size();
			pair<int, pair<int, int>> best;
			best = make_pair(INT_MAX, make_pair(0, 0));
			for (int rep = 0; rep < 3; ++rep) {
				i = rand() % order.size();
				j = rand() % order.size();
				if (i == j) {
					continue;
				}
				best = min(best, { abs(i - j), {i, j} });
			}
			if (best.first == INT_MAX) {
				continue;
			}
			swap(new_order[i], new_order[j]);
		}
		else if (type == 3) {
			if (order.empty()) {
				continue;
			}
			new_order.erase(new_order.begin() + rand() % new_order.size());
		}
		else {
			assert(false);
		}

		long long new_score = estimate(new_order);
		if (new_score > current_score) {
			current_score = new_score;
			order = new_order;
			in_standby = vector<bool>(L, false);
			for (int il : new_order) {
				in_standby[il] = true;
			}
		}
	}
	cerr << estimate(order) << endl;
	int days_left = 0;
	long long total_score = 0;
	while (true) {
		pair<long long, pair<int, vector<int>>> best;
		best.first = 0;
		for (int il : order) {
			if (!in_standby[il]) {
				vector<int> remaining;
				for (int b : libs[il].books) {
					if (!is_scanned[b]) {
						remaining.push_back(b);
					}
				}
				sort(remaining.begin(), remaining.end(), [&](int a, int b) {
					return book_scores[a] > book_scores[b];
					});
				long long reading = max(0, D - days_left - libs[il].signup) * (long long)libs[il].perday;
				if (reading < (long long)remaining.size()) {
					remaining.resize(reading);
				}
				long long gain = 0;
				for (int b : remaining) {
					gain += book_scores[b];
				}
				gain = gain * 1000 * 1000 / pow(libs[il].signup, 1.5);
				if (gain > best.first) {
					best = { (int)gain, {il, remaining} };
				}
				break;
			}
		}
		if (best.first == 0) {
			break;
		}
		int il = best.second.first;
		in_standby[il] = true;
		R.emplace_back(il, best.second.second);
		for (int b : best.second.second) {
			assert(!is_scanned[b]);
			is_scanned[b] = true;
			total_score += book_scores[b];
		}
		days_left += libs[il].signup;
	}

	/*OUTPUT*/
	printf("%d\n", (int)R.size());
	for (pair<int, vector<int>> pp : R) {
		printf("%d %d\n", pp.first, (int)pp.second.size());
		for (int x : pp.second) {
			printf("%d ", x);
		}
		puts("");
	}

	/*OUTPUT*/
	/*int total_score = 0;
	printf("%d\n", (int)R.size());
	for (auto res : R) {
		printf("%d %d\n", res.first, (int)res.second.size());
		for (int b : res.second) {
			printf("%d ", b);
			total_score += B[b]->score;
		}
		printf("\n");
	}*/
	cerr << "score = " << total_score / 1000. / 1000. << endl;
}