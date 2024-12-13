#include "gamelib.h"


// DICHIARAZIONE VARIABIILI

// n. giocatori totali
int n_giocatori = 0;

// n. giocatori morti
int n_giocatori_morti = 0;

// variabile d'appoggio per genera_mappa
bool mappa_esistente = false;

// indica la possibilità di uscire dalle impostazioni della mappa
int mappa_terminata = 0;

// finché è true dimostra che la partita non è finita
bool gioco_in_corso = true;

// puntatore alla struct Giocatore
static struct Giocatore* PtrGiocatore = NULL;

// puntatori alle struct Zona_segrete
static struct Zona_segrete* pFirst = NULL;
static struct Zona_segrete* pLast = NULL;

// n. zone create
int n_zone = 0;

// contiene i puntatori dei giocatori morti
struct Giocatore* giocatori_morti[4] = {NULL, NULL, NULL, NULL}; // Inizializza tutti i puntatori a NULL

// se un giocatore è riuscito a fuggire da un combattimento
bool scappato = false;

// se un giocatore viene rispedito alla prima zona
bool rispedito = false;

// se la porta è stata scassinata o no
bool scassinata = true;


// DICHIARAZIONE DELLE FUNZIONI

// Funzioni utili al programma
static int randomize (int, int);
static void azzera_valori(void);
static void logo_vittoria(void);
static void logo_sconfitta(void);
static void stampa_giocatore(struct Giocatore*);
static const char* stampa_classe_giocatore(struct Giocatore*);
static void stampa_zona(struct Zona_segrete*);
static const char* stampa_tipo_zona(struct Zona_segrete*);
static const char* stampa_tipo_tesoro(struct Zona_segrete*);
static const char* stampa_tipo_porta(struct Zona_segrete*);
static const char* stampa_nome_abitante(struct Abitante_segrete*);
static struct Abitante_segrete* crea_abitante(void);
static void morte_giocatore(struct Giocatore*);
static bool is_giocatore_morto(struct Giocatore*);

// Funzioni in imposta_gioco()
static void numero_giocatori(void);
static void imposta_giocatori(void);
static void svuota_mappa(void);
static void imposta_mappa(void);
static void genera_mappa(void);
static void inserisci_zona(void);
static void cancella_zona(void);
static void stampa_mappa(void);
static void chiudi_mappa(void);
static void modifica_mappa(void);
static void assegna_num_zone(void);
static void imposta_partenza_giocatori(void);

// Funzioni in gioca()
static void regolamento(void);
static void scelta_turno(int*);
static void avanza(struct Giocatore*);
static void indietreggia(struct Giocatore*);
static void apri_porta(struct Giocatore*, Zona_segrete*);
static void prendi_tesoro(struct Giocatore*, Zona_segrete*);
static void incontro_abitante(struct Giocatore*);
static int scappa(struct Giocatore*, Abitante_segrete*);
static void combatti(struct Giocatore*, Abitante_segrete*);
static void attacco_giocatore(struct Giocatore*, Abitante_segrete*);
static void difesa_giocatore(struct Giocatore*, Abitante_segrete*);
static void gioca_potere_speciale(struct Giocatore*, Abitante_segrete*);
static void lancia_dadi(int n_dadi, int* risultati);
static void resoconto_finale(struct Giocatore*);


// DEFINIZIONE DELLE FUNZIONI

// elimina dal buffer dati superflui
void svuotaBuffer(void){
    while (getchar () != '\n' && getchar () != EOF);
}


// ritorna un n. random nell'intervallo passato
static int randomize(int nMin, int nMax){
    int nRand = nMin + (rand() % (nMax - nMin + 1));
    return nRand;
}


// azzera tutte le variabili (struct Giocatore compresa)
static void azzera_valori(void){
    free(PtrGiocatore);
    PtrGiocatore = NULL;
    n_giocatori = 0;
    n_giocatori_morti = 0;
    gioco_in_corso = true;
    PtrGiocatore = NULL;
    giocatori_morti[0] = giocatori_morti[1] = giocatori_morti[2] = giocatori_morti[3] = NULL;
    scappato = false;
    rispedito = false;
    scassinata = true;
}


// stampa il logo a inizio gioco
void stampa_titolo(void){

    printf("\n\n");
    printf("     ▄▀▀▀▀▄  ▄▀▄▄▄▄   ▄▀▀█▄   ▄▀▀▀▀▄    ▄▀▀▀▀▄   ▄▀▀▀▀▄    ▄▀▀▄ ▀▄  ▄▀▀█▄  \n");
    printf("    █ █   ▐ █ █    ▌ ▐ ▄▀ ▀▄ █    █    █      █ █         █  █ █ █ ▐ ▄▀ ▀▄ \n");
    printf("       ▀▄   ▐ █        █▄▄▄█ ▐    █    █      █ █    ▀▄▄  ▐  █  ▀█   █▄▄▄█ \n");
    printf("    ▀▄   █    █       ▄▀   █     █     ▀▄    ▄▀ █     █ █   █   █   ▄▀   █  \n");
    printf("     █▀▀▀    ▄▀▄▄▄▄▀ █   ▄▀    ▄▀▄▄▄▄▄▄▀ ▀▀▀▀   ▐▀▄▄▄▄▀ ▐ ▄▀   █   █   ▄▀   \n");
    printf("     ▐      █     ▐  ▐   ▐     █                ▐         █    ▐   ▐   ▐    \n");
    printf("            ▐                  ▐                          ▐                 \n");

    printf("\n");
    printf("                 ▄▀▀▀▀▄   ▄▀▀▄ ▄▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▀▀▀▄  ▄▀▀▀█▀▀▄ \n");
    printf("                █      █ █   █    █ ▐  ▄▀   ▐ █ █   ▐ █    █  ▐ \n");
    printf("                █      █ ▐  █    █    █▄▄▄▄▄     ▀▄   ▐   █     \n");
    printf("                 ▀▄▄▄▄▀▄   █    █     █    ▌  ▀▄   █     █      \n");
    printf("                        █   ▀▄▄▄▄▀   ▄▀▄▄▄▄    █▀▀▀    ▄▀       \n");
    printf("                        ▐            █    ▐    ▐      █         \n");
    printf("                                     ▐                ▐         \n");


    printf("\n\n");
    printf("             | | _    _ . _  _ _   _|_ _   __|_   _ | _   . _    _ \n");
    printf("             |_|| |  (_||(_)(_(_)   | (/__/  ||_|(_||(/_  || |  (_ \n");
    printf("                      _|                                          \n");
}


// stampa il logo in caso di vittoria
static void logo_vittoria(void){

    printf("\n\n");
    printf(YELLOW "             _______ _________           _________ _       _________ _______ \n" RESET);
    printf(YELLOW "   |\\     /|(  ___  )\\__   __/  |\\     /|\\__   __/( (    /|\\__   __/(  ___  )\n" RESET);
    printf(YELLOW "   | )   ( || (   ) |   ) (     | )   ( |   ) (   |  \\  ( |   ) (   | (   ) |\n" RESET);
    printf(YELLOW "   | (___) || (___) |   | |     | |   | |   | |   |   \\ | |   | |   | |   | |\n" RESET);
    printf(YELLOW "   |  ___  ||  ___  |   | |     ( (   ) )   | |   | (\\ \\) |   | |   | |   | |\n" RESET);
    printf(YELLOW "   | (   ) || (   ) |   | |      \\ \\_/ /    | |   | | \\   |   | |   | |   | |\n" RESET);
    printf(YELLOW "   | )   ( || )   ( |___) (___    \\   /  ___) (___| )  \\  |   | |   | (___) |\n" RESET);
    printf(YELLOW "   |/     \\||/     \\|\\_______/     \\_/   \\_______/|/    )_)   )_(   (_______)\n" RESET);
    printf("\n\n\n\n\n\n\n\n\n");
}


// stampa il logo in caso di sconfitta
static void logo_sconfitta(void){

    printf("\n\n\n");
    printf(ORANGE "                  )          (      (         (    (        )   \n" RESET);
    printf(RED "               ( /(   (      )\\ )   )\\ )      )\\ ) )\\ )  ( /(   \n" RESET);
    printf(ORANGE "               )\\())  )\\    (()/(  (()/( (   (()/((()/(  )\\())  \n" RESET);
    printf(RED "              ((_)\\((((_)(   /(_))  /(_)))\\   /(_))/(_))((_)\\   \n" RESET);
    printf("               _%s((%s_%s))\\ %s_%s )\\ (%s_%s))   (%s_%s)) ((%s_%s) (%s_%s)) (%s_%s))    ((%s_%s)  \n" RESET , ORANGE, RESET, ORANGE, RESET, ORANGE, RESET, ORANGE, RESET, ORANGE, RESET, ORANGE, RESET, ORANGE, RESET, ORANGE, RESET, ORANGE);
    printf("              | || |%s(_)%s_\\%s(_)%s|_ _|  | _ \\| __|| _ \\/ __|  / _ \\  \n" RESET, ORANGE, RESET, ORANGE, RESET);
    printf("              | __ | / _ \\   | |   |  _/| _| |   /\\__ \\ | (_) | \n");
    printf("              |_||_|/_/ \\_\\ |___|  |_|  |___||_|_\\|___/  \\___/  \n");
    printf("\n\n\n");
}


