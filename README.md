<div align="center">

# 🏛️ Nagarsevak Connect

### Pune Municipal Corporation — Smart Civic Issue Management Platform

*A comprehensive C++ terminal application demonstrating advanced Object-Oriented Programming principles*

---

[![Language](https://img.shields.io/badge/Language-C++17-blue?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![Paradigm](https://img.shields.io/badge/Paradigm-OOP-green?style=for-the-badge)](https://en.wikipedia.org/wiki/Object-oriented_programming)
[![Platform](https://img.shields.io/badge/Platform-Terminal-black?style=for-the-badge)](https://en.wikipedia.org/wiki/Command-line_interface)
[![License](https://img.shields.io/badge/License-Academic-orange?style=for-the-badge)](#)

</div>

---

## 📌 Overview

**Nagarsevak Connect** is a menu-driven, terminal-based civic issue management system modeled after the Pune Municipal Corporation's public grievance portal. Citizens can report infrastructure problems — potholes, water leaks, electricity failures, garbage overflow — across 41 municipal wards. Each complaint is tracked, persisted to disk, and ranked by community upvotes to intelligently prioritize civic action.

The project is purpose-built to be a **complete, living demonstration of the C++ OOP syllabus**, with every major language feature organically woven into the application logic rather than presented as isolated textbook examples.

---

## 🎯 Core Objectives

| Goal | Description |
|---|---|
| **Civic Utility** | Simulate a real-world complaint management workflow for urban governance |
| **OOP Showcase** | Demonstrate all 10 prescribed OOP topics within a single cohesive codebase |
| **Data Persistence** | Maintain complaint records across sessions via file I/O |
| **Robust Error Handling** | Gracefully manage invalid input, file failures, and edge cases |

---

## 🏗️ Architecture & OOP Design

The entire application is built on a clean inheritance hierarchy with well-defined responsibilities at each layer.

```
std::exception  ←─────────────────────── C++ Standard Library
      │
      └── ServerException          (Base custom exception + timestamp)
              ├── FileException    (File I/O error wrapping)
              └── InputException   (User input validation error)

ISerializable   (Pure Abstract Interface — serialize to string)
ILoggable       (Pure Abstract Interface — log creation event)
      │
      └── Report                   (Abstract base — encapsulates core complaint data)
              │                    + Static counters (totalReports, lifetimeReports)
              │
              └── CivicIssue       (Single Inheritance — adds ward, category, upvotes)
                      │            + Operator Overloading (++, <, ==, <<)
                      │            + Friend Functions (printIssueUrgency)
                      │
                      └── UrgentIssue   (Multilevel Inheritance — emergency escalation)

IssueManager<T>  (Class Template — generic container with dynamic memory management)
```

---

## 📚 OOP Concepts Demonstrated

| # | Concept | Implementation |
|---|---|---|
| 1 | **Classes, Objects, Encapsulation** | `Report`, `CivicIssue`, `UrgentIssue` with `private`/`protected`/`public` access |
| 2 | **Static Members** | `Report::totalReports`, `Report::lifetimeReports` — live object counters |
| 3 | **Constructors & Destructors** | Default, parameterized, copy constructors; virtual destructors for safe polymorphic cleanup |
| 4 | **Inheritance** | Single (`Report → CivicIssue`), Multilevel (`→ UrgentIssue`), Multiple (`Report` implements `ISerializable + ILoggable`) |
| 5 | **Polymorphism** | `virtual display()` overridden at each level; runtime dispatch via base-class pointer array |
| 6 | **Abstraction** | Pure virtual interfaces `ISerializable` and `ILoggable`; abstract `Report::display() = 0` |
| 7 | **Operator Overloading** | Unary `++` (upvote), Binary `<` (priority compare), `==` (ID equality), stream `<<` |
| 8 | **Friend Functions** | `operator<<` and `printIssueUrgency()` access protected members without membership |
| 9 | **Templates** | `IssueManager<T>` class template; `printHeader<T>` function template |
| 10 | **Exception Handling** | Custom exception hierarchy; `try/catch/throw` around all I/O and user input paths |
| 11 | **Dynamic Memory** | `new`/`delete` for heap-allocated issues; dynamic array resizing in `IssueManager` |
| 12 | **Pointers & `this`** | `T**` pointer-to-pointer arrays; `this->` used explicitly in constructors and operators |
| 13 | **File I/O** | Pipe-delimited serialization to `issues.txt`; full load/save lifecycle |
| 14 | **Inline & Default Args** | `inline` getters/setters; `loc = "None"` default parameter in `CivicIssue` constructor |

---

## 🚀 Getting Started

### Prerequisites

- **C++17** compatible compiler (`g++`, `clang++`, or MSVC)
- No external libraries or build tools required

### Compile & Run

```bash
# Compile
g++ -std=c++17 -o nagarsevak backend.cpp

# Run
./nagarsevak        # Linux / macOS
nagarsevak.exe      # Windows
```

> A pre-built `nagarsevak.exe` is included for Windows users who want to run immediately without compiling.

---

## 🖥️ Application Menu

```
=========================================
   WELCOME TO NAGARSEVAK CONNECT
=========================================

-----------------------------------------
   NAGARSEVAK CONNECT - MAIN MENU
-----------------------------------------
  1. Report New Issue
  2. View All Issues
  3. View Issues with Priority
  4. Upvote an Issue
  5. Search Issue by ID
  6. Clear All Issues
  7. Demo: Polymorphism
  8. Demo: Copy Constructor
  9. Show OOP Stats
  0. Exit
-----------------------------------------
```

| Option | Feature | Key OOP Demonstrated |
|---|---|---|
| `1` | Report a new civic complaint | Dynamic `new`, copy constructor, file I/O |
| `2` | View all complaints | Virtual `display()`, polymorphic dispatch |
| `3` | View with priority analysis | Friend function `printIssueUrgency()` |
| `4` | Upvote a complaint | Unary `operator++`, stream `operator<<` |
| `5` | Search by Issue ID | Pointer traversal, `dynamic_cast` |
| `6` | Clear all issues | Destructor chain, static counter verification |
| `7` | Polymorphism live demo | Base-pointer array, `UrgentIssue`, `dynamic_cast`, `operator<` |
| `8` | Copy constructor demo | Deep copy semantics, live object counter |
| `9` | OOP statistics | `static` member access via scope resolution `::` |

---

## 📁 Project Structure

```
nagarsevak_connect/
├── backend.cpp          # Complete C++ source — all OOP logic
├── nagarsevak.exe       # Pre-built Windows executable
├── issues.txt           # Persistent complaint storage (auto-created)
├── index.html           # Frontend UI (web view, supplementary)
├── style.css            # Frontend styling
├── package.json         # Node.js config (frontend dev server)
├── vite.config.ts       # Vite build configuration
├── tsconfig.json        # TypeScript configuration
├── .env.example         # Environment variable template
└── README.md            # This file
```

---

## 🗺️ Ward Coverage

The system covers all **41 wards** of the Pune Municipal Corporation, from **Vishrambaug (Ward 1)** to **Sangvi (Ward 41)**, including major localities such as Shivajinagar, Kothrud, Hadapsar, Aundh, Baner, Hinjewadi, and Wakad.

---

## 🔬 Key Design Decisions

- **Heap-allocated complaint objects** — `IssueManager<T>` holds `T**` (array of pointers), enabling polymorphic storage of both `CivicIssue` and `UrgentIssue` objects through a single container.
- **Pipe-delimited serialization** — issues are serialized as `id|title|desc|ward|category|votes|location` for simple, human-readable flat-file persistence.
- **Exception hierarchy mirroring the domain** — `FileException` and `InputException` both inherit `ServerException`, which itself inherits `std::exception`, showcasing hierarchical exception design.
- **Live object tracking** — `Report::totalReports` decrements on destructor, providing real-time visibility into memory lifecycle during demos.

---

## 👥 Team

| Name | Role |
|---|---|
| **Samarth Khadse** | Lead Developer |
| **Shrikant Kole** | Contributor |

---

<div align="center">

*Nagarsevak Connect — Bridging citizens and civic governance through technology*

**Pune Municipal Corporation · C++ OOP Academic Project · SY 2025–26**

</div>
