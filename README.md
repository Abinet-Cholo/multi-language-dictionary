# 📖 Multi-language Dictionary “Translator”

A **C++-based multi-language dictionary** implementing a custom hash table for efficient storage, retrieval, and management of translations across English, German, French, and Spanish. Built as part of *Data Structures (CS-UH 1050)*, the project demonstrates mastery of **hashing, collision handling, memory management, and efficient search**.

---

## ✨ Features

- **Custom Hash Table Implementation**:  
  - Designed from scratch without STL hash-map.  
  - Supports insertion, deletion (via lazy deletion), search, and export.  
  - Handles collisions using **linear and quadratic probing**.  

- **Multiple Hash Functions**:  
  - Polynomial rolling, cyclic shift, division/MAD methods.  
  - Benchmarked **3+ functions** on 800K+ word datasets to minimize collisions.  

- **Multi-language Support**:  
  - English → German (~820K words)  
  - English → French (~110K words)  
  - English → Spanish (~31K words)  

- **Lazy Deletion**: Efficiently marks entries as deleted without immediate restructuring, improving performance.  

- **Memory Management**: Destructor cleans up dynamically allocated memory to prevent leaks.  

---

## 🛠 Tech Stack

- **Language**: C++  
- **Data Structures**: Custom Hash Table, Vector  
- **Build**: Makefile (portable to Linux/macOS/Unix)  

---

## 📂 Project Structure

```
translator/
 ├── hashtable.cpp    # HashTable implementation
 ├── hashtable.h      # HashTable class definition
 ├── main.cpp         # Program entrypoint
 ├── vector.h         # Optional custom vector implementation
 ├── makefile         # Build configuration
 └── report.pdf       # Hash function evaluation & results
```

---

## 🚀 Getting Started

### 1. Compile
```bash
make
```

### 2. Run
```bash
./translator
```

---

## 💻 Usage

Supported commands in the interactive shell:

- `import <file>` → Import translations from a dataset file (e.g., `en-de.txt`)  
- `add <word>:<translation>` → Add a new word/translation  
- `find <word>` → Look up all translations of a word (shows comparisons made)  
- `delWord <word>` → Delete a word and all translations (lazy deletion)  
- `delTranslation <word> <language>` → Remove a translation in one language  
- `delMeaning <word> <meaning> <language>` → Remove a specific meaning  
- `export <language> <file>` → Export dictionary for a specific language  

Example:
```
> import en-fr.txt
[INFO] Imported 110,322 entries (collisions: 2.1 avg)
> find house
house (FR): maison, habitation
Comparisons: 3
> delMeaning house habitation fr
[INFO] Deleted meaning: habitation
```

---

## ✅ Hash Function Evaluation

- Polynomial Rolling → Avg collisions: 2.1  
- Cyclic Shift → Avg collisions: 3.4  
- MAD (Multiply-Add-Divide) → Avg collisions: 2.6  

📊 Default function chosen: **Polynomial Rolling Hash** (lowest collisions overall).  

---

## 🔮 Future Improvements

- Add GUI for easier interaction.  
- Extend support to more languages.  
- Store translations in external database (e.g., SQLite).  
- Implement parallelism for faster imports on large datasets.  

---

This project is a **scalable, efficient multi-language dictionary** built with C++, demonstrating advanced data structure design and performance optimization.
