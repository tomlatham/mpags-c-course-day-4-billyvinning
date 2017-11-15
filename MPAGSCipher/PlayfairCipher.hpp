#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

// Standard library includes
#include <string>
#include <vector>
#include <map>
// Our project headers
#include "CipherMode.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */

class PlayfairCipher {
  public:
    /**
     * Create a new PlayfairCipher with the given key
     *
     * \param key the key to use in the cipher
     */
    PlayfairCipher( const std::string& key );

    /**
     * Sets the active key
     *
     * \param key the encryption or decryption key
     * \return void 
     */
    void setKey( const std::string& key );

    /**
     * Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher( const std::string& inputText, const CipherMode cipherMode ) const;
  private:
    /// The cipher key, essentially a constant shift to be applied
    std::string key_ = "A";

    /// Type definition for the coordinates in the 5x5 table
    using PlayfairCoords = std::pair<size_t,size_t>;

    /// The key map, maps letter to set of x,y coordinates in a 5x5 matrix
    std::map<char,PlayfairCoords> mapLetter2Coords_;

    /// The key map, maps set of x,y coordinates to a letter
    std::map<PlayfairCoords,char> mapCoords2Letter_;
};

    

#endif
