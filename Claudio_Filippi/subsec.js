function subSec(ago, pagliaio) {
    var temp = 0;
    var found = true;
    for(var i = 0; i <= pagliaio.length - ago.length; i++) {
        temp = i;
        found = true;
        for (var j = 0; j < ago.length; j++) {
            if(pagliaio[temp] == ago[j] )
                temp+=1;
            else{
                found = false;
                break;
            }
        }
        if(found) return true;
    }
    return false;
}

console.log(subSec([5,6,7],[1,2,3,4,5,6]));
console.log(subSec([true, false], []))
console.log(subSec([1,3.14,5], [1,3.14,4,1,3.14,5]))