function pota(t, k) {
    if (t == undefined) {
        return;
    }
    if (t.sx && (t.sx > k || t.sx.val > k)) {
        elimina(t.sx);
        delete t.sx;
        pota(t.dx);
    }
    if (t.dx && (t.dx > k || t.dx.val > k)) {
        elimina(t.dx);
        delete t.dx;
        pota(t.sx);
    }
    pota(t.sx);
    pota(t.dx);
    return;
}


function elimina(t) {
    if (t == undefined) {
        return;
    }
    elimina(t.sx);
    elimina(t.dx);
    delete t.sx;
    delete t.dx;
    delete t;
}
let t = {"val": 1, "sx": {"val": 8, "sx": 7, "dx": 1}, "dx": {"val": 3, "sx": 5, "dx": undefined}}; 
pota(t, 4);
console.log(t);
