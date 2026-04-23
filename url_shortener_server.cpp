#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <vector>
#include <cstdlib>

// ===============================
// Utility
// ===============================

std::string normalize(const std::string& url) {
    if (!url.empty() && url.back() == '/')
        return url.substr(0, url.size() - 1);
    return url;
}

// ===============================
// Open in browser
// ===============================

void openInBrowser(const std::string& url) {
#ifdef _WIN32
    std::string cmd = "start " + url;
#elif __APPLE__
    std::string cmd = "open " + url;
#else
    std::string cmd = "xdg-open " + url;
#endif
    system(cmd.c_str());
}
