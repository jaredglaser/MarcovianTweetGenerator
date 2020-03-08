# MarcovianTweetGenerator

The marcov chain algorithm used is essentially what is described in https://drmysterian.com/how-to-code-a-python-markov-chain-text-generator/, but using a map with a word as the key and a vector holding the possible next words with their probabilities. The program picks a random word to start with and then chooses a random number and compares it against the probabilites of each of the possible next words. Whichever next word is chosen by the random number is then located in the map... this will loop until the full tweet is formed.

# Dependencies
Boost >= 1.61.0

OpenSSL >= 1.1

Ryan Geary's fork of twitcurl (https://github.com/theryangeary/twitcurl)

# Installation in Ubuntu environment

Gather Dependencies:

  `sudo apt install libboost-all-dev libssl-dev cmake libcurl4-gnutls-dev`
  
Clone this Repository:

  `git clone https://github.com/jaredglaser/MarcovianTweetGenerator`
  
Clone the Twitter API Repo: I'm using Ryan Geary's fork that allows me to get the extended versions of the tweets.

  `git clone https://github.com/theryangeary/twitcurl.git`
  
Make twitcurl:

  `cd twitcurl/libtwitcurl`
  
  `sudo make`
  
Move the library file to /usr/lib/. You may have to copy it as both libtwitcurl.so and libtwitcurl.so.1

  `sudo cp libtwitcurl.so.1.0 /usr/lib/libtwitcurl.so`
  
You may also have to place the directory called /include/curl in /usr/include

  'sudo cp /include/curl /usr/include'
 
Go back to the clone of this Repo, make it, then run it:

  `cd ..\MarcovianTweetGenerator`
  
  `make`
  
  `./a.out`
  
If you get an error about it not being able to see the library files. Try running this command:

`sudo /sbin/ldconfig -v`

This should update ld so it can find the shared libraries that you have moved into /usr/lib
  

