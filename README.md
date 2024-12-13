# Progetto di Programmazione Procedurale UniPG

# Scalogna-Quest: Un gioco testuale in C

**Autore:** Alessio Alunni Santoni
**Anno Accademico:** 2023/2024

## Descrizione del progetto

"Scalogna-Quest" è un gioco testuale in C ispirato a Hero Quest, progettato come esercizio di programmazione strutturata. I giocatori affrontano un'avventura in un dungeon pieno di pericoli, tesori e decisioni strategiche.  
L'obiettivo è attraversare la mappa e arrivare per primi all'ultima stanza, affrontando abitanti delle segrete, scassinando porte e gestendo i propri punti vita e caratteristiche speciali.

### Caratteristiche principali
- **Gestione dinamica della memoria:** Allocazione e deallocazione delle strutture di gioco in tempo reale.
- **Mappa interattiva:** Creazione e modifica di una mappa a liste doppiamente collegate.
- **Classi dei giocatori:** Barbaro, Nano, Elfo, Mago, ognuna con caratteristiche uniche (punti vita, dadi attacco/difesa, mente, potere speciale).
- **Turni dinamici:** Ordine casuale dei turni per ogni round.
- **Eventi casuali:** Incontri con abitanti delle segrete e porte da scassinare con esiti imprevedibili.
- **Combattimenti strategici:** Sistema di dadi per attacco e difesa, con effetti visibili delle azioni di gioco.

### File del progetto
- **`main.c`:** Contiene il menu principale del gioco e coordina le chiamate alle funzioni della libreria.
- **`gamelib.c`:** Include tutte le funzionalità di gioco, come la gestione della mappa, i combattimenti e i turni.
- **`gamelib.h`:** Dichiarazioni delle funzioni non statiche e definizioni delle strutture e degli enumeratori utilizzati.

## Requisiti tecnici
- **Compilatore:** GCC 9.3.0 (Ubuntu 20.4).
- **Standard:** C11.  
Assicurati di compilare con i flag `-std=c11 -Wall` per il rispetto dello standard e per rilevare eventuali warning.

## Come compilare
```bash
gcc -c main.c -std=c11 -Wall
gcc -c gamelib.c -std=c11 -Wall
gcc -o scalogna-quest main.o gamelib.o
```

## Come giocare
Avvia il gioco con il comando:  
```bash
./scalogna-quest
