/**
*
*               tictactoe.c
*       19 / 06 / 2013 | 20 / 06 / 2013
*
**/

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <time.h> /// Pour le hasard et pour l'attente
#include <fmodex/fmod.h>
#include "tictactoe.h"

void initialiserEcran()
{
    fin_Jeu = FAUX;
    gagne = FAUX;
    gagnant = JOUEUR;
    Joueur = 0;
    Ordinateur = 0;
    ecran = NULL;
    xp = NULL;
    op = NULL;
    srand(time(NULL));
    police = NULL;
    couleur.b = 0;
    couleur.g = 0;
    couleur.r = 0;
    texte = NULL;

    FMOD_System_Create(&sys);
    FMOD_System_Init(sys,2,FMOD_INIT_NORMAL,NULL);
    resultat = FMOD_System_CreateSound(sys,"Computer_Mouse.aif",FMOD_CREATESAMPLE,0,&son);
    if (resultat != FMOD_OK)
    {
        fprintf (stderr," Impossible de charger le(s) fichier(s) son \n");
        exit (EXIT_FAILURE);
    }

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr," ERREUR : SDL_init(SDL_INIT_VIDEO) == -1 \n");
        exit(EXIT_FAILURE);
    }
    ecran = SDL_SetVideoMode(3 * TAILLE_CASSE,3 * TAILLE_CASSE,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (ecran == NULL)
    {
        fprintf(stderr," ERREUR : ecran == NULL \n");
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption("Tic-Tac-Toe",NULL); /// titre
    SDL_WM_SetIcon(SDL_LoadBMP("icone.bmp"), NULL); /// icone
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));    /// ecran blanc
    SDL_Flip(ecran);
}

void initialiserMatrice()
{
    int i = 0, j = 0;

    for (i = 0;i < 3;i++)
        for (j = 0;j < 3;j++)
        T[i][j] = VIDE;
}

void choisirXO()
{
    SDL_Event event;

    TTF_Init();
    police = TTF_OpenFont("angelina.ttf",120);
    xp = TTF_RenderText_Blended(police,"X",couleur);
    op = TTF_RenderText_Blended(police,"O",couleur);
    texte = TTF_RenderText_Blended(police," X/O ? ",couleur);

    position.x = (ecran->w / 2) - (texte->w / 2);
    position.y = (ecran->h / 2) - (texte->h / 2);
    SDL_BlitSurface(texte,NULL,ecran,&position);
    /**
    position.x = 0;
    position.y = 0;
    SDL_BlitSurface(x,NULL,ecran,&position);
    position.x = ecran->w - op->w;
    position.y = 0;
    SDL_BlitSurface(op,NULL,ecran,&position);
    **/
    SDL_Flip(ecran);

    while (1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        exit(EXIT_SUCCESS);
                    break;
                    case SDLK_x:
                        Joueur = X;
                        Ordinateur = O;
                        return;
                    break;
                    case SDLK_o:
                        Joueur = O;
                        Ordinateur = X;
                        return;
                    break;
                    default: break;
                }
            break;
            default: break;
        }
    }
}

void jouer()
{
    while (VRAI)
    {
        jouerJoueur();
        effacerEcran();
        afficherEcran();
        verifierJeu();
            if (gagne == VRAI || fin_Jeu == VRAI) break;
        jouerOrdinateur();
        effacerEcran();
        afficherEcran();
        verifierJeu();
            if (gagne == VRAI || fin_Jeu == VRAI) break;
    }
    effacerEcran();
    police = TTF_OpenFont("angelina.ttf",40);
    if (gagne == VRAI)
    {
        if (gagnant == JOUEUR)
        {
            // Le joueur a gagné
            texte = TTF_RenderText_Blended(police," Vous avez gagné ",couleur);
            position.x = (ecran->w / 2) - (texte->w / 2);
            position.y = (ecran->h / 2) - (texte->h / 2);
            SDL_BlitSurface(texte,NULL,ecran,&position);
            resultat = FMOD_System_CreateSound(sys,"Long_Applause.mp3",FMOD_CREATESAMPLE,0,&son);
            if (resultat != FMOD_OK)
            {
                fprintf (stderr," Impossible de charger le(s) fichier(s) son \n");
                exit (EXIT_FAILURE);
            }
            FMOD_System_PlaySound(sys,FMOD_CHANNEL_FREE,son,0,NULL);
        }
        else
        {
            // L'ordinateur a gagné
            texte = TTF_RenderText_Blended(police," L'ordinateur a gagné ",couleur);
            position.x = (ecran->w / 2) - (texte->w / 2);
            position.y = (ecran->h / 2) - (texte->h / 2);
            SDL_BlitSurface(texte,NULL,ecran,&position);
            resultat = FMOD_System_CreateSound(sys,"Boing_Cartoonish.mp3",FMOD_CREATESAMPLE,0,&son);
            if (resultat != FMOD_OK)
            {
                fprintf (stderr," Impossible de charger le(s) fichier(s) son \n");
                exit (EXIT_FAILURE);
            }
            FMOD_System_PlaySound(sys,FMOD_CHANNEL_FREE,son,0,NULL);

        }
    }
    else
    {
            // Egalité
            texte = TTF_RenderText_Blended(police," Egalité ",couleur);
            position.x = (ecran->w / 2) - (texte->w / 2);
            position.y = (ecran->h / 2) - (texte->h / 2);
            SDL_BlitSurface(texte,NULL,ecran,&position);
            resultat = FMOD_System_CreateSound(sys,"DJ_Scratching.mp3",FMOD_CREATESAMPLE,0,&son);
            if (resultat != FMOD_OK)
            {
                fprintf (stderr," Impossible de charger le(s) fichier(s) son \n");
                exit (EXIT_FAILURE);
            }
            FMOD_System_PlaySound(sys,FMOD_CHANNEL_FREE,son,0,NULL);

    }
    SDL_Flip(ecran);
    pause();
}

