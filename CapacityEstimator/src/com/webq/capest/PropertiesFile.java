package com.webq.capest;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class PropertiesFile {
    private static Properties r = new Properties();
    static {
        try {
            r.load(new FileInputStream("webq.properties"));
        } catch (IOException e) {
            System.err.println("This is a setup issue. The configuration file webq.properties is not found.");
            e.printStackTrace();
            System.exit(1);
        }
    }

    public static double getDouble(String key) {
        return Double.parseDouble(r.getProperty(key));
    }

    public static String getString(String key) {
        return r.getProperty(key);
    }

    public static int getInt(String key) {
        return Integer.parseInt(r.getProperty(key));
    }

    public static boolean getBoolean(String key) {
        return Boolean.parseBoolean(r.getProperty(key));
    }
}
