package io.kay;

public class Main {

    public static void main(String[] args) {
        // 3 possibilities which could be null in this line
        var value = Helper.getMap().get("data").getData().toLowerCase();

        System.out.println(value);
    }
}
