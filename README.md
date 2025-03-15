# Mutes-Algorithm
Mute´s Algorithm is an obfuscation algorithm that aims to solve the problem with quantum computers or fast servers cracking information


it works by having a dictionary called a wordsource
this wordsource contains a large amount of words when encoding something we look if the words of our input exist in the wordsource/dictionary if they do we take the index or uid of that word in the wordspace and generate an output string of these indecises/ids

this will be our password now! we keep it secret.

now should an attacker get access to our dictionary/wordsource and have a functional quantum computer/fast server while they may generate thousands or millions of sets of random texts and information they can never figure out the true initial message this renders all attempts at brute forcing the truth with bare computing power useless and makes a random guess highly unlikely.






How It Works
The Wordsource (Dictionary) 

The algorithm maintains a wordsource, which is a structured dictionary containing a large collection of words (**NOTE it is recommended to use multilingual librariers these are at most a few hundred megabytes this prohibits language restructuring by detecting common words**), each assigned a unique index (UID).
Encoding Process

When encoding text, the algorithm checks if each word exists in the wordsource.
If found, it replaces the word with its corresponding UID.
If not found, it adds the new word to the wordsource, assigns a new UID, and then encodes it.
The output is a sequence of indices that act as the encoded message.
Security Through Obfuscation

The encoded text is meaningless without access to the original wordsource.
Even if an attacker has the wordsource and a quantum computer, they cannot deduce the original message through brute force.
Generating random sentences or texts to match the encoded sequence is computationally impractical, making brute-force guessing highly unlikely.

this can also be twisted one could for Example keep the wordsource hidden but the encoded text visible which would also make it impossible to practically deduce the words especially if the attacker doesnt know the language of the messengers. due to lacking the wordsouce so in other words this logic ensures one cannot figure out the previous message/information aslong as he is missing one of the other components add hashing to it and you have an even stronger algorithm this could easily be used with large services that keep a big dictionary aswell as store all words from all users that are not yet in the dictionary in the dictionary if done properly this makes breaking into an account without an earlier access practically impossible even with a quantum computer as one will just assemble random user data which might be from a real user or might also be completly fake.


This repo will contain a c++ prototype.
