#include "hashtable.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;


Translation::Translation(string meanings, string language) {     // Constructor
    this->language = language;   // Set the language
    stringstream ss(meanings);  // Create a stringstream object
    string meaning;
    addMeaning(meanings);       // Add the meanings to the translation
}

void Translation::addMeaning(string newMeanings) {    
    stringstream ss(newMeanings);                // Create a stringstream object
    string meaning; // Create a string to store the meanings
    while (getline(ss, meaning, ';')) {  // Split the meanings by semicolon
        bool meaning_exist = false;  
        for (int i = 0; i < meanings.size(); i++) {     // Iterate through the meanings vector
            if (meanings[i] == meaning) {               // Check if the meaning already exists
                meaning_exist = true;                   // Set the flag to true
                break;
            }
        }
        if (!meaning_exist) {                           // If the meaning does not exist
            meanings.push_back(meaning);        // Add the meaning to the vector
        }
    }
}

void Translation::display() {
    cout << setw(8) << left<< language;             // Display the language
    cout <<" : ";
    for (int i = 0; i < meanings.size(); i++) {        // Iterate through the meanings vector
        cout << meanings[i];                            // Display the meanings
        if (i < meanings.size() - 1) {
            cout << "; ";                   // Add a semicolon if there are more meanings
        }
    }
    cout << endl;
}

Entry::Entry(string word, string meanings, string language) {           // constructor implementation
    this->word = word;                                                  
    Translation trans(meanings, language);                              // Create a new translation object
    translations.push_back(trans);                                      // Add the translation to the vector
    deleted = false;                                                    // Set the deleted flag to false
}

void Entry::addTranslation(string newMeanings, string language) {          // Add translation function implementation
    for (int i = 0; i < translations.size(); i++) {                     // Iterate through the translations vector
        if (translations[i].language == language) {                     // Check if the translation already exists
            translations[i].addMeaning(newMeanings);                // Add the meanings to the translation
            return;
        }
    }
    Translation trans(newMeanings, language);
    translations.push_back(trans);
}

void Entry::print() {
    for (int i = 0; i < translations.size(); i++) {
        translations[i].display();
    }
}



HashTable::HashTable(int capacity) {
    buckets = new Entry*[capacity];
    for (int i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }
    this->capacity = capacity;
    size = 0;
    collisions = 0;
}

/*unsigned long HashTable::hashCode(string word) {
    unsigned long hash = 0;
    for (int i = 0; i < word.length(); ++i) {
        char c = word[i];
        hash = (hash * 31 + c) % capacity;
    }
    return hash;
}*/


/*unsigned long HashTable::hashCode(string word) {
    unsigned long hash = 5381; // Initial hash value
    for (size_t i = 0; i < word.length(); ++i) {
        char c = word[i];
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % capacity; // Modulo to fit within the capacity of the hash table
}*/

// FNV-1a hash function
unsigned long HashTable::hashCode(string str) {
    const unsigned int fnv_prime = 16777619; // FNV prime value
    const unsigned int offset_basis = 2166136261;  // FNV offset basis
    unsigned int hash = offset_basis;  // Initial hash value

    for (char c : str) {  // Iterate through the characters of the string
        hash ^= c;  // XOR the hash with the character
        hash *= fnv_prime;  // Multiply the hash by the FNV prime value
    }

    return hash%capacity;  // Modulo to fit within the capacity of the hash table
}



unsigned int HashTable::getSize() {
    return size;
}

unsigned int HashTable::getCollisions() {
    return collisions;
}


void HashTable::import(string path) {
    ifstream file(path);
    //file.open(path); // Open the file
    bool opened = true;
    if (!file.is_open()) { // Check if the file is opened successfully
        cerr << "File not opened"<<endl;  // Display an error message
        opened = false;
    }
    int count = 0;
    string lang;    // Create a string to store the language
    getline(file, lang);// Read the first
    string line;
    while (getline(file, line)) {// Read the file line by line
        stringstream ss(line);// Create a stringstream object
        string word, meanings;
        getline(ss, word, ':');// Split the line by colon
        getline(ss, meanings);// Get the meanings
        insert(word, meanings, lang);// Insert the word into the hash table
        count++;
    }
    file.close();
    if (opened){
    cout<<count<<" "<<lang<<" words have been imported successfully."<<endl;// Display a success message
    }
}