// stampa tutte le informazioni del giocatore passato
static void stampa_giocatore(struct Giocatore* GiocatoreTmp){
    printf(BOLD "\nNome:%s %s", RESET, GiocatoreTmp -> nome_giocatore);
    printf(BOLD "\nClasse:%s %s", RESET, stampa_classe_giocatore(GiocatoreTmp));
    printf(BOLD "\nPunti vita:%s %u", RESET, GiocatoreTmp -> p_vita);
    printf(BOLD "\nDadi attacco:%s %u", RESET, GiocatoreTmp -> dadi_attacco);
    printf(BOLD "\nDadi difesa:%s %u", RESET, GiocatoreTmp -> dadi_difesa);
    printf(BOLD "\nMente:%s %u", RESET, GiocatoreTmp -> mente);
    printf(BOLD "\nPotere speciale:%s %u", RESET, GiocatoreTmp -> potere_speciale);
}


// ritorna la classe del giocatore passato
static const char* stampa_classe_giocatore(struct Giocatore* GiocatoreTmp){
    switch (GiocatoreTmp -> classe){
        case barbaro: return "Barbaro";
        case nano: return "Nano";
        case elfo: return "Elfo";
        case mago: return "Mago";
        default: return "Classe sconosciuta";
    }
}


// stampa tutte le informazioni della zona passata
static void stampa_zona(struct Zona_segrete* pZonaTmp){

    printf(BOLD "\nTipo:%s %s", RESET, stampa_tipo_zona(pZonaTmp));
    printf(BOLD "\nN.:%s %u - %d", RESET, pZonaTmp -> n_stanza, n_zone);

    if(pZonaTmp -> porta == nessuna_porta){
        printf(BOLD "\nPorta:%s Nessuna Porta", RESET);
    }else{
        printf(BOLD "\nPorta:%s ...", RESET);
    }

    if(pZonaTmp -> tesoro == nessun_tesoro){
        printf(BOLD "\nTesoro:%s Nessun Tesoro", RESET);
    }else{
        printf(BOLD "\nTesoro:%s ...", RESET);
    }
}


// ritorna il tipo della zona passata
static const char* stampa_tipo_zona(struct Zona_segrete* pZonaTmp){
    switch (pZonaTmp -> zona){
        case corridoio: return "Corridoio";
        case scala: return "Scala";
        case sala_banchetto: return "Sala Banchetto";
        case magazzino: return "Magazzino";
        case giardino: return "Giardino";
        case porto_guardia: return "Porto Guardia";
        case prigione: return "Prigione";
        case cucina: return "Cucina";
        case armeria: return "Armeria";
        case tempio: return "Tempio";
        default: return "Tesoro sconosciuto";
    }
}


// ritorna il tipo_tesoro della zona passata
static const char* stampa_tipo_tesoro(struct Zona_segrete* pZonaTmp){
    switch (pZonaTmp -> tesoro){
        case nessun_tesoro: return "Nessun Tesoro";
        case veleno: return "Veleno";
        case guarigione: return "Guarigione";
        case doppia_guarigione: return "Doppia Guarigione";
        default: return "Tesoro sconosciuto";
    }
}


// ritorna il tipo_porta della zona passata
static const char* stampa_tipo_porta(struct Zona_segrete* pZonaTmp){
    switch (pZonaTmp -> porta){
        case nessuna_porta: return "Nessuna Porta";
        case porta_normale: return "Porta Normale";
        case porta_da_scassinare: return "Porta da Scassinare";
        default: return "Porta sconosciuta";
    }
}


// ritorna il nome dell'abitante passato
static const char* stampa_nome_abitante(struct Abitante_segrete* AbitanteTmp){
    switch (AbitanteTmp -> nome) {
        case Goblinus: return "Goblinus";
        case Skeletor: return "Skeletor";
        case Orkus: return "Orkus";
        case Mordrax: return "Mordrax";
        case Trollgar: return "Trollgar";
        case Zogroth: return "Zogroth";
        case Vampyro: return "Vampyro";
        case Grimfang: return "Grimfang";
        case Darkshade: return "Darkshade";
        case Wraithus: return "Wraithus";
        case Shadowlurker: return "Shadowlurker";
        case Bonecrusher: return "Bonecrusher";
        case Deathclaw: return "Deathclaw";
        case Bloodfang: return "Bloodfang";
        case Nightstalker: return "Nightstalker";
        default: return "Nome sconosciuto";
    }
}


// crea un abitante delle segrete
static struct Abitante_segrete* crea_abitante(void){

    struct Abitante_segrete* AbitanteTmp = (struct Abitante_segrete*) malloc(sizeof(struct Abitante_segrete));

    if (AbitanteTmp == NULL) {
        system("clear");

        printf(RED "\nERRORE: Allocazione della memoria per Abitante_segrete fallita, combattimento non possibile...\n" RESET);
        return NULL;
    }

    AbitanteTmp -> nome = randomize(0, 14);
    AbitanteTmp -> p_vita = randomize(3, 4);
    AbitanteTmp -> dadi_attacco = randomize(1, 3);
    AbitanteTmp -> dadi_difesa = randomize(1, 2);

    return AbitanteTmp;
}


// aggiunge il puntatore del giocatore morto all'array giocatori_morti[]
static void morte_giocatore(struct Giocatore* GiocatoreTmp){

    for (int i = 0; i < 4; i++) {
        if (giocatori_morti[i] == NULL) {  // Trova la prima posizione libera
            giocatori_morti[i] = GiocatoreTmp;
            n_giocatori_morti++;
            break;
        }
    }

    if(n_giocatori == n_giocatori_morti){  // controlla se è morto l'ultimo giocatore in partita
        gioco_in_corso = false;  // SONO MORTI TUTTI I GIOCATORI
    }
}


// controlla se il giocatore passato è presente nell'array giocatori_morti[]
static bool is_giocatore_morto(struct Giocatore* GiocatoreTmp){
    for(int i = 0; i < n_giocatori_morti; i++){
        if (giocatori_morti[i] == GiocatoreTmp){
            return true;
        }
    }
    return false;
}


// l'utente sceglie il numero dei giocatori
static void numero_giocatori(void){
    int input_valid;

    do {

        printf("\nInserisci il numero di giocatori (1-4): ");

        input_valid = scanf("%d", &n_giocatori);  // Controlla che l'input sia un numero intero valido e non una lettera o un simbolo

        svuotaBuffer();
        system("clear");

        if(input_valid != 1 || n_giocatori < 1 || n_giocatori > 4) {
            printf(RED "\nERRORE: Numero giocatori non consentito...\n" RESET);
        }
    }while(input_valid != 1 || n_giocatori < 1 || n_giocatori > 4);
}


