package io.kay;

public final class OtherClass {

    public static void methodStack3() {
        exceptionMethod();
    }

    private static void exceptionMethod() {
        try {
            throw new Exception("This may be an easy to fix exception...");
        } catch (Exception ex) {
            System.err.println("but I'm hiding it!");
        }
    }
}
