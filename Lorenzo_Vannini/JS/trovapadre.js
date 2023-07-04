function trovaPadre(root, n) {
    // Se il nodo e' la radice o il nodo cercato
    if (root.val == n) return undefined;

    // Se il nodo non e' quello cercato e ha figli
    if (root.figli != undefined) {
        // Scorro sui figli
        for (figlio of root.figli) {
            // Guardo se sono i nodi che cerco
            res = trovaPadre(figlio, n)
            // Se sono i nodi che cerco
            if (res == undefined)
                // Restituisco i valore del nodo padre
                return root.val

            // Se ottengo un valore dalle chiamate ricorsive lo trasmetto al chimante
            if (res != undefined && res != -1)
                return res
        }
    }
    return -1
}

function trovaPadre2(root, n, padre = undefined) {
    if (root.val == n) return padre
    if (root.figli) {
        for (figlio of root.figli) {
            lookup = trovaPadre2(figlio, n, root)
            if (lookup != undefined) return lookup
        }
    }
    return undefined
}

var T = { val: 6, figli: [{ val: 9 }, { val: 10, figli: [{ val: 7 }] }, { val: 2 }] };

console.log(trovaPadre(T, 7))
console.log(trovaPadre2(T, 7).val)