void HashTable::insert(string word, string meanings, string language) {
    string words = "";
    for (int i = 0; i < word.length(); i++){ // Convert the word to lowercase
            char c = word[i];  
            words += tolower(c);
        }
    
    unsigned int index = hashCode(words);  // Calculate the hash code
    if (buckets[index] == nullptr || buckets[index]->word == words) {  // Check if the bucket is empty or the word already exists

        if (buckets[index] == nullptr) {  // If the bucket is empty, create a new entry
            
            buckets[index] = new Entry(words, meanings, language);
            size++;
        } else if (!buckets[index]->deleted){
            
            buckets[index]->addTranslation(meanings, language); // If the word already exists in the bucket, add translations to it
        }
        else if (buckets[index]->deleted){   // If the word already exists in the bucket, add translations to it
            buckets[index] = new Entry(words, meanings, language);   // Create a new entry
            size++;  // Increment the size
        }
    } else {
        // Handle collision using linear probing
        unsigned int i = 1;  // Create a variable to track the number of probes
        collisions++;  // Increment the number of collisions
        while(buckets[(index + i) % capacity] != nullptr && buckets[(index + i) % capacity]->word != words) {    // Iterate through the buckets
            i++;  // Increment the probe count
            collisions++;  // Increment the number of collisions
        }
        if (buckets[(index + i) % capacity] == nullptr) {    // If an empty bucket is found, create a new entry
            
            buckets[(index + i) % capacity] = new Entry(words, meanings, language);    // Create a new entry
            size++;   // Increment the size
        } else if (!buckets[(index + i) % capacity]->deleted){  // If the word already exists in a different bucket, add translations to it
            
            buckets[(index + i) % capacity]->addTranslation(meanings, language);   // Add translations to the word
        }
        else if (buckets[(index + i) % capacity]->deleted){      // If the word already exists in a different bucket, add translations to it
            buckets[(index + i) % capacity] = new Entry(words, meanings, language);   // Create a new entry
            size++;  // Increment the size
    
        }

    }
    
}


void HashTable::delWord(string word) {    // Delete word function implementation
    string words = "";
    for (int i = 0; i < word.length(); i++){ // Convert the word to lowercase
            char c = word[i];  
            words += tolower(c);
        }
    unsigned int index = hashCode(words);  // Calculate the hash code

    if (buckets[index] != nullptr && buckets[index]->word == words) {    // If the word is found in the bucket at the calculated index
        if (!buckets[index]->deleted){    // If the word is not marked as deleted
        buckets[index]->deleted = true;   // Mark the word as deleted
        size--;
        cout << word << " has been successfully deleted from the Dictionary."<<endl; // Display a success message
        }
        else {
            cout <<word<< " not found in the Dictionary"<<endl;   // Display a message if the word is not found
        }
    } else {
        // Search for the word in case of collision
        unsigned int i = 1;
        while (buckets[(index + i) % capacity] != nullptr && buckets[(index + i) % capacity]->word != words) {    // Iterate through the buckets
            i++;
        }
        if (buckets[(index + i) % capacity] != nullptr && buckets[(index + i) % capacity]->word == words) {     // If the word is found in a different bucket
            if(!buckets[(index + i) % capacity]->deleted){  // If the word is not marked as deleted
            buckets[(index + i) % capacity]->deleted = true;  // Mark the word as deleted
            size--;
            cout << word << " has been successfully deleted from the Dictionary."<<endl;  // Display a success message
            }
            else{
                cout <<word<< " not found in the Dictionary"<<endl;  // Display a message if the word is not found
            }
        }
    }
}

void HashTable::addWord(string word, string meanings, string language) {   
    if (word.empty() || all_of(word.begin(), word.end(), [](unsigned char c) { return isspace(c); }) ||
        meanings.empty() || all_of(meanings.begin(), meanings.end(), [](unsigned char c) { return isspace(c); }) ||
        language.empty() || all_of(language.begin(), language.end(), [](unsigned char c) { return isspace(c); })) {// Check if any input is empty or consists only of whitespace characters
        
        cout << "Invalid input. Please provide a valid word, meaning, and language." << endl;  // Display an error message
        return;
    }
    
   
    insert(word, meanings, language); // Insert the word into the hash table
    cout << word << " has been successfully added to the Dictionary." << endl;  // Display a success message
}


