package com.webq.capest;

import static com.webq.capest.PropertiesFile.getString;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Date;
import java.text.SimpleDateFormat;

import org.apache.log4j.Logger;

public class PerSecondLogging {
    static private PrintWriter pw = null;
    static Logger logger = Logger.getLogger("PerSecondLogging");
    public static void log() {
        initLogger();
        PowerRatio powerRatio = PowerRatioData.getPerSecondPowerRatio();

        long ms = System.currentTimeMillis();
        SimpleDateFormat sdf = new SimpleDateFormat("MMM dd,yyyy HH:mm:ss");

        pw.printf(
                "%s Respt:%.7f Arrv:%d Success:%d Failed:%d Tput:%d "+
                "PowerRatio:%.2f Capacity:%.2f Discarded:%.2f %d\n",
                sdf.format(new Date(ms)),
                powerRatio.getAverageResponseTime(),
                powerRatio.getArrivedRequestCount(),
                powerRatio.getSuccessfulRequestCount(),
                powerRatio.getFailedRequestCount(),
                powerRatio.getArrivedRequestCount(),
                powerRatio.getPowerRatio(),
                CapacityEstimator.currentCapacity,
                powerRatio.getDiscardedResponseTimePercent(),
                powerRatio.minId
                );
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

