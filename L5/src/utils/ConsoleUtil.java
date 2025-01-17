package utils;

import java.util.Scanner;

public class ConsoleUtil {
    private static final Scanner scanner = new Scanner(System.in);

    public static String readLine(String prompt) {
        System.out.print(prompt);
        return scanner.nextLine();
    }

    public static int readInt(String prompt) {
        System.out.print(prompt);
        while (!scanner.hasNextInt()) {
            System.out.println("Invalid input. Please enter a number.");
            System.out.print(prompt);
            scanner.next();
        }
        int value = scanner.nextInt();
        scanner.nextLine(); // Consume the newline character
        return value;
    }

    public static double readDouble(String prompt) {
        System.out.print(prompt);
        while (!scanner.hasNextDouble()) {
            System.out.println("Invalid input. Please enter a number.");
            System.out.print(prompt);
            scanner.next();
        }
        double value = scanner.nextDouble();
        scanner.nextLine(); // Consume the newline character
        return value;
    }
}