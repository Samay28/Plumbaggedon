import java.util.Scanner;
import java.util.Timer;
import java.util.TimerTask;

public class WordChainGame {
    private static String currentPlayer;
    private static String lastWord;
    private static boolean gameInProgress = true;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Welcome to the Word Chain Game!");
        System.out.print("Enter Player 1's name: ");
        String player1 = scanner.nextLine();
        System.out.print("Enter Player 2's name: ");
        String player2 = scanner.nextLine();

        // Start the first turn
        currentPlayer = player1;
        playTurn(scanner);

        scanner.close();
    }

    private static void playTurn(Scanner scanner) {
        System.out.println("\n" + currentPlayer + "'s turn!");

        if (lastWord != null) {
            System.out.println("The last word was: " + lastWord);
        }

        System.out.print("Enter a word: ");
        String inputWord = scanner.nextLine();

        if (!isValidWord(inputWord)) {
            endGame(currentPlayer + " entered an invalid word: " + inputWord);
            return;
        }

        // Check if the word starts with the last letter of the previous word
        if (lastWord != null && inputWord.charAt(0) != lastWord.charAt(lastWord.length() - 1)) {
            endGame(currentPlayer + " did not follow the word chain rule.");
            return;
        }

        lastWord = inputWord;

        // Switch to the other player
        currentPlayer = (currentPlayer.equals("Player 1")) ? "Player 2" : "Player 1";

        // Schedule the next turn
        Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                if (gameInProgress) {
                    playTurn(scanner);
                }
            }
        }, 15000); // 15 seconds time limit
    }

    private static boolean isValidWord(String word) {
        // You can implement dictionary API integration here
        // For simplicity, just check if the word contains only letters
        return word.matches("[a-zA-Z]+");
    }

    private static void endGame(String message) {
        gameInProgress = false;
        System.out.println("\nGame Over!");
        System.out.println(message);
    }
}
