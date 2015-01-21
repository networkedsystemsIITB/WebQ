package com.webq.capest;

import static com.webq.capest.PropertiesFile.getInt;
import static com.webq.capest.PropertiesFile.getString;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;

import org.apache.log4j.Logger;

public class TokenGenCommunicator {
	
	static Socket socket = null;
	static Logger logger = Logger.getLogger("TokenGenCommunicator");
	static PrintWriter toProxy1;
	public static void init() {
		try {
			String ip = getString("webq.proxy1.ip");
			int port = getInt("webq.proxy1.port");
			logger.debug("Proxy1 IP: " + ip);
			logger.debug("Proxy1 port: " + port);
			socket = new Socket(ip, port);
			toProxy1 = new PrintWriter(socket.getOutputStream(), true);
			logger.debug("connected to proxy1");
		} catch (IOException e) {
			logger.error("", e);
		}
	}
	
	public static void conveyNewCapacity(String value) {
		logger.debug("To TokenGen: >" + value + "<");
		toProxy1.println(value);
		toProxy1.flush();
	}
	
	public static void conveyNewCapacity(double value) {
		conveyNewCapacity(Integer.toString((int)value));
	}
}
