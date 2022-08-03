function isPermutation(a, b) {
    if (a.length !== b.length) return false;
    let map = new Map(a.map(x => [x, { count: 0 }]));
    a.forEach(x => map.get(x).count++);
    return b.every(x => {
        let match = map.get(x);
        return match && match.count--;
    });
}

function check_ins(start, to_ins) {
    if (start.length == 0) return to_ins;
    let insert = true;
    start.forEach(element => {
        if (isPermutation(element, to_ins)) insert = false;
    });
    if(insert){
        start.push(to_ins);
        return start;
    } 
    return start;
}

function rec_stra(n) {
    let res = [];
    for (let i = 2; i < n; i++) {
        if (n % i == 0) {
            res = [check_ins(res, [i, n / i])];
            let partial = rec_stra(n / i);
            if (partial.length != 0) {
                partial.forEach(element => {
                    let tmp = [i];
                    res = check_ins(res, tmp.concat(element));
                });
            }

        }
    }
    return res;
}

let res = rec_stra(60);
console.log(res);