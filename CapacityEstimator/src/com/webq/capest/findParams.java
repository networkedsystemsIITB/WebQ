package com.webq.capest;

import matlabcontrol.MatlabConnectionException;
import matlabcontrol.MatlabInvocationException;
import matlabcontrol.MatlabProxy;
import matlabcontrol.MatlabProxyFactory;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class findParams {
	public void findCapacity() 
	//public static void main(String[] args) throws MatlabConnectionException, MatlabInvocationException
	{
	    //Create a proxy, which we will use to control MATLAB
	    MatlabProxyFactory factory = new MatlabProxyFactory();
	   
	    try
	    {
	    	MatlabProxy proxy = factory.getProxy();
	    	proxy.eval("find_params");
	    	proxy.disconnect();
	    }
	    catch(Exception e)
	    {
	    	e.printStackTrace();
	    }

	    
	    //Disconnect the proxy from MATLAB
	    
	    
		String capacity="";
		
	    //Reading Capacity.txt
	    try (BufferedReader br = new BufferedReader(new FileReader("capacity.txt")))
		{

			String sCurrentLine;

			while ((sCurrentLine = br.readLine()) != null) {
				System.out.println(sCurrentLine);
				capacity=sCurrentLine;
				
			}

		} catch (IOException e) {
			e.printStackTrace();
		} 
	    
	    TokenGenCommunicator.init();
	    
	    //Convey Capacity
	    TokenGenCommunicator.conveyNewCapacity(capacity);
	   
	    String hardness="";
	    //Reading hardness.txt
	    try (BufferedReader br = new BufferedReader(new FileReader("hardness.txt")))
		{

			String sCurrentLine;

			while ((sCurrentLine = br.readLine()) != null) {
				System.out.println(sCurrentLine);
				hardness=sCurrentLine;
				System.out.println(hardness.length());
			}

		} catch (IOException e) {
			e.printStackTrace();
		} 
	    
	    //Convey hardness
	    TokenGenCommunicator.conveyHardness(hardness);
	}
}