void HashTable::delTranslation(string word, string language) {    // Delete translation function implementation  
    string words = "";
    for (int i = 0; i < word.length(); i++){  // Convert the word to lowercase
            char c = word[i];  
            words += tolower(c);
        }
    unsigned int index = hashCode(words);   
    if (buckets[index] != nullptr && buckets[index]->word == words && !buckets[index]->deleted) {      // If the word is found in the bucket at the calculated index
        
        for (int i = 0; i < buckets[index]->translations.size(); i++) {        // Iterate through the translations vector
            if (buckets[index]->translations[i].language == language) {         // Check if the translation exists
                
                buckets[index]->translations.erase(buckets[index]->translations.begin() + i);    // Remove the translation
                if (buckets[index]->translations.empty()){       // If all translations are removed
                    buckets[index]->deleted = true;     // Mark the word as deleted
                    size--;   // Decrement the size
                }
                cout << "Translation has been successfully deleted from the dictionary"<<endl;   // Display a success message
                return;
            }
        }
    } else {
        // Handle collision using linear probing
        unsigned int i = 1;
        while (buckets[(index + i) % capacity] != nullptr && buckets[(index + i) % capacity]->word != words) {    // Iterate through the buckets
            i++;
        }
        if (buckets[(index + i) % capacity] != nullptr && buckets[(index + i) % capacity]->word == words && !buckets[(index + i) % capacity]->deleted) {  // If the word is found in a different bucket due to collision
            
            for (int j = 0; j < buckets[(index + i) % capacity]->translations.size(); j++) {    // Iterate through the translations vector
                if (buckets[(index + i) % capacity]->translations[j].language == language) {    // Check if the translation exists
                    
                    buckets[(index + i) % capacity]->translations.erase(buckets[(index + i) % capacity]->translations.begin() + j); // Remove the translation
                    if (buckets[(index + i) % capacity]->translations.empty()){   // If all translations are removed
                        buckets[(index + i) % capacity]->deleted = true;   // Mark the word as deleted
                        size--;  // Decrement the size
                    }
                    cout << "Translation has been successfully deleted from the dictionary"<<endl;  //  Display a success message
                    return;
                }
            }
        }
    }
    
    cout << "Translation not found for the word in the specified language." << endl;   // Display a message if the translation is not found
}


void HashTable::delMeaning(string word, string meaning, string language) {
    string words = "";  
    for (int i = 0; i < word.length(); i++){  // Convert the word to lowercase
            char c = word[i];  
            words += tolower(c);
        }
    unsigned int index = hashCode(words);
    if (buckets[index] != nullptr && buckets[index]->word == words && !buckets[index]->deleted ) { // If the word is found in the bucket at the calculated index
        
        for (int i = 0; i < buckets[index]->translations.size(); i++) {  // Iterate through the translations vector
            if (buckets[index]->translations[i].language == language) {  // Check if the translation exists
                
                for (int j = 0; j < buckets[index]->translations[i].meanings.size(); j++) {   // Iterate through the meanings vector
                    if (buckets[index]->translations[i].meanings[j] == meaning) {    // Check if the meaning exists
                        buckets[index]->translations[i].meanings.erase(buckets[index]->translations[i].meanings.begin() + j);   // Remove the meaning
                        cout<<"Meaning has been successfully deleted from the Dictionary."<<endl;  // Display a success message
                        
                        if (buckets[index]->translations[i].meanings.empty()) {    // If all meanings are removed
                            buckets[index]->translations.erase(buckets[index]->translations.begin() + i);    // Remove the translation
                            if (buckets[index]->translations.empty()){    // If all translations are removed
                                buckets[index]->deleted = true;  // Mark the word as deleted
                                size--;  // Decrement the size
                            }

                        }
                        return;
                    }
                }
                
                cout << "Meaning not found." << endl;   // Display a message if the meaning is not found
                return;
            }
        }
    } else {
        // Handle collision using linear probing
        unsigned int i = 1;
        while (buckets[(index + i) % capacity] != nullptr && buckets[(index + i) % capacity]->word != words) {   // Iterate through the buckets
            i++;
        }
        if (buckets[(index + i) % capacity] != nullptr && buckets[(index + i) % capacity]->word == words && !buckets[(index + i) % capacity]->deleted) {    // If the word is found in a different bucket due to collision
            
            for (int k = 0; k < buckets[(index + i) % capacity]->translations.size(); k++) {   // Iterate through the translations vector
                if (buckets[(index + i) % capacity]->translations[k].language == language) {   // Check if the translation exists
                    
                    for (int l = 0; l < buckets[(index + i) % capacity]->translations[k].meanings.size(); l++) {   // Iterate through the meanings vector
                        if (buckets[(index + i) % capacity]->translations[k].meanings[l] == meaning) {          // Check if the meaning exists
                            buckets[(index + i) % capacity]->translations[k].meanings.erase(buckets[(index + i) % capacity]->translations[k].meanings.begin() + l);   // Remove the meaning
                            cout<<"Meaning has been successfully deleted from the Dictionary."<<endl;   //Display a success message
                            
                            if (buckets[(index + i) % capacity]->translations[k].meanings.empty()) {    // If all meanings are removed
                                buckets[(index + i) % capacity]->translations.erase(buckets[(index + i) % capacity]->translations.begin() + k);   // Remove the translation
                                if (buckets[(index + i) % capacity]->translations.empty()){   // If all translations are removed
                                    buckets[(index + i) % capacity]->deleted = true;   // Mark the word as deleted
                                    size--;  // Decrement the size
                                }
                            }
                            return;
                        }
                    }
                    
                    cout << "Meaning not found." << endl; // Display a message if the meaning is not found
                    return;
                }
            }
        }
    }
    
    cout << "Word not found in the hash table." << endl;// Display a message if the word is not found
}


