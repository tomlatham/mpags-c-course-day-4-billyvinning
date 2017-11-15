#include<string>
#include<PlayfairCipher.hpp>
#include<CipherMode.hpp>
#include<iostream>
#include<algorithm>
#include<utility>
#include<map>

PlayfairCipher::PlayfairCipher(const std::string& key)
{
	setKey(key);
}

void PlayfairCipher::setKey(const std::string& key)
{
	//store the original key
	key_ = key;

	// Append the alphabet
	key_.append("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	// Make sure the key is upper case
	std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);
	
	// Remove non-alpha characters
	auto iterNonAlpha = std::remove_if(key_.begin(), key_.end(), [] (char in) {return !isalpha(in);});
	key_.erase(iterNonAlpha, key_.end());

	// Change J -> I
	std::transform(key_.begin(), key_.end(), key_.begin(), [] (char in) {if(in == 'J'){return 'I';} else{return in;}});
	
	// Remove duplicated letters
	// Return true if alreadySeen contains the relevant character, appends alreadySeen
	std::string alreadySeen{""};
	auto iterDuplicates = std::remove_if(key_.begin(), key_.end(), [&] (char in) {if(alreadySeen.find(in)!=std::string::npos){return true;}else{alreadySeen+=in; return false;}});
	key_.erase(iterDuplicates, key_.end());

	// Store the coords of each letter
	size_t x{0},y{0};
	PlayfairCoords elem{x,y};
	
	for(size_t i = 0; i < key_.length() ; i++)
	{
		elem = std::make_pair(x,y);
		mapLetter2Coords_.insert(std::make_pair(key_[i], elem));
		mapCoords2Letter_.insert(std::make_pair(elem, key_[i]));
		x++;

		if(x==5)
		{
			x=0;
			y++;
		}
		if(y==5)
		{
			break;
		}
	
	}
}

std::string PlayfairCipher::applyCipher(const std::string& inputText, const CipherMode cipherMode) const
{
	std::string input { inputText };

	
	PlayfairCoords firstCoord, secondCoord;
	// TEL - there's a lot of duplicated code here due to you introducing the switch this early
	//     - also, having the replacement of J with I and insertion of X or Q to separate
	//       identical letters in a digraph only in the encrypt raises some potential problems,
	//       since you cannot guarantee that the text provided by the user for decryption
	//       necessarily obeys the rules
	//     - see our solution for another approach
	switch(cipherMode)
	{
		case CipherMode::Encrypt:
			{
				std::cout << "Encrypting..." << std::endl;
				std::transform(input.begin(), input.end(), input.begin(), [] (char in) {if(in == 'J'){return 'I';} else{return in;}});
				// If repeated chars in a digraph add an X or Q if XX
				const std::string x = "X";
				const std::string q = "Q";

				for(size_t i {0}; i < input.length(); i+=2)
				{
					if (i+1 == input.length())
					{
						// Check if this is the last character
						break;
					}
					else if(input[i] == input[i+1] && input[i] !='X') 
					{
						// If this character is the same as
						// the next and neither are X,
						// insert an X between them
						input.insert(i+1, x);
					}
					else if(input[i] == input[i+1] && input[i] == 'X')
					{
						// If this character is the same as
						// the next and both are X,
						// insert a Q between them
						input.insert(i+1, q);
					}
				}

				std::cout << input << std::endl;

				// if the size of input is odd, add a trailing Z
				// unless the last character is already a Z,
				// in which case add an X
				if(input.length()%2 != 0)
				{
					input += (input[input.length()-1] == 'Z') ? 'X' : 'Z';
				}
	
				std::cout << input << std::endl;

				// Loop over the input in Digraphs
				for(size_t i = 0; i < input.length(); i+=2)
				{
					firstCoord = mapLetter2Coords_.find(input[i])->second;
					secondCoord = mapLetter2Coords_.find(input[i+1])->second;

					// Apply the rules to these coords to get `new' coords
					if(firstCoord.first == secondCoord.first)
					{
						firstCoord.second = (firstCoord.second + 1)%5;
						secondCoord.second = (secondCoord.second + 1)%5;
					}
					else if(firstCoord.second == secondCoord.second)
					{
						firstCoord.first = (firstCoord.first + 1)%5;
						secondCoord.first = (secondCoord.first + 1)%5;
					}
					else
					{
						size_t store = firstCoord.first;
						firstCoord.first = secondCoord.first;
						secondCoord.first = store;
					}

					// 	- Find the letter associated with the new coords
					input[i] = mapCoords2Letter_.find(firstCoord)->second;	
					input[i+1] = mapCoords2Letter_.find(secondCoord)->second;
				}

				
			}break;

		case CipherMode::Decrypt:
			{
				std::cout << "Decrypting..." << std::endl;
			
				for(size_t i = 0; i < input.length(); i+=2)
				{
					firstCoord = mapLetter2Coords_.find(input[i])->second;
					secondCoord = mapLetter2Coords_.find(input[i+1])->second;

					// Apply the rules to these coords to get decrypted coords
					if(firstCoord.first == secondCoord.first)
					{
						firstCoord.second = (5 + firstCoord.second - 1)%5;
						secondCoord.second = (5 + secondCoord.second - 1)%5;
					}
					else if(firstCoord.second == secondCoord.second)
					{
						firstCoord.first = (5 + firstCoord.first - 1)%5;
						secondCoord.first = (5 + secondCoord.first - 1)%5;
					}
					else
					{
						size_t store = firstCoord.first;
						firstCoord.first = secondCoord.first;
						secondCoord.first = store;
					}

					// 	- Find the letter associated with the new coords
					input[i] = mapCoords2Letter_.find(firstCoord)->second;	
					input[i+1] = mapCoords2Letter_.find(secondCoord)->second;
				}
			}break;
	}
	
	// return the text
	std::cout << "Playfair cipher applied!" << std::endl;
	return input;

}
