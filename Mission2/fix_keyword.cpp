#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "fix_keyword.h"
using namespace std;

vector<Node> weekBest[7]; //월 ~ 일
vector<Node> twoBest[2]; //평일, 주말
int UZ = 9;

// 레벤슈타인 거리 계산 알고리즘 (문자열 유사도 검사)
int Levenshtein::CalcDist(const std::string& a, const std::string& b) {
	const size_t len_a = a.size();
	const size_t len_b = b.size();

	std::vector<std::vector<int>> d(len_a + 1, std::vector<int>(len_b + 1));

	for (size_t i = 0; i <= len_a; ++i) d[i][0] = i;
	for (size_t j = 0; j <= len_b; ++j) d[0][j] = j;

	for (size_t i = 1; i <= len_a; ++i) {
		for (size_t j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}
// 점수 환산
bool KeywordFixer::similer(const std::string& a, const std::string& b) {
	if (a.empty() || b.empty()) return false;

	int dist = Algo->CalcDist(a, b);
	int max_len = std::max(a.length(), b.length());
	// 유사도 비율 (1.0: 완전히 같음, 0.0: 전혀 다름)
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= 80) return true;
	return false;
}
void KeywordFixer::_ParseInput(string InputWord, string InputDay, int& DayIndex, int& IsWeekend) {
	IsWeekend = 0;
	if (InputDay == "monday") DayIndex = 0;
	else if (InputDay == "tuesday") DayIndex = 1;
	else if (InputDay == "wednesday") DayIndex = 2;
	else if (InputDay == "thursday") DayIndex = 3;
	else if (InputDay == "friday") DayIndex = 4;
	else if (InputDay == "saturday") {
		DayIndex = 5;
		IsWeekend = 1;
	}
	else if (InputDay == "sunday") {
		DayIndex = 6;
		IsWeekend = 1;
	}
}
string KeywordFixer::_FindChalHit(string InputWord, int DayIndex, int IsWeekend) {
	for (Node& node : weekBest[DayIndex]) {
		if (similer(node.name, InputWord)) {
			return node.name;
		}
	}

	for (Node& node : twoBest[IsWeekend]) {
		if (similer(node.name, InputWord)) {
			return node.name;
		}
	}
	return InputWord;
}
bool KeywordFixer::_IsPerfectHit(string InputWord, int DayIndex, int IsWeekend) {
	bool hit = false;
	for (Node& node : weekBest[DayIndex]) {
		if (node.name == InputWord) {
			hit = true;
			Scal.IncreaseScore(node);
			break;
		}
	}

	for (Node& node : twoBest[IsWeekend]) {
		if (node.name == InputWord) {
			hit = true;
			Scal.IncreaseScore(node);
			break;
		}
	}
	Scal.IncreaseUZ();
	return hit;
}
void KeywordFixer::_AddNewWord(string InputWord, int DayIndex, int IsWeekend) {
	weekBest[DayIndex].push_back({ InputWord, UZ });
	std::sort(weekBest[DayIndex].begin(), weekBest[DayIndex].end());
	twoBest[IsWeekend].push_back({ InputWord, UZ });
	std::sort(twoBest[IsWeekend].begin(), twoBest[IsWeekend].end());

	if (weekBest[DayIndex].size() == 11) {
		vector<Node> temp;
		bool bSkip = false;
		for (Node& node : weekBest[DayIndex]) {
			if (bSkip == false) {
				bSkip = true;
			}
			else {
				temp.push_back({ node.name, node.point });
			}
		}
		while (weekBest[DayIndex].size() != 0) {
			weekBest[DayIndex].pop_back();
		}
		for (Node& node : temp) {
			weekBest[DayIndex].push_back({ node.name, node.point });
		}
	}
	if (twoBest[IsWeekend].size() == 11) {
		vector<Node> temp;
		bool bSkip = false;
		for (Node& node : twoBest[IsWeekend]) {
			if (bSkip == false) {
				bSkip = true;
			}
			else {
				temp.push_back({ node.name, node.point });
			}
		}
		while (twoBest[IsWeekend].size() != 0) {
			twoBest[IsWeekend].pop_back();
		}
		for (Node& node : temp) {
			twoBest[IsWeekend].push_back({ node.name, node.point });
		}
	}
}
void KeywordFixer::Reset(void) {
	for (int i = 0; i < 7; i++) {
		while (weekBest[i].size() != 0) {
			weekBest[i].pop_back();
		}
	}
	for (int i = 0; i < 2; i++) {
		while (twoBest[i].size() != 0) {
			twoBest[i].pop_back();
		}
	}
	UZ = 9;
}
void KeywordFixer::SetScoreForTest(int value) {
	Scal.SetScore(value);
}
int KeywordFixer::GetScore(string InputWord, string InputDay) {
	int DayIndex = 0;
	int IsWeekend = 0;
	_ParseInput(InputWord, InputDay, DayIndex, IsWeekend);
	for (Node& node : weekBest[DayIndex]) {
		if (node.name == InputWord) {
			return node.point;
		}
	}
	for (Node& node : twoBest[IsWeekend]) {
		if (node.name == InputWord) {
			return node.point*10000;
		}
	}
	return 0;
}
string KeywordFixer::FindWord(string InputWord, string InputDay) {
	// [refac] too long, delete repeatation
	int DayIndex = 0;
	int IsWeekend = 0;
	_ParseInput(InputWord, InputDay, DayIndex, IsWeekend);
	//완벽확인
	if (_IsPerfectHit(InputWord, DayIndex, IsWeekend)) {
		return InputWord;
	}
	//찰떡확인
	string ChalWord = _FindChalHit(InputWord, DayIndex, IsWeekend);
	if (InputWord != ChalWord) {
		return ChalWord;
	}
	//완벽 HIT / 찰떡 HIT 둘다 아닌경우
	_AddNewWord(InputWord, DayIndex, IsWeekend);

	return InputWord;
}
void ScoreCalculater::SetScore(int value) {
	for (int i = 0; i < 7; i++) {
		for (Node& node : weekBest[i]) {
			node.point = value;
		}
	}
	for (int i = 0; i < 2; i++) {
		int num = 1;
		for (Node& node : twoBest[i]) {
			node.point = value;
		}
	}
}
void ScoreCalculater::_ResetScore(void) {
	//재정렬 작업
	UZ = 9;
	for (int i = 0; i < 7; i++) {
		int num = 1;
		for (Node& node : weekBest[i]) {
			node.point = num;
			num++;
		}
	}
	for (int i = 0; i < 2; i++) {
		int num = 1;
		for (Node& node : twoBest[i]) {
			node.point = num;
			num++;
		}
	}
}
void ScoreCalculater::IncreaseScore(Node& node) {
	bool bResetScore = false;
	long long temp = 0;
	if (min_score > (double)node.point * 0.1) {
		node.point++;
	}
	else {
		temp = node.point + node.point * 0.1;
		if (temp <= max_score) {
			node.point += node.point * 0.1;
		}
	}
	if ((node.point > max_score) || (temp > max_score)) {
		_ResetScore();
	}
}
void ScoreCalculater::IncreaseUZ(void) {
	if (++UZ >= max_score) _ResetScore();
}
#ifndef UNIT_TEST
int main() {
	ifstream fin{ "keyword_weekday_500.txt" }; //500개 데이터 입력
	Levenshtein LevenAlgo;
	KeywordFixer F(&LevenAlgo);
	for (int i = 0; i < 500; i++) {
		string InputWord, InputDay;	// [refac] t1, t2 naimg
		fin >> InputWord >> InputDay; // [refac] file access fail case
		string ret = F.FindWord(InputWord, InputDay);
		std::cout << ret << "\n";
	}
}
#endif