// permette di impostare nome e classe dei giocatori
static void imposta_giocatori(void){

    struct Giocatore* GiocatoreTmp = PtrGiocatore;
    
    //creazione del giocatore i
    for(int i=0; i < n_giocatori; i++, GiocatoreTmp++){
        bool nome_incorretto;

        do{
            nome_incorretto = false;
            printf("\nInserisci il nome del %d° giocatore: ", (i+1));
            
            if(fgets(GiocatoreTmp -> nome_giocatore, 63, stdin) == NULL){

                system("clear");
                printf(RED "\nErrore nella lettura del nome. Riprova...\n" RESET);

                nome_incorretto = true;
                continue;
            }

            int len = strlen(GiocatoreTmp -> nome_giocatore);

            if ((len > 0 && (GiocatoreTmp -> nome_giocatore [len - 1]) == '\n')){
                GiocatoreTmp -> nome_giocatore [len - 1] = '\0';
                len--;
            }else{
                svuotaBuffer();
            }

            if(len == 0){
                system("clear");

                printf(RED "\nERRORE: Il nome non può essere vuoto...\n" RESET);
                nome_incorretto = true;
                continue;
            }

            //controlla se il nome è già stato utilizzato da un altro giocatore
            for(struct Giocatore* AppoggioGiocatore = PtrGiocatore; AppoggioGiocatore < GiocatoreTmp; AppoggioGiocatore++){
                if(strcmp(AppoggioGiocatore -> nome_giocatore, GiocatoreTmp -> nome_giocatore)==0){
                    
                    system("clear");
                    printf(RED "\nERRORE: Il nome che hai scritto è già stato utilizzato\n" RESET);
                    nome_incorretto = true;
                    break;
                }
            }
        }while(nome_incorretto);

        // il giocatore sceglie la propria classe
        int sceltaClasse = 0;
        do{
            printf(BOLD "\n       Personaggio %s|%s  Dadi attacco  %s|%s  Dadi difesa  %s|%s  Punti vita\n" RESET, RESET, BOLD, RESET, BOLD, RESET, BOLD);
            printf("                   |                |               |");
            printf(BOLD "\n  1)%s     Barbaro   |        3       |        2      |       8    \n",  RESET);
            printf("                   |                |               |");
            printf(BOLD "\n  2)%s       Nano    |        2       |        2      |       7    \n",  RESET);
            printf("                   |                |               |");
            printf(BOLD "\n  3)%s       Elfo    |        2       |        2      |       6    \n",  RESET);
            printf("                   |                |               |");
            printf(BOLD "\n  4)%s       Mago    |        1       |        2      |       4    \n",  RESET);
            printf("\n\n");
            printf("Scegli la classe del tuo personaggio (1-4): ");
            scanf("%d", &sceltaClasse);

            svuotaBuffer();
            system("clear");

            if(sceltaClasse<1 || sceltaClasse>4){
                printf(RED "\n\nERRORE: Scelta non consentita...\n" RESET);
            }

        }while(sceltaClasse<1 || sceltaClasse>4);

        switch (sceltaClasse){
            case 1:
                GiocatoreTmp -> classe = barbaro;
                GiocatoreTmp -> dadi_attacco = 3;
                GiocatoreTmp -> dadi_difesa = 2;
                GiocatoreTmp -> p_vita = 8;
                GiocatoreTmp -> mente = randomize(1, 2);
                GiocatoreTmp -> potere_speciale = 0;
                break;
            case 2:
                GiocatoreTmp -> classe = nano;
                GiocatoreTmp -> dadi_attacco = 2;
                GiocatoreTmp -> dadi_difesa = 2;
                GiocatoreTmp -> p_vita = 7;
                GiocatoreTmp -> mente = randomize(2, 3);
                GiocatoreTmp -> potere_speciale = 1;
                break;
            case 3:
                GiocatoreTmp -> classe = elfo;
                GiocatoreTmp -> dadi_attacco = 2;
                GiocatoreTmp -> dadi_difesa = 2;
                GiocatoreTmp -> p_vita = 6;
                GiocatoreTmp -> mente = randomize(3, 4);
                GiocatoreTmp -> potere_speciale = 1;
                break;
            case 4:
                GiocatoreTmp -> classe = mago;
                GiocatoreTmp -> dadi_attacco = 1;
                GiocatoreTmp -> dadi_difesa = 2;
                GiocatoreTmp -> p_vita = 4;
                GiocatoreTmp -> mente = randomize(4, 5);
                GiocatoreTmp -> potere_speciale = 3;
                break;
            default:
                break;
        }

        //si sceglie se scambiare i punti delle abilità
        int sceltaPunti = 0;
        system("clear");

        do{
            printf("\nQualità attuali:");
            stampa_giocatore(GiocatoreTmp);

            printf("\n\nVuoi sacrificare 1 punto mente per avere 1 punto vita, viceversa, o nulla?");
            printf(BOLD "\n1)%s -1 punti mente -> +1 punti vita", RESET);
            printf(BOLD "\n2)%s +1 punti mente -> -1 punti vita", RESET);
            printf(BOLD "\n3)%s Non fare nulla\n\n", RESET);

            scanf("%d", &sceltaPunti);

            svuotaBuffer();
            system("clear");

            switch(sceltaPunti){
                case 1:
                    if(GiocatoreTmp -> mente > 0){
                        GiocatoreTmp -> mente--;
                        GiocatoreTmp -> p_vita++;
                        printf(GREEN "\nPunti sacrificati correttamente\n" RESET);
                    }else{
                        printf(RED "\nHai esaurito i punti mente...\n" RESET);
                    }
                    break;
                case 2:
                    if(GiocatoreTmp -> p_vita > 0){
                        GiocatoreTmp -> p_vita--;
                        GiocatoreTmp -> mente++;
                        printf(GREEN "\nPunti sacrificati correttamente\n" RESET);
                    }else{
                        printf(RED "\nHai esaurito i punti vita...\n" RESET);
                    }
                    break;
                case 3:
                    break;
                default:
                    printf(RED "\nERRORE: Scelta non consentita...\n" RESET);
                    break;
            }

        }while(sceltaPunti < 1 || sceltaPunti > 3); //cosi dovrebbe dare la possibilita di sacrificare un punto e stop
    }
}


// svuota tutta la mappa eliminandola dall'heap e azzera altre variabili inerenti ad essa
static void svuota_mappa(void){

    struct Zona_segrete* pZonaTmp = pFirst;
    struct Zona_segrete* pSuccessiva;

    while (pZonaTmp != NULL) {
        pSuccessiva = pZonaTmp -> zona_successiva;
        free(pZonaTmp);
        pZonaTmp = pSuccessiva;
    }

    pFirst = NULL;
    pLast = NULL;
    n_zone = 0;
    mappa_esistente = false;
    mappa_terminata = 0;
}


// permette di impostare la mappa (generare, cancellare e stampare zone)
static void imposta_mappa(void){

    int sceltaMappa = 0;

    do{
        printf("\nScegli una delle seguenti opzioni per impostare la mappa");
        printf(BOLD "\n1)%s Genera mappa (crea 15 zone)",  RESET);
        printf(BOLD "\n2)%s Inserisci una zona",  RESET);
        printf(BOLD "\n3)%s Cancella una zona",  RESET);
        printf(BOLD "\n4)%s Stampa la mappa",  RESET);
        printf(BOLD "\n5)%s Termina impostazone mappa",  RESET);
        printf("\n\n");
        printf("Numero zone mappa: %d\n\n", n_zone);

        scanf("\n%d", &sceltaMappa);

        svuotaBuffer();
        system("clear");

        switch (sceltaMappa){
        case 1:
            genera_mappa();
            break;
        case 2:
            inserisci_zona();
            break;
        case 3:
            cancella_zona();
            break;
        case 4:
            stampa_mappa();
            break;
        case 5:
            chiudi_mappa();

            //funione in piu nel menu di creazione della mappa che sposta l'ultima zona per prima
            modifica_mappa();

            stampa_mappa(); // e la stampa

            
            break;
        default:
            printf(RED "\nERRORE: Scelta non consentita...\n" RESET);
            break;
        }

    }while(mappa_terminata == 0);
}

// crea 15 zone (sovrascrivendo le ultime 15 o aggiungendone altre)
static void genera_mappa(void){

    int sovrascriviZona = 0;

    // mappa_esistente aiuta a capire se è la prima volta che si generano 15 zone o no
    if(mappa_esistente){

        do{
            printf(BOLD "\n1)%s Genera altre 15 zone (+15)", RESET);
            printf(BOLD "\n2)%s Sovrascrivi le ultime 15 già presenti\n\n", RESET);

            scanf("%d", &sovrascriviZona);

            svuotaBuffer();

            system("clear");

            if(sovrascriviZona < 1 || sovrascriviZona > 2){
                printf(RED "\nERRORE: Scelta non consentita...\n" RESET);
            }
        }while(sovrascriviZona < 1 || sovrascriviZona > 2);


        if(sovrascriviZona == 2){  // L'utente sceglie di modificare le ultime 15 zone esistenti

            if(n_zone >= 15){
                struct Zona_segrete* pZonaTmp = pLast;

                for(int i = 0; i < 15 && pZonaTmp != NULL; i++) {

                    pZonaTmp->zona = randomize(0, 9);
                    pZonaTmp->porta = randomize(0, 2);
                    pZonaTmp->tesoro = randomize(0, 3);

                    pZonaTmp = pZonaTmp->zona_precedente;
                }

                system("clear");
                printf(GREEN "\nUltime 15 zone modificate correttamente\n" RESET);
            }else{
                system("clear");
                printf(RED "\nERRORE: Non hai almeno 15 zone da modificare...\n" RESET);
            }

        }else if(sovrascriviZona == 1){  // L'utente sceglie di crearne altre 15 nuove

            struct Zona_segrete* pZonaTmp = NULL;
            struct Zona_segrete* pZonaPrecedente = pLast;

            for(int i=0; i<15; i++){
            
                pZonaTmp = (struct Zona_segrete*) malloc(sizeof(struct Zona_segrete));

                if (pZonaTmp == NULL) {
                    printf(RED "\nERRORE: allocazione della memoria non riuscita...\n" RESET);
                    return;
                }

                if(i==14){
                    pZonaTmp -> zona_successiva = NULL;
                    pLast = pZonaTmp;
                }

                pZonaPrecedente -> zona_successiva = pZonaTmp;
                pZonaTmp -> zona_precedente = pZonaPrecedente;

                pZonaTmp -> zona = randomize(0, 9);
                pZonaTmp -> porta = randomize(0, 2);
                pZonaTmp -> tesoro = randomize(0, 3);

                pZonaPrecedente = pZonaTmp;
            }

            n_zone += 15;

            system("clear");
            printf(GREEN "\nAltre 15 zone create correttamente\n" RESET);
        }
    }else{

        // mappa_esistente == false  -  è la prima volta che si entra nella funzione
        if(pLast == NULL){

            struct Zona_segrete* pZonaTmp = NULL;
            struct Zona_segrete* pZonaPrecedente = NULL;

            for(int i=0; i<15; i++){
                
                pZonaTmp = (struct Zona_segrete*) malloc(sizeof(struct Zona_segrete));

                if (pZonaTmp == NULL) {
                    printf(RED "\nERRORE: allocazione della memoria non riuscita...\n" RESET);
                    return;
                }

                if(i==0){
                    pZonaTmp -> zona_precedente = NULL;
                    pFirst = pZonaTmp;

                }else if(i==14){
                    pZonaPrecedente -> zona_successiva = pZonaTmp;
                    pZonaTmp -> zona_precedente = pZonaPrecedente;

                    pZonaTmp -> zona_successiva = NULL;
                    pLast = pZonaTmp;
                }else{
                    pZonaPrecedente -> zona_successiva = pZonaTmp;
                    pZonaTmp -> zona_precedente = pZonaPrecedente;
                }

                pZonaTmp -> zona = randomize(0, 9);
                pZonaTmp -> porta = randomize(0, 2);
                pZonaTmp -> tesoro = randomize(0, 3);

                pZonaPrecedente = pZonaTmp;
            }

            mappa_esistente = true;

            n_zone += 15;

            system("clear");
            printf(GREEN "\nPrime 15 zone create correttamente\n" RESET);

        }else{  // si creano 15 zone dopo l'ultima esistente (probabilmente creata in inserisci_zona())

            struct Zona_segrete* pZonaTmp = NULL;
            struct Zona_segrete* pZonaPrecedente = pLast;

            for(int i=0; i<15; i++){
            
                pZonaTmp = (struct Zona_segrete*) malloc(sizeof(struct Zona_segrete));

                if (pZonaTmp == NULL) {
                    printf(RED "\nERRORE: allocazione della memoria non riuscita...\n" RESET);
                    return;
                }

                if(i==14){
                    pZonaTmp -> zona_successiva = NULL;
                    pLast = pZonaTmp;
                }

                pZonaPrecedente -> zona_successiva = pZonaTmp;
                pZonaTmp -> zona_precedente = pZonaPrecedente;

                pZonaTmp -> zona = randomize(0, 9);
                pZonaTmp -> porta = randomize(0, 2);
                pZonaTmp -> tesoro = randomize(0, 3);

                pZonaPrecedente = pZonaTmp;
            }

            mappa_esistente = true;

            n_zone += 15;

            system("clear");
            printf(GREEN "\n15 zone create correttamente dopo le ultime inserite\n" RESET);
        }
    }
}


