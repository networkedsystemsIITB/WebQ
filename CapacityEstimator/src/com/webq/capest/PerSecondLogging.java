package com.webq.capest;

import static com.webq.capest.PropertiesFile.getString;

import java.io.FileNotFoundException;
import java.io.PrintWriter;

import org.apache.log4j.Logger;

public class PerSecondLogging {
    static private PrintWriter pw = null;
    static Logger logger = Logger.getLogger("PerSecondLogging");
    public static void log() {
        initLogger();
        PowerRatio powerRatio = PowerRatioData.getPerSecondPowerRatio();
        pw.println(System.currentTimeMillis()/1000 + " Respt:" +
                powerRatio.getAverageResponseTime() + " Arrv:" +
                powerRatio.getArrivedRequestCount() + " Success:" +
                powerRatio.getSuccessfulRequestCount() + " Failed:" +
                powerRatio.getFailedRequestCount() + " Tput:" +
                powerRatio.getArrivedRequestCount() + " PowerRatio:" +
                powerRatio.getPowerRatio() + " Capacity:" +
                CapacityEstimator.currentCapacity + " Discarded%:" +
                powerRatio.getDiscardedResponseTimePercent() + " " +
                powerRatio.minId);
        pw.flush();
    }
    private static void initLogger() {
        if(pw == null) {
            try {
                pw = new PrintWriter(getString("webq.logging.filename"));
            } catch (FileNotFoundException e) {
                logger.error("", e);
            }
        }
    }
}

