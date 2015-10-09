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
                "%s Respt:%.7f Arrv1:%d Success1:%d Failed1:%d Tput1:%f "+
                        "Arrv2:%d Success2:%d Failed2:%d Tput2:%f "+
                "PowerRatio:%.2f Capacity:%.2f Discarded:%.2f %d\n",
                sdf.format(new Date(ms)),
                powerRatio.getAverageResponseTime(),
                powerRatio.getArrivedRequestCount(0),
                powerRatio.getSuccessfulRequestCount(0),
                powerRatio.getFailedRequestCount(0),
                powerRatio.getThroughput(0),
                powerRatio.getArrivedRequestCount(1),
                powerRatio.getSuccessfulRequestCount(1),
                powerRatio.getFailedRequestCount(1),
                powerRatio.getThroughput(1),
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

