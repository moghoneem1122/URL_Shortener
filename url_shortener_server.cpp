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


class Storage {
private:
    std::string file = "urls.txt";
    std::unordered_map<std::string, std::string> s2l;
    std::unordered_map<std::string, std::string> l2s;

public:
    Storage() { load(); }

    void load() {
        std::ifstream f(file);
        std::string line;

        while (std::getline(f, line)) {
            size_t pos = line.find('|');
            if (pos == std::string::npos) continue;

            std::string shortUrl = line.substr(0, pos);
            std::string longUrl = line.substr(pos + 1);

            s2l[shortUrl] = longUrl;
            l2s[longUrl] = shortUrl;
        }
    }

    void save(const std::string& s, const std::string& l) {
        s2l[s] = l;
        l2s[l] = s;

        std::ofstream f(file, std::ios::app);
        f << s << "|" << l << "\n";
    }

    bool hasShort(const std::string& s) { return s2l.count(s); }
    bool hasLong(const std::string& l) { return l2s.count(l); }

    std::string getShort(const std::string& l) { return l2s[l]; }

    std::string getLong(const std::string& s) {
        return hasShort(s) ? s2l[s] : "";
    }

  
    std::vector<std::pair<std::string, std::string>> getAll() {
        std::vector<std::pair<std::string, std::string>> data;
        for (auto& it : s2l) {
            data.push_back(it);
        }
        return data;
    }
};
 
struct Result {
    std::string message;
    std::string short_url;
    bool duplicate;
};



class Service {
private:
    Storage storage;
    Generator gen;

public:
    Result shorten(std::string url) {
        url = normalize(url);

        if (!Validator::isValidUrl(url)) {
            return {"Invalid URL format", "", false};
        }

        if (storage.hasLong(url)) {
            return {
                "Duplicate URL detected (already exists)",
                storage.getShort(url),
                true
            };
        }

        std::string shortUrl = gen.generate(url);
        storage.save(shortUrl, url);

        return {
            "Short URL created successfully",
            shortUrl,
            false
        };
    }

    std::string resolve(const std::string& shortUrl) {
        return storage.getLong(shortUrl);
    }

    
    std::vector<std::pair<std::string, std::string>> getAllUrls() {
        return storage.getAll();
    }
};


int main() {
    Service service;

    while (true) {
        std::cout << "\n===========================\n";
        std::cout << " URL SHORTENER CLI SYSTEM\n";
        std::cout << "===========================\n";
        std::cout << "1. Shorten URL\n";
        std::cout << "2. Open Short URL\n";
        std::cout << "3. Show all URLs\n";
        std::cout << "4. Exit\n";
        std::cout << "Choose: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::string url;
            std::cout << "Enter long URL: ";
            std::getline(std::cin, url);

            Result res = service.shorten(url);

            std::cout << "\n--- RESULT ---\n";
            std::cout << res.message << "\n";

            if (!res.short_url.empty()) {
                std::cout << "Short URL: " << res.short_url << "\n";
            }
        }

        else if (choice == 2) {
            std::string shortUrl;
            std::cout << "Enter short URL (/gh/id): ";
            std::getline(std::cin, shortUrl);

            std::string longUrl = service.resolve(shortUrl);

            std::cout << "\n--- RESULT ---\n";

            if (longUrl.empty()) {
                std::cout << "URL not found\n";
            } else {
                std::cout << "Original URL: " << longUrl << "\n";
                std::cout << "Opening in browser...\n";
                openInBrowser(longUrl);
            }
        }

        else if (choice == 3) {
            auto all = service.getAllUrls();

            std::cout << "\n--- ALL SHORTENED URLS ---\n";

            if (all.empty()) {
                std::cout << "No URLs stored yet\n";
            } else {
                for (auto& p : all) {
                    std::cout << p.first << "  --->  " << p.second << "\n";
                }
            }
        }

        else if (choice == 4) {
            std::cout << "Bye 👋\n";
            break;
        }

        else {
            std::cout << "Invalid choice\n";
        }
    }

    return 0;
}
