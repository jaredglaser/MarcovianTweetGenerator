# MarcovianTweetGenerator

The marcov chain algorithm used is essentially what is described in https://drmysterian.com/how-to-code-a-python-markov-chain-text-generator/, but using 2d arrays to store the parsed data.

# Dependencies
Boost >= 1.61.0

OpenSSL >= 1.1

cmake >= 3.13.4

# Installation in Ubuntu environment

Gather Dependencies:

  `sudo apt install libboost-all-dev libssl-dev cmake libcurl4-gnutls-dev`
  
Clone this Repository:

  `git clone https://github.com/jaredglaser/MarcovianTweetGenerator`
  
Clone the Twitter API Repo: I'm using Ryan Geary's fork that allows me to get the extended versions of the tweets.

  `git clone https://github.com/theryangeary/twitcurl.git`
  
Make the Twitter API Repo:

  `cd twitcurl/libtwitcurl`
  
  `sudo make`
  
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
  

