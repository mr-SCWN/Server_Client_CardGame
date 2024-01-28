import socket
import sys
import cv2

suits = ["Hearts", "Diamonds", "Clubs", "Spades"]
ranks = ["Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"]



def ShowCard(suit, rank):
    filename = f"PNG-cards-1.3/{rank.lower()}_of_{suit.lower()}.png"
    img = cv2.imread(filename, cv2.IMREAD_ANYCOLOR)
    if img is not None:
        return img
    else:
        return None

def main():
    rank_card = ""
    suit_card = ""

    # Ensure server address and port are provided
    if len(sys.argv) != 3:
        print("Usage: python client.py <server> <port>")
        sys.exit(1)

    server_address = sys.argv[1]
    server_port = int(sys.argv[2])

    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Connect the socket to the server
    sock.connect((server_address, server_port))
    print(f"Connected to {server_address} on port {server_port}")

    window_name = "Card"
    cv2.namedWindow(window_name, cv2.WINDOW_AUTOSIZE)

    while True:

        # Receive message from the server
        data = sock.recv(4096)
        if not data:
            print("Server closed the connection")
            break

        message = data.decode()

        text = message.split() #8 7
        for i in range(len(text)- 1):
            word1 = text[i][1:]
            word2 = text[i+1][:-1]
            if word1 in ranks and word2 in suits:
                rank_card = word1
                suit_card = word2
        print(message)


        if suit_card in suits and rank_card in ranks:
    
            img = ShowCard(suit_card, rank_card)
            if img is not None:
                cv2.imshow(window_name, img)
                if cv2.waitKey(1) & 0xFF == ord('q'):  # Press 'q' to quit
                    break
            else:
                continue


        # Check if it's the client's turn and send response
        if message == "Twoja kolej":
            print("Napisz, czy następna karta jest większa (M/m) lub mniejsza (L/l)")
            user_input = input()
            if user_input:
                sock.sendall(user_input.encode())

    # Clean up connection
    sock.close()

if __name__ == "__main__":
    main()
