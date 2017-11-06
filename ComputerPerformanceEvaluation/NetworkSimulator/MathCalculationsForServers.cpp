#include <iostream>
#include <iomanip>
using namespace std;


static char *serviceType, *arrivalType;
static double serviceMean, arrivalMean;
static double utilization, serviceVariation, arrivalVariation, responseTime, jobs;



/*============================================
*findVariation 
*finds variation for types present
============================================*/
static double findVariation( char *dataType )
{
	if (dataType == "constant") return 0;
	else if (dataType == "uniform")
	{
		return 1.0/12.0 * 1	;
	}

	else if (dataType == "exponential")
	{
		double lambda = 2;
		return 1 / ( lambda * lambda );
	}

	return 0;
}



/*============================================
* findUtilization
* finds the utilization
============================================*/
static double findUtilization( double avgServiceTime, double avgInterarrivalTime)
{
	utilization = avgServiceTime/ avgInterarrivalTime;
	return utilization;
}

/*============================================
* findResponseTime
* finds the response time using G/G/1
============================================*/
static double findResponseTime( double avgService, double variationService, double utilization, double variationArrival)
{
	static double outside = avgService / (1.0 - utilization);
	static double numerator = (variationService * variationService + 1.0) * (variationArrival * variationArrival - 1.0);
	static double denominator = (utilization * utilization * variationService * variationService + 1.0);
	static double inside = 1.0 - utilization * (1.0 - variationService * variationService - numerator / denominator) / 2.0;
	responseTime = inside * outside;
	return responseTime;
}

/*=========================================
*findJobs
=========================================*/
static double findJobs( double utilization )
{
	jobs = utilization / (1.0 - utilization);
	return jobs;
}

/*============================================
*findCalculations
*finds all items 
===========================================*/
static void findCalculations(char *inArrivalType, double inArrivalMean, char *inServiceType, double inServiceMean )
{
	serviceType = inServiceType;
	serviceMean = inServiceMean;
	arrivalType = inArrivalType;
	arrivalMean = inArrivalMean;
	
	serviceVariation = findVariation( serviceType );
	arrivalVariation = findVariation( arrivalType );
	findUtilization(serviceMean, arrivalMean);
	findResponseTime(serviceMean, serviceVariation, utilization, arrivalVariation);
	findJobs(utilization);
}


/*===========================================
*printResults
*prints results
=========================================*/
static void printResults()
{
	cout << setw(20) << setprecision(5) << setfill(' ')
		<< "\Results: \tAverage \t Deviation\n"
		<< "Interarrival:\t| " << arrivalMean << "\t|" << arrivalVariation << "\n"
		<< "Response:\t| " << responseTime << "\t|" << "" << "\n"
		<< "Service:\t| " << serviceMean << "\t|" << serviceVariation << "\n"
		<< "Queue:\t\t| " << jobs << "\t|" << "" << "\n"
		<< "Utilization:\t| " << utilization << "\t|" << "" << "\n\n\n";
}