---
apply: by file patterns
patterns: .md
---

# AI Rules v2: Generating a Professional README

This document defines strict rules for AI when generating a **professional open-source README**.

The goal is to ensure the README is:

* accurate
* honest
* technically useful
* structured for developers

---

# Core Philosophy

AI must follow these core principles.

## Honesty First

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

## Be Transparent About Limitations

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

## Developer-First Documentation

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

# Mandatory README Structure

AI must generate the README using this structure.

```
Project Title
Short Description

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

# Title and Summary

The README must begin with:

```
# ProjectName
```

Then provide a description.

Example:

```
A lightweight C++20 timer manager based on chrono.
```

Rules:

* 1–2 sentences
* factual
* concise

---

# Structure

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

# Markdown Formatting Rules

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

# Required Engineering Sections

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

# Language Style Rules

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

# Ideal README Length

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

# Output Format

AI must output:

```
pure Markdown
```

No explanation outside the README.