// permette di inserire una nuova zona in una posizione a piacere tra 1 e n_zone+1
static void inserisci_zona(void){

    int punto_zona = 0;

    do{
        printf("\nIn quale posizione vuoi inserire la zona? (1-%d): ", n_zone+1);
        scanf("%d", &punto_zona);

        svuotaBuffer();
                    
        system("clear");
                    
        if(punto_zona < 1 || punto_zona > n_zone+1){
            printf(RED "\nERRORE: Posizione non consentita...\n" RESET);
        }
    }while(punto_zona < 1 || punto_zona > n_zone+1);


    struct Zona_segrete* pZonaTmpNuova = (struct Zona_segrete*) malloc(sizeof(struct Zona_segrete));
    struct Zona_segrete* pZonaTmp = NULL;


    if(n_zone > 0){

        //questo if permette di fare meno cicli for possibili, partendo da pLast piuttosto che da pFirst, o viceversa
        if(n_zone - punto_zona < punto_zona){  // parto da pLast

            pZonaTmp = pLast;

            for(int i=0; i < (n_zone-punto_zona); i++){
                pZonaTmp = pZonaTmp -> zona_precedente;
            }
        }else{  // parto da pFirst
            pZonaTmp = pFirst;

            for(int i=1; i < punto_zona; i++){
                pZonaTmp = pZonaTmp -> zona_successiva;
            }
        }

        if(punto_zona == 1){  // sto aggiungendo nella prima posizione

            pFirst = pZonaTmpNuova;
            pZonaTmpNuova -> zona_precedente = NULL;
            pZonaTmpNuova -> zona_successiva = pZonaTmp;
                
            pZonaTmp -> zona_precedente = pZonaTmpNuova;

        }else if(punto_zona > n_zone){  // sto aggiungendo nell'ultima posizione

            pZonaTmp -> zona_successiva = pZonaTmpNuova;

            pZonaTmpNuova -> zona_precedente = pZonaTmp;
            pZonaTmpNuova -> zona_successiva = NULL;
            pLast = pZonaTmpNuova;

        }else{  // sto aggiungendo in una posizione centrale

            struct Zona_segrete* pZonaPrecedente = pZonaTmp -> zona_precedente;
            pZonaPrecedente -> zona_successiva = pZonaTmpNuova;

            pZonaTmp -> zona_precedente = pZonaTmpNuova;
            
            pZonaTmpNuova -> zona_precedente = pZonaPrecedente;
            pZonaTmpNuova -> zona_successiva = pZonaTmp;
        }
            
    }else{  // questa è la prima zona da creare
            
        pZonaTmpNuova -> zona_successiva = NULL;
        pZonaTmpNuova -> zona_precedente = NULL;
        pFirst = pZonaTmpNuova;
        pLast = pZonaTmpNuova;
    }

    pZonaTmpNuova -> zona = randomize(0, 9);
    pZonaTmpNuova -> porta = randomize(0, 2);
    pZonaTmpNuova -> tesoro = randomize(0, 3);

    n_zone++;

    system("clear");
    printf(GREEN "\nZona inserita correttamente\n" RESET);
}


// permette di cancellare una zona in una posizione a piacere tra 1 e n_zone
static void cancella_zona(){

    if(n_zone>0){

        int punto_zona = 0;
                
        do{
            printf("\nQualè la zona che vuoi eliminare? (1-%d): ", n_zone);
            scanf("%d", &punto_zona);

            svuotaBuffer();
                    
            system("clear");
                        
            if(punto_zona < 1 || punto_zona > n_zone){
                printf(RED "\nERRORE: Punto non consentito...\n" RESET);
            }
        }while(punto_zona < 1 || punto_zona > n_zone);

        struct Zona_segrete* pZonaTmp = NULL;

        if(n_zone > 1){

            //questo if permette di fare meno cicli for possibili, partendo da pLast piuttosto che da pFirst, o viceversa
            if(n_zone - punto_zona < punto_zona){  //parto da pLast

                pZonaTmp = pLast;

                for(int i=0; i < (n_zone-punto_zona); i++){
                    pZonaTmp = pZonaTmp -> zona_precedente;
                }
            }else{  //parto da pFirst
                pZonaTmp = pFirst;

                for(int i=1; i < punto_zona; i++){
                    pZonaTmp = pZonaTmp -> zona_successiva;
                }
            }

            if(punto_zona == 1){  // sto eliminando nella prima posizione
                
                struct Zona_segrete* pZonaSuccessiva = pZonaTmp -> zona_successiva;

                pFirst = pZonaSuccessiva;
                pZonaSuccessiva -> zona_precedente = NULL;

            }else if(punto_zona == n_zone){  // sto eliminando nell'ultima posizione

                struct Zona_segrete* pZonaPrecedente = pZonaTmp -> zona_precedente;

                pLast = pZonaPrecedente;
                pZonaPrecedente -> zona_successiva = NULL;

            }else{  // sto eliminando in una posizione centrale

                struct Zona_segrete* pZonaSuccessiva = pZonaTmp -> zona_successiva;
                struct Zona_segrete* pZonaPrecedente = pZonaTmp -> zona_precedente;

                pZonaSuccessiva -> zona_precedente = pZonaPrecedente;
                pZonaPrecedente -> zona_successiva = pZonaSuccessiva;
            }

            free(pZonaTmp);
            pZonaTmp = NULL;
            
        }else{  // esiste una sola zona
            pZonaTmp = pFirst;

            free(pZonaTmp);
            pZonaTmp = NULL;
            pFirst = NULL;
            pLast = NULL;
        }

        n_zone--;

        printf(GREEN "\n%d° zona cancellata correttamente\n" RESET, punto_zona);
    }else{

        system("clear");
        printf(RED "\nERRORE: Devi almeno generare una zona...\n" RESET);
    }
}


// stampa tutta la mappa creata
static void stampa_mappa(void){

    if(n_zone>0){

        struct Zona_segrete* pZonaTmp = pFirst;

        printf(GREEN "+------------------+--------------------+--------------------------+\n" RESET);
        printf("| %sZona%s             | %sTesoro%s             | %sPorta%s                    |\n", GREEN, RESET, GREEN, RESET, GREEN, RESET);
        printf(GREEN "+------------------+--------------------+--------------------------+\n" RESET);
        

        for(int i=0; i<n_zone; i++){

            printf("| %-16s | %-18s | %-24s |\n",
                stampa_tipo_zona(pZonaTmp),
                stampa_tipo_tesoro(pZonaTmp),
                stampa_tipo_porta(pZonaTmp));

            pZonaTmp = pZonaTmp -> zona_successiva;
        }
        
        printf(GREEN "+------------------+--------------------+--------------------------+\n" RESET);
    
    }else{    
        printf(RED "\nERRORE: Devi almeno generare una zona...\n" RESET);
    }
}


// permette di uscire da imposta_mappa() utilizzando la variabile mappa_terminata
static void chiudi_mappa(void){
    if(n_zone >= 15){

        int scelta_utente = 0;

        do{
            printf("\nSei sicuro di voler terminare la creazione della mappa?");
            printf("\n");
            printf(GREEN "\n1) Termina mappa e torna al menu di gioco per iniziare la partita\n" RESET);
            printf(ORANGE "\n2) Annulla l'operazione e torna alla modifica della mappa\n\n" RESET);

            scanf("%d", &scelta_utente);

            svuotaBuffer();
                        
            system("clear");

            if(scelta_utente < 1 || scelta_utente > 2){
                printf(RED "\nERRORE: Scelta non consentita...\n" RESET);
            }

        }while(scelta_utente < 1 || scelta_utente > 2);

        if(scelta_utente == 1){
            mappa_terminata = 1;
            assegna_num_zone();
        }else{
            mappa_terminata = 0;
        }

    }else{

        system("clear");
        printf(RED "\nERRORE: La mappa deve essere composta da almeno 15 zone...\n" RESET);
    }
}


