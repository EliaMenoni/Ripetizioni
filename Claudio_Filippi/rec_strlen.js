function rec_strlen(str) {
    let [head, ...tail] = str;
    if(head == undefined) return 0;
    return 1 + rec_strlen(tail);
}

function test(func, data) {
    data.forEach(element => {
        console.log(element + " ==> " + func(element));        
    });
}

let data = ["ciao", "123456", "123456789", "123456789123", "123"]
test(rec_strlen, data);