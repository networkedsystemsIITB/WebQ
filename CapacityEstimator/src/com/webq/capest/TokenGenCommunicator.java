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
    static PrintWriter[] toProxy1 = new PrintWriter[noOfProxy];
    public static void init() {
        try {
            for( int i = 0; i < noOfProxy; i++ ){
                String proxyNo = Integer.toString(i+1);
                String ip = getString("webq.proxy"+proxyNo+".ip");
                int port = getInt("webq.proxy"+proxyNo+".port");
                socket = new Socket(ip, port);
                toProxy1[i] = new PrintWriter(socket.getOutputStream(), true);
                logger.debug("Proxy"+proxyNo+" IP: " + ip);
                logger.debug("Proxy"+proxyNo+" port: " + port);
                logger.debug("connected to proxy"+proxyNo);
            }
        } catch (IOException e) {
            logger.error("", e);
        }
    }

    public static void conveyNewCapacity(String value) {
        logger.debug("Capacity To TokenGen: >" + value + "<");
        try{
            for( int i = 0; i < noOfProxy; i++ ){
                toProxy1[i].println("c" + value);
                toProxy1[i].flush();
            }
        } catch (Exception e) {
            logger.error("", e);
        }
    }
    public static void conveyHardness(String value) {
        logger.debug("Hardness To TokenGen: >" + value + "<");
        try{
            for( int i = 0; i < noOfProxy; i++ ){
                toProxy1[i].println("h" + value);
                toProxy1[i].flush();
            }
        } catch (Exception e) {
            logger.error("", e);
        }
    }

    public static void conveyNewCapacity(double value) {
        conveyNewCapacity(Integer.toString((int)value));
    }
}
