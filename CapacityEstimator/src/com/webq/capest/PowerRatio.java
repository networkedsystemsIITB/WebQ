package com.webq.capest;

import static com.webq.capest.PropertiesFile.getDouble;

import org.apache.log4j.Logger;

/**
 * Handles the data samples required for power ratio calculation. 
 */
class PowerRatio implements Cloneable {
	private double totalResponseTime = 0;
	private int totalSuccessfulRequests = 0;
	private int totalArrivedRequests = 0;
	private int totalFailedRequests = 0;
	
	private static double responseTimeHardCut = getDouble("webq.logic.responsetime.hardcut");
	
	final private static double epoch = CapacityManager.epoch;
	public static Logger logger = Logger.getLogger("PowerRatio");
	
	public int minId = -1;
	private long totalResponseTimeCount = 0;
	private long totalResponseTimeDiscarded = 0;
	
	public double getTotalResponseTime() {
		return totalResponseTime;
	}
	
	public double getAverageResponseTime() {
		return totalResponseTime / totalResponseTimeCount;
	}
	
	public int getSuccessfulRequestCount() {
		return totalSuccessfulRequests;
	}
	
	public int getArrivedRequestCount() {
		return totalArrivedRequests;
	}
	
	public int getFailedRequestCount() {
		return totalFailedRequests;
	}

	public void recordCompletion(int id, double responseTimes, long countOfRequests) {
//		logger.debug("Completion recorded: " + responseTimes);
		if(minId < id) {
			minId = id;
		}
		totalSuccessfulRequests += countOfRequests;
		if(responseTimes < responseTimeHardCut) {
			totalResponseTime += responseTimes;
			totalResponseTimeCount += countOfRequests;
		} else {
			totalResponseTimeDiscarded  += countOfRequests;
		}
	}
	
	public void recordArrival(int id) {
//		logger.debug("Arrival recorded");
		if(minId < id) {
			minId = id;
		}
		totalArrivedRequests++;
	}
	
	public void recordFailure(int id) {
		if(minId < id) {
			minId = id;
		}
		logger.debug("Failure recorded");
		totalFailedRequests++;
	}
	
	
	
	public double getPowerRatio() {
		//power ratio is goodput / avg response time
		return (totalSuccessfulRequests == 0 || totalResponseTimeCount == 0) 
				? -1 
				//goodput / respt
				: (totalSuccessfulRequests / epoch) / (totalResponseTime / totalResponseTimeCount);
	}

	public double getThroughput() {
		return totalSuccessfulRequests*1.0 / epoch;
	}
	
	public long getResponseTimeCount() {
		return totalResponseTimeCount;
	}
	
	public double getDiscardedResponseTimePercent() {
		if(totalSuccessfulRequests > 0) return totalResponseTimeDiscarded/totalSuccessfulRequests * 100.0;
		else return 0;
	}
	
	protected PowerRatio clone() throws CloneNotSupportedException {
		return (PowerRatio)(super.clone());
	}
}