static void modifica_mappa(void){
    Zona_segrete* pPrima = pFirst;

    //pFirst diventerà quella che ora è l'ultima zona (pLast)
    //e pPrima diventerà la seconda zona

    if(n_zone > 1){
        pFirst = pLast;

        pLast = pLast -> zona_precedente;
        pLast -> zona_successiva = NULL;

        pFirst -> zona_successiva = pPrima;
        pFirst -> zona_precedente = NULL;

        pPrima -> zona_precedente = pFirst;
    }

}


// viene chiamata a mappa terminata e assegna ad ogni stanza un numero da 1 a n_stanze in modo ordinato
static void assegna_num_zone(void){

    Zona_segrete* pZonaTmp = pFirst;

    for(unsigned char i=1; pZonaTmp != NULL && i <= n_zone; i++){
        pZonaTmp -> n_stanza = i;  // Assegna il numero alla zona
        pZonaTmp = pZonaTmp -> zona_successiva;
    }
}


// imposta la zona di partenza a pFirst a tutti i giocatori
static void imposta_partenza_giocatori(void){
    struct Giocatore* GiocatoreTmp = PtrGiocatore;

    for(int i=0; i < n_giocatori; i++, GiocatoreTmp++){
        GiocatoreTmp -> posizione = pFirst;
    }
}


// stampa il regolamento prima di iniziare a giocare
static void regolamento(void){
    printf(BOLD "\n    ╔═══════════════════════════════════════════════════════════════╗\n" RESET);
    printf(BOLD "    ║                          REGOLAMENTO                          ║\n" RESET);
    printf(BOLD "    ╚═══════════════════════════════════════════════════════════════╝\n" RESET);

    printf("\n  Durante ogni turno, i giocatori hanno modo di avanzare o indietreggiare");
    printf("\n  nelle varie zone della mappa. Accedendo ad alcune zone, ci si può");
    printf("\n  imbattere in una porta da scassinare, superabile grazie ai punti mente");
    printf("\n  del giocatore. Se il tentativo fallisse, il giocatore dovrà affrontare le");
    printf("\n  conseguenze impreviste. In ogni stanza in cui si mette piede, si prende in");
    printf("\n  automatico il tesoro nascosto, se presente. Inoltre, è probabile incontrare");
    printf("\n  gli abitanti che vivono nelle segrete, esseri poco affidabili e spesso");
    printf("\n  difficili da superare. Il primo giocatore che riesce ad attraversare");
    printf("\n  tutta la mappa e giungere sano e salvo all'ultima zona delle segrete,");
    printf("\n  vincerà. Avanzate con cautela, combattete con astuzia, e usate il vostro");
    printf("\n  potere speciale al momento giusto!\n");

    printf("\n  Durante il gioco utilizzare il tasto %s%sinvio%s per navigare tra le diverse", BOLD, ITALIC, RESET);
    printf("\n  fasi di gioco e avanzare nei turni.\n");

    printf(BOLD "\n    ╔═══════════════════════════════════════════════════════════════╗\n" RESET);
    printf(BOLD "    ║                       BUON DIVERTIMENTO!                      ║\n" RESET);
    printf(BOLD "    ╚═══════════════════════════════════════════════════════════════╝\n" RESET);
}


// popola l'array turni[], presente in gioca(), per renderli casuali
static void scelta_turno(int* turni){

    //inserisco nell'array i numeri da 1 a n_giocatori
    for(int i=0; i < n_giocatori; i++){
        turni[i] = i + 1;
    }

    //sposto casualmente i numeri all'interno dell'array
    for(int i=1, j=0, temp=0; i < n_giocatori; i++){

        j = randomize(0, i);

        temp = turni[i];
        turni[i] = turni[j];
        turni[j] = temp;
    }
}


// permette di avanzare alla zona successiva
static void avanza(struct Giocatore* GiocatoreTmp){

    struct Zona_segrete* pZonaTmp = GiocatoreTmp -> posizione;
    pZonaTmp = pZonaTmp -> zona_successiva;


    stampa_giocatore(GiocatoreTmp);
    printf("\n");
    stampa_zona(pZonaTmp);
    printf("\n\n");
    
    getchar();
    system("clear");
    
    // si incontra una porta
    if(pZonaTmp -> porta == porta_normale || pZonaTmp -> porta == porta_da_scassinare){

        printf(MAGENTA "\n%s ha incontrato una %s ora dovrà aprirla con invio...\n\n" RESET, GiocatoreTmp -> nome_giocatore, stampa_tipo_porta(pZonaTmp));
        
        getchar();
        system("clear");

        apri_porta(GiocatoreTmp, pZonaTmp);
    }

    if(!rispedito){  // il giocatore non è stato rispedito alla prima zona

        if(scassinata){   // la porta_da_scassinare è stata aperta

            if(GiocatoreTmp -> p_vita != 0){  // se il giocatore è ancora vivo

                prendi_tesoro(GiocatoreTmp, pZonaTmp);

                getchar();
                system("clear");

                if(GiocatoreTmp -> p_vita != 0){  // controlla se il giocatore è ancora vivo dopo il possibile avvelenamento
                
                    
                    int apparizione_abitante = randomize(1, 100);  // potrebbe apparire un altro abitante delle segrete
                    
                    if(apparizione_abitante <= 33){

                        printf(ORANGE "\nE' apparso un abitante delle segrete\n" RESET);
                        incontro_abitante(GiocatoreTmp);

                        if(!scappato){  // se non è fuggito dal combattimento

                            if(GiocatoreTmp -> p_vita != 0){  // controlla se il giocatore è ancora vivo dopo il combattimento

                                if(pZonaTmp == pLast){  // se il giocatore è nell'ultima zona ha vinto
                                    gioco_in_corso = false;
                                }else{
                                    printf(GREEN "\n%s è avanzato con successo\n\n" RESET, GiocatoreTmp -> nome_giocatore);
                                    GiocatoreTmp -> posizione = pZonaTmp;
                                }
                            }else{

                                //anche se il giocatore muore, la sua posizione avanza (servirà per resoconto_finale())
                                GiocatoreTmp -> posizione = pZonaTmp;
                                morte_giocatore(GiocatoreTmp);
                            }

                        }else{  // il giocatore è fuggito (dall'abitante nella stanza) e ora indietreggia

                            scappato = false;
                            pZonaTmp = pZonaTmp -> zona_precedente;
                            GiocatoreTmp -> posizione = pZonaTmp;
                            prendi_tesoro(GiocatoreTmp, pZonaTmp);  // prende il tesoro nella stanza

                            if(GiocatoreTmp -> p_vita == 0){  // il giocatore potrebbe essere morto dopo il possibile avvelenamento
                                morte_giocatore(GiocatoreTmp);
                            }
                        }
                    }else{  // non ha incontrato nessun abitante
                        if(pZonaTmp == pLast){  // se il giocatore è nell'ultima zona ha vinto
                            gioco_in_corso = false;
                        }else{
                            printf(GREEN "\n%s è avanzato con successo\n\n" RESET, GiocatoreTmp -> nome_giocatore);
                            GiocatoreTmp -> posizione = pZonaTmp;
                        }
                    }
                }else{
                    
                    //anche se il giocatore muore, la sua posizione avanza (servirà per resoconto_finale())
                    GiocatoreTmp -> posizione = pZonaTmp;
                    morte_giocatore(GiocatoreTmp);
                }
            }else{
                morte_giocatore(GiocatoreTmp);
            }
            
        }else{  // il giocatore non è riuscito a scassinare la porta
            
            scassinata = true;  // torna true per sicurezza

            if(scappato){// il giocatore è riuscito a scappare (dall'abitante prima della porta) e ora indietreggia
                scappato = false;
                pZonaTmp = pZonaTmp -> zona_precedente -> zona_precedente;
                if(pZonaTmp == NULL){
                    pZonaTmp = pFirst;
                }
                GiocatoreTmp -> posizione = pZonaTmp;
                prendi_tesoro(GiocatoreTmp, pZonaTmp);
            }

            if(GiocatoreTmp -> p_vita == 0){  // il giocatore potrebbe essere morto per via del combattimento
                morte_giocatore(GiocatoreTmp);
            }else{
                printf(RED "\n%s non è riuscito ad avanzare...\n\n" RESET, GiocatoreTmp -> nome_giocatore);
            }
        }
        
    }else{  // il giocatore è stato rispedito alla prima zona
        rispedito = false;
    }
}


