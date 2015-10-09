package com.webq.capest;

import static com.webq.capest.PropertiesFile.getInt;

import java.util.Timer;
import java.util.TimerTask;

import org.apache.log4j.Logger;

public class CapacityManager {

    private static CapacityEstimator capacityEstimator = null;
    private static CapacityChangeDetector capacityChangeDetector = null;
    private static Logger logger = Logger.getLogger("CapacityManager");

    private static Timer capacityEstimationTimer = new Timer();
    private static Timer TrainingPeriodTimer = new Timer();
    private static Timer capacityChangeDetectionTimer = new Timer();
    private static Timer loggingTimer = new Timer();

    public static long epoch = getInt("webq.logic.epoch");
    public static long trainingperiod = getInt("webq.logic.trainingperiod");

    public static void main(String[] args) throws InterruptedException {
        logger.debug("Epoch: " + epoch);
        TokenGenCommunicator.init();

        schedulePerSecondLogging();
        gencapestinp();
        scheduleCapacityEstimation();
        startTokenCheckListener();
    }

    private static void startTokenCheckListener() {
        logger.debug("enter startTokenCheckListener");
        Thread networkListener = new Thread() {
            public void run() {
                TokenCheckCommunicator nc = new TokenCheckCommunicator();
                nc.listenToProxy2();
            }
        };
        networkListener.run();
        logger.debug("Exit startTokenCheckListener");
    }

     public static void gencapestinp() {
        logger.debug("enter genipfile");
        capacityEstimator = new CapacityEstimator();
        TimerTask estimationTask = new TimerTask() {
            public void run() {
                capacityEstimator.handleEpoch();
            }
        };

        capacityEstimationTimer.schedule(estimationTask, 0, (long) epoch*1000);
        logger.debug("exit genipfile");
    } 

    public static void scheduleCapacityEstimation() {
        logger.debug("enter scheduleCapacityEstimation");
        capacityEstimator = new CapacityEstimator();
        TimerTask estimationTask = new TimerTask() {
            public void run() {
                capacityEstimator.handleEndofTrainingPeriod();
           }
       };

       capacityEstimationTimer.schedule(estimationTask, (long) trainingperiod*1000);
       logger.debug("exit scheduleCapacityEstimation");
   }

   /* public static void scheduleCapacityChangeDetection() {
        capacityEstimationTimer.cancel();
        capacityEstimationTimer.purge();
        capacityEstimator = null;

        capacityChangeDetector = new CapacityChangeDetector();
        TimerTask capacityChangeDetectionTask = new TimerTask() {
            public void run() {
                capacityChangeDetector.handleEpoch();
            }
        };

        capacityChangeDetectionTimer.schedule(capacityChangeDetectionTask, 0, epoch * 1000);
    }*/

    public static void schedulePerSecondLogging() {
        TimerTask perSecondLoggingTask = new TimerTask() {
            public void run() {
                PerSecondLogging.log();
            }
        };

        loggingTimer.schedule(perSecondLoggingTask, 0, 1000);
    }

}
