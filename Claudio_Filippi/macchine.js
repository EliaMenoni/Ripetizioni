class Macchina {
    constructor(ID, targa, marca, modello, porte, km, cilindrata, tipologia, data) {
        this.ID = ID;
        this.setTarga(targa);
        this.marca = marca;
        this.modello = modello;
        this.porte = porte;
        this.km = km;
        this.cilindrata = cilindrata;
        this.tipologia = tipologia;
        this.data = data;
    }

    setTarga(targa) {
        let regEx = /^([A-Z]{2}[0-9]{2}[A-Z]{2})$/;
        if (!regEx.test(targa))
            throw "Formato targa non conforme";
        else
            this.targa = targa ;
    }
};

try {
    new Macchina(1, "AA116BB", "BMW", "X3", 5, 10000, 100, "Disel", new Date("2022", "10", "12"))
} catch (e) {
    console.log(e)
}