void jouerJoueur()
{
    SDL_Event event;
    booleen continuer = VRAI;
    int x = 0, y = 0;

    while (continuer == VRAI)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                exit(EXIT_FAILURE);
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        exit(EXIT_FAILURE);
                    break;
                    default: break;
                }
            break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        x = event.button.x / TAILLE_CASSE;
                        y = event.button.y / TAILLE_CASSE;
                        if (T[y][x] == VIDE)
                        {
                            T[y][x] = Joueur;
                            continuer = FAUX;
                            resultat = FMOD_System_CreateSound(sys,"Computer_Mouse.aif",FMOD_CREATESAMPLE,0,&son);
                            if (resultat != FMOD_OK)
                            {
                                fprintf (stderr," Impossible de charger le(s) fichier(s) son \n");
                                exit (EXIT_FAILURE);
                            }
                            FMOD_System_PlaySound(sys,FMOD_CHANNEL_FREE,son,0,NULL);
                        }
                    break;
                    default: break;
                }
            break;
            default: break;
        }
    }
}

/**
*          0 1 2
*       0 |X|O|X|
*       1 |O|X|O|
*       2 |X|O|X|
**/

void afficherEcran()
{
    int i = 0, j = 0;
    SDL_Surface *px = NULL;

    px = IMG_Load("px.gif");

    for (i = 0;i < ecran->w;i += px->w)
    {
        position.x = i;
        position.y = 0;
        SDL_BlitSurface(px,NULL,ecran,&position);

        position.y = ecran->h - px->h;
        SDL_BlitSurface(px,NULL,ecran,&position);

        position.y = ecran->h / 3 - px->h;
        SDL_BlitSurface(px,NULL,ecran,&position);

        position.y = 2 * ecran->h / 3 - px->h;
        SDL_BlitSurface(px,NULL,ecran,&position);

        position.x = 0;
        position.y = i;
        SDL_BlitSurface(px,NULL,ecran,&position);

        position.x = ecran->w - px->w;
        SDL_BlitSurface(px,NULL,ecran,&position);

        position.x = ecran->w / 3 - px->w;
        SDL_BlitSurface(px,NULL,ecran,&position);

        position.x = 2 * ecran->w / 3 - px->w;
        SDL_BlitSurface(px,NULL,ecran,&position);
    }

    for (i = 0;i < 3;i++)
        for (j = 0;j < 3;j++)
        {
            position.x = j * TAILLE_CASSE + 10;
            position.y = i * TAILLE_CASSE + 10;
            if (T[i][j] == X)
            {
                SDL_BlitSurface(xp,NULL,ecran,&position);
            }
            else if (T[i][j] == O)
            {
                SDL_BlitSurface(op,NULL,ecran,&position);
            }
        }
    SDL_Flip(ecran);
}

void effacerEcran()
{
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));    /// ecran blanc
    SDL_Flip(ecran);
}

void verifierJeu()
{
    char c = 0;
    int i = 0, j = 0;

    c = T[0][0];
    if (c != VIDE && ((c == T[0][1] && c == T[0][2]) || (c == T[1][0] && c == T[2][0]) || (c == T[1][1] && c == T[2][2])))
    {
        gagne = VRAI;
        gagnant = (c == Joueur)?JOUEUR:ORDINATEUR;
        return;
    }
    c = T[1][1];
    if (c != VIDE && ((c == T[0][1] && c == T[2][1]) || (c == T[1][0] && c == T[1][2]) || (c == T[0][2] && c == T[2][0])))
    {
        gagne = VRAI;
        gagnant = (c == Joueur)?JOUEUR:ORDINATEUR;
        return;
    }
    c = T[2][2];
    if (c != VIDE && ((c == T[1][2] && c == T[0][2]) || (c == T[2][1] && c == T[2][0])))
    {
        gagne = VRAI;
        gagnant = (c == Joueur)?JOUEUR:ORDINATEUR;
        return;
    }
    gagne = FAUX;
    fin_Jeu = VRAI;

    for (i = 0;i < 3;i++)
        for (j = 0;j < 3;j++)
            if (T[i][j] == VIDE)
                fin_Jeu = FAUX;
}

void jouerOrdinateur()
{
    int x = 0, y = 0;

    srand(time(NULL));
    attendre(rand() % 3 + 1); /// 1, 2, ou 3 secondes
    do
    {
        x = (int) rand() % 3;
        y = (int) rand() % 3;
    }
    while (T[x][y] != VIDE);
    T[x][y] = Ordinateur;
    resultat = FMOD_System_CreateSound(sys,"mouseclick2.wav",FMOD_CREATESAMPLE,0,&son);
    if (resultat != FMOD_OK)
    {
        fprintf (stderr," Impossible de charger le(s) fichier(s) son \n");
        exit (EXIT_FAILURE);
    }
    FMOD_System_PlaySound(sys,FMOD_CHANNEL_FREE,son,0,NULL);
}

void pause()
{
    booleen continuer = VRAI;
    SDL_Event event;

    while (continuer == VRAI)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                continuer = FAUX;
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = FAUX;
                    break;
                    default:
                    break;
                }
            break;
            default:
            break;
        }
    }
}

void quitter()
{
    FMOD_Sound_Release(son);
    FMOD_System_Close(sys);
    FMOD_System_Release(sys);

    SDL_FreeSurface(ecran);
    SDL_FreeSurface(xp);
    SDL_FreeSurface(op);
}

void attendre(int n)
{
    time_t deb,fin;

    time(&deb);
    do
    {
        time(&fin);
    }
    while (fin - deb < n);
}