void HashTable::exportData(string language, string filePath) {
    ofstream outputFile;  // Create an output file stream
    outputFile.open(filePath);
    if (!outputFile.is_open()) {  // Check if the file is opened successfully
        cerr << "Unable to open file for writing: " << filePath << endl;  // Display an error message
        return;
    }

   
    outputFile << language << endl;   // Write the language name to the file
    int count = 0;
    
    for (int i = 0; i < capacity; i++) {  // Iterate through the buckets
        if (buckets[i] != nullptr && !buckets[i]->deleted ) {// Check if the bucket is not empty and the word is not marked as deleted
            
            for (int j = 0; j < buckets[i]->translations.size(); j++) {  // Iterate through the translations vector
                
                if (buckets[i]->translations[j].language == language) {  // Check if the translation is in the specified language
                    
                    count++;  // Increment the count
                    outputFile << buckets[i]->word << ":"; // Write the word to the file
                    for (int k = 0; k < buckets[i]->translations[j].meanings.size(); k++) {// Iterate through the meanings vector
                        outputFile << buckets[i]->translations[j].meanings[k];// Write the meanings to the file
                        if (k < buckets[i]->translations[j].meanings.size() - 1) {  // Add a semicolon if there are more meanings
                            outputFile << ";";  // Add a semicolon
                        }
                    }
                    outputFile << endl; // Add a new line
                    
                    break;
                }
            }
        }
    }
    if (count>0){
    cout<<count <<" records have been successfully exported to "<< filePath<<endl;// Display a success message
    }
    else{
        cout<<"No records found for the specified language."<<endl;// Display a message if no records are found
    }

    outputFile.close();// Close the file
}



void HashTable::find(string word) {
    string words = "";//    Convert the word to lowercase
    for (int i = 0; i < word.length(); i++){  //    Convert the word to lowercase
            char c = word[i];  
            words += tolower(c);
        }
    unsigned int index = hashCode(words);  // Calculate the hash code
    int comparisons = 0; // Counter to track the number of comparisons
    bool exit_truth = false;
    if (buckets[index] != nullptr && buckets[index]->word == words) {  // If the word is found in the bucket at the calculated index
        
        if (!buckets[index]->deleted) {  // If the word is not marked as deleted
            comparisons++;  // Increment the number of comparisons
            exit_truth = true;
            cout << word << " found in the Dictionary after "<< comparisons<<" comparisons."<<endl;  // Display the number of comparisons
            buckets[index]->print();  // Print the word
                    } else {
            cout << word << " was not found." << endl;  //    Display a message if the word is marked as deleted
            comparisons++;  // Increment the number of comparisons
        }
    } else {
        // Handle collision using linear probing
        unsigned int i = 1;
        while (buckets[(index + i) % capacity] != nullptr && buckets[(index + i) % capacity]->word != words) {// Iterate through the buckets
            i++;
            comparisons++;// Increment the number of comparisons
        }
        if (buckets[(index + i) % capacity] != nullptr && buckets[(index + i) % capacity]->word == words) {// If the word is found in a different bucket due to collision
            
            if (!buckets[(index + i) % capacity]->deleted) {// If the word is not marked as deleted
                comparisons++;  // Increment the number of comparisons
                exit_truth = true;
                cout << word << " found in the Dictionary after "<< comparisons<<" comparisons."<<endl; // Display the number of comparisons
                buckets[(index + i) % capacity]->print();// Print the word
                
            } else {
                cout << word << " not found in the Dictionary" << endl; // Display a message if the word is marked as deleted
                comparisons++;  // Increment the number of comparisons
            }
        }
    }

    
        // Word not found
    if (!exit_truth) {//    Display a message if the word is not found
        cout << word << " not found in the Dictionary." << endl;  // Display a message if the word is not found
    }
}


HashTable::~HashTable() {  // Destructor implementation
    for (int i = 0; i < capacity; i++) {  // Iterate through the buckets
        delete buckets[i];  // Delete the bucket
    }
    delete[] buckets;  // Delete the array
}