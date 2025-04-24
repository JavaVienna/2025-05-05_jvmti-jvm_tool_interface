package io.kay;

public final class Main {

    public static void main(String[] args) {
        methodStack1();
    }

    private static void methodStack1() {
        methodStack2();
    }

    private static void methodStack2() {
        OtherClass.methodStack3();
    }
}
