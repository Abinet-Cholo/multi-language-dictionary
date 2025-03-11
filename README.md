# Multi-language Dictionary ("Translator") 📚🌍

This repository contains the solution for **Assignment 3: Multi-language Dictionary** from the **Data Structures (CS-UH 1050)** course, Spring 2024 at NYU Abu Dhabi. The program implements a multi-language dictionary using hash tables, providing efficient storage and retrieval of word translations in various languages.

## Project Overview 🌐
The dictionary program allows users to:
- Import word translations from files 📂.
- Add, search, and delete words and translations 🔍❌.
- Handle collisions using linear or quadratic probing 🔧.
- Implement lazy deletion for efficient memory management 🧹.

## Files 📄
- **hashtable.cpp**: Implementation of the `HashTable` class.
- **hashtable.h**: Header file for the `HashTable` class.
- **main.cpp**: Contains the main program logic and user interface.
- **vector.h**: Custom vector implementation (if applicable).
- **makefile**: To build the project.
- **pdf_report.pdf**: Documentation and experimental results on hash functions.

## Requirements ⚙️
- C++ (Linux/Unix/macOS)
- Makefile to compile the project

## How to Run ▶️
1. Clone this repository.
2. Build the project using:
   ```bash
   make
   ```
3. Run the program:
   ```bash
   ./dictionary
   ```

## Features ✨
- **Importing**: Load translations from `en-de.txt`, `en-fr.txt`, and `en-es.txt`.
- **Adding/Searching**: Add and search for words with translations.
- **Deleting**: Lazy deletion of words, translations, or meanings.
- **Exporting**: Export dictionary entries to a file.

## Contact 📬
Feel free to reach out to me for any questions or feedback:
- **Email**: ab11475@nyu.edu
- **LinkedIn**: [abinet-cholo-a6a13029a](https://www.linkedin.com/in/abinet-cholo-a6a13029a)
