package com.webq.capest;

import static com.webq.capest.PropertiesFile.getInt;
import static com.webq.capest.PropertiesFile.getString;

import java.io.IOException;
import java.io.*;
import java.net.Socket;

import org.apache.log4j.Logger;

public class TokenGenCommunicator {

    static Socket socket = null;
    static Logger logger = Logger.getLogger("TokenGenCommunicator");
    static OutputStream out;
    public static void init() {
        try {
            String ip = getString("webq.proxy1.ip");
            int port = getInt("webq.proxy1.port");
            logger.debug("Proxy1 IP: " + ip);
            logger.debug("Proxy1 port: " + port);
            socket = new Socket(ip, port);
            out = socket.getOutputStream();
            logger.debug("connected to proxy1");
        } catch (IOException e) {
            logger.error("", e);
        }
    }

    public static void conveyNewCapacity(String value) {
        logger.debug("To TokenGen: >" + value + "<");
    }

    public static void conveyNewCapacity(double value) {
        try{
            out.write((int) value );
            out.flush();
        } catch (IOException e) {
            logger.error("", e);
        }
    }
}
