function rec_reverse(str) {
    let [head, ...tail] = str;
    if(head == undefined) return ""; // Oppure str.length == 0 ???
    // if(str.length == 1) return str;
    return rec_reverse(tail) + head;
}

function test(func, data) {
    data.forEach(element => {
        console.log(element + " ==> " + func(element));        
    });
}

let data = ["ciao", "come", "stai", "oggi", "claudio"]
test(rec_reverse, data);
