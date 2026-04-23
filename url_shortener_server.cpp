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


// ===============================
// Validator
// ===============================

class Validator {
public:
    static bool isValidUrl(const std::string& url) {
        std::regex pattern("^(http://|https://)[^\\s]+$");
        return std::regex_match(url, pattern);
    }
};




class Generator {
private:
    std::unordered_map<std::string, std::string> known = {
        {"github.com", "gh"},
        {"youtube.com", "yt"},
        {"docs.google.com", "docs"},
        {"meet.google.com", "meet"},
        {"calendar.google.com", "gcal"}
    };

    std::string getDomain(std::string url) {
        size_t pos = url.find("://");
        if (pos != std::string::npos)
            url = url.substr(pos + 3);

        if (url.find("www.") == 0)
            url = url.substr(4);

        size_t end = url.find('/');
        return url.substr(0, end);
    }

    std::string getAlias(const std::string& domain) {
        if (known.count(domain))
            return known[domain];

        std::string alias;
        for (char c : domain) {
            if (c == '.') break;
            if (!strchr("aeiou", tolower(c)))
                alias += c;
            if (alias.size() >= 4) break;
        }
        return alias.empty() ? "lnk" : alias;
    }

    std::string getKey(const std::string& url) {
        size_t pos = url.find("v=");
        if (pos != std::string::npos)
            return url.substr(pos + 2, 6);

        size_t last = url.find_last_of('/');
        if (last != std::string::npos && last < url.size() - 1) {
            std::string seg = url.substr(last + 1);
            seg.erase(std::remove_if(seg.begin(), seg.end(),
                [](char c) { return !isalnum(c); }),
                seg.end());
            return seg.substr(0, 6);
        }
        return "id";
    }

public:
    std::string generate(const std::string& url) {
        std::string domain = getDomain(url);
        std::string alias = getAlias(domain);
        std::string key = getKey(url);

        return "/" + alias + "/" + key;
    }
};

