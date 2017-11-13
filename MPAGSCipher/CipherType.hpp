#ifndef MPAGSCIPHER_CIPHERTYPE_HPP
#define MPAGSCIPHER_CIPHERTYPE_HPP

/**
 * \file CipherType.hpp
 * \brief Contains the declaration of the CipherType enumeration
 */

/**
 * \enum CipherType
 * \brief Enumeration to define the types in which the ciphers can operate
 */
enum class CipherType {
  caesar, ///< Apply caesar cipher
  playfair  ///< Apply playfair cipher
};

#endif // MPAGSCIPHER_CIPHERTYPE_HPP
