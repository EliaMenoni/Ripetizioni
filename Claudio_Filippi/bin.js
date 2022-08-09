// 10 = 0101 <- bit più significativo
// 10 = 1010 -> bit meno significativo

// val -> num base 10 da convertire in base 2
function bin_arr(val) {
    let res = [];

    do {
        let resto = val % 2;
        val = Math.floor((val / 2));
        res.unshift(resto);
    } while (val != 0);
    return res;
}

function bin_str(val) {
    let res = "";

    do {
        let resto = val % 2;
        val = Math.floor((val / 2));
        res = String(resto) + res;
    } while (val != 0);
    return res;
}

function spiana(str, num) {
    let res = str;
    for (var i = 0; i < Math.abs(str.length - num); i++) res = "0" + res;
    return res;
}

function sum(a, b) {
    console.log(a);
    console.log(b);
    let res = "";
    let riporto = 0;
    if (a.length < b.length) a = spiana(a, b.length);
    else b = spiana(b, a.length);

    for (let i = a.length - 1; i >= 0; i--) {
        let tmp = Number(a[i]) + Number(b[i]) + riporto;
        riporto = 0;
        switch (tmp) {
            case 0:
                res = "0" + res;
                break;
            case 1:
                res = "1" + res;
                break;
            case 2:
                res = "0" + res;
                riporto = 1;
                break;
            case 3:
                res = "1" + res;
                riporto = 1;
                break;
        }
    }
    if (riporto != 0) {
        res = "1" + res;
    }
    return res;
}


console.log(sum(bin_str(16), bin_str(5)));

&       -->     
&&      -->     