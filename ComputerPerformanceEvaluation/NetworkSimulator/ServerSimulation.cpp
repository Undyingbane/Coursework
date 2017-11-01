#include <iostream>
#include <queue> //queue
#include <iomanip> //setw
#include "RandomNumbers.cpp" //number generation

using namespace std;

//Number of items to process
static const int TOTAL_NUMBER_SERVICES = 100;

class SimulateServer
{
private:
	char *arrivalType, *serviceType;
	double arrivalMean, serviceMean;
	double totalTime = 0;

	//Storing results to find std dev
	double serviceTimeArr[TOTAL_NUMBER_SERVICES];
	double interArrivalTimeArr[TOTAL_NUMBER_SERVICES * 2];
	double responseTimeArr[TOTAL_NUMBER_SERVICES * 2];
	double queueArr[TOTAL_NUMBER_SERVICES];
	double serverUtilizationArr[TOTAL_NUMBER_SERVICES];

	//Std dev
	double devService, devInterarrival, devResponse, devQueue, devUtilization;

	
	
	//values needed for results

	//TOTALS
	double totalInterarrivalTime, totalServiceTime, totalResponseTime = 0, serverUtilizationFactor = 0, queueFactor = 0;

	//AVERAGES
	double avgInterarrivalTime, avgServiceTime, avgResponseTime, avgUtilization, avgQueue;

	//RESULTS
	double timeToNextArrival, currentServiceRemaining;
	double nextArrivalTime;

	//Pointers to functions
	double (*arrivalGeneration)(double);
	double(*serviceGeneration)(double);

	//Counter for trials
	int serviceCounter = 0;
	int arrivalCounter = 0;

	//Current Queue
	queue <double> simulatorQueue;


	void resetData()
	{
		 totalTime = 0;
		 totalInterarrivalTime = 0;
		 totalServiceTime = 0;
		 totalResponseTime = 0;
		 serverUtilizationFactor = 0;
		 queueFactor = 0;
	}

	void waitForArrival()
	{
		totalTime += timeToNextArrival;				//TIME
		arrive();
	}


	void getNextArrival()  //GENERATE NEXT ARRIVAL DATA
	{
		timeToNextArrival = arrivalGeneration(arrivalMean); 
		nextArrivalTime = totalTime + timeToNextArrival;
		totalInterarrivalTime += timeToNextArrival;			//TOTAL Interarrival time for all arrival times generated

		interArrivalTimeArr[arrivalCounter ] = timeToNextArrival;
	}

	void getNextService() //GENERATE NEXT SERVICE
	{
		serviceCounter++;	
		currentServiceRemaining = serviceGeneration(serviceMean); //GENERATE NEXT SERVICE TIME
		totalServiceTime += currentServiceRemaining; 

		serviceTimeArr[serviceCounter -1] = currentServiceRemaining;
	}

	void arrive()	//PUSH, GENERATE NEXT ARRIVAL
	{
		queueArr[serviceCounter - 1] = simulatorQueue.size();
		simulatorQueue.push(nextArrivalTime );
		arrivalCounter++;
		getNextArrival();
	}

	void arriveWhileServicing()
	{
		currentServiceRemaining = currentServiceRemaining - timeToNextArrival; //decrement service time
		totalTime += timeToNextArrival;		//THIS MUCH TIME PASSED		
		queueFactor += (double)( simulatorQueue.size() ) * timeToNextArrival; //Every time passes need to do this
		arrive();
	}

	void completeService()
	{
		totalResponseTime += totalTime - simulatorQueue.front(); //Response time is found as dif between arrival and working on
		responseTimeArr[serviceCounter - 1] = totalTime - simulatorQueue.front();
		simulatorQueue.pop();

		timeToNextArrival = timeToNextArrival - currentServiceRemaining;  //find time to next arrival
		totalTime += currentServiceRemaining; //THIS MUCH TIME PASSED
		queueFactor += (double)( simulatorQueue.size() ) * currentServiceRemaining; //every time passes need to do this
	}

	void service()
	{
		{
			//IF NEXT JOB ARRIVES BEFORE CURRENT SERVICE FINISHES
			if (timeToNextArrival < currentServiceRemaining)  { arriveWhileServicing(); }
			//WORK ON NEXT ITEM
			else 
			{ 
				completeService(); 
				getNextService();
			}
		}
	}

	void generateAverages()
	{
		avgInterarrivalTime = totalInterarrivalTime / arrivalCounter;
		avgServiceTime = totalServiceTime / serviceCounter;
		avgResponseTime = totalResponseTime / totalTime;
		avgUtilization =  avgServiceTime / avgInterarrivalTime;
		avgQueue = queueFactor / totalTime;
	}

	double findDeviation( double *item)
	{
		double tempArr[TOTAL_NUMBER_SERVICES];
		double tempSum = 0;
		double tempMean;
		double sumSquareDifference;
		for (int i = 0; i < TOTAL_NUMBER_SERVICES; i++)
		{
			tempArr[i] = item[i];
			tempSum += tempArr[i];
		}
		tempMean = tempSum / TOTAL_NUMBER_SERVICES;

		for (int i = 0; i < TOTAL_NUMBER_SERVICES; i++)
		{
			sumSquareDifference = (tempArr[i] - tempMean) * (tempArr[i] - tempMean);
		}
		return sumSquareDifference / (TOTAL_NUMBER_SERVICES - 1);
	}

	void calculateDeviations()
	{
		for (int i = 0; i < TOTAL_NUMBER_SERVICES; i++)
		{
			serverUtilizationArr[i] = serviceTimeArr[i] / interArrivalTimeArr[i];
		}
		devInterarrival = findDeviation(interArrivalTimeArr);
		devService = findDeviation(serviceTimeArr);
		devResponse = findDeviation(responseTimeArr);
		devQueue = findDeviation(queueArr);
		devUtilization = findDeviation(serverUtilizationArr);
	}
	
public:
	SimulateServer(char *arrivalType, double arrivalMean, char *serviceType, double serviceMean)
	{
		this->arrivalType = arrivalType;
		this->arrivalMean = arrivalMean;
		this->serviceType = serviceType;
		this->serviceMean = serviceMean;
	}

	void simulate()
	{
		//Generate functions to be used for indicated server
		if (arrivalType == "uniform") arrivalGeneration = generateUniform;
		else if (arrivalType == "exponential") arrivalGeneration = generateExponential;
		else if (arrivalType == "constant") arrivalGeneration = generateConstant;

		if (serviceType == "uniform") serviceGeneration = generateUniform;
		else if (serviceType == "exponential") serviceGeneration = generateExponential;
		else if (serviceType == "constant") serviceGeneration = generateConstant;


		//Generate initial values
		resetData();
		getNextArrival();
		getNextService();
		

		while (serviceCounter < TOTAL_NUMBER_SERVICES)
		{
			if (simulatorQueue.empty())  waitForArrival();
			service();
		}
		
		generateAverages();
		calculateDeviations();
		printResults();
	}

	void printResults()
	{
		cout << setw(20) << setprecision(5) << setfill(' ')
			<< "\Results: \tAverage \t Deviation\n"
			<< "Interarrival:\t| " << avgInterarrivalTime << "\t|" << devInterarrival << "\n"
			<< "Response:\t| " << avgResponseTime << "\t|" << devResponse << "\n"
			<< "Service:\t| " << avgServiceTime << "\t|" << devService << "\n"
			<< "Queue:\t\t| " << avgQueue << "\t|" << devService << "\n"
			<< "Utilization:\t| " << avgUtilization << "\t|" << devUtilization << "\n\n\n";
	}
};