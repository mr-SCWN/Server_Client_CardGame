# Projekt zaliczeniowy z przedmiotu Sieci Komputerowe

## Gra karciana: "Mniej Więcej"

### Opis
W projekcie są zaimplementowany serwer współbieżny, który obsługuje graczy, oraz klienty. 

Gry są w parach dla dwóch użytkowników. Projekt wykorzystuje gniazda TCP/IP do komunikacji między serwerem a klientami.

Projekt jest zrealizowany w dwóch plikach: `server.cpp` i `client.cpp` lub `client.py` (w `client.py` są dodane obrazki kard).

- `server.cpp` - obsługuje serwer i logikę gry, przesyła informacje do graczy (ilość punktów, karta na stole) w formacie tekstowym, obsługuje odpowiedzi od klientów.
- `client.cpp` - obsługuje klientów, łączy się z serwerem i daje możliwość odpowiedzi (M/m - jeśli gracz myśli, że następna karta będzie większa od obecnej, lub L/l - jeśli będzie mniejsza).
- `client.py` - obsługuje klientów, łączy się z serwerem i daje możliwość odpowiedzi (M/m - jeśli gracz myśli, że następna karta będzie większa od obecnej, lub L/l - jeśli będzie mniejsza), dodane obrazki kart.

W folderze PNG-cards-1.3 są dodane obrazki kart.

#### Działający program klientów w Pythonie:
![github-small](https://github.com/mr-SCWN/Server_Client_CardGame/blob/main/ScreenShot_GUI.png)

### Jak uruchomić projekt

#### Linux
- Skompiluj serwer w terminalu: `g++ -Wall ./server.cpp -o ./server`
- Uruchom plik `server` w terminalu: `./server`

#### Windows (C++)
- Skompiluj plik klienta w CMD: `g++ -Wall ./client.cpp -o client -lws2_32`
- Uruchom plik `client` w CMD: `client "IP adres serwera" 8080` (8080 - numer portu)

#### Windows (Python)
- Uruchom plik `client.py` w CMD: `python client.py "IP adres serwera" 8080` (8080 - numer portu)

### Logika gry

Na stole leży pierwsza karta. Gra wykorzystuje karty, a dwaj gracze kolejno mówią, czy następna karta jest większa (M/m) lub mniejsza (L/l) od tej, która już leży.

Jeśli gracz zgadnie, to otrzymuje 2 pkt, jeśli kolor jest taki sam jak u poprzedniej karty; jeśli zgadnie, ale kolor jest inny, otrzymuje 1 pkt.

Jeśli gracz nie zgadnie, tura przechodzi do następnego gracza i obowiązują takie same reguły.

Sprawdzenie, kto wygrał, odbywa się, gdy skończą się karty. Wygrywa ten, kto ma więcej punktów.



<table width="100%">
  <tr>
    <td align="right">
      Ivan Kaliadzich 153936
    </td>
  </tr>
</table>


