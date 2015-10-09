package com.webq.capest;

import static com.webq.capest.PropertiesFile.getBoolean;
import static com.webq.capest.PropertiesFile.getDouble;
import static com.webq.capest.PropertiesFile.getInt;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.FileOutputStream; 
import java.util.Properties;

import org.apache.commons.math3.analysis.polynomials.PolynomialFunction;
import org.apache.commons.math3.analysis.solvers.LaguerreSolver;
import org.apache.commons.math3.exception.NoBracketingException;
import org.apache.commons.math3.fitting.PolynomialCurveFitter;
import org.apache.commons.math3.fitting.WeightedObservedPoint;
import org.apache.log4j.Logger;

public class CapacityEstimator {

    Properties properties = new Properties();
    private static Logger logger = Logger.getLogger("CapacityEstimator");

    private boolean capacityFound = false;
    public static double currentCapacity;
    private double previousCapacity;
    private double initialCapacity;
    private double cdelta = 10; //will be set later in the logic, at appropriate time
    private double cdeltaRelative = getDouble("webq.logic.cdeltarelative");
    private double cjump = getDouble("webq.logic.cjump");
    private double relativeCapacityForLocalSearch = getDouble("webq.logic.relativecapacityforlocalsearch");
    private cProgressionMethod cMethod = cProgressionMethod.MULTIPLY;

    private enum cProgressionMethod {
        INCREMENT, MULTIPLY,
    };

    private int fittingDegree = getInt("webq.fitting.degree");
    private int minObservations = getInt("webq.fitting.minobservation");
    private PolynomialCurveFitter fitter = PolynomialCurveFitter.create(fittingDegree);
    private LaguerreSolver solver = new LaguerreSolver();
    private int totalExtraObservations = getInt("webq.fitting.extraobservations");
    private int extraObservationsTillNow = 0;

    final static boolean bypassDiscovery = getBoolean("webq.bypass.bypassdiscovery");
    final static int capacity = getInt("webq.bypass.capacity");

    private boolean overload = false;

    private int stableEpochCount = 0;
    private int requiredStableEpochs = getInt("webq.logic.stability.requiredStableEpochs");
    private int unstableEpochCount = 0;
    private int tolerableUnstableEpochs = getInt("webq.logic.stability.maxUnstableEpochs");

    private double stabilityCushion = 1;
    private double tightnessCushion = getDouble("webq.logic.tightnessCushion");

    private double estimatedLoadAtPowerRatioPeaks = -1;
    private static double peakPowerRatio = -1;

    private static String isLower = null;
    private static boolean isNear = false;

    PrintWriter pwGood = null;
    PrintWriter pwOverload = null;
    public int id=1;
    public int inSys0=0;
    public int inSys1=0;
    public static long epoch = getInt("webq.logic.epoch");

    public CapacityEstimator() {
       // this(getInt("webq.logic.initialcapacity"));
    }

    /*public CapacityEstimator(int c0) {
        setCurrentCapacity(c0);
        initialCapacity = c0;

        try {
            pwGood = new PrintWriter(new File("goodpoints.java.log"));
            pwOverload = new PrintWriter(new File("overloadpoints.java.log"));
        } catch (FileNotFoundException e) {
            logger.error("cant create goodpoints logging file. exiting.");
            System.exit(1);
        }

       // handleCapacityDiscoveryBegins();
    }*/

    /**
     * Sets currentCapacity attibute of this class, and the WebQOriginal class.
     */
    private void setCurrentCapacity(double newCapacity) {
        currentCapacity = newCapacity;
        TokenGenCommunicator.conveyNewCapacity(newCapacity);
    }

    /**
     * Initializes the values for capacity discovery. Schedules next epoch.
     *
     * Called upon WEBQ_CAPACITY_DISCOVERY_BEGINS event.
     */
    /*public void handleCapacityDiscoveryBegins() {
        setCurrentCapacity(initialCapacity);
    }

    private void scheduleCapacityDiscoveryEnds() {
        handleCapacityDiscoveryEnds();
    }*/

