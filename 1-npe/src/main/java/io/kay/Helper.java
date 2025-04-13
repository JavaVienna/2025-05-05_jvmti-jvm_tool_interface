package io.kay;

import java.util.Map;
import java.util.HashMap;

public class Helper {

    public static Map<String, Main.Data> getMap() {
        return Map.of("data", createData());
    }

    private static Main.Data createData() {
        return new Main.Data(null);
    }
}
