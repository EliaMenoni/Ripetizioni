function rec_pal(word){
    console.log(word);
    var len = word.length;
    if(len == 1) return true;
    if(len == 2) return word[0] == word[1];

    return word[0] == word[len-1] && rec_pal(word.slice(1, -1));
}

console.log(rec_pal("ciao"));
console.log(rec_pal("ABCDEF"));
console.log(rec_pal("AABBCCBBAA"));
console.log(rec_pal("AABBCBBAA"));
console.log(rec_pal("AABBCDBBAA"));