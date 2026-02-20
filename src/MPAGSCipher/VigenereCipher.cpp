#include "VigenereCipher.hpp"
#include "Alphabet.hpp"
#include "CaesarCipher.hpp"

#include <algorithm>
#include <iostream>
#include <string>

/**
 * \file VigenereCipher.cpp
 * \brief Contains the implementation of the VigenereCipher class
 */

void VigenereCipher::setKey(const std::string& key )
{
    // Store the key
    key_ = key;

    // Make sure the key is uppercase
    std::transform(std::begin(key_), std::end(key_), std::begin(key_),
                    ::toupper);

    // Remove non-alphabet characters
    key_.erase(std::remove_if(std::begin(key_), std::end(key_),
                                [](char c) { return !std::isalpha(c); }),
                std::end(key_));

    // Check if the key is empty and replace with default if so
    if (key_ == ""){key_ = "KEY";}

    // loop over the key
    for(std::size_t i = 0; i < key_.size(); i++)
    {
        // Find the letter position in the alphabet
        std::size_t pos{Alphabet::alphabet.find(key_[i])};

        // Create a CaesarCipher using this position as a key
        CaesarCipher cipher{pos};

        // Insert a std::pair of the letter and CaesarCipher into the lookup
        charLookup_.insert({key_[i], cipher});
    }
}

VigenereCipher::VigenereCipher(const std::string& key)
{
// Set the given key
setKey(key);
}

std::string VigenereCipher::applyCipher(const std::string& inputText,
                            const CipherMode cipherMode) const
{
    std::string output{""};

    // For each letter in input:
    for(std::size_t i = 0; i < inputText.size(); i++)
    {
        // Find the corresponding letter in the key, repeating/truncating as required
        char key_char{key_[i % key_.size()]}; 

        // Find the Caesar cipher from the lookup
        CaesarCipher cipher{charLookup_.at(key_char)};

        // Run the (de)encryption
        // Add the result to the output
        std::string oneChar(1, inputText[i]);
        output += cipher.applyCipher(oneChar, cipherMode);
    }
    return output;
}