    /**
     * Checks if the capacity is found, and updates the values as per Algorithm 1.
     *
     * Also schedules the next epoch event. Called upon WEBQ_CAPACITY_DISCOVERY_EPOCH event.
     *
     * @throws IOException
     */
    public void handleEpoch() {
        logger.debug("epoch called");

        //collect all the data - goodput, resp time and ip load
        PrintWriter currentPw = null;
        try {
            currentPw = new PrintWriter(new FileOutputStream(new File("capacityEstimator.inp"),true));
        }
        catch (FileNotFoundException e) {
            logger.error("cant create input file. exiting.");
            System.exit(1);
        }
        
        	PowerRatio currentPowerRatio = PowerRatioData.getCurrentPowerRatio();
        	if((currentPowerRatio.getSuccessfulRequestCount(0)+currentPowerRatio.getSuccessfulRequestCount(1))!=0)
        	{
			//currentPw.println("more text");
        	currentPw.printf("%.2f\t%.2f\t%.2f\t%.2f\t%.4f\t%d\n",
        	(currentPowerRatio.getArrivedRequestCount(0)+inSys0)/(float)epoch,
        	(currentPowerRatio.getArrivedRequestCount(1)+inSys1)/(float)epoch,
        	currentPowerRatio.getThroughput(0),
        	currentPowerRatio.getThroughput(1),
        	currentPowerRatio.getAverageResponseTime(),
		id);
        	currentPw.flush();
		id++;
		inSys0+=(currentPowerRatio.getArrivedRequestCount(0)-currentPowerRatio.getSuccessfulRequestCount(0)-currentPowerRatio.getFailedRequestCount(0));
                inSys1+=(currentPowerRatio.getArrivedRequestCount(1)-currentPowerRatio.getSuccessfulRequestCount(1)-currentPowerRatio.getFailedRequestCount(1));
        	}
       /* if (bypassDiscovery) {
            setCurrentCapacity(capacity);
            scheduleCapacityDiscoveryEnds();
            return;
        }

        boolean activityEnded = false;
        PowerRatio currentPowerRatio = PowerRatioData.getCurrentPowerRatio();

        if(currentPowerRatio.getArrivedRequestCount() == 0) {
            logger.debug("No arrived request, returning.");
            return;
        }

        if(PowerRatioData.estimatedPowerRatioCurve == null) {
            isLower = "NA";
        } else {
            isLower = "" + isLower(currentPowerRatio.getPowerRatio(), PowerRatioData.estimatedPowerRatioCurve.value(currentPowerRatio.getThroughput()), stabilityCushion);
        }
        isNear = isNear(currentPowerRatio.getThroughput(), currentCapacity, tightnessCushion);
        if(capacityFound) {
            //see if the observation is stable
            if(isNear(currentPowerRatio.getThroughput(), currentCapacity, tightnessCushion)) {
                logger.trace("capacityfound: nearby point found");
                stableEpochCount++;
            } else {
                logger.trace("capacityfound: faraway point found, resetting stableEpochCount to 0");
                stableEpochCount = 0; //we need consistent stable epochs, no outliers in between
            }

            //if we have enough stable samples
            if(stableEpochCount >= requiredStableEpochs) {
                logger.info("capacityfound: stable epochs found. Finishing discovery. Scheduling capacity change module.");
                overload = false;

                scheduleCapacityDiscoveryEnds();

                activityEnded = true;
//              SimulationParameters.endSimulation();
            }
            printDebug(currentPowerRatio);
        } else {
            //detect if overload situation has changed
            if(overload) {
                if(!isLower(currentPowerRatio.getPowerRatio(), PowerRatioData.estimatedPowerRatioCurve.value(currentPowerRatio.getThroughput()), stabilityCushion)
                        //&& isNear(currentPowerRatio.getThroughput(), currentCapacity, tightnessCushion)) {
                        ) {
                    logger.trace("overloadrecheck: nearby point found");
                    stableEpochCount++;
                } else {
                    logger.trace("overloadrecheck: far away point found, resetting stableEpochCount to 0");
                    stableEpochCount = 0; //we need consistent stable epochs, no outliers in between
                    unstableEpochCount++;
                }

                //if we have enough stable samples
                if(stableEpochCount >= requiredStableEpochs) {
                    logger.info("overloadrecheck: stable epochs found. Ending overload.");
                    overload = false;
                    stableEpochCount = 0;
                }
            } else {
                //recheck for overload
                if(peakPowerRatio != -1 &&
                        (isLower(currentPowerRatio.getPowerRatio(), PowerRatioData.estimatedPowerRatioCurve.value(currentPowerRatio.getThroughput()), stabilityCushion)
                            //|| !isNear(currentPowerRatio.getThroughput(), currentCapacity, tightnessCushion))) {
                                )) {
                    overload = true; //should not occur in between the estimation, ideally, but just to be safe, this is here
                    stableEpochCount = 0;
                    logger.info("overloadrecheck: detected overload");
                    if(previousCapacity > 0) {
                        currentCapacity = previousCapacity;
                    }
                }
            }

            overload = false; //XXX: testing - bypass the overload completely
            //act according to new conclusion of overload or not
            if(!overload) {
                //record the observation
                double xValue = currentPowerRatio.getThroughput();
//              xValue += SimulationParameters.webq.totalRequestsInTheSystem;
//              if(xValue <)
                PowerRatioData.observations.add(new WeightedObservedPoint(1, xValue, currentPowerRatio.getPowerRatio()));
                printPointInFileForPlotting(currentPowerRatio);

                //just a sanity check for correctness of logic
                if(capacityFound) {
                    logger.error("Epoch called after capacity found. Detection should be called rather than this.");
                    return;
                }

                //try fitting a curve
                PowerRatioData.estimatedPowerRatioCurve = new PolynomialFunction(fitter.fit(PowerRatioData.observations));
                PolynomialFunction rateOfChange = PowerRatioData.estimatedPowerRatioCurve.polynomialDerivative();
                estimatedLoadAtPowerRatioPeaks = -1;
                try {
                    //if we have enough points to fit, and enough range to reverse-solve the fitting
                    if(PowerRatioData.observations.size() > minObservations) {
                        estimatedLoadAtPowerRatioPeaks = solver.solve(100, rateOfChange, initialCapacity, currentCapacity);
                        logger.debug("curvefitting: got one estimate: " + estimatedLoadAtPowerRatioPeaks);
                        logger.debug("curvefitting: curve: " + PowerRatioData.estimatedPowerRatioCurve);
                    } else {
                        logger.debug("curvefitting: waiting for " + minObservations + ", got only " + PowerRatioData.observations.size());
                    }
                } catch(NoBracketingException e) {
                    //no peak (monotonously increasing function)
                    logger.debug("curvefitting: nobracketingexception");
//                  if(cMethod != cProgressionMethod.INCREMENT) {
                        extraObservationsTillNow = 0;
//                  }
                }

                //if we really have a solution from the latest fitting
                if(estimatedLoadAtPowerRatioPeaks > 0) {
                    //actual peak power ratio
                    peakPowerRatio = PowerRatioData.estimatedPowerRatioCurve.value(estimatedLoadAtPowerRatioPeaks);

                    //if peakPowerRatio is really a maxima (value on both sides is less than it)
                    if (PowerRatioData.estimatedPowerRatioCurve.value(estimatedLoadAtPowerRatioPeaks - cdelta) < peakPowerRatio
                            && PowerRatioData.estimatedPowerRatioCurve.value(estimatedLoadAtPowerRatioPeaks + cdelta) < peakPowerRatio) {
                        //get the maxima for some consistent samples
                        if(extraObservationsTillNow < totalExtraObservations) {
                            extraObservationsTillNow++;
                            logger.debug("------------------new FOUND1 capacity: " + estimatedLoadAtPowerRatioPeaks);
                        } else {
                            //finally, accept the found capacity
                            capacityFound = true;
                            overload = true;
                            setCurrentCapacity(estimatedLoadAtPowerRatioPeaks);
                            logger.debug("------------------new FOUND2 capacity: " + estimatedLoadAtPowerRatioPeaks);
                            logger.info("------------------final capacity found, waiting for stable region");
                        }

                        //when we find the peak the first time, the progression method will be multiply, and capacity wont be found
                        //change the progression method to increment to get finer samples in the region of capacity
                        if(cMethod != cProgressionMethod.INCREMENT && !capacityFound) {
                            cMethod = cProgressionMethod.INCREMENT;
                            cdelta = estimatedLoadAtPowerRatioPeaks * cdeltaRelative; //2% of estimated capacity
                            setCurrentCapacity(estimatedLoadAtPowerRatioPeaks * relativeCapacityForLocalSearch); //80% of estimated capacity

                            //since we have scheduled much more requests than the actual capacity, assume the overload
                            overload = true;
                            stableEpochCount = 0;
                            logger.info("------------------first estimate found, assuming overload. waiting it to finish.");

                            //remove the last point added, as it was actually an overloaded point
                            //and we needed it to just get the first version of curve

                            PowerRatioData.observations.remove(PowerRatioData.observations.size()-1);
                            PowerRatioData.observations.remove(PowerRatioData.observations.size()-1);
                        }
                    } else {
                        //its a local minima, where power ratio is minimum.

                        //this is basically overfitting of curve, and we should continue observing the system,
                        //till we find a maxima.

                        logger.debug("curvefitting: found a minima...");
                    }
                }
            }

            printPointInFileForPlotting(currentPowerRatio);
            printDebug(currentPowerRatio);

            if(!capacityFound) {
                //change the capacity to next step/jump only if we have enough load to sustain
                //if the load in last epoch was too low, then stay put at the current estimate
                if (!overload && !isLower(currentPowerRatio.getThroughput(), currentCapacity, tightnessCushion)) { //open
//              if (!overload) { //closed
                    double newCapacity = 0;
                    if (cMethod == cProgressionMethod.MULTIPLY) {
                        newCapacity = currentCapacity * cjump;
                    } else {
                        newCapacity = currentCapacity + cdelta;
                    }
                    setCurrentCapacity(newCapacity);
                } else if(unstableEpochCount >= tolerableUnstableEpochs && estimatedLoadAtPowerRatioPeaks == -1) {
                    unstableEpochCount = 0;
                    logger.debug("overload is too long, and first estimate of capacity is not found yet. backing off and reducing capacity.");
                    double newCapacity = 0;
                    cMethod = cProgressionMethod.INCREMENT;
                    newCapacity = currentCapacity * relativeCapacityForLocalSearch;
                    cdelta = newCapacity * cdeltaRelative;
                    setCurrentCapacity(newCapacity);
                }
            } else {
                printEquationInFileForPlotting();
            }
        }

        if (!activityEnded) {
            scheduleNextEpoch();
        }*/
    }