// permette di indietreggiare alla zona precedente
static void indietreggia(struct Giocatore* GiocatoreTmp){

    struct Zona_segrete* pZonaTmp = GiocatoreTmp -> posizione;
    pZonaTmp = pZonaTmp -> zona_precedente;

    stampa_giocatore(GiocatoreTmp);
    printf("\n");
    stampa_zona(pZonaTmp);
    printf("\n\n");

    getchar();
    system("clear");
    
    prendi_tesoro(GiocatoreTmp, pZonaTmp);

    getchar();
    system("clear");

    if(GiocatoreTmp -> p_vita != 0){  // controlla se il giocatore è vivo dopo il possibile avvelenamento
        
        
        int apparizione_abitante = randomize(1, 100);  // potrebbe apparire un abitante delle segrete da combattere
            
        if(apparizione_abitante <= 33){

            printf(ORANGE "\nE' apparso un abitante delle segrete\n" RESET);
            
            incontro_abitante(GiocatoreTmp);

            if(!scappato){  // se il giocatore non è scappato

                if(GiocatoreTmp -> p_vita != 0){  // se il giocatore è ancora vivo dopo il combattimento
                    printf(GREEN "\nSei indietreggiato con successo\n\n" RESET);
                    GiocatoreTmp -> posizione = pZonaTmp;
                }else{
                    //anche se il giocatore è morto, sarà morto nella stanza nel quale è indietreggiato
                    GiocatoreTmp -> posizione = pZonaTmp;
                }
            }else{  // il giocatore è riuscito a scappare e ora torna alla zona in cui era prima

                pZonaTmp = pZonaTmp -> zona_successiva;
                GiocatoreTmp -> posizione = pZonaTmp;
                scappato = false;
                prendi_tesoro(GiocatoreTmp, pZonaTmp);
                if(GiocatoreTmp -> p_vita == 0){
                    morte_giocatore(GiocatoreTmp);
                }
            }
        }else{  // non ha incontrato nessun abitante
            printf(GREEN "\nSei indietreggiato con successo\n\n" RESET);
            GiocatoreTmp -> posizione = pZonaTmp;
        }
    }else{
        //anche se il giocatore è morto, sarà morto nella stanza nel quale è indietreggiato
        GiocatoreTmp -> posizione = pZonaTmp;
    }
}


// gestisce l'apertura della porta e i possibili inconvenienti
static void apri_porta(struct Giocatore* GiocatoreTmp, Zona_segrete* pZonaTmp){

    if(pZonaTmp -> porta == porta_da_scassinare){
        
        int tiro_dado = randomize(1, 6);

        printf(MAGENTA "\nTirando i dadi è uscito il numero %d\n" RESET, tiro_dado);

        if(tiro_dado > GiocatoreTmp -> mente){
            int num_casuale = randomize(1, 10);

            printf("\nI punti mente di %s sono minori (%u), quindi ora giocherà con la sorte...\n", GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> mente);
            getchar();

            if(num_casuale <= 1){  // 10% - si rinizia il percorso dalla prima stanza delle segrete

                GiocatoreTmp -> posizione = pFirst;
                printf(RED "\n%s è stato rispedito alla prima stanza delle segrete\n" RESET, GiocatoreTmp -> nome_giocatore);
                prendi_tesoro(GiocatoreTmp, pFirst);

                rispedito = true;
                scassinata = false;

            }else if(num_casuale <= 6){  // 50% - si perde un punto vita

                if(GiocatoreTmp -> p_vita > 0){
                    GiocatoreTmp -> p_vita--;
                }
                
                printf(RED "%s ha perso un punto vita\n" RESET, GiocatoreTmp -> nome_giocatore);

                scassinata = false;

            }else{  // 40% - appare un abitante delle segrete che si deve combattere
                
                printf(ORANGE "\nE' apparso un abitante delle segrete\n" RESET);
                incontro_abitante(GiocatoreTmp);

                scassinata = false;
            }

            // la porta rimane da scassinare e il giocatore resta nella stanza in cui si trovava

        }else{  // la porta è stata scassinata e diventa una porta normale
            
            scassinata = true;
            printf(GREEN "\nI punti mente di %s sono maggiori (%u) ed è riuscito a scassinare la porta\n" RESET, GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> mente);
            pZonaTmp -> porta = porta_normale;
        }

    }else{  // la porta è normale e non va scassinata

        int apertura_porta = 0;

        do{
            printf(BOLD "\n1)%s Apri la porta\n\n",  RESET);
            scanf("%d", &apertura_porta);

            svuotaBuffer();

            system("clear");

            if(apertura_porta != 1){
                printf(RED "\nERRORE: Scelta non consentita...\n" RESET);
            }

        }while(apertura_porta != 1);
    }
}


// gestisce l'interazione tra il giocatore e il tesoro
static void prendi_tesoro(struct Giocatore* GiocatoreTmp, Zona_segrete* pZonaTmp){

    printf(MAGENTA "\nIl tesoro presente nella stanza è %s\n" RESET, stampa_tipo_tesoro(pZonaTmp));

    switch(pZonaTmp -> tesoro){
        case nessun_tesoro:  // non accade nulla

            printf(MAGENTA "\nUscirai dalla stanza a mani vuote...\n\n" RESET);
            break;
        case veleno:  // -2 punti vita

            printf(RED "\n%s perde 2 punti vita\n\n" RESET, GiocatoreTmp -> nome_giocatore);

            if(GiocatoreTmp -> p_vita > 2){  // il giocatore sopravvive al veleno
                GiocatoreTmp -> p_vita -= 2;
            }else{                           // oppure il veleno uccide il giocatore
                GiocatoreTmp -> p_vita = 0;
                printf(RED "\n%s è morto di avvelenamento...\n\n" RESET, GiocatoreTmp -> nome_giocatore);
            }
            break;
        case guarigione:  // +1 punti vita

            printf(GREEN "\n%s guadagna 1 punto vita\n\n" RESET, GiocatoreTmp -> nome_giocatore);
            GiocatoreTmp -> p_vita += 1;
            break;
        case doppia_guarigione:  // +2 punti vita

            printf(GREEN "\n%s guadagna 2 punti vita\n\n" RESET, GiocatoreTmp -> nome_giocatore);
            GiocatoreTmp -> p_vita += 2;
            break;
        default:
            break;
    }
}


// gestisce l'incontro con l'abitante
static void incontro_abitante(struct Giocatore* GiocatoreTmp){

    struct Abitante_segrete* AbitanteTmp = crea_abitante();

    if(AbitanteTmp != NULL){

        int ris_fuga = 0;

        do{
            int scelta_fuga = 0;

            do{
                printf(BOLD "\n1)%s Combatti\n", RESET);
                printf(BOLD "\n2)%s Scappa\n", RESET);
                printf(BOLD "\n3)%s Usa potere speciale\n\n", RESET);

                scanf("%d", &scelta_fuga);

                svuotaBuffer();

                system("clear");

                if(scelta_fuga < 1 || scelta_fuga > 3){
                    printf(RED "\nERRORE: Scelta non consentita...\n" RESET);
                }

            }while(scelta_fuga < 1 || scelta_fuga > 3);

            if(scelta_fuga == 1){  // il giocatore sceglie di combattere
                combatti(GiocatoreTmp, AbitanteTmp);
                ris_fuga = 1;
                break;

            }else if(scelta_fuga == 2){  // il giocatore sceglie di scappare
                ris_fuga = scappa(GiocatoreTmp, AbitanteTmp);

                if(GiocatoreTmp -> p_vita == 0){  // interrompo il ciclo se il giocatore muore
                    ris_fuga = 1;
                    break;
                }
            }else if(scelta_fuga == 3){  // il giocatore sceglie di usare il potere speciale

                if(GiocatoreTmp -> potere_speciale > 0){
                    gioca_potere_speciale(GiocatoreTmp, AbitanteTmp);
                    ris_fuga = 1;
                    break;

                }else{
                    printf(RED "\nERRORE: Non hai abbastanza potere speciale rimasto...\n" RESET);
                    ris_fuga = 0;
                }
            }

        }while(ris_fuga == 0);
    }
}


