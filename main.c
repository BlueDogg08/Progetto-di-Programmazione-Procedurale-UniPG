#include "gamelib.h"

int main(){

    int scelta = 0;
    bool impostato = false;  // è true se il gioco è stato completamente impostato
    int input_valid = 0;  // aiuta l'inserimento corretto della scelta

    time_t t;
    srand((unsigned) time(&t));

    system("clear");
    stampa_titolo();

    printf("\n\nPremi invio per continuare: ");

    getchar();
    system("clear");

    do{
        printf(BOLD "1)%s Imposta gioco\n",  RESET);
        printf(BOLD "2)%s Gioca\n",  RESET);
        printf(BOLD "3)%s Termina gioco\n\n",  RESET);
        
        input_valid = scanf("%d", &scelta);

        svuotaBuffer();

        if(input_valid != 1){
            scelta = 0;
        }

        system("clear");

        switch(scelta){
            case 1:
                system("clear");
                imposta_gioco();
                impostato = true;
                break;
            case 2:
                system("clear");
                if(impostato){
                    gioca();
                }else{
                    printf(RED "\nERRORE: Imposta il gioco prima di iniziare...\n\n" RESET);
                }
                impostato = false;  // quando finisce la partita bisogna reimpostare il gioco
                break;
            case 3:
                system("clear");
                if(termina_gioco() == 0){  // l'utente non vuole terminare il gioco
                    scelta = 0;
                }

                break;
            default:
                printf(RED "\nERRORE: Inserisci una scelta valida (1, 2 o 3)...\n\n" RESET);
                break;
        }

    }while(scelta!=3);
}
