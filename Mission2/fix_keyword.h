#pragma once
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct Node {
	string name;
	int point;

	bool operator<(const Node& other) const {
		return point < other.point;
	}
};
// 레벤슈타인 거리 계산 알고리즘 (문자열 유사도 검사)
struct DistancceAlgorithm {
	virtual int CalcDist(const std::string& a, const std::string& b) = 0;
};
struct Levenshtein : public DistancceAlgorithm {
	int CalcDist(const std::string& a, const std::string& b);
};
struct ScoreCalculater {
public:
	void IncreaseScore(Node& node);
	void IncreaseUZ(void);
private:
	void _ResetScore(void);
	int min_score = 1;
	int max_score = 2100000000;
};
struct KeywordFixer {
public:
	KeywordFixer(DistancceAlgorithm* p) {
		Algo = p;
	}
	int GetScore(string InputWord, string InputDay);
	void Reset(void);
	bool similer(const std::string& a, const std::string& b);
	string FindWord(string InputWord, string InputDay);
	void input();
private:
	ScoreCalculater Scal;
	DistancceAlgorithm* Algo;
	bool _ParseInput(string InputWord, string InputDay, int& DayIndex, int& IsWeekend);
	string _FindChalHit(string InputWord, int DayIndex, int IsWeekend);
	bool _IsPerfectHit(string InputWord, int DayIndex, int IsWeekend);
	void _AddNewWord(string InputWord, int DayIndex, int IsWeekend);
};