// gestisce la fuga del giocatore dal combattimento con l'abitante
static int scappa(struct Giocatore* GiocatoreTmp, Abitante_segrete* AbitanteTmp){

    system("clear");

    int tiro_dado = randomize(1, 6);

    printf(MAGENTA "\nTirando i dadi è uscito il numero %d\n" RESET, tiro_dado);

    if(tiro_dado > GiocatoreTmp -> mente){  // il giocatore non è riuscito a scappare

        printf("\nI punti mente di %s sono minori (%u)...\n", GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> mente);

        getchar();

        printf(RED "\n%s subirà un attacco e potrà utilizzare solo la metà dei dadi difesa\n" RESET, GiocatoreTmp -> nome_giocatore);

        int risultati_giocatore[3] = {0};
        int risultati_abitante[3] = {0};

        getchar();
        system("clear");
                
        if(AbitanteTmp -> dadi_attacco > 1){
            printf(ORANGE "\nL'abitante %s lancia %u dadi attacco\n" RESET, stampa_nome_abitante(AbitanteTmp), AbitanteTmp -> dadi_attacco);
        }else{
            printf(ORANGE "\nL'abitante %s lancia %u dado attacco\n" RESET, stampa_nome_abitante(AbitanteTmp), AbitanteTmp -> dadi_attacco);
        }
        lancia_dadi(AbitanteTmp -> dadi_attacco, risultati_abitante);

        int dadi_difesa = GiocatoreTmp -> dadi_difesa / 2;
        
        printf("\n%s lancia %u dado difesa (dadi dimezzati)\n", GiocatoreTmp -> nome_giocatore, dadi_difesa);
        
        lancia_dadi(dadi_difesa, risultati_giocatore);

        getchar();
        system("clear");

        if(risultati_abitante[0] == 0){  // l'abitante non puo togliere vita
            
            printf(GREEN "\nL'attacco dell'abitante %s è fallito... (nessun teschio)\n" RESET, stampa_nome_abitante(AbitanteTmp));

        }else{  // l'abitante puo togliere vita (teschi > 0)
            printf(ORANGE "\nL'attacco dell'abitante %s è riuscito (teschi: %d)\n" RESET, stampa_nome_abitante(AbitanteTmp), risultati_abitante[0]);
            
            printf(ORANGE "\nScudi bianchi di %s: %d\n\n" RESET, GiocatoreTmp -> nome_giocatore, risultati_giocatore[1]);
            
            if(risultati_giocatore[1] < risultati_abitante[0]){  // il giocatore subisce colpi
                int danno = risultati_abitante[0] - risultati_giocatore[1];

                getchar();
                system("clear");

                if(GiocatoreTmp -> p_vita > danno){  // il giocatore sopravvive ai colpi
                    GiocatoreTmp -> p_vita -= danno;
                    if(danno > 1){
                        printf(RED "\n%s subisce %d colpi\n" RESET, GiocatoreTmp -> nome_giocatore, danno);
                    }else{
                        printf(RED "\n%s subisce %d colpo\n" RESET, GiocatoreTmp -> nome_giocatore, danno);
                    }
                }else{
                    printf(RED "\n%s è morto\n" RESET, GiocatoreTmp -> nome_giocatore);
                    GiocatoreTmp -> p_vita = 0;
                }
            }else{  // il giocatore non subisce colpi (ha piu scudi dei colpi inflitti)
                printf(GREEN "\n%s si è parato da tutti i colpi...\n" RESET, GiocatoreTmp -> nome_giocatore);
            }
        }

        printf("\n------------------------------------------------\n");
        printf("| " GREEN "Giocatore" RESET ": %-15s | " BOLD "Punti Vita" RESET ": " GREEN "%3d" RESET " |\n", GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> p_vita);
        printf("------------------------------------------------\n\n");

        getchar();
        system("clear");

        return 0;
    }else{  // il giocatore indietreggia di zona e passa il turno a quello dopo

        printf(GREEN "\n%s è riuscito a scappare e ora indietreggia" RESET, GiocatoreTmp -> nome_giocatore);
        scappato = true;
        return 1;
    }
}


// gestisce il combattimento tra il giocatore e l'abitante
static void combatti(struct Giocatore* GiocatoreTmp, Abitante_segrete* AbitanteTmp){

    system("clear");
    printf(MAGENTA "\nCombattimento:\n" RESET);

    while(GiocatoreTmp -> p_vita > 0 && AbitanteTmp -> p_vita > 0){

        printf(MAGENTA "\nInizia il turno. Lancia i dadi premendo invio: " RESET);
        getchar();
        system("clear");

        int tiro_giocatore = randomize(1, 6);
        int tiro_abitante = randomize(1, 6);

        printf(BOLD "\nRisultati ottenuti:" RESET);
        printf(BOLD "\n\n%s:%s %d", GiocatoreTmp -> nome_giocatore, RESET, tiro_giocatore);
        printf(BOLD "\n\nAbitante %s:%s %d\n\n", stampa_nome_abitante(AbitanteTmp), RESET, tiro_abitante);

        getchar();

        if(tiro_giocatore >= tiro_abitante){  // il giocatore attacca e l'abitante si difende

            printf(GREEN "\nInizia il turno attaccando %s\n\n" RESET, GiocatoreTmp -> nome_giocatore);

            attacco_giocatore(GiocatoreTmp, AbitanteTmp);

            if(AbitanteTmp -> p_vita > 0){

                printf(GREEN "\nOra tocca all'abitante %s attaccare\n\n" RESET, stampa_nome_abitante(AbitanteTmp));
                difesa_giocatore(GiocatoreTmp, AbitanteTmp);
            }

        }else{  // l'abitante attacca e il giocatore si difende

            printf(ORANGE "\nInizia il turno attaccando l'abitante %s\n\n" RESET, stampa_nome_abitante(AbitanteTmp));

            difesa_giocatore(GiocatoreTmp, AbitanteTmp);

            if(GiocatoreTmp -> p_vita > 0){

                printf(GREEN "\nOra tocca al giocatore %s difendersi\n\n" RESET, GiocatoreTmp -> nome_giocatore);
                attacco_giocatore(GiocatoreTmp, AbitanteTmp);
            }
        }
    }
}


// gestisce la situazione in cui il giocatore attacca l'abitante
static void attacco_giocatore(struct Giocatore* GiocatoreTmp, Abitante_segrete* AbitanteTmp){

    getchar();
    system("clear");

    int risultati_giocatore[3] = {0};
    int risultati_abitante[3] = {0};

    if(GiocatoreTmp -> dadi_attacco > 1){
        printf("\n%s lancia %u dadi attacco\n", GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> dadi_attacco);
    }else{
        printf("\n%s lancia %u dado attacco\n", GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> dadi_attacco);
    }
    lancia_dadi(GiocatoreTmp -> dadi_attacco, risultati_giocatore);

    if(AbitanteTmp -> dadi_difesa > 1){
        printf("\nL'abitante %s lancia %u dadi difesa\n\n", stampa_nome_abitante(AbitanteTmp), AbitanteTmp -> dadi_difesa);
    }else{
        printf("\nL'abitante %s lancia %u dado difesa\n\n", stampa_nome_abitante(AbitanteTmp), AbitanteTmp -> dadi_difesa);
    }
    lancia_dadi(AbitanteTmp -> dadi_difesa, risultati_abitante);

    getchar();
    system("clear");

    if(risultati_giocatore[0] == 0){  // il giocatore non puo togliere vita (0 teschi)
        printf(RED "\n%s il tuo attacco è fallito... (nessun teschio)\n" RESET, GiocatoreTmp -> nome_giocatore);

    }else{  // il giocatore puo togliere vita (teschi > 0)

        printf(GREEN "\n%s il tuo attacco è riuscito (teschi: %d)\n" RESET, GiocatoreTmp -> nome_giocatore, risultati_giocatore[0]);
        printf(ORANGE "\nScudi neri dell'abitante %s: %d\n\n" RESET, stampa_nome_abitante(AbitanteTmp), risultati_abitante[2]);

        getchar();
        system("clear");

        if(risultati_abitante[2] < risultati_giocatore[0]){  // l'abitante subisce colpi
            int danno = risultati_giocatore[0] - risultati_abitante[2];

            if(AbitanteTmp -> p_vita > danno){  // l'abitante sopravvive ai colpi
                AbitanteTmp -> p_vita -= danno;
                if(danno > 1){
                    printf(GREEN "\nL'abitante %s subisce %d colpi\n" RESET, stampa_nome_abitante(AbitanteTmp), danno);
                }else{
                    printf(GREEN "\nL'abitante %s subisce %d colpo\n" RESET, stampa_nome_abitante(AbitanteTmp), danno);
                }
                    
            }else{
                printf(GREEN "\nL'abitante %s è morto\n" RESET, stampa_nome_abitante(AbitanteTmp));
                AbitanteTmp -> p_vita = 0;
            }
        }else{  // l'abitante non subisce colpi (ha piu scudi dei colpi inflitti)
            printf("\nL'abitante %s si è parato da tutti i colpi...\n", stampa_nome_abitante(AbitanteTmp));
        }
    }

    printf("\n------------------------------------------------\n");
    printf("| " GREEN "Giocatore" RESET ": %-15s | " BOLD "Punti Vita" RESET ": " GREEN "%3u" RESET " |\n", GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> p_vita);
    printf("| " ORANGE "Abitante " RESET ": %-15s | " BOLD "Punti Vita" RESET ": " ORANGE "%3u" RESET " |\n", stampa_nome_abitante(AbitanteTmp), AbitanteTmp -> p_vita);
    printf("------------------------------------------------\n\n");

    getchar();
    system("clear");
}


