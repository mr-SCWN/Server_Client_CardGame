#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <random>
#include <chrono>
#include <wait.h>
#include <fstream>

using namespace std;

int MAX_GAMES = 5; // Maximum of games in server

struct deck // struct for cards
{
    string Suit, Rank;
};

vector <deck> cards(0);

vector <string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"}; // adding all possible variants of suits and ranks
vector <string> ranks = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

int score_first_player = 0, score_second_player = 0;


bool sendMessage(int playerSocket, const string& message) { // function for sending messages for users
    int len = message.length();
    const char* buffer = message.c_str();

    int sent = send(playerSocket, buffer, len, 0);
    if (sent == -1) {
        cerr << "Błąd wysyłania wiadomości: " << strerror(errno) << endl;
        return false;
    }

    return true;
}

int count_card(string rank_of_card){ // function for counting the value of cards
    if (rank_of_card == "Two") return 2;
    if (rank_of_card == "Three") return 3;
    if (rank_of_card == "Four") return 4;
    if (rank_of_card == "Five") return 5;
    if (rank_of_card == "Six") return 6;
    if (rank_of_card == "Seven") return 7;
    if (rank_of_card == "Eight") return 8;
    if (rank_of_card == "Nine") return 9;
    if (rank_of_card == "Ten") return 10;
    if (rank_of_card == "Jack") return 11;
    if (rank_of_card == "Queen") return 12;
    if (rank_of_card == "King") return 13;
    if (rank_of_card == "Ace") return 14;
}


void playGame(int player1, int player2){ // functiom for handling the game and for the game logic
    
    int player_playing = player1 , player_waiting = player2;
    int ind = 0;

    while (true){
        string score1 = to_string(score_first_player), score2 = to_string(score_second_player); // sending message with score and card on top
        string firstMessage1 = "\nMasz punktow: " + score1 +"\n", firstMessage2 = "Przeciwnik ma punktow: " + score2+"\n";
        string secondMEssage1= "\nMasz punktow: " + score2+"\n", secondMessage2 = "Przeciwnik ma punktow: " + score1+"\n";
        sendMessage(player_playing, firstMessage1); sendMessage(player_waiting, secondMEssage1); 
        sendMessage(player_playing, firstMessage2); sendMessage(player_waiting, secondMessage2); 
        string now = "'" + cards[ind].Rank + " " + cards[ind].Suit + "' teraz jest na górze. Zostało " + to_string(51-ind) +" kart w talii" ;
        sendMessage(player_playing, now); sendMessage(player_waiting, now);
        
        bool valid_answer = false;
        string answer = "";

        while (!valid_answer){ // checking right input 
            sleep(1);
            sendMessage(player_playing, "Twoja kolej");
            
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            int numBytes = recv(player_playing, buffer , sizeof(buffer-1), 0);
            if (numBytes <= 0){
                cerr << "Błąd podczas odbierania odpowiedzi" << endl;
                answer = "";
            }
            buffer[numBytes] = '\0';
            answer = string(buffer);

            if (answer != "M" && answer!="m" && answer!="L" && answer!="l"){
                sendMessage(player_playing, "Złe wybrałeś wariant odpowiedzi\n");
            } else {
                valid_answer = true;
            }
        }

        ind++;
        if (ind == 51){     // checking the end of the game
            if (score_first_player > score_second_player ) {
                sendMessage(player_playing, "\n\nKoniec gry. Gratulacje, wygrałeś");
                sendMessage(player_waiting, "\n\nKoniec gry. Powodzenia następnym razem");
            } else if (score_first_player < score_second_player ){
                sendMessage(player_waiting, "\n\nKoniec gry. Gratulacje, wygrałeś");
                sendMessage(player_playing, "\n\nKoniec gry. Powodzenia następnym razem");
            } else {    
                sendMessage(player_playing, "\n\nKoniec gry. Remis! Ładna próba");
                sendMessage(player_waiting, "\n\nKoniec gry. Remis! Ładna próba");
            }

            break;     
            } else {   // updating user's score 
            deck next_card; next_card.Rank = cards[ind].Rank; next_card.Suit = cards[ind].Suit;
            
            string mes_chage_card = "Karta została zmieniona na '" + next_card.Rank + " " + next_card.Suit +  "'\n";
            sendMessage(player_playing, mes_chage_card); sendMessage(player_waiting, mes_chage_card);
            
            if ((count_card(next_card.Rank) > count_card(cards[ind-1].Rank) && (answer == "M" ||  answer == "m" ))
                    || (count_card(next_card.Rank) < count_card(cards[ind-1].Rank) && (answer == "L" || answer == "l" ))){
                    
                    if (next_card.Suit == cards[ind-1].Suit){
                        score_first_player+=2;
                    }else {
                        score_first_player+=1;
                    }

            } else {  //changing player and his score 
                sendMessage(player_playing , "\nNie zgadłeś. Przejście tury\n");

                swap (player_playing, player_waiting);
                swap (score_first_player , score_second_player );
            }
        } 
        
    }

}


int main(){
    for (int i=0 ; i<4; i++){ //adding all cards to the deck
        for (int j = 0; j<13; j++){
            deck one_card ;
            one_card.Rank = ranks[j];
            one_card.Suit = suits[i];
            cards.push_back(one_card);
        }
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));


    int server_fd, player1, player2; 
    struct sockaddr_in addr;    // initialize a server port
    int opt = 1;
    int addrlen = sizeof(addr);
    pid_t child_pid;
    int GamesRunning = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 3);


    while (true) {
        if (GamesRunning < MAX_GAMES) {
            player1 = accept(server_fd, (struct sockaddr *)&addr, (socklen_t*)&addrlen);
            if (player1 < 0) { 
                cerr << "Błąd przyjmowania gracza nr1" << endl;
                continue;
            }
            cout << "Gracz 1 połączony." << endl;  // creatinng the first palyer and waiting for the second one
            sendMessage(player1, "Witaj gracz nr1, czekamy na gracza nr2");

            player2 = -1;
            while (player2 < 0) { // creating the second player
                player2 = accept(server_fd, (struct sockaddr *)&addr, (socklen_t*)&addrlen);
                if (player2 < 0) {
                    std::cerr << "Błąd przyjmowania gracza nr2." << endl;
                }
            }
            cout << "Gracz 2 połączony. Gra startuje." << endl;
            sendMessage(player2, "Witaj gracz nr2. Zaczynamy gre\n");
            sendMessage(player1, "Gracz nr2 dołączył. Zaczynamy gre\n");

            child_pid = fork(); // fork child is handling the game
            if (child_pid == 0) {
                playGame(player1, player2);
                close(player1);
                close(player2);
                exit(0);
            } else if (child_pid > 0) { // fork parent is waiting for the next game
                close(player1);
                close(player2);
                GamesRunning++;
            } else {
                cerr << "Błąd tworzenia fork'u." << endl;
                return 1;
            }
        }
            // waiting for the ending the game 
        waitpid(-1, NULL, WNOHANG);
        GamesRunning--;
    }

    close(server_fd);
    
    
    return 0;
}