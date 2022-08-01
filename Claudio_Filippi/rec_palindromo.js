function rec_pal(word) {
    var len = word.length;
    if (len == 1) return true;
    if (len == 2) return word[0] == word[1];

    return word[0] == word[len - 1] && rec_pal(word.slice(1, -1));
}

function test(func, data) {
    data.forEach(element => {
        console.log(element + " ==> " + func(element));        
    });
}

let data = ["ciao", "ABCDEF", "AABBCCBBAA", "AABBCBBAA", "AABBCDBBAA"]
test(rec_pal, data);
