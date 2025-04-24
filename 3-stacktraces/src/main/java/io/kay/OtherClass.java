package io.kay;

public final class OtherClass {

    public static void methodStack3() {
        exceptionMethod("This may be an easy to fix exception...", "This is argument is unused", 42);
        System.out.println("This statement is unreachable normally");
    }

    private static void exceptionMethod(String message, String otherString, int someInt) {
        try {
            throw new Exception(message);
        } catch (Exception ex) {
            System.err.print("...but I'm hiding it");
        } finally {
            System.err.print("!");
        }

        System.err.println("!");
    }
}
