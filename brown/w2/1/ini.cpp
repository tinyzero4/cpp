#include "ini.h"

#include <string_view>
#include <iostream>

namespace Ini {

    pair<string_view, string_view> Split(string_view line, char by) {
        size_t pos = line.find(by);
        string_view left = line.substr(0, pos);

        if (pos < line.size() && pos + 1 < line.size()) {
            return {left, line.substr(pos + 1)};
        } else {
            return {left, string_view()};
        }
    }

    size_t Document::SectionCount() const {
        return sections.size();
    }

    const Section &Document::GetSection(const string &name) const {
        return sections.at(name);
    }

    Section &Document::AddSection(string name) {
        return sections[name];
    }

    Document Load(istream &input) {
        Document document;
        Section* section;
        for (std::string line; getline(input, line);) {
            if (line.empty()) continue;
            if (line[0] == '[') {
                section = &document.AddSection(line.substr(1, line.length() - 2));
            } else {
                (*section).insert(Split(line, '='));
            }
        }
        return document;
    }

}

