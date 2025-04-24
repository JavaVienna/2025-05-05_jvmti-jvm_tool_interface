package io.kay;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;

public final class Main {

    private static final String SECRET = readSecret();

    public static void main(String[] args) throws IOException {
        System.out.print("Please enter the secret key: ");
        var reader = new BufferedReader(new InputStreamReader(System.in));
        var input = reader.readLine();

        if (secretMatches(input)) {
            System.out.println("Your entered secret key is correct!");
        } else {
            System.out.println("Please try again");
        }
    }

    private static String readSecret() {
        try {
            return Files.readString(Path.of("secret.txt"));
        } catch (IOException e) {
            throw new RuntimeException("Unable to read secret", e);
        }
    }

    private static boolean secretMatches(String input) {
        return SECRET.equals(input);
    }
}
