#include "screen.h"
#include "menus.h"
#include "keyboard.h"
#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_COLOR_BROWN   "\x1b[33m"
#define ANSI_COLOR_TESTE   "\x1b[1;93m"
#define ANSI_COLOR_RESET   "\x1b[0m"  // Reseta para a cor padrão

void artmenu(){
    int i;
    screenClear();

     char *cvz_art[] = {
          ANSI_COLOR_GREEN"*   ##    #      ###    #  #   #####         #  #    ##           "ANSI_COLOR_BROWN"####    ##    #   #  ####   ###    ####    ## *   *"ANSI_COLOR_RESET,
          ANSI_COLOR_GREEN"*  #  #   #       #     ## #     #           #  #   #  #             #   #  #   ## ##  #   #   #     #      #  # *  *"ANSI_COLOR_RESET,
          ANSI_COLOR_GREEN"*  #      #       #     # ##     #           #  #    #              #    #  #   # # #  ####    #     ###     # *    *"ANSI_COLOR_RESET,
          ANSI_COLOR_GREEN"*  #      #       #     #  #     #           ####     #            #     #  #   # # #  #   #   #     #        # *   *"ANSI_COLOR_RESET,
          ANSI_COLOR_GREEN"*  #  #   #       #     #  #     #            ##    #  #          #      #  #   #   #  #   #   #     #      #  # *  *"ANSI_COLOR_RESET,
          ANSI_COLOR_GREEN"*   ##    ####   ###    #  #     #            ##     ##          "ANSI_COLOR_BROWN" ####    ##    #   #  ####   ###    ####    ## *   *"ANSI_COLOR_RESET,
         ANSI_COLOR_TESTE"##**+++=====-------------==--------------------------::-----------=====----===========+**#" ANSI_COLOR_GREEN"      ⠀⠀⠀ ⠀⠀⢀⡠⠖⠊⠉⠉⠉⠉⢉⠝⠉⠓⠦⣄⠀⠀⠀⠀⠀⠀⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_YELLOW"**+++==--------:::::::::::::::::::::::::::.::::::::::::::::::::::---------::----------=++*" ANSI_COLOR_GREEN"           ⠀⢀⡴⣋⠀⠀⣤⣒⡠⢀⠀⠐⠂⠀⠤⠤⠈⠓⢦⡀⠀⠀⠀⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_TESTE"*+++===-=-------:::::.:::::::...::::::......-==-.....::.::::::::::::::::::::::--------==++" ANSI_COLOR_GREEN"  ⠀⠀⠀     ⣰⢋⢬⠀⡄⣀⠤⠄⠀⠓⢧⠐⠥⢃⣴⠤⣤⠀⢀⡙⣆⠀⠀⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"*++++=--------:::::::...........::::....:-.:#%%#...............:::::::::::::::---------==+" ANSI_COLOR_GREEN"   ⠀⠀⠀  ⠀⢠⡣⢨⠁⡘⠉⠀⢀⣤⡀⠀⢸⠀⢀⡏⠑⠢⣈⠦⠃⠦⡘⡆⠀⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"*++++=--------=------::------------:.....::=++**-::...................::::::::---------==+" ANSI_COLOR_GREEN"       ⠀⢸⡠⠊⠀⣇⠀⠀⢿⣿⠇⠀⡼⠀⢸⡀⠠⣶⡎⠳⣸⡠⠃⡇⠀⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"###**+++++===+======-----------------:....:+%%%#:........::::::...........:::::::------==+" ANSI_COLOR_GREEN"    ⢀⠔⠒⠢⢜⡆⡆⠀⢿⢦⣤⠖⠒⢂⣽⢁⢀⠸⣿⣦⡀⢀⡼⠁⠀⠀⡇⠒⠑⡆"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"##****+==+=======++====----------------::-+******+-:::::::::::::::::::::.::::::---===++**#" ANSI_COLOR_GREEN"    ⡇⠀⠐⠰⢦⠱⡤⠀⠈⠑⠪⢭⠩⠕⢁⣾⢸⣧⠙⡯⣿⠏⠠⡌⠁⡼⢣⠁⡜⠁"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"#***++++++++++=+++++=========---------=+**##*******+=::::::::::::::::::::::-----=+++****#%" ANSI_COLOR_GREEN"   ⠈⠉⠻⡜⠚⢀⡏⠢⢆⠀⠀⢠⡆⠀⠀⣀⣀⣀⡀⠀⠀⠀⠀⣼⠾⢬⣹⡾⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"###**+++++++*+++=============--------*#*###**+**#*##*-:::::::::::--------------=+**+=++*#%" ANSI_COLOR_GREEN"      ⠀ ⠀⠉⠀⠀⠈⣇⠀⠀⠀⣴⡟⢣⣀⡔⡭⣳⡈⠃⣼⠀⠀⠀⣼⣧⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%###**+++++=++==--------===--------:+#%#****++*##**%*+:::::::::::--------------==++===+*#%" ANSI_COLOR_GREEN"       ⠀⠀⠀⠀⠀⠀⠀⢹⠀⠀⣸⣿⣿⣿⡿⣷⣿⣿⣷⠀⡇⠀⠀⠀⠙⠊⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%###*+++++======---------------::---*%%*****+**##*#%#*---:---::::--------------=======+*##" ANSI_COLOR_GREEN"   ⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢸⣠⠀⢻⠛⠭⢏⣑⣛⣙⣛⠏⠀⡇⠀⠀⠀⠀⠀⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%###*++++=+==+==---===-=-------::::-#%%=+++*###****%#*::::-----:::----:-------========++*#" ANSI_COLOR_GREEN"  ⠀⠀⠀⠀⠀⠀⠀⠀  ⠀⠀⡏⠠⠜⠓⠉⠉⠀⠐⢒⡒⡍⠐⡇⠀⠀⠀⠀⠀⠀⠀"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%%##*+++===+===-------------:-::::::=##=+*##%####**##-..:::-----:::::----======-=====+++*#"ANSI_COLOR_RED"        Seja Bem Vindo ao Apocalipse Z"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%%#***++++=====------:::---::::::::::-+#%%%%#%%%%%#==:::::::::---::------=====--===+=++*#%"ANSI_COLOR_RED"          Para Iniciar Sua Jornada de"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%##***+++++++==--:::::::----:::::::::-##%########%%#--::::::::::-::-----=====---=====++**#"ANSI_COLOR_RED"            Sobrevivência Pressione 1"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%#**+++++==+==---:::--::----::::::::+**#*%%###%%%####+:::::::::------:---=====--===++**##%"ANSI_COLOR_RED"           Pressione 2 Para Visualizar"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%#**++++*++====---:::-------::::::=***#**++++=-:-=+###*::::::::::-----::--------======+*##"ANSI_COLOR_RED"              As Intruções do Jogo"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%##***+++++=====---::---------:::+%*+=--==+**-:::-=-**##-:::::::--------::-:::::--+**=++*#"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%##**+++=++==--==+----------:::::**+---::-+++=====+++#*#=:.:::::::--::=*=::::--::-+**+=++*"ANSI_COLOR_RED"                 Está Com Medo?"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%%#**+++=====+++**+=----:::-::---**=====+*#++##%#########-:::::::::::-=++-::-+*=-=+**+=+** "ANSI_COLOR_RED"                  Pressione 3"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%#**+++===+==+*+++++=-:::::-===-+##=+*#*#%#+#%%%%####*%##=:....::::::-=++===+*#****###**#%  "ANSI_COLOR_RED"            E Fuja Enquanto Pode"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"##**++==++++++******==--:::-====+##=*%%#%%%*#%%%%%%%#*%##=:::::----===+*++++++********+*##"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%#**++=+***++++++***+====---==--*%#=%%%#%%%*%%%%%%#%%#%%%==----==++++++++*###***###%*####%"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%%##**************#***++=-------*%%+%%##%%%%%%%%%%#%%#%##=+=======+++++++***#**##%###%%%%%"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%%%%####********#######*+===---=#%%*%%*#%%%%%%%%%%##%#%%*===+===++++++++++++***###%%###%%%"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"%%%%####****##*#########*+====++#%%#%##%%%%%%%#%%%##%+@%*==++++++++**++++++**###%%###%%%%%"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"@%%%####**#**#######*****+===**%#%%*%#%%%%%%%%#%%%%%**%%#%*********+*++++**###%%%%%##%%%%@"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"@@%%%%%#########**********+===+++**+*##%%=-%%%#%%%%%=*###%***###***+*++**##########%%%%%%@"ANSI_COLOR_RESET,
         ANSI_COLOR_BLACK"@@@@%%%%%#%%%%%##**++*++*++=++=-==+**%%%%==%%%#*@%%%=+*+*++********+****#########%%%%%%@@@"ANSI_COLOR_RESET,
        
     };
     for(i = 0; i<sizeof(cvz_art) / sizeof (cvz_art[0]); i++) {
        printf("%s\n", cvz_art[i]);}

     while (1) {
        if (keyhit()) {
            int key = readch();
            if (key == '\n' || key == '1') {
                fflush(stdout);
                screenClear();
                break;
            }
        }
    }
}
