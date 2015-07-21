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
    static int noOfProxy = getInt("webq.noOfProxy");
    static OutputStream[] out = new OutputStream[noOfProxy];
    public static void init() {
        try {
            for( int i = 0; i < noOfProxy; i++ ){
                String proxyNo = Integer.toString(i+1);
                String ip = getString("webq.proxy"+proxyNo+".ip");
                int port = getInt("webq.proxy"+proxyNo+".port");
                socket = new Socket(ip, port);
                out[i] = socket.getOutputStream();
                logger.debug("Proxy"+proxyNo+" IP: " + ip);
                logger.debug("Proxy"+proxyNo+" port: " + port);
                logger.debug("connected to proxy"+proxyNo);
            }
        } catch (IOException e) {
            logger.error("", e);
        }
    }

    public static void conveyNewCapacity(String value) {
        logger.debug("To TokenGen: >" + value + "<");
    }

    public static void conveyNewCapacity(double value) {
        try{
            for( int i = 0; i < noOfProxy; i++ ){
                out[i].write((int) value );
                out[i].flush();
            }
        } catch (IOException e) {
            logger.error("", e);
        }
    }
}
