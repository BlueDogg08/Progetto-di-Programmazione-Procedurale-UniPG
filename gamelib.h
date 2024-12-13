#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

// colori utilizzati nel gioco
#define RESET   "\033[0m"
#define BOLD    "\e[1m"
#define ITALIC  "\033[3m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define ORANGE  "\033[1;33m"
#define YELLOW  "\033[1;93m"
#define MAGENTA "\033[1;35m"

// dichiarazione delle funzioni da chiamare in main.c
void stampa_titolo(void);
void imposta_gioco(void);
void gioca(void);
int termina_gioco(void);
void svuotaBuffer(void);


enum classe_giocatore{barbaro, nano, elfo, mago};
enum Tipo_zona{corridoio, scala, sala_banchetto, magazzino, giardino, porto_guardia, prigione, cucina, armeria, tempio};
enum Tipo_tesoro{nessun_tesoro, veleno, guarigione, doppia_guarigione};
enum Tipo_porta{nessuna_porta, porta_normale, porta_da_scassinare};
enum nome_abitante{Goblinus, Skeletor, Orkus, Mordrax, Trollgar, Zogroth, Vampyro, Grimfang, Darkshade, Wraithus, Shadowlurker, Bonecrusher, Deathclaw, Bloodfang, Nightstalker};


typedef struct Giocatore{
    char nome_giocatore[64];
    enum classe_giocatore classe;
    struct Zona_segrete* posizione;  // puntatore alla zona dove si trova il giocatore
    unsigned char p_vita;
    unsigned char dadi_attacco;  // n. dadi attacco
    unsigned char dadi_difesa;  // n. dadi difesa
    unsigned char mente;  // n. punti mente
    unsigned char potere_speciale;  // n. utilizzi possibili del potere speciale
} Giocatore;


typedef struct Zona_segrete{
    struct Zona_segrete* zona_successiva;  // avanzare nella mappa
    struct Zona_segrete* zona_precedente;  // tornare indietro nella mappa
    unsigned char n_stanza;  // n. stanza (da 1 a n_stanze)
    enum Tipo_zona zona;
    enum Tipo_tesoro tesoro;
    enum Tipo_porta porta;
} Zona_segrete;


typedef struct Abitante_segrete{
    enum nome_abitante nome;
    unsigned char p_vita;
    unsigned char dadi_attacco;  // n. dadi attacco
    unsigned char dadi_difesa;  // n. dadi difesa
} Abitante_segrete;
