# huffman-coder
Console implementation of Huffman's coding algorithm

An implementation of Huffman's coding algorithm which allows to encode the input text and output the calculated probabilities for each symbol in the text, dictionary of prefix-free codes assigned to each symbol as well as the full encoded input message and calculated average codeword length and entropy. Because of its nondeterministic nature, in this case the algorithm searches the tree from "left-to-right" and assigns "0" to the smaller probability value in tree and "1" to the bigger probability value in tree - the proper codeword length for all symbols is still preserved. The output sequences and encoded message are additionally displayed as negated.

<img src = screenshot_1.png width=600>
