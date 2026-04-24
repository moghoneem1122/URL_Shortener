



#  URL Shortener CLI System (C++)

## Overview

This project is a lightweight **URL Shortener Service** implemented in C++ using only the Standard Library. It provides a simple Command-Line Interface (CLI) that allows users to shorten long URLs, store them persistently, and retrieve or open them later.

The system focuses on **clean architecture, simplicity, and portability**, making it suitable for learning backend logic and system design fundamentals.

---

##  Features

### 🔗 URL Shortening

* Converts long URLs into short, readable links
* Format:

  ```
  /alias/key
  ```
* Example:

  ```
  https://github.com/user/repo → /gh/repo
  ```

---

###  Smart URL Generation

* Extracts domain name automatically
* Uses predefined aliases for known domains:

  * `github.com` → `gh`
  * `youtube.com` → `yt`
  * `docs.google.com` → `docs`
  * `meet.google.com` → `meet`
  * `calendar.google.com` → `gcal`
* Generates fallback aliases for unknown domains

---

###  Duplicate Detection

* Prevents storing duplicate URLs
* Returns the existing short URL if the same long URL is entered

---

###  Persistent Storage

* Stores all mappings in a local file:

  ```
  urls.txt
  ```
* Automatically loads saved data on startup

---

###  URL Resolution

* Converts short URLs back to original URLs
* Allows opening the URL directly in the browser

---

###  Cross-Platform Browser Support

Automatically detects OS:

* Windows → `start`
* macOS → `open`
* Linux → `xdg-open`

---

###  CLI Interface

Interactive menu:

```
1. Shorten URL
2. Open Short URL
3. Show all URLs
4. Exit
```

---

##  Project Structure

```
.
├── url_shortener_server.cpp   # Main source file
├── urls.txt                   # Data storage (auto-created)
└── README.md                  # Documentation
```

---

##  System Design

### 🔹 Validator

* Uses regex to validate URL format
* Accepts only `http://` and `https://` URLs

---

### 🔹 Generator

* Extracts:

  * Domain
  * Alias
  * Unique key
* Produces short URL in `/alias/key` format

---

### 🔹 Storage

* Handles file operations
* Maintains:

  * Short → Long mapping
  * Long → Short mapping
* Ensures persistence using `urls.txt`

---

### 🔹 Service

* Core logic layer
* Handles:

  * URL shortening
  * Duplicate detection
  * URL resolution

---

### 🔹 CLI (Main Function)

* Handles user interaction
* Displays menu and processes input

---

## ⚙️ Build & Run

###  Requirements

* C++ compiler supporting C++11 (e.g., `g++`)

---

### 🔨 Compile

```bash
g++ -std=c++11 url_shortener_server.cpp -o shortener_server
```

---

###  Run

```bash
./shortener_server
```

---

##  Example Usage

### ➤ Shorten URL

```
Enter long URL: https://github.com/user/project

--- RESULT ---
Short URL created successfully
Short URL: /gh/project
```

---

### ➤ Open Short URL

```
Enter short URL (/gh/id): /gh/project

--- RESULT ---
Original URL: https://github.com/user/project
Opening in browser...
```

---

### ➤ Show All URLs

```
--- ALL SHORTENED URLS ---
/gh/project  --->  https://github.com/user/project
```

---

## 📁 Data Storage Format

Stored in `urls.txt`:

```
/short|long_url
```

Example:

```
/gh/project|https://github.com/user/project
```

---

##  Limitations

* CLI-based (no web interface)
* No custom short URLs
* No collision handling for generated keys
* Basic URL validation only

---

##  Future Improvements

* Add HTTP server (REST API)
* Support custom aliases
* Implement collision handling
* Add analytics (click tracking)
* Improve validation and parsing
* Add unit tests

---

##  Design Goals

* Simplicity
* Maintainability
* Readability
* Zero external dependencies
* Cross-platform compatibility

---

##  Author Notes

This project demonstrates a **minimal backend system** for URL shortening, focusing on:

* Core logic implementation
* Data structures (maps, vectors)
* File persistence
* Clean modular design

It is suitable for:

* Learning C++ system design
* Backend fundamentals
* Academic or internship projects




