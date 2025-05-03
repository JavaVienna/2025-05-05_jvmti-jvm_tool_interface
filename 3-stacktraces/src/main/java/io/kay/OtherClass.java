package io.kay;

public final class OtherClass {

    public static void methodStack3() {
        if (exceptionMethod("This argument is unused", 42)) {
            System.out.println("This statement is unreachable normally");
        }
    }

    private static boolean exceptionMethod(String otherString, int someInt) {
        try {
            throw new Exception("This may be an easy to fix exception...");
        } catch (Exception ex) {
            System.err.print("...but I'm hiding it");
        } finally {
            System.err.print("!");
        }

        System.err.println("!");
        return false;
    }
}
