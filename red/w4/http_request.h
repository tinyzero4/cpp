#pragma once

#include <string_view>
#include <iostream>

using namespace std;

struct HttpRequest {
    string_view method, uri, protocol;
};


