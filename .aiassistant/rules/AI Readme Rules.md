---
apply: by file patterns
patterns: .md
---

# AI Rules v2: Generating a Professional Bilingual README (Chinese + English)

This document defines strict rules for AI when generating a **professional open-source README**.

The goal is to ensure the README is:

* accurate
* honest
* technically useful
* bilingual
* structured for developers

---

# 1. Core Philosophy

AI must follow these core principles.

## 1.1 Honesty First

The README must **not exaggerate the project**.

AI must avoid:

* marketing language
* exaggerated claims
* unrealistic performance claims

Examples of **forbidden language**:

❌ “industry-leading performance”
❌ “revolutionary system”
❌ “the best solution ever”

Allowed style:

✔ factual
✔ measurable
✔ modest

Example:

Bad:

```
This library provides an extremely powerful timer system.
```

Good:

```
This library provides a simple timer manager built on top of C++ chrono.
```

---

## 1.2 Be Transparent About Limitations

The README **must describe the project's limitations**.

AI must include a section describing:

* known limitations
* unfinished features
* design tradeoffs
* potential problems

Example:

```
## Limitations

- Timer IDs are currently allocated sequentially.
- The implementation has not been tested on Windows.
- The API may change in future versions.
```

Transparency increases **developer trust**.

---

## 1.3 Developer-First Documentation

The README must prioritize:

* usability
* installation
* examples
* architecture explanation

Avoid long storytelling.

Focus on:

```
What it is
How to use it
How it works
What problems it has
```

---

# 2. Bilingual Documentation Rules

The README must contain **Chinese + English**.

Rules:

1. Chinese section must appear **first**
2. English section must appear **after**
3. The two sections must contain **equivalent content**

Example structure:

```
Chinese Documentation
---------------------

English Documentation
```

Do **not mix languages in the same paragraph**.

---

# 3. Mandatory README Structure

AI must generate the README using this structure.

```
Project Title
Short Description

Chinese Documentation
    项目介绍
    功能特性
    项目结构
    安装
    使用示例
    设计说明
    已知问题
    开发说明
    贡献指南
    许可证

English Documentation
    Introduction
    Features
    Project Structure
    Installation
    Usage
    Design
    Limitations
    Development
    Contributing
    License
```

---

# 4. Title and Summary

The README must begin with:

```
# ProjectName
```

Then provide a **bilingual description**.

Example:

```
A lightweight C++20 timer manager based on chrono.

一个基于 C++20 chrono 实现的轻量级计时器管理器。
```

Rules:

* 1–2 sentences
* factual
* concise

---

# 5. Chinese Section

Start with:

```
---

# 中文文档
```

---

## 项目介绍

Explain:

* 项目目标
* 解决的问题
* 基本设计

Length:

3–6 sentences.

Avoid exaggerated claims.

---

## 功能特性

Use bullet points.

Example:

```
- 基于 C++20 chrono
- 支持多个计时器
- 线程安全
- 简单 API
```

Rules:

* 3–8 items
* concise

---

## 项目结构

Provide a directory tree.

Example:

```
project
├── include/
│   └── chronometer_manager.h
├── src/
│   └── chronometer_manager.cpp
├── examples/
│   └── example.cpp
└── README.md
```

---

## 安装

Prefer **CMake**.

Example:

```bash
git clone https://github.com/example/project.git
cd project
cmake -B build
cmake --build build
```

---

## 使用示例

Provide a minimal example.

Example:

```cpp
auto id = ChronometerManager::Instance().Start();

DoWork();

auto duration =
    ChronometerManager::Instance().Stop(id);
```

Example must be:

* short
* realistic
* compilable

---

## 设计说明

Explain major design choices.

Example topics:

* 为什么使用 chrono
* 为什么使用单例
* 线程安全策略

Use bullet points.

---

## 已知问题

This section is **mandatory**.

Example:

```
## 已知问题

- 当前实现没有对象池，计时器创建成本较高
- Windows 平台尚未测试
- API 仍可能调整
```

AI must list **realistic limitations**.

---

## 开发说明

Describe development requirements.

Example:

```
- C++20
- GCC 13+
- CMake 3.20+
```

---

## 贡献指南

Example:

```
1 Fork repository
2 Create feature branch
3 Submit pull request
```

---

## 许可证

Example:

```
MIT License
```

---

# 6. English Section

Start with:

```
---

# English Documentation
```

Sections must match the Chinese structure.

```
Introduction
Features
Project Structure
Installation
Usage
Design
Limitations
Development
Contributing
License
```

English should be **natural technical English**, not literal translation.

---

# 7. Markdown Formatting Rules

AI must follow these rules.

## Headings

```
# Title
## Section
### Subsection
```

Do not skip heading levels.

---

## Code Blocks

Always specify language.

Example:

````
```cpp
````

or

````
```bash
````

---

## Lists

Use `-`.

Example:

```
- Feature A
- Feature B
```

---

## Directory Trees

Use plain text trees.

Example:

```
project
├── include
├── src
└── examples
```

---

# 8. Required Engineering Sections

AI must ensure the README contains:

```
Installation
Usage Example
Project Structure
Limitations
Development Requirements
```

These sections are **not optional**.

---

# 9. Language Style Rules

AI must write in:

```
clear
neutral
technical
precise
```

Avoid:

```
marketing tone
exaggeration
fluffy descriptions
```

Prefer:

```
short sentences
bullet lists
code examples
```

---

# 10. Ideal README Length

Recommended size:

Small libraries:

```
200 – 400 lines
```

Medium projects:

```
400 – 800 lines
```

---

# 11. Output Format

AI must output:

```
pure Markdown
```

No explanation outside the README.

