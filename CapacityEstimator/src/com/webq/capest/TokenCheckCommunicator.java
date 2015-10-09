package com.webq.capest;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

import org.apache.log4j.Logger;

import static com.webq.capest.PropertiesFile.getInt;

/**
 * Listens to proxy2 for response times of requests.
 * @author bhavin
 *
 */
public class TokenCheckCommunicator {

    private ServerSocket serverSocket;
    private BufferedReader reader;
    private Socket socket = null;
    private int port = getInt("webq.tokencheck.port");
    Logger logger = Logger.getLogger("TokenCheckCommunicator");

    public TokenCheckCommunicator() {
        connectWithTokenCheck();
    }

    private void connectWithTokenCheck() {
        try {
            serverSocket = new ServerSocket(port);
            socket = serverSocket.accept();
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            logger.debug("connected with TokenCheck");
        } catch (IOException e) {
            logger.error("Something went wrong in creating the server socket.");
            logger.error("", e);
            System.exit(1);
        }
    }

    public void listenToProxy2() {
        double responseTimes = 0;
        long requestCount = 1;
        int url;
//      char[] charbuf = new char[255];

        // TODO: decide on what to do upon IOException
        while (true) {
            try {
//              int count = reader.read(charbuf);
                String line = reader.readLine();
//              if(count > 0) {
                if(line != null) {
                    // TODO: listen to socket and compute the two variables above
			logger.debug("Received: " + line);
                    if(line.charAt(0) == 'A') {
                        url=Integer.parseInt(line.substring(line.indexOf('_')+1));
                        PowerRatioData.handleArrival(url);
                    } else if(line.charAt(0) == 'F') {
                        url=Integer.parseInt(line.substring(line.indexOf('_')+1));
                        PowerRatioData.handleFailure(url);
                    } else {
                        //String line = String.valueOf(charbuf);
                      logger.debug("Received: " + line);
//                      logger.debug(line.charAt(0));
//                      logger.debug(line.charAt(line.length()-1));
                        responseTimes = Double.parseDouble(line.substring(0,line.indexOf('_')));
                        url=Integer.parseInt(line.substring(line.indexOf('_')+1));
                        requestCount = 1;
                        PowerRatioData.handleCompletion(responseTimes/1000000, requestCount,url);
                    }
                } else {
                    logger.warn("Waiting for TokenCheck to reconnect.");
                    connectWithTokenCheck();
                    continue;
                }
            } catch (IOException | NumberFormatException e) {
                logger.error("", e);
                if (!socket.isConnected()) {
                    logger.error("TokenCheck connection lost. Waiting for tokencheck to reconnect.");
                    try {
                        serverSocket.close();
                        socket = serverSocket.accept();
                        continue;
                    } catch (IOException e1) {
                        logger.error("Something went wrong in recreating the server socket.");
                        logger.error("", e1);
                        System.exit(1);
                    }
                }
            }
        }
    }
}
