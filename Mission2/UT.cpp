#include <gmock/gmock.h>
#include "fix_keyword.h"
using namespace testing;
#ifdef UNIT_TEST
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#endif
class FixWordTestFixture : public Test {
public:
	Levenshtein LevenAlgo;
};

TEST_F(FixWordTestFixture, AddNewWord) {
	KeywordFixer F(&LevenAlgo);
	string a = "hello";
	string b = "saturday";
	
	EXPECT_EQ("hello", F.FindWord(a, b));
}
TEST_F(FixWordTestFixture, AddMoreTenWord) {
	KeywordFixer F(&LevenAlgo);
	F.Reset();
	string b = "saturday";
	F.FindWord("hello", b);
	F.FindWord("bread", b);
	F.FindWord("lemon", b);
	F.FindWord("apple", b);
	F.FindWord("book", b);
	F.FindWord("box", b);
	F.FindWord("phone", b);
	F.FindWord("laptop", b);
	F.FindWord("earphone", b);
	F.FindWord("mouse", b);
	F.FindWord("keyboard", b);
	EXPECT_EQ(0, F.GetScore("hello", b));
	EXPECT_EQ(20, F.GetScore("keyboard", b));
}
TEST_F(FixWordTestFixture, SimilerWord) {
	KeywordFixer F(&LevenAlgo);
	F.Reset();
	string b = "saturday";
	F.FindWord("helloworld", b);
	EXPECT_EQ("helloworld", F.FindWord("helloworl", b));
}
TEST_F(FixWordTestFixture, NotSimilerWord) {
	KeywordFixer F(&LevenAlgo);
	F.Reset();
	string b = "sunday";
	F.FindWord("helloworld", b);
	EXPECT_NE("helloworld", F.FindWord("ehllowordl", b));
}
TEST_F(FixWordTestFixture, OverMaxScore) {
	KeywordFixer F(&LevenAlgo);
	F.Reset();
	string b = "sunday";
	F.FindWord("hello", b);
	F.FindWord("bread", b);
	F.FindWord("lemon", b);
	F.FindWord("lemon", b);
	F.FindWord("lemon", b);
	F.FindWord("lemon", b);
	F.SetScoreForTest(2100000000);
	F.FindWord("lemon", b);
	EXPECT_EQ(3, F.GetScore("lemon", b));
}
TEST_F(FixWordTestFixture, ChalHitCase) {
	KeywordFixer F(&LevenAlgo);
	F.Reset();
	string b = "sunday";
	F.FindWord("hello", b);
	F.FindWord("bread", b);
	F.FindWord("lemon", b);
	F.FindWord("apple", b);
	F.FindWord("book", b);
	F.FindWord("box", b);
	F.FindWord("phone", b);
	F.FindWord("laptop", b);
	F.FindWord("earphone", b);
	F.FindWord("mouse", b);
	F.FindWord("keyboard", b);
	F.FindWord("helloworld", b);
	
	EXPECT_EQ("helloworld", F.FindWord("helloworl", "saturday"));
	EXPECT_EQ(21*10000, F.GetScore("helloworld", "saturday"));
}