    /**
     * Checks if newValue deviates from baseValue only with tolerance%. If yes, returns true.
     * @param newValue New observation
     * @param baseValue Base observation w.r.t. which the actual deviation will be computed
     * @param tolerance percentage deviation tolerable from baseValue
     * @return true if newValue is near-enough to baseValue
     */
   /* private boolean isNear(double newValue, double baseValue, double tolerance) {
        boolean isNear = Math.abs((newValue - baseValue)/baseValue) <= tolerance;
        if(!isNear) {
            logger.debug("NewValue:" + newValue + " baseValue:" + baseValue + " tol:" + tolerance + " Ratio:" + Math.abs((newValue - baseValue)/baseValue));
        }
        return isNear;
    }*/

   /* private boolean isLower(double newValue, double baseValue, double tolerance) {
        if(newValue > baseValue) {
            return false;
        }
        return !isNear(newValue, baseValue, tolerance);
    }*/

    private void printDebug(PowerRatio currentPowerRatio) {
        PrintWriter currentPw = null;
        if(!overload) {
            currentPw = pwGood;
        } else {
            currentPw = pwOverload;
        }
        currentPw.printf("%.2f\t%.2f\t%.2f\t%.4f\t%.2f    \t%s\t%s\t%s\t%d\t%d\n",
                currentCapacity,
                currentPowerRatio.getThroughput(0),
                currentPowerRatio.getThroughput(1),
                currentPowerRatio.getAverageResponseTime(),
                currentPowerRatio.getPowerRatio(),
                overload, isLower, isNear, currentPowerRatio.minId, System.currentTimeMillis());
        currentPw.flush();

        logger.debug("Respt:" + currentPowerRatio.getAverageResponseTime() + " Arrv1:" +
                currentPowerRatio.getArrivedRequestCount(0) + " Arrv2:" +
                currentPowerRatio.getArrivedRequestCount(1) + " Success1:" +
                currentPowerRatio.getSuccessfulRequestCount(0) + " Success2:" +
                currentPowerRatio.getSuccessfulRequestCount(1) + " Failed1:" +
                currentPowerRatio.getFailedRequestCount(0) + " Failed2:" +
                currentPowerRatio.getFailedRequestCount(1) + " Tput1:" +
                currentPowerRatio.getThroughput(0) + " Tput2:" +
                currentPowerRatio.getThroughput(1) + " PowerRatio:" +
                currentPowerRatio.getPowerRatio() + " Cap:" +
                CapacityEstimator.currentCapacity + " " +
                "Lower:" + isLower + " " +
                "Near:" + isNear + " " +
                "Overload:" + overload);
    }

    private void printPointInFileForPlotting(PowerRatio currentPowerRatio) {
    }

    private void scheduleNextEpoch() {
    }

    private void printEquationInFileForPlotting() {
    }

    private void printDistanceInFileForPlotting() {
    }

    private void cleanupCapacityChange() {
    }

    private void initCapacityChangeModule() {
    }

    private void scheduleCapacityChangeEpoch() {
    }

    /**
     * Finalizes the capacity discovery. Removes the scheduled epoch event.
     *
     * Called upon WEBQ_CAPACITY_DISCOVERY_ENDS event.
     */
    /*public void handleCapacityDiscoveryEnds() {
        CapacityManager.scheduleCapacityChangeDetection();
        //TODO: give necessary information to capacity change detector
    }*/

    public void handleEndofTrainingPeriod(){
        //Call findParams.java to estimate capacity and hardness
        findParams fp = new findParams();
        fp.findCapacity();
    }
}
