let file = require("fs");

function leggiStudenti(file) {
    var B = [];
    let A = readFileSync("studenti.txt");
    // A.split("\n");
    A.split("\n").forEach(element => {
        B.push(new Studenti(element.split(",")[0], element.split(",")[1], element.split(",")[2], element.split(",")[3]));
    });

    return B;
}

class Studenti {
    constructor(nome, cognome, citta, votoLaurea) {
        this.nome = nome;
        this.cognome = cognome;
        this.citta = citta;
        this.votoLaurea = votoLaurea;
    }
}

console.log(leggiStudenti("studenti.txt"))
