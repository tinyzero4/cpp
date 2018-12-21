#include <string>
#include <list>

#include "test_runner.h"

using namespace std;

class Editor {

public:
    Editor() {
        cursor_position = text.begin();
    }

    void Left() {
        if (cursor_position != text.begin()) --cursor_position;
    }

    void Right() {
        if (cursor_position != text.end()) ++cursor_position;
    }

    void Insert(char token) {
        text.insert(cursor_position, token);
    }

    void Cut(size_t tokens = 1) {
        if (noContentToCopy(tokens)) return;
        Copy(tokens);
        auto dist_to_end = (size_t) distance(cursor_position, text.end());
        const auto &last = next(cursor_position, min(dist_to_end, tokens));
        text.erase(cursor_position, last);
        cursor_position = last;
    }

    void Copy(size_t tokens = 1) {
        if (noContentToCopy(tokens)) return;
        auto dist_to_end = (size_t) distance(cursor_position, text.end());
        buffer = list<char>(cursor_position, next(cursor_position, min(dist_to_end, tokens)));
    }

    void Paste() {
        if (buffer.empty()) return;
        cursor_position = text.insert(cursor_position, buffer.begin(), buffer.end());
        cursor_position = next(cursor_position, buffer.size());
    }

    string GetText() const {
        return string(text.begin(), text.end());
    }

private:
    list<char> text;
    list<char> buffer;
    list<char>::iterator cursor_position;

    bool noContentToCopy(size_t tokens) {
        return tokens == 0 || cursor_position == text.end();
    }

};

void TypeText(Editor &editor, const string &text) {
    for (char c : text) {
        editor.Insert(c);
    }
}

void TestEditing() {
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for (size_t i = 0; i < text_len; ++i) {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for (size_t i = 0; i < text_len - first_part_len; ++i) {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
}

void TestReverse() {
    Editor editor;

    const string text = "esreveR";
    for (char c : text) {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
    Editor editor;
    ASSERT_EQUAL(editor.GetText(), "");

    editor.Left();
    editor.Left();
    editor.Right();
    editor.Right();
    editor.Copy(0);
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
    return 0;
}