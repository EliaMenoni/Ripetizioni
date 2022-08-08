function isPermutation(a, b) {
    if (a.length !== b.length) return false;
    let map = new Map(a.map(x => [x, { count: 0 }]));
    a.forEach(x => map.get(x).count++);
    return b.every(x => {
        let match = map.get(x);
        return match && match.count--;
    });
}

function check_permutation(set, data) {
    let res = false;
    set.forEach(element => {
        if (isPermutation(element, data)) res = true;
    });
    return res;
}

function union(left, right) {
    if (left.length == 0) return right;

    right.forEach(element => {
        if (!check_permutation(left, element)) left.push(element);
    });

    return left;
}

function strafatto(n) {
    let sec = [];
    for (let i = 2; i < n - 1; i++) {
        if (n % i == 0) {
            sec = union(sec, [[i, n / i]]);
            let sub = strafatto(n / i);
            if (sub != undefined)
                sub.forEach(element => {
                    element.unshift(i);
                });
            sec = union(sec, sub);
        }
    }
    return sec;
}

console.log(strafatto(60));