// gestisce la situazione in cui l'abitante attacca il giocatore
static void difesa_giocatore(struct Giocatore* GiocatoreTmp, Abitante_segrete* AbitanteTmp){

    getchar();
    system("clear");

    int risultati_giocatore[3] = {0};
    int risultati_abitante[3] = {0};

    if(AbitanteTmp -> dadi_attacco > 1){
        printf(ORANGE "\nL'abitante %s lancia %u dadi attacco\n" RESET, stampa_nome_abitante(AbitanteTmp), AbitanteTmp -> dadi_attacco);
    }else{
        printf(ORANGE "\nL'abitante %s lancia %u dado attacco\n" RESET, stampa_nome_abitante(AbitanteTmp), AbitanteTmp -> dadi_attacco);
    }
    
    lancia_dadi(AbitanteTmp -> dadi_attacco, risultati_abitante);

    if(GiocatoreTmp -> dadi_difesa > 1){
        printf("\n%s lancia %u dadi difesa\n\n", GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> dadi_difesa);
    }else{
        printf("\n%s lancia %u dado difesa\n\n", GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> dadi_difesa);
    }
    lancia_dadi(GiocatoreTmp -> dadi_difesa, risultati_giocatore);

    getchar();
    system("clear");

    if(risultati_abitante[0] == 0){  // l'abitante non puo togliere vita (0 teschi)
        printf(GREEN "\nL'attacco dell'abitante %s è fallito... (nessun teschio)\n" RESET, stampa_nome_abitante(AbitanteTmp));

    }else{  // l'abitante puo togliere vita (teschi > 0)
        printf(ORANGE "\nL'attacco dell'abitante %s è riuscito (teschi: %d)\n" RESET, stampa_nome_abitante(AbitanteTmp), risultati_abitante[0]);
        printf(GREEN "\nScudi bianchi di %s: %d\n\n" RESET, GiocatoreTmp -> nome_giocatore, risultati_giocatore[1]);

        getchar();
        system("clear");

        if(risultati_giocatore[1] < risultati_abitante[0]){  // il giocatore subisce colpi
            int danno = risultati_abitante[0] - risultati_giocatore[1];

            if(GiocatoreTmp -> p_vita > danno){  // il giocatore sopravvive ai colpi
                GiocatoreTmp -> p_vita -= danno;
                if(danno > 1){
                    printf(RED "\n%s subisce %d colpi\n" RESET, GiocatoreTmp -> nome_giocatore, danno);
                }else{
                    printf(RED "\n%s subisce %d colpo\n" RESET, GiocatoreTmp -> nome_giocatore, danno);
                }
            }else{
                printf(RED "\n%s è morto\n" RESET, GiocatoreTmp -> nome_giocatore);
                GiocatoreTmp -> p_vita = 0;
            }
        }else{  // il giocatore non subisce colpi (ha piu scudi dei colpi inflitti)
            printf(GREEN "\n%s si è parato da tutti i colpi...\n" RESET, GiocatoreTmp -> nome_giocatore);
        }
    }

    printf("\n------------------------------------------------\n");
    printf("| " GREEN "Giocatore" RESET ": %-15s | " BOLD "Punti Vita" RESET ": " GREEN "%3u" RESET " |\n", GiocatoreTmp -> nome_giocatore, GiocatoreTmp -> p_vita);
    printf("| " ORANGE "Abitante " RESET ": %-15s | " BOLD "Punti Vita" RESET ": " ORANGE "%3u" RESET " |\n", stampa_nome_abitante(AbitanteTmp), AbitanteTmp -> p_vita);
    printf("------------------------------------------------\n\n");

    getchar();
    system("clear");
}


// il giocatore utilizza il potere speciale
static void gioca_potere_speciale(struct Giocatore* GiocatoreTmp, Abitante_segrete* AbitanteTmp){

    printf(GREEN "\nComplimenti %s hai ucciso l'abitante %s delle segrete con il potere speciale\n" RESET, GiocatoreTmp -> nome_giocatore, stampa_nome_abitante(AbitanteTmp));
    GiocatoreTmp -> potere_speciale -= 1;
}


// è di supporto per il combattimento, "lancia i dadi" e inserisce i valori in risultati[]
static void lancia_dadi(int n_dadi, int* risultati){

    for(int i=0; i<n_dadi; i++){
        int risultato = randomize(1, 6);

        if(risultato <= 3) {  
            risultati[0]++;         // +1 teschio
        }else if(risultato <= 5) {
            risultati[1]++;         // +1 scudo bianco
        }else{
            risultati[2]++;         // +1 scudo nero
        }
    }
}


// stampa il resoconto della partita, i giocatori morti, quelli ancora vivi, e forse il vincitore
static void resoconto_finale(struct Giocatore* GiocatoreTmp){

    if(GiocatoreTmp -> p_vita != 0){  // cè un vincitore

        logo_vittoria();

        getchar();
        system("clear");

        printf(YELLOW "\nVINCITORE:%s %s\n", RESET, GiocatoreTmp -> nome_giocatore);
        getchar();
        
        // cerca i giocatori ancora vivi

        printf(ORANGE "\nGIOCATORI RIMASTI IN VITA:\n" RESET);
        getchar();

        bool in_partita = false;
        int j=0;
        for(int i = 0; i < n_giocatori; i++){
            if(PtrGiocatore[i].p_vita != 0 && &PtrGiocatore[i] != GiocatoreTmp){
                j++;
                printf(ORANGE "\n%d°) %s (%s) - posizione: %s - %u p.v. rimasti" RESET, j, PtrGiocatore[i].nome_giocatore, stampa_classe_giocatore(&PtrGiocatore[i]), stampa_tipo_zona(PtrGiocatore[i].posizione), PtrGiocatore[i].p_vita);
                getchar();
                in_partita = true;
            }
        }

        if(!in_partita){
            printf(ORANGE "\nNessun giocatore è rimasto in vita...\n" RESET);
            getchar();
        }

        // stampa i giocatori morti

        printf(RED "\n\nGIOCATORI MORTI:\n" RESET);
        getchar();

        if(n_giocatori_morti > 0){
            for(int i=0; i<n_giocatori_morti; i++){
                printf(RED "\n%d°) %s (%s) - morto in %s" RESET, i+1, giocatori_morti[i] -> nome_giocatore, stampa_classe_giocatore(giocatori_morti[i]), stampa_tipo_zona(giocatori_morti[i] -> posizione));
                getchar();
            }
        }else{
            printf(RED "\nNessun giocatore è morto...\n" RESET);
            getchar();
        }

    }else{  // sono tutti morti

        logo_sconfitta();

        getchar();
        system("clear");

        printf(RED "\nNESSUN VINCITORE...\n" RESET);
        getchar();

        printf(RED "\nGiocatori morti:\n" RESET);
        getchar();

        // stampa i giocatori morti

        for(int i=0; i<n_giocatori; i++){
            printf(RED "\n%d°) %s (%s) - morto in %s" RESET, i+1, giocatori_morti[i] -> nome_giocatore, stampa_classe_giocatore(giocatori_morti[i]), stampa_tipo_zona(giocatori_morti[i] -> posizione));
            getchar();
        }
    }

    printf("\n\n\n\n\nPremi invio per tornare al menu iniziale: ");

    getchar();
    system("clear");
}


// chiama tutte le funzioni riguardo l'impostazione del gioco
void imposta_gioco(void){

    azzera_valori();

    numero_giocatori();

    //crea i giocatori e passa il puntatore del primo a PtrGiocatore
    PtrGiocatore = (struct Giocatore*) calloc(n_giocatori, sizeof(struct Giocatore));

    system("clear");

    imposta_giocatori();

    svuota_mappa();  // resetta tutta la mappa e libera la memoria

    imposta_mappa();
    
    imposta_partenza_giocatori();  // imposto la posizione di partenza dei giocatori
}


// chiama le funzioni che hanno a che fare con la partita
void gioca(void){

    regolamento();  // stampa il regolamento

    getchar();
    system("clear");

    if(PtrGiocatore != NULL && mappa_terminata == 1){
        do{  // ogni ciclo è un turno

            printf(BOLD "\nInizia il turno\n" RESET);
                
            int turni[n_giocatori];
                
            scelta_turno(turni);

            for(int i=0; i<n_giocatori; i++){
                
                if(is_giocatore_morto(&PtrGiocatore[turni[i]-1])){  // il giocatore è morto e non giocherà il turno
                    continue;
                }

                int scelta_avanzamento = 0;
                bool scelta_valida = false;

                do{
                    printf("\n%s vuoi:\n", PtrGiocatore[turni[i]-1].nome_giocatore);
                    printf(BOLD "\n1)%s Avanzare", RESET);
                    printf(BOLD "\n2)%s Indietreggiare\n\n", RESET);
                    scanf("%d", &scelta_avanzamento);

                    svuotaBuffer(); 

                    system("clear");

                    // nel caso il giocatore che si trova in prima zona voglia indietreggiare
                    if(scelta_avanzamento == 2 && PtrGiocatore[turni[i]-1].posizione == pFirst){
                        printf(RED "\nERRORE: Non puoi indietreggiare, sei ancora alla prima zona...\n" RESET);

                    }else if(scelta_avanzamento < 1 || scelta_avanzamento > 2){
                        printf(RED "\nERRORE: Scelta non consentita...\n" RESET);

                    }else{
                        scelta_valida = true;
                    }
                     

                }while(!scelta_valida);

                if(scelta_avanzamento == 1){
                    avanza(&PtrGiocatore[turni[i]-1]);
                }else if(scelta_avanzamento == 2){
                    indietreggia(&PtrGiocatore[turni[i]-1]);
                }

                getchar();
                system("clear");

                if(!gioco_in_corso){  // se il gioco è stato interrotto (per la morte di tutti i giocatori, o per un vincitore)

                    resoconto_finale(&PtrGiocatore[turni[i]-1]);
                    break;
                }
            }

        }while(gioco_in_corso);
    }
}


// permette di chiudere l'intero gioco
int termina_gioco(void){

    int scelta_chiusura = 0;

    do{
        printf("\nSei sicuro di voler uscire dal gioco?\n");
        printf(RED "\n1) Esci dal gioco" RESET);
        printf(YELLOW "\n2) Annulla\n\n" RESET);
        scanf("%d", &scelta_chiusura);

        svuotaBuffer();

        system("clear");

        if(scelta_chiusura < 1 || scelta_chiusura > 2){
            printf(RED "\nERRORE: Scelta non consentita...\n" RESET);
        }

    }while(scelta_chiusura < 1 || scelta_chiusura > 2);

    if(scelta_chiusura == 1){
        return 1;  // l'utente vuole uscire
    } else {
        return 0;  // l'utente vuole tornare al menu
    }
}
