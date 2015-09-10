package com.webq.capest;

import java.util.ArrayList;

import org.apache.commons.math3.analysis.polynomials.PolynomialFunction;
import org.apache.commons.math3.fitting.WeightedObservedPoint;
import org.apache.log4j.Logger;

public class PowerRatioData {

    public static ArrayList<WeightedObservedPoint> observations = new ArrayList<WeightedObservedPoint>();

    private static PowerRatio currentPowerRatio = new PowerRatio();
    private static PowerRatio perSecondPowerRatio = new PowerRatio();
    static Logger logger = Logger.getLogger("PowerRatioData");

    public static double maxDistance = Double.MAX_VALUE;
    public static PolynomialFunction estimatedPowerRatioCurve = null;
    private static int id = 0;

    public PowerRatioData() {}

    private static Object currentPowerRatioLock = new Object();
    private static Object perSecondPowerRatioLock = new Object();

    /**
     * Returns and resets the power ratio object.
     */
    public static PowerRatio getCurrentPowerRatio() {
        try {
            //if multiple requests are getting completed, then this will get updated very quickly.
            //hence return a copy.
            PowerRatio duplicate;
            synchronized(currentPowerRatioLock) {
                duplicate = currentPowerRatio.clone();
                currentPowerRatio = new PowerRatio();
            }
            logger.debug("request arrival count for type 1: " + duplicate.getArrivedRequestCount(0));
            logger.debug("request arrival count for type 2: " + duplicate.getArrivedRequestCount(1));
            return duplicate;
        } catch (CloneNotSupportedException e) {
            //never ever be called
            logger.error("Exception: ", e);
        }
        return null; //never should be the case, cloning will always work
    }

    public static PowerRatio getPerSecondPowerRatio() {
        try {
            //if multiple requests are getting completed, then this will get updated very quickly.
            //hence return a copy.
            PowerRatio duplicate;
            synchronized(perSecondPowerRatioLock) {
                duplicate = perSecondPowerRatio.clone();
                perSecondPowerRatio = new PowerRatio();
            }
            return duplicate;
        } catch (CloneNotSupportedException e) {
            //never ever be called
            logger.error("Exception: ", e);
        }
        return null; //never should be the case, cloning will always work
    }

    /**
     * Handles the request, and records its response time for power ratio calculation.
     */
    public static void handleCompletion(double responseTimes, long countOfRequests, int url) {
        synchronized(currentPowerRatioLock) {
        currentPowerRatio.recordCompletion(id, responseTimes, countOfRequests, url);
        }
        synchronized(perSecondPowerRatioLock) {
            perSecondPowerRatio.recordCompletion(id, responseTimes, countOfRequests, url);
        }
        id++;
    }

    public static void handleFailure(int url) {
        synchronized(currentPowerRatioLock) {
            currentPowerRatio.recordFailure(id, url);
        }
        synchronized(perSecondPowerRatioLock) {
            perSecondPowerRatio.recordFailure(id, url);
        }
        id++;
    }

    public static void handleArrival(int url) {
        synchronized(currentPowerRatioLock) {
            currentPowerRatio.recordArrival(id, url);
        }
        synchronized(perSecondPowerRatioLock) {
            perSecondPowerRatio.recordArrival(id, url);
        }
        id++;
    }

    public static void resetCurrentPowerRatio() {
        synchronized(currentPowerRatioLock) {
            currentPowerRatio = new PowerRatio();
        }
    }

    public static void resetPerSecondPowerRatio() {
        synchronized(perSecondPowerRatioLock) {
            perSecondPowerRatio = new PowerRatio();
        }
    }
}
