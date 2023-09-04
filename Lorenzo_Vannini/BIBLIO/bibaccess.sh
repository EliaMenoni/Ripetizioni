#!/bin/bash
# bibacces.sh --query log1.log log2.log log3.log
type=""
files=""
for i in $@; do                        # for con i che va da 0 a $@ (l'array degli argomenti)
    if [[ $i == --* ]]; then           # mi matcha tutte le parole che iniziano con il doppio --
        type=$i                        # avrà trovato query o loan e salvo dentro type quale dei due casi è
    else
        files="${files}$i "            # altrimenti vuol dire che ho trovato un file e lo concateno alla lista di files
    fi
done
# echo $type
# echo $files

tot=0
tot_file=0
base_log_dir="./bibserver/logs/"
IFS=" "

read -a files <<< "$files"     # Spezzo la lista dei file in un array
for file in "${files[@]}"; do   # Ciclo sull'array (per ogni file e per ogni riga di ogni file)
    tot_file=0
    while read line; do
        IFS=' '                                 #IFS = variabile in cui salvare il valore per il quale spezzare la riga di testo
        read -a data <<< $line
        if [ "--${data[0]}" == $type ]          # guardo se il tipo corrisponde con un matching
        then
            tot=$(($tot+${data[1]}))            # conteggio il numero di query o loan
            tot_file=$(($tot_file+${data[1]}))  # tot file è il conteggio del file (log1, log2...) mentre tot è il conteggio delle risposte inviate dal server
        fi
    done < "$base_log_dir$file"
    echo "$file $tot_file"
done
echo "${type//\-} $tot"
