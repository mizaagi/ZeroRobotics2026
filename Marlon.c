//When the plot was last watered
double waterTimes[6];
//Plant time for the certain plot
double PT[6];
//Holds the plant's grow time
double PlantTime[6];
//Contains the amount of times watered on a plot used to determine whether to plant, water, or harvest
int charge[6];
//The current value of crops in correct id order
double cropValue[6];
// variable to check which plot of the cycle in loop I am checking.
int count;
// How many times I have visited Astronaut
int astro;
// stores the crop id of the bonus crop, set to 0 if no bonus crop
int bonus;
float plotCoordinates[6][2];
int targetPlot; // Plot to perform actions on
int cycle;
bool end;

void init(){
    cycle = 0;
bonus = 0;
    astro = 0;
    count = 0;
waterTimes[0] = 0.0;
    waterTimes[1] = 0.0;
    waterTimes[2] = 0.0;
    waterTimes[3] = 0.0;
    waterTimes[4] = 0.0;
    waterTimes[5] = 0.0;
   
    PT[0] = 0.0;
    PT[1] = 0.0;
    PT[2] = 0.0;
    PT[3] = 0.0;
    PT[4] = 0.0;
    PT[5] = 0.0;
   
    PlantTime[0] = 6.0;
    PlantTime[1] = 8.0;
    PlantTime[2] = 5.0;
    PlantTime[3] = 12.0;
    PlantTime[4] = 3.0;
    PlantTime[5] = 9.0;
   
    charge[0] = 0;
    charge[1] = 0;
    charge[2] = 0;
    charge[3] = 0;
    charge[4] = 0;
    charge[5] = 0;

    plotCoordinates[0][0] = -0.35;
    plotCoordinates[0][1] = 0.60;
    plotCoordinates[1][0] = 0.35;
    plotCoordinates[1][1] = 0.60;
    plotCoordinates[2][0] = -0.15;
    plotCoordinates[2][1] = 1.00;
    plotCoordinates[3][0] = -0.35;
    plotCoordinates[3][1] = 1.40;
    plotCoordinates[4][0] = 0.35;
    plotCoordinates[4][1] = 1.40;
    plotCoordinates[5][0] = 0.15;
    plotCoordinates[5][1] = 1.00;
   
cropValue[0] = 6.0;
cropValue[1] = 7.0;
cropValue[2] = 5.0;
cropValue[3] = 11.0;
cropValue[4] = 3.0;
cropValue[5] = 8.0;

targetPlot = 0;
end = false;
}
void wCrop(int plotNum, int cropNum) {
    game.MovePlot(plotNum);
    if (game.GetTime() > 240) return;
    game.PlantCrop(plotNum, cropNum);
    waterTimes[plotNum-1] = game.GetTime();
    charge[plotNum-1] = 1;
    PT[plotNum-1] = cropNum == 1 ? 6.0 : cropNum == 2 ? 8.0 : cropNum == 3 ? 5.0 : cropNum == 4 ? 12.0 : cropNum == 5 ? 3.0 : 9.0;
    game.WaterCrop(plotNum);
    cropValue[cropNum-1] = cropValue[cropNum-1] * 0.75;
    if (cropNum == bonus) {
        bonus = 0;
    }
}
float selfAbs(float x) {
    if (x < 0) x *= -1;
    return x;
}
int bestCrop() {
    float bestValRatio = 0;
    int bestCrop = 1;
    // for (int j = 0; j < 6; j++) {
    //     float valRatio = (cropValue[j]) / (PlantTime[j]);
    //     if (valRatio > bestValRatio) {
    //         bestValRatio = valRatio;
    //         bestCrop = j+1;
    //     }
    // }
    for (int i = 0; i < 6; i++) {
        if (240-game.GetTime() < 2.5*PlantTime[i]) cropValue[i] = 0;
        if (cropValue[i] > cropValue[bestCrop-1]) bestCrop = i+1;
    }
    if (bonus != 0) bestCrop = bonus;
    return bestCrop;
}
float distanceVal(int plot) {
    return selfAbs(game.GetRobotPositionX()-plotCoordinates[plot-1][0]) + selfAbs(game.GetRobotPositionY()-plotCoordinates[plot-1][1]);
}
float timeVal(int plot, float pTime) {
    return waterTimes[plot-1] + PT[plot-1] - pTime;
}
void forceWater() {
    while (game.GetWaterUnits() == 0) {
        game.MoveWatering();
        game.FillWateringCan();
        if (game.OpponentAtWatering()) {
            game.MoveTo(0, 0, 0);        
           
        }
    }
}
void forceAstro() {
    while (bonus == 0) {
        game.MoveAstronaut();
        bonus = game.GetBonusCrop();
        if (game.OpponentAtAstronaut() && bonus == 0) {
            game.MoveTo(0, 1.5, 0);        
           
        }
    }
}
int realBestPlot() {
    float pTime            = game.GetTime();
    float bestPlotCalc = 50000;
    int bestPlot = 0;
    float bestPlotDistance = 50000;
    // int   bestPlotByDist   = 0;
    float bestPlotTime     = 50000;
    // int   bestPlotByTime   = 0;
    for (int i = 0; i < 6; i++) {
        float plotDistance = distanceVal(i+1);
        float plotTime     = timeVal(i+1, pTime);
        // if (plotDistance < bestPlotDistance) {
        //     bestPlotByDist = i+1;
        //     bestPlotDistance = plotDistance;
        // }
        // if (plotTime < bestPlotTime) {
        //     bestPlotByTime = i+1;
        //     bestPlotTime = plotTime;
        // }
        DEBUG(("Plot: %d", i+1));
        DEBUG(("Distance: %.2f", plotDistance));
        DEBUG(("Time: %.2f", plotTime));
        DEBUG(("Charge: %d", charge[i]));
        if (!end) {
            float evaluation =  plotTime*0.10 - (charge[i] == 1 ? 5 : 1);
            if (bestPlotCalc > evaluation && plotDistance != 0 && plotTime < 0) {
                bestPlotCalc = evaluation;
                bestPlot = i+1;
            }
        } else {
            float evaluation =  plotTime*0.10 - (charge[i] == 2 ? 5 : 1);
            if (bestPlotCalc > evaluation && plotDistance != 0 && charge[i] != 0 && plotTime < 0) {
                bestPlotCalc = evaluation;
                bestPlot = i+1;
            }
        }
    }
    return bestPlot;
    // if (bestPlotByTime == bestPlotByDist) return bestPlotByTime;
    // else if (bestPlotTime * distanceVal(bestPlotByTime) > bestPlotDistance * timeVal(bestPlotByDist, pTime)) {
    //     return bestPlotByDist;
    // } return bestPlotByTime;
}
void loop(){
    if (cycle == 0) {
        wCrop(1, bestCrop());
        wCrop(3, bestCrop());
        wCrop(4, bestCrop());
        forceAstro();
        wCrop(5, bestCrop());
        wCrop(6, bestCrop());
        wCrop(2, bestCrop());
        cycle = 1;
    } if (cycle == 1) {
        forceWater();
        targetPlot = realBestPlot();
        game.MovePlot(targetPlot);
        // while (timeVal(targetPlot, game.GetTime()) > 0 && game.GetTime() < 239) {
        //     game.MoveTo(game.GetRobotPositionX() + 0.5, game.GetRobotPositionY() + 0.5, game.GetRobotPositionZ() + 0.5);
        //     game.MoveTo(game.GetRobotPositionX() - 0.5, game.GetRobotPositionY() - 0.5, game.GetRobotPositionZ() - 0.5);
        // }
        if (charge[targetPlot-1] == 1) {
            game.WaterCrop(targetPlot);
            waterTimes[targetPlot-1] = game.GetTime();
            charge[targetPlot-1] = 2;
        } else if (charge[targetPlot-1] == 2) {
            int astronumprior = game.GetCurrentBonusIndex();
            int harvestScorePrior = game.GetHarvestScore();
            game.HarvestCrop(targetPlot);
            int astronumafter = game.GetCurrentBonusIndex();
            int harvestScoreAfter = game.GetHarvestScore();
            if (harvestScorePrior != harvestScoreAfter) {
                if (astronumafter > astronumprior && astronumafter != 3) {
                    // astro = astronumafter;
                    forceAstro();
                    // int placeholder = game.GetCurrentBonusIndex();
                    // DEBUG(("place %d", placeholder));
                }
                charge[targetPlot-1] = 0;
                // helper(plotNum);
                if (!end) wCrop(targetPlot, bestCrop());
            }
            // game.HarvestCrop(targetPlot);
           
        }
        if (game.GetTime() > 210) end = true;
    }
    if (game.GetTime() < 240) loop();
    else {
        for (int i = 0; i < 6; i++) DEBUG(("Charge: %d", charge[i]));
        game.EndGame();
    }
}
