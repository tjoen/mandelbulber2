/*
 * lights.cpp
 *
 *  Created on: May 18, 2014
 *      Author: krzysztof
 */


#include "lights.hpp"
#include <stdlib.h>
#include "calculate_distance.hpp"
#include "system.hpp"
#include "common_math.h"

cLights::cLights()
{
	lights = NULL;
	numberOfLights = 0;
	lightsReady = false;
}

cLights::cLights(const parameters::container *_params, const parameters::container *_fractal)
{
	Set(_params, _fractal);
}

cLights::~cLights()
{
	if(lights)
	{
		delete[] lights;
		lights = NULL;
	}
}

void cLights::Set(const parameters::container *_params, const parameters::container *_fractal)
{
	WriteLog("Preparation of lights started");
	//move parameters from containers to structures
	const cParamRender *params = new cParamRender(_params);
	const cFourFractals *fourFractals = new cFourFractals(_fractal, _params);

	srand(params->auxLightRandomSeed);

	numberOfLights = params->auxLightNumber + 4;

	if(lights) delete[] lights;
	lights = new sLight[numberOfLights];

	sDistanceOut distanceOut;

	int trial_number = 0;
	double radius_multiplier = 1.0;

	if (params->auxLightNumber > 4)
	{
		for (int i = 0; i < params->auxLightNumber; i++)
		{
			trial_number++;

			CVector3 random;
			random.x = (Random(2000000) / 1000000.0 - 1.0) + (Random(1000000) / 1.0e12);
			random.y = (Random(2000000) / 1000000.0 - 1.0) + (Random(1000000) / 1.0e12);
			random.z = (Random(2000000) / 1000000.0 - 1.0) + (Random(1000000) / 1.0e12);

			CVector3 position = params->auxLightRandomCenter + random * params->auxLightDistributionRadius * radius_multiplier;

			sDistanceIn distanceIn(position, 0.0, false);
			double distance = CalculateDistance(*params, *fourFractals, distanceIn, &distanceOut);

			if (trial_number > 1000)
			{
				radius_multiplier *= 1.1;
				trial_number = 0;
			}

			if (distance > 0 && distance < params->auxLightMaxDist * radius_multiplier)
			{
				radius_multiplier = 1.0;

				lights[i + 4].position = position;

				sRGB colour(20000 + Random(80000), 20000 + Random(80000), 20000 + Random(80000));
				double maxColour = dMax(colour.R, colour.G, colour.B);
				colour.R = colour.R * 65536.0 / maxColour;
				colour.G = colour.G * 65536.0 / maxColour;
				colour.B = colour.B * 65536.0 / maxColour;
				lights[i + 4].colour = colour;

				lights[i + 4].intensity = distance * distance / params->auxLightMaxDist;
				lights[i + 4].enabled = true;
				printf("Light no. %d: x=%f, y=%f, z=%f, distance=%f\n", i, position.x, position.y, position.z, distance);
			}
			else
			{
				i--;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (params->auxLightPreEnabled[i])
		{
			lights[i].position = params->auxLightPre[i];
			lights[i].intensity = params->auxLightPreIntensity[i];
			lights[i].colour = params->auxLightPreColour[i];
			lights[i].enabled = true;
		}
		else
		{
			lights[i].position = CVector3(0.0,0.0,0.0);
			lights[i].intensity = 0.0;
			lights[i].colour = sRGB(0,0,0);
			lights[i].enabled = false;
		}
	}

	lightsReady = true;

	delete params;
	delete fourFractals;

	WriteLog("Preparation of lights finished");
}

cLights::sLight cLights::GetLight(int index) const
{
	sLight light;
	if(lightsReady)
	{
		if(index < numberOfLights)
		{
			light = lights[index];
		}
		else
		{
			qCritical() << "Tried to get light of wrong index. Index:" << index;
		}
	}
	else
	{
		qCritical() << "Lights not initialized";
	}
	return light;
}

void cLights::Copy(const cLights& _lights)
{
	numberOfLights = _lights.numberOfLights;
	lightsReady = _lights.lightsReady;
	if(lights) delete[] lights;
	lights = new sLight[numberOfLights];

	for(int i=0; i < numberOfLights; i++)
	{
		lights[i] = _lights.lights[i];
	}
}