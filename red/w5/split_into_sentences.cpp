#include "test_runner.h"

#include <vector>
#include <utility>
#include <iostream>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template<typename Token>
using Sentence = vector<Token>;

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template<typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    vector<vector<Token>> sentences;
    vector<Token> sentence;

    sentence.push_back(move(tokens[0]));
    for (int i = 1; i < tokens.size(); i++) {
        Token token = move(tokens[i]);
        if (!sentence.empty() && sentence.back().IsEndSentencePunctuation() && !token.IsEndSentencePunctuation()) {
            sentences.push_back(move(sentence));
            sentence.clear();
        }
        sentence.push_back(move(token));
    }

    if (!sentence.empty()) sentences.push_back(move(sentence));
    return sentences;
}


struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }

    bool operator==(const TestToken &other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream &operator<<(ostream &stream, const TestToken &token) {
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"},
                                                  {"into"},
                                                  {"sentences"},
                                                  {"!"}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"}, {"into"}, {"sentences"}, {"!"}}
                                        })
    );

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"},
                                                  {"into"},
                                                  {"sentences"},
                                                  {"!", true}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
                                        })
    );

    ASSERT_EQUAL(
            SplitIntoSentences(vector<TestToken>({{"Split"},
                                                  {"into"},
                                                  {"sentences"},
                                                  {"!", true},
                                                  {"!", true},
                                                  {"Without"},
                                                  {"copies"},
                                                  {".", true}})),
            vector<Sentence<TestToken>>({
                                                {{"Split"},   {"into"},   {"sentences"}, {"!", true}, {"!", true}},
                                                {{"Without"}, {"copies"}, {".", true}},
                                        })
    );
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
    